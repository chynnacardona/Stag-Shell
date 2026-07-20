#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "../include/beetle.h"
#include "../include/sha_helper.h"

#define MAX_ACTIVE_HASHES 1000

// acts like a database for hashed
typedef struct {
    char source_identifier[256]; // Stores filename (Mode 1) or Sector Map ID (Mode 2)
    char hash[65];
} ActiveInventoryFile;

ActiveInventoryFile active_db[MAX_ACTIVE_HASHES];
int active_count = 0;

typedef struct {
    const char *extension;
    const unsigned char *magic_bytes;
    size_t length;
} LocalSignature;

//--- Mode 1: Disk Based ---
void disk_map(const char *disk_path) {

}
//--- Mode 2: Directory Based ---
void directory_map(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("[!] Error opening target validation directory");
        return;
    }

    
;}

void run_trail(const char *file_path, const char *active_file) {
   
}
