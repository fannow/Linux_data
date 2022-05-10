#include"log.h"
/**********************************��־��Logger ��Ա����ʵ��*****************************************************/
Logger::Logger(string _name) :
			m_name(_name), m_level(LogLevel::DEBUG) {
	//��ʽ����
	format.reset(new Formattor("%d [%p] <%f:%l> %m %n "));
}
void Logger::Log(LogLevel::Level level, LogEvent::ptr event) {
	if (level >= m_level) {
		//���Լ���Ϊ����ָ�봫�뵽������ȥ
		auto self = shared_from_this();
		for (auto& i : m_appender) {
			i->Log(self,level, event);
		}
	}
}


//��־����
void  Logger::debug(LogEvent::ptr event) {

	Log(LogLevel::DEBUG, event);
}
void  Logger::info(LogEvent::ptr event) {
	Log(LogLevel::INFO, event);
}
void  Logger::warn(LogEvent::ptr event) {
	Log(LogLevel::WARN, event);
}
void  Logger::error(LogEvent::ptr event) {
	Log(LogLevel::ERROR, event);
}

void  Logger::fatal(LogEvent::ptr event) {
	Log(LogLevel::FATAL, event);
}


void Logger::AddAppender(LogAppender::ptr appender)//���
{
	if (!appender->getformattor()) {
		//������־��ʽ
		appender->setformattor(format);
	}
	m_appender.push_back(appender);
}
void Logger::DelAppendrt(LogAppender::ptr appender)//ɾ��
{
	//��������Ѱ��appender
	for (auto it = m_appender.begin();it != m_appender.end();it++) {
		if (*it == appender) {
			//ֱ��ɾ���Ϳ���
			m_appender.erase(it);
			break;
		}
	}
}
/**********************************��־�����LogAppender ��Ա����ʵ��****************************************************/
//����fileAppender�ຯ��ʵ��
/*�������ڲ������õ����ͱ���*///��д����override���麯����
void FileAppender::Log(Logger::ptr logger,LogLevel::Level level, LogEvent::ptr event) {
		if (level >= m_level) {
			//����Ӧ��־������ļ���
			m_filestream << m_forattor->format(logger,level,event);
			/*ofstream�Ǵ��ڴ浽Ӳ��*/
		}
}
//��־�ļ����´�
bool  FileAppender::repoen() {
	if (m_filestream) {
		//�ȹر��ļ�
		m_filestream.close();
	}
	//�ڴ��ļ�
	m_filestream.open(m_file_name);
	//˫�ط񶨽�int����ת��Ϊbool����
	//�õ�ԭ����ֵ��bool����
		return !!m_filestream;

}
//����stdoutAppenderʵ���ຯ��
void stdoutAppender::Log(Logger::ptr logger,LogLevel::Level level, LogEvent::ptr event){
	if (level >= m_level) {
	
		//����Ӧ����־ʱ������Ϊ�����־��ʽ
		cout << m_forattor->format(logger,level,event);
	}
}

/**********************************��ʽ���� ��־��ʽ ��Ա����ʵ��****************************************************/

/**********************************��ʽ����ʵ��***************************************************/
//item����������־��ʽ
class Messageitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	Messageitemforma(string str="") {}

	void format(ostream&os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getss();
	}

};
class Levelitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	Levelitemforma(string str="") {}

	void format(ostream&os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
		os << LogLevel::Tostring(level);
	}

};
//��������������ʱ��
class Pleaseitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	Pleaseitemforma(string str="") {}
	void format(ostream&os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getplease() ;
	}

};
class lognameitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	lognameitemforma(string str="") {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << logger->getname();
	}
};
//�߳�id
class ThreadIditemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	ThreadIditemforma(string str="") {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
		os << event->getthreadid();
	}
};
//Э��id
class fiberiditemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	fiberiditemforma(string str="") {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getfiberid() ;
	}
};
//����ʱ��
class Timeitemforma :public Formattor::Item {
private:
	//ʱ���ʽ
	string time_formate;
public:
	typedef shared_ptr<Messageitemforma> ptr;
	Timeitemforma(string format = "%Y:%m:%d %H:%M:%S") :
		time_formate(format) {
		if (time_formate.empty()) {
			time_formate = "%Y:%m:%d %H:%M:%S";
		}
	}


	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		//��־ʱ���ʽ����
	//s/*truct tm tm;
	//time_t t = event->gettime();
	//localtime(&t,&tm);
	//char buff[64];*/
	//strftime(buff,sizeof(buff), time_formate.c_str(), &tm);
		os << event->gettime();
	}

};


//�ļ�����
class filenameitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	filenameitemforma(string str="") {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getfile() ;
	}
};
//�ļ��к�
class lineitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	lineitemforma(string str="") {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getline();
	}
};


class stringitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	stringitemforma(string format) :
		str(format) {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << str;
	}
private:
	//ʱ���ʽ
	string str;
};


//����
class newlineitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	newlineitemforma(string format) :
		str(format) {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os <<  endl;
	}
private:
	//ʱ���ʽ
	string str;
};
//const char* m_file = NULL;      //�ļ���
//int32_t m_line = 0;				  //�к�
//uint32_t m_please = 0;             //��������ʱ�䣨���룩
//uint32_t m_threadid = 0;           //�߳�id
//uint32_t m_fiberId = 0;            //Э��id
//uint64_t m_time = 0;               //ʱ���

