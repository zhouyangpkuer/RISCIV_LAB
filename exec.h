#ifndef _EXEC_H
#define _EXEC_H
#include "params.h"
using namespace std;

bool LUI(uint rd, uint imm)
{
	int temp = imm << 12;

	//signed extended;
	reg[rd] = (lint)temp;
	PC += 4;
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

bool JAL(uint rd, uint imm)
{
	if(rd != 0)
	{
		reg[rd] = (lint)(PC + 4);
	}
	PC += (int)imm;
	return true;
}
bool JALR(uint rd, uint rs1, uint imm)
{
	ulint temp = PC;
	PC = ((reg[rs1] + (int)imm) >> 1) << 1;
	if(rd != 0)
	{
		reg[rd] = (lint)(temp + 4);
	}
	return true;
}

bool BEQ(uint rs1, uint rs2, uint imm)
{
	if(reg[rs1] == reg[rs2])
		PC += (int)imm;
	else
		PC += 4;
	return true;
}

bool BNE(uint rs1, uint rs2, uint imm)
{
	if(reg[rs1] != reg[rs2])
		PC += (int)imm;
	else
		PC += 4;
	return true;
}

bool BLT(uint rs1, uint rs2, uint imm)
{
	if(reg[rs1] < reg[rs2])
		PC += (int)imm;
	else
		PC += 4;
	return true;
}

bool BGE(uint rs1, uint rs2, uint imm)
{
	if(reg[rs1] >= reg[rs2])
		PC += (int)imm;
	else
		PC += 4;
	return true;
}

bool BLTU(uint rs1, uint rs2, uint imm)
{
	if((ulint)(reg[rs1]) < (ulint)(reg[rs2]))
		PC += (int)imm;
	else
		PC += 4;
	return true;
}

bool BGEU(uint rs1, uint rs2, uint imm)
{
	if((ulint)(reg[rs1]) >= (ulint)(reg[rs2]))
		PC += (int)imm;
	else
		PC += 4;
	return true;
}

bool LB(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (int)imm;
	reg[rd] = (lint)(char)(vm[target_addr]);
	PC += 4;
	return true;
}

bool LBU(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (int)imm;
	reg[rd] = (lint)(ulint)(vm[target_addr]);
	PC += 4;
	return true;
}
//first size, next sign
bool LH(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (int)imm;
	short int * p = (short int *)(vm + target_addr);
	reg[rd] = (lint)(*p);
	PC += 4;
	return true;
}

bool LHU(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (int)imm;
	unsigned short int * p = (unsigned short int *)(vm + target_addr);
	reg[rd] = (lint)(ulint)(*p);
	PC += 4;
	return true;
}

bool LW(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (int)imm;
	unsigned int * p = (unsigned int *)(vm + target_addr);
	reg[rd] = (lint)(*p);
	PC += 4;
	return true;
}
bool SB(uint rs1, uint rs2, uint imm)
{
	ulint target_addr = reg[rs1] + (int)imm;
	vm[target_addr] = (uchar)(reg[rs2] & ((1 << 8) - 1)); 
	PC += 4;
	return true;
}
bool SH(uint rs1, uint rs2, uint imm)
{
	ulint target_addr = reg[rs1] + (int)imm;
	unsigned short int *p = (unsigned short int *)(vm + target_addr);
	*p = (unsigned short int)(reg[rs2] & ((1 << 16) - 1)); 
	PC += 4;
	return true;
}
bool SW(uint rs1, uint rs2, uint imm)
{
	ulint target_addr = reg[rs1] + (int)imm;
	unsigned int *p = (unsigned int *)(vm + target_addr);
	*p = (unsigned int)(reg[rs2] & (((ulint)1 << 32) - 1)); 
	PC += 4;
	return true;
}

bool ADDI(uint rs1, uint rd, uint imm)
{
	reg[rd] = reg[rs1] + (lint)(int)imm;
	PC += 4;	
}

bool SLTI(uint rs1, uint rd, uint imm)
{
	if(reg[rs1] < (lint)(int)imm)
		reg[rd] = 1;
	else
		reg[rd] = 0;
	PC += 4;	
	return true;
}
bool SLTIU(uint rs1, uint rd, uint imm)
{
	if((ulint)reg[rs1] < (ulint)imm)
		reg[rd] = 1;
	else
		reg[rd] = 0;
	PC += 4;	
	return true;
}
bool XORI(uint rs1, uint rd, uint imm)
{
	reg[rd] = reg[rs1] ^ (lint)(int)imm;
	PC += 4;	
	return true;
}
bool ORI(uint rs1, uint rd, uint imm)
{
	reg[rd] = reg[rs1] | (lint)(int)imm;
	PC += 4;	
	return true;
}

bool ANDI(uint rs1, uint rd, uint imm)
{
	reg[rd] = reg[rs1] & (lint)(int)imm;
	PC += 4;	
	return true;
}

bool SLLI(uint rs1, uint rd, uint shamt)
{
	reg[rd] = (ulint)reg[rs1] << shamt;
	PC += 4;
	return true;
}
bool SRLI(uint rs1, uint rd, uint shamt)
{
	reg[rd] = (ulint)reg[rs1] >> shamt;
	PC += 4;
	return true;
}
bool SRAI(uint rs1, uint rd, uint shamt)
{
	reg[rd] = reg[rs1] >> shamt;
	PC += 4;
	return true;
}

bool ADD(uint rs1, uint rs2, uint rd)
{
	reg[rd] = reg[rs1] + reg[rs2];
	PC += 4;
	return true;
}
bool SUB(uint rs1, uint rs2, uint rd)
{
	reg[rd] = reg[rs1] - reg[rs2];
	PC += 4;
	return true;
}
bool SLL(uint rs1, uint rs2, uint rd)
{
	reg[rd] = (ulint)reg[rs1] << (reg[rs2] & ((1 << 5) - 1));
	PC += 4;
	return true;
}
bool SLT(uint rs1, uint rs2, uint rd)
{
	if(reg[rs1] < reg[rs2])
		reg[rd] = 1;
	else
		reg[rd] = 0;
	PC += 4;
	return true;
}
bool SLTU(uint rs1, uint rs2, uint rd)
{
	if((ulint)reg[rs1] < (ulint)reg[rs2])
		reg[rd] = 1;
	else
		reg[rd] = 0;
	PC += 4;
	return true;
}
bool XOR(uint rs1, uint rs2, uint rd)
{
	reg[rd] = reg[rs1] ^ reg[rs2];
	PC += 4;
	return true;
}
bool SRL(uint rs1, uint rs2, uint rd)
{
	reg[rd] = (ulint)reg[rs1] >> (reg[rs2] & ((1 << 5) - 1));
	PC += 4;
	return true;
}
bool SRA(uint rs1, uint rs2, uint rd)
{
	reg[rd] = reg[rs1] >> (reg[rs2] & ((1 << 5) - 1));
	PC += 4;
	return true;
}
bool OR(uint rs1, uint rs2, uint rd)
{
	reg[rd] = reg[rs1] | reg[rs2];
	PC += 4;
	return true;
}
bool AND(uint rs1, uint rs2, uint rd)
{
	reg[rd] = reg[rs1] & reg[rs2];
	PC += 4;
	return true;
}
#endif //_EXEC_H