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
filename    :    ConfigMgr.h
author      :    zWX229156
created     :    2015/01/07
description :    �����ļ���ȡ��
copyright   :    Copyright (c) 2015-2017 Huawei Tech.Co.,Ltd
history     :    2015/01/07 ��ʼ�汾
*********************************************************************/

#ifndef __CONFIG_MGR_H__
#define __CONFIG_MGR_H__

#include "eLTE_Types.h"
#include <string>


#define CONFIG_FILE_NAME			"eLTE_SDK.ini"

#define SECTION_SERVER_INFO			"ServerInfo"

#define KEY_SERVER_IP				"IP"
#define KEY_SERVER_PORT				"Port"

class CConfigMgr
{
private:
	CConfigMgr();
	~CConfigMgr();

public:
	//ʵ��������
	static CConfigMgr& Instance();
	//��ʼ��
	ELTE_INT32 Init();
	//ȥ��ʼ��
	ELTE_VOID Exit();
	//��ȡ������IP
	const std::string& GetServerIP() const {return m_strServerIP;};
	//��ȡ�������˿ں�
	const ELTE_UINT32& GetServerPort() const {return m_uiServerPort;};
	//��ȡ��������
	const std::string& GetPassword() const {return m_strPassword;};

private:
	//��ȡ�����ļ�����
	ELTE_INT32 LoadConfigFile();

private:
	// ServerInfo
	std::string m_strServerIP;
	ELTE_UINT32 m_uiServerPort;
	std::string m_strPassword;
};

#endif
