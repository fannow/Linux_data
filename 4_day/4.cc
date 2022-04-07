#include<iostream>
#include<stdlib.h>
#include<unistd.h>
using namespace std;
int main(){
cout<<"我是父进程 pid为："<<getpid()<<endl;

pid_t id=fork();
if(id==0){


cout<<"我是子进程 pid为："<<getpid()<<endl;
sleep(1);
}else if(id>0){
cout<<"我是父进程 pid为："<<getpid()<<endl;
sleep(1);
}

return 0;
}
