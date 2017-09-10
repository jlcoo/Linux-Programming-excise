#ifndef CLThreadProxy_H
#define CLThreadProxy_H

#include "CLStatus.h"

class CLMessageObserver;
class CLThread;

/*
该类用于创建一个线程，其采用的通信机制是CLMessageQueue。
*/
class CLThreadProxy
{
public:
	/*
	pMsgObserver应从堆中分配，且不必调用delete，pstrThreadName所代表的线程名称必须是唯一的
	默认情况下bWaitForDeath为false，若为true，则会在析构函数中等待新线程死亡
	*/
	CLThreadProxy(CLMessageObserver *pMsgObserver, const char *pstrThreadName);
	CLThreadProxy(CLMessageObserver *pMsgObserver, const char *pstrThreadName, bool bWaitForDeath);
	virtual ~CLThreadProxy();

	/*
	Run方法无论返回正确或出错，均只可调用一次。否则，另建一个CLThreadProxy对象
	*/
	CLStatus Run(void *pContext);
	
private:
	CLThreadProxy(const CLThreadProxy&);
	CLThreadProxy& operator=(const CLThreadProxy&);

private:
	CLThread *m_pThread;
	
	bool m_bWaitForDeath;
};

#endif