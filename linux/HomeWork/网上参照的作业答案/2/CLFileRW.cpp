#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "CLFileRW.h"

#define LOG_FILE_NAME "a.txt"
#define BUFFER_SIZE 4096

CLFileRW* CLFileRW::m_pFile = 0;
pthread_mutex_t *CLFileRW::m_pMutexForCreatingFile = CLFileRW::InitializeMutex();

pthread_mutex_t *CLFileRW::InitializeMutex()
{
	pthread_mutex_t *p = new pthread_mutex_t;

	if(pthread_mutex_init(p, 0) != 0)
	{
		delete p;
		return 0;
	}

	return p;
}

CLFileRW::CLFileRW()
{
	m_Fd = open(LOG_FILE_NAME, O_RDWR | O_CREAT|O_APPEND, S_IRUSR | S_IWUSR); 
	if(m_Fd == -1)
		throw "In CLFileRW::CLFileRW(), open error";

	m_pBuffer = new char[BUFFER_SIZE];//创建缓存区
	m_nUsedBytesForBuffer = 0;

	m_bFlagForProcessExit = false;
	m_bFlagForLastReaded=false;
	m_pMutexForUseFile = new pthread_mutex_t;
	if (pthread_mutex_init(m_pMutexForUseFile, 0) != 0)//初始化文件使用互斥量
	{
		delete m_pMutexForUseFile;
		delete [] m_pBuffer;
		close(m_Fd);

		throw "In CLFileRW::CLFileWR(), pthread_mutex_init error";
	}
}

CLStatus CLFileRW::FileWrite(const char *wMsg)
{
	CLFileRW *pFile = CLFileRW::GetInstance();//获取文件操作对象
	if(pFile == 0)
		return CLStatus(-1, 0);
	
	CLStatus s = pFile->FWrite(wMsg);
	if(s.IsSuccess())
		return CLStatus(0, 0);
	else
		return CLStatus(-1, 0);
}




CLStatus CLFileRW::FWrite(const char *wMsg)
{
	
	if(wMsg == 0)
		return CLStatus(-1, 0);
	int len_strmsg = strlen(wMsg);

	if (len_strmsg == 0)
		return CLStatus(-1, 0);


	if (pthread_mutex_lock(m_pMutexForUseFile) != 0)//向文件中写数据时，对文件进行加锁
		return CLStatus(-1, 0);
	if(m_bFlagForLastReaded)//如果上次操作为读，则清空缓存
	{
		
		m_nUsedBytesForBuffer=0;
	}
	m_bFlagForLastReaded=false;//修改读标志，为下次读写操作做准备
	try
	{
		if ((len_strmsg> BUFFER_SIZE) || (m_bFlagForProcessExit))//如果要写入的字符串长度大于缓存长度或程序已退出，则直接                                                                         //向文件中写入数据
			throw WriteMsgToFile(wMsg);

		unsigned int nleftroom = BUFFER_SIZE - m_nUsedBytesForBuffer;
		if (len_strmsg > nleftroom)//如果要写入的字符串长度大于剩余缓存长度，则先把缓存中已有的数据写入文件中,再将需要写的					          //字符串写入缓存
		{
			lseek(m_Fd,0,SEEK_END);
			if(write(m_Fd,m_pBuffer, m_nUsedBytesForBuffer) == -1)
				throw CLStatus(-1, errno);

			m_nUsedBytesForBuffer = 0;
		}

		memcpy(m_pBuffer + m_nUsedBytesForBuffer, wMsg, len_strmsg);//将需要写的字符串写入缓存		
		m_nUsedBytesForBuffer += len_strmsg;
		throw CLStatus(0, 0);
	}
	catch (CLStatus& s)
	{
		if (pthread_mutex_unlock(m_pMutexForUseFile) != 0)//写操作结束，解锁
			return CLStatus(-1, 0);

		return s;
	}
}

CLStatus CLFileRW::WriteMsgToFile(const char *wMsg)
{
	lseek(m_Fd,0,SEEK_END);
	if(write(m_Fd, wMsg, strlen(wMsg)) == -1)
		return CLStatus(-1, errno);

	return CLStatus(0, 0);
}
CLStatus CLFileRW::Flush()
{
	if(m_bFlagForLastReaded)//如果上次操作是读，则不能刷新缓存
	{
		return CLStatus(0, 0);
	}
	
	if (pthread_mutex_lock(m_pMutexForUseFile) != 0)
		return CLStatus(-1, 0);

	try
	{
	        if(m_nUsedBytesForBuffer == 0)//缓存数据长度为0
		    throw CLStatus(0, 0);
		lseek(m_Fd,0,SEEK_END);
		if(write(m_Fd, m_pBuffer, m_nUsedBytesForBuffer) == -1)
			throw CLStatus(-1, errno);

		m_nUsedBytesForBuffer = 0;
		printf("flush the buffer.\n");//提示信息

		throw CLStatus(0, 0);
	}
	catch(CLStatus &s)
	{
		if (pthread_mutex_unlock(m_pMutexForUseFile) != 0)
			return CLStatus(-1, 0);
		
	    return s;
	}
}

