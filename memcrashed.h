//
//  memcrashed.h
//  cdos
//
//  Created by Andre Zay on 27.04.2018.
//  Copyright © 2018 Andre Zay. All rights reserved.
//

#ifndef memcrashed_h
#define memcrashed_h

#include "ddos.h"

void memcrashed_status(void);
void memcrashed_ddos(_dos_param* _p);
void memcrashed_init(const char* hostfile);


#endif /* memcrashed_h */
