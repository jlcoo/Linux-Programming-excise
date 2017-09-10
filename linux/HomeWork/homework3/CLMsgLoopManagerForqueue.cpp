#include <string.h>
#include "CLMsgLoopManagerForqueue.h"
#include "CLQueue.h"
#include "CLExecutiveNameServer.h"
#include "CLThreadCommunicationByqueue.h"
#include "CLLogger.h"

CLMsgLoopManagerForqueue::CLMsgLoopManagerForqueue(CLMessageObserver *pMsgObserver, const char* pstrThreadName) : CLMessageLoopManager(pMsgObserver)
{
	if((pstrThreadName == 0) || (strlen(pstrThreadName) == 0))
		throw "In CLMsgLoopManagerForqueue::CLMsgLoopManagerForqueue(), pstrThreadName error";

	m_strThreadName = pstrThreadName;

	m_pMsgQueue = new CLQueue;
}

CLMsgLoopManagerForqueue::~CLMsgLoopManagerForqueue()
{
}

CLStatus CLMsgLoopManagerForqueue::Initialize()
{
	CLExecutiveNameServer *pNameServer = CLExecutiveNameServer::GetInstance();
	if(pNameServer == 0)
	{
		delete m_pMsgQueue;
		m_pMsgQueue = 0;
		CLLogger::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Initialize(), CLExecutiveNameServer::GetInstance error", 0);
		return CLStatus(-1, 0);
	}

	CLStatus s = pNameServer->Register(m_strThreadName.c_str(), new CLThreadCommunicationByqueue(m_pMsgQueue));
	if(!s.IsSuccess())
	{
		delete m_pMsgQueue;
		m_pMsgQueue = 0;
		CLLogger::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Initialize(), pNameServer->Register error", 0);
		return CLStatus(-1, 0);
	}
	else
	{
		return CLStatus(0, 0);
	}
}

CLStatus CLMsgLoopManagerForqueue::Uninitialize()
{
	CLExecutiveNameServer *pNameServer = CLExecutiveNameServer::GetInstance();
	if(pNameServer == 0)
	{
		CLLogger::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Uninitialize(), CLExecutiveNameServer::GetInstance error", 0);
		return CLStatus(-1, 0);
	}

	return pNameServer->ReleaseCommunicationPtr(m_strThreadName.c_str());
}

CLMessage* CLMsgLoopManagerForqueue::WaitForMessage()
{
	return m_pMsgQueue->GetMessage();
}
