#include<iostream>
#include<queue>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>
using namespace std;
struct test{

public:
int a;
int b;
test(int a1,int b2):
a(a1),
b(b2)
{

}
};
class Pthread_Creat {
private:
	queue<struct test> que;
	size_t cap;
	pthread_mutex_t lock;
	pthread_cond_t cond;//生产者
	pthread_cond_t cond1;//消费者

public:

	bool isfull() {
	
		return que.size() >= cap;
	}
	bool isempty() {
	
		return que.empty();
	}
	Pthread_Creat(int cat) :cap(cat)
	{

		pthread_mutex_init(&lock, NULL);
		pthread_cond_init(&cond, NULL);
		pthread_cond_init(&cond1,NULL);
		
	}
	~Pthread_Creat() {
		pthread_mutex_destroy(&lock);
		pthread_cond_destroy(&cond);
		pthread_cond_destroy(&cond1);

	}
	void Put(struct test put) {
		pthread_mutex_lock(&lock);//
		if (isfull()) {//判断临界资源是否为满状态
			pthread_cond_signal(&cond);//唤醒消费者
			pthread_cond_wait(&cond1,&lock);//唤醒生产者
		
		}
		que.push(put);
		pthread_mutex_unlock(&lock);


	}
	//1．你为什么要等 ?
	//2．你怎么知道条件不满足 ?
	//3．你要判断，就必须保证进入临界区
	//4、持有锁进入的临界区!!
	//5. wait时，必须释放锁!6
	//	1。在调用pthread_cond_wait()该函教的时侯，自动会释放lock !
	//	//2当该函数被被返回时，返回到了临界区内，所以，该函数会让该线程重新持有该锁!!
	void Out(struct test&out) {
		pthread_mutex_lock(&lock);
		if (isempty()) {//判断临界资源是否为空状态
			pthread_cond_signal(&cond1);//唤醒生产者
			pthread_cond_wait(&cond, &lock);//组设消费者

		}
		out = que.front();
		que.pop();
		pthread_mutex_unlock(&lock);

	}

	};

pthread_mutex_t lock;
pthread_mutex_t lock1;

//生产者
void* thread1(void* rid) {
	Pthread_Creat* thread1 = (Pthread_Creat*)rid;
	while (true) {
pthread_mutex_lock(&lock);
		int data = rand() % 10 + 1;
		int da=rand()%100+1;
struct test t(data,da);
		thread1->Put(t);
		cout << "生产者数据为：" << data <<" "<<da<< endl;
pthread_mutex_unlock(&lock);
	//	sleep(3);
	}

}
//消费者
void* thread2(void* rid) {
	Pthread_Creat* thread1 = (Pthread_Creat*)rid;
	
	while (true) {
pthread_mutex_lock(&lock1);
struct test t(0,0);
		thread1->Out(t);
		int add=t.a+t.b;
		cout << "消费者拿到数据为：" << t.a<<" "<<t.b<<"和为： "<<add<< endl;
pthread_mutex_unlock(&lock1);
	//`:	sleep(1);
	}
}
	int main() {
		Pthread_Creat *thread=new Pthread_Creat(10);
		pthread_t t2;
		pthread_t t1;
		pthread_create(&t1, NULL, thread1, (void*)thread);
		pthread_create(&t2, NULL, thread2, (void*)thread);
		pthread_t t3;
		pthread_t t4;
		pthread_create(&t3, NULL, thread1, (void*)thread);
		pthread_create(&t4, NULL, thread2, (void*)thread);

		pthread_join(t1,NULL);
		pthread_join(t2,NULL);
		pthread_join(t3,NULL);
		pthread_join(t4,NULL);
pthread_mutex_destroy(&lock);
pthread_mutex_destroy(&lock1);
		return 0;

	}
