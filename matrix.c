#define mat_len 3
int A[mat_len][mat_len];
int B[mat_len][mat_len];
int C[mat_len][mat_len];
int D[mat_len][mat_len] = 
{
	{0, 0, 0}, 
	{0, 5, 10}, 
	{0, 10, 20} 
};
void func()
{
	for(int i = 0; i < mat_len; i++)
	{
		for(int j = 0; j < mat_len; j++)
		{
			int temp = 0;
			for(int k = 0; k < mat_len; k++)
			{
				temp += A[i][k] * B[k][j];
			}
			C[i][j] = temp;
		}
	}
}

int main(void)
{
    for(int i = 0; i < mat_len; i++)
    {
    	for(int j = 0; j < mat_len; j++)
    	{
    		A[i][j] = i * j % 1007;
    		B[i][j] = i * j % 1017;	
    	}
    }
    func();
    int flag = 1;
   	for(int i = 0; i < mat_len; i++)
   	{
   		for(int j = 0; j < mat_len; j++)
   		{
   			if(C[i][j] != D[i][j])
   				flag = 0;
   		}
   	}
   	if(flag == 1)
    	return 0;
    while(1);
}