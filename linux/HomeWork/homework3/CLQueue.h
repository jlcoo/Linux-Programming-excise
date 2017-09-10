#ifndef CLQueue_H
#define CLueue_H

#include "CLStatus.h"
#include "CLMutex.h"
#include "CLEvent.h"

#define LENGTH 10
class CLMessage;

class CLQueue
{
public:
	CLQueue();
	virtual ~CLQueue();

public:
	CLStatus PushMessage(CLMessage * pMessage);
	CLMessage* GetMessage();	

private:
	CLStatus Push(CLMessage * pMessage);
	CLMessage* Pop();

	CLQueue(const CLQueue&);
	CLQueue& operator=(const CLQueue&);

	CLMessage *my_Queue[LENGTH];
	int start,tail,count;
	CLMutex m_Mutex;
	CLEvent m_Event;

};



#endif
