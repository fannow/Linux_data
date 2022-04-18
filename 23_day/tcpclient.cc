#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
using namespace std;
class tcpsever{
  private:
  string ip;
  int port;
  int sock;//监听套接字
  public:
  tcpsever(string ip_="127.0.0.1",int port_=9999):
  ip(ip_),port(port_)
  {
  }
  void Init(){
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
      cout<<"创建失败"<<endl;
    }
    struct sockaddr_in in_addr;
    in_addr.sin_family=AF_INET;
    in_addr.sin_port=htons(port);
    in_addr.sin_addr.s_addr=inet_addr(ip.c_str());
  
    if(connect(sock,(struct sockaddr*)&in_addr,sizeof(in_addr))!=0){
      cout<<"链接建立失败"<<endl;
      exit(2);
    }
  }
   void star(){
   char buf[64];
    while(1){
    string sin;
    cout<<"请输入请求#";
    //cin>>sin;
    fflush(stdout);
    size_t ss=read(0,buf,strlen(buf)-1);
    //fflush(stdout);
    if(ss>0){ 
      send(sock,buf,sizeof(buf),0);
      char buff[64];
      size_t s=recv(sock,buff,sizeof(buff)-1,0);
      if(s>0){

        buff[s-1]=0;
      cout<<buff<<endl;
        }
      

       }
     }
   }   
   ~tcpsever(){
   close(sock); 
   }
};

int main(int argc,char*argv[]){

tcpsever tc(argv[1],atoi(argv[2]));
tc.Init();
tc.star();
return 0;
}
