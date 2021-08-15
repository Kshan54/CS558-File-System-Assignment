#include <iostream>
#include <stdio.h>
#include<fstream>
#include<vector>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<cstring>
using namespace std;
struct inode{
	char name[8]="";
	int size;
	int blockpointer[8];
	int used;
};
void initialize_disk(){
	FILE* fp;
	struct inode in[16];
	char buff[128];
	fp=fopen("disk1","r+b");
	if(fp==NULL)
	{
		cout<<"Cannot open the File!"<<endl;
	}
	fseek (fp,0,SEEK_SET);
	fread(buff,1,128, (FILE*)fp);
	cout<<"Superblock Initialized  ! "<<endl;
	int count=0;
	for(int i=0;i<128;i++)
	{
		if(buff[i]=='1')
		{
			count+=1;
		}
		cout<<buff[i]<<" ";
	}
	if(count==1){
		for(int i=0;i<16;i++)
		{
			in[i].size=0;
			for(int j=0;j<8;j++)
			{
				in[i].blockpointer[j]=-1;
			}
			in[i].used=0;
		}
		fseek (fp,128 ,SEEK_SET);
		fwrite(in,sizeof(struct inode),16,fp);
	}
	cout<<endl<<"16 Inodes Initialized after 128 bytes of Superblock or restored if Disk is Not empty !"<<endl; 
	struct inode in1[16];
	fseek (fp,128 ,SEEK_SET);
	fread(in1,sizeof(struct inode),16,fp);
	for(int i=0;i<16;i++)
	{
		cout<<i<<"- iNode details :";
		cout<<"  Name :"<<in1[i].name<<" Used :"<<in1[i].used<< " Size :"<<in1[i].size <<endl;
	}
	cout<<endl;
	fclose(fp);
}
void createfile(char* name, int size) {
	if (size > 8)
	{
		cout << "Cannot Create File as size exceeds Maximum Limit" << endl;
		return;
	}
	FILE * fp;
	fp=fopen("disk1","r+b");
	fseek (fp,0,SEEK_SET);
	char buff[128];
	memset(buff,0,128);
	fread(buff,1,128, (FILE*)fp);
	int space_available=0;
	bool flag=false;
	vector<int> index;
	for(int i=0;i<128;i++)
	{
		if(buff[i]!='1')
		{
			index.push_back(i);
			space_available+=1;
			if(space_available==size)
			{
				flag=true;
				break;
			}	
		}
	}
	if(flag)
	{
		struct inode in1[16];
		fseek (fp,128 ,SEEK_SET);
		fread(in1,sizeof(struct inode),16,fp);
		for(int i=0;i<16;i++)
		{
			if(strcmp(in1[i].name,name)==0)
			{
				cout<<"File with same name exists hence not possible ! "<<endl;
				fclose(fp);
				return;
			}
		}
		for(int i=0;i<16;i++)
		{
			if(in1[i].used==0)
			{
				strncpy(in1[i].name, name, 8);
				in1[i].used=1;
				in1[i].size=size;
				for(int j=0;j<index.size();j++)
				{
					in1[i].blockpointer[j]=index[j];
					buff[index[j]]='1';
				}
				cout<<"File named: "<<name<<" created Successfully !"<<endl;
				fseek(fp,0,SEEK_SET);
				fwrite(buff,1,128,fp);
				fseek(fp,128,SEEK_SET);
				fwrite(in1,sizeof(struct inode),16,fp);
				fclose(fp);
				return;
			}
		}
		
		
	}
	else
	{
		cout<<" Not sufficient Space ! "<<endl;
	}	
}
void writefile(char * name, int blocknum, char* buff)
{
	FILE *fp;
	int global_block=-1;
	int actual_add=-1;	
	fp =fopen("disk1","r+b");
	if(fp==NULL)
	{
		cout<<" Disk not Found ! "<<endl;
		return;
	}
	fseek(fp,128,SEEK_SET);
	struct inode in[16];
	fread(in,sizeof(struct inode),16,fp);
	for(int i=0;i<16;i++)
	{
		if(in[i].used==1 && strcmp(in[i].name,name)==0)
		{
			if(in[i].blockpointer[blocknum]!=-1)
			{
				global_block=in[i].blockpointer[blocknum];
				actual_add=(global_block*(1024));
				fseek(fp,actual_add,SEEK_SET);
				fwrite(buff,1,1024,fp);
				cout<<"Write Successfull for file name: "<<name<<" Blocknum: "<<blocknum<<endl;
				fclose(fp);
				return;	
			}
			else
			{
				cout<<"Incorrect or deleted Blocknumber ! "<<endl;
				fclose(fp);
				return;
			}	
		}
	}
	cout<<"File name does not exist so deletion not Possible !"<<endl;
	return ;	
}
void readfile(char * name, int blocknum, char* buff)
{
	FILE *fp;
	int global_block=-1;
	int actual_add=-1;	
	fp =fopen("disk1","r+b");
	if(fp==NULL)
	{
		cout<<" Disk not Found ! "<<endl;
		return;
	}
	fseek(fp,128,SEEK_SET);
	struct inode in[16];
	fread(in,sizeof(struct inode),16,fp);
	for(int i=0;i<16;i++)
	{
		if(in[i].used==1 && strcmp(in[i].name,name)==0)
		{
			if(in[i].blockpointer[blocknum]!=-1)
			{
				global_block=in[i].blockpointer[blocknum];
				actual_add=(global_block*(1024));
				fseek(fp,actual_add,SEEK_SET);
				fread(buff,1,1024,fp);
				cout<<"Read Successfull for file name: "<<name<<" Blocknum: "<<blocknum<<endl;
				fclose(fp);
				return;	
			}
			else
			{
				cout<<"Incorrect or deleted Blocknumber ! "<<endl;
				fclose(fp);
				return;
			}	
		}
	}
	cout<<"File name does not exist so deletion not Possible !"<<endl;
	return ;
	
}
void deletefile(char* name)
{
	FILE *fp;
	int global_block=-1;
	int actual_add=-1;	
	fp =fopen("disk1","r+b");
	if(fp==NULL)
	{
		cout<<" Disk not Found ! "<<endl;
		return;
	}
	fseek(fp,128,SEEK_SET);
	struct inode in[16];
	bool flag=false;
	char superblock[1024];
	fread(in,sizeof(struct inode),16,fp);
	fseek(fp,0,SEEK_SET);
	fread(superblock,1,128,fp);
	for(int i=0;i<16;i++)
	{
		if(in[i].used==1 && strcmp(in[i].name,name)==0){
			flag=true;
			cout<<"deleting "<<in[i].name<<endl;
			in[i].used=0;
			in[i].size=0;
			char *ename = new char[8];
			memset(ename,0,8);		
			strncpy(in[i].name, ename, 8);
			for(int j=0;j<8;j++)
			{
				superblock[in[i].blockpointer[j]]=0;
				in[i].blockpointer[j]=-1;
			}
			break;
		}
	}
	if(flag)
	{
		fseek(fp,128,SEEK_SET);
		fwrite(in,sizeof(struct inode),16,fp);
		fseek(fp,0,SEEK_SET);
		fwrite(superblock,1,128,fp);
		cout<<"File named: "<<name<<" Deleted Successfully "<<endl;
		fclose(fp);
		return;
	}
	else
	{
		cout<<"File deletion not Possible as file name not found !"<<endl;
		return;
	}

}
void ls(){
	FILE * fp;
	fp=fopen("disk1","rb");
	if(fp==NULL)
	{
		cout<<"Disk not Found !"<<endl;
		return;
	}
	fseek(fp,128,SEEK_SET);
	struct inode in[16];
	fread(in,sizeof(struct inode),16,fp);
	for(int i=0;i<16;i++)
	{
		if(in[i].used==1)
		{
			cout<<"Name : "<<in[i].name<<" Size: "<< in[i].size <<endl;
		}
		
	}
	fclose(fp);
	return;
}
int main()
{
	initialize_disk();
	cout<<"Menu : "<<endl;
	cout<<"1. Create file of size upto 8KB "<<endl;
	cout<<"2. Write to the File (if file size is 4KB) {you can choose any of the 4 Blocks to write Numbered(0-3) in 4KB File}"<<endl;
	cout<<"3. Read from the Specified Block of File "<<endl;
	cout<<"4. To list all the Files present in Disk "<<endl;
	cout<<"5. Delete the File given File is Present on the Disk"<<endl;
	while(1){
		int k;
		cin.clear();
		cout<<"Enter your Choice from{ 1 - 5} & -1 to end :"<<endl;
		cin>>k;
		if(k==-1)
		{
			break;
		}
		switch(k){
			case 1:
	        	{
	        		cout<<"Enter the Name of the File to be created : "<<endl;
	        		string str;
	        		int size;
	        		cin.ignore();
	        		getline(cin,str);
	        		cout<<"Enter the size of File "<<endl;
	        		cin>>size;
				cin.clear();
	        		createfile(&str[0],size);
	            		break;
	        	}
	           	 // Write onto the File 
			case 2 :{
					cout<<"Enter the Name of the File on which you want to write : "<<endl;
					string str,str_out;
					cin.ignore();
					getline(cin,str);
					cout<<"Enter the data you want to write into the File :"<<endl;
					getline(cin,str_out);
					int blocknum;
					cout<<"Enter the Block Number of File in which you want to write : "<<endl;
					cin>>blocknum;
					if(blocknum>8)
					{
						cout<<"Incorrect Block Number entered !"<<endl;
						break;
					}
					writefile(&str[0],blocknum, &str_out[0]);
					break;

			}
			case 3 :{
					char buff[1024];
					memset(buff, 0, 1024);
					cout<<"Enter the Name of the File From which you want to Read : "<<endl;
					string str;
					cin.ignore();
					getline(cin,str);
					int blocknum;
					cout<<"Enter the Block Number from which you want to Read :"<<endl;
					cin>>blocknum;
					if(blocknum>8)
					{
						cout<<"Incorrect Block Number entered !"<<endl;
						break;
					}
					readfile(&str[0],blocknum,buff);
					cout << buff << endl;
					break;
			}
		
			case 4:{
					ls();
					break;
			}
			case 5:{
				cout<<"Enter the Name of the File to be Deleted : "<<endl;
				string str;
				cin.ignore();
				getline(cin,str);
				cin.clear();
				deletefile(&str[0]);
				break;
			}
			default:{
				cout<<"Enter Correct case ! "<<endl;
				cin.clear();
				break;
			}
		}

	}
	return 0;	
}
  
