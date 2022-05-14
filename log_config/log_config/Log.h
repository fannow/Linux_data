#pragma once
#pragma once
#ifndef __LOG_H__
#define __LOG_H__
#pragma warning( disable : 4996 )
#include<iostream>
#include<string>
#include<tuple>
#include<memory>
#include<vector>
#include<string>
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
#include"Signal.h"

#include"Until.h"
using namespace std;
class Formater;
class Logger;

/*
* ���ڿ���̨�����
*/
#define LOG_LEVEL(logger, level) \
    if(logger->getlevel() <= level) \
			LogEventWarp(LogEvent::ptr(new LogEvent(logger, level,__FILE__,\
			__LINE__, 0, 1,2, time(0)))).getSS()
#define LOG_DEBUG(logger) LOG_LEVEL(logger, LogLevel::DEBUG)
#define LOG_ERROR(logger) LOG_LEVEL(logger, LogLevel::ERROR)
#define LOG_FATAL(logger) LOG_LEVEL(logger, LogLevel::FATAL)
#define LOG_INFO(logger) LOG_LEVEL(logger, LogLevel::INFO)
#define LOG_WARN(logger) LOG_LEVEL(logger, LogLevel::WARN)




/*
* ����˵����__VA_ARGS__:�����ں��滻�����У���ʾ�ɱ�����б�
* �����ļ������
*
*/
#define FMT_LOG_LEVEL(logger, level,fmt,...) \
    if(logger->getlevel() <= level) \
			LogEventWarp(LogEvent::ptr(new LogEvent(logger, level,__FILE__,\
			__LINE__, 0, 1,2, time(0)))).getevent()->format(fmt,__VA_ARGS__)



#define FMT_LOG_DEBUG(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::DEBUG,fmt,__VA_ARGS__)
#define FMT_LOG_ERROR(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::ERROR,fmt,__VA_ARGS__)
#define FMT_LOG_FATAL(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::FATAL,fmt,__VA_ARGS__)
#define FMT_LOG_INFO(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::INFO,fmt,__VA_ARGS__)
#define FMT_LOG_WARN(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::WARN,fmt,__VA_ARGS__)


#define LOG_ROOT()  LogRoot::GetInstance()->getroot()

/*
* ��־����
*
*/
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
	static const char* Tostring(LogLevel::Level level);
};

/*
* ��־�¼�
* ��Ա������
* 	const char* m_file = NULL;			 �ļ���
*	int32_t m_line = 0;				     �к�
*	uint32_t m_please = 0;               ��������ʱ�䣨���룩
*	uint32_t m_threadid = 0;             �߳�id
*	uint32_t m_fiberId = 0;              Э��id
*	uint64_t m_time = 0;                 ʱ���
*	stringstream m_ss;				     ��ʽ��ʽ
* ����˵����
* vasprintf()����:int vasprintf (char **buf, const char *format, va_list ap)����ʽ�����ݴӿɱ�����б�д�뻺����
*
*/

class LogEvent {
public:
	//Ĭ�Ϲ��캯��
	LogEvent() {};
	LogEvent(shared_ptr<Logger> _ptr, LogLevel::Level level, const char* file, int32_t line,
		uint32_t please, uint32_t threadid, uint32_t fiberid, uint32_t time);

	typedef shared_ptr<LogEvent> ptr;

	const char* getfile() { return m_file; } //�����ļ�����
	int32_t getline() { return m_line; }      //�����к�
	uint32_t getplease() { return m_please; };//���س�������ʱ��
	uint32_t getthreadid() { return m_threadid; }//�����߳�id
	uint32_t getfiberid() { return m_fiberId; }//����Э��id
	uint64_t gettime() { return m_time; }//��������ʱ��
	string getss() { return m_ss.str(); }//������ʽ
	shared_ptr<Logger> GetLogger() { return logger; }
	stringstream& getSS() { return m_ss; }  //������ʽ���
	LogLevel::Level GetLevel() { return level; }//��ȡ��־����


	void format(const char* fmt, ...);	//��ʽ����
	void format(const char* fmt, va_list al);//��ʽ����   va_list:�ɱ�����б�


private:

