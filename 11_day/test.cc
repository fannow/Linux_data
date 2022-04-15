#include<iostream>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
using namespace std;
int main(){
	 int pipfd[2];
	pipe(pipfd);
	
	pid_t id=fork();
if(id==0){
//child 写入 一般是pipfd[0]为读pipfdp[1]为写
//所以要关闭pipfd[0]
close(pipfd[0]);
int count=0;
while(1){
const char*str="hello word!...............";
write(pipfd[1],str,strlen(str)-1);
cout<<"chiuld"<<count<<endl;
count++;
//if(count==5){
//close(pipfd[1]);
//break;
//}
}
exit(2);
}else{

//father
//父进程要关闭pipfd[1]
//
//
//
int count=0;
char buff[64];
while(1){
//s为实际读入多少字节
//sizeof(buff)是期望读入多少字节
sleep(1);
ssize_t s=read(pipfd[0],buff,sizeof(buff)-1);
cout<<"father 实际读取字节数;"<<s<<endl;
if(s>0){
buff[s]=0;
cout<<"father:"<<buff<<endl;
}
if(count++==3){
close(pipfd[0]);
break;

}
}
int status=0;
waitpid(id,&status,0);
cout<<"statu:"<<((status)&0x7f)<<endl;
}


	return 0;
}