CLFileRW* CLFileRW::GetInstance()//只创建一次文件操作对象
{
	if(m_pFile != 0)
		return m_pFile;
	
	if(m_pMutexForCreatingFile == 0)
		return 0;

	if(pthread_mutex_lock(m_pMutexForCreatingFile) != 0)
		return 0;

	if(m_pFile == 0)
	{
		try
		{
			m_pFile = new CLFileRW;
		}
		catch(const char*)
		{
			pthread_mutex_unlock(m_pMutexForCreatingFile);
			return 0;
		}
		
		if (atexit(CLFileRW::OnProcessExit) != 0)
		{
			m_pFile->m_bFlagForProcessExit = true;

			if(pthread_mutex_unlock(m_pMutexForCreatingFile) != 0)
				return 0;
		}
		else
			if(pthread_mutex_unlock(m_pMutexForCreatingFile) != 0)
				return 0;

		return m_pFile;
	}

	if(pthread_mutex_unlock(m_pMutexForCreatingFile) != 0)
		return 0;

	return m_pFile;
}

void CLFileRW::OnProcessExit()//程序退出时，需要自动刷新缓存
{
	CLFileRW *pFile = CLFileRW::GetInstance();
	if (pFile != 0)
	{
		pthread_mutex_lock(pFile->m_pMutexForUseFile);
		pFile->m_bFlagForProcessExit = true;
		pthread_mutex_unlock(pFile->m_pMutexForUseFile);

		pFile->Flush();
	}
}
CLStatus CLFileRW::FileRead(char *rMsg, int rLength)
{
	CLFileRW *pFile = CLFileRW::GetInstance();//获取文件操作对象
	if(pFile == 0)
		return CLStatus(-1, 0);
	
	if (rMsg==0)
	{
		return CLStatus(-1, 0);
	}
	if (rLength == 0)
	{
		return CLStatus(0, 0);
	}
	
	CLStatus s = pFile->FRead(rMsg, rLength);
	if (s.IsSuccess())
		return CLStatus(0, 0);
	else
		return CLStatus(-1, 0);
	

}


CLStatus CLFileRW::FRead(char *rMsg, int rLength)
{
	
	if(rLength==0)
		return CLStatus(0,0);
	if (pthread_mutex_lock(m_pMutexForUseFile) != 0)//向从文件中读数时，对文件进行加锁
		return CLStatus(-1, 0);
	if(!m_bFlagForLastReaded)//如果上次操作为写，则为了保证写入和读出的一致性，需要刷新缓存。这里没有调用Flush()，避免产生死锁
	{
		printf("writed,now flush.\n");//提示信息
		lseek(m_Fd,0,SEEK_END);		
		if(write(m_Fd,m_pBuffer, m_nUsedBytesForBuffer) == -1)
			throw CLStatus(-1, errno);
		m_nUsedBytesForBuffer = 0;		
		
	}
	m_bFlagForLastReaded=true;
	try
	{
		if (rLength > BUFFER_SIZE)//如果读取长度大于缓存长度，则直接从文件中读出内容，再将读出的内容填满缓存
          			
		{
			lseek(m_Fd,0,SEEK_SET);
			if (read(m_Fd, rMsg, rLength) == -1)
				throw CLStatus(-1,0);
			memcpy(m_pBuffer, rMsg, BUFFER_SIZE);
			m_nUsedBytesForBuffer = BUFFER_SIZE;
			printf("read a lot,now read from file.\n");//提示信息
			throw CLStatus(0, 0);
		}
		if (rLength<m_nUsedBytesForBuffer)//如果读取长度小于缓存中已有数据的长度，则从缓存中读数
		{
			memcpy(rMsg, m_pBuffer, rLength);
			printf("read from buffer.\n");//提示信息
			throw CLStatus(0, 0);
		}
		if (rLength>m_nUsedBytesForBuffer)//如果读取长度大于缓存中已有数据的长度，则从文件中读出内容，再将读出的内容放入缓						  //存
		{
			lseek(m_Fd,0,SEEK_SET);
			if (read(m_Fd, rMsg, rLength) == -1)	
				throw CLStatus(-1, 0);
			memcpy(m_pBuffer, rMsg, rLength);
			m_nUsedBytesForBuffer = rLength;
			printf("read from file.\n");//提示信息
			throw CLStatus(0, 0);
			
			
		}
		
	}
	catch (CLStatus& s)
	{
		if (pthread_mutex_unlock(m_pMutexForUseFile) != 0)//读操作结束，解锁
			return CLStatus(-1, 0);

		return s;
	}
	
}
