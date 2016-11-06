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

bool decode(uchar * p_entry)
{
	// char buff[100];
	reg[0] = 0;
	regl[0] = 0;

	uint INS;
	uint * p_ins;
	PC = (ulint)p_entry;


	bool exit = false;

	while(!exit)
	{
		p_ins = (uint *)(vm + PC);
		INS = *p_ins;
	
		uint opcode = INS & ((1 << 7) - 1);

		switch(opcode)
		{
		//LUI
		case 0b0110111:
			uint rd = get_part(7, 11, INS);
			uint imm = get_part(12, 31, INS);
			bool flag = LUI(rd, imm);
			if(!flag)
			{
				printf("error in LUI\n");
				exit(0);
			}
			break;


		//AUIPC
		case 0b0010111:
			uint rd = get_part(7, 11, INS);
			uint imm = get_part(12, 31, INS);
			bool flag = AUIPC(rd, imm);
			if(!flag)
			{
				printf("error in AUIPC\n");
				exit(0);	
			}
			break;



		//JAL
		case 0b1101111:
	
			break;




		//JALR
		case 0b1100111:
			break;
	


		default:
			break;
		};
	}


	return true;
}



#endif// _DECODE_H

