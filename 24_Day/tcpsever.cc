#include<iostream>
#include<sstream>
#include<signal.h>
#include<stdlib.h>
#include<string>
#include<cstdio>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
using namespace std;
#define num 5
class tcpsever{
  private:
  int port;
  int listen_sock;//监听套接字
  public:
  tcpsever(int port_=9999){
  port=port_;
  }
  void Init(){
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
  }
   void star(){
     signal(SIGCHLD,SIG_IGN);//忽略子进程退出时发给父进程的信号，系统将自动回收子进程
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
    string str2="";
    stringstream str1;
    str1<<ntohs(addr.sin_port);
    str1>>str2;
    str+=str2;//to_string(addr.sin_port);
    cout<<"get a new link..."<<str<<endl;
    pid_t p;
    size_t s=fork();
    if(s==0){
    close(listen_sock);
    service(sock);
    exit(0);
    }
    close(sock);
    //service(sock);

    }

  }
   void service(int sock){
    while(1){
    char buff[64];
    struct sockaddr_in add;
    socklen_t len=sizeof(add);
    size_t s=recv(sock,buff,sizeof(buff)-1,0);
    if(s>0){
    buff[s]=0;
    cout<<buff<<endl;
    string str="sever have...";
    send(sock,str.c_str(),sizeof(str)-1,0);

    }else if(s==0){
    cout<<"client have quited...."<<endl;
    break;
    close(sock);//关闭套接字因为服务器一直在运行如果不关闭可用的套接字将会越来越少。
    
    }else if(s<0){
   cout<<"文件读取失败"<<endl;
   break;
        }

     }
    close(sock);
    }
};

int main(int argc,char*argv[]){

tcpsever tc(atoi(argv[1]));
tc.Init();
tc.star();
return 0;
}
