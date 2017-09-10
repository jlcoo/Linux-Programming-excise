#ifndef Cache_H
#define Cache_H

#include "CLStatus.h"
#include <stdlib.h>

class CLCacher
{
public:
	CLCacher();
	~CLCacher() {};

	CLStatus OpenFile(const char *FileName);
	CLStatus ReadFile(off_t offset, int length);
	CLStatus WriteFile(const char *pstrMsg);
	CLStatus LseekFile(off_t offset);
	CLStatus CloseFile();
	CLStatus Flush();

private:
	int  _Fd;						//文件描述符
	char *_Buffer;
	unsigned int _nUsedBytesForBuffer;
};



#endif
