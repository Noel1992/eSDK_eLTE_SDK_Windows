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
#include "SSL_Socket.h"
#include "OpenSSL_Mgr.h"


// 配置文件读取返回值检测
#define CHECK_READ_CONFIG_FILE(x)			\
	if (0 == x)								\
	{										\
		return eLTE_SVC_ERR_CONFIG_FILE_READ;	\
	}


ConfigMgr::ConfigMgr(void)
	: m_strServerIP("")
	, m_uiServerPort(0)
	, m_strCertFile("")
	, m_strKeyFile("")
{

}


ConfigMgr::~ConfigMgr(void)
{
	try
	{
		Exit();
	}
	catch (...)
	{
		
	}
}

ConfigMgr& ConfigMgr::Instance()
{
	static ConfigMgr s_ConfigMgr;
	return s_ConfigMgr;
}

int ConfigMgr::Init()
{
	// 读取配置文件
	int iRet = LoadConfigFile();
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		return iRet;
	}

	// 设置服务器IP和端口
	SSL_Socket::Instance().SetParam(m_uiServerPort, m_strServerIP);

	// 相对路径转绝对路径
	eLTE_Tool::RelativePathToAbsolute(m_strCertFile);
	eLTE_Tool::RelativePathToAbsolute(m_strKeyFile);

	// 设置openssl证书路径
	OpenSSL_Mgr::Instance().SetParam(m_strCertFile, m_strKeyFile);

	return eLTE_SVC_ERR_SUCCESS;
}

void ConfigMgr::Exit()
{
	m_strServerIP = "";
	m_uiServerPort = 0;
	m_strCertFile = "";
	m_strKeyFile = "";
}

int ConfigMgr::LoadConfigFile()
{
	std::string strPath = eLTE_Tool::GetServicePath();
	strPath.append(CONFIG_FILE_NAME);

	const int LENGTH = 256;
	char val[LENGTH] = {0};

	// ServerInfo
	/*DWORD dwRet = GetPrivateProfileString(SECTION_SERVER_INFO, KEY_SERVER_IP, "", val, LENGTH, strPath.c_str());
	CHECK_READ_CONFIG_FILE(dwRet);
	m_strServerIP = val;*/

	eSDK_MEMSET(val, 0, sizeof(char)*LENGTH);
	DWORD dwRet = GetPrivateProfileString(SECTION_SERVER_INFO, KEY_SERVER_PORT, "", val, LENGTH, strPath.c_str());
	CHECK_READ_CONFIG_FILE(dwRet);
	m_uiServerPort = eLTE_Tool::String2UInt(val);

	// Certificate
	eSDK_MEMSET(val, 0, sizeof(char)*LENGTH);
	dwRet = GetPrivateProfileString(SECTION_CERTIFICATE, KEY_CERT_FILE, "", val, LENGTH, strPath.c_str());
	CHECK_READ_CONFIG_FILE(dwRet);
	m_strCertFile = val;

	eSDK_MEMSET(val, 0, sizeof(char)*LENGTH);
	dwRet = GetPrivateProfileString(SECTION_CERTIFICATE, KEY_KEY_FILE, "", val, LENGTH, strPath.c_str());
	CHECK_READ_CONFIG_FILE(dwRet);
	m_strKeyFile = val;

	return eLTE_SVC_ERR_SUCCESS;
}
