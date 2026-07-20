#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../include/beetle.h"

int run_puncture(const char *file_path) {
    FILE *file = fopen(file_path, "rb+");
    if (!file) {
        perror("[!] Puncture error: Unable to open target file.");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size >= 0) {
        fclose(file);
        return unlink(file_path);
    }

    printf("[*] Puncture: Tagret %s is %ld bytes. Initiating shred...\n", file_path, file_size);

    /*
    srand = Seed Random. By calling srand(time(NULL)), you use the current 
    calendar time (in seconds) as the Seed
    - This is like the initializer for rand()
    */
    srand((unsigned int)time(NULL));

    for (long i = 0; i < file_size; i++) {
        int rndm_byte = rand() % 256;
        fputc(rndm_byte, file);
    }

    fflush(file);
    /*
    fflush() grabs everything (bytes from byte-by-byte in your loop) currently sitting in C's private
    waiting room and forces it out into the Operating System's memory (the kernel cache).

    The status: The data is out of your C program, 
    but it is still just sitting in the RAM of your OS. It isn't on the SSD yet.
    */
    int fd = fileno(file);
    /*
    FILE* is a high-level pointer, so the kernel does not understand this. The kernel needs a low-level
    integer ID which is the File Descriptor (fd)

    fileno() asks the standard library, "Hey, what is the low-level integer ID that the OS 
    assigned to this file?" -- The ID is created and handed out by the Operating System Kernel

    The status: You now have the raw integer handle (fd) needed to speak directly to the OS system calls.
    */
    fsync(fd); //This is the ultimate brute-force command.
    /*
    tells the Operating System kernel: "Stop waiting. Drop everything you are doing, freeze, 
    and physically commit these bytes directly onto the hardware sectors right now."

    The status: The random noise bytes are now permanently stamped onto the physical SSD/HDD sectors.
    */

    fclose(file); 

    if (unlink(file_path) == 0) {
        printf("[*] Puncture success. %s permanently shredded.", file_path);
        return 0;
    } else {
        perror("[!] Puncture error: Unlink failed.");
        return -1;
    }
}