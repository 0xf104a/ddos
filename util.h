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
#include <stdint.h>

#include <stdint.h>

#define SIZE_BYTES 0
#define SIZE_KB 1
#define SIZE_MB 2
#define SIZE_GB 3
#define SIZE_TB 4
//For rand_cwmc
#define PHI 0x9e3779b9

typedef struct _slist_node{ /*node*/
    struct _slist_node *next;
    char* val;
}_node;

typedef struct _list{ /*header*/
    uint64_t length;
    _node* first;
    _node* last;
} slist;

int randport(void);
char randchar(void);
char* randstring(size_t sz);
int randrange(int start, int end);
const char* getarg(const char arg[2], const char* argv[], int argc);
bool checkarg(const char arg[2], const char* argv[], int argc);
bool checklarg(const char* arg, const char* argv[], int argc);
char* readfile(char* filename);
const char* getlarg(const char* arg, const char* argv[], int argc);
void sleep_ms(int milliseconds);
char* dtoa(double x);
char* bytes2mb(int64_t bcount);
char* bytes2any(int64_t bcount,uint8_t type);
char* metrics2str(uint8_t type);
uint8_t str2metrics(char* metrics);
const char* sgetlarg(const char *arg, const char* argv[], int argc,const char* _default);
const char* sgetarg(const char arg[2], const char* argv[], int argc,const char* _default);
bool is_root(void);
slist* create_slist(void);
void add_slist(slist* this,char* x);
void free_slist(slist *this);
_node* nth_slist(slist* this,uint64_t n);
void init_rand(uint32_t x);
uint32_t rand_cmwc(void);

#endif /* util_h */
