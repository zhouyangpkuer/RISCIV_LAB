#ifndef _EXEC_H
#define _EXEC_H
#include "params.h"
// #define DEBUG
using namespace std;

void print_type(const char *str)
{
	printf("%s:\n", str);
}
void print_reg(uint num)
{
	printf("REG%u: %lld,", num, reg[num]);
}
void print_pc()
{
	printf("PC: %llu,", PC);
}

bool LUI(uint rd, uint imm)
{
	#ifdef DEBUG
	print_type("LUI");
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif


	int temp = imm << 12;

	//signed extended;
	reg[rd] = (lint)temp;
	PC += 4;



	#ifdef DEBUG
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif

	return true;
}

bool AUIPC(uint rd, uint imm)
{
	#ifdef DEBUG
	print_type("AUIPC");
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif



	int temp = imm << 12;
	lint temp2 = (lint)temp;
	// PC += temp2;
	reg[rd] = (lint)(PC + temp2);
	PC += 4;



	#ifdef DEBUG
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif

	return true;
}

bool JAL(uint rd, uint imm)
{	
	#ifdef DEBUG
	print_type("JAL");
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif



	if(rd != 0)
	{
		reg[rd] = (lint)(PC + 4);
	}
	//[20,1]
	PC += ((int)imm << 11) >> 11;



	#ifdef DEBUG
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif

	return true;
}
bool JALR(uint rd, uint rs1, uint imm)
{
	#ifdef DEBUG
	print_type("JALR");
	print_reg(rd);
	print_reg(rs1);
	print_pc();
	printf("\n");
	#endif


	ulint temp = PC;
	//[11,0]
	PC = ((reg[rs1] + (((int)imm << 20) >> 20)) >> 1) << 1;
	if(rd != 0)
	{
		reg[rd] = (lint)(temp + 4);
	}


	#ifdef DEBUG
	print_reg(rd);
	print_reg(rs1);
	print_pc();
	printf("\n");
	#endif

	return true;
}




bool BEQ(uint rs1, uint rs2, uint imm)
{
	#ifdef DEBUG
	print_type("BEQ");
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif


	if(reg[rs1] == reg[rs2])
		PC += ((int)imm << 19) >> 19;
	else
		PC += 4;



	#ifdef DEBUG
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	return true;
}
bool BNE(uint rs1, uint rs2, uint imm)
{
	#ifdef DEBUG
	print_type("BNE");
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif



	if(reg[rs1] != reg[rs2])
		PC += ((int)imm << 19) >> 19;
	else
		PC += 4;



	#ifdef DEBUG
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	
	return true;
}
bool BLT(uint rs1, uint rs2, uint imm)
{
	#ifdef DEBUG
	print_type("BLT");
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	


	if(reg[rs1] < reg[rs2])
		PC += ((int)imm << 19) >> 19;
	else
		PC += 4;
	


	#ifdef DEBUG
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	return true;
}
bool BGE(uint rs1, uint rs2, uint imm)
{
	#ifdef DEBUG
	print_type("BGE");
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	

	if(reg[rs1] >= reg[rs2])
		PC += ((int)imm << 19) >> 19;
	else
		PC += 4;
	

	#ifdef DEBUG
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	
	return true;
}
bool BLTU(uint rs1, uint rs2, uint imm)
{
	#ifdef DEBUG
	print_type("BLTU");
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	

	
	if((ulint)(reg[rs1]) < (ulint)(reg[rs2]))
		PC += ((int)imm << 19) >> 19;
	else
		PC += 4;
	


	#ifdef DEBUG
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	
	return true;
}
bool BGEU(uint rs1, uint rs2, uint imm)
{
	#ifdef DEBUG
	print_type("BGEU");
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	


	if((ulint)(reg[rs1]) >= (ulint)(reg[rs2]))
		PC += ((int)imm << 19) >> 19;
	else
		PC += 4;
	


	#ifdef DEBUG
	print_reg(rs1);
	print_reg(rs2);
	print_pc();
	printf("\n");
	#endif
	
	return true;
}




bool LB(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	reg[rd] = (lint)(char)(vm[target_addr]);
	PC += 4;
	return true;
}

