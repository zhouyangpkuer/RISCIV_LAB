#include "load.h"
#include "decode.h"
#include "exec.h"
#include "params.h"

const char * filename = "./dhry2.1/dry2";
// const char * filename = "hello";
// const char * filename = "ack";

using namespace std;
int main()
{
	int fd = open(filename, O_RDONLY);
	vm = new uchar[VM_SIZE];
	// for(int i = 0; i < VM_SIZE; i++)
	// 	vm[i] = 0;

	uchar * p_entry = load(fd);
	// printf("%x\n", p_entry);


	bool flag = decode(p_entry);

	
	if(flag)
		printf("\nSuccess!\n");
	else
		printf("\nSomething bad happens!\n");

	close(fd);
	delete[] vm;
	return 0;
}	