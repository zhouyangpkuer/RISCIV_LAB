#include <iostream>
using namespace std;
typedef unsigned int uint;
int main()
{
	uint imm = 0b111111111111;
	int temp = (int)imm;
	cout << ((temp << 20) >> 20) << endl;
	cout << (((int)imm << 20) >> 20) << endl;
	return 0;
}