#include <params.h>

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
void dubug_before(int n, const char * str)
{
	switch(n)
	{
		//rd + PC
		case 0:
			print_type(str);
			print_reg(rd);
			print_pc();
			printf("\n");
			break;
		//rd + rs1 + PC
		case 1:
			print_type(str);
			print_reg(rd);
			print_reg(rs1);
			print_pc();
			printf("\n");
			break;
		//rs1 + rs2 + PC
		case 2:
			print_type(str);
			print_reg(rs1);
			print_reg(rs2);
			print_pc();
			printf("\n");
			break;
	}
}
void dubug_after(int n)
{
	switch(n)
	{
		//rd + PC
		case 0:
			print_reg(rd);
			print_pc();
			printf("\n\n");
			break;
		//rd + rs1 + PC
		case 1:
			print_reg(rd);
			print_reg(rs1);
			print_pc();
			printf("\n\n");
			break;
		//rs1 + rs2 + PC
		case 2:
			print_reg(rs1);
			print_reg(rs2);
			print_pc();
			printf("\n\n");
			break;
	}
}