#include<iostream>
#include<string>
#include<tuple>
#include<cstdint>
#include<memory>
#include<vector>
#include<list>
#include<sstream>
#include<fstream>
#include<map>
#include<functional>
#include<time.h>
#include<cstdio>
using namespace std;
class Logger;
//日志事件
class LogEvent {
public:
	//默认构造函数
	LogEvent() {};
	LogEvent(const char *file,int32_t line,uint32_t please,uint32_t threadid,uint32_t fiberid ,uint32_t time) 
			:m_file(file),m_line(line),m_please(please),m_threadid(threadid),m_fiberId(fiberid),m_time(time) {};
	typedef shared_ptr<LogEvent> ptr;


	const char* getfile() {return m_file;}
	int32_t getline() { return m_line; }
	uint32_t getplease() { return m_please; };
	uint32_t getthreadid() { return m_threadid; }
	uint32_t getfiberid() { return m_fiberId; }
	uint64_t gettime() { return m_time; }
	string getss() { return m_ss.str(); }
	stringstream &getSS() { return m_ss; }
private:
	const char* m_file = NULL;      //文件名
	int32_t m_line = 0;				  //行号
	uint32_t m_please = 0;             //程序运行时间（毫秒）
	uint32_t m_threadid = 0;           //线程id
	uint32_t m_fiberId = 0;            //协程id
	uint64_t m_time = 0;               //时间戳
	stringstream m_ss;				  //		
	
};
//日志级别
class LogLevel {
public:
	typedef shared_ptr<LogLevel> ptr;
	enum Level {
		UNKNOW=0,
		DEBUG = 1,
		INFO=2 ,
		WARN =3,
		ERROR=4,
		FATAL=5
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
	string format(shared_ptr<Logger> logger,LogLevel::Level leve,LogEvent::ptr event);
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
		virtual void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) = 0;
	
		
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
	virtual void Log(shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event)=0;


	//设置日志格式
	void setformattor(Formattor::ptr val) { m_forattor = val; }
	//返回日志格式
	Formattor::ptr getformattor() { return m_forattor; }

protected:

	LogLevel::Level m_level;//日志级别
	Formattor::ptr m_forattor;//输出日志格式//将用于输出日志

};
//日志器
class Logger:public enable_shared_from_this<Logger> {
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

	string getname()const { return m_name; }

private:
	string m_name;             //日志名称
	LogLevel::Level m_level;	  //日志级别（满足这各级别就会输出）
	list<LogAppender::ptr> m_appender;  //日志列表

	Formattor::ptr format;
};

//输出到控制台stdout
class stdoutAppender :public LogAppender  {
public:
	typedef shared_ptr<stdoutAppender> ptr;
	virtual void Log(Logger::ptr logger,LogLevel::Level level, LogEvent::ptr event)override/*仅在类内部起作用的类型别名*/;//复写（即override）虚函数。
private:
};
//输出到日志文件中files
class FileAppender :public LogAppender {
public:
	FileAppender(string& filename) :
		m_file_name(filename) {}
	typedef shared_ptr<FileAppender> ptr;
	 void Log(Logger::ptr logger,LogLevel::Level level, LogEvent::ptr event) override/*仅在类内部起作用的类型别名*/;//复写（即override）虚函数。
	//重新打开文件，打开文件成功返回true
	bool repoen();
private:
	string m_file_name;//日志文件
	ofstream m_filestream;//文件流
};

	