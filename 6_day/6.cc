#include<iostream>
#include<unistd.h>
using namespace std;
int main(){
execl("/usr/bin/ls","ls","-a",NULL);
cout<<"11111"<<endl;
return 0;
}
