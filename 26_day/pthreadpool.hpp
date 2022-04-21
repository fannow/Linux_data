#include<iostream>
#include<string.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<string>
#include<map>
#include<cstdio>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<queue>
using namespace std;
class test {
private:
	int sock;
  map<string,string> mp;

public:
	test() {}
  test(int a) :sock(a) {
    mp.insert(pair<string,string>("苹果","apple"));
    mp.insert(pair<string,string>("apple","a apple"));
    mp.insert(pair<string,string>("香蕉","banana"));
    mp.insert(pair<string,string>("红色","red"));
    mp.insert(pair<string,string>("黑色","block"));
    mp.insert(pair<string,string>("蓝色","blue"));
    mp.insert(pair<string,string>("绿色","green"));

  
  }
  void run(){
  char buff[64];
    size_t s=recv(sock,buff,sizeof(buff)-1,0);
    if(s>0){
      buff[s]=0;
      string str="";
   
      string sss=buff;
      cout<<"fan:"<<buff<<endl;
      if(mp.find(sss)!=mp.end()){
   cout<<"find"<<endl;     
        send(sock,mp[sss].c_str(),mp[sss].size(),0);
      }else{
        cout<<"no find"<<endl;
      send(sock,buff,strlen(buff),0);
      }
    }else if(s==0){
        cout<<"client have quited...."<<endl;
         // 因为服务器一直在运行如果不关闭可用的套接字将会越来越少。
        //break;
    }else if(s<0){
   cout<<"文件读取失败"<<endl;
    }
    
  
  }
  ~test() {
    close(sock);
  }
};
class ThreadPool {
private:
	queue<test*> q;
	int cap;//线程池数量
	pthread_mutex_t lock;
	pthread_cond_t cond;
	bool quit;//线程突出
public:
	bool isempty() {
		return q.size() == 0;
	}
	ThreadPool(int cap_=5) :cap(cap_), quit(false)
	{}
	void Init() {
		pthread_mutex_init(&lock, NULL);
		pthread_cond_init(&cond, NULL);
		pthread_t t;
		for (int i = 0; i < cap; i++) {

			pthread_create(&t, NULL, thread_running, (void*)this);

		}
	}

	void Lock() {
		pthread_mutex_lock(&lock);
	}
	void UnLock() {
		pthread_mutex_unlock(&lock);
	}
	void Waitcond() {
		pthread_cond_wait(&cond, &lock);//若没有直接阻塞自己
	}
	void threadswakeup() {

		pthread_cond_broadcast(&cond);//唤醒所有的线程
	}
	bool Quit() {

		return quit;
	}
	static void* thread_running(void* rid) {
		ThreadPool* this_d = (ThreadPool*)rid;
		while (true) {//检测是否达到线程退出条件
			//pthread_mutex_lock(&lock);
			this_d->Lock();
			while ( this_d->isempty()) {//检测有没有任务来
				//这里检测quit因为在线程退出后还会唤醒线程，要保证线程不退出才能接到任务
			//pthread_cond_wait(&cond);//若没有直接阻塞自己
				this_d->Waitcond();
			}
			test* t;
			

			this_d->out(&t);//提取出任务
			//只有保证线程不退出才能拿任务
			t->run();//执行任务在任务类中执行
			this_d->UnLock();
			//pthread_mutex_unlock(&lock);
      delete t;    
		}
  }
public:
	void put(test&t) {
		pthread_mutex_lock(&lock);
		q.push(&t);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond);;
	}
	void threadquit() {
		if (!isempty()) {//任务队列中有任务不能线程不能退出
			return;
		}
		quit = true;//线程退出条件
		threadswakeup();
	}
	void out(test**t) {
		test* a = q.front();
		*t = a;
		q.pop();

	}
	~ThreadPool() {

		pthread_mutex_destroy(&lock);
		pthread_cond_destroy(&cond);

	}
  
   void service(int sock){
    while(1){
    char buff[64];
    struct sockaddr_in add;
    socklen_t len=sizeof(add);
    size_t s=recv(sock,buff,sizeof(buff)-1,0);
    if(s>0){
    buff[s]=0;
    cout<<buff<<endl;
    string str="sever have...";
    send(sock,str.c_str(),sizeof(str)-1,0);

    }else if(s==0){
    cout<<"client have quited...."<<endl;
    break;
    close(sock);//关闭套接字因为服务器一直在运行如果不关闭可用的套接字将会越来越少。
    
    }else if(s<0){
   cout<<"文件读取失败"<<endl;
   break;
    }
    }
  }
};


