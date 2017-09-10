#include "CLThreadCommunicationByqueue.h"
#include "CLQueue.h"
#include "CLMessage.h"

CLThreadCommunicationByqueue::CLThreadCommunicationByqueue(CLQueue*pMsgQueue)
{
	if(pMsgQueue == 0)
		throw "In CLThreadCommunicationByqueue::CLThreadCommunicationByqueue(), pMsgQueue error";
	
	m_pMsgQueue = pMsgQueue;
}

CLThreadCommunicationByqueue::~CLThreadCommunicationByqueue()
{
	delete m_pMsgQueue;
}

CLStatus CLThreadCommunicationByqueue::PostExecutiveMessage(CLMessage *pMessage)
{
	if(pMessage == NULL)
		return CLStatus(-1, 0);
	
	return m_pMsgQueue->PushMessage(pMessage);
}