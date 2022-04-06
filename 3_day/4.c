#include<stdio.h>
#include<stdlib.h>

int g_adder=100;
int g;
int main(int argc,char*argv[],char*env[]){
printf("code adder:%p\n",main);
printf("g addr:%p\n",&g_adder);
printf("g uniunt adder:%p\n",&g);
char *newcode=(char*)malloc(10);
printf("heap adder:%p\n",newcode);
printf("stack adder:%p\n",&newcode);
printf("opt adder:%p\n",argv[0]);
printf("opt adder:%p\n",argv[argc-1]);
printf("env adder:%p\n",env[0]);

return 0;
}
