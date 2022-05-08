#include<iostream>
#include<string>
#include<stdint>
#include<memory>
using namespace std;
//��־�¼�
class LogEvent {
public:
	//Ĭ�Ϲ��캯��
	LogEvent() {};
	typedef shared_ptr<LogEvent> ptr;
private:
	const char* NameFile = NULL;//��־�ļ�����
	int32_t MLine = 0;
	uint32_t MThreadId = 0;//�߳�id
	uint32_t MFiberId = 0;//Э��id
	uint64_t MTime = 0;//ʱ��
	string m_content;
	
};
//��־����
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
//��־��ʽ����
class Formattor {
public:
	typedef shared_ptr<Formattor> ptr;
private:

};
//��־���
class LogAppender {
public:
	typedef shared_ptr<LogAppender> ptr;
private:

};
//��־��
class Logger {
public:
	typedef shared_ptr<Logger> ptr;
private:

};
