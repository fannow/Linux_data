#include<iostream>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
using namespace std;
int main(){
int fd=open("fifo",O_WRONLY);
if(fd>0){
char buff[64];
while(1){
cout<<"请输入:";
fflush(stdout);
ssize_t s=read(0,buff,sizeof(buff)-1);
if(s>0){
buff[s]=0;
//const char *str="nihao! ";
write(fd,buff,s);
sleep(1);
}
}
}

return 0;
}
