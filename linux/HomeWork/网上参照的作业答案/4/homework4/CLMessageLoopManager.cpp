#include "CLMessageLoopManager.h"
#include "CLMessageObserver.h"
#include "CLMessage.h"
#include "CLLog.h"
#include "CLExecutiveInitialFinishedNotifier.h"

CLMessageLoopManager::CLMessageLoopManager(CLMessageObserver *pMessageObserver)
{
	m_pMessageObserver = pMessageObserver;
}

CLMessageLoopManager::~CLMessageLoopManager()
{
	if(m_pMessageObserver != 0)
		delete m_pMessageObserver;
}

CLStatus CLMessageLoopManager::Register(unsigned long lMsgID, CallBackForMessageLoop pMsgProcessFunction)
{
	m_MsgMappingTable[lMsgID] = pMsgProcessFunction;

	return CLStatus(0, 0);
}

CLStatus CLMessageLoopManager::EnterMessageLoop(void *pContext)
{
	SLExecutiveInitialParameter *para = (SLExecutiveInitialParameter *)pContext;
	
	if(m_pMessageObserver == 0)
	{
		CLLog::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), m_pMessageObserver == 0", 0);
		para->pNotifier->NotifyInitialFinished(false);
		return CLStatus(-1, 0);
	}

	CLStatus s = Initialize();
	if(!s.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), Initialize error", 0);
		para->pNotifier->NotifyInitialFinished(false);
		return CLStatus(-1, 0);
	}

	CLStatus s1 = m_pMessageObserver->Initialize(this, para->pContext);
	if(!s1.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), m_pMessageObserver->Initialize error", 0);

		CLStatus s2 = Uninitialize();
		if(!s2.IsSuccess())
			CLLog::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), Uninitialize() error", 0);	

		para->pNotifier->NotifyInitialFinished(false);		
		return CLStatus(-1, 0);
	}

	para->pNotifier->NotifyInitialFinished(true);
	
	while(true)
	{
		CLMessage *pMsg = WaitForMessage();
		if(pMsg == 0)
		{
			CLLog::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), pMsg == 0", 0);
			continue;
		}
		
		CLStatus s3 = MessageDispatch(pMsg);

		delete pMsg;

		if(s3.m_clReturnCode == QUIT_MESSAGE_LOOP)
			break;
	}

	CLStatus s4 = Uninitialize();
	if(!s4.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), Uninitialize() error", 0);
		return CLStatus(-1, 0);
	}

	return CLStatus(0, 0);
}

CLStatus CLMessageLoopManager::MessageDispatch(CLMessage *pMessage)
{
	std::map<unsigned long, CallBackForMessageLoop>::iterator it;
	it = m_MsgMappingTable.find(pMessage->m_clMsgID);

	if(it == m_MsgMappingTable.end())
	{
		CLLog::WriteLogMsg("In CLMessageLoopManager::MessageDispatch(), it == m_MsgMappingTable.end", 0);
		return CLStatus(-1, 0);
	}
	
	CallBackForMessageLoop pFunction = it->second;

	if(pFunction != 0)
		return (m_pMessageObserver->*pFunction)(pMessage);
	else
	{
		CLLog::WriteLogMsg("In CLMessageLoopManager::MessageDispatch(), pFunction==0", 0);
		return CLStatus(-1, 0);
	}
}