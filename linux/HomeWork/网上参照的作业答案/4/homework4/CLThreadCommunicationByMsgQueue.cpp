#include "CLThreadCommunicationByMsgQueue.h"
#include "CLMessageQueue.h"
#include "CLMessage.h"

CLThreadCommunicationByMsgQueue::CLThreadCommunicationByMsgQueue(CLMessageQueue *pMsgQueue)
{
	m_pMsgQueue = pMsgQueue;
}

CLThreadCommunicationByMsgQueue::~CLThreadCommunicationByMsgQueue()
{
	if(m_pMsgQueue != 0)
		delete m_pMsgQueue;
}

CLStatus CLThreadCommunicationByMsgQueue::PostExecutiveMessage(CLMessage *pMessage)
{
	if(m_pMsgQueue == 0)
	{
		delete pMessage;
		return CLStatus(-1, 0);
	}

	return m_pMsgQueue->PushMessage(pMessage);
}