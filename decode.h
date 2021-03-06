#ifndef _DECODE_H
#define _DECODE_H
#include "params.h"
#include "exec.h"

using namespace std;

//include the end bit
//[start, end]

bool decode(uchar * p_entry)
{
	// char buff[100];
	// reg[0] = 0;
	for(int i = 0; i < 32; i++)
		reg[i] = 0;

	reg[reg_zero] = 0;
	reg[reg_sp] = SP_VALUE;

	uint INS;
	uint * p_ins;
	PC = (ulint)p_entry;


	bool EXIT = false;
	uint rd, rs1, rs2, rm, imm, opcode, funct3, funct7, shamt;
	uint rs3, funct5, fmt;
	bool flag;

	FILE * file_res = fopen("./PC_res.txt", "w");

	lint cnt = 0;
	while(!EXIT)
	{
		cnt ++;
		// p_ins = (uint *)(vm + PC);
		// INS = *p_ins;
		INS = readMem(PC, 4);

		// if(cnt % 1000000 == 0)
			fprintf(file_res, "0x%X ", PC);
		opcode = INS & ((1 << 7) - 1);

		switch(opcode)
		{
			//LUI
			case 0b0110111:
				rd = get_part(7, 11, INS);
				imm = get_part(12, 31, INS);
				flag = LUI(rd, imm);
				warning(flag, "LUI");
				break;
	
			//AUIPC
			case 0b0010111:
				rd = get_part(7, 11, INS);
				imm = get_part(12, 31, INS);
				flag = AUIPC(rd, imm);
				warning(flag, "AUIPC");
				break;
	
			//JAL
			case 0b1101111:
				rd = get_part(7, 11, INS);
				imm = 0;
				imm += get_part(21, 30, INS) << 1;
				imm += get_part(20, 20, INS) << 11;
				imm += get_part(12, 19, INS) << 12;
				imm += get_part(31, 31, INS) << 20;
				flag = JAL(rd, imm);
				warning(flag, "JAL");
				break;
	
			//JALR
			case 0b1100111:
				rd = get_part(7, 11, INS);
				rs1 = get_part(15, 19, INS);
				imm = get_part(20, 31, INS);
				flag = JALR(rd, rs1, imm);
				warning(flag, "JALR");
				break;
				
			//conditional branch
			case 0b1100011:
			 	rs1 = get_part(15, 19, INS);
			 	rs2 = get_part(20, 24, INS);
			 	imm = 0;
				imm += get_part(8, 11, INS) << 1;
				imm += get_part(25, 30, INS) << 5;
				imm += get_part(7, 7, INS) << 11;
				imm += get_part(31, 31, INS) << 12;
	
				funct3 = get_part(12, 14, INS);
				switch(funct3)
				{
					//BEQ
					case 0b000:
						flag = BEQ(rs1, rs2, imm);
						warning(flag, "BEQ");
						break;
					//BNE
					case 0b001:
						flag = BNE(rs1, rs2, imm);
						warning(flag, "BNE");
						break;
					//BLT
					case 0b100:
						flag = BLT(rs1, rs2, imm);
						warning(flag, "BLT");
						break;
					//BGE
					case 0b101:
						flag = BGE(rs1, rs2, imm);
						warning(flag, "BGE");
						break;
					//BLTU
					case 0b110:
						flag = BLTU(rs1, rs2, imm);
						warning(flag, "BLTU");
						break;
					//BGEU
					case 0b111:
						flag = BGEU(rs1, rs2, imm);
						warning(flag, "BGEU");
						break;
					//theoretically impossible 
					default:
						printf("unknown conditional branch funct3 0x%X PC 0x%X\n", funct3, PC);
						exit(0);
				};
				break;
			//Load
			case 0b0000011:
				rs1 = get_part(15, 19, INS);
				rd = get_part(7, 11, INS);
				imm = get_part(20, 31, INS);
				funct3 = get_part(12, 14, INS);
				switch(funct3)
				{
					//LB
					case 0b000:
						flag = LB(rs1, rd, imm);
						warning(flag, "LB");
						break;
					//LH
					case 0b001:
						flag = LH(rs1, rd, imm);
						warning(flag, "LH");
						break;
					//LW
					case 0b010:
						flag = LW(rs1, rd, imm);
						warning(flag, "LW");
						break;
					//LBU
					case 0b100:
						flag = LBU(rs1, rd, imm);
						warning(flag, "LBU");
						break;
					//LHU
					case 0b101:
						flag = LHU(rs1, rd, imm);
						warning(flag, "LHU");
						break;
					//LWU
					case 0b110:
						flag = LWU(rs1, rd, imm);
						warning(flag, "LWU");
						break;
					//LD
					case 0b011:
						flag = LD(rs1, rd, imm);
						warning(flag, "LD");
						break;

					//theoretically impossible 
					default:
						printf("unknown load funct3 0x%X PC 0x%X\n", funct3, PC);
						exit(0);
				};
				break;
			//store
			case 0b0100011:
				rs1 = get_part(15, 19, INS);
				rs2 = get_part(20, 24, INS);
				imm = 0;
				imm += get_part(7, 11, INS);
				imm += get_part(25, 31, INS) << 5;
				funct3 = get_part(12, 14, INS);
				switch(funct3)
				{
					//SB
					case 0b000:
						flag = SB(rs1, rs2, imm);
						warning(flag, "SB");
						break;
					//SH
					case 0b001:
						flag = SH(rs1, rs2, imm);
						warning(flag, "SH");
						break;
					//SW
					case 0b010:
						flag = SW(rs1, rs2, imm);
						warning(flag, "SW");
						break;
					//SD
					case 0b011:
						flag = SD(rs1, rs2, imm);
						warning(flag, "SD");
						break;

					default:
						printf("unknown store funct3 0x%X PC 0x%X\n", funct3, PC);
						exit(0);
				};
				break;
			//ALU
			case 0b0010011:
				rs1 = get_part(15, 19, INS);
				rd = get_part(7, 11, INS);
				funct3 = get_part(12, 14, INS);
				imm = get_part(20, 31, INS);
				shamt = get_part(20, 25, INS);
				switch(funct3)
				{
					//ADDI
					case 0b000:
						flag = ADDI(rs1, rd, imm);
						warning(flag, "ADDI");
						break;
					//SLTI
					case 0b010:
						flag = SLTI(rs1, rd, imm);
						warning(flag, "SLTI");
						break;
					//SLTIU
					case 0b011:
						flag = SLTIU(rs1, rd, imm);
						warning(flag, "SLTIU");
						break;
					//XORI
					case 0b100:
						flag = XORI(rs1, rd, imm);
						warning(flag, "XORI");
						break;
					//ORI
					case 0b110:
						flag = ORI(rs1, rd, imm);
						warning(flag, "ORI");
						break;
					//ANDI
					case 0b111:
						flag = ANDI(rs1, rd, imm);
						warning(flag, "ANDI");
						break;
					//SLLI
					case 0b001:
						flag = SLLI(rs1, rd, shamt);
						warning(flag, "SLLI");
						break;
					//SRLI, SRAI
					case 0b101:
						funct7 = get_part(26, 31, INS);
						//SRLI
						if(funct7 == 0b000000)
						{
							flag = SRLI(rs1, rd, shamt);
							warning(flag, "SRLI");
						}
						//SRAI
						else if(funct7 == 0b010000)
						{
							flag = SRAI(rs1, rd, shamt);
							warning(flag, "SRAI");
						}
						else
						{
							printf("unknown SRXI funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);
						}
						break;

					default:
					 	printf("unknown ALU funct3 0x%X PC 0x%X\n", funct3, PC);
					 	exit(0);

				};
				break;

			//ALU_2
			case 0b0110011:
				rs1 = get_part(15, 19, INS);
				rs2 = get_part(20, 24, INS);
				rd = get_part(7, 11, INS);
				funct3 = get_part(12, 14, INS);
				funct7 = get_part(25, 31, INS);
				switch(funct3)
				{
					//ADD, SUB
					case 0b000:
						//ADD
						if(funct7 == 0b0000000)
						{	
							flag = ADD(rs1, rs2, rd);
							warning(flag, "ADD");
						}
						//SUB
						else if(funct7 == 0b0100000)
						{
							flag = SUB(rs1, rs2, rd);
							warning(flag, "SUB");
						}
						//MUL
						else if(funct7 == 0b0000001)
						{
							flag = MUL(rs1, rs2, rd);
							warning(flag, "MUL");
						}
						else
						{
							printf("unknown ADD SUB funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);	
						}
						break;
					case 0b001:
						//SLL
						if(funct7 == 0b0000000)
						{
							flag = SLL(rs1, rs2, rd);
							warning(flag, "SLL");
						}
						//MULH
						else if(funct7 == 0b0000001)
						{
							flag = MULH(rs1, rs2, rd);
							warning(flag, "MULH");
						}
						else
						{
							printf("unknown SLL MULH funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);
						}

						break;
					case 0b010:
						//SLT
						if(funct7 == 0b0000000)
						{
							flag = SLT(rs1, rs2, rd);
							warning(flag, "SLT");
						}
						//MULHSU
						else if(funct7 == 0b0000001)
						{
							flag = MULHSU(rs1, rs2, rd);
							warning(flag, "MULHSU");
						}
						else
						{
							printf("unknown SLT MULHSU funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);
						}
						break;
					
					case 0b011:
						//SLTU
						if(funct7 == 0b0000000)
						{
							flag = SLTU(rs1, rs2, rd);
							warning(flag, "SLTU");
						}
						// MULHU
						else if(funct7 == 0b0000001)
						{
							flag = MULHU(rs1, rs2, rd);
							warning(flag, "MULHU");
						}
						else
						{
							printf("unknown SLTU MULHU funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);		
						}
						break;
					
					case 0b100:
						//XOR
						if(funct7 == 0b0000000)
						{
							flag = XOR(rs1, rs2, rd);
							warning(flag, "XOR");
						}
						//DIV
						else if(funct7 == 0b0000001)
						{
							flag = DIV(rs1, rs2, rd);
							warning(flag, "DIV");
						}	
						else
						{
							printf("unknown XOR DIV funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);		
						}
						break;
					
					//SRL,SRA
					case 0b101:
						//SRL
						if(funct7 == 0b0000000)
						{
							flag = SRL(rs1, rs2, rd);
							warning(flag, "ADD");
						}
						//SRA
						else if(funct7 == 0b0100000)
						{
							flag = SRA(rs1, rs2, rd);
							warning(flag, "SRA");
						}
						//DIVU
						else if(funct7 == 0b0000001)
						{
							flag = DIVU(rs1, rs2, rd);
							warning(flag, "DIVU");	
						}
						else
						{
							printf("unknown SRL A funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);
						}
						break;
					case 0b110:
						//OR
						if(funct7 == 0b0000000)
						{
							flag = OR(rs1, rs2, rd);
							warning(flag, "OR");
						}
						//REM
						else if(funct7 == 0b0000001)
						{
							flag = REM(rs1, rs2, rd);
							warning(flag, "REM");	
						}
						else
						{
							printf("unknown OR REM funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);	
						}
						break;
					
					case 0b111:
						//AND
						if(funct7 == 0b0000000)
						{
							flag = AND(rs1, rs2, rd);
							warning(flag, "AND");
						}
						//REMU
						else if(funct7 == 0b0000001)
						{	
							flag = REMU(rs1, rs2, rd);
							warning(flag, "REMU");	
						}
						else
						{
							printf("unknown AND REMU funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);	
						}
						break;
					
					default:
						printf("unknown ALU2 funct3 0x%X PC 0x%X\n", funct3, PC);
						exit(0);
				};
				break;


			//ALUW
			case 0b0011011:
				rs1 = get_part(15, 19, INS);
				rd = get_part(7, 11, INS);
				funct3 = get_part(12, 14, INS);
				// funct7 = get_part(25, 31, INS);
				funct7 = get_part(26, 31, INS);
				imm = get_part(20, 31, INS);
				shamt = get_part(20, 25, INS);
				switch(funct3)
				{
					//ADDIW
					case 0b000:
						flag = ADDIW(rs1, rd, imm);
						warning(flag, "ADDIW");
						break;
					//SLLIW
					case 0b001:
						flag = SLLIW(rs1, rd, shamt);
						warning(flag, "SLLIW");
						break;
					//SRLIW,SRAIW
					case 0b101:
						//SRLIW
						if(funct7 == 0b000000)
						{
							flag = SRLIW(rs1, rd, shamt);
							warning(flag, "SRLIW");
						}
						//SRAIW
						else if(funct7 == 0b010000)
						{
							flag = SRAIW(rs1, rd, shamt);
							warning(flag, "SRAIW");
						}
						else
						{
							printf("unknown SRLIW SRAIW funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);
						}
						break;
					default:
						printf("unknown ALUW funct3 0x%X PC 0x%X\n", funct3, PC);
						exit(0);
				};
				break;

			//ALUW2
			case 0b0111011:
				rs1 = get_part(15, 19, INS);
				rs2 = get_part(20, 24, INS);
				rd = get_part(7, 11, INS);
				funct3 = get_part(12, 14, INS);
				funct7 = get_part(25, 31, INS);
				switch(funct3)
				{
					//ADDW, SUBW
					case 0b000:
						//ADDW
						if(funct7 == 0b0000000)
						{	
							flag = ADDW(rs1, rs2, rd);
							warning(flag, "ADDW");
						}
						//SUBW
						else if(funct7 == 0b0100000)
						{
							flag = SUBW(rs1, rs2, rd);
							warning(flag, "SUBW");
						}
						//MULW
						else if(funct7 == 0b0000001)
						{
							flag = MULW(rs1, rs2, rd);
							warning(flag, "MULW");
						}
						else
						{
							printf("unknown ADDW SUBW funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);
						}
						break;
					//SLLW
					case 0b001:
						flag = SLLW(rs1, rs2, rd);
						warning(flag, "SLLW");
						break;
					//DIVW
					case 0b100:
						flag = DIVW(rs1, rs2, rd);
						warning(flag, "DIVW");
						break;
					//SRLW,SRAW
					case 0b101:
						//SRLW
						if(funct7 == 0b0000000)
						{
							flag = SRLW(rs1, rs2, rd);
							warning(flag, "SRLW");
						}
						//SRAW
						else if(funct7 == 0b0100000)
						{
							flag = SRAW(rs1, rs2, rd);
							warning(flag, "SRAW");
						}
						//DIVUW
						else if(funct7 == 0b0000001)
						{
							flag = DIVUW(rs1, rs2, rd);
							warning(flag, "DIVUW");
						}
						else 
						{
							printf("unknown SRLW SRAW funct7 0x%X PC 0x%X\n", funct7, PC);
							exit(0);
						}
						break;
					//REMW
					case 0b110:
						flag = REMW(rs1, rs2, rd);
						warning(flag, "REMW");
						break;
					//REMUW
					case 0b111:
						flag = REMUW(rs1, rs2, rd);
						warning(flag, "REMUW");
						break;
					default:
						printf("unknown ALUW2 funct3 0x%X PC 0x%X\n", funct3, PC);
						exit(0);

				};
				break;
				
			//FENCE, FENCE.I
			case 0b0001111:
				printf("FENCE and FENCE.I, uncompleted!\n");
				exit(0);

			//ECALL, EBREAK, CSR
			case 0b1110011:
				if(INS == 0b1110011)
				{
					bool flag = ECALL(EXIT);
					warning(flag, "ECALL");
				}
				else
				{	
					printf("EBREAK and CSR, uncompleted!\n");
					exit(0);
				}
				break;

			//LWU, LD contained before;
		
			
			//SD contained before;


			//SLLI, SRLI, SRAI contained before;


			//32M contained before
			// case 0b0110011:
	
			//64M
			// case 0b0111011:

			
			case 0b0000111:
				FLX(INS);
				break;

			case 0b0100111:
				FSX(INS);
				break;

			case 0b1010011:
				FOP(INS);
				break;

			case 0b1000011:
				FMADDX(INS);
				break;

			//F below------------------------------------------------------------------
			// case 0b0000111:
			// 	//FLW
			// 	rd = get_part(7,11,INS);
			// 	rs1 = get_part(15,19,INS);
			// 	funct3 = get_part(12, 14, INS);
			// 	imm = get_part(20,31,INS);
			// 	//FLD
			// 	if(funct3 == 0b011)
			// 	{
			// 		flag = FLD(rs1, imm, rd);
			// 		warning(flag, "FLD");
			// 	}
			// 	//FLW
			// 	else if(funct3 == 0b010)
			// 	{
			// 		flag = FLW(rs1,imm,rd);
			// 		warning(flag, "FLW");
			// 	}
			// 	else 
			// 	{
			// 		printf("unknown FLD FLW funct3 0x%X PC 0x%X\n", funct3, PC);
			// 		exit(0);
			// 	}
			// 	break;
			// case 0b0100111:
			// 	//FSW
			// 	rs1 = get_part(15,19,INS);
			// 	rs2 = get_part(20,24,INS);
			// 	imm = 0;
			// 	imm += get_part(7, 11, INS);
			// 	imm += get_part(25, 31, INS) << 5;
			// 	// imm += get_part(7, 11, INS) + get_part(25, 31, INS) << 5;

			// 	funct3 = get_part(12, 14, INS);
			// 	//FSD
			// 	if(funct3 == 0b011)
			// 	{
			// 		flag = FSD(rs1, imm, rs2);
			// 		warning(flag, "FSD");
			// 	}
			// 	//FSW
			// 	else if(funct3 == 0b010)
			// 	{
			// 		flag = FSW(rs1,rs2,imm);
			// 		warning(flag, "FSW");
			// 	}
			// 	else 
			// 	{
			// 		printf("unknown FSD FSW funct3 0x%X PC 0x%X\n", funct3, PC);
			// 		exit(0);
			// 	}
			// 	break;
			// 	//FMADD.D
			// case 0b1000011:
			// {
			// 	rs1 = get_part(15, 19, INS);
			// 	rs2 = get_part(20, 24, INS);
			// 	rs3 = get_part(27, 31, INS);
			// 	rd = get_part(7, 11, INS);
			// 	uint fmt = get_part(25, 26, INS);
			// 	//FMADD_D
			// 	if(fmt == 0b01)
			// 	{
			// 		flag = FMADD_D(rs1, rs2, rs3, rd);
			// 		warning(flag, "FMADD_D");
			// 	}
			// 	else 
			// 	{
			// 		printf("unknown FMADD_D fmt 0x%X PC 0x%X\n", fmt, PC);
			// 		exit(0);
			// 	}
			// 	break;
			// }
			// //FMSUB.D
			// case 0b1000111:
			// {
			// 	rs1 = get_part(15, 19, INS);
			// 	rs2 = get_part(20, 24, INS);
			// 	rs3 = get_part(27, 31, INS);
			// 	rd = get_part(7, 11, INS);
			// 	uint fmt = get_part(25, 26, INS);
			// 	//FMSUB_D
			// 	if(fmt == 0b01)
			// 	{
			// 		flag = FMSUB_D(rs1, rs2, rs3, rd);
			// 		warning(flag, "FMSUB_D");
			// 	}
			// 	else 
			// 	{
			// 		printf("unknown FMSUB_D fmt 0x%X PC 0x%X\n", fmt, PC);
			// 		exit(0);
			// 	}
			// 	break;
			// }
			// //FNMSUB.D
			// case 0b1001011:
			// {
			// 	rs1 = get_part(15, 19, INS); 
			// 	rs2 = get_part(20, 24, INS);
			// 	rs3 = get_part(27, 31, INS);
			// 	rd = get_part(7, 11, INS);
			// 	uint fmt = get_part(25, 26, INS);
			// 	if(fmt == 0b01)
			// 	{
			// 		flag = FNMSUB_D(rs1, rs2, rs3, rd);
			// 		warning(flag, "FNMSUB_D");
			// 	}
			// 	else 
			// 	{
			// 		printf("unknown FNMSUB_D fmt 0x%X PC 0x%X\n", fmt, PC);
			// 		exit(0);
			// 	}
			// 	break;
			// }
			// //FNMADD
			// case 0b1001111:
			// {
			// 	rs1 = get_part(15, 19, INS);
			// 	rs2 = get_part(20, 24, INS);
			// 	rs3 = get_part(27, 31, INS);
			// 	rd = get_part(7, 11, INS);
			// 	uint fmt = get_part(25, 26, INS);
			// 	if(fmt == 0b01)
			// 	{
			// 		flag = FNMADD_D(rs1, rs2, rs3, rd);
			// 		warning(flag, "FNMADD_D");
			// 	}
			// 	else 
			// 	{
			// 		printf("unknown FNMADD_D fmt 0x%X PC 0x%X\n", fmt, PC);
			// 		exit(0);
			// 	}
			// 	break;
			// }

			// //floating point operations
			// case 0b1010011:
			// {
			// 	funct5 = get_part(25, 31, INS);
			// 	switch (funct5)
			// 	{
			// 		//FMV_X_D
			// 		case 0b1110001:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FMV_X_D(rs1, rd);
			// 			warning(flag, "FMV_X_D");
			// 			break;
			// 		}
			// 		//FMV_D_X
			// 		case 0b1111001:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FMV_D_X(rs1, rd);
			// 			warning(flag, "FMV_D_X");
			// 			break;
			// 		}
			// 		//FADD_D
			// 		case 0b0000001:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FADD_D(rs1, rs2, rd);
			// 			warning(flag, "FADD_D");
			// 			break;
			// 		}
			// 		//FSUB_D
			// 		case 0b0000101:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);
						
			// 			flag = FSUB_D(rs1, rs2, rd);
			// 			warning(flag, "FSUB_D");
			// 			break;
			// 		}
			// 		//FMUL_D
			// 		case 0b0001001:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FMUL_D(rs1, rs2, rd);
			// 			warning(flag, "FMUL_D");
			// 			break;
			// 		}
			// 		//FUL_S
			// 		case 0b0001000:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FMUL_S(rs1, rs2, rd);
			// 			warning(flag, "FMUL_S");
			// 			break;
			// 		}
			// 		//FDIV_D
			// 		case 0b0001101:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FDIV_D(rs1, rs2, rd);
			// 			warning(flag, "FDIV_D");
			// 			break;
			// 		}
			// 		//FDIV_S
			// 		case 0b0001100:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FDIV_S(rs1, rs2, rd);
			// 			warning(flag, "FDIV_S");
			// 			break;
			// 		}
			// 		//FSQRT_D
			// 		case 0b0101101:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FSQRT_D(rs1, rd);
			// 			warning(flag, "FSQRT_D");
			// 			break;
			// 		}
			// 		//FSGNJ_D
			// 		case 0b0010001:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);
			// 			funct3 = get_part(12, 14, INS);

			// 			switch (funct3)
			// 			{
			// 				case 0b000:
			// 				{
			// 					flag = FSGNJ_D(rs1, rs2, rd);
			// 					warning(flag, "FSGNJ_D");
			// 					break;
			// 				}
			// 				case 0b001:
			// 				{
			// 					flag = FSGNJN_D(rs1, rs2, rd);
			// 					warning(flag, "FSGNJN_D");
			// 					break;
			// 				}
			// 				case 0b010:
			// 				{
			// 					flag = FSGNJX_D(rs1, rs2, rd);
			// 					warning(flag, "FSGNJX_D");
			// 					break;
			// 				}
			// 				default:
			// 				{
			// 					printf("unknown FSGNJ_D funct3 0x%X PC 0x%X\n", funct3, PC);
			// 					exit(0);
			// 				}
			// 			}
			// 			break;
			// 		}
			// 		//FCVT_S_D
			// 		case 0b0100000:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FCVT_S_D(rs1, rd);
			// 			warning(flag, "FCVT_S_D");
			// 			break;
			// 		}
			// 		//FCVT_D_S
			// 		case 0b0100001:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rd = get_part(7, 11, INS);

			// 			flag = FCVT_D_S(rs1, rd);
			// 			warning(flag, "FCVT_D_S");
			// 			break;
			// 		}
			// 		//Fcmp
			// 		case 0b1010001:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);
			// 			uint rm = get_part(12, 14, INS);
			// 			switch (rm)
			// 			{
			// 				//FEQ_D
			// 				case 0b010:
			// 				{
			// 					flag = FEQ_D(rs1, rs2, rd);
			// 					warning(flag, "FEQ_D");
			// 					break;
			// 				}
			// 				//FLT_D
			// 				case 0b001:
			// 				{
			// 					flag = FLT_D(rs1, rs2, rd);
			// 					warning(flag, "FLT_D");
			// 					break;
			// 				}
			// 				//FLE_D
			// 				case 0b000:
			// 				{
			// 					flag = FLE_D(rs1, rs2, rd);
			// 					warning(flag, "FLE_D");
			// 					break;
			// 				}
			// 				default:
			// 					printf("unknown Fcmp rm 0x%X PC 0x%X\n", rm, PC);
			// 					exit(0);
			// 			}
			// 			break;
			// 		}
			// 		case 0b1100001:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);
			// 			switch (rs2)
			// 			{
			// 				//FCVT_W_D
			// 				case 0b00000:
			// 				{
			// 					flag = FCVT_W_D(rs1, rd);
			// 					warning(flag, "FCVT_W_D");
			// 					break;
			// 				}
			// 				//FCVT_WU_D
			// 				case 0b00001:
			// 				{
			// 					flag = FCVT_WU_D(rs1, rd);
			// 					warning(flag, "FCVT_WU_D");
			// 					break;
			// 				}
			// 				default:
			// 					printf("unknown FCVT_W rs2 0x%X PC 0x%X\n", rs2, PC);
			// 					exit(0);
			// 			}
			// 			break;
			// 		}
			// 		case 0b1101001:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);
			// 			switch (rs2)
			// 			{
			// 				//FCVT_D_W
			// 				case 0b00000:
			// 				{
			// 					flag = FCVT_D_W(rs1, rd);
			// 					warning(flag, "FCVT_D_W");
			// 					break;
			// 				}
			// 				//FCVT_D_WU
			// 				case 0b00001:
			// 				{
			// 					flag = FCVT_D_WU(rs1, rd);
			// 					warning(flag, "FCVT_D_WU");
			// 					break;
			// 				}
			// 				default:
			// 					printf("unknown FCVT_D rs2 0x%X PC 0x%X\n", rs2, PC);
			// 					exit(0);
			// 			}
			// 			break;
			// 		}
			// 		case 0b1101000:
			// 		{
			// 			rs1 = get_part(15, 19, INS);
			// 			rs2 = get_part(20, 24, INS);
			// 			rd = get_part(7, 11, INS);
			// 			switch (rs2)
			// 			{
			// 				//FCVT_S_W
			// 				case 0b00000:
			// 				{
			// 					flag = FCVT_S_W(rs1, rd);
			// 					warning(flag, "FCVT_S_W");
			// 					break;
			// 				}
			// 				case 0b00010:
			// 				{
			// 					flag = FCVT_S_L(rs1, rd);
			// 					warning(flag, "FCVT_S_L");
			// 					break;
			// 				}
			// 				default:
			// 					printf("unknown FCVT_S rs2 0x%X PC 0x%X\n", rs2, PC);
			// 					exit(0);
			// 			}
			// 			break;
			// 		}

			// 		default:
			// 			printf("unknown floating point operations funct5 0x%X PC 0x%X\n", funct5, PC);
			// 			exit(0);
			// 	}
			// 	break;
			// }
				//F------------------------------------------------------
	
	
	
	


	
	
	
	
			//theoretically impossible
			default:
				printf("unknown ins opcode 0x%X PC 0x%X\n", opcode, PC);
				exit(0);
		};
	}


	return true;
}



#endif// _DECODE_H

