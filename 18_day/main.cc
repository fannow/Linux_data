#include<iostream>
#include<queue>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>
using namespace std;
class Pthread_Creat{
private:
queue<int> que;
pthread_mutex_t lock;
pthread_cond_t cond;
pthread_cond_t cond1;
pthread_t t2;
pthread_t t1;
public:
Pthread_Creat(int cat):que(cat)
{

pthread_mutex_init(&lock,NUll);
pthread_cond_init(&cond,NULL);
pthread_cond_init(&cond1,NULL);
pthread_create(&t1,NULL,thread1,(void*)"thread1");
pthread_create(&t2,NULL,thread2,(void*)"thread2");
}
void Destroy(){
pthread_mutex_destroy(&lock);
pthread_cond_destroy(&cond);
pthread_cond_destroy(&cond1);

}
void Put(int put){


}
void In(int in){


}
void Join(){
pthread_join(t1,NULL);
pthread_join(t2,NULL);
}
void*thread1(void*rid){
while(1){
cout<<"I am"<<(void*)rid<<endl;
sleep(1);
}
}
void*thread2(void*rid){
while(1){

cout<<"I am"<<(void*)rid<<endl;
sleep(1);
}
};


int main(){
Pthread_Creat thread1(10);
thread.Join();
thread.Destroy();
return 0;

}
