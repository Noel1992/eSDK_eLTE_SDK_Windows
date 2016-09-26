/********************************************************************
filename    :    eLTE_Error.h
author      :    zWX229156
created     :    2015/10/12
description :    �����붨��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/10/12 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_ERROR_H__
#define __ELTE_ERROR_H__


//////////////////////////////////////////////////////////////////////////
// eLTE_RM_SDK �����붨�� 2135102XXX
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
} eLTE_RM_SDK_RET_ERROR_CODE_E;

//////////////////////////////////////////////////////////////////////////

#endif
