#include <stdio.h>
#include<sys/types.h>
int main(){
while(1){
printf("pid:%d\n",getpid());
printf("ppid:%d\n",getppid());

}
return ;
}
