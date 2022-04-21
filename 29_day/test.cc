#include<iostream>
#include<unistd.h>
#include<errno.h>
#include<cstdlib>
#include<cstdio>
#include<fcntl.h>
using namespace std;
void Fcntl(int local){

    int fcnt= fcntl(local,F_GETFD);
    if(fcnt<0){
      cout<<"fcntl filed!";
     }
    fcntl(local,F_SETFL,fcnt | O_NONBLOCK);
}
int main(){

  Fcntl(0);
  while(1){
sleep(2);
  char buff[1024];

  int local=read(0,buff,sizeof(buff));
  if(local>0){

    buff[local]=0;
    cout<<"cout:"<<buff<<endl;
  }
  //read读取条件不满足
  else if(local<0&&errno==EAGAIN){
cout<<"errno:"<<errno<<endl;
cout<<"检测缓冲区中有没有数据。。。。。。。。。。"<<"local:"<<local<<endl;
  }else{
    //read读出错
    cout<<"read error"<<endl;
  }
  

  }
  return 0;
}
