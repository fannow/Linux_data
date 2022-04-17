#include<iostream>
#include<pthread.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
using namespace std;
int a=10;
void *thread(void*rid){
int i=0;
while(1){
//pthread_detach(pthread_self());
//pthread_cancel(pthread_self());
sleep(1);
cout<<"I am "<<(char*)rid<<" pid:"<<getpid()<<"a address "<<&a<<" a:"<<a<<endl;
cout<<"I am "<<(char*)rid<<endl;
//break;
}
return (void*)10;
}
void *thread_runing(void*rid){

pthread_detach(pthread_self());
int i=0;
while(1){
//pthread_detach(pthread_self());
//int a=9;
//a/=0;
//if(i==3){
//break;
//pthread_cancel(pthread_self());
//}
sleep(1);

cout<<"I am "<<(char*)rid<<" pid:"<<getpid()<<"a address "<<&a<<" a:"<<a<<endl;
cout<<"I am "<<(char*)rid<<endl;
}
return (void*)10;
//exit(10);
//pthread_exit((void*)10);
}
int main(){
pthread_t tid;
pthread_t tid1;
pthread_create(&tid,NULL,thread_runing,(void*)"thread 1");
pthread_create(&tid,NULL,thread_runing,(void*)"thread 1");
pthread_create(&tid1,NULL,thread_runing,(void*)"thread 2");
//while(1){

cout<<"I am man thread"<<" pid:"<<getpid()<<"a address "<<&a<<" a:"<<a<<endl;
cout<<"I am main thread"<<endl;
cout<<"before a:"<<a<<endl;
sleep(10);
a=100;

cout<<"after a:"<<a<<endl;
//pthread_cancel(tid);
//break;
//}
void *ret=NULL;
pthread_join(tid,&ret);
cout<<"thread 1 quit code"<<(long long)ret<<endl;
void *ret1=NULL;
pthread_join(tid1,&ret1);
cout<<"thread 2 quit code"<<(long long)ret1<<endl;
return 0;

}

