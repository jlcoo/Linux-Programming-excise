#include <iostream>
#include "CLStatus.h"
#include "CLLogger.h"

using namespace std;

class A
{
public:
	~A()
	{
		cout << "aaaaaaa" << endl;
		CLLogger::WriteLogMsg("aaaa", 0);
	}
};

A a;

CLStatus f()
{
	return CLStatus(-1, 4);
}

int main()
{
	CLStatus s = f();
	if(!s.IsSuccess())
		CLLogger::WriteLogMsg("this is an error", s.m_clErrorCode);

	return 0;
}