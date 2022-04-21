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
    mp.insert(pair<string,string>("ƻ��","apple"));
    mp.insert(pair<string,string>("apple","a apple"));
    mp.insert(pair<string,string>("�㽶","banana"));
    mp.insert(pair<string,string>("��ɫ","red"));
    mp.insert(pair<string,string>("��ɫ","block"));
    mp.insert(pair<string,string>("��ɫ","blue"));
    mp.insert(pair<string,string>("��ɫ","green"));

  
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
         // ��Ϊ������һֱ������������رտ��õ��׽��ֽ���Խ��Խ�١�
        //break;
    }else if(s<0){
   cout<<"�ļ���ȡʧ��"<<endl;
    }
    
  
  }
  ~test() {
    close(sock);
  }
};
class ThreadPool {
private:
	queue<test*> q;
	int cap;//�̳߳�����
	pthread_mutex_t lock;
	pthread_cond_t cond;
	bool quit;//�߳�ͻ��
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
		pthread_cond_wait(&cond, &lock);//��û��ֱ�������Լ�
	}
	void threadswakeup() {

		pthread_cond_broadcast(&cond);//�������е��߳�
	}
	bool Quit() {

		return quit;
	}
	static void* thread_running(void* rid) {
		ThreadPool* this_d = (ThreadPool*)rid;
		while (true) {//����Ƿ�ﵽ�߳��˳�����
			//pthread_mutex_lock(&lock);
			this_d->Lock();
			while ( this_d->isempty()) {//�����û��������
				//������quit��Ϊ���߳��˳��󻹻ỽ���̣߳�Ҫ��֤�̲߳��˳����ܽӵ�����
			//pthread_cond_wait(&cond);//��û��ֱ�������Լ�
				this_d->Waitcond();
			}
			test* t;
			

			this_d->out(&t);//��ȡ������
			//ֻ�б�֤�̲߳��˳�����������
			t->run();//ִ����������������ִ��
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
		if (!isempty()) {//������������������̲߳����˳�
			return;
		}
		quit = true;//�߳��˳�����
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
    close(sock);//�ر��׽�����Ϊ������һֱ������������رտ��õ��׽��ֽ���Խ��Խ�١�
    
    }else if(s<0){
   cout<<"�ļ���ȡʧ��"<<endl;
   break;
    }
    }
  }
};


