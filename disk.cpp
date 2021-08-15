#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include<cstring>
using namespace std;
int main()
{
  int i, fd;
  char *buf;
  char* s1="disk1";
  printf( "Creating  a 128KB  file in %s\n",s1);
  printf("This flat file will act as disk and will hold your filesystem\n");
  fstream myfile(s1, ios::in | ios::out | ios::trunc);

  printf("Formatting your filesystem...\n");

  buf = new char[1024];
  
  /* write super block */
  memset(buf,0,1024);
  buf[0]='1';
  /* write out the super block */
  myfile.write(buf,1024); 
  buf[0]=0;
  /* write out the remaining 127 data blocks, all zeroed out */
  for(i=0;i<127;i++){
    	myfile.write(buf,1024);  
  }
  exit(1);

}
