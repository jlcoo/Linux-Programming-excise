#include "CLQueue.h"
#include "CLCriticalSection.h"
#include "CLMessage.h"
#include "CLLogger.h"

CLQueue::CLQueue() : m_Event(true)
{
	start = 0;
	tail = 0;
	count = 0;
}

CLQueue::~CLQueue()
{
	while(true)
	{
		CLMessage *pMsg = Pop();
		if(pMsg == 0)
			break;

		delete pMsg;
	}
}

CLStatus CLQueue::PushMessage(CLMessage * pMessage)
{
	if(pMessage == NULL)
		return CLStatus(-1, 0);
	
	CLStatus s = Push(pMessage);
	if(!s.IsSuccess())
	{
		CLLogger::WriteLogMsg("In CLQueue::PushMessage(), Push error", 0);
		return CLStatus(-1, 0);
	}

	CLStatus s2 = m_Event.Set();
	if(!s2.IsSuccess())
	{
		CLLogger::WriteLogMsg("In CLQueue::PushMessage(), m_Event.Set error", 0);
		return CLStatus(-1, 0);
	}
	
	return CLStatus(0, 0);
}

CLMessage* CLQueue::GetMessage()
{
	CLStatus s = m_Event.Wait();
	if(!s.IsSuccess())
	{
		CLLogger::WriteLogMsg("In CLQueue::GetMessage(), m_Event.Wait error", 0);
		return 0;
	}

	return Pop();
}

CLStatus CLQueue::Push(CLMessage * pMessage)
{
	try
	{
		CLCriticalSection cs(&m_Mutex);

		if(count == LENGTH){
			CLLogger::WriteLogMsg("In CLQueue::Push(), Queue overflow", 0);
			return CLStatus(-1, 0);
		}
		my_Queue[tail] = pMessage;
		count++;
		tail = (tail + 1) % LENGTH;  //控制溢出
	}
	catch(const char* str)
	{
		CLLogger::WriteLogMsg("In CLQueue::Push(), exception arise", 0);
		delete pMessage;
		return CLStatus(-1, 0);
	}

	return CLStatus(0, 0);
}

CLMessage* CLQueue::Pop()
{
	CLMessage *p;
	try
	{
		CLCriticalSection cs(&m_Mutex);
		if(count == 0)
		{
			return 0;
		}
		p = my_Queue[start];
		count--;
		start = (start + 1) % LENGTH;
		return p;
	}
	catch(const char* str)
	{
		CLLogger::WriteLogMsg("In CLQueue::Pop(), exception arise", 0);
		return 0;
	}
}
