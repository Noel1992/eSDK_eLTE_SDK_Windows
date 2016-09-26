#include "stdafx.h"
//log manage class 
#include "eLTE_Log.h"


FunctionPrint::FunctionPrint(const std::string& functionName)
	: m_functionName(functionName)
{
	std::string strLog("{");
	strLog.append(m_functionName);
	strLog.append("} --- ENTRY");
	Log_Run_Debug(LOG_MODULE_NAME, strLog.c_str());
}

FunctionPrint::~FunctionPrint()
{
	try
	{
		std::string strLog("{");
		strLog.append(m_functionName);
		strLog.append("} --- LEAVE");
		Log_Run_Debug(LOG_MODULE_NAME, strLog.c_str());
	}
	catch (...)
	{

	}
}
