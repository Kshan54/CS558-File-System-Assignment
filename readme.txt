Compile the program as following:
1. First Compile disk.cpp which will create 128kb Flat File which will act as Disk for our File System
	
	COMPILE : g++ disk.cpp
	RUN     : ./a.out 
This Creates a disk named disk1 which is of size 128 KB

2. After the Disk is Created we will use it as File system and compile filesystem.cpp and use console to create file , delete file , read & write , List Files on Block accordingly:

	COMPILE : g++ filesystem.cpp
	RUN	: ./a.out

Now use The Console to test Accordingly.
