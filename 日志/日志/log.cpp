#include"log.h"
/**********************************日志器Logger 成员函数实现*****************************************************/
Logger::Logger(string name = "root"):
			m_name(name) {};
void Logger::Log(LogLevel::Level level, LogEvent::ptr event) {
	if (level >= m_level) {
		for (auto& i : m_appender) {
			i->Log(level, event);
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
	Log(LogLevel::FATAl, event);
}


void Logger::AddAppender(LogAppender::ptr appender)//添加
{
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
void FileAppender::Log(LogLevel::Level level, LogEvent::ptr event) {
if (level >= m_level) {
			//将对应的日志时间设置为输出日志格式
			cout << m_forattor->format(event);
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
void stdoutAppender::Log(LogLevel::Level level, LogEvent::ptr event){
	if (level >= m_level) {
		//将对应的日志时间设置为输出日志格式
		cout << m_forattor->format(event);
	}
}