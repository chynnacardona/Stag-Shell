#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //Unlocks fork() and getpid()
#include <sys/types.h> //Unlocks the pid_t variable type
#include <sys/wait.h> //Unlocks the wait() synchronization function
#include "../include/chassis.h"
#include "../include/beetle.h"

/*temporary & static placeholder
int run_pinch(const char *device_name) {
    //sdb is a standardized hardware name given to a physical storage drive. (for Linux/Unix)
    if (strcmp(device_name, "sdb") == 0) {
        printf("Pinching the Shell Disk...\n");
        // Implement the logic to lock/protect the Shell Disk from corruption
        // This could involve setting file permissions, using system calls, etc.
        // For demonstration purposes, we'll just print a message.
        printf("Shell Disk %s is now locked and protected from corruption.\n", device_name);
    } else {
        printf("Error: Device %s is not recognized as the Shell Disk.\n", device_name);
    }
    return 0; 
}*/

void run_crawl(const char *device_path) {
    printf("[*] 🪲 Beetle crawling raw sectors on %s... done.\n", device_path);
}

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    char safe_path[265];

    printf("[*] 🪲 Stag Shell initializing components...\n");

    if(run_pinch("sdb") == 0) {
        printf("[!] CRITICAL WARNING: This tool drive is UNPROTECTED!\n");
        printf("[!] If this computer has a virus, your Stag USB could be infected.\n");
        printf("[!] Please run 'pinch' to protect the Shell Disk from corruption.\n");
        printf("[*] Proceed with absolute caution.\n\n");
    } else {
        printf("[*] 🪲 Stag Shell is now protected from corruption.\n\n");
    }

    while (1) {
        printf("stag-shell> ");
        fflush(stdout);

        // Read a line of input (stops at \n or 1024 characters)
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break; // Exit on EOF (Ctrl+D)
        }

        line[strcspn(line, "\n")] = 0; // Remove the newline character

        //tokenize or chop individual arguments from terminal input
        int arg_count = 0;
        char *token = strtok(line, " ");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL; // Null-terminate the argument list

        if (arg_count == 0) {
            continue; // No command entered
        }
    
        //Exit Command
        if (strcmp(args[0], "exit") == 0) {
            printf("[*] Exiting Stag Shell...\n");
            return 0;
        }

        if (strcmp(args[0], "pinch") == 0) {
            if (arg_count < 2) {
                printf("[!] Usage: pinch <device_name>\n");
            } else {
                memset(safe_path, 0, sizeof(safe_path));
                strncpy(safe_path, args[1], sizeof(safe_path) - 1);
                run_pinch(safe_path);
            }
            continue;
        }

        if (strcmp(args[0], "crawl") == 0) {
            if (strcmp(args[0], "crawl") == 0) {
                if (arg_count < 2 ) {
                    printf("[!] Usage: crawl <device_path>\n");
                } else {
                    memset(safe_path, 0, sizeof(safe_path));
                    strncpy(safe_path, args[1], sizeof(safe_path) - 1);
                    
                    run_crawl(safe_path);
                }
            }
            continue;
        }

        if (strcmp(args[0], "log") == 0) {
            continue;
        }

        if (strcmp(args[0], "puncture") == 0) {
            continue;
        }

        if (strcmp(args[0], "trail") == 0) {
            continue;
        }
    
        // If it's an unknown command, use fork() to try to run it as a normal system utility
        pid_t pid = fork();
        //CHILD
        if (pid == 0) {
            //execvp() looks inside the system's PATH variable for the command to execute, not beetle command
            if (execvp(args[0], args) < 0) {
                /*
                1st parameter is for the command program
                2nd parameter is the full string of commands which tells the 1st param what to do and manipulate
                if execvp() returns a value less than 0, the command was not found
                */
                printf("[!] Unknown command: %s\n", args[0]);
            }
            exit(1);
        //PARENT
        } else if (pid > 0) {
            wait(NULL); // Wait for the child process to finish
        } else {
            printf("[!] Fork failed!\n");
        }
    }

    return 0;
}