bool LBU(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	reg[rd] = (lint)(ulint)(vm[target_addr]);
	PC += 4;
	return true;
}
//first size, next sign
bool LH(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	short int * p = (short int *)(vm + target_addr);
	reg[rd] = (lint)(*p);
	PC += 4;
	return true;
}
bool LHU(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	unsigned short int * p = (unsigned short int *)(vm + target_addr);
	reg[rd] = (lint)(ulint)(*p);
	PC += 4;
	return true;
}
bool LW(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	int * p = (int *)(vm + target_addr);
	reg[rd] = (lint)(*p);
	PC += 4;
	return true;
}
bool LWU(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	unsigned int * p = (unsigned int *)(vm + target_addr);
	reg[rd] = (lint)(ulint)(*p);
	PC += 4;
	return true;		
}
bool LD(uint rs1, uint rd, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	lint * p = (lint *)(vm + target_addr);
	reg[rd] = (*p);
	PC += 4;
	return true;
}


bool SB(uint rs1, uint rs2, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	vm[target_addr] = (uchar)(reg[rs2] & ((1 << 8) - 1)); 
	PC += 4;
	return true;
}
bool SH(uint rs1, uint rs2, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	unsigned short int *p = (unsigned short int *)(vm + target_addr);
	*p = (unsigned short int)(reg[rs2] & ((1 << 16) - 1)); 
	PC += 4;
	return true;
}
bool SW(uint rs1, uint rs2, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	unsigned int *p = (unsigned int *)(vm + target_addr);
	*p = (unsigned int)(reg[rs2] & (((ulint)1 << 32) - 1)); 
	PC += 4;
	return true;
}
bool SD(uint rs1, uint rs2, uint imm)
{
	ulint target_addr = reg[rs1] + (((int)imm << 20) >> 20);
	ulint *p = (ulint *)(vm + target_addr);
	*p = (ulint)reg[rs2]; 
	PC += 4;
	return true;	
}



bool ADDI(uint rs1, uint rd, uint imm)
{
	#ifdef DEBUG
	print_type("ADDI");
	print_reg(rs1);
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif
	

	reg[rd] = reg[rs1] + (lint)(((int)imm << 20) >> 20);
	PC += 4;	


	#ifdef DEBUG
	print_reg(rs1);
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif
	return true;
}
bool ADDIW(uint rs1, uint rd, uint imm)
{
	#ifdef DEBUG
	print_type("ADDIW");
	print_reg(rs1);
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif


	int simm = (int)imm;
	int temp = (int)reg[rs1] + ((simm << 20) >> 20);
	reg[rd] = (lint)temp;
	PC += 4;
	

	#ifdef DEBUG
	print_reg(rs1);
	print_reg(rd);
	print_pc();
	printf("\n");
	#endif
	return true;
}
bool SLTI(uint rs1, uint rd, uint imm)
{
	if(reg[rs1] < (lint)(((int)imm << 20) >> 20))
		reg[rd] = 1;
	else
		reg[rd] = 0;
	PC += 4;	
	return true;
}
bool SLTIU(uint rs1, uint rd, uint imm)
{
	if((ulint)reg[rs1] < (ulint)(((int)imm << 20) >> 20))
		reg[rd] = 1;
	else
		reg[rd] = 0;
	PC += 4;	
	return true;
}




bool XORI(uint rs1, uint rd, uint imm)
{
	reg[rd] = reg[rs1] ^ (lint)(((int)imm << 20) >> 20);
	PC += 4;	
	return true;
}
bool ORI(uint rs1, uint rd, uint imm)
{
	reg[rd] = reg[rs1] | (lint)(((int)imm << 20) >> 20);
	PC += 4;	
	return true;
}
bool ANDI(uint rs1, uint rd, uint imm)
{
	reg[rd] = reg[rs1] & (lint)(((int)imm << 20) >> 20);
	PC += 4;	
	return true;
}




