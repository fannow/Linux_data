#pragma once
#include"Log.h"



/******************��־��ʽ���Ƶ�Ԫ***********************/
class Messageitemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	Messageitemforma(string str = "") {}

	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getss();
	}

};
class Levelitemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	Levelitemforma(string str = "") {}

	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
		os << LogLevel::Tostring(level);
	}

};
//��������������ʱ��
class Pleaseitemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	Pleaseitemforma(string str = "") {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getplease();
	}

};
class lognameitemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	lognameitemforma(string str = "") {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->GetLogger()->getname();
	}
};
//�߳�id
class ThreadIditemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	ThreadIditemforma(string str = "") {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
		os << event->getthreadid();
	}
};
//Э��id
class fiberiditemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	fiberiditemforma(string str = "") {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getfiberid();
	}
};
//����ʱ��
class Timeitemforma :public Formater::Item {
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


	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		//��־ʱ���ʽ����
		struct tm* tm;
		time_t t = event->gettime();
		tm = localtime(&t);
		char buff[64];
		strftime(buff, sizeof(buff), time_formate.c_str(), tm);
		os << buff;
	}

};


//�ļ�����
class filenameitemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	filenameitemforma(string str = "") {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getfile();
	}
};
//�ļ��к�
class lineitemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	lineitemforma(string str = "") {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getline();
	}
};


class stringitemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	stringitemforma(string format) :
		str(format) {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << str;
	}
private:

	string str;
};
class tabitemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	tabitemforma(string format) {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << '\t';
	}
private:
};

//����
class newlineitemforma :public Formater::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	newlineitemforma(string format) :
		str(format) {}
	void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << endl;
	}
private:
	//ʱ���ʽ
	string str;
};

/*****************��־����***********************/
const char* LogLevel::Tostring(LogLevel::Level level) {
	switch (level) {
		/*case UNKNOW:
			return "UNKNOW";
			break;
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
/*****************��־�¼�***********************/

LogEvent::LogEvent(shared_ptr<Logger> _ptr, LogLevel::Level level, const char* file,
	int32_t line, uint32_t please, uint32_t threadid, uint32_t fiberid, uint32_t time)
	:logger(_ptr), level(level), m_file(file), m_line(line), m_please(please),
	m_threadid(threadid), m_fiberId(fiberid), m_time(time) {
}

void LogEvent::format(const char* fmt, ...) {
	va_list al;
	va_start(al, fmt);
	format(fmt, al);
	va_end(al);

}
void LogEvent::format(const char* fmt, va_list al) {
	char* buff = NULL;
	int len = vasprintf(&buff, fmt, al);

	if (len != -1) {
		m_ss << string(buff, len);
	}
	delete buff;
}





/*****************��־��ʽ***********************/

Formater::Formater(string pattern) :
	m_pattern(pattern) {
	Formater::Init();
}

string Formater::format(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
	stringstream ss;

	for (auto& i : m_items) {
		//ʹ���ڲ��������������־����
		i->format(ss, logger, level, event);
	}
	return ss.str();

}
/*
*�ڲ�ѭ����%��һ���ַ���ʼ������ͬʱ���Ϊ״̬0
*�����״̬0�����������{���Ϊ״̬1����ȡ%��{֮����Ӵ������״̬1�����������}���Ϊ״̬2����ȡ{��}֮����Ӵ�
*��Ŀǰ������ڲ�ѭ������������������ʱ��������ѭ������������push��vecԪ������Ҳ��ƥ��
*/
void Formater::Init() {

	//tuple(str,fmt,type)
	vector<tuple<string, string, int>>vec;

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
		while (n < m_pattern.size()) {
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
					//	cout << "#" << fmt << endl;
					fmt_staus = 0;
					++n;
					break;
				}

			}
			++n;
			if (n == m_pattern.size()) {
				if (str.empty()) {
					str = m_pattern.substr(i - 1);
				}
			}
		}

		if (fmt_staus == 0) {
			if (!nstr.empty()) {
				vec.push_back(make_tuple(nstr, string(), 0));
				nstr.clear();
			}
			str = m_pattern.substr(i + 1, n - i - 1);

			//������־����־�洢��������

			vec.push_back(make_tuple(str, fmt, 1));
			i = n - 1;
		}
		else if (fmt_staus == 1) {
      m_error=true;
			cout << "pattern parse error" << m_pattern << "-" << m_pattern.substr(i) << endl;
			//��ʽ����
			vec.push_back(make_tuple("<<pattern_error>>", fmt, 0));
		}
	}
	if (!nstr.empty()) {
		vec.push_back(make_tuple(nstr, "", 0));

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
	static map<string, function<Formater::Item::ptr(string str)>> formar_item = {

#define xx(str,c) \
		{ #str,[](string fmt) {return Formater::Item::ptr(new c(fmt));}}

		xx(m, Messageitemforma),
		xx(p, Levelitemforma),
		xx(r, Pleaseitemforma),
		xx(c, lognameitemforma),
		xx(t, ThreadIditemforma),
		xx(n, newlineitemforma),
		xx(d, Timeitemforma),
		xx(f, filenameitemforma),
		xx(l, lineitemforma),
		xx(T, tabitemforma),
		xx(F, fiberiditemforma),

#undef xx

	};
	for (auto& i : vec) {
		if (get<2>(i) == 0) {
			m_items.push_back(Formater::Item::ptr(new stringitemforma(get<0>(i))));
		}
		else {
			auto it = formar_item.find(get<0>(i));
			if (it == formar_item.end()) {
        m_error=true;
				m_items.push_back(Formater::Item::ptr(new stringitemforma("<<error_format %" + get<0>(i) + ">>")));
			}
			else {
				m_items.push_back(it->second(get<1>(i)));

			}

		}

	}

}


/********************��־��********************/
void Logger::AddAppender(LogAppender::ptr appender)//���
{
	if (!appender->GetFormat()) {
		//������־��ʽ
		appender->SetFormat(format);
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

Logger::Logger(string _name) :
	m_name(_name), m_level(LogLevel::DEBUG) {
	//��ʽ����
	format.reset(new Formater("%d%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m %n"));
}
void Logger::Log(LogLevel::Level level, LogEvent::ptr event) {
	if (level >= m_level) {
		//���Լ���Ϊ����ָ�봫�뵽������ȥ
		auto self = shared_from_this();
    if(!m_appender.empty()){
	  	for (auto& i : m_appender) {
	  		i->Log(self, level, event);
	  	}
    }else if(root){
     root->Log(level,event);
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

/**********************************��־�����LogAppender ��Ա����ʵ��****************************************************/
//����fileAppender�ຯ��ʵ��
/*�������ڲ������õ����ͱ���*///��д����override���麯����
void FileAppender::Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) {
	if (level >= m_level) {
		//����Ӧ��־������ļ���
		m_filestream << m_format->format(logger, level, event);
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
	m_filestream.open(m_file_name.c_str());
	//˫�ط񶨽�int����ת��Ϊbool����
	//�õ�ԭ����ֵ��bool����
	return !!m_filestream;

}
//����stdoutAppenderʵ���ຯ��
void StdOutAppender::Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) {
	if (level >= m_level) {

		//����Ӧ����־ʱ������Ϊ�����־��ʽ
		cout << m_format->format(logger, level, event);
	}
}
