#ifndef CLMsgLoopManagerForMsgQueue_H
#define CLMsgLoopManagerForMsgQueue_H

#include "CLMessageLoopManager.h"

class CLMessageQueue;

class CLMsgLoopManagerForMsgQueue : public CLMessageLoopManager
{
public:
	/*
	pMsgQueue应从堆中分配，且不必显示调用delete
	*/
	CLMsgLoopManagerForMsgQueue(CLMessageQueue *pMsgQueue);
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
};

#endif