bool SLLI(uint rs1, uint rd, uint shamt)
{
	reg[rd] = (ulint)reg[rs1] << shamt;
	PC += 4;
	return true;
}
bool SLLIW(uint rs1, uint rd, uint shamt)
{
	lint temp = (ulint)reg[rs1] << shamt;
	reg[rd] = ((temp & (((ulint)1 << 32) - 1)) << 32) >> 32;
	PC += 4;
	return true;
}
bool SRLI(uint rs1, uint rd, uint shamt)
{
	reg[rd] = (ulint)reg[rs1] >> shamt;
	PC += 4;
	return true;
}
bool SRLIW(uint rs1, uint rd, uint shamt)
{
	lint temp = (ulint)reg[rs1] >> shamt;
	reg[rd] = ((temp & (((ulint)1 << 32) - 1)) << 32) >> 32;
	PC += 4;
	return true;
}
bool SRAI(uint rs1, uint rd, uint shamt)
{
	reg[rd] = reg[rs1] >> shamt;
	PC += 4;
	return true;
}
bool SRAIW(uint rs1, uint rd, uint shamt)
{
	lint temp = reg[rs1] >> shamt;
	reg[rd] = ((temp & (((ulint)1 << 32) - 1)) << 32) >> 32;
	PC += 4;
	return true;
}





bool ADD(uint rs1, uint rs2, uint rd)
{
	reg[rd] = reg[rs1] + reg[rs2];
	PC += 4;
	return true;
}

bool ADDW(uint rs1, uint rs2, uint rd)
{
	lint temp = reg[rs1] + reg[rs2];
	reg[rd] = ((temp & (((ulint)1 << 32) - 1)) << 32) >> 32;
	PC += 4;
	return true;	
}

bool SUB(uint rs1, uint rs2, uint rd)
{
	reg[rd] = reg[rs1] - reg[rs2];
	PC += 4;
	return true;
}
bool SUBW(uint rs1, uint rs2, uint rd)
{
	lint temp = reg[rs1] - reg[rs2];
	reg[rd] = ((temp & (((ulint)1 << 32) - 1)) << 32) >> 32;
	PC += 4;
	return true;	
}

