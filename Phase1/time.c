#include <time.h>
#include <stdio.h>

int main()
{
	int t = time(0); //syscall for getting the system current time 
	printf ("%s", ctime(&t)); //converting format to standard
	return 0;
}

