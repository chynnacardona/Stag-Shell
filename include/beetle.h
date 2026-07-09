#ifndef BEETLE_H
#define BEETLE_H

void run_log(void);
void run_crawl(const char *device_path);
void run_trail(const char *file_path);
int run_pinch(const char *device_name);
void run_puncture(const char *file_path);

#endif

/*
- log : Network and IP auditing
- crawl : Hardware scraping, recover hard-deleted files
- trail : File tracer, finding source of a file
- pinch : locking/protecting the Shell Disk from corruption
- puncture : hard-delete files
*/