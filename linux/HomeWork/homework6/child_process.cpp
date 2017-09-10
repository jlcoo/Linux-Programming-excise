#include <iostream>

class CLChildWordCountObserver : public CLMessageObserver
{
	virtual CLStatus Initialize(CLMessageLoopManager *pMesageLoop, void* pContext)
	{
		pMesageLoop->Register(FATHER_INIT_MESSAGE_ID, (CallBackForMessageLoop)(&CLChildWordCountObserver::On_ReadDir));
		pMesageLoop->Register(FATHER_ACK_MESSAGE_ID, (CallBackForMessageLoop)(&CLChildWordCountObserver::On_Father_Ack));
		pMesageLoop->Register(QUIT_MESSAGE_ID, (CallBackForMessageLoop)(&CLChildWordCountObserver::On_Quit));

		CLSharedExecutiveCommunicationByNamedPipe *pSender = new CLSharedExecutiveCommunicationByNamedPipe("father_pipe");

		pSender->RegisterSerializer(CHILD_INIT_MESSAGE_ID, new CLChildInitMsgSerializer);
		pSender->RegisterSerializer(CHILD_SEND_REQUEST_MESSAGE_ID, new CLChildSendRequestMsgSerializer);
		pSender->RegisterSerializer(INTERMADIATE_RESULT_MESSAGE_ID, new CLIntermediateResualtMsgSerializer);
		pSender->RegisterSerializer(CHILD_WORK_FINISH_MESSAGE_ID, new CLChildWorkFinishMsgSerializer);
		pSender->RegisterSerializer(QUIT_MESSAGE_ID, new CLQuitMsgSerializer);

		CLExecutiveNameSever::GetInstance()->Register("father_pipe", pSender);
		CLChildInitMsg *pChildInitMsg = new CLChildInitMsg;
		pChildInitMsg->childname = childname;

		CLExecutiveNameSever::PostExecutiveMessage("father_pipe", pChildInitMsg);

		return CLStatus(0, 0);
	}
	CLStatus On_ReadDir(CLMessage *pm)
	{
		WordCount wc(dirname);
		wc.DirWordCount(word_table);
	}
	CLStatus On_Father_Ack(CLMessage *pm)
	{
	}
	CLStatus On_Quit(CLMessage *pm)
	{
		CLExecutiveNameSever::GetInstance()->ReleaseCommunicationPtr("father_pipe");
		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}
private:
	string childname;
	map<string, unsigned int>word_table;
}



int main(int argc, char **argv)
{

	try
	{
		if(!CLLibExecutiveInitializer::Initialize().IsSuccess()){
			cout << "Initialize error" << endl;
			return 0;
		}
		CLNonThreadForMsgLoop child_nonthread(new CLChildWordCountObserver(argv[1]),
			argv[1], EXECUTIVE_BETWEEN_PROCESS_USE_PIPE_QUEUE);

		child_nonthread.RegisterDeserializer(FATHER_INIT_MESSAGE_ID, new CLFatherInitMsgDeserializer);
		child_nonthread.RegisterDeserializer(FATHER_ACK_MESSAGE_ID, new CLFatherAckMsgDeserializer);
		child_nonthread.RegisterDeserializer(QUIT_MESSAGE_ID, new CLQuitMsgDeserializer);
		child_nonthread.Run(0);
		throw CLStatus(0, 0);
	}
	catch(CLStatus& s)
	{

	}

	return 0;
}


