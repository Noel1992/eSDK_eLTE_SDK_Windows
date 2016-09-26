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
filename    :    eLTE_Error.h
author      :    yubinbin wx200189
created     :    2015/01/12
description :    ����˺Ϳͻ��˴����붨��(����˺Ϳͻ��˹����ļ�)
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_ERROR_H__
#define __ELTE_ERROR_H__


//////////////////////////////////////////////////////////////////////////
// eLTE_SDK �����붨�� 2135102XXX
typedef enum
{
	eLTE_SDK_ERR_SUCCESS = 0,					// �ɹ�

	// 2135102001 - 2135102100 ��ͨ����
	eLTE_SDK_ERR_FAILED = 2135102001,			// ͨ��ʧ��
	eLTE_SDK_ERR_INVALID_PARAM,					// �Ƿ����
	eLTE_SDK_ERR_NULL_POINTER,					// ��ָ�����
	eLTE_SDK_ERR_CREATE_OBJECT,                 // ��������ʧ��
	eLTE_SDK_ERR_CREATE_DIRECTORY,              // ����Ŀ¼ʧ��
	eLTE_SDK_ERR_LOG_INIT,                      // ��־ģ���ʼ��ʧ��
	eLTE_SDK_ERR_SDL_INIT,                      // SDL��ʼ��ʧ��

	// 2135102101 - 2135102200 �������
	eLTE_SDK_ERR_WSA_START_UP = 2135102101,     // ��ʼ���׽��ֶ�̬��ʧ��
	eLTE_SDK_ERR_CONNECT_SERVER,                // ���ӷ�����ʧ��
	eLTE_SDK_ERR_SSL_CONNECT,                   // SSL����ʧ��
	eLTE_SDK_ERR_SEND_MSG,                      // ��������ʧ��
	eLTE_SDK_ERR_WAIT_TIME_OUT,                 // �ȴ���ʱ
	eLTE_SDK_ERR_SERVER_NOT_RUNNING,            // ����δ����

	// 2135102201 - 2135102300 �ļ���������
	eLTE_SDK_ERR_CONFIG_FILE_LOAD = 2135102201,	// ����ini�����ļ�ʧ��
	eLTE_SDK_ERR_CONFIG_FILE_READ,				// ��ȡini�����ļ�ʧ��
	eLTE_SDK_ERR_CONFIG_FILE_WRITE,				// д��ini�����ļ�ʧ��

	//2135102301 - 2135102400 XML��������
	eLTE_SDK_ERR_XML_CONSTRUCT = 2135102301,	// ����XML�ַ���ʧ��
	eLTE_SDK_ERR_XML_PARSE,						// ����XML�ַ���ʧ��
	eLTE_SDK_ERR_XML_FIND_ELEM,					// ����XML�ڵ�ʧ��
	eLTE_SDK_ERR_XML_GET_ELEM_VALUE,		    // ��ȡXML�ڵ�ʧ��


	eLTE_SDK_ERR_SERVICE_EXPIRE	= 2135103009   // ������ڣ��ͻ�����Ҫ��������
} eLTE_SDK_RET_ERROR_CODE_E;

//////////////////////////////////////////////////////////////////////////
// eLTE_NativeService �����붨�� 2135103XXX
typedef enum
{
	eLTE_SVC_ERR_SUCCESS = 0,					// �ɹ�

	// 2135103001 - 2135103100 ��ͨ����
	eLTE_SVC_ERR_FAILED = 2135103001,			// ͨ��ʧ��
	eLTE_SVC_ERR_INVALID_PARAM,					// �Ƿ����
	eLTE_SVC_ERR_NULL_POINTER,					// ��ָ�����
	eLTE_SVC_ERR_NOT_LOGIN,						// û�е�¼
	eLTE_SVC_ERR_WAIT_LOGIN_RESULT,				// ��¼��
	eLTE_SVC_ERR_MEMORY_APPLY,					// �ڴ��������
	eLTE_SVC_ERR_NEW_SESSIONID,					// ��ȡSessionIDʧ��
	eLTE_SVC_ERR_INVALID_SESSIONID,				// �Ƿ�SessionID
	eLTE_SVC_ERR_SERVICE_EXPIRE,				// ������ڣ��ͻ�����Ҫ��������
	eLTE_SVC_ERR_CONNECT_PASSWD,				// �������벻��ȷ

	// 2135103101 - 2135103200 �������
	eLTE_SVC_ERR_SSLSOCKET_INIT = 2135103101,	// �����׽��ֳ�ʼ��ʧ��
	eLTE_SVC_ERR_SSLSOCKET_RECV,				// �����׽��ֽ���ʧ��
	eLTE_SVC_ERR_SSLSOCKET_SEND,				// �����׽��ַ���ʧ��

	// 2135103201 - 2135103300 �ļ���������
	eLTE_SVC_ERR_CONFIG_FILE_LOAD = 2135103201,	// ����ini�����ļ�ʧ��
	eLTE_SVC_ERR_CONFIG_FILE_READ,				// ��ȡini�����ļ�ʧ��
	eLTE_SVC_ERR_CONFIG_FILE_WRITE,				// д��ini�����ļ�ʧ��

	//2135103301 - 2135103400 XML��������
	eLTE_SVC_ERR_XML_CONSTRUCT = 2135103301,	// ����XML�ַ���ʧ��
	eLTE_SVC_ERR_XML_PARSE,						// ����XML�ַ���ʧ��
	eLTE_SVC_ERR_XML_FIND_ELEM,					// ����XML�ڵ�ʧ��
	eLTE_SVC_ERR_XML_GET_ELEM_VALUE				// ��ȡXML�ڵ�ʧ��

} eLTE_SERVICE_RET_ERROR_CODE_E;

//////////////////////////////////////////////////////////////////////////

#endif
