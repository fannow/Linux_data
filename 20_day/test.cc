#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<queue>
using namespace std;
class test{
private:
int base;
public:
//bool isempty(){
//return q.size()==0;
//}
test(){}
test(int a):base(a){}
void run(){


cout<<"test is running。。"<<base<<"线程id:"<<pthread_self()<<endl;
}
~test(){}
};
class ThreadPool{
public:
queue<test*> q;
int cap;//线程池数量
pthread_mutex_t lock;
pthread_cond_t cond;
public:
bool isempty(){
return q.size()==0;
}
ThreadPool(int cap_):cap(cap_)
{}
void Init(){
pthread_mutex_init(&lock,NULL);
pthread_cond_init(&cond,NULL);
pthread_t t;
for(int i=0;i<cap;i++){

pthread_create(&t,NULL,thread_running,(void*)this);

}
}

void Lock(){


pthread_mutex_lock(&lock);
}
void Unlock(){


pthread_mutex_unlock(&lock);
}
void Waitcond(){

pthread_cond_wait(&cond,&lock);//若没有直接阻塞自己

}
static void *thread_running(void*rid){
ThreadPool *this_d=(ThreadPool*)rid;
while(1){
//pthread_mutex_lock(&lock);
this_d->Lock();
while(this_d->isempty()){//检测有没有任务来
//pthread_cond_wait(&cond);//若没有直接阻塞自己
this_d->Waitcond();
}
test t;
this_d->out(t);//提取出任务
t.run();//执行任务在任务类中执行
this_d->Unlock();
//pthread_mutex_unlock(&lock);
}

}
void put(test&t){
pthread_mutex_lock(&lock);
q.push(&t);
pthread_mutex_unlock(&lock);
pthread_cond_signal(&cond);;
}

void out(test &t){
test *a=q.front();
t=*a;
q.pop();

}
~ThreadPool(){

pthread_mutex_destroy(&lock);
pthread_cond_destroy(&cond);

}
};
int main(){
ThreadPool *td=new ThreadPool(10);

td->Init();
while(1){

int s=rand()%10+1;
test t(s);
td->put(t);
sleep(1);
}
return 0;
}
