//
//  util.h
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#ifndef util_h
#define util_h

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int randport(void);
char randchar(void);
char* randstring(int sz);
int randrange(int start, int end);
const char* getarg(const char arg[2], const char* argv[], int argc);
bool checkarg(const char arg[2], const char* argv[], int argc);
bool checklarg(const char* arg, const char* argv[], int argc);
char* readfile(char* filename);
const char* getlarg(const char* arg, const char* argv[], int argc);
void sleep_ms(int milliseconds);
char* dtoa(double x);
char* bytes2mb(int64_t bcount);
#endif /* util_h */
