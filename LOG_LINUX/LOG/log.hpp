#ifndef __LOG_H__
#define __LOG_H__
#pragma warning( disable : 4996 )
#include<iostream>
#include<string>
#include<tuple>
#include<memory>
#include<vector>
#include<list>
#include<sstream>
#include<fstream>
#include<string.h>
#include<map>
#include<functional>
#include<time.h>
#include<cstdio>
#include<map>
#include<stdarg.h>
#include"signal.h"
#include"until.hpp"
using namespace std;

//stdout����
#define LOG_LEVEL(logger, level) \
    if(logger->getlevel() <= level) \
			LogEventWarp(LogEvent::ptr(new LogEvent(logger, level,__FILE__,\
			__LINE__, 0, GetThreadId(),GetFiberId(), time(0)))).getSS()
#define LOG_DEBUG(logger) LOG_LEVEL(logger, LogLevel::DEBUG)
#define LOG_ERROR(logger) LOG_LEVEL(logger, LogLevel::ERROR)
#define LOG_FATAL(logger) LOG_LEVEL(logger, LogLevel::FATAL)
#define LOG_INFO(logger) LOG_LEVEL(logger, LogLevel::INFO)
#define LOG_WARN(logger) LOG_LEVEL(logger, LogLevel::WARN)

//�ļ�����
#define FMT_LOG_LEVEL(logger, level,fmt,...) \
    if(logger->getlevel() <= level) \
			LogEventWarp(LogEvent::ptr(new LogEvent(logger, level,__FILE__,\
			__LINE__, 0, 1,2, time(0)))).getevent()->format(fmt,__VA_ARGS__)

// __VA_ARGS__:�����ں��滻�����У���ʾ�ɱ�����б�

#define FMT_LOG_DEBUG(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::DEBUG,fmt,__VA_ARGS__)
#define FMT_LOG_ERROR(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::ERROR,fmt,__VA_ARGS__)
#define FMT_LOG_FATAL(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::FATAL,fmt,__VA_ARGS__)
#define FMT_LOG_INFO(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::INFO,fmt,__VA_ARGS__)
#define FMT_LOG_WARN(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::WARN,fmt,__VA_ARGS__)




class Logger;
//��־����
class LogLevel {
public:
	typedef shared_ptr<LogLevel> ptr;
	enum Level {
		UNKNOW = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};
	static const char* Tostring(LogLevel::Level level) {
		switch (level) {
			/*case UNKNOW:
				return "UNKNOW";
				break;
			case DEBUG:
				return "DEBUG";
				break;
			case INFO:
				return "INFO";
				break;
			case WARN:
				return "WARN";
				break;
			case ERROR:
				return "ERROR";
				break;
			case FATAL:
				return "FATAL";
				break;*/

				/*
				* #define XX(name)  name:  return #name;  break;

					XX(DEBUG);
				   ...
				#undef XX

				*/
#define XX(name)\
	case name:\
	return #name;\

			XX(DEBUG);
			XX(WARN);
			XX(INFO);
			XX(ERROR);
			XX(FATAL);
#undef XX
		default:
			return "UNKNOW";
		}
		return "UNKNOW";
	}

};



//��־�¼�
class LogEvent {
public:
	//Ĭ�Ϲ��캯��
	LogEvent() {};
	LogEvent(shared_ptr<Logger> _ptr, LogLevel::Level level, const char* file, int32_t line, uint32_t please, uint32_t threadid, uint32_t fiberid, uint32_t time)
		:logger(_ptr), level(level), m_file(file), m_line(line), m_please(please), m_threadid(threadid), m_fiberId(fiberid), m_time(time) {};
	typedef shared_ptr<LogEvent> ptr;


	const char* getfile() { return m_file; }
	int32_t getline() { return m_line; }
	uint32_t getplease() { return m_please; };
	uint32_t getthreadid() { return m_threadid; }
	uint32_t getfiberid() { return m_fiberId; }
	uint64_t gettime() { return m_time; }
	string getss() { return m_ss.str(); }
	//va_list:�ɱ�����б�
	/*
vasprintf()���� :int vasprintf (char **buf, const char *format, va_list ap)
����ʽ�����ݴӿɱ�����б�д�뻺����
*/	void format(const char* fmt,...);
	void format(const char* fmt, va_list al);
	//��ȡ��־��
	shared_ptr<Logger> GetLogger() { return logger; }
	stringstream& getSS() { return m_ss; }

	//��ȡ��־����
	LogLevel::Level GetLevel() { return level; }

private:
	//��־��
	shared_ptr<Logger> logger;
	//��־����
	LogLevel::Level level;

