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

#include "stdafx.h"
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
