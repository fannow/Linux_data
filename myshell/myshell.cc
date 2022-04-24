#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
using namespace std;
const int size=256;
const int num=14;
int main(){
char cmd[size];
const char *host="[root@VM-0-4-centos]#";
while(1){
cmd[0]=0;
cout<<host;
fgets(cmd,size,stdin);//获取命令行参数
char*args[num];
cmd[strlen(cmd)-1]='\0';
args[0]=strtok(cmd," ");
int i=1;
while(1){
args[i]=strtok(NULL," ");
if(args[i]==NULL){
break;

}
i++;
}
pid_t id=fork();
if(id<0){
continue;
}
if(id==0){
execvp(args[0],args);
exit(1);
}
int status=0;
pid_t ret=waitpid(id,&status,0);
if(ret>0){
cout<<"进程退出码："<<((status>>8)&0xff)<<endl;

}
}



return 0;
}
