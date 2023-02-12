// JsonSample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <fstream>
#include "Windows.h"
#include "json/json.h"
#include "errno.h"
using namespace std;

class JsonSample
{
private:
	string  m_configFilename;
	Json::Value m_jsonConfigRoot;


public:
	explicit JsonSample(const string& configFilename)
		: m_configFilename(configFilename)
	{
		//std::cout << "Current path is " << m_configFilename << '\n'; 
	}

	int32_t InitConfig();
	void ReadAdf4001();
	void ReadHwVersion();
	
};

int32_t JsonSample::InitConfig()
{
	std::ifstream fileStream;
	fileStream.open(m_configFilename);
	if (!fileStream.is_open())
	{
		cout << "Config file open failed" << endl;
		return RETURN_ERROR;
	}

	Json::Reader JsonReader;
	if (!JsonReader.parse(fileStream, m_jsonConfigRoot))
	{
		cout << "Config file parse failed" << endl;
		return RETURN_ERROR;
	}

	return RETURN_OK;
}

void JsonSample::ReadAdf4001()
{
	int32_t ret = RETURN_OK;
	const uint32_t ADF_DEVICE_COUNT = 2;
	Json::Value adfJsonConf[ADF_DEVICE_COUNT];

	adfJsonConf[0] = m_jsonConfigRoot["ADF4002"]["DevId1"];
	adfJsonConf[1] = m_jsonConfigRoot["ADF4002"]["DevId2"];

	for (uint32_t i = 0; i < ADF_DEVICE_COUNT; ++i)
	{
		if (adfJsonConf[i].isArray())
		{
			for (uint32_t j = 0; j < adfJsonConf[i].size(); ++j)
			{
				int32_t addr = adfJsonConf[i][j]["addr"].asInt();
				string  data = adfJsonConf[i][j]["data"].asString();
				cout << "addr:" << addr << ",data:" << data << endl;
			}
		}
	}
	
	cout << endl;
}

void JsonSample::ReadHwVersion()
{
	Json::Value hardwareVersionJsonConf = m_jsonConfigRoot["HwVersion"];
	
	float version = hardwareVersionJsonConf["Version"].asFloat();
	string author = hardwareVersionJsonConf["Author"].asString();
	string date = hardwareVersionJsonConf["Date"].asString();
	bool official = hardwareVersionJsonConf["Offcial"].asBool();
	
	cout << "version:" << version << endl;
	cout << "author:" << author << endl;
	cout << "date:" << date << endl;
	cout << std::boolalpha  << "official:" << official << endl;

	cout << endl;
}

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

int main()
{
	string currentDir = GetCurrentDir();
	JsonSample jsonSample(currentDir + "\\hwinit.conf");
	
	jsonSample.InitConfig();
	jsonSample.ReadHwVersion();
	jsonSample.ReadAdf4001();
	
	return 0;
}


