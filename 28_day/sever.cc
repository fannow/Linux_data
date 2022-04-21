#include<iostream>
#include<sys/wait.h>
#include<netinet/in.h>
#include<cstdio>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstdlib>
#include<sys/socket.h>
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
          close(listen_sock);
          service(sock);
        exit(0);
        }
        close(sock);
        
       // waitpid(-1,NULL,0);
      }
      }
       void service(int sock){
         signal(SIGCHLD,SIG_IGN);//线程分离
          char buff[1024*2];
        int s=recv(sock,buff,sizeof(buff),0);
     		if(s>0){ 
          buff[s]=0;
          cout<<buff<<endl;
          string response="HTTP/1.0 200 OK\r\n";
          response+="Content-type: text/html\r\n";
          response+="\r\n";
          response+="\
            <!DOCTYPE html>\
            <html>\
            <head>\
            <meta charset=\"utf-8\">\
            <title>helloword</title>\
            </head>\
            <body>\
            <h1>hello word</h1>\
            </body>\
            </html> ";
        send(sock,response.c_str(),response.size(),0); 
        }   
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
