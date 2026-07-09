#include <stdio.h>
#include <string.h>
#include "../include/beetle.h"

#if defined (__linux__)
    #include <sys/mount.h> //sufficient for Linux, automatically pulls everything
#elif defined (__APPLE__) || defined(__FreeBSD__)
    #include <sys/param.h> //gives you the physical mount() function signature, it often relies on system configuration settings, sizes, and limits that are defined inside <sys/param.h>.
    #include <sys/mount.h>
#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h> //master header for everything Windows. It unlocks Windows-specific basic data types, handles, and thread controls.
    #include <fileapi.h>  //where Windows keeps its versions of file operations.
    #include <winioctl.h> //Short for Windows Input/Output Control. This header provides the control codes needed to communicate directly with physical storage device drivers on Windows
#endif

int run_pinch(const char *device_name) {
    printf("[*] Waiting for Host Operating System...\n");

    #if defined (__linux__)
        // Linux-specific code to lock the device
        if (mount(device_name, device_name, NULL, MS_RDONLY | MS_REMOUNT, NULL) == 0) {
            printf("[*] Device %s is now locked and protected from corruption.\n", device_name);
            return 1; // Success
        } else {
            perror("Error locking device");
            return 0; // Failure
        }
    #elif defined (__APPLE__) || defined (__FreeBSD__) 
        // macOS/FreeBSD-specific code to lock the device
        if (mount("hfs", device_name, MNT_RDONLY | MNT_UPDATE, NULL) == 0) {
            if (mount("", device_name, MNT_RDONLY | MNT_UPDATE, NULL) == 0) {
                printf("[*] Device %s is now locked and protected from corruption.\n", device_name);
                return 1; // Success
            } else {
                perror("Error locking device");
                return 0; // Failure
            }
           
        } else {
             printf("[*] Device %s is now locked and protected from corruption.\n", device_name);
            return 1; // Failure
        }
    #elif defined (_WIN32)
        /*
        Windows-specific code to lock the device

        GENERIC_READ | GENERIC_WRITE so this basially means we can interact with the Drive,
        while FSCTL_LOCK_VOLUME is the write blocker

        - We use GENERIC_READ so your shell can inspect the drive properties.
        - We use GENERIC_WRITE not because we want to write files, but because changing 
        a device's status (like locking it down) is considered a "write-privilege configuration command" 
        - If we didn't pass GENERIC_WRITE here, Windows would reject the next command (the write-blocking)
        - Once Windows gives you that communication handle, you immediately deploy your shield. This is where 
        FSCTL_LOCK_VOLUME acts as your software write-blocker.
        */
        HANDLE hDevice = CreateFile(device_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (hDevice == INVALID_HANDLE_VALUE) {
            printf("Error opening device %s: %lu\n", device_name, GetLastError());
            return 0; // Failure
        }

        DWORD bytesReturned;
        if (DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
            printf("[*] Device %s is now locked and protected from corruption.\n", device_name);
            return 1; // Success
        } else {
            printf("Error locking device %s: %lu\n", device_name, GetLastError());
            CloseHandle(hDevice);
            return 0;
        }
    #else
        printf("Unsupported operating system for device locking.\n");
        return 0; 
    #endif     

    return 0; 
    }