//��־��ʽ����
//�ַ��������㷨
//������־��ʽ


/*
*�ڲ�ѭ����%��һ���ַ���ʼ������ͬʱ���Ϊ״̬0
*�����״̬0�����������{���Ϊ״̬1����ȡ%��{֮����Ӵ������״̬1�����������}���Ϊ״̬2����ȡ{��}֮����Ӵ�
*��Ŀǰ������ڲ�ѭ������������������ʱ��������ѭ������������push��vecԪ������Ҳ��ƥ��
*/
void Formattor::Init() {

	//tuple(str,fmt,type)
	vector<tuple<string,string, int>>vec;

	string nstr;
	for (size_t i = 0;i < m_pattern.size();i++) {
		if (m_pattern[i] != '%') {
			nstr.append(1, m_pattern[i]);
			continue;
		}
			//��ʵ����һ��%
		if ((i + 1) < m_pattern.size()) {
			if (m_pattern[i + 1] == '%') {
				nstr.append(1, '%');
				continue;
			}
		}
		size_t n = i + 1;
		int fmt_staus = 0;
		int fmt_beign = 0;


		string str;
		string fmt;
		while (n<m_pattern.size()) {
			//�����ո�
			if (!isalpha(m_pattern[n]) && m_pattern[n] != '}' && m_pattern[n] != '{') {
				str = m_pattern.substr(i + 1, n - i - 1);
				break;
			}
			if (fmt_staus == 0) {
				if (m_pattern[n] == '{') {
					//�ַ����ָ�
					str = m_pattern.substr(i + 1, n - i - 1);
					fmt_staus = 1;
					fmt_beign = n;
					n++;
					continue;
				}
							
			}
			else if (fmt_staus == 1) {
				if (m_pattern[n] == '}') {
					//�ַ����ָ�
					fmt = m_pattern.substr(fmt_beign + 1, n - fmt_beign - 1);
					fmt_staus = 2;
				
					break;
				}
				
			}
			++n;
		}
	
		if (fmt_staus == 0) {
			if (!nstr.empty()) {
				vec.push_back(make_tuple(nstr, string(), 0));
				nstr .clear();
			}
			str = m_pattern.substr(i + 1, n - i - 1);
		
			//������־����־�洢��������
			
			vec.push_back(make_tuple(str, fmt, 1));
			i = n-1;
		}
		else if (fmt_staus == 1) {
			cout << "pattern parse error" << m_pattern <<"-" << m_pattern.substr(i) << endl;
			//��ʽ����
			vec.push_back(make_tuple("<<pattern_error>>", fmt, 0));
		}
		else if (fmt_staus == 2) {
		
			if (!nstr.empty()) {
				vec.push_back(make_tuple(nstr, string(), 0));
				nstr.clear();
			}

			vec.push_back(make_tuple(str, fmt, 1));
			i = n - 1;
		}
		
		
	}
	if (!nstr.empty()) {
		vec.push_back(make_tuple(nstr,string(), 0));
	
	}

	/*
	* %m--->��Ϣ��
	* %p--->��־����
	* %r--->������ʱ��
	* %c--->��־����
	* %t--->�߳�id
	* %n--->�س�
	* %d--->ʱ��
	* %f--->�ļ���
	* %l--->�к�
	*/

	//function<Item::ptr(string str)> ����ֵΪiter::ptr ����Ϊstring�ú���
	static map<string, function<Item::ptr(string str)>> formar_item = {
		//{"m",[](string fmt) {return Formattor::Item::ptr(new Messageitemforma(fmt));}}
#define xx(str,c) \
		{#str,[](string fmt) {return Formattor::Item::ptr(new c(fmt));}}
		xx(m,Messageitemforma),
		xx(p, Levelitemforma),
		xx(r, Pleaseitemforma),
		xx(c, lognameitemforma),
		xx(t, ThreadIditemforma),
		xx(n, newlineitemforma),
		xx(d, Timeitemforma),
		xx(f, filenameitemforma),
		xx(l, lineitemforma),
#undef xx
		
	};
	for(auto& i : vec) {
		if (get<2>(i) == 0) {
			m_items.push_back(Formattor::Item::ptr(new stringitemforma(get<0>(i))));
		}
		else {
			auto it = formar_item.find(get<0>(i));
			if (it == formar_item.end()) {
				m_items.push_back(Formattor::Item::ptr(new stringitemforma("<<error_format %" + get<0>(i)+">>")));
			}
			else {
				m_items.push_back(it->second(get<1>(i)));

			}
			
		}
	cout << "(" << get<0>(i) << ")-(" << get<1>(i) << ")-(" << get<2>(i)<<")" << endl;

		
	
	
	}

}

//��־����
string Formattor::format(shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
	stringstream ss;
	
	for (auto& i : m_items) {
		//ʹ���ڲ��������������־����
		i->format(ss,logger,level, event);
	}
	return ss.str();
}


int main() {
	Logger::ptr logger(new Logger());
	logger->AddAppender(LogAppender::ptr(new stdoutAppender()));


	LogEvent::ptr event(new LogEvent(__FILE__, __LINE__, 0, 1, 2, time(0)));
//	event->getSS() << "hello log";

	logger->Log(LogLevel::Level::DEBUG, event);
}