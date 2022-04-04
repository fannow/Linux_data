#include<stdio.h>
#include<unistd.h>
int main(){
printf("一个分支\n");
pid_t ret=fork();
printf("pid:%d,ppid:%d,ret:%d\n",getpid(),getppid(),ret);
printf("两个分支\n");
return 0;
}
