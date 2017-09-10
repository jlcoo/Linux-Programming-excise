#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMessageQueue.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForMsgQueue.h"
#include "CLMessageObserver.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLExecutiveNameServer.h"
#include "CLThreadProxy.h"
#include "CLNonThreadProxy.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define ADD_MSG 0
#define QUIT_MSG 1
#define RESULT_MSG 2

#define CALCULATE_THREAD_NUM 10
#define CALCULATE_REQUEST_NUM 12

class CLAdder;
class CLControl;
class CLUser;

class CLAddMessage : public CLMessage
{
public:
    	friend class CLAdder;

	CLAddMessage(int Op1, int Op2):CLMessage(ADD_MSG)
	{
		m_Op1 = Op1;
		m_Op2 = Op2;
	}

	virtual ~CLAddMessage()
	{
	}

public:
	int m_Op1;
	int m_Op2;
};

class CLResultMessage : public CLMessage
{
public:
	friend class CLUser;
	
	CLResultMessage(int Op1, int Op2, int result) : CLMessage(RESULT_MSG)
	{
		m_Op1 = Op1;
		m_Op2 = Op2;
		m_iResult = result;
	}

	virtual ~CLResultMessage()
	{
	}

private:
	int m_Op1;
	int m_Op2;
	int m_iResult;
};

class CLQuitMessage : public CLMessage
{
public:
    	CLQuitMessage() : CLMessage(QUIT_MSG)
       {
	}

	virtual ~CLQuitMessage()
	{
	}
};

class CLAdder : public CLMessageObserver
{
public:
	CLAdder()
	{
	}

	virtual ~CLAdder()
	{
	}

	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext)
	{
		pMessageLoop->Register(ADD_MSG, (CallBackForMessageLoop)(&CLAdder::On_AddMsg));
		pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLAdder::On_QuitMsg));
		cout << "Add a CLAdder" << endl;
		return CLStatus(0, 0);
	}

	CLStatus On_AddMsg(CLMessage *pM)
	{
		CLAddMessage *pAddMsg = (CLAddMessage *)pM;		
		int i = pAddMsg->m_Op1 + pAddMsg->m_Op2;
		CLExecutiveNameServer::PostExecutiveMessage("user", new CLResultMessage(pAddMsg->m_Op1, pAddMsg->m_Op2, i));
		return CLStatus(0, 0);
	}	

	CLStatus On_QuitMsg(CLMessage *pM)
	{
		cout << "Adder quit..." << endl;		
		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}
};

class CLUser : public CLMessageObserver
{
private:
	int request_count;

public:
	CLUser() 
	{
		request_count = 0;
	}

	virtual ~CLUser()
	{
	}

	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext)
	{
		int i = 9;
		pMessageLoop->Register(RESULT_MSG, (CallBackForMessageLoop)(&CLUser::On_ResultMsg));
		pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLUser::On_QuitMsg));
		cout << "user" << endl;
		for(i = 0; i < CALCULATE_REQUEST_NUM; i++)
			CLExecutiveNameServer::PostExecutiveMessage("adder", new CLAddMessage(((int)rand())/32767, 10));
		return CLStatus(0, 0);
	}
	
	CLStatus On_ResultMsg(CLMessage *pM)
	{
		int i;
		CLResultMessage *pResultMsg = (CLResultMessage *)pM;		
		cout << pResultMsg->m_Op1 << " + " << pResultMsg->m_Op2 << " = " << pResultMsg->m_iResult << endl;
		request_count++;
		if(request_count == CALCULATE_REQUEST_NUM)
		{
			for(i = 0; i < CALCULATE_THREAD_NUM; i++)
			{
				CLExecutiveNameServer::PostExecutiveMessage("adder", new CLQuitMessage());
			}
			sleep(1);
			CLExecutiveNameServer::PostExecutiveMessage("user", new CLQuitMessage());
		}		
		return CLStatus(0, 0);
	}	

	CLStatus On_QuitMsg(CLMessage *pM)
	{
		cout << "User thread quit..." << endl;
		
		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}
};

int main()
{	
	CLThreadProxy *cal_thread[CALCULATE_THREAD_NUM];
	CLNonThreadProxy *user;
	CLMessageQueue *cal_pMsgQueue;
	CLMessageQueue *user_pMsgQueue;
	int i;
	cal_pMsgQueue = new CLMessageQueue;
	user_pMsgQueue = new CLMessageQueue;
	for (i = 0; i < CALCULATE_THREAD_NUM; i++)
	{
		cal_thread[i] = new CLThreadProxy(new CLAdder, "adder", true, cal_pMsgQueue);
		cal_thread[i]->Run(0);
	}
	
	user = new CLNonThreadProxy(new CLUser, "user", user_pMsgQueue);
	user->Run(0);
	return 0;
}
