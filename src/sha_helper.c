#include "../include/sha_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h> 

int filre_sha256(const char *file_path, char *out_hex) {
    FILE *file = fopen(file_path, "rb");
    if (!file) return -1;

    //Initialization structure doe OpenSSL to caclulate SHA256 cryptographic hash
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    //
    const int buffer_size = 32768;
    unsigned char *buffer = malloc(buffer_size);
    if (!buffer) {
        fclose(file);
        return -1;
    }

    size_t bytes_read = 0;
    while ((bytes_read = fread(buffer, 1 , buffer_size, file))) {
        SHA256_Update(&sha256, buffer, bytes_read);
    }
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(out_hex + (i * 2), "%02x", hash[i]);
    }
    out_hex[64] = '\0';

    free(buffer);
    fclose(file);
    return 0;
}