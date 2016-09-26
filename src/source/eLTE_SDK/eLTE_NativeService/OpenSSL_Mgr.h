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
filename    :    OpenSSL_Mgr.h
author      :    yubinbin wx200189
created     :    2015/01/12
description :    封装openssl接口，安全加密socket通信
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 初始版本
*********************************************************************/

#ifndef __OPENSSL_MGR_H__
#define __OPENSSL_MGR_H__

#include <string>
#include "openssl/ssl.h"


class OpenSSL_Mgr
{
private:
	OpenSSL_Mgr(void);
	~OpenSSL_Mgr(void);

public:
	static OpenSSL_Mgr& Instance();
	void SetParam(const std::string& strCertificateFilePath, const std::string& strPrivateKeyFilePath);
	int Init_SSL_CTX();
	void Uninit_SSL_CTX();
	SSL* Accept_SSL(int socketfd);

private:
	void Clean();

private:
	SSL_CTX* m_ctx;
	std::string m_strCertificateFilePath;
	std::string m_strPrivateKeyFilePath;
};

#endif
