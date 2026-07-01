#ifndef CHASSIS_H //if chassis does not exist, define it
#define CHASSIS_H

#define MAX_LINE 1024 //1024 bytes per line 
#define MAX_ARGS 64 //64 arguments or commands per line 

#endif

/*
chassis does both the structural concept and physical memory limit.
- For structural concept: it defines the maximum number of arguments and the maximum length of a line.
so it holds the limits of what we can put inside the terminal.
- For physical memory limits: because of that structure or rule, there is a memory limit.

This defines the limitation inside main.c, which holds the loop in the terminal 
*/