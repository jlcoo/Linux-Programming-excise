#include "CLThreadProxy.h"
#include "CLThread.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLMsgLoopManagerForMsgQueue.h"
#include "CLLog.h"
#include "CLThreadInitialFinishedNotifier.h"
#include "CLEvent.h"

CLThreadProxy::CLThreadProxy(CLMessageObserver *pMsgObserver, const char *pstrThreadName)
{
	m_bWaitForDeath = false;
	
	m_pThread = new CLThread(new CLExecutiveFunctionForMsgLoop(new CLMsgLoopManagerForMsgQueue(pMsgObserver, pstrThreadName)));
}

CLThreadProxy::CLThreadProxy(CLMessageObserver *pMsgObserver, const char *pstrThreadName, bool bWaitForDeath)
{
	m_bWaitForDeath = bWaitForDeath;

	m_pThread = new CLThread(new CLExecutiveFunctionForMsgLoop(new CLMsgLoopManagerForMsgQueue(pMsgObserver, pstrThreadName)), bWaitForDeath);
}

CLThreadProxy::~CLThreadProxy()
{
	if((m_bWaitForDeath) && (m_pThread != 0))
	{
		CLStatus s = m_pThread->WaitForDeath();
		if(!s.IsSuccess())
			CLLog::WriteLogMsg("In CLThreadProxy::~CLThreadProxy(), m_pThread->WaitForDeath error", 0);
	}
}
	
CLStatus CLThreadProxy::Run(void *pContext)
{
	if(m_pThread == 0)
		return CLStatus(-1, 0);
	
	CLEvent event;
	CLThreadInitialFinishedNotifier notifier(&event);
		
	SLExecutiveInitialParameter para;
	para.pContext = pContext;
	para.pNotifier = &notifier;

	CLStatus s = m_pThread->Run(&para);
	if(!s.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLThreadProxy::Run(), m_pThread->Run error", 0);
		m_bWaitForDeath = false;
		m_pThread = 0;
		return CLStatus(-1, 0);
	}

	CLStatus s1 = event.Wait();
	if(!s1.IsSuccess())
		CLLog::WriteLogMsg("In CLThreadProxy::Run(), event.Wait error", 0);

	if(notifier.IsInitialSuccess())
		return CLStatus(0, 0);
	else
		return CLStatus(-1, 0);
}