	const char* m_file = NULL;      //�ļ���
	int32_t m_line = 0;				  //�к�
	uint32_t m_please = 0;             //��������ʱ�䣨���룩
	uint32_t m_threadid = 0;           //�߳�id
	uint32_t m_fiberId = 0;            //Э��id
	uint64_t m_time = 0;               //ʱ���
	stringstream m_ss;				  //		

};


//��־��ʽ����
class Formattor {
public:
	typedef shared_ptr<Formattor> ptr;
	Formattor(string pattern) :
		m_pattern(pattern) {
		Init();
	}
	//ͨ��pattern������item�е���Ϣ

	//��־����
	string format(shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event);
public:
	//��־������ģ��
	class Item {
	public:
		typedef shared_ptr<Item> ptr;
		virtual ~Item() {}
		//��־��������

		/*
		* ostream��Ϊ���
		*/
		virtual void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) = 0;


	};
	//����ֱ�������һ����


	//��pattern����
	void Init();
private:
	string m_pattern;//���������־��һ����
	vector<Item::ptr> m_items;

};
//��־���
class LogAppender {
public:
	typedef shared_ptr<LogAppender> ptr;
	virtual~LogAppender() {}
	virtual void Log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;


	//������־��ʽ
	void setformattor(Formattor::ptr val) { m_forattor = val; }
	//������־��ʽ
	Formattor::ptr getformattor() { return m_forattor; }
	void setlevel(LogLevel::Level level) { m_level = level; }
	LogLevel::Level getlevel() { return m_level; }

protected:

	LogLevel::Level m_level;//��־����
	Formattor::ptr m_forattor;//�����־��ʽ//�����������־

};
//��־��

class Logger :public enable_shared_from_this<Logger> {
public:
	typedef shared_ptr<Logger> ptr;
	Logger(string _name = "root");
	void Log(LogLevel::Level level, LogEvent::ptr event);

	void AddAppender(LogAppender::ptr appender);//���
	void DelAppendrt(LogAppender::ptr appender);//ɾ��

	//��־����
	void debug(LogEvent::ptr event);
	void info(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);
	LogLevel::Level getlevel() { return m_level; }//��ȡleve
	void setleve(LogLevel::Level level) { m_level = level; }
	void setformat(Formattor::ptr fmt) { format = fmt; }
	string getname()const { return m_name; }

private:
	string m_name;             //��־����
	LogLevel::Level m_level;	  //��־���������������ͻ������
	list<LogAppender::ptr> m_appender;  //��־�б�

	Formattor::ptr format;
};

//���������̨stdout
class stdoutAppender :public LogAppender {
public:
	typedef shared_ptr<stdoutAppender> ptr;
	virtual void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)override/*�������ڲ������õ����ͱ���*/;//��д����override���麯����
private:
};
//�������־�ļ���files
class FileAppender :public LogAppender {
public:
	FileAppender(string filename) :
		m_file_name(filename) {
		//�ȴ��ļ�
		repoen();
	
	}
	typedef shared_ptr<FileAppender> ptr;
	void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override/*�������ڲ������õ����ͱ���*/;//��д����override���麯����
   //���´��ļ������ļ��ɹ�����true
	bool repoen();
private:
	string m_file_name;//��־�ļ�
	ofstream m_filestream;//�ļ���
};



//wrap��Ϊһ����ʱ������ʹ�����ֱ��������������־ֱ��д�룬Ȼ����־��������������ָ�룬��������������������棬���򲻽������޷��ͷ�
//��־�¼���װ��
class LogEventWarp {
public:

	LogEventWarp(LogEvent::ptr e)
		:m_event(e) {}
	~LogEventWarp() {
		m_event->GetLogger()->Log(m_event->GetLevel(), m_event);
	}
	shared_ptr<LogEvent>	getevent() { return m_event; }
	//��ȡ��־������
	stringstream& getSS() { return m_event->getSS(); }
private:
	//��־�¼�
	shared_ptr<LogEvent> m_event;


};

//��־������
class LogMessage {
public:
	LogMessage() {
		//Ĭ��Ϊstdout
		root .reset(new Logger());
		root->AddAppender(LogAppender::ptr(new stdoutAppender()));
	}
	map<string, Logger::ptr> mp;
	Logger::ptr root;
public:
	Logger::ptr getlogger(string name) { 
		auto it = mp.find(name);
		return it == mp.end() ? root : it->second;
	}
	void Init();


};

typedef Signalla<LogMessage> LogRoot;

#endif // !__LOG_H__