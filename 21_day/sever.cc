#include<sys/types.h>
#include<sstream>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<iostream>
#include<string>
#include<cstdio>
using namespace std;
class SocketSever{
private:
//string ip;
int post;
int sock;//文件描述符，创建套接字返回值
public:
SocketSever(int _post=8080):
post(_post)
{
}
void Init(){

sock=socket(AF_INET,SOCK_DGRAM,0);
cout<<"sock:"<<sock<<endl;
struct sockaddr_in in_addr;//IP地址和端口号在内核层面绑定
in_addr.sin_family=AF_INET;//设置协议家族一般为AF_INET IPV4
in_addr.sin_port=htons(post);//设置端口号，因为是网络通信所以要把主机序列转成网络序列
//in_addr.sin_addr.s_addr=inet_addr(ip.c_str());//设置IP地址,将字符串ip转成点分十进制的网络IP地址，因为string是一个类而inet_addr参数是一个字符串所以使用了c_str()转换。
in_addr.sin_addr.s_addr=INADDR_ANY;
if(bind(sock,(const struct sockaddr*)&in_addr,sizeof(in_addr))<0){

cout<<"绑定失败。。。"<<endl;
exit(1);
}
}
void star(){
 char buff[64];
while(1){
struct sockaddr_in addr;
socklen_t len=sizeof(addr);//实际读到的大小
ssize_t size=recvfrom(sock,buff,sizeof(buff)-1,0,(struct sockaddr*)&addr,&len);//接受
if(size>0){
char buf[16];
sprintf(buf,"%d",ntohs(addr.sin_port));
string IP_PORT=inet_ntoa(addr.sin_addr);
IP_PORT+="  ";//to_string(ntohs(addr.sin_port).str());
IP_PORT+=buf;
cout<<"client‘s IP and port is"<<IP_PORT<<endl;
buff[size]=0;
cout<<"client:"<<buff<<endl;
string str="sever 已经收到数据。。。";
size_t s=sendto(sock,str.c_str(),sizeof(str)-1,0,(struct sockaddr*)&addr,len);

}	
}
}
~SocketSever(){
close(sock);
}
};
int main(int argc,char *argv[]){
SocketSever ss(atoi(argv[1]));
ss.Init();
ss.star();
	
return 0;
}
