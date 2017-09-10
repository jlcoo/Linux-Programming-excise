#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLMessageLoopManager.h"

CLExecutiveFunctionForMsgLoop::CLExecutiveFunctionForMsgLoop(CLMessageLoopManager *pMsgLoopManager)
{
	m_pMsgLoopManager = pMsgLoopManager;
}

CLExecutiveFunctionForMsgLoop::~CLExecutiveFunctionForMsgLoop()
{
	if(m_pMsgLoopManager != 0)
		delete m_pMsgLoopManager;
}

CLStatus CLExecutiveFunctionForMsgLoop::RunExecutiveFunction(void* pContext)
{
	if(m_pMsgLoopManager != 0)
		return m_pMsgLoopManager->EnterMessageLoop(pContext);
	
	return CLStatus(-1, 0);
}
