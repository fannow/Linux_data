#include<iostream>
#include <semaphore.h>
#include<vector>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>
using namespace std;
class thread{
private:
vector<int> v;
int cap;
sem_t sem1;//生产者
sem_t sem2;//消费者
int p_index;//生产者线程对应环形队列的位置
int c_index;//消费者。。。。。。。。。。。

public:
thread(int cap_=10):cap(cap_)
{
//cap=cap_;
v.reserve(cap);
p_index=0;
c_index=0;

sem_init(&sem1,0,10);
sem_init(&sem2,0,0);
}

void p(sem_t&sem){
sem_wait(&sem);

}
void V(sem_t&sem){

sem_post(&sem);

}
void put(int in){
p(sem1);

//放数据
v[c_index]=in;
c_index++;
c_index%=cap;
//cout<<sem2<<endl;
V(sem2);


}
void out(int &out){
p(sem2);
//取数据
out=v[p_index];
V(sem1);
p_index++;
p_index%=cap;
cout<<"---------"<<endl;
//V(sem1);
}
~thread(){
sem_destroy(&sem1);
sem_destroy(&sem2);
c_index=0;
p_index=0;
//index表明生产者和消费者决定使用临界资源的那一份
}
//private:
//vector<int> v;
//int cap;
//sem_t sem1;//生产者
//sem_t sem2;//消费者
//int p_index;//生产者线程对应环形队列的位置
//int c_index;//消费者。。。。。。。。。。。
};

//生产者
void*thread1(void*rid){
thread *threadd=(thread*)rid;

while(1){
int data=rand()%10+1;
threadd->put(data);
cout<<"生产者生产数据："<<data<<endl;
}
}
//消费者
void*thread2(void*rid){
thread *threadd=(thread*)rid;
cout<<"消费者"<<endl;
while(1){
int data=0;
threadd->out(data);
cout<<"消费者拿到数据："<<data<<endl;
sleep(1);
}
}
int main(){
thread *threadd=new thread(10);
pthread_t t1,t2;
pthread_create(&t1,NULL,thread1,(void*)threadd);

pthread_create(&t2,NULL,thread2,(void*)threadd);
pthread_join(t1,NULL);
pthread_join(t2,NULL);
}
