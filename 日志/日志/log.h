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
//��־�¼�
class LogEvent {
public:
	//Ĭ�Ϲ��캯��
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
	const char* m_file = NULL;      //�ļ���
	int32_t m_line = 0;				  //�к�
	uint32_t m_please = 0;             //��������ʱ�䣨���룩
	uint32_t m_threadid = 0;           //�߳�id
	uint32_t m_fiberId = 0;            //Э��id
	uint64_t m_time = 0;               //ʱ���
	stringstream m_ss;				  //		
	
};
//��־����
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
	string format(shared_ptr<Logger> logger,LogLevel::Level leve,LogEvent::ptr event);
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
		virtual void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) = 0;
	
		
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
	virtual void Log(shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event)=0;


	//������־��ʽ
	void setformattor(Formattor::ptr val) { m_forattor = val; }
	//������־��ʽ
	Formattor::ptr getformattor() { return m_forattor; }

protected:

	LogLevel::Level m_level;//��־����
	Formattor::ptr m_forattor;//�����־��ʽ//�����������־

};
//��־��
class Logger:public enable_shared_from_this<Logger> {
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

	string getname()const { return m_name; }

private:
	string m_name;             //��־����
	LogLevel::Level m_level;	  //��־���������������ͻ������
	list<LogAppender::ptr> m_appender;  //��־�б�

	Formattor::ptr format;
};

//���������̨stdout
class stdoutAppender :public LogAppender  {
public:
	typedef shared_ptr<stdoutAppender> ptr;
	virtual void Log(Logger::ptr logger,LogLevel::Level level, LogEvent::ptr event)override/*�������ڲ������õ����ͱ���*/;//��д����override���麯����
private:
};
//�������־�ļ���files
class FileAppender :public LogAppender {
public:
	FileAppender(string& filename) :
		m_file_name(filename) {}
	typedef shared_ptr<FileAppender> ptr;
	 void Log(Logger::ptr logger,LogLevel::Level level, LogEvent::ptr event) override/*�������ڲ������õ����ͱ���*/;//��д����override���麯����
	//���´��ļ������ļ��ɹ�����true
	bool repoen();
private:
	string m_file_name;//��־�ļ�
	ofstream m_filestream;//�ļ���
};

	