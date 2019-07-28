#include<stdio.h>
#include<unistd.h>


int main(void)
{
	char i = 0;

	while(i<10)
	{
		printf("hello world,i come %d times\n", i+1);
		sleep(1);
		i++;
	}
	return 0;
}
