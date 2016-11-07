#ifndef _DECODE_H
#define _DECODE_H
#include "params.h"
#include "exec.h"
using namespace std;

//include the end bit
//[start, end]
uint get_part(int start, int end, uint INS)
{
	INS >>= start;
	INS &= ((1 << (end - start + 1)) - 1);
	return INS;
}

void warning(bool flag, const char * type)
{
	if(!flag)
	{
		string str = "error in ";
		str.append(type);
		char buff[200];
		strcpy(buff, str.c_str());
		printf("%s\n", buff);
		exit(0);
	}
}
bool decode(uchar * p_entry)
{
	// char buff[100];
	// reg[0] = 0;
	reg[0] = 0;
	reg[14] = SP;

	uint INS;
	uint * p_ins;
	PC = (ulint)p_entry;


	bool EXIT = false;
	uint rd, rs1, rs2, imm, opcode, funct3, funct7, shamt;
	bool flag;

	while(!EXIT)
	{
		p_ins = (uint *)(vm + PC);
		INS = *p_ins;
	
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
			case 0b1100001:
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
						printf("unknown conditional branch funct3 %x\n", funct3);
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


					//LD
					case 0b011:



					//theoretically impossible 
					default:
						printf("unknown load funct3 %x\n", funct3);
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



					default:
						printf("unknown store funct3 %x\n", funct3);
						exit(0);
				};
				break;
			//ALU
			case 0b0010011:
				rs1 = get_part(15, 19, INS);
				rd = get_part(7, 11, INS);
				funct3 = get_part(12, 14, INS);
				imm = get_part(20, 31, INS);
				shamt = get_part(20, 24, INS);
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
						funct7 = get_part(25, 31, INS);
						//SRLI
						if(funct7 == 0b0000000)
						{
							flag = SRLI(rs1, rd, shamt);
							warning(flag, "SRLI");
						}
						//SRAI
						else if(funct7 == 0b0100000)
						{
							flag = SRAI(rs1, rd, shamt);
							warning(flag, "SRAI");
						}
						else
						{
							printf("unknown SRXI funct7 %x\n", funct7);
							exit(0);
						}
						break;

					default:
					 	printf("unknown ALU funct3 %x\n", funct3);
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
						else
						{
							printf("unknown ADD SUB funct7 %x\n", funct7);
							exit(0);	
						}
						break;
					//SLL
					case 0b001:
						flag = SLL(rs1, rs2, rd);
						warning(flag, "SLL");
						break;
					//SLT
					case 0b010:
						flag = SLT(rs1, rs2, rd);
						warning(flag, "SLT");
						break;
					
					//SLTU
					case 0b011:
						flag = SLTU(rs1, rs2, rd);
						warning(flag, "SLTU");
						break;
					
					//XOR
					case 0b100:
						flag = XOR(rs1, rs2, rd);
						warning(flag, "XOR");
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
							warning(flag, "ADD");
						}
						else
						{
							printf("unknown SRL A funct7 %x\n", funct7);
							exit(0);
						}
						break;
					//OR
					case 0b110:
						flag = OR(rs1, rs2, rd);
						warning(flag, "OR");
						break;
					
					//AND
					case 0b111:
						flag = AND(rs1, rs2, rd);
						warning(flag, "AND");
						break;
					
					default:
						printf("unknown ALU2 funct3 %x\n", funct3);
						exit(0);
				};
			//FENCE, FENCE.I
			case 0b0001111:
			

			//SCALL, SBREAK, RD
			case 0b1110011:


			//LWU, LD contained before;
		
			
			//SD contained before;


			//SLLI, SRLI, SRAI contained before;


			//ALUW
			case 0b0011011:


			//ALUW2
			case 0b0111011:



	
	
	
	
	
	
	
			//theoretically impossible
			default:
				printf("unknown ins opcode %x\n", opcode);
				exit(0);
		};
	}


	return true;
}



#endif// _DECODE_H

