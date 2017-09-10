#include <iostream>
#include "CLStatus.h"
#include "cache.h"

using namespace std;

int main(int argc, char **argv)
{
	CLCacher a;

	a.OpenFile("test.txt");
	a.WriteFile("hello world!");
	a.ReadFile(2, 2);
	a.CloseFile();

	return 0;
}

