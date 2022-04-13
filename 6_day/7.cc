#include<iostream>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
using namespace std;
int main(){
pid_t id=fork();
if(id==0){
execl("/usr/bin/ls","ls","-a",NULL);
exit(0);
}
pid_t ret=waitpid(id,NULL,0);
if(ret>0){
cout<<"ret success"<<endl;
}
return 0;
}
