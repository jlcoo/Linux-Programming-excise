#include <stdio.h>
#include <stdlib.h>
struct line{
	int data;
	char num;
	char privaote[0];
};

int main()
{
	// char *p;
	// p = (char*) malloc(1);
	// *p = -129;
	int data = -1;
	if(data < sizeof(data)){
		// printf("%d\n", sizeof(struct line));
		printf("ok\n");
	} else {
		printf("error\n");
	}

}
