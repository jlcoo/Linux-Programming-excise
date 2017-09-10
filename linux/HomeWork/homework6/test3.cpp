#include <stdio.h>
struct st_t
{
	int status;
	short *pdata;
	char errstr[32];
};
st_t st[16];
int main(int argc, char const *argv[])
{
	char *p = (char*)(st[2].errstr+32);
	printf("%d\n", (p-(char*)(st)));

	for(int i = 10, j = 1; i = j = 0; i--,j+=2){
		printf("hello\n");
	}
	
	return 0;
}