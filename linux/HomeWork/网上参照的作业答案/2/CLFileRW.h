#ifndef CLFileRW_H
#define CLFileRW_H

#include <pthread.h>
#include "CLStatus.h"


class CLFileRW
{
public:
	static CLFileRW * GetInstance();//获取文件操作对象
	static CLStatus FileWrite(const char *wMsg);//文件的写操作
	static CLStatus FileRead(char *rMsg, int rLength);//文件的读操作
	
	CLStatus FWrite(const char *wMsg);
	CLStatus FRead(char *rMsg, int rLength);
	CLStatus Flush();//刷新缓存

private:
	static void OnProcessExit();//程序退出时执行该程序
	CLStatus WriteMsgToFile(const char *wMsg);
	static pthread_mutex_t *InitializeMutex();//初始化互斥量

private:
	CLFileRW(const CLFileRW&);
	CLFileRW& operator=(const CLFileRW&);

	CLFileRW();
	~CLFileRW();

private:
	int m_Fd;//文件标识符
	pthread_mutex_t *m_pMutexForUseFile;//文件使用互斥量
	
	static CLFileRW *m_pFile;//文件操作对象的指针
	static pthread_mutex_t *m_pMutexForCreatingFile;//创建文件互斥量

private:
	char *m_pBuffer;//指向缓存
	unsigned int m_nUsedBytesForBuffer;//已使用的缓存的长度

private:
	bool m_bFlagForProcessExit;//程序退出标志
	bool m_bFlagForLastReaded;//上次操作是否是读
};

#endif
