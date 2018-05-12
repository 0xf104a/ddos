//
//  message.h
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#ifndef message_h
#define message_h

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define HEADER "\033[95m"
#define OKBLUE "\033[94m"
#define OKGREEN "\033[92m"
#define WARNING "\033[93m"
#define FAIL "\033[91m"
#define ENDC "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

#define INFO "%s[*]:%s", OKBLUE, ENDC
#define ERROR "%s[-]:%s", FAIL, ENDC
#define WARN "%s[!]:%s", WARNING, ENDC
#define SUCCESS "%s[+]:%s", OKGREEN, ENDC

#define PRINT_FORMATTED(STYLE) \
    va_list args;              \
    va_start(args, format);    \
    flockfile(stdout);         \
    printf("\33[2K");          \
    printf(STYLE);             \
    vprintf(format, args);     \
    printf("\n");              \
    funlockfile(stdout);       \
    va_end(args);

#define PRINT_FORMATTED_NO_NEWLINE(STYLE) \
    va_list args;                         \
    va_start(args, format);               \
    printf(STYLE);                        \
    vprintf(format, args);                \
    va_end(args);

extern bool hide_warnings;
extern bool hide_errors;

void info(const char* format, ...);
void warning(const char* format, ...);
void error(const char* format, ...);
void success(const char* format, ...);
void success_n(const char* format, ...);
void die(const char* format, ...);
void dperror(const char* format,...);
#endif /* message_h */
