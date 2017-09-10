#include "CLCriticalSection.h"
#include "CLMutex.h"
#include "CLLog.h"
#include "CLStatus.h"

CLCriticalSection::CLCriticalSection(CLMutex *pMutex)
{
	if(pMutex == 0)
	{
		CLLog::WriteLogMsg("In CLCriticalSection::CLCriticalSection(), pMutex == 0", 0);
		throw "error";
	}
	
	m_pMutex = pMutex;

	CLStatus s = m_pMutex->Lock();
	if(!s.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLCriticalSection::CLCriticalSection(), m_pMutex->Lock error", 0);
		throw "error";
	}
}

CLCriticalSection::~CLCriticalSection()
{
	if(m_pMutex != 0)
	{
		CLStatus s = m_pMutex->Unlock();
		if(!s.IsSuccess())
		{
			CLLog::WriteLogMsg("In CLCriticalSection::~CLCriticalSection(), m_pMutex->Unlock error", 0);
			throw "error";
		}
	}
}

