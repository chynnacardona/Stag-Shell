#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/beetle.h"

typedef struct {
    const char *extension;
    const unsigned char *magic_bytes;
    size_t length;
    const char *description;
} FileSignature;

void run_crawl(const char *device_path) {
    printf("[*] 🪲 Beetle crawling raw sectors on %s\n", device_path);

    FILE *disk = fopen(device_path, "rb");
    if (disk == NULL) {
        perror("[!] Error opening target device.");
        printf("[!] Please ensure you have the correct path to access the device.\n");
        return;
    }

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

    unsigned char sector[512]; // Buffer to hold the sector data
    unsigned long sectors_scraped = 0;
    unsigned long files_recovered = 0;

    printf("[*] Scanning storage blocks... press Ctrl+C to halt.\n");

    while (freed(sector, 1, sizeof(sector), disk) == sizeof(sector)) {
        sectors_scraped++;

        for (size_t i = 0; i < num_signatures; i++) {
            if (memcmp(sector, signatures[i].magic_bytes, signatures[i].length) == 0) {
                printf("[*] Found %s at sector %lu\n", signatures[i].description, sectors_scraped);
                files_recovered++;
                break;
            }
        }
    }

    printf("\n[*] Crawling complete. Total sectors scraped: %lu\n", sectors_scraped);
    printf("[*] Total files recovered: %lu\n", files_recovered);
    fclose(disk);
}