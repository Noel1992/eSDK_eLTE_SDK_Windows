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
author      :    yubinbin wx200189
created     :    2015/01/12
description :    Ini配置文件管理类，负责读取Ini文件，并初始化本地参数
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_CONFIG_MGR_H__
#define __ELTE_OCX_CONFIG_MGR_H__


#define CONFIG_FILE_NAME			"eLTE_NativeService.ini"

#define SECTION_SERVER_INFO			"ServerInfo"
#define SECTION_CERTIFICATE			"Certificate"

#define KEY_SERVER_IP				"IP"
#define KEY_SERVER_PORT				"Port"
#define KEY_CERT_FILE				"CertFile"
#define KEY_KEY_FILE				"KeyFile"

class ConfigMgr
{
private:
	ConfigMgr(void);
	~ConfigMgr(void);

public:
	static ConfigMgr& Instance();
	int Init();
	void Exit();

private:
	int LoadConfigFile();

private:
	// ServerInfo
	std::string m_strServerIP;
	unsigned int m_uiServerPort;

	// Certificate
	std::string m_strCertFile;
	std::string m_strKeyFile;
};

#endif
