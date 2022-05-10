#include"log.h"
/**********************************日志器Logger 成员函数实现*****************************************************/
Logger::Logger(string _name) :
			m_name(_name), m_level(LogLevel::DEBUG) {
	//格式控制
	format.reset(new Formattor("%d [%p] <%f:%l> %m %n "));
}
void Logger::Log(LogLevel::Level level, LogEvent::ptr event) {
	if (level >= m_level) {
		//将自己作为智能指针传入到函数中去
		auto self = shared_from_this();
		for (auto& i : m_appender) {
			i->Log(self,level, event);
		}
	}
}


//日志级别
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


void Logger::AddAppender(LogAppender::ptr appender)//添加
{
	if (!appender->getformattor()) {
		//设置日志格式
		appender->setformattor(format);
	}
	m_appender.push_back(appender);
}
void Logger::DelAppendrt(LogAppender::ptr appender)//删除
{
	//遍历链表寻找appender
	for (auto it = m_appender.begin();it != m_appender.end();it++) {
		if (*it == appender) {
			//直接删除就可以
			m_appender.erase(it);
			break;
		}
	}
}
/**********************************日志输出器LogAppender 成员函数实现****************************************************/
//子类fileAppender类函数实现
/*仅在类内部起作用的类型别名*///复写（即override）虚函数。
void FileAppender::Log(Logger::ptr logger,LogLevel::Level level, LogEvent::ptr event) {
		if (level >= m_level) {
			//将对应日志输出到文件中
			m_filestream << m_forattor->format(logger,level,event);
			/*ofstream是从内存到硬盘*/
		}
}
//日志文件重新打开
bool  FileAppender::repoen() {
	if (m_filestream) {
		//先关闭文件
		m_filestream.close();
	}
	//在打开文件
	m_filestream.open(m_file_name);
	//双重否定将int类型转换为bool类型
	//得到原先数值得bool类型
		return !!m_filestream;

}
//子类stdoutAppender实现类函数
void stdoutAppender::Log(Logger::ptr logger,LogLevel::Level level, LogEvent::ptr event){
	if (level >= m_level) {
	
		//将对应的日志时间设置为输出日志格式
		cout << m_forattor->format(logger,level,event);
	}
}

/**********************************格式编码 日志格式 成员函数实现****************************************************/

/**********************************格式子类实现***************************************************/
//item子类设置日志格式
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
//程序启动后消耗时间
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
//线程id
class ThreadIditemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	ThreadIditemforma(string str="") {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
		os << event->getthreadid();
	}
};
//协程id
class fiberiditemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	fiberiditemforma(string str="") {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getfiberid() ;
	}
};
//运行时间
class Timeitemforma :public Formattor::Item {
private:
	//时间格式
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
		//日志时间格式设置
	//s/*truct tm tm;
	//time_t t = event->gettime();
	//localtime(&t,&tm);
	//char buff[64];*/
	//strftime(buff,sizeof(buff), time_formate.c_str(), &tm);
		os << event->gettime();
	}

};


//文件名称
class filenameitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	filenameitemforma(string str="") {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os << event->getfile() ;
	}
};
//文件行号
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
	//时间格式
	string str;
};


//换行
class newlineitemforma :public Formattor::Item {
public:
	typedef shared_ptr<Messageitemforma> ptr;
	newlineitemforma(string format) :
		str(format) {}
	void format(ostream &os, shared_ptr<Logger> logger, LogLevel::Level leve, LogEvent::ptr event) override {
		os <<  endl;
	}
private:
	//时间格式
	string str;
};
//const char* m_file = NULL;      //文件名
//int32_t m_line = 0;				  //行号
//uint32_t m_please = 0;             //程序运行时间（毫秒）
//uint32_t m_threadid = 0;           //线程id
//uint32_t m_fiberId = 0;            //协程id
//uint64_t m_time = 0;               //时间戳

//日志格式解析
//字符串解析算法
//控制日志格式


/*
*内层循环从%下一个字符开始遍历。同时标记为状态0
*如果在状态0的情况下遇到{标记为状态1，获取%到{之间的子串如果在状态1的情况下遇到}标记为状态2。获取{到}之间的子串
*从目前看如果内层循环所有条件都不满足时会陷入死循环，而且下面push的vec元素类型也不匹配
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
			//其实还是一个%
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
			//遇见空格
			if (!isalpha(m_pattern[n]) && m_pattern[n] != '}' && m_pattern[n] != '{') {
				str = m_pattern.substr(i + 1, n - i - 1);
				break;
			}
			if (fmt_staus == 0) {
				if (m_pattern[n] == '{') {
					//字符串分割
					str = m_pattern.substr(i + 1, n - i - 1);
					fmt_staus = 1;
					fmt_beign = n;
					n++;
					continue;
				}
							
			}
			else if (fmt_staus == 1) {
				if (m_pattern[n] == '}') {
					//字符串分割
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
		
			//解析日志将日志存储在数组中
			
			vec.push_back(make_tuple(str, fmt, 1));
			i = n-1;
		}
		else if (fmt_staus == 1) {
			cout << "pattern parse error" << m_pattern <<"-" << m_pattern.substr(i) << endl;
			//格式错误
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
	* %m--->消息体
	* %p--->日志级别
	* %r--->启动的时间
	* %c--->日志名称
	* %t--->线程id
	* %n--->回车
	* %d--->时间
	* %f--->文件名
	* %l--->行号
	*/

	//function<Item::ptr(string str)> 返回值为iter::ptr 参数为string得函数
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

//日志解析
string Formattor::format(shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
	stringstream ss;
	
	for (auto& i : m_items) {
		//使用内部类派生类进行日志解析
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