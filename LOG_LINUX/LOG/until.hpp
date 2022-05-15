#pragma once
#ifndef __UNTIL_H__
#define __UNTIL_H__
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<iostream>
#include<cstdint>
using namespace std;
//获取线程id
pid_t GetThreadId() {
	return syscall(SYS_gettid);
}
//协程id
uint32_t GetFiberId() {
	return 0;
}

#endif // !__UNTIL_H__
