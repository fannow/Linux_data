#include<iostream>
#include<iomanip>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
using namespace std;
void *thread_run(void *rid){
while(1){

cout<<"I am "<<(char*)rid<<"pid"<<getpid()<<"线程id"<<hex<<pthread_self()<<endl;
sleep(1);
}


}
 int main(){
pthread_t tid;
pthread_create(&tid,NULL,thread_run,(void *)"thread1");
while(1){
cout<<"I am main thread pid:"<<getpid()<<"线程id："<<hex<<pthread_self()<<endl;
sleep(1);
}
return 9;
}
