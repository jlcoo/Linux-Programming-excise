#ifndef CLMsgLoopManagerForMsgQueue_H
#define CLMsgLoopManagerForMsgQueue_H

#include <string>
#include "CLMessageLoopManager.h"

class CLMessageQueue;

class CLMsgLoopManagerForMsgQueue : public CLMessageLoopManager
{
public:
	/*
	pMsgQueue和pMsgObserver均应从堆中分配，且不必显示调用delete
	*/
	CLMsgLoopManagerForMsgQueue(CLMessageObserver *pMsgObserver, const char* pstrThreadName);
	virtual ~CLMsgLoopManagerForMsgQueue();

protected:
	virtual CLStatus Initialize();
	virtual CLStatus Uninitialize();
	
	virtual CLMessage* WaitForMessage();

private:
	CLMsgLoopManagerForMsgQueue(const CLMsgLoopManagerForMsgQueue&);
	CLMsgLoopManagerForMsgQueue& operator=(const CLMsgLoopManagerForMsgQueue&);

private:
	CLMessageQueue *m_pMsgQueue;
	std::string m_strThreadName;
};

#endif