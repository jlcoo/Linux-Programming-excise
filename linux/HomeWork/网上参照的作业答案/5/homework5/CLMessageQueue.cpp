#include "CLMessageQueue.h"
#include "CLCriticalSection.h"
#include "CLMessage.h"
#include "CLLog.h"

CLMessageQueue::CLMessageQueue() : m_Event(true)
{
}

CLMessageQueue::~CLMessageQueue()
{
	while(true)
	{
		CLMessage *pMsg = Pop();
		if(pMsg == 0)
			break;

		delete pMsg;
	}
}

CLStatus CLMessageQueue::PushMessage(CLMessage * pMessage)
{
	CLStatus s = Push(pMessage);
	if(!s.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageQueue::PushMessage(), Push error", 0);
		return CLStatus(-1, 0);
	}

	CLStatus s2 = m_Event.Set();
	if(!s2.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageQueue::PushMessage(), m_Event.Set error", 0);
		return CLStatus(-1, 0);
	}
	
	return CLStatus(0, 0);
}

CLMessage* CLMessageQueue::GetMessage()
{
	CLStatus s = m_Event.Wait();
	if(!s.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageQueue::GetMessage(), m_Event.Wait error", 0);
		return 0;
	}

	return Pop();
}

CLStatus CLMessageQueue::Push(CLMessage * pMessage)
{
	try
	{
		CLCriticalSection cs(&m_Mutex);

		m_MessageQueue.push(pMessage);
	}
	catch(const char* str)
	{
		CLLog::WriteLogMsg("In CLMessageQueue::Push(), exception arise", 0);
		delete pMessage;
		return CLStatus(-1, 0);
	}

	return CLStatus(0, 0);
}

CLMessage* CLMessageQueue::Pop()
{
	try
	{
		CLCriticalSection cs(&m_Mutex);

		if(m_MessageQueue.empty())
			return 0;

		CLMessage *p = m_MessageQueue.front();
		m_MessageQueue.pop();
		return p;
	}
	catch(const char* str)
	{
		CLLog::WriteLogMsg("In CLMessageQueue::Pop(), exception arise", 0);
		return 0;
	}
}
