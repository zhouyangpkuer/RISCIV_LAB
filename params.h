#ifndef _PARAMS_H
#define _PARAMS_H

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <elf.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>
#include <stdlib.h>
#include <cstring>

//byte
#define VM_SIZE 1024 * 1024 * 250

typedef unsigned char uchar;
typedef unsigned long long int ulint;
typedef unsigned int uint;
typedef long long int lint;
typedef long double ldouble;


uchar * vm;

lint reg[32];
double f_reg[32];

ldouble f_regl[32];


ulint PC; 

#endif //_PARAMS_H