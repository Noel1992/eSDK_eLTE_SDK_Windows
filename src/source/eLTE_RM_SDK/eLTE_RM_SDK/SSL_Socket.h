/********************************************************************
filename		: 	SSL_Socket.h
author			:	zWX229156
created		    :	2015/1/7
description	    :	socket�����ࣨ������˽������ӡ��Ͽ������Լ�������Ϣ��ת����
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/7 ��ʼ�汾
*********************************************************************/

#ifndef __SSL_SOCKET_H__
#define __SSL_SOCKET_H__

#include <string>
#include "eLTE_Types.h"
//#include "eLTE_Message.h"
#include "mutex_locker.h"
//#include "eLTE_Xml.h"

class CUserMgr;//lint !e763
class SSL_Socket
{
public:
	SSL_Socket();
	~SSL_Socket();

	//���÷������˿ں��Լ�IP��ַ
	ELTE_VOID SetParam(ELTE_UINT32 uiPort, const std::string& strHost);
	//��ʼ��socket
	ELTE_INT32 Init_SSL_Socket();
	//�ͷ�socket
	ELTE_VOID Uninit_SSL_Socket();
	//��������
	ELTE_INT32 BeginConnect(const ELTE_INT32& bConnectAgain = FALSE);	
	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);
	//������Ϣ
	ELTE_INT32 SendMsg(const std::string& reqXmlStr);
	//��������
	ELTE_INT32 ConnectAgain();

	//��ջ���
	ELTE_INT32 CleanBuffer();

public:
	static UINT __stdcall StartReceiveScriptServer(LPVOID lpParam);

private:
	//�ͷ���Դ
	ELTE_VOID CloseResource();
	//���ӷ���
	ELTE_INT32 ConnectSSL(const ELTE_INT32& bConnectAgain = FALSE);

public:
	//����
	ELTE_INT32 KeepAlive();

private:
	ELTE_UINT32  m_uiPort;
	std::string  m_strHost;
	ELTE_UINT32  m_socket;
	//ELTE_UINT32  m_socketGTR;
	//ELTE_UINT32  m_socketGTRAccept;
	CUserMgr*    m_pUserMgr;	
};
#endif //__SSL_SOCKET_H__
