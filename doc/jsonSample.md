C++工程实践使用JsonCpp读取Json配置

#### 1 安装JsonCpp

vcpkg安装请参考链接 https://blog.csdn.net/qq_41023270/article/details/109274433

E:\CodeSamples\vcpkg>.\vcpkg install jsoncpp:x64-windows
Installing 3/3 jsoncpp:x64-windows...
Building jsoncpp[core]:x64-windows...
-- Downloading https://github.com/open-source-parsers/jsoncpp/archive/1.9.5.tar.gz -> open-source-parsers-jsoncpp-1.9.5.tar.gz...
-- Extracting source E:/CodeSamples/vcpkg/downloads/open-source-parsers-jsoncpp-1.9.5.tar.gz
-- Using source at E:/CodeSamples/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean
-- Found external ninja('1.10.2').
-- Configuring x64-windows
-- Building x64-windows-dbg
-- Building x64-windows-rel
CMake Warning at scripts/cmake/vcpkg_copy_pdbs.cmake:44 (message):
  Could not find a matching pdb file for:

  *   E:/CodeSamples/vcpkg/packages/jsoncpp_x64-windows/bin/jsoncpp.dll
    E:/CodeSamples/vcpkg/packages/jsoncpp_x64-windows/debug/bin/jsoncpp.dll

Call Stack (most recent call first):
  ports/jsoncpp/portfile.cmake:30 (vcpkg_copy_pdbs)
  scripts/ports.cmake:147 (include)

![](E:\CodeSamples\Cpp_Examples\doc\jsonCpp.PNG)

安装完成后, bin目录下为Release版本的 jsoncpp.dll, lib目录下为Release版本的 jsoncpp.lib, include为头文件, debug目录下为debug版本的dll和lib库.



#### 2  使用JsonCpp读取json文件

hwinit.conf为json形式器件配置, 可以动态修改寄存器数值用于硬件调试，内容如下：

```json
{
	"HwVersion": {
		"Version": 1.2,
		"Author": "test",
		"Date": "20220105",
		"Offcial":1
	},

	"ADF4002": {
		"DevId1": [{
				"addr": 1,
				"data": "0x1"
			},
			{
				"addr": 2,
				"data": "0x2"
			},
			{
				"addr": 3,
				"data": "0x3"
			}
		],
		"DevId2": [{
				"addr": 7,
				"data": "0x7"
			},
			{
				"addr": 8,
				"data": "0x8"
			},
			{
				"addr": 9,
				"data": "0x9"
			}
		]
	}

}
```



以下为读取上述数据代码:

```c++
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
```

```c++
Output：
c++version:1.2
author:test
date:20220105
official:true

addr:1,data:0x1
addr:2,data:0x2
addr:3,data:0x3
addr:7,data:0x7
addr:8,data:0x8
addr:9,data:0x9
```



##### 源代码参考github:https://github.com/hero-joey/Cpp_Examples