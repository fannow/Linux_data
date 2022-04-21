#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"Math.hpp"
using namespace std;
class client{
  private:
    string ip;
    int sock;
    int post;
  public:
    client(string ip_,int post_){
      ip=ip_;
      post=post_;
    }
    void Init(){
      sock=socket(AF_INET,SOCK_STREAM,0);
      if(sock<0){
        cout<<"套接字创建失败!"<<endl;
        exit(0);
      }
       struct sockaddr_in in_addr;
      in_addr.sin_family=AF_INET;
      in_addr.sin_port=htons(post);
      in_addr.sin_addr.s_addr=inet_addr(ip.c_str());
  
      if(connect(sock,(struct sockaddr*)&in_addr,sizeof(in_addr))!=0){
         cout<<"链接建立失败"<<endl;
         exit(2);

      }
    
    }
      void star(){
        Maths m;      
        

        cout<<"请输入a:";
        cin>>m.a;
        cout<<"请输入b:";
        cin>>m.b;
        cout<<"请输入操作符：";
        cin>>m.c;
      
      
        cout<<m.a<<m.b<<m.c<<endl;
        int s=send(sock,&m,sizeof(m),0);
        if(s<0){
          cout<<"传输错误"<<endl;
        }

        int sys=recv(sock,&m,sizeof(m),0);
        cout<<sys<<endl;
        if(sys>0){
        
          cout<<"执行结果："<<m.a<<m.c<<m.b<<"="<<m.code<<endl;
        }


      
    

  
}
      ~client(){
        close(sock);
      }
};
int main(int argc,char *argv[]){
client c(argv[1],atoi(argv[2]));
c.Init();
c.star();

}