bool SLL(uint rs1, uint rs2, uint rd)
{
	reg[rd] = (ulint)reg[rs1] << (reg[rs2] & ((1 << 5) - 1));
	PC += 4;
	return true;
}
bool SLLW(uint rs1, uint rs2, uint rd)
{
	lint temp = (ulint)reg[rs1] << (reg[rs2] & ((1 << 5) - 1));
	reg[rd] = ((temp & (((ulint)1 << 32) - 1)) << 32) >> 32;
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
bool SRLW(uint rs1, uint rs2, uint rd)
{
	lint temp = (ulint)reg[rs1] >> (reg[rs2] & ((1 << 5) - 1));
	reg[rd] = ((temp & (((ulint)1 << 32) - 1)) << 32) >> 32;
	PC += 4;
	return true;	
}

bool SRA(uint rs1, uint rs2, uint rd)
{
	reg[rd] = reg[rs1] >> (reg[rs2] & ((1 << 5) - 1));
	PC += 4;
	return true;
}
bool SRAW(uint rs1, uint rs2, uint rd)
{
	lint temp = reg[rs1] >> (reg[rs2] & ((1 << 5) - 1));
	reg[rd] = ((temp & (((ulint)1 << 32) - 1)) << 32) >> 32;
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


bool MUL(uint rs1, uint rs2, uint rd)
{
	lint temp = reg[rs1] * reg[rs2];
	reg[rd] = temp & (((ulint)1 << 32) - 1);
	PC += 4;
	return true;
}
bool MULH(uint rs1, uint rs2, uint rd)
{
	lint temp = reg[rs1] * reg[rs2];
	reg[rd] = (temp >> 32) & (((ulint)1 << 32) - 1);
	PC += 4;
	return true;
}
bool MULHSU(uint rs1, uint rs2, uint rd)
{
	lint temp = (ulint)reg[rs1] * reg[rs2];
	reg[rd] = (temp >> 32) & (((ulint)1 << 32) - 1);
	PC += 4;
	return true;
}
bool MULHU(uint rs1, uint rs2, uint rd)
{
	ulint temp = (ulint)reg[rs1] * (ulint)reg[rs2];
	reg[rd] = (temp >> 32) & (((ulint)1 << 32) - 1);
	PC += 4;
	return true;
}
bool DIV(uint rs1, uint rs2, uint rd)
{
	lint temp = reg[rs1] / reg[rs2];
	reg[rd] = temp;
	PC += 4;
	return true;
}
bool DIVU(uint rs1, uint rs2, uint rd)
{
	ulint temp = (ulint)reg[rs1] / (ulint)reg[rs2];
	reg[rd] = temp;
	PC += 4;
	return true;
}
bool REM(uint rs1, uint rs2, uint rd)
{
	lint temp = reg[rs1] % reg[rs2];
	reg[rd] = temp;
	PC += 4;
	return true;	
}
bool REMU(uint rs1, uint rs2, uint rd)
{
	ulint temp = (ulint)reg[rs1] % (ulint)reg[rs2];
	reg[rd] = temp;
	PC += 4;
	return true;
}

bool MULW(uint rs1, uint rs2, uint rd)
{
	int temp = ((int)(reg[rs1] & (((ulint)1 << 32) - 1))) * ((int)(reg[rs2] & (((ulint)1 << 32) - 1)));
	reg[rd] = (lint)temp;
	PC += 4;
	return true;
}
bool DIVW(uint rs1, uint rs2, uint rd)
{
	int temp = ((int)(reg[rs1] & (((ulint)1 << 32) - 1))) / ((int)(reg[rs2] & (((ulint)1 << 32) - 1)));
	reg[rd] = (lint)temp;
	PC += 4;
	return true;
}
bool DIVUW(uint rs1, uint rs2, uint rd)
{
	uint temp = ((uint)(reg[rs1] & (((ulint)1 << 32) - 1))) / ((uint)(reg[rs2] & (((ulint)1 << 32) - 1)));
	reg[rd] = (lint)(int)temp;
	PC += 4;
	return true;
}
bool REMW(uint rs1, uint rs2, uint rd)
{
	int temp = ((int)(reg[rs1] & (((ulint)1 << 32) - 1))) % ((int)(reg[rs2] & (((ulint)1 << 32) - 1)));
	reg[rd] = (lint)temp;
	PC += 4;
	return true;
}
bool REMUW(uint rs1, uint rs2, uint rd)
{
	uint temp = ((uint)(reg[rs1] & (((ulint)1 << 32) - 1))) % ((uint)(reg[rs2] & (((ulint)1 << 32) - 1)));
	reg[rd] = (lint)(int)temp;
	PC += 4;
	return true;
}

bool ECALL()
{
	switch(reg[reg_sys_num])
	{
		case 64:
			// sys_write();
			printf("sys_write\n");
			break;
		case 93:
			printf("Successfully exit!\n");
			exit(0);
		default:
			printf("Other sys_call %lld, uncompleted!\n", reg[reg_sys_num]);
			break;
	};
	PC += 4;
	return true;
}

//F instruction below
bool FLW(uint rs1, uint imm, uint rd)
{
	ulint target_addr = reg[rs1] + (int)imm;
	float *p = (float *)(vm + target_addr);
	f_reg[rd].f = (float)(*p);
	PC += 4;
	return true;
}

bool FSW(uint rs1, uint rs2, uint imm)
{
	ulint target_addr = reg[rs1] + (int)imm;
	float *p = (float *)(vm + target_addr);
	*p = f_reg[rs2].f;
	PC += 4;
	return true;
}

bool FMUL_S(uint rs1, uint rs2, uint rd)
{
	float temp = f_reg[rs1].f * f_reg[rs2].f;
	f_reg[rd].f = temp;
	PC += 4;
	return true;
}

bool FDIV_S(uint rs1, uint rs2, uint rd)
{
	float temp = f_reg[rs1].f / f_reg[rs2].f;
	f_reg[rd].f = temp;
	PC += 4;
	return true;
}

bool FCVT_S_W(uint rs1, uint rd)
{
	f_reg[rd].f = float(reg[rd]&0xffffffff);
	PC += 4;
	return true;
}

bool FCVT_S_L(uint rs1, uint rd)
{
	f_reg[rd].f = float(reg[rd]);
	PC += 4;
	return true;
}




#endif //_EXEC_H