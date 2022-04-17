#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
int main(int argv,char *argc[]){

//kill(atoi(argc[1]),atoi(argc[2]));
//raise(9);
alarm(1);
int count=0;
while(1){

cout<<"running"<<count<<endl;
count++;
}
return 0;
}
