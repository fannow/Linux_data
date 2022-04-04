#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
int ret = fork();
if(ret < 0){
perror("fork");
return 1;
}
else if(ret == 0){ //child

int count=5;
while(count){
printf("I am child : %d!, ret: %d\n", getpid(), ret);
sleep(100);
count--;
}
exit(0);
}else{ //father
while(1){
printf("I am father : %d!, ret: %d\n", getpid(), ret);
}}
sleep(100);
return 0;
}
