
#include<iostream>
#include<pthread.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
using namespace std;
int ticket=10000;
pthread_mutex_t lock;
void *thread_runing(void *rid){
//usleep(1000);
while(1){
pthread_mutex_lock(&lock);
if(ticket>0){
usleep(1000);
cout<<"I am "<<(char*)rid<<"我抢到第"<<ticket<<"张票了"<<endl;
ticket--;
pthread_mutex_unlock(&lock);
usleep(1000);
}else{

cout<<"票已售完"<<endl;
pthread_mutex_unlock(&lock);
break;
}

}


}
int main(){
pthread_mutex_init(&lock,NULL);//对互斥锁的初始化
pthread_t tid;
pthread_create(&tid,NULL,thread_runing,(void*)"thread 1");
pthread_t tid1;
pthread_t tid2;
pthread_create(&tid1,NULL,thread_runing,(void*)"thread 2");
pthread_create(&tid2,NULL,thread_runing,(void*)"thread 3");

void *ret=NULL;
pthread_join(tid,&ret);
cout<<"thread 1 quit code "<<(long long)ret<<endl;
void *ret1=NULL;
pthread_join(tid1,&ret1);
cout<<"thread 2 quit code "<<(long long)ret1<<endl;
void *ret2=NULL;
pthread_join(tid2,&ret);
cout<<"thread 3 quit code "<<(long long)ret<<endl;
pthread_mutex_destroy(&lock);//释放锁资源
return 0;

