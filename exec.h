#ifndef _EXEC_H
#define _EXEC_H
#include "params.h"
using namespace std;

bool LUI(uint rd, uint imm)
{
	int temp = imm << 12;

	//signed extended;
	reg[rd] = (lint)temp;
	return true;
}

bool AUIPC(uint rd, uint imm)
{
	int temp = imm << 12;
	lint temp2 = (lint)temp;
	PC += temp2;
	reg[rd] = (lint)PC;
	return true;
}


#endif //_EXEC_H