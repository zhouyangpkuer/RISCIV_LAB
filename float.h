#include "params.h"
using namespace std;


void errorInstr(uint INS)
{
	printf("error in PC %x INS%x\n", PC, INS);
	exit(0);
}

void FLX(uint instr)
{
	ulint rd = RD(instr);
	ulint rs1 = RS1(instr);
	ulint offset = signExtend((instr >> 20), 12);

	ulint addr = reg[rs1] + offset; //注意是reg

	switch ((instr & 0x7000) >> 12)
	{
	case 0x2: //FLW;
		freg[rd] = readMem(addr, 4);
		PC += 4;
		break;
	case 0x3: //FLD;
		freg[rd] = readMem(addr, 8);
		PC += 4;
		break;
	default: errorInstr(instr);
	}
}

void FSX(uint instr)
{
	ulint rs1 = RS1(instr);
	ulint rs2 = RS2(instr);
	int32_t imm = ((instr >> 20) & 0x00000fe0) | ((instr >> 7) & 0x0000001f);
	ulint offset = signExtend(imm, 12);

	ulint addr = reg[rs1] + offset; //注意是reg

	switch ((instr & 0x7000) >> 12)
	{
	case 0x2: //FSW;
		writeMem(addr, freg[rs2] & 0xffffffff, 4);
		PC += 4;
		break;
	case 0x3: //FSD;
		writeMem(addr, freg[rs2] & 0xffffffffffffffff, 8);
		PC += 4;
		break;
	default: errorInstr(instr);
	}
}

inline float itof(uint p)
{
	return *(float *)(&p);
}

inline uint ftoi(float p)
{
	return *(uint *)(&p);
}

inline double itod(ulint p)
{
	return *(double *)(&p);
}

inline ulint dtoi(double p)
{
	return *(ulint *)(&p);
}


