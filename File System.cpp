//Assignment 2, Question 3 
// File System
//Kaushal Dewangan 204101071
// Pratik Kulkar 204101031
#include  <iostream>
#include  <bits/stdc++.h>
using namespace std;

//created the structure for inode information
struct Inode_Info{
    char name[8];
    int size;
    int blockPointers[8];
    int used;
};

//created Disk structure
struct DISK{
  int Free_block_list[127];           // records of the free block disk locations
  struct Inode_Info inode_info[16];  // inode information for 16 files
  char Blocks[127][1024];           //127 blocks 1024B(1KB) each
}disk;

int file_ptr=0, total_file=0;
////////////////////////////////////////////////////////////////////////////////////////////////////
//This function will create the files in the disk blocks
void create(char name[8], int size)
{
	if(strlen(name)<1 || strlen(name)>8) 
	{cout<<"Invalid Input !! The lenght of file name should be at least 1 and at most 8.\n"; return;}
    
	int find=0;
   for(int i=0;i<16;i++)  // find the file if already exist or not
    {
        if(strcmp(name,disk.inode_info[i].name)==0)
        { find=1;}
    }

    if(find==1){cout<<"Invalid Input !! filename "<<name<<" is already exist .\n"; return;}
    
    

    if(total_file>=16) {cout<<"Invalid Input !! The File system supports at most 16 files.\n"; return;}
    
    cout<<"Enter File bolck Size : "; 
    cin>>size; 

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if(total_file==15&&size>7){cout<<"Invalid Input !! The size of last file is at most 7.\n"; return;
	}

    if(size<1) {cout<<"Invalid Input !! file block size should be numeric and greater then 0 .\n"; return;}
    if(size>8) {cout<<"Invalid Input !! file block size should not be greater then 8 .\n"; return;}
    
     for(int i=0;i<16;i++)
    {
        if(disk.inode_info[i].size!=0) {file_ptr=(file_ptr+1)%16; break;}
    }

    strncpy(disk.inode_info[file_ptr].name, name, 8);
    
    disk.inode_info[file_ptr].size=size;
    int j=0;

    for(int i=0;i<127;i++)   // assign memory into the empty disk blocks
    {
        if(disk.Free_block_list[i]==0)
        {
        disk.inode_info[file_ptr].blockPointers[j++]=i;
        disk.Free_block_list[i]=1;
        }
        if(j==size) break;
        
    }
    disk.inode_info[file_ptr].used=0;
    total_file++;
    cout<<"file "<<name<< " successfully created\n";
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// function for delete the file if already exist 
void delete_(char name[8])
{
    int loc=0,find=0;
    for(int i=0;i<16;i++)
    {
        if(strcmp(name,disk.inode_info[i].name)==0)
        {loc=i; find=1;}

    }
    if(find==0){cout<<"Invalid Input !! filename "<<name<<" is not present.\n"; return;}

    int size=disk.inode_info[loc].size;
    for(int i=0;i<size;i++)
    {
    	// free the disk blocks
        disk.Free_block_list[disk.inode_info[loc].blockPointers[i]]=0;
    }
    disk.inode_info[loc].size=0;
    disk.inode_info[loc].name[0]='\0';
    total_file--;
    cout<<"file "<<name<< "successfully deleted0\n";
  
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// read the file with specific block number if exist
void read(char name[8], int blockNum, char buf[1024])
{
  int find=0,loc=0;
  for(int i=0;i<16;i++) // find the file location in disk block
    {
        if(strcmp(name,disk.inode_info[i].name)==0)
        { find=1; loc=i;}
    }

    if(find==0){cout<<"Invalid Input !! filename "<<name<<" is not exist .\n"; return;}
   
    int size=disk.inode_info[loc].size;
      cout<<"Enter bolck number between 1 to "<<size<<" :"; 
   cin>>blockNum;
   cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	 
    if(blockNum>size||blockNum< 1) {cout<<"Invalid Input !! block number should be numeric and not be greater then block size .\n"; return;}
	blockNum--;
    int Disk_location=disk.inode_info[loc].blockPointers[blockNum];

    string s=disk.Blocks[Disk_location];
    cout<<"\n**************************FILE************************************\n";
    cout<<s<<"\n";
     cout<<"***************************END************************************\n";

      if(s.size()==0) cout<<"Empty file block.\n";
      else
    cout<<"\nfile read successfully!\n";

  
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// this function writes the charactors into the specific given disk block
void write_(char name[8], int blockNum, char buf[1024])
{
  int loc=0,find=0;
    for(int i=0;i<16;i++)
    {
        if(strcmp(name,disk.inode_info[i].name)==0)
        {loc=i; find=1;}
    }
     if(find==0){cout<<"Invalid Input !! filename "<<name<<" is not exist .\n"; return;}
int size=disk.inode_info[loc].size;
   cout<<"Enter bolck number between 1 to "<<size<<" :"; 
   cin>>blockNum;
   
   cin.clear();
cin.ignore(numeric_limits<streamsize>::max(), '\n');   
    if(blockNum>size||blockNum<1) {cout<<"Invalid Input !! block number should not be greater then block size .\n"; return;}

    string s;
    cout<<"Write into block num:"<<blockNum<<" (upto 1024 characters) :";
    if(s.size()>1024) {cout<<"Invalid Input !! The lenght of characters in block should not be greater then 1024 cha.\n"; return;}
    getline(cin >> ws, s);
    strcpy(buf, s.c_str());
    
    disk.inode_info[loc].used=1;
   blockNum--; 
  int Disk_location=disk.inode_info[loc].blockPointers[blockNum];

  strncpy(disk.Blocks[Disk_location],buf, 1024);
  
	cout<<"\nfile successfully written\n";

}
/////////////////////////////////////////////////////////////////////////////////////////
// list out all the files and its block size which are created 
void ls()
{
    for(int i=0;i<16;i++)
    if(disk.inode_info[i].name[0]!='\0')
    cout<<"->"<<disk.inode_info[i].name<<"     "<<disk.inode_info[i].size<<" blocks\n";  
}
int main()
{
    int exit=0;
    while(!exit)
    {
    	//for(int i=0;i<127;i++) cout<<disk.Free_block_list[i]; cout<<"\n";
    	cout<<"________________________________________________________________________________________________________\n";
        cout<<"1. create file.     ";
        cout<<"2. delete file.     ";
        cout<<"3. read file.      ";
        cout<<"4. write file.     ";
        cout<<"5. lists files.    ";
        cout<<"6. exit\n";
        cout<<"________________________________________________________________________________________________________\n";
        cout<<"Enter Choice: ";
        int scase;
        cin>>scase;
        cin.clear();
       cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch(scase)
        {
            case 1 : {  char name[999]; int size; cout<<"Enter File Name (without space): ";
                            cin>>name;
                            create(name,size);
                            break;
                        }
            case 2 : {  char name[8]; int size; cout<<"Enter File Name : ";
                            cin>>name;
                                delete_(name);
                            break;
                        }

                 case 3 : {
                             int blockNum; char buf[1024];
                            char name[8]; int size; cout<<"Enter File Name : ";
                            cin>>name;
                            read(name, blockNum, buf);
                          break;
                      }

              case 4 : {
                           char name[8]; int blockNum; 
                           char  buf[1024];
                           cout<<"Enter File Name(without space) : ";
                            cin>>name; 
                            write_(name, blockNum, buf);
                          break;
                        }

                     
            case 5 : { ls();	break;	}

            case 6 : {exit=1; break;}

            default :  cout<<"invalid choice\n";

        }
    }
return 0;
}
