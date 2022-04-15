#include<iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
using namespace std;
int main(){
key_t k=ftok("./",0x666666);
int shat=shmget(k,4096,0);//创建共享内存
if(shat<0){

cout<<"share 创建失败!"<<endl;
return 1;
}

//jianl
//建立宿舍关系
char *str=(char*)shmat(shat,NULL,0);
char ch='a';
for(;ch<='z';ch++){
str[ch-'a']=ch;
sleep(5);

}
shmdt(str);
shmat(shat,IPC_RMID,NULL);//删除共享内存
return 0;
}
