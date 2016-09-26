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

#include "StdAfx.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"


eLTE_Log::eLTE_Log()
	: m_logPath(INVALID_LOG_PATH)
	, m_logLevel(INVALID_LOG_LEVEL)
{

}

eLTE_Log::~eLTE_Log()
{
	try
	{
		m_logPath = INVALID_LOG_PATH;
		m_logLevel = INVALID_LOG_LEVEL;
	}
	catch(...){}
}

eLTE_Log& eLTE_Log::Instance()
{
	static eLTE_Log s_eLTE_Log;
	return s_eLTE_Log;
}

eLTE_RET eLTE_Log::SetLogPath(const std::string& logPath)
{
	// 相对目录转绝对目录
	m_logPath = logPath;
	eLTE_Tool::RelativePathToAbsolute(m_logPath);

	// 如果存在同名文件，则重命名文件，再新建日志目录
	if(!access(m_logPath.c_str(), F_OK ))
	{
		DWORD dwAttr = GetFileAttributes(eLTE_Tool::ANSIToUnicode(m_logPath).c_str());
		if( INVALID_FILE_ATTRIBUTES == dwAttr )
		{
			LOG_RUN_ERROR("GetFileAttributes failed %s.", m_logPath.c_str());
			return eLTE_ERR_FAILED;
		}
		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
		{
			//dir
			return eLTE_ERR_SUCCESS;
		}
		CTime time = CTime::GetCurrentTime();
		CString szTime = time.Format(_T("%Y%m%d%H%M%S"));
		std::string strFileNewName = m_logPath;
		strFileNewName.append("_");
		strFileNewName.append(eLTE_Tool::UnicodeToANSI(szTime.GetBuffer()));
		if(!MoveFile(eLTE_Tool::ANSIToUnicode(m_logPath).c_str(), eLTE_Tool::ANSIToUnicode(strFileNewName).c_str()))
		{
			LOG_RUN_ERROR("Rename failed, oldFileName:%s, newFileName:%s.", m_logPath.c_str(), strFileNewName.c_str());
			return eLTE_ERR_FAILED;
		}
		LOG_RUN_DEBUG("Rename successed, oldFileName:%s, newFileName:%s.", m_logPath.c_str(), strFileNewName.c_str());
	}

	return eLTE_ERR_SUCCESS;
}

eLTE_RET eLTE_Log::SetLogLevel(unsigned int logLevel)
{
	// 参数校验
	if (ERROR_LEVEL < logLevel)
	{
		// 日志级别不正确
		return eLTE_ERR_FAILED;
	}

	m_logLevel = logLevel;

	return eLTE_ERR_SUCCESS;
}

FunctionPrint::FunctionPrint(const std::string& functionName)
	: m_functionName(functionName)
{
	std::string strLog("{");
	strLog.append(m_functionName);
	strLog.append("} --- ENTRY");
	Log_Run_Debug(PRODUCT_NAME,strLog.c_str());
}

FunctionPrint::~FunctionPrint()
{
	try
	{
		std::string strLog("{");
		strLog.append(m_functionName);
		strLog.append("} --- LEAVE");
		Log_Run_Debug(PRODUCT_NAME,strLog.c_str());
	}
	catch (...)
	{

	}
}
