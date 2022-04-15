#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
using namespace std;
int main(){
mkfifo("fifo",0644);
int fd=open("fifo",O_RDONLY);
if(fd>=0){
char buff[64];
while(1){
ssize_t s=read(fd,buff,sizeof(buff)-1);
if(s>0){
buff[s]=0;
cout<<"sever :"<<buff<<endl;
}
}
}
close(fd);
return 0;
}
