/*Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
eSDK is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
		http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

/********************************************************************
filename    :    eLTE_Log.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    日志管理类。使用平台eSDKLogAPI模块，主要进行日志文件输出
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_LOG_H__
#define __ELTE_OCX_LOG_H__

#include "DataType.h"
#include "eSDKLogAPI.h"
#include <sstream>


//lint -e773 -e1788 -e1712 -e506 -e774
//此处宏函数不能加括号
//////////////////////////////////////////////////////////////////////////
// 日志采集宏定义 
#define LOG_INI_FILE_NAME		"eSDKClientLogCfgOcx.ini"
#define PRODUCT_NAME			"eSDK-eLTE-OCX-Windows-Cpp"
#define INTERFACE_TYPE          "1"
#define PROTOCOL_TYPE           "COM"

#define LOG_INIT(inifile, loglevel, logpath)\
	LogInit(PRODUCT_NAME, inifile, loglevel, logpath);

#define LOG_EXIT()\
	LogFini(PRODUCT_NAME)

#define LOG_INTERFACE_INFO(resCode, fmt, ...) {\
	SYSTEMTIME rspTime;\
	GetLocalTime(&rspTime);\
	char strReqTime[256] = {0};\
	char strRspTime[256] = {0};\
	(void)_snprintf_s(strReqTime,256,_TRUNCATE,"%04d-%02d-%02d %02d:%02d:%02d %03d",reqTime.wYear,reqTime.wMonth,reqTime.wDay,reqTime.wHour,reqTime.wMinute,reqTime.wSecond,reqTime.wMilliseconds);\
	(void)_snprintf_s(strRspTime,256,_TRUNCATE,"%04d-%02d-%02d %02d:%02d:%02d %03d",rspTime.wYear,rspTime.wMonth,rspTime.wDay,rspTime.wHour,rspTime.wMinute,rspTime.wSecond,rspTime.wMilliseconds);\
	char strResCode[256] = {0};\
	(void)_itoa_s(resCode, strResCode, 256, 10);\
	if(0 == resCode)\
	{\
		Log_Interface_Info(PRODUCT_NAME,INTERFACE_TYPE,PROTOCOL_TYPE,__FUNCTION__,"","","",strReqTime,strRspTime,strResCode,fmt,##__VA_ARGS__);\
	}\
	else\
	{\
		Log_Interface_Error(PRODUCT_NAME,INTERFACE_TYPE,PROTOCOL_TYPE,__FUNCTION__,"","","",strReqTime,strRspTime,strResCode,fmt,##__VA_ARGS__);\
	}\
}

#define LOG_RUN_DEBUG(fmt, ...) {\
	char buffer[1024] = {0};\
	(void)_snprintf_s(buffer,1024,_TRUNCATE,fmt,##__VA_ARGS__);\
	std::string strLog("{");\
	strLog.append(__FUNCTION__);\
	strLog.append("} ");\
	strLog.append(buffer);\
	Log_Run_Debug(PRODUCT_NAME,strLog.c_str());\
}

#define LOG_RUN_INFO(fmt, ...) {\
	char buffer[1024] = {0};\
	(void)_snprintf_s(buffer,1024,_TRUNCATE,fmt,##__VA_ARGS__);\
	std::string strLog("");\
	strLog.append(buffer);\
	Log_Run_Info(PRODUCT_NAME,strLog.c_str());\
}

#define LOG_RUN_WARN(fmt, ...) {\
	char buffer[1024] = {0};\
	(void)_snprintf_s(buffer,1024,_TRUNCATE,fmt,##__VA_ARGS__);\
	std::string strLog("");\
	strLog.append(buffer);\
	Log_Run_Warn(PRODUCT_NAME,strLog.c_str());\
}

#define LOG_RUN_ERROR(fmt, ...) {\
	char buffer[1024] = {0};\
	(void)_snprintf_s(buffer,1024,_TRUNCATE,fmt,##__VA_ARGS__);\
	std::string strLog("");\
	strLog.append(buffer);\
	Log_Run_Error(PRODUCT_NAME,strLog.c_str());\
}

#define LOG_TRACE()
#define LOG_INTERFACE_TRACE() \
	FunctionPrint functionPrint(__FUNCTION__);\
	SYSTEMTIME reqTime;\
	GetLocalTime(&reqTime)


//带参数打印info级别日志，最多6个参数
#define FUNCNAME() "{" << __FUNCTION__ << "} "
#define EXP1(param1) #param1 << " = " << (param1)
#define EXP2(param1, param2) \
	EXP1(param1) << ", " << EXP1(param2)
#define EXP3(param1, param2, param3) \
	EXP2(param1, param2) << ", " << EXP1(param3)
#define EXP4(param1, param2, param3, param4) \
	EXP3(param1, param2, param3) << ", " << EXP1(param4)
#define EXP5(param1, param2, param3, param4, param5) \
	EXP4(param1, param2, param3, param4) << ", " << EXP1(param5)
#define EXP6(param1, param2, param3, param4, param5, param6) \
	EXP5(param1, param2, param3, param4, param5) << ", " << EXP1(param6)

#define INFO_PARAM1(param1) {\
	std::ostringstream ostr;\
	ostr<< EXP1(param1);\
	Log_Run_Info(PRODUCT_NAME,ostr.str().c_str());\
}

#define INFO_PARAM2(param1, param2) {\
	std::ostringstream ostr;\
	ostr << EXP2(param1, param2);\
	Log_Run_Info(PRODUCT_NAME,ostr.str().c_str());\
}

#define INFO_PARAM3(param1, param2, param3) {\
	std::ostringstream ostr;\
	ostr << EXP3(param1, param2, param3);\
	Log_Run_Info(PRODUCT_NAME,ostr.str().c_str());\
}

#define INFO_PARAM4(param1, param2, param3, param4) {\
	std::ostringstream ostr;\
	ostr << EXP4(param1, param2, param3, param4);\
	Log_Run_Info(PRODUCT_NAME,ostr.str().c_str());\
}

#define INFO_PARAM5(param1, param2, param3, param4, param5) {\
	std::ostringstream ostr;\
	ostr << EXP5(param1, param2, param3, param4, param5);\
	Log_Run_Info(PRODUCT_NAME,ostr.str().c_str());\
}

#define INFO_PARAM6(param1, param2, param3, param4, param5, param6) {\
	std::ostringstream ostr;\
	ostr << EXP6(param1, param2, param3, param4, param5, param6);\
	Log_Run_Info(PRODUCT_NAME,ostr.str().c_str());\
}
//////////////////////////////////////////////////////////////////////////


//日志类
class eLTE_Log
{
public:
	static eLTE_Log& Instance();
	eLTE_RET SetLogPath(const std::string& logPath);
	eLTE_RET SetLogLevel(unsigned int logLevel);

private:
	eLTE_Log();
	~eLTE_Log();

public:
	std::string m_logPath;
	unsigned int m_logLevel;
};


// 函数进出类
class FunctionPrint
{
public:
	FunctionPrint(const std::string& functionName);
	~FunctionPrint();

private:
	std::string m_functionName;
};

#endif
