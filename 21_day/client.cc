#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<iostream>
#include<string>
using namespace std;
class Socketclient{
private:
string ip;
int post;
int sock;//文件描述符，创建套接字返回值
public:
//ip是对应client的IP地址和端口号
Socketclient(string ip_="127.0.0.1",int _post=8080):
ip(ip_),
post(_post)
{
}
//客户端不需要绑定只需创建套接字
void Init(){

sock=socket(AF_INET,SOCK_DGRAM,0);
cout<<"sock:"<<sock<<endl;
//struct sockaddr_in in_addr;//IP地址和端口号在内核层面绑定
//in_addr.sin_family=AF_INET;//设置协议家族一般为AF_INET IPV4
//in_addr.sin_port=htons(post);//设置端口号，因为是网络通信所以要把主机序列转成网络序列
//in_addr.sin_addr.s_addr=inet_addr(ip.c_str());//设置IP地址,将字符串ip转成点分十进制的网络IP地址，因为string是一个类而inet_addr参数是一个字符串所以使用了c_str()转换。
//if(bind(sock,(const struct sockaddr*)&in_addr,sizeof(in_addr))<0){

//cout<<"绑定失败。。。"<<endl;
//exit(1);
//}
}
//client是先收后发，client是先发后收
void star(){
 //char buff[64];
 string str;
while(1){
cout<<"请输入要发送的内容：";
cin>>str;
struct sockaddr_in in_addr;//IP地址和端口号在内核层面绑定
in_addr.sin_family=AF_INET;//设置协议家族一般为AF_INET IPV4

in_addr.sin_port=htons(post);//设置端口号，因为是网络通信所
in_addr.sin_addr.s_addr=inet_addr(ip.c_str());//设置IP地址,将字符串ip转成点分十进制的网络IP地址，因为string是一个类而inet_addr参数是一个字符串所以使用了c_str()转换。以要把主机序列转成网络序列
//
size_t s=sendto(sock,str.c_str(),sizeof(str)-1,0,(struct sockaddr*)&in_addr,sizeof(in_addr));
if(s<0){
cout<<"传输错误"<<endl;
}
char buff[64];
struct sockaddr_in addr;
socklen_t t=sizeof(addr);
ssize_t size=recvfrom(sock,buff,sizeof(buff)-1,0,(struct sockaddr*)&addr,&t);
if(size>0){
buff[size]=0;
cout<<"服务器反馈。。。"<<endl;
}
}

//struct sockaddr_in addr;
//socklen_t t struct sockaddr*)&in_addr,sizeof(in_addr)en=sizeof(addr);//实际读到的大小
//ssize_t size=recvfrom(sock,buff,sizeof(buff)-1,0,(struct sockaddr*)&addr,&len);//接受
//if(size>0){
//buff[size]=0;
//cout<<"client:"<<buff<<endl;
//string str="client 已经收到数据。。。";
//size_t s=sendto(sock,str.c_str(),sizeof(str)-1,0,(struct sockaddr*)&addr,len);
//}	
//}
}
~Socketclient(){
close(sock);
}
};
void up(string str){
cout<<str<<"请输入IP地址和端口号"<<endl;

}

int main(int argc,char *argv[]){
if(argc!=3){
up(argv[0]);
exit(1);
}

Socketclient ss(argv[1],atoi(argv[2]));
ss.Init();
ss.star();
	
return 0;
}
