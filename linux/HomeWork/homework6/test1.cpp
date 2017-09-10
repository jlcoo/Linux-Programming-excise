#include <iostream>

using namespace std;

int max(int &a, int &b)
{
	return (a > b ? a : b);
}

int main (int argc, char **argv)
{
	int a, b, c;

	cin >> a >> b >> c;

	int r1 = a + b*c;
	int r2 = a*(b+c);
	int r3 = a*b*c;
	int r4 = (a+b)*c;

	int result = max(max(r1,r2), max(r3, r4));
	cout << result;
	

	cout << a << b << max(a,b);

	return 0;
}
