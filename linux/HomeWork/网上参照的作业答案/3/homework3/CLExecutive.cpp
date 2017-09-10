#include "CLExecutive.h"
#include "CLExecutiveFunctionProvider.h" //不加此也能编译通过，但有警告 分析之

CLExecutive::CLExecutive(CLExecutiveFunctionProvider *pExecutiveFunctionProvider)
{
	m_pExecutiveFunctionProvider = pExecutiveFunctionProvider;
}

CLExecutive::~CLExecutive()
{
	delete m_pExecutiveFunctionProvider;
}
