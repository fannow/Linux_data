#include<iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/shm.h>
using namespace std;
int main(){
key_t k=ftok("./",0x666666);
cout<<"k"<<k<<endl;
int shat=shmget(k,4096,IPC_CREAT|IPC_EXCL|0666);//创建共享内存
if(shat<0){

cout<<"share 创建失败!"<<endl;
return  1;
}

//jianl
//建立宿舍关系 
char *str=(char*)shmat(shat,NULL,0);
while(1){
sleep(1);
cout<<str<<endl;
}
shmdt(str);
shmat(shat,IPC_RMID,NULL);//删除共享内存
return 0;
}
