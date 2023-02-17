使用log4cplus打印日志

log4cplus代码下载编译

https://github.com/log4cplus/log4cplus/releases

选择v2.0.8,  当时v2.1.0编译时出现错误，exception类未实现。

<img src="E:\CodeSamples\Cpp_Examples\doc\log4cplus编译.png" style="zoom:50%;" />

选择Debug_Unicode和Release_Unicode分别编译, log4cplus为动态dll, log4cplusS为静态lib, loggingserver为日志打印服务



使用log4cplus配置日志打印

log4cplus.properties文件配置如下：

```properties
#不能有空行, 至少# 开头.
log4cplus.rootLogger=DEBUG, ALL_MSGS_TO_CONSOLE, ALL_MSGS_TO_FILE
#
log4cplus.appender.ALL_MSGS_TO_CONSOLE=log4cplus::ConsoleAppender
log4cplus.appender.ALL_MSGS_TO_CONSOLE.layout=log4cplus::PatternLayout
log4cplus.appender.ALL_MSGS_TO_CONSOLE.layout.ConversionPattern=%d{%m/%d/%y %H:%M:%S,%Q} [%-5p] %m %n 
#%d{%m/%d/%y %H:%M:%S,%Q} [%-5p] [%M:%L]  %m %n   [%M:%L]用于打印函数和行号
# 
log4cplus.appender.ALL_MSGS_TO_FILE=log4cplus::RollingFileAppender
log4cplus.appender.ALL_MSGS_TO_FILE.MaxFileSize=1MB
log4cplus.appender.ALL_MSGS_TO_FILE.MaxBackupIndex=100
log4cplus.appender.ALL_MSGS_TO_FILE.layout=log4cplus::PatternLayout
log4cplus.appender.ALL_MSGS_TO_FILE.layout.ConversionPattern=%d{%m/%d/%y %H:%M:%S,%Q} [%-5p] %m %n
log4cplus.appender.ALL_MSGS_TO_FILE.File=./main.log
```

rootLogger日志打印级别为Debug, appender分别为ALL_MSGS_TO_CONSOLE, ALL_MSGS_TO_FILE, 其中ALL_MSGS_TO_FILE单个日志文件大小1MB(log4plus要求日志最小200K，小于200K的设置不生效)， 最大100个日志文件.

PatternLayout参考: https://support.dcmtk.org/docs-360/classlog4cplus_1_1PatternLayout.html



使用参考代码如下:

// LogSample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

```c++
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
	for (int32_t i = 0; i < 1000000; ++i)
	{
		TestA a;
		LOG_DEBUG("Log4::Initialze is Ok");
		LOG_TRACE("Test for test")
	}

}
```

![](E:\CodeSamples\Cpp_Examples\doc\日志文件.PNG)



日志打印与设置格式一致，修改循环次数, 日志可以循环覆盖



参考资料：https://sourceforge.net/p/log4cplus/wiki/CodeExamples/

代码路径:https://github.com/hero-joey/Cpp_Examples/tree/main/PracticeSample/LogSample