#include<iostream>
	#include<pthread.h>
	#include<stdlib.h>
	#include<unistd.h>
	#include<sys/types.h>
	using namespace std;
	void *thread_runing(void*rid){
	int i=0;
	while(1){
	pthread_detach(pthread_self());
	sleep(1);
	cout<<"I am "<<(char*)rid<<endl;
	break;
	}
	return (void*)10;
	
	}
	int main(){
	pthread_t tid;
	pthread_create(&tid,NULL,thread_runing,(void*)"thread 1");
	while(1){
	
	cout<<"I am main thread"<<endl;
	sleep(4);
	break;
	}
	void *ret=NULL;
	pthread_join(tid,&ret);
	cout<<"thread 1 quit code"<<(long long)ret<<endl;
	return 0;
	
	}
