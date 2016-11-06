#include "load.h"
#include "decode.h"
#include "exec.h"
#include "params.h"

const char * filename = "hello";

using namespace std;
int main()
{
	int fd = open(filename, O_RDONLY);
	uchar * vm = new uchar[VM_SIZE];

	uchar * p_entry = load(fd, vm);
	printf("%x\n", p_entry);

	bool flag = decode(vm, p_entry);
	
	if(flag)
		printf("\nSuccess!\n");
	else
		printf("\nSomething bad happens!\n");

	close(fd);
	return 0;
}	