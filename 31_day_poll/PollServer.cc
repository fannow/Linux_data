#include<iostream>
#include<unistd.h>
#include<poll.h>
using namespace std;
int main(){
  struct pollfd rnfds[1];
  rnfds[0].fd=0;//stdin'输入
  rnfds[0].events =POLLIN;
  rnfds[0].revents=0;
  while(1){
    switch(poll(rnfds,1,5000)){
      case 0:
        cout<<"等待时间超时"<<endl;
        break;
        case -1:
        cout<<"等待出错"<<endl;
        default:
        if(rnfds[0].fd==0&&(rnfds[0].revents&POLLIN)){
        char buff[1024];
        int s=read(0,buff,sizeof(buff));
        if(s>0){
          buff[s]=0;
          cout<<"stdin:"<<buff<<endl;
        }
        cout<<"等待成功"<<endl;
        break;
    }
  
    }   
  }

  return 0;
}
