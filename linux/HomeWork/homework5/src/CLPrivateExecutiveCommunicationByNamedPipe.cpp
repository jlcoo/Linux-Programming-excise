#include "CLPrivateExecutiveCommunicationByNamedPipe.h"

CLPrivateExecutiveCommunicationByNamedPipe::CLPrivateExecutiveCommunicationByNamedPipe(const char *pstrExecutiveName) : CLExecutiveCommunicationByNamedPipe(pstrExecutiveName, false)
{
}

CLPrivateExecutiveCommunicationByNamedPipe::~CLPrivateExecutiveCommunicationByNamedPipe()
{
	// int fd = open(m_strExecutiveName.c_str(), O_RDONLY | O_NONBLOCK);

	// if(fd == -1)
	// {
	// 	CLLogger::WriteLogMsg("In CLMessageQueueByNamedPipe::CLMessageQueueByNamedPipe(), open error", errno);
	// 	throw "In CLMessageQueueByNamedPipe::CLMessageQueueByNamedPipe(), open error";
	// }

	// for(;;){
	// 	CLMessage *pMsg = NULL;

	// 	if(read(fd, &pMsg, sizeof(CLMessage *)) == -1)
	// 		break;

	// 	delete pMsg;
	// }
	// if(close(fd) == -1){
	// 	CLLogger::WriteLogMsg("CLExecutiveCommunicationByNamedPipe::~CLExecutiveCommunicationByNamedPipe(), close", errno);
	// }

}

char *CLPrivateExecutiveCommunicationByNamedPipe::GetMsgBuf(CLMessage *pMsg, unsigned int *pLength)
{
	int len = sizeof(CLMessage *);
	char *pBuf = new char[len];
	*((long *)pBuf) = (long)pMsg;

	*pLength = len;
	return pBuf;
}