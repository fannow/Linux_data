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
//��־�¼�
class LogEvent {
public:
	//Ĭ�Ϲ��캯��
	LogEvent() {};
	typedef shared_ptr<LogEvent> ptr;
private:
	const char* NameFile = NULL;      //�ļ���
	int32_t MLine = 0;				  //�к�
	uint32_t MElpase = 0;             //��������ʱ�䣨���룩
	uint32_t MThreadId = 0;           //�߳�id
	uint32_t MFiberId = 0;            //Э��id
	uint64_t MTime = 0;               //ʱ���
	string MContent;				  //		
	
};
//��־����
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
//��־��ʽ����
class Formattor {
public:
	typedef shared_ptr<Formattor> ptr;
	Formattor(string& pattern) :
		m_pattern(pattern) {}
	//ͨ��pattern������item�е���Ϣ

	//��־����
	string format(LogEvent::ptr event);
private:
	//��־������ģ��
	class Item {
	public:
		typedef shared_ptr<Item> ptr;
		virtual ~Item() {}
		//��־��������

		/*
		* ostream��Ϊ���
		*/
		virtual void format(ostream os,LogEvent::ptr event) = 0;
		
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
	virtual void Log(LogLevel::Level level, LogEvent::ptr event)=0;


	//������־��ʽ
	void setformattor(Formattor::ptr val) { m_forattor = val; }
	//������־��ʽ
	Formattor::ptr getformattor() { return m_forattor; }

protected:

	LogLevel::ptr m_level;//��־����
	Formattor::ptr m_forattor;//�����־��ʽ//�����������־

};
//��־��
class Logger {
public:
	typedef shared_ptr<Logger> ptr;
	Logger(string name = "root");
	void Log(LogLevel::ptr level, LogAppender::ptr appender);

	void AddAppender(LogAppender::ptr appender);//���
	void DelAppendrt(LogAppender::ptr appender);//ɾ��

	//��־����
	void debug(LogEvent::ptr level);
	void info(LogEvent::ptr level);
	void warn(LogEvent::ptr level);
	void error(LogEvent::ptr level);
	void fatal(LogEvent::ptr level);
	LogLevel::Level getlevel() { return m_level; }//��ȡleve
	void setleve(LogLevel::Level level) { m_level = level; }

	LogLevel::Level getlevel();//��ȡleve
	void setleve(LogLevel::Level level);

private:
	string m_name;             //��־����
	LogLevel::Level m_level;	  //��־���������������ͻ������
	list<LogAppender::ptr> m_appender;  //��־�б�
};

//���������̨stdout
class stdoutAppender :public LogAppender  {
public:
	typedef shared_ptr<stdouotAppender> ptr;
	virtual void Log(LogLevel::Level level, LogEvent::ptr event)override/*�������ڲ������õ����ͱ���*/;//��д����override���麯����
private:
};
//�������־�ļ���files
class FileAppender :public LogAppender {
public:
	FileAppender(string& filename) :
		m_file_name(filename) {}
	typedef shared_ptr<FileAppender> ptr;
	virtual void Log(LogLevel::Level level, LogEvent::ptr event) override/*�������ڲ������õ����ͱ���*/;//��д����override���麯����
	//���´��ļ������ļ��ɹ�����true
	bool repoen();
private:
	string m_file_name;//��־�ļ�
	ofstream m_filestream;//
};

