int ack(int m,int n)  
{  
    if(m == 0)  
        return n+1;  
    else if(n == 0)  
        return ack(m-1,1);  
    else  
        return ack(m-1,ack(m,n-1));  
}  
int main(void)
{
	int m = 3, n = 9;
	int res = ack(m, n);
	//printf("%d", res);
	if(res == 4093)
		return 0;
	else
		while(1);
}
