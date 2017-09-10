#include "CLThreadRW.h"
#include "CLFileRW.h"
#include <stdio.h>
#include <string.h>

CLReadThread::CLReadThread(int length)
{
		rLength=length;
}
CLReadThread::~CLReadThread()
{
}
CLStatus CLReadThread::RunThreadFunction()
{
	CLStatus s=CLFileRW::FileRead(m_ReadContext,rLength);
	if(!s.IsSuccess())
	{
		printf("read file error.\n");
		return CLStatus(-1,0);
	}
	printf("read file success.\n");//gai
	printf("%s\n",m_ReadContext);
	return CLStatus(0,0);
}
////////////////////////////////////
CLWriteThread::CLWriteThread(char *wStr)
{
	strcpy(m_WriteContext,wStr);
}
	
CLWriteThread::~CLWriteThread()
{
}
CLStatus CLWriteThread::RunThreadFunction()
{
	CLStatus s=CLFileRW::FileWrite(m_WriteContext);
	
	if(!s.IsSuccess())
	{
		printf("write file error.\n");
		return CLStatus(-1,0);
	}
	printf("write file success.\n");//gai	
	return CLStatus(0,0);
}
