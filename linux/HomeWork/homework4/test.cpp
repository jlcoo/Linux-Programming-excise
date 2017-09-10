#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "CLThread.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForqueue.h"
#include "CLMessageObserver.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLExecutiveNameServer.h"
#include "CLExecutiveCommunication.h"
#include "CLThreadForMsgLoop.h"
#include "CLNonThreadForMsgLoop.h"
#include "CLThreadForMsgLoop.h"

using namespace std;

#define ADD_MSG 0
#define QUIT_MSG 1
#define RESULT_MSG 2

#define CALCULATE_THREAD_NUM 10
#define CALCULATE_REQUEST_NUM 1200

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
public:
	CLUser()
	{
		request_count = 0;
	}
	virtual ~CLUser() {}

	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext)
	{
		int i = 9;
		pMessageLoop->Register(RESULT_MSG, (CallBackForMessageLoop)(&CLUser::On_ResultMsg));
		pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLUser::On_QuitMsg));
		cout << "user" << endl;

		for(i = 0; i < CALCULATE_REQUEST_NUM; i++)
			CLExecutiveNameServer::PostExecutiveMessage("control", new CLAddMessage(i, 10));

		return CLStatus(0, 0);
	}

	CLStatus On_ResultMsg(CLMessage *pM)
	{
		int i;
		char buffer[15];
		CLResultMessage *pResultMsg = (CLResultMessage *)pM;

		cout << pResultMsg->m_Op1 << " + " << pResultMsg->m_Op2 << " = " << pResultMsg->m_iResult << endl;
		request_count++;

		if(request_count == CALCULATE_REQUEST_NUM)  //线程池的大小为10
		{
			for(i = 0; i < CALCULATE_THREAD_NUM; i++)
			{
				sprintf(buffer,"adder%d",i);
				CLExecutiveNameServer::PostExecutiveMessage(buffer, new CLQuitMessage());
			}
			CLExecutiveNameServer::PostExecutiveMessage("control", new CLQuitMessage());
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

private:
	int request_count;
};

class CLControl : public CLMessageObserver
{
private:
	int request_count;

public:
	CLControl()
	{
		request_count = 0;
	}

	virtual ~CLControl()
	{
	}

	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext)
	{
		pMessageLoop->Register(ADD_MSG, (CallBackForMessageLoop)(&CLControl::On_AddMsg));
		pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLControl::On_QuitMsg));

		cout << "control" << endl;
		return CLStatus(0, 0);
	}
	
	CLStatus On_AddMsg(CLMessage *pM)
	{
	CLAddMessage *pAddMsg = (CLAddMessage *)pM;
		char buffer[15];
		int i = request_count % CALCULATE_THREAD_NUM;
		sprintf(buffer,"adder%d",i);
		CLExecutiveNameServer::PostExecutiveMessage(buffer, new CLAddMessage(pAddMsg->m_Op1, pAddMsg->m_Op2));
		request_count++;
		return CLStatus(0, 0);
	}	

	CLStatus On_QuitMsg(CLMessage *pM)
	{
		cout << "Main control thread quit..." << endl;
		
		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}
};

int main()
{
	CLNonThreadForMsgLoop *user;
	CLThreadForMsgLoop *control;
	CLThreadForMsgLoop *cal_thread[CALCULATE_THREAD_NUM];
	int i;
	char buffer[15];

	for (i = 0; i < CALCULATE_THREAD_NUM; i++)    //çº¿ç¨‹æ± é¢„å…ˆåˆ†é…?0ä¸ªçº¿ç¨?
	{
		sprintf(buffer,"adder%d",i);
		cal_thread[i] = new CLThreadForMsgLoop(new CLAdder, buffer, true);
		cal_thread[i]->Run(0);
	}

	control = new CLThreadForMsgLoop(new CLControl, "control" ,true); //æ–°å»ºä¸€ä¸ªæŽ§åˆ¶çº¿ç¨?
	control->Run(0);

	user = new CLNonThreadForMsgLoop(new CLUser, "user");  //æ–°å»ºä¸€ä¸ªç”¨æˆ·çº¿ç¨?
	user->Run(0);

	return 0;
}
