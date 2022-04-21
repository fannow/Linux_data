#include<iostream>
#include<sstream>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<string>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include "ThreadPool.hpp"
using namespace std;
#define num 5
class tcpsever{
  private:
  int port;
  int listen_sock;//监听套接字
  ThreadPool *tp;
  public:
  tcpsever(int port_=9999):
    tp(NULL)
  {
  port=port_;
  }
  void Init(){
   // tp->Init();
    listen_sock=socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock<0){
      cout<<"创建失败"<<endl;
    }
    struct sockaddr_in in_addr;
    in_addr.sin_family=AF_INET;
    in_addr.sin_port=htons(port);
    in_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(listen_sock,(const struct sockaddr*)&in_addr,sizeof(in_addr))<0){
      cout<<"绑定失败1"<<endl;
      exit(3);
    }
    
    if(listen(listen_sock,num)<0){
      cout<<"绑定失败2"<<endl;
      exit(4);
    }
    
    tp=new ThreadPool();
    tp->ThreadPoolInit();
  }
   void star(){
     struct sockaddr_in addr;
     socklen_t len=sizeof(addr);
    while(1){
    //获取链接
    int sock=accept(listen_sock,(struct sockaddr*)&addr,&len);
    if(sock<0){
    cout<<"链接获取失败"<<sock<<endl;
    continue;
    }
    char buf[16];
    sprintf(buf,"%d",ntohs(addr.sin_port));
    string str="";
    str=inet_ntoa(addr.sin_addr);
    str+=":";
    //string str2="";
   // stringstream str1;
   // str1<<ntohs(addr.sin_port);
   // str1>>str2;
    //str+=str2;//to_string(addr.sin_port);
    cout<<"get a new link..."<<str<<endl;
    Task* p1=new Task(sock);
  
    tp->Put(*p1);
     }
   }
  
};

int main(int argc,char*argv[]){

tcpsever tc(atoi(argv[1]));
tc.Init();
tc.star();
return 0;
}
