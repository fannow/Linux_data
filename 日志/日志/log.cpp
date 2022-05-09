#include"log.h"
/**********************************��־��Logger ��Ա����ʵ��*****************************************************/
Logger::Logger(string name = "root"):
			m_name(name) {};
void Logger::Log(LogLevel::Level level, LogEvent::ptr event) {
	if (level >= m_level) {
		for (auto& i : m_appender) {
			i->Log(level, event);
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
	Log(LogLevel::FATAl, event);
}


void Logger::AddAppender(LogAppender::ptr appender)//���
{
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
void FileAppender::Log(LogLevel::Level level, LogEvent::ptr event) {
if (level >= m_level) {
			//����Ӧ����־ʱ������Ϊ�����־��ʽ
			cout << m_forattor->format(event);
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
void stdoutAppender::Log(LogLevel::Level level, LogEvent::ptr event){
	if (level >= m_level) {
		//����Ӧ����־ʱ������Ϊ�����־��ʽ
		cout << m_forattor->format(event);
	}
}