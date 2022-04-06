#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

int tag=100;
int main(){

pid_t tar=fork();
if(tar==0){
while(1){

tag=1000;
printf("tag:%d,tag ader:%p\n",tag,&tag);
sleep(1);
}
}else if(tar>0){
while(1){

printf("tag :%d,tag adder:%p\n",tag,&tag);
sleep(1);
}
}
return 0;
}
