#include <iostream>

class CLFatherWordCountObserver : public CLMessageObserver
{
public:

	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext)
	{
		pMessageLoop->Register(CHILD_INIT_MESSAGE_ID, (CallBackForMessageLoop)(&CLFatherWordCountObserver::On_Child_Init));
		pMessageLoop->Register(CHILD_SEND_REQUEST_MESSAGE_ID, (CallBackForMessageLoop)(&CLFatherWordCountObserver::On_Child_Send_Request));
		pMessageLoop->Register(INTERMEDIATE_RESULT_MESSAGE_ID, (CallBackForMessageLoop)(&CLFatherWordCountObserver::On_Intermediate_Result));
		pMessageLoop->Register(CHILD_WORK_FINISH_MESSAGE_ID, (CallBackForMessageLoop)(&CLFatherWordCountObserver::On_Child_Work_Finish));

		for(int i = 0; i < m_process_num; i++){

			m_Child[i] = CLProcess(new CLProccessFunctionForExer, true);
			if(!((m_Child[i]->Run((void*)(runcmd.c_str()))).IsSuccess)){
				cout << "Run error" << endl;
				m_Child[i] = NULL;
			}
		}

		return CLStatus(0, 0);
	}
	CLStatus On_Child_Init(CLMessage *pm)
	{
		CLSharedExecutiveCommunicationByNamedPipe *pSender = new CLSharedExecutiveCommunicationByNamedPipe(childname.c_str());
		pSender->RegisterSesializer(FATHER_INIT_MESSAGE_ID, new CLFatherInitMsgSerializer);
		pSender->RegisterSesializer(FATHER_ACK_MESSAGE_ID, new CLFatherAckMsgSerializer);
		pSender->RegisterSesializer(QUIT_MESSAGE_ID, new CLQuitMsgSerializer);

		CLExecutiveNameServer::GetInstance()->Register(childname.c_str(), pSender);

		CLFatherInitMsg *pFatherInitMsg = new CLFatherInitMsg;
		pFatherInitMsg->dirname = m_dirlist[m_dir_num++];
		CLExecutiveNameServer::PostExectiveMessage(childname.c_str(), pFatherInitMsg);

		return CLStatus(0, 0);
	}
	CLStatus On_Child_Send_Request(CLMessage *pm)
	{

	}
	CLStatus On_Intermediate_Request(CLMessage *pm)
	{
		
	}
	CLStatus On_Child_Work_Finish(CLMessage *pm)
	{
		CLExecutiveNameServer::GetInstance()->ReleaseCommunicationPtr(childname.c_str());

		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}

private:
	CLExecutive **m_Child;
	map<string, unsigned int> word_table;
};

int main(int argc, char **argv)
{
	CLFatherWordCountObserver *pFartherWordCountObserver = new CLFatherWordCountObserver;

	for(int i = 1; i < argc; i++)
		pFartherWordCountObserver->PushDirname(argv[i]);
	pFartherWordCountObserver->Init();

	CLNonThreadForMsgLoop father_nonthread(pFartherWordCountObserver, "father_pipe",
		EXECUTIVE_BETWEEN_PROCESS_USE_PIPE_QUEUE);

	father_nonthread.RegisterDeserializer(CHILD_INIT_MESSAGE_ID, new CLChildInitMsgDeserializer);
	father_nonthread.RegisterDeserializer(CHILD_SEND_REQUEST_MESSAGE_ID, new CLChildSendRequestMsgDeserializer);
	father_nonthread.RegisterDeserializer(INTERMEDIATE_RESULT_MESSAGE_ID, new CLIntermediateResultMsgDeserializer);
	father_nonthread.RegisterDeserializer(CHILD_WORK_FINISH_MESSAGE_ID, new CLChildWorkFinishMsgDeserializer);

	father_nonthread.Run(0);

	return 0;
}

