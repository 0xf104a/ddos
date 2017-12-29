//
//  util.c
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#include "util.h"
#include <stdio.h>
int randport()
{
    return rand() % 65534 + 1;
}
char randchar()
{
    return rand() % 126 + 1;
}
char* randstring(int sz)
{
    char* s = (char*)malloc(sz * sizeof(char));
    for (int i = 0; i < sz; i++) {
        s[i] = randchar();
    }
    return s;
}
int randrange(int start, int end)
{
    assert(start < end);
    return rand() % (start + 1 - end) + start;
}
const char* getarg(const char arg[2], const char* argv[], int argc)
{

    for (int i = 0; i < argc; i++) {
#ifdef DEBUG
        printf("%s ", argv[i]);
#endif
        if (!strcmp(arg, argv[i])) {

            if (i < argc) {
                return argv[i + 1];
            }
        }
    }
    return NULL;
}
const char* getlarg(const char* arg, const char* argv[], int argc)
{

    for (int i = 0; i < argc; i++) {
#ifdef DEBUG
        printf("%s ", argv[i]);
#endif
        if (!strcmp(arg, argv[i])) {

            if (i < argc) {
                return argv[i + 1];
            }
        }
    }
    return NULL;
}
bool checkarg(const char arg[2], const char* argv[], int argc)
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], arg)) {
            return true;
        }
    }
    return false;
}

bool checklarg(const char* arg, const char* argv[], int argc)
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], arg)) {
            return true;
        }
    }
    return false;
}

char* readfile(char* filename)
{
    char* buffer = 0;
    long length;
    FILE* f = fopen(filename, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }
    return buffer;
}
void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}
