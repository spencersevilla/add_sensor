#ifndef _add_defs_h
#define _add_defs_h

extern int is_controller;
extern struct delayed_work hello_timer_dw;

#define HELLO_INTERVAL (3*HZ)
#define CLEANUP_INTERVAL (HELLO_INTERVAL + 1*HZ)

#define ADD_HELLO_SIZE 8

#define AF_ADD 1

#endif