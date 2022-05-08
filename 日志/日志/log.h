#include<iostream>
#include<string>
#include<stdint>
#include<memory>
using namespace std;
//日志事件
class LogEvent {
public:
	//默认构造函数
	LogEvent() {};
	typedef shared_ptr<LogEvent> ptr;
private:
	const char* NameFile = NULL;//日志文件名字
	int32_t MLine = 0;
	uint32_t MThreadId = 0;//线程id
	uint32_t MFiberId = 0;//协程id
	uint64_t MTime = 0;//时间
	string m_content;
	
};
//日志级别
class LogLevel {
public:
	typedef shared_ptr<LogLevel> ptr;
private:
	enum Level {
		DEBUG = 1;
		INFO = 2;
		WARN = 3;
		ERROR = 4;
		FATAL = 5;
	};
};
//日志格式控制
class Formattor {
public:
	typedef shared_ptr<Formattor> ptr;
private:

};
//日志输出
class LogAppender {
public:
	typedef shared_ptr<LogAppender> ptr;
private:

};
//日志器
class Logger {
public:
	typedef shared_ptr<Logger> ptr;
private:

};
