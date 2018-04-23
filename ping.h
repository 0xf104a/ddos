//
//  ping.h
//  cdos
//
//  Created by Andre Zay on 22.04.2018.
//  Copyright © 2018 Andre Zay. All rights reserved.
//

#ifndef ping_h
#define ping_h

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>


bool ping(char *_host,uint32_t max,int _timeout);

#endif /* ping_h */
