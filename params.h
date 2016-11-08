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
#include <string>
#include <stdlib.h>
//byte
#define VM_SIZE 1024 * 1024 * 250
#define SP_VALUE (VM_SIZE - 1024 * 1024)


#define reg_zero 0
#define reg_sp 2
#define reg_a0 10
#define reg_a1 11
#define reg_a2 12
#define reg_a3 13
#define reg_a4 14
#define reg_a5 15
#define reg_sys_num 17



typedef unsigned char uchar;
typedef unsigned long long int ulint;
typedef unsigned int uint;
typedef long long int lint;
typedef long double ldouble;


// ulint b;
// ulint x(int lo, int len);
// ulint xs(int lo, int len);
// ulint imm_sign();

// lint i_imm() { return lint(b) >> 20; }
// lint s_imm() { return x(7, 5) + (xs(25, 7) << 5); }
// lint sb_imm() { return (x(8, 4) << 1) + (x(25,6) << 5) + (x(7,1) << 11) + (imm_sign() << 12); }
// lint u_imm() { return lint(b) >> 12 << 12; }
// lint uj_imm() { return (x(21, 10) << 1) + (x(20, 1) << 11) + (x(12, 8) << 12) + (imm_sign() << 20); }

// ulint x(int lo, int len) { return (b >> lo) & ((ulint(1) << len)-1); }
// ulint xs(int lo, int len) { return int64_t(b) << (64-lo-len) >> (64-len); }
// ulint imm_sign() { return xs(63, 1); }



uchar * vm;

lint reg[32];
// double f_reg[32];
/*union fff
{
	uint i;
	float f;
} f_reg[FLEN];
fff f_fcsr;//status and control register
//31-8:0  7:5: Rounding Mode 7:NV 3:DZ 2:OF 1:UF 0:NX

ldouble f_reg[32];
*/
union mydouble
{
	double d;
	float f[2];
} f_reg[32];


ulint PC; 

#endif //_PARAMS_H