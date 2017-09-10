#ifndef CLNonThreadProxy_H
#define CLNonThreadProxy_H

#include "CLStatus.h"

class CLMessageObserver;
class CLExecutiveFunctionProvider;

/*
该类用于让线程直接进入消息循环，而不是创建新线程
*/
class CLNonThreadProxy
{
public:
	/*
	pMsgObserver应从堆中分配，且不必调用delete，pstrThreadName所代表的线程名称必须是唯一的
	*/
	CLNonThreadProxy(CLMessageObserver *pMsgObserver, const char *pstrThreadName);
	virtual ~CLNonThreadProxy();

	CLStatus Run(void *pContext);

private:
	CLNonThreadProxy(const CLNonThreadProxy&);
	CLNonThreadProxy& operator=(const CLNonThreadProxy&);

private:
	CLExecutiveFunctionProvider *m_pFunctionProvider;
};

#endif