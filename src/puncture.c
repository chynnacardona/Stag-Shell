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

    srand((unsigned int)time(NULL));

    for (long i = 0; i < file_size; i++) {
        int rndm_byte = rand() % 256;
        fputc(rndm_byte, file);
    }

    fflush(file);
    int fd = fileno(file);
    fsync(fd);

    fclose(file);

    if (unlink(file_path) == 0) {
        printf("[*] Puncture success. %s permanently shredded.", file_path);
        return 0;
    } else {
        perror("[!] Puncture error: Unlink failed.");
        return -1;
    }
}