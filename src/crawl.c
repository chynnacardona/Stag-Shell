#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //Unlocks fork() and getpid()
#include <sys/stat.h> 
#include <dirent.h> //Allows to open and read folder components
#include "../include/beetle.h"

typedef struct {
    const char *extension;
    const unsigned char *magic_bytes;
    size_t length;
    const char *description;
} FileSignature;

void run_crawl(const char *device_path) {
    printf("[*] 🪲 Beetle crawling on %s\n", device_path);

    FileSignature signatures[] = {
        {".png", (unsigned char[]){0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}, 8, "PNG Image"},
        {".jpg", (unsigned char[]){0xFF, 0xD8, 0xFF}, 3, "JPEG Image"},
         {".gif", (unsigned char[]){0x47, 0x49, 0x46, 0x38}, 4, "GIF Image"},

        {".pdf", (unsigned char[]){0x25, 0x50, 0x44, 0x46}, 4, "PDF Document"},
        {".zip", (unsigned char[]){0x50, 0x4B, 0x03, 0x04}, 4, "ZIP Archive"},

        {".mp3", (unsigned char[]){0x49, 0x44, 0x33}, 3, "MP3 Audio"},
        {".wav", (unsigned char[]){0x52, 0x49, 0x46, 0x46}, 4, "WAV Audio"},
        {".avi", (unsigned char[]){0x52, 0x49, 0x46, 0x46}, 4, "AVI Video"},
        {".mp4", (unsigned char[]){0x00, 0x00, 0x00, 0x18}, 4, "MP4 Video"},

        {".exe",  (unsigned char[]){0x4D, 0x5A}, 2, "Windows Executable / DOS MZ"},
        {".elf",  (unsigned char[]){0x7F, 0x45, 0x4C, 0x46}, 4, "Linux ELF Executable"},
        {".dmg",  (unsigned char[]){0x6B, 0x6F, 0x6C, 0x79}, 4, "macOS Disk Image Trailer"}
    };
    size_t num_signatures = sizeof(signatures) / sizeof(signatures[0]);

       mkdir("recovered_files", 0755); // Create a directory to store recovered files

// ─── MODE 1: TARGET IS A RAW HARDWARE DISK NODE ─────────────────────────
    if (strncmp(device_path, "/dev/", 5) == 0) {

        FILE *disk = fopen(device_path, "rb");
        if (disk == NULL) {
            perror("[!] Error opening target device.");
            printf("[!] Please ensure you have the correct path to access the device.\n");
            return;
        }

        unsigned char sector[512]; // Buffer to hold the sector data
        unsigned long sectors_scraped = 0;
        unsigned long files_recovered = 0;

        while (fread(sector, 1, sizeof(sector), disk) == sizeof(sector)) {
            sectors_scraped++;

            for (size_t i = 0; i < num_signatures; i++) {
                if (memcmp(sector, signatures[i].magic_bytes, signatures[i].length) == 0) {
                    printf("[*] Found %s at sector %lu\n", signatures[i].description, sectors_scraped);
                    files_recovered++;

                    char out_filename[256]; //unique name for recovered file
                    snprintf(out_filename, sizeof(out_filename), "recovered_files/recovered_%lu%s", 
                        files_recovered, signatures[i].extension);

                    printf("[*] Extraxting %s found at sector %lu -> %s\n", signatures[i].description, sectors_scraped, out_filename);

                    FILE *recovered_file = fopen(out_filename, "wb");
                    if (recovered_file != NULL) {
                        fwrite(sector, 1, sizeof(sector), recovered_file);

                        unsigned char crawl_buffer[512];
                        long cur_position = ftell(disk); 

                        for (int block = 0; block < 2000; block++) {
                            if (fread(crawl_buffer, 1,sizeof(crawl_buffer), disk) == sizeof(crawl_buffer)) {
                                fwrite(crawl_buffer, 1, sizeof(crawl_buffer), recovered_file);
                            } else {
                                break;
                            }
                        }

                        fclose(recovered_file);
                        fseek(disk, cur_position, SEEK_SET); // Return to the original position in the disk
                    }
                    break;
                }
            }
        }

        printf("\n[*] Crawling complete. Total sectors scraped: %lu\n", sectors_scraped);
        printf("[*] Total files recovered: %lu\n", files_recovered);
        fclose(disk);
    }

// ─── MODE 2: TARGET IS A REGULAR DIRECTORY FOLDER ───────────────────────
    else {
        DIR *dir = opendir(device_path);
        if (dir == NULL) {
            perror("[!] Error opening target directory.");
            printf("[!] Please ensure you have the correct path to access the directory.\n");
            return;
        }

        struct dirent *entry;
        unsigned long sectors_scraped = 0;
        unsigned long files_recovered = 0;

        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strncmp(entry->d_name, "..", 2) == 0) {
                continue; // Skip current and parent directory entries
            }

            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", device_path, entry->d_name);

            FILE *file = fopen(full_path, "rb");
            if (file == NULL) {
                continue;
            }

            sectors_scraped++;

            unsigned char header[16];
            size_t bytes_read = fread(header, 1, sizeof(header), file);

            for (size_t i = 0; i < num_signatures; i++) {
                if (bytes_read >= signatures[i].length &&
                memcmp(header, signatures[i].magic_bytes, signatures[i].length) == 0) {

                    files_recovered++;

                    char destination[1200];
                    snprintf(destination, sizeof(destination), "recovered_files/recovered_%lu%s", 
                        files_recovered, signatures[i].extension);

                    printf("[*] Match! File '%s' is verified structurally as a %s\n", entry->d_name, signatures[i].description);
                   
                    FILE *dest_file = fopen(destination, "wb");
                    if (dest_file != NULL) {
                        fseek(file, 0, SEEK_SET);

                        unsigned char crawl_buffer[512];
                        size_t bytes;
                        while ((bytes = fread(crawl_buffer, 1, sizeof(crawl_buffer), file)) > 0) {
                            fwrite(crawl_buffer, 1, bytes, dest_file);
                        }
                        fclose(dest_file);
                    }
                    break;
                }
            }
            fclose(file);
        }
        closedir(dir);
        printf("\n[*] Crawling complete. Total files scanned: %lu\n", sectors_scraped);
        printf("[*] Total files recovered: %lu\n", files_recovered);
    }
}