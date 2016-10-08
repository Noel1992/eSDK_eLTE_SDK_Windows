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
#include "ConfigMgr.h"
#include "eLTE_Error.h"
#include "eLTE_Tool.h"
#include "UserMgr.h"

// 配置文件读取返回值检测
#define CHECK_READ_CONFIG_FILE(x)			\
	if(0 == x)								\
	{										\
		return eLTE_SDK_ERR_CONFIG_FILE_READ;	\
	}



CConfigMgr::CConfigMgr()
	: m_strServerIP("")
	, m_uiServerPort(0)
	, m_strPassword("")
{

}


CConfigMgr::~CConfigMgr()
{
	try
	{
		Exit();
	}
	catch(...)
	{
		
	}	
}

CConfigMgr& CConfigMgr::Instance()
{
	static CConfigMgr s_ConfigMgr;
	return s_ConfigMgr;
}

ELTE_INT32 CConfigMgr::Init()
{
	// 读取配置文件
	ELTE_INT32 iRet = LoadConfigFile();
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		return iRet;
	}

	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID CConfigMgr::Exit()
{
	m_strServerIP = "";
	m_uiServerPort = 0;
	m_strPassword = "";
}

ELTE_INT32 CConfigMgr::LoadConfigFile()
{
	std::string strPath = eLTE_Tool::GetDllPath(ELTE_SDK_DLL_NAME);
	strPath.append(CONFIG_FILE_NAME);

	const ELTE_INT32 LENGTH = 256;
	ELTE_CHAR val[LENGTH] = {0};

	// ServerInfo
	/*ELTE_ULONG dwRet = GetPrivateProfileString(SECTION_SERVER_INFO, KEY_SERVER_IP, "", val, LENGTH, strPath.c_str());
	CHECK_READ_CONFIG_FILE(dwRet);
	m_strServerIP = val;*/

	eSDK_MEMSET(val, 0, sizeof(char)*LENGTH);
	ELTE_ULONG dwRet = GetPrivateProfileString(SECTION_SERVER_INFO, KEY_SERVER_PORT, "", val, LENGTH, strPath.c_str());
	CHECK_READ_CONFIG_FILE(dwRet);
	m_uiServerPort = eLTE_Tool::String2UInt(val);

	//m_strPassword = eLTE_Tool::GetEncryptPassword(strPath);

	return eLTE_SDK_ERR_SUCCESS;
}
