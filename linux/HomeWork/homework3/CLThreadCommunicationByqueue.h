#ifndef CLThreadCommunicationByqueue_H
#define CLThreadCommunicationByqueue_H

#include "CLExecutiveCommunication.h"
#include "CLStatus.h"

class CLMessage;
class CLQueue;

class CLThreadCommunicationByqueue : public CLExecutiveCommunication
{
public:
	/*
	pMsgQueue必须从堆中分配，且不必显示调用delete
	*/
	CLThreadCommunicationByqueue(CLQueue *pMsgQueue);
	virtual ~CLThreadCommunicationByqueue();

	virtual CLStatus PostExecutiveMessage(CLMessage *pMessage);

private:
	CLThreadCommunicationByqueue(const CLThreadCommunicationByqueue&);
	CLThreadCommunicationByqueue& operator=(const CLThreadCommunicationByqueue&);

private:
	CLQueue *m_pMsgQueue;
};

#endif