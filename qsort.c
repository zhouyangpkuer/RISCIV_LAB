#define vec_len 10000
int vec[vec_len];
void myqsort(int *a, int left, int right)
{
    if(left >= right)
    {
        return;
    }
    int i = left;
    int j = right;
    int key = a[left];
    
    while(i < j)
    {
        while(i < j && key <= a[j])
        {
            j--;
        }
        a[i] = a[j];
        while(i < j && key >= a[i])
        {
            i++;
        }
        a[j] = a[i];
    }
    
    a[i] = key;
    myqsort(a, left, i - 1);
    myqsort(a, i + 1, right);
}
int main(void)
{
    for(int i = 0; i < vec_len; i++)
    {
        vec[i] = i * i % 100007;
    }
    myqsort(vec, 0, vec_len - 1);
    return 0;
}