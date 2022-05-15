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

//stdout启动
#define LOG_LEVEL(logger, level) \
    if(logger->getlevel() <= level) \
			LogEventWarp(LogEvent::ptr(new LogEvent(logger, level,__FILE__,\
			__LINE__, 0, GetThreadId(),GetFiberId(), time(0)))).getSS()
#define LOG_DEBUG(logger) LOG_LEVEL(logger, LogLevel::DEBUG)
#define LOG_ERROR(logger) LOG_LEVEL(logger, LogLevel::ERROR)
#define LOG_FATAL(logger) LOG_LEVEL(logger, LogLevel::FATAL)
#define LOG_INFO(logger) LOG_LEVEL(logger, LogLevel::INFO)
#define LOG_WARN(logger) LOG_LEVEL(logger, LogLevel::WARN)

//文件启动
#define FMT_LOG_LEVEL(logger, level,fmt,...) \
    if(logger->getlevel() <= level) \
			LogEventWarp(LogEvent::ptr(new LogEvent(logger, level,__FILE__,\
			__LINE__, 0, 1,2, time(0)))).getevent()->format(fmt,__VA_ARGS__)

// __VA_ARGS__:用于在宏替换部分中，表示可变参数列表；

#define FMT_LOG_DEBUG(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::DEBUG,fmt,__VA_ARGS__)
#define FMT_LOG_ERROR(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::ERROR,fmt,__VA_ARGS__)
#define FMT_LOG_FATAL(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::FATAL,fmt,__VA_ARGS__)
#define FMT_LOG_INFO(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::INFO,fmt,__VA_ARGS__)
#define FMT_LOG_WARN(logger,fmt,...) FMT_LOG_LEVEL(logger, LogLevel::WARN,fmt,__VA_ARGS__)




class Logger;
//日志级别
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



//日志事件
class LogEvent {
public:
	//默认构造函数
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
	//va_list:可变参数列表
	/*
vasprintf()函数 :int vasprintf (char **buf, const char *format, va_list ap)
将格式化数据从可变参数列表写入缓冲区
*/	void format(const char* fmt,...);
	void format(const char* fmt, va_list al);
	//获取日志器
	shared_ptr<Logger> GetLogger() { return logger; }
	stringstream& getSS() { return m_ss; }

	//获取日志级别
	LogLevel::Level GetLevel() { return level; }

private:
	//日志器
	shared_ptr<Logger> logger;
	//日志级别
	LogLevel::Level level;

	const char* m_file = NULL;      //文件名
	int32_t m_line = 0;				  //行号
	uint32_t m_please = 0;             //程序运行时间（毫秒）
	uint32_t m_threadid = 0;           //线程id
	uint32_t m_fiberId = 0;            //协程id
	uint64_t m_time = 0;               //时间戳
	stringstream m_ss;				  //		

};


//日志格式控制
class Formattor {
public:
	typedef shared_ptr<Formattor> ptr;
	Formattor(string pattern) :
		m_pattern(pattern) {
		Init();
	}
	//通过pattern解析出item中的信息

	//日志解析
	string format(shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event);
public:
	//日志解析子模块
	class Item {
	public:
		typedef shared_ptr<Item> ptr;
		virtual ~Item() {}
		//日志解析函数

		/*
		* ostream作为输出
		*/
		virtual void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) = 0;


	};
	//子类分别负责具体的一部风


	//对pattern解析
	void Init();
private:
	string m_pattern;//输出的是日志得一部分
	vector<Item::ptr> m_items;

};
//日志输出
class LogAppender {
public:
	typedef shared_ptr<LogAppender> ptr;
	virtual~LogAppender() {}
	virtual void Log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;


	//设置日志格式
	void setformattor(Formattor::ptr val) { m_forattor = val; }
	//返回日志格式
	Formattor::ptr getformattor() { return m_forattor; }
	void setlevel(LogLevel::Level level) { m_level = level; }
	LogLevel::Level getlevel() { return m_level; }

protected:

	LogLevel::Level m_level;//日志级别
	Formattor::ptr m_forattor;//输出日志格式//将用于输出日志

};
//日志器

class Logger :public enable_shared_from_this<Logger> {
public:
	typedef shared_ptr<Logger> ptr;
	Logger(string _name = "root");
	void Log(LogLevel::Level level, LogEvent::ptr event);

	void AddAppender(LogAppender::ptr appender);//添加
	void DelAppendrt(LogAppender::ptr appender);//删除

	//日志级别
	void debug(LogEvent::ptr event);
	void info(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);
	LogLevel::Level getlevel() { return m_level; }//获取leve
	void setleve(LogLevel::Level level) { m_level = level; }
	void setformat(Formattor::ptr fmt) { format = fmt; }
	string getname()const { return m_name; }

private:
	string m_name;             //日志名称
	LogLevel::Level m_level;	  //日志级别（满足这各级别就会输出）
	list<LogAppender::ptr> m_appender;  //日志列表

	Formattor::ptr format;
};

//输出到控制台stdout
class stdoutAppender :public LogAppender {
public:
	typedef shared_ptr<stdoutAppender> ptr;
	virtual void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)override/*仅在类内部起作用的类型别名*/;//复写（即override）虚函数。
private:
};
//输出到日志文件中files
class FileAppender :public LogAppender {
public:
	FileAppender(string filename) :
		m_file_name(filename) {
		//先打开文件
		repoen();
	
	}
	typedef shared_ptr<FileAppender> ptr;
	void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override/*仅在类内部起作用的类型别名*/;//复写（即override）虚函数。
   //重新打开文件，打开文件成功返回true
	bool repoen();
private:
	string m_file_name;//日志文件
	ofstream m_filestream;//文件流
};



//wrap作为一个零时对象，在使用完后直接析构，触发日志直接写入，然而日志本身是智能智能指针，如果在声明在主函数里面，程序不结束将无法释放
//日志事件包装器
class LogEventWarp {
public:

	LogEventWarp(LogEvent::ptr e)
		:m_event(e) {}
	~LogEventWarp() {
		m_event->GetLogger()->Log(m_event->GetLevel(), m_event);
	}
	shared_ptr<LogEvent>	getevent() { return m_event; }
	//获取日志内容流
	stringstream& getSS() { return m_event->getSS(); }
private:
	//日志事件
	shared_ptr<LogEvent> m_event;


};

//日志管理器
class LogMessage {
public:
	LogMessage() {
		//默认为stdout
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