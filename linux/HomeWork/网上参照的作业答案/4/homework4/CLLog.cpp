#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "CLLog.h"

#define LOG_FILE_NAME "CLLog.txt"
#define MAX_SIZE 265

CLLog* volatile CLLog::m_pLog = 0;
pthread_mutex_t *CLLog::m_pMutex = CLLog::InitializeMutex();

CLLog::CLLog()
{
	m_Fd = open(LOG_FILE_NAME, O_RDWR | O_CREAT | O_APPEND, S_IRUSR); 

	m_pMutexForFile = new pthread_mutex_t;
	
	int r = pthread_mutex_init(m_pMutexForFile, 0);
	if(r != 0)
	{
		delete m_pMutexForFile;
		m_pMutexForFile = 0;
	}
}

CLLog::~CLLog()
{
	if(m_pMutexForFile != 0)
		delete m_pMutexForFile;
	
	if(m_Fd != -1)
		close(m_Fd);
	
	if(m_pLog != 0)
		delete m_pLog;

	if(m_pMutex != 0)
		delete m_pMutex;
}

CLStatus CLLog::WriteLogMsg(const char *pstrMsg, long lErrorCode)
{
	CLLog *pLog = CLLog::GetInstance();
	if(pLog == 0)
		return CLStatus(-1, 0);
	
	CLStatus s = pLog->WriteLog(pstrMsg, lErrorCode);
	if(s.IsSuccess())
		return CLStatus(0, 0);
	else
		return CLStatus(-1, 0);
}

CLStatus CLLog::WriteLog(const char *pstrMsg, long lErrorCode)
{
	if(m_Fd == -1)
		return CLStatus(-1, 0);

	if(m_pMutexForFile == 0)
		return CLStatus(-1, 0);
		
	int p = pthread_mutex_lock(m_pMutexForFile);
	if(p != 0)
		return CLStatus(-1, 0);
		
	ssize_t r = write(m_Fd, pstrMsg, strlen(pstrMsg));
	if(r == -1)
	{
		pthread_mutex_unlock(m_pMutexForFile);
		return CLStatus(-1, 0);
	}

	char buf[MAX_SIZE];
	snprintf(buf, MAX_SIZE, "	Error code: %ld\r\n",  lErrorCode);

	r = write(m_Fd, buf, strlen(buf));
	if(r == -1)
	{
		pthread_mutex_unlock(m_pMutexForFile);
		return CLStatus(-1, 0);
	}

	p = pthread_mutex_unlock(m_pMutexForFile);
	if(p != 0)
		return CLStatus(-1, 0);

	return CLStatus(0, 0);
}

pthread_mutex_t *CLLog::InitializeMutex()
{
	pthread_mutex_t *p = new pthread_mutex_t;

	int r = pthread_mutex_init(p, 0);
	if(r != 0)
	{
		delete p;
		return 0;
	}
	
	return p;
}

CLLog* CLLog::GetInstance()
{
	if(m_pLog == 0)
	{
		if(m_pMutex == 0)
			return 0;
		
		int r = pthread_mutex_lock(m_pMutex);
		if(r != 0)
			return 0;
		
		if(m_pLog == 0)
		{
			m_pLog = new CLLog;
		}

		r = pthread_mutex_unlock(m_pMutex);
		if(r != 0)
			return 0;
	}

	return m_pLog;
}
