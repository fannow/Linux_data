//#include"Log.h"
//
//int main() {
//	//日志部分
//	Logger::ptr logger(new Logger());
//	logger->AddAppender(LogAppender::ptr(new StdOutAppender()));
//	//现在时间
//	//LogEvent::ptr event(new LogEvent(__FILE__, __LINE__, 0, GetThreadId(), GetFiberId()/*协程id*/, time(0)));
//	//设置消息体
//	//event->getSS() << "hello log";
//	//logger->Log(LogLevel::Level::DEBUG, event);
//	LOG_DEBUG(logger) << "hello log";
//	LOG_INFO(logger) << "hello log";
//	LOG_ERROR(logger) << "hello log";
//	LOG_FATAL(logger) << "hello log";
//	LOG_WARN(logger) << "hello log";
//	//FileAppender::ptr file(new FileAppender("./log.txt"));
//
//	////重新设置格式
//	//Formattor::ptr  fmt(new Formattor("%d%T%m%n"));
//	//logger->setformat(fmt);
//	//logger->AddAppender(file);
//	////文件启动
//	////logger->AddAppender(LogAppender::ptr(new FileAppender()));
//	//FMT_LOG_ERROR(logger, "nihao %s","aa");
//	//auto it = LogRoot::GetInstance()->getlogger("xx");
//
//
//
//}
#include"Config.h"
#include"Log.h"

ConfigVar<int>::ptr config_in = Config::LookUp("sysou.port", (int)8080, "system port");
int main() {
	LOG_INFO(LOG_ROOT())  <<config_in->getval();
	LOG_INFO(LOG_ROOT()) << config_in->Tostring();


	return 0;


}