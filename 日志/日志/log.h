#include<iostream>
#include<string>
#include<tuple>
#include<stdint>
#include<memory>
#include<vector>
#include<list>
#include<stringstreams>
#include<fstream>

using namespace std;
//日志事件
class LogEvent {
public:
	//默认构造函数
	LogEvent() {};
	typedef shared_ptr<LogEvent> ptr;
private:
	const char* NameFile = NULL;      //文件名
	int32_t MLine = 0;				  //行号
	uint32_t MElpase = 0;             //程序运行时间（毫秒）
	uint32_t MThreadId = 0;           //线程id
	uint32_t MFiberId = 0;            //协程id
	uint64_t MTime = 0;               //时间戳
	string MContent;				  //		
	
};
//日志级别
class LogLevel {
public:
	typedef shared_ptr<LogLevel> ptr;
	enum Level {
		DEBUG = 1;
		INFO = 2;
		WARN = 3;
		ERROR = 4;
		FATAL = 5;
	};
private:
	
};
//日志格式控制
class Formattor {
public:
	typedef shared_ptr<Formattor> ptr;
	Formattor(string& pattern) :
		m_pattern(pattern) {}
	//通过pattern解析出item中的信息

	//日志解析
	string format(LogEvent::ptr event);
private:
	//日志解析子模块
	class Item {
	public:
		typedef shared_ptr<Item> ptr;
		virtual ~Item() {}
		//日志解析函数

		/*
		* ostream作为输出
		*/
		virtual void format(ostream os,LogEvent::ptr event) = 0;
		
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
	virtual void Log(LogLevel::Level level, LogEvent::ptr event)=0;


	//设置日志格式
	void setformattor(Formattor::ptr val) { m_forattor = val; }
	//返回日志格式
	Formattor::ptr getformattor() { return m_forattor; }

protected:

	LogLevel::ptr m_level;//日志级别
	Formattor::ptr m_forattor;//输出日志格式//将用于输出日志

};
//日志器
class Logger {
public:
	typedef shared_ptr<Logger> ptr;
	Logger(string name = "root");
	void Log(LogLevel::ptr level, LogAppender::ptr appender);

	void AddAppender(LogAppender::ptr appender);//添加
	void DelAppendrt(LogAppender::ptr appender);//删除

	//日志级别
	void debug(LogEvent::ptr level);
	void info(LogEvent::ptr level);
	void warn(LogEvent::ptr level);
	void error(LogEvent::ptr level);
	void fatal(LogEvent::ptr level);
	LogLevel::Level getlevel() { return m_level; }//获取leve
	void setleve(LogLevel::Level level) { m_level = level; }

	LogLevel::Level getlevel();//获取leve
	void setleve(LogLevel::Level level);

private:
	string m_name;             //日志名称
	LogLevel::Level m_level;	  //日志级别（满足这各级别就会输出）
	list<LogAppender::ptr> m_appender;  //日志列表
};

//输出到控制台stdout
class stdoutAppender :public LogAppender  {
public:
	typedef shared_ptr<stdouotAppender> ptr;
	virtual void Log(LogLevel::Level level, LogEvent::ptr event)override/*仅在类内部起作用的类型别名*/;//复写（即override）虚函数。
private:
};
//输出到日志文件中files
class FileAppender :public LogAppender {
public:
	FileAppender(string& filename) :
		m_file_name(filename) {}
	typedef shared_ptr<FileAppender> ptr;
	virtual void Log(LogLevel::Level level, LogEvent::ptr event) override/*仅在类内部起作用的类型别名*/;//复写（即override）虚函数。
	//重新打开文件，打开文件成功返回true
	bool repoen();
private:
	string m_file_name;//日志文件
	ofstream m_filestream;//
};

