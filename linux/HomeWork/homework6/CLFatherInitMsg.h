#ifndef CLFatherInitMsg_H
#define CLFatherInitMsg_H

class CLFatherInitMsg : public CLMessage
{
public:
	CLFatherInitMsg():CLMessage(FATHER_INIT_MESSAGE_ID) {}
public:
	string dirname;
};

class CLFatherInitMsgSerializer : public CLMessageSerializer
{
public:
	virtual char *Serialize(CLMessage *pMsg, unsigned int *pFullLength, unsigned int HeadLength)
	{
		CLFatherInitMsg *p = dynamic_cast<CLFatherInitMsg *>(pMsg);
		if(p == 0){
			cout << "dynamic_cast errror" << endl;
			return 0;
		} 

		unsigned int dir_len = p->dirname.size();

		*pFullLength = HeadLength + sizeof(long) + sizeof(unsigned int) + dir_len;
		char *pBuf = new char[*pFullLength];
		long *pId = (long*)(pBuf + HeadLength);
		*pId = p->m_clMsgID;

		unsigned int *len = (unsigned int*)(pBuf + HeadLength + sizeof(long));
		*len = dir_len;

		char *dirname = (char*)(HeadLength + sizeof(long) + sizeof(unsigned int) + pBuf);
		memcpy(dirname, p->dirname.c_str(), dir_len);

		return pBuf;
	}
};

class CLFatherInitMsgDeserializer : public CLMessageDeserializer
{
public:
	virtual CLMessage *Deserialze(char *pBuffer)
	{
		long id = *(long*)pBuffer;
		if(id != FATHER_INIT_MESSAGE_ID)
			return 0;

		CLFatherInitMsg *p = new CLFatherInitMsg;

		unsigned int t_len = *((unsigned int *)(pBuffer + sizeof(id)));
		char str[t_len + 1];
		memset(str, 0 , sizeof(char)*(t_len + 1));
		const char *dirname = (char*)(pBuffer + sizeof(id) + sizeof(t_len));
		memcpy(str, dirname, t_len);
		p->dirname = string(str);

		return p;
	}
};


#endif