void FOP(uint instr)
{
	ulint rd = RD(instr);
	ulint rs1 = RS1(instr);
	ulint rs2 = RS2(instr);
	uint rm = FUNCT3(instr);

	switch (((instr) >> 25) & 0x7f)
	{
	case 0x0: //FADD_S
		freg[rd] = ftoi(itof((uint)freg[rs1]) + itof((uint)freg[rs2]));
		PC += 4;
		break;
	case 0x4: //FSUB_S
		freg[rd] = ftoi(itof((uint)freg[rs1]) - itof((uint)freg[rs2]));
		PC += 4;
		break;
	case 0x8: //FMUL_S
		freg[rd] = ftoi(itof((uint)freg[rs1]) * itof((uint)freg[rs2]));
		PC += 4;
		break;
	case 0xc: //FDIV_S
		freg[rd] = ftoi(itof((uint)freg[rs1]) / itof((uint)freg[rs2]));
		PC += 4;
		break;
	case 0x2c: //FSQRT_S
		freg[rd] = ftoi(sqrt(itof((uint)freg[rs1])));
		PC += 4;
		break;
	case 0x10:
		switch (FUNCT3(instr))
		{
		case 0x0: //FSGNJ_S
			freg[rd] = (freg[rs1] & (~0x80000000)) | (freg[rs2] & 0x80000000);
			PC += 4;
			break;
		case 0x1: //FSGNJN_S
			freg[rd] = (freg[rs1] & (~0x80000000)) | (~freg[rs2] & 0x80000000);
			PC += 4;
			break;
		case 0x2: //FSGNJX_S
			freg[rd] = (freg[rs1] & (~0x80000000)) | ((freg[rs1]^freg[rs2]) & 0x80000000);
			PC += 4;
			break;
		default:errorInstr(instr);
		}
		break;
	case 0x14:
		switch (FUNCT3(instr))
		{
		case 0x0: //FMIN_S
			// freg[rd] = ftoi(FLT_MIN);
			PC += 4;
			break;
		case 0x1: //FMAX_S
			// freg[rd] = ftoi(FLT_MAX);
			PC += 4;
			break;
		default:errorInstr(instr);
		}
		break;
	case 0x50:
		switch (FUNCT3(instr))
		{
		case 0x0: //FLE_S
			reg[rd] = itof((uint)freg[rs1]) <= itof((uint)freg[rs2]);
			PC += 4;
			break;
		case 0x1: //FLT_S
			reg[rd] = itof((uint)freg[rs1]) < itof((uint)freg[rs2]);
			PC += 4;
			break;
		case 0x2: //FEQ_S
			reg[rd] = itof((uint)freg[rs1]) == itof((uint)freg[rs2]);
			PC += 4;
			break;
		default:
			errorInstr(instr);
		}
		break;
	case 0x60:
		switch (rs2)
		{
		case 0x0: //FCVT_W_S
			reg[rd] = (int32_t)itof((uint)freg[rs1]);
			PC += 4;
			break;
		case 0x1: //FCVT_WU_S
			reg[rd] = (uint)itof((uint)freg[rs1]);
			PC += 4;
			break;
		case 0x2: //FCVT_L_S
			reg[rd] = (int64_t)itof((uint)freg[rs1]);
			PC += 4;
			break;
		case 0x3: //FCVT_LU_S
			reg[rd] = (ulint)itof((uint)freg[rs1]);
			PC += 4;
			break;
		default:errorInstr(instr);
		}
		break;
	case 0x68:
		switch (rs2)
		{
		case 0x0: //FCVT_S_W
			freg[rd] = ftoi((float)((int32_t)reg[rs1]));
			PC += 4;
			break;
		case 0x1: //FCVT_S_WU
			freg[rd] = ftoi((float)((uint)reg[rs1]));
			PC += 4;
			break;
		case 0x2: //FCVT_S_L
			freg[rd] = ftoi((float)((int64_t)reg[rs1]));
			PC += 4;
			break;
		case 0x3: //FCVT_S_LU
			freg[rd] = ftoi((float)((ulint)reg[rs1]));
			PC += 4;
			break;
		default:errorInstr(instr);
		}
		break;
	case 0x70:
		switch (FUNCT3(instr))
		{
		case 0x0: //FMV_X_S 将浮点编码存入
			reg[rd] = (uint)freg[rs1];
			PC += 4;
			break;
		case 0x1: //FCLASS_S
			switch (fpclassify(itof((uint)freg[rs1])))
			{
			case FP_INFINITE:
				if (freg[rs1] & 0x80000000)
					reg[rd] = 0; //-INF
				else
					reg[rd] = 7; //INF
				PC += 4;
				break;
			case FP_NAN: //signaling NAN 即抛出异常
				reg[rd] = 9; //quiet NAN
				PC += 4;
				break;
			case FP_NORMAL:
				if (freg[rs1] & 0x80000000)
					reg[rd] = 1; //-normal
				else
					reg[rd] = 6; //normal
				PC += 4;	
				break;
			case FP_SUBNORMAL:
				if (freg[rs1] & 0x80000000)
					reg[rd] = 2; //-subnormal
				else
					reg[rd] = 5; //subnormal
				PC += 4;
				break;
			case FP_ZERO:
				if (freg[rs1] & 0x80000000)
					reg[rd] = 3; //-0
				else
					reg[rd] = 4; //0
				PC += 4;	
				break;
			default:errorInstr(instr);
			}
			break;
		default: errorInstr(instr);
		}
		break;
	case 0x78:
		if (FUNCT3(instr) == 0x0) //FMV_S_X
			freg[rd] = (uint)reg[rs1];
		else
			errorInstr(instr);
		PC += 4;	
		break;


	case 0x1: //FADD_D
		freg[rd] = dtoi(itod(freg[rs1]) + itod(freg[rs2]));
		PC += 4;	
		break;
	case 0x5: //FSUB_D
		freg[rd] = dtoi(itod(freg[rs1]) - itod(freg[rs2]));
		PC += 4;	
		break;
	case 0x9: //FMUL_D
		freg[rd] = dtoi(itod(freg[rs1]) * itod(freg[rs2]));
		PC += 4;	
		break;
	case 0xd: //FDIV_D
		freg[rd] = dtoi(itod(freg[rs1]) / itod(freg[rs2]));
		PC += 4;	
		break;
	case 0x2d: //FSQRT_D
		freg[rd] = dtoi(sqrt(itod(freg[rs1])));
		PC += 4;	
		break;
	case 0x11:
		switch (FUNCT3(instr))
		{
		case 0x0: //FSGNJ_D
			freg[rd] = (freg[rs1] & (~0x8000000000000000)) | (freg[rs2] & 0x8000000000000000);
			PC += 4;	
			break;
		case 0x1: //FSGNJN_D
			freg[rd] = (freg[rs1] & (~0x8000000000000000)) | (~freg[rs2] & 0x8000000000000000);
			PC += 4;	
			break;
		case 0x2: //FSGNJX_D
			freg[rd] = (freg[rs1] & (~0x8000000000000000)) | ((freg[rs1] ^ freg[rs2]) & 0x8000000000000000);
			PC += 4;	
			break;
		default:errorInstr(instr);
		}
		break;
	case 0x15:
		switch (FUNCT3(instr))
		{
		case 0x0: //FMIN_D
			// freg[rd] = dtoi(DBL_MIN);
			PC += 4;	
			break;
		case 0x1: //FMAX_D
			// freg[rd] = dtoi(DBL_MAX);
			PC += 4;	
			break;
		default:errorInstr(instr);
		}
		break;
	case 0x20: //FCVT_S_D double -> float
		if (rs2 == 0x1)
			freg[rd] = ftoi((float)itod(freg[rs1]));
		else
			errorInstr(instr);
		PC += 4;	
		break;
	case 0x21: //FCVT_D_S float -> double
		if (rs2 == 0x0)
			freg[rd] = dtoi((double)itof((uint)freg[rs1]));
		else
			errorInstr(instr);
		PC += 4;	
		break;
	case 0x51:
		switch (FUNCT3(instr))
		{
		case 0x0: //FLE_D
			reg[rd] = itod(freg[rs1]) <= itod(freg[rs2]);
			PC += 4;	
			break;
		case 0x1: //FLT_D
			reg[rd] = itod(freg[rs1]) < itod(freg[rs2]);
			PC += 4;	
			break;
		case 0x2: //FEQ_D
			reg[rd] = itod(freg[rs1]) == itod(freg[rs2]);
			PC += 4;	
			break;
		default:
			errorInstr(instr);
		}
		break;
	case 0x61:
		switch (rs2)
		{
		case 0x0: //FCVT_W_D
			reg[rd] = (int32_t)itod(freg[rs1]);
			PC += 4;	
			break;
		case 0x1: //FCVT_WU_D
			reg[rd] = (uint)itod(freg[rs1]);
			PC += 4;	
			break;
		case 0x2: //FCVT_L_D
			reg[rd] = (int64_t)itod(freg[rs1]);
			PC += 4;	
			break;
		case 0x3: //FCVT_LU_D
			reg[rd] = (ulint)itod(freg[rs1]);
			PC += 4;	
			break;
		default:errorInstr(instr);
		}
		break;
	case 0x69:
		switch (rs2)
		{
		case 0x0: //FCVT_D_W
			freg[rd] = dtoi((double)((int32_t)reg[rs1]));
			PC += 4;	
			break;
		case 0x1: //FCVT_D_WU
			freg[rd] = dtoi((double)((uint)reg[rs1]));
			PC += 4;	
			break;
		case 0x2: //FCVT_D_L
			freg[rd] = dtoi((double)((int64_t)reg[rs1]));
			PC += 4;	
			break;
		case 0x3: //FCVT_D_LU
			freg[rd] = dtoi((double)((ulint)reg[rs1]));
			PC += 4;	
			break;
		default:errorInstr(instr);
		}
		break;
	case 0x71:
		switch (FUNCT3(instr))
		{
		case 0x0: //FMV_X_D 将浮点编码存入
			reg[rd] = freg[rs1];
			PC += 4;	
			break;
		case 0x1: //FCLASS_D
			switch (fpclassify(itod(freg[rs1])))
			{
			case FP_INFINITE:
				if (freg[rs1] & 0x8000000000000000)
					reg[rd] = 0; //-INF
				else
					reg[rd] = 7; //INF
				PC += 4;	
				break;
			case FP_NAN: //signaling NAN 即抛出异常z
				reg[rd] = 9; //quiet NAN
				PC += 4;	
				break;
			case FP_NORMAL:
				if (freg[rs1] & 0x8000000000000000)
					reg[rd] = 1; //-normal
				else
					reg[rd] = 6; //normal
				PC += 4;	
				break;
			case FP_SUBNORMAL:
				if (freg[rs1] & 0x8000000000000000)
					reg[rd] = 2; //-subnormal
				else
					reg[rd] = 5; //subnormal
				PC += 4;	
				break;
			case FP_ZERO:
				if (freg[rs1] & 0x8000000000000000)
					reg[rd] = 3; //-0
				else
					reg[rd] = 4; //0
				PC += 4;	
				break;
			default:errorInstr(instr);
			}
			break;
		default: errorInstr(instr);
		}
		break;
	case 0x79:
		if (FUNCT3(instr) == 0x0) //FMV_D_X
			freg[rd] = reg[rs1];
		else
			errorInstr(instr);
		PC += 4;	
		break;

	default: errorInstr(instr);
	}
}

void FMADDX(uint instr)
{
	ulint rd = RD(instr);
	ulint rs1 = RS1(instr);
	ulint rs2 = RS2(instr);
	ulint rs3 = (instr >> 27) & 0x1f;
	uint rm = FUNCT3(instr);

	switch (((instr) >> 25) & 0x3)
	{
	case 0x0: //FMADD_S
		freg[rd] = ftoi(itof((uint)freg[rs1]) * itof((uint)freg[rs2]) + itof((uint)freg[rs3]));
		PC += 4;	
		break;
	case 0x1: //FMADD_D
		freg[rd] = dtoi(itod(freg[rs1]) * itod(freg[rs2]) + itod(freg[rs3]));
		PC += 4;	
		break;
	default: errorInstr(instr);
	}

}