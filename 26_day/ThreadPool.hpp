#pragma once

#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>

#define NUM 5

class Task{
    public:
      //  int base;
        int sock;
        std::map<std::string, std::string> dict;
    public:
        Task(){}
        Task(int _sock)
            :sock(_sock)
        {
            dict.insert(std::pair<std::string, std::string>("apple","苹果"));
        }

        void Run()
        {
            char buffer[64];
            std::cout << "task is running " << std::endl;
             size_t s = recv(sock, buffer, sizeof(buffer)-1, 0);
             if(s > 0){
                 buffer[s] = 0;
                 std::string key = buffer;
                 std::cout<<"介绍成功"<<std::endl;
                 std::cout<<buffer<<std::endl;
                 std::cout<<dict[key]<<std::endl;
                 send(sock, dict[key].c_str(), dict[key].size(), 0);
             }
             else if(s == 0){
                 std::cout << "client quit ...." << std::endl;
             }
             else {
                 std::cout << "recv client data error..." << std::endl;
             }
            //std::cout <<"thread is[" << pthread_self() << "] task run ... done: base# "<< base << " pow is# "<< pow(base,2) << std::endl;
        }
        ~Task(){
            std::cout << "server close sock :" << sock << std::endl;
            close(sock);
        }
};

class ThreadPool{
    private:
        std::queue<Task*> q;
        int max_num;
        pthread_mutex_t lock;
        pthread_cond_t cond; //only consumer, thread pool thread;
        //bool quit;
    public:
        void LockQueue()
        {
            pthread_mutex_lock(&lock);
        }
        void UnlockQueue()
        {
            pthread_mutex_unlock(&lock);
        }
        bool IsEmpty()
        {
            return q.size() == 0;
        }
        void ThreadWait()
        {
            pthread_cond_wait(&cond, &lock);
        }
        void ThreadWakeup()
        {
            //if(low_water > 30){
            //    pthread_cond_broadcast(&cond);
            //}
            pthread_cond_signal(&cond);
        }
        void ThreadsWakeup()
        {
            pthread_cond_broadcast(&cond);
        }
    public:
        ThreadPool(int _max=NUM):max_num(_max)//,quit(false)
        {}
        static void *Routine(void *arg) //
        {
            ThreadPool *this_p = (ThreadPool*)arg;
            while(true){
                this_p->LockQueue();
                while(this_p->IsEmpty()){
                    this_p->ThreadWait();
                }

                Task *t;
                this_p->Get(&t);
                this_p->UnlockQueue();
                t->Run();
                delete t;
            }
        }
        void ThreadPoolInit()
        {
            pthread_mutex_init(&lock, NULL);
            pthread_cond_init(&cond,NULL);
            pthread_t t;
            for(int i = 0; i < max_num; i++){
                pthread_create(&t, NULL, Routine, this);
            }
        }
        //server
        void Put(Task &in)
        {
            LockQueue();
            q.push(&in);
            UnlockQueue();

            ThreadWakeup();
        }
        //Thread pool t;
        void Get(Task **out)
        {
            Task*t = q.front();
            q.pop();
            *out = t;
        }
        void ThreadQuit()
        {
            //if(!IsEmpty()){
            //    std::cout << "task queue is not empty" << std::endl;
            //    return;
            //}
            //quit = true;
            //ThreadsWakeup();
        }
        ~ThreadPool()
        {
            pthread_mutex_destroy(&lock);
            pthread_cond_destroy(&cond);
        }

};









