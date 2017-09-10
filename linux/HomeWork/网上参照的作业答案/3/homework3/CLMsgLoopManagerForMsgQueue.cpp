#include "CLMsgLoopManagerForMsgQueue.h"
#include "CLMessageQueue.h"

CLMsgLoopManagerForMsgQueue::CLMsgLoopManagerForMsgQueue(CLMessageQueue *pMsgQueue)
{
	m_pMsgQueue = pMsgQueue;
}

CLMsgLoopManagerForMsgQueue::~CLMsgLoopManagerForMsgQueue()
{
	if(m_pMsgQueue != 0)
		delete m_pMsgQueue;
}

CLStatus CLMsgLoopManagerForMsgQueue::Initialize()
{
	return CLStatus(0, 0);
}

CLStatus CLMsgLoopManagerForMsgQueue::Uninitialize()
{
	return CLStatus(0, 0);
}
	
CLMessage* CLMsgLoopManagerForMsgQueue::WaitForMessage()
{
	if(m_pMsgQueue != 0)
	{
		return m_pMsgQueue->GetMessage();
	}

	return 0;
}