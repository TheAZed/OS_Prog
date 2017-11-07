#include <time.h>
#include <stdio.h>

int main()
{
	int t = time(0);
	printf ("%s", ctime(&t));
	return 0;
}

