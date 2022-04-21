#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<cstdio>
#include<sys/types.h>
#include<string.h>
using namespace std;
#define NUM sizeof(fd_set)*8
class SelectServer{
private:
  int post;
  int listen_sock;//监听套接字
int fd_set_arrar[NUM];//将要关注的套接字存进一个数组方便操作
public:
  SelectServer(int _post=8080){
  post=_post;
  }
  void Init(){
    //创建监听套接字
    listen_sock=socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock<0){
      cerr<<"socket error。。。"<<endl;
      exit(0);
    }
    struct sockaddr_in in_addr;
    in_addr.sin_family=AF_INET;
    in_addr.sin_port=htons(post);
    in_addr.sin_addr.s_addr=INADDR_ANY;
    int op=1;
    setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));//端口复用

    if(bind(listen_sock,(const struct sockaddr*)&in_addr,sizeof(in_addr))<0){
        cerr<<"bind error....."<<endl;
        exit(1);
        }
    if(listen(listen_sock,5)<0){
      cerr<<"listen error。。。"<<endl;
      exit(2);
    }
    for(int i=0;i<NUM;i++){
      fd_set_arrar[i]=-1;
    }
    fd_set_arrar[0]=listen_sock;//将监听套接字存进套接字数组方便更新
    
  }


  void severic(fd_set *fd){
    for(int i=0;i<NUM;i++){
      if(fd_set_arrar[i]==-1){
        //无效套接字
        continue;
      }
      //检测套接字数组中有没有就绪套接字
      //读就绪分为底层连接就绪和普通读就绪事件
      if(FD_ISSET(fd_set_arrar[i],fd)){
        //读条件就绪
  cout<<"获取到就绪套接字"<<fd_set_arrar[i]<<endl;
      if(fd_set_arrar[i]==listen_sock){
      //底层建立连接也是读操作
      //所以也要建测是不是监听套接字
      //来了一个链接事件
      //所以要获取链接
      struct sockaddr_in addr;
      socklen_t len=sizeof(addr);
      cout<<"监听套接字"<<fd_set_arrar[i]<<endl;
       int sock=accept(fd_set_arrar[i],(struct sockaddr*)&addr,&len);
       if(sock<0){
         cerr<<"获取连接失败"<<endl;

       }
       cout<<"get a new link..."<<endl;
       //将新建套接字添加到套接字数组
       Add_arrayy(sock);
      }
       else{
        //来了一个普通就绪读事件
        char buff[1024];
        int s=recv(fd_set_arrar[i],buff,sizeof(buff),0);
        if(s>0){
          buff[s]=0;
          cout<<"client##"<<buff<<endl;

        }
        //任务完成关闭对应的文件描述符；
        close(fd_set_arrar[i]);
        //将关闭的文件描述符从数组中删除,传入对应的索引
        DeleteSock(i);
      }
      }
      
      
    }
  }
  void DeleteSock(int index){
    if(index<=NUM&&index>=0){
      fd_set_arrar[index]=-1;
  }
  }
  void Add_arrayy(int sock){
    int i=0;
    for(;i<NUM;i++){
      if(fd_set_arrar[i]==-1){
        break;
      }
    }
    if(i>=NUM){
      cout<<"fd_set_arrar hava full..."<<endl;
      close(sock);
    }else{
    fd_set_arrar[i]=sock;
    cout<<"sock :"<<sock<<"add arrar success"<<endl;
   }
  }
  void strar(){
    //因为服务器在启动时默认打开一个网络套接字，所以在maxfd子初始赋值为监听套接字的值
   // int  maxfd=listen_sock;
   // 
   int maxfd=-1;
    
    struct timeval timeout={5,0};//每5等待第一次
    while(1){
      fd_set rnfds;
      //清空rnfds
      
      FD_ZERO(&rnfds);
      //设置rnfds
    
      //每次对fdns重新设定
        cout<<"fd_set_array:";
      for(int i=0;i<NUM;i++){
        if(fd_set_arrar[i]!=-1){
        FD_SET(fd_set_arrar[i],&rnfds);
        //重新设定maxfd
        cout<<fd_set_arrar[i];
        if(maxfd<fd_set_arrar[i]){
          maxfd=fd_set_arrar[i];
        }

      }
    }
      cout<<endl;
     timeout={5,0};//因为既是输入型参数，又是输出型参数 如果等待超时返回值为0就会将等待时间置为0
    // 所以必须在每次select重新设置
    //
    cout<<"selsect begin"<<endl;
       switch(select(maxfd+1,&rnfds,NULL,NULL,NULL)){
         case 0:
           cout<<"等待时间超时"<<endl;
           break;
         case -1:
           cout<<"slect error"<<endl;
           break;
         default:
          severic(&rnfds);
           break;

       }
    }
  }   
  ~SelectServer(){
    close(listen_sock);
  }
};
void openov(string rip){
cout<<rip<<endl;
}
int main(int argc,char*argv[]){
  if(argc!=2){
  openov(argv[1]);
  exit(0);
  }
SelectServer *ss=new SelectServer(atoi(argv[1]));
ss->Init();
ss->strar();
  return 0;
}
