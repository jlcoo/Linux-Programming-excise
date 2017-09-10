#include "CLThreadRW.h"
#include "unistd.h"
#include<stdio.h>

int main()
{
	char str[20]="1234567";
	char str1[20]="abcdefg";
	CLThread *wThread=new CLWriteThread(str);
	CLThread *wThread1=new CLWriteThread(str1);
	CLThread *rThread=new CLReadThread(6);
	CLThread *rThread1=new CLReadThread(9); 

	wThread->Run();
	sleep(1);//先确保文件中已有数据，之后三个操作并行执行
	wThread1->Run();
	rThread->Run();
	rThread1->Run();
	
	wThread->WaitForDeath();
	wThread1->WaitForDeath();
	rThread->WaitForDeath();
	rThread1->WaitForDeath();

	delete wThread;
	delete wThread1;
	delete rThread;
	delete rThread1;
	return 0;
}
