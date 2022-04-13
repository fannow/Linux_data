#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;
int main(){
pid_t id = fork();
if(id<0){
cout<<"fork error !"<<endl;
}
 if(id==0){
//childer
int count=0;
while(count<10){
cout<<"child is running "<<count<<endl;
count++;
sleep(1);
}
//exit(1);
int a=1/0;
}
cout<<"father wait before!"<<endl;
int ret1=0;
pid_t ret=waitpid(id,&ret1,0);
cout<<"子进程退出码："<<((ret1>>8)&0xff)<<endl;
cout<<"子进程接受信号："<<(ret1&0x7f)<<endl;
if(ret>0){
cout<<"wait success"<<endl;
}else{

cout<<"wait failed"<<endl;
}
cout<<"father weait after!"<<endl;

return 0;
}
