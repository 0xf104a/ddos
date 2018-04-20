//
//  util.c
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#include "util.h"
#include "message.h"

#include <stdio.h>
#include <math.h>
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

            if (i < argc-1) {
                return argv[i + 1];
            }else{
                die("Argument required for key:%s",arg);
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

            if (i < argc-1) {
                return argv[i + 1];
            }else{
                die("Argument required for key:%s",arg);
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
char* dtoa(double x){//double to char*
    char *s=(char*)malloc(sizeof(x)+1);
    sprintf(s,"%0.1f",x);
    return s;
}
char* bytes2mb(int64_t bcount){
    return strcat(dtoa(bcount/pow(1024.0,2)), "Mb");
}
char* bytes2any(int64_t bcount,uint8_t type){
    return dtoa(bcount/pow(1024.0,2));
}
char* metrics2str(uint8_t type){
    if(type==SIZE_BYTES){
        return "B";
    }else if(type==SIZE_KB){
        return "Kb";
    }else if(type==SIZE_MB){
        return "Mb";
    }else if(type==SIZE_GB){
        return "Gb";
    }else{
        return "Tb";
    }
}
uint8_t str2metrics(char* metrics){
    if(strcmp(metrics, "b")==0){
        return SIZE_BYTES;
    }else if(strcmp(metrics, "Kb")==0){
        return SIZE_KB;
    }else if(strcmp(metrics, "Mb")==0){
        return SIZE_MB;
    }else if(strcmp(metrics, "Gb")==0){
        return SIZE_GB;
    }else if(strcmp(metrics, "Tb")==0){
        return SIZE_TB;
    }else{
        return -1;
    }
}
