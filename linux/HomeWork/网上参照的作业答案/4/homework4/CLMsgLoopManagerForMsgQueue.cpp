#include "CLMsgLoopManagerForMsgQueue.h"
#include "CLMessageQueue.h"
#include "CLExecutiveNameServer.h"
#include "CLThreadCommunicationByMsgQueue.h"
#include "CLLog.h"

CLMsgLoopManagerForMsgQueue::CLMsgLoopManagerForMsgQueue(CLMessageObserver *pMsgObserver, const char* pstrThreadName) : CLMessageLoopManager(pMsgObserver)
{
	m_strThreadName = pstrThreadName;

	m_pMsgQueue = new CLMessageQueue;
}

CLMsgLoopManagerForMsgQueue::~CLMsgLoopManagerForMsgQueue()
{
}

CLStatus CLMsgLoopManagerForMsgQueue::Initialize()
{
	CLExecutiveNameServer *pNameServer = CLExecutiveNameServer::GetInstance();
	if(pNameServer == 0)
	{
		delete m_pMsgQueue;
		m_pMsgQueue = 0;
		CLLog::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Initialize(), CLExecutiveNameServer::GetInstance error", 0);
		return CLStatus(-1, 0);
	}

	CLStatus s = pNameServer->Register(m_strThreadName.c_str(), new CLThreadCommunicationByMsgQueue(m_pMsgQueue));
	if(!s.IsSuccess())
	{
		delete m_pMsgQueue;
		m_pMsgQueue = 0;
		CLLog::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Initialize(), pNameServer->Register error", 0);
		return CLStatus(-1, 0);
	}
	else
	{
		return CLStatus(0, 0);
	}
}

CLStatus CLMsgLoopManagerForMsgQueue::Uninitialize()
{
	CLExecutiveNameServer *pNameServer = CLExecutiveNameServer::GetInstance();
	if(pNameServer == 0)
	{
		CLLog::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Uninitialize(), CLExecutiveNameServer::GetInstance error", 0);
		return CLStatus(-1, 0);
	}

	return pNameServer->ReleaseCommunicationPtr(m_strThreadName.c_str());
}
	
CLMessage* CLMsgLoopManagerForMsgQueue::WaitForMessage()
{
	if(m_pMsgQueue != 0)
	{
		return m_pMsgQueue->GetMessage();
	}

	return 0;
}
