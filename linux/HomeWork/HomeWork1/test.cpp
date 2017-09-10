#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main()
{
  int fd = open("a.c", O_RDWR | O_APPEND);
  if(-1 == fd)
  {
    cout << "open error" << endl;
    return 0;
  }

  char buf[3];
  if(read(fd, &buf, 2) == -1)
  { 
    cout << "read error" << endl;
    close(fd);
    return 0;
  }
  buf[2] = '\0';
  printf("%s\n", buf);
  // cout << buf <<endl;

  off_t CurrentPosition = lseek(fd, 0, SEEK_CUR);
  if(CurrentPosition == -1)
  {
    cout << "lseek error" << endl;
    close(fd);
    return 0;
  }

  cout << "CurrentPosition:  " << CurrentPosition << endl;

  if(lseek(fd, 4, SEEK_SET) == -1)
  {
    cout << "lseek error" << endl;
    close(fd);
    return 0;
  }

  off_t NewCurrentPosition = lseek(fd, 0, SEEK_CUR);
  if(NewCurrentPosition == -1)
  {
    cout << "lseek error" << endl;
    close(fd);
    return 0;
  }

  cout << "NewCurrentPosition:  " << NewCurrentPosition << endl;

  // char buf1 = 'P';
  if(read(fd, &buf, 2) == -1)
  {
    cout << "read error" << endl;
    close(fd);    
    return 0;
  }
  buf[2] = '\0';
  printf("%s\n", buf);
  // cout << buf <<endl;

  close(fd);

  return 0;
}
