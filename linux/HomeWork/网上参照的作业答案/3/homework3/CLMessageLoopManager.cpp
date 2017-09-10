#include "CLMessageLoopManager.h"
#include "CLMessage.h"
#include "CLLog.h"

CLMessageLoopManager::CLMessageLoopManager()
{
}

CLMessageLoopManager::~CLMessageLoopManager()
{
}

CLStatus CLMessageLoopManager::EnterMessageLoop(void *pContext)
{
	CLStatus s = Initialize();
	if(!s.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), Initialize error", 0);
		return CLStatus(-1, 0);
	}
	
	while(true)
	{
		CLMessage *pMsg = WaitForMessage();
		if(pMsg == 0)
		{
			CLLog::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), pMsg == 0", 0);
			continue;
		}
		
		CLStatus s3 = MessageDispatch(pMsg);

		delete pMsg;

		if(s3.m_clReturnCode == QUIT_MESSAGE_LOOP)
			break;
	}

	CLStatus s4 = Uninitialize();
	if(!s4.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), Uninitialize() error", 0);
		return CLStatus(-1, 0);
	}

	return CLStatus(0, 0);
}
