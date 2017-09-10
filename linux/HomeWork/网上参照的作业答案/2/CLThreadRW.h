#ifndef CLTHREADRW_H
#define CLTHREADRW_H

#include "CLThread.h"
#include "CLStatus.h"
class CLReadThread:public CLThread
{
public:
	CLReadThread(int length);
	
	virtual ~CLReadThread();
	
public:
	virtual CLStatus RunThreadFunction();
	
private:
	char m_ReadContext[20];
	int rLength;
	
};
class CLWriteThread:public CLThread
{
public:
	CLWriteThread(char *wStr);
	
	virtual ~CLWriteThread();
	
public:
	virtual CLStatus RunThreadFunction();
	
private:
	char m_WriteContext[20];
};

#endif
