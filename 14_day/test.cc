#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
using namespace std;
void handler(int sign){
sleep(1);
cout<<"捕捉到"<<sign<<"号信号"<<endl;
}
int main(){

while(1){
signal(11,handler);
sleep(1);
//raise(9);
//abort();
int *b;
*b=100;
sleep(1);
//cout<<"this is running..."<<endl;
}
return 0;
}
