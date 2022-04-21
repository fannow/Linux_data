#include<iostream>
#include<sys/wait.h>
#include<netinet/in.h>
#include<cstdio>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstdlib>
#include<sys/socket.h>
#include"Math.hpp"
using namespace std;
class sever{
  private:
    int listen_sock;//监听套接字
    int post;
  public:
    sever(int post_){
      post=post_;
    }
    void Init(){
      listen_sock=socket(AF_INET,SOCK_STREAM,0);
      if(listen_sock<0){
        cout<<"套接字创建失败!"<<endl;
        exit(0);
      }
      struct sockaddr_in addr;
      addr.sin_family=AF_INET;
      addr.sin_port=htons(post);
      addr.sin_addr.s_addr=INADDR_ANY;
      if(bind(listen_sock,(struct sockaddr*)&addr,sizeof(addr))<0){
        cout<<"绑定失败!"<<endl;
        exit(1);
      }
      if(listen(listen_sock,5)<0){
        cout<<"绑定失败!"<<endl;
        exit(2);
      }
    }
      void star(){

    while(1){
        
        struct sockaddr_in in_addr;
        socklen_t len=sizeof(in_addr);
        int sock=accept(listen_sock,(struct sockaddr*)&in_addr,&len);
        if(sock<0){
          cout<<"获取链接失败!"<<endl;
          continue;
        }
        char s1[16];
        
        sprintf(s1,"%d",ntohs(in_addr.sin_port));
        string IP_PORT=inet_ntoa(in_addr.sin_addr);
        IP_PORT+=":";
      
        cout<<"get a new link"<<IP_PORT<<s1<<endl;
        int id=fork();
        if(id==0){
          cout<<"zijc"<<endl;
          close(listen_sock);
          service(sock);
        exit(0);
        }
        close(sock);
        
        waitpid(-1,NULL,0);
      }
      }
       void service(int sock){
        Maths m;
        size_t s=recv(sock,&m,sizeof(m)-1,0);
       // cout<<m.a<<m.b<<m.c<<endl;
        if(s>0){
          m.code=0;
         m.op=0;
          switch(m.c){
              case '+':
                m.code=m.a+m.b;
                break;
              case '-':
                m.code=m.a-m.b;
                break;
              case '*':
                m.code=m.a*m.b;
                break;
              case '/':
                if(m.b!=0){
                m.code=m.a/m.b;
                }else{
                  m.op=1;
                }
                break;
              case '%':
                if(m.b!=0){
                m.code=m.a%m.b;
                }else{
                  m.op=2;
                }
                break;
              default:
                  m.op=3;
                  break;
              

          }
        }
        send(sock,&m,sizeof(m),0);
       // cout<<m.a<<m.b<<m.c<<endl;
        
             
    close(sock);  
    }
      ~sever(){
        close(listen_sock);
      }
};
int main(int argc,char*argv[]){
sever s(atoi(argv[1]));
s.Init();
s.star();

}
