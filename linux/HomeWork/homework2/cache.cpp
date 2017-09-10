#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "cache.h"

using namespace std;

#define BUFFER_SIZE 64//缓存大小，设置为64 

CLCacher::CLCacher()
{
	_Buffer = new char[BUFFER_SIZE];
	_nUsedBytesForBuffer = 0;
}

CLStatus CLCacher::OpenFile(const char *FileName)
{
	_Fd = open(FileName, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); 
	if(-1 == _Fd){
		printf("in CLCacher::OpenFile open file error\n");
		return CLStatus(-1, 0);
	}
}

CLStatus CLCacher::Flush()
{
	if(_Fd == -1)
		return CLStatus(-1, 0);

	if(_Buffer == 0)
		return CLStatus(-1, 0);

	if(_nUsedBytesForBuffer == 0)
		return CLStatus(0, 0);
	
	ssize_t r = write(_Fd, _Buffer, _nUsedBytesForBuffer);
	if(r == -1)
		return CLStatus(-1, errno);

	_nUsedBytesForBuffer = 0;

	return CLStatus(0, 0);
}

CLStatus CLCacher::LseekFile(off_t offset)
{
	off_t NewCurrentPosition = lseek(_Fd, offset, SEEK_SET);
	if(NewCurrentPosition == -1){
		printf("lseek error\n");
		return CLStatus(-1, 0);
	}

	printf("NewCurrentPosition: %d\n", NewCurrentPosition);

	return CLStatus(0, 0);
}


CLStatus CLCacher::WriteFile(const char *pstrMsg)
{
	if(pstrMsg == 0)
		return CLStatus(-1, 0);

	if(strlen(pstrMsg) == 0)
		return CLStatus(-1, 0);

	if(_Buffer == 0)
		return CLStatus(-1, 0);

	unsigned int nleftroom = BUFFER_SIZE - _nUsedBytesForBuffer;
	unsigned int len_strmsg = strlen(pstrMsg);

	if(len_strmsg > BUFFER_SIZE){
		if(_Fd == -1)
			return CLStatus(-1, 0);

		ssize_t wb = write(_Fd, pstrMsg, len_strmsg);
		if(wb == -1)
			return CLStatus(-1, 0);

		return CLStatus(0, 0);
	}

	if(len_strmsg > nleftroom)
	{
		CLStatus s = Flush();
		if(!s.IsSuccess())
			return CLStatus(-1, 0);
	}

	memcpy(_Buffer + _nUsedBytesForBuffer, pstrMsg, len_strmsg);

	_nUsedBytesForBuffer += len_strmsg;
}

CLStatus CLCacher::ReadFile(off_t offset, int length)
{
	if(_Buffer == 0)
		return CLStatus(-1, 0);

	Flush();    //flush before read file
	LseekFile(offset);

	if(length == 0){
		printf("you want to read nothing\n");
		return CLStatus(0, 0);
	} else if(length > BUFFER_SIZE){
		if (read(_Fd, _Buffer, BUFFER_SIZE) == -1){
			printf("read error\n");
			return CLStatus(-1, 0);
		}
		printf("read a lot,now read from file.\n");//提示信息
		_nUsedBytesForBuffer = BUFFER_SIZE;
		return CLStatus(0, 0);
	} else if(length < _nUsedBytesForBuffer){
		printf("read from buffer.\n");//提示信息
		return CLStatus(0, 0);
	} else if(length > _nUsedBytesForBuffer){
		printf("read from file.\n");//提示信息
		if (read(_Fd, _Buffer, BUFFER_SIZE) == -1){
			printf("read error\n");
			return CLStatus(-1, 0);
		}
		printf("%s\n", _Buffer);
		return CLStatus(0, 0);
	}

}

CLStatus CLCacher::CloseFile()
{
	close(_Fd);
}
