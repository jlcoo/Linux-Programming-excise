#include "CLNonThreadProxy.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLMsgLoopManagerForMsgQueue.h"
#include "CLLog.h"
#include "CLThreadInitialFinishedNotifier.h"

CLNonThreadProxy::CLNonThreadProxy(CLMessageObserver *pMsgObserver, const char *pstrThreadName, CLMessageQueue *m_pMsgQueue)
{
	m_pFunctionProvider = new CLExecutiveFunctionForMsgLoop(new CLMsgLoopManagerForMsgQueue(pMsgObserver, pstrThreadName, m_pMsgQueue));
}

CLNonThreadProxy::~CLNonThreadProxy()
{
	if(m_pFunctionProvider != 0)
		delete m_pFunctionProvider;
}

CLStatus CLNonThreadProxy::Run(void *pContext)
{
	if(m_pFunctionProvider == 0)
		return CLStatus(-1, 0);

	CLThreadInitialFinishedNotifier notifier(0);		

	SLExecutiveInitialParameter para;
	para.pContext = pContext;
	para.pNotifier = &notifier;

	return m_pFunctionProvider->RunExecutiveFunction(&para);
}
