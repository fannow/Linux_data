#include<iostream>
#include<pthread.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
using namespace std;
pthread_mutex_t lock;
pthread_cond_t cond;
//void *thread1(void*rid)
void *thread1(void*rid)
{
while(1){
pthread_cond_wait(&cond,&lock);//cond条件不满足就等待
cout<<"I am "<<(char*)rid<<" 调用成功"<<endl;
}
}
void *thread2(void*rid)
{
while(1){
pthread_cond_signal(&cond);//唤醒等待的线程thread1
cout<<"I am "<<(char*)rid<<"调用线程thread1"<<endl;
sleep(1);
}
}
int main(){
pthread_mutex_init(&lock,NULL);
pthread_cond_init(&cond,NULL);

pthread_t t1,t2;
pthread_create(&t1,NULL,thread1,(void*)"thread1");
pthread_create(&t2,NULL,thread2,(void*)"thread2");




pthread_join(t1,NULL);
pthread_join(t2,NULL);



pthread_mutex_destroy(&lock);
pthread_cond_destroy(&cond);
return 0;
}