	shared_ptr<Logger> logger;			//��־��	
	LogLevel::Level level;				//��־����
	const char* m_file = NULL;			//�ļ���
	int32_t m_line = 0;				    //�к�
	uint32_t m_please = 0;              //��������ʱ�䣨���룩
	uint32_t m_threadid = 0;            //�߳�id
	uint32_t m_fiberId = 0;             //Э��id
	uint64_t m_time = 0;                //ʱ���
	stringstream m_ss;				    //��ʽ��ʽ		

};





/*
*��־������
*/
class Formater {
public:
	typedef shared_ptr<Formater> ptr;
	Formater(string pattern);
	string format(shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event);
public:
	/*
	* ��־���Ƶ�Ԫ
	* ��ʽ������ģ�鸺�����ĸ�ʽ����
	*/
	class Item {
	public:
		typedef shared_ptr<Item> ptr;
		virtual ~Item() {}

		virtual void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) = 0;
	};

	void Init();//��ʼ��  �����û�����Ҫ�������־��ʽ
private:
	string m_pattern;        //
	vector<Item::ptr> m_items;

};


//��־���
class LogAppender {
public:

	typedef shared_ptr<LogAppender> ptr;
	virtual~LogAppender() {}
	virtual void Log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;//��־����ֱ���ò�ͬ�Ŀ�������Log�������ͬ�ĵط�
	void SetFormat(Formater::ptr val) { m_format = val; }//������־����
	Formater::ptr GetFormat() { return m_format; }  //������־��ʽ
	void setlevel(LogLevel::Level level) { m_level = level; }//������־����
	LogLevel::Level GetLevel() { return m_level; }//������־���

protected:

	LogLevel::Level m_level;//��־����
	Formater::ptr m_format;//�����־��ʽ//�����������־

};



/*
*��־��
*/
class Logger :public enable_shared_from_this<Logger> {
public:
	typedef shared_ptr<Logger> ptr;
	Logger(string _name = "root");
	void Log(LogLevel::Level level, LogEvent::ptr event);

	void AddAppender(LogAppender::ptr appender);//�����־�����
	void DelAppendrt(LogAppender::ptr appender);//ɾ����־�����

	/*
	*��־����
	*/
	void debug(LogEvent::ptr event);
	void info(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);


	LogLevel::Level getlevel() { return m_level; }//������־����
	string getname()const { return m_name; } //������־����


	void setleve(LogLevel::Level level) { m_level = level; }
	void setformat(Formater::ptr fmt) { format = fmt; }


private:
	string m_name;             //��־����
	LogLevel::Level m_level;	  //��־���������������ͻ������
	list<LogAppender::ptr> m_appender;  //��־�б�

	Formater::ptr format;  //��־��ʽ
};



/*
* ����̨�����
* override�������ڲ������õ����ͱ�����д�����麯������
*/
class StdOutAppender :public LogAppender {
public:
	typedef shared_ptr<StdOutAppender> ptr;
	virtual void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)override;//���������̨
private:
};


/*
* �ļ������
*/
class FileAppender :public LogAppender {
public:
	typedef shared_ptr<FileAppender> ptr;

	FileAppender(string filename) :
		m_file_name(filename) {
		//�ȴ��ļ�
		repoen();

	}

	void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;//������ļ�


	bool repoen();//���´���־�ļ�
private:
	string m_file_name;//��־�ļ�
	ofstream m_filestream;//�ļ���
};



/*
* ��־������
*/
class LogMessage {
public:
	LogMessage() {
		//Ĭ��Ϊstdout
		root.reset(new Logger());
		root->AddAppender(LogAppender::ptr(new StdOutAppender()));
	}
	map<string, Logger::ptr> mp;
	Logger::ptr root;
public:
	Logger::ptr getlogger(string name) {
		auto it = mp.find(name);
		return it == mp.end() ? root : it->second;
	}
	void Init();
	Logger::ptr getroot() { return root; }


};
/*
* ��־�¼���װ��
*
* LogEventWarp��Ϊһ����ʱ������ʹ�����ֱ��������������־ֱ��д�룬
*Ȼ����־����������ָ�룬��������������������棬���򲻽������޷��ͷ�
*/
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

typedef Signall<LogMessage> LogRoot;

#endif