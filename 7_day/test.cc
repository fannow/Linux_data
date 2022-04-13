#include<iostream>
#include<stdio.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;
int main(){
//close(1);
int fp=open("fd2.txt",O_WRONLY | O_APPEND | O_CREAT);
//int fp1=open("fp1.txt",O_RDONLY | O_CREAT );
//int fp2=open("fp2.txt",O_RDONLY | O_CREAT);
if(fp<0){
cout<<"open fail"<<endl;
}
dup2(fp,1);
close(fp);
char str[]="hello word!\n";
char str1[]="hello China!\n";
char str2[]="hello shanxi\n";
write(1,str,strlen(str));
printf(str1);
fprintf(stdout,str2);
fork();
//fflush(stdout);
//fork();
close(fp);
return 0;
}
