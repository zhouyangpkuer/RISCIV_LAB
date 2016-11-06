#define mat_len 300
int A[mat_len][mat_len];
int B[mat_len][mat_len];
int C[mat_len][mat_len];
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
    return 0;
}