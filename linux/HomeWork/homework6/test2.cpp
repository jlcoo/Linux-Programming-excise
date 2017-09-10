#include <iostream>
#include <stdio.h>
using namespace std;

int sub(int x)
{
	int b[10];
	int num;
	int x1 = x;
	for(num = 0; x > 0; num++){
		b[num] = x%10;
		x /= 10;
	}
	// cout << num << endl;
	int sum = 0;
	for(int i = 0; i < num; i++){
		sum += b[i];
	}
	cout << sum <<endl;

	// cout << endl;

	return (x1 - sum);
}

int main(int argc, char **argv)
{
	int in_num;
	cin >> in_num;

	int i;
	for(i = in_num; ; i++)
	{
		if(sub(i) >= in_num)
			break;

		cout << sub(i) << endl;
	}
	cout << i;


	return 0;
}