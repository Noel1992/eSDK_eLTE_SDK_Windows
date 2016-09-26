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
#include "OpenSSL_Mgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"

//lint -e438
OpenSSL_Mgr::OpenSSL_Mgr(void)
	: m_ctx(NULL)
	, m_strCertificateFilePath("")
	, m_strPrivateKeyFilePath("")
{

}

OpenSSL_Mgr::~OpenSSL_Mgr(void)
{
	try
	{
		Clean();
	}
	catch(...)
	{

	}
}//lint !e1579

OpenSSL_Mgr& OpenSSL_Mgr::Instance()
{
	static OpenSSL_Mgr s_OpenSSL_Mgr;
	return s_OpenSSL_Mgr;
}

void OpenSSL_Mgr::SetParam(const std::string& strCertificateFilePath, const std::string& strPrivateKeyFilePath)
{
	INFO_PARAM2(strCertificateFilePath, strPrivateKeyFilePath);
	m_strCertificateFilePath = strCertificateFilePath;
	m_strPrivateKeyFilePath = strPrivateKeyFilePath;
}

int OpenSSL_Mgr::Init_SSL_CTX()
{
	LOG_TRACE();
	// 状态校验
	if (NULL != m_ctx)
	{
		LOG_RUN_ERROR("SSL CTX have been init.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	// SSL 库初始化
	SSL_library_init();
	// 载入所有 SSL 算法
	OpenSSL_add_all_algorithms();
	// 载入所有 SSL 错误消息
	SSL_load_error_strings();

	// TLSv1.2
	const SSL_METHOD* _method = TLSv1_2_server_method();
	if (NULL == _method)
	{
		LOG_RUN_ERROR("TLSv1_2_server_method failed.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}
	m_ctx = SSL_CTX_new(_method);
	if (NULL == m_ctx)
	{
		LOG_RUN_ERROR("SSL_CTX_new failed.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	// 载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥
	if (0 >= SSL_CTX_use_certificate_file(m_ctx, m_strCertificateFilePath.c_str(), SSL_FILETYPE_PEM))
	{
		SSL_CTX_free(m_ctx);
		m_ctx = NULL;
		LOG_RUN_ERROR("SSL_CTX_use_certificate_file failed.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	// 载入用户私钥
	if (0 >= SSL_CTX_use_PrivateKey_file(m_ctx, m_strPrivateKeyFilePath.c_str(), SSL_FILETYPE_PEM))
	{
		SSL_CTX_free(m_ctx);
		m_ctx = NULL;
		LOG_RUN_ERROR("SSL_CTX_use_PrivateKey_file failed.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	// 检查用户私钥是否正确
	if (0 == SSL_CTX_check_private_key(m_ctx))
	{
		SSL_CTX_free(m_ctx);
		m_ctx = NULL;
		LOG_RUN_ERROR("SSL_CTX_check_private_key failed.");
		return eLTE_SVC_ERR_SSLSOCKET_INIT;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

void OpenSSL_Mgr::Uninit_SSL_CTX()
{
	LOG_TRACE();
	Clean();
}

void OpenSSL_Mgr::Clean()
{
	if (NULL != m_ctx)
	{
		SSL_CTX_free(m_ctx);
		m_ctx = NULL;
	}
	m_strCertificateFilePath = "";
	m_strPrivateKeyFilePath = "";
}

SSL* OpenSSL_Mgr::Accept_SSL(int socketfd)
{
	LOG_TRACE();
	// 状态校验
	if (NULL == m_ctx)
	{
		LOG_RUN_ERROR("Please Init SSL CTX first.");
		return NULL;
	}

	// 基于 ctx 产生一个新的 SSL
	SSL* _ssl = SSL_new(m_ctx);
	if (NULL == _ssl)
	{
		LOG_RUN_ERROR("SSL_new failed.");
		return NULL;
	}

	// 将连接用户的 socket 加入到 SSL
	SSL_set_fd(_ssl, socketfd);
	
	// 建立 SSL 连接
	if (-1 == SSL_accept(_ssl))
	{
		// 关闭 SSL 连接
		SSL_shutdown(_ssl);
		// 释放 SSL
		SSL_free(_ssl);
		_ssl = NULL;
		LOG_RUN_ERROR("SSL_accept failed.");
		return NULL;//lint !e438
	}

	return _ssl;
}
