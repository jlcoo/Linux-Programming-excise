#include <iostream>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char **argv)
{
	char buf[] = "abcdefghij";

	int fd = open("a.txt", O_RDWR | O_APPEND);
	if(-1 == fd){
		cout << "open error" << endl;
	}
	if(lseek(fd, 5, SEEK_SET) == -1){
		cout << "lseek error" << endl;
	}
	if(write(fd, buf, 10) != 10){
		cout << "write error!" << endl;
	}
	close(fd);

	if((fd = open("a.txt", O_RDWR | O_APPEND)) == -1){
		cout << "read open error" << endl;
	}
	if(lseek(fd, 5, SEEK_SET) == -1){
		cout << "read lseek error" << endl;
	}
	if(read(fd, buf, 10) != 10){
		cout << "read error!" << endl;
	}

	cout << buf << endl;

	if(read(fd, buf, 10) != 10){
		cout << "read error!" << endl;
	}
	cout << buf << endl;
	close(fd);

	return 0;
}

