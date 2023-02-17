// LogSample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/fileinfo.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/initializer.h>
#include <log4cplus/tstring.h>
#include <Windows.h>

#define LOG_TRACE(...) { \
    LOG4CPLUS_INFO( log4cplus::Logger::getRoot( ), "["<<  __func__ << "]" <<__VA_ARGS__ ); \
}

#define LOG_DEBUG(...) { \
    LOG4CPLUS_DEBUG( log4cplus::Logger::getRoot( ), __VA_ARGS__ ); \
}

#define LOG_INFO(...)  { \
    LOG4CPLUS_INFO( log4cplus::Logger::getRoot( ), __VA_ARGS__ );  \

std::string GetCurrentDir()
{
	char buf[256] = { 0 };

	int length = GetModuleFileNameA(NULL, buf, sizeof(buf) - 1);
	if (length > 0)
	{
		for (int i = length - 1; i > 0; i--)
		{
			if (buf[i] == '\\')
			{
				buf[i] = '\0';
				break;
			}
		}
	}

	return buf;
}

class TestA
{
public:
	TestA()
	{
		LOG_DEBUG("TestA() called");
	}
};

int main()
{
	std::string currentDir = GetCurrentDir();
	std::string configFilePath = currentDir + "\\log4cplus.properties";
	//日志系统初始化
	log4cplus::initialize();

	//日志配置初始化
	//log4cplus::tstring configFile = LOG4CPLUS_TEXT(configFilePath);
	log4cplus::tstring configFile = LOG4CPLUS_STRING_TO_TSTRING(configFilePath);
	log4cplus::PropertyConfigurator configurator(configFile);
	configurator.configure();

	//日志打印
	for (int32_t i = 0; i < 100; ++i)
	{
		TestA a;
		LOG_DEBUG("Log4::Initialze is Ok");
		LOG_TRACE("Test for test")
	}
}

