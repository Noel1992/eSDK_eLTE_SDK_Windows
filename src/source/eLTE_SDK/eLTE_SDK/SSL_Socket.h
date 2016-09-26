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
#include "eLTE_Message.h"
#include "mutex_locker.h"
#include "eLTE_Xml.h"

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
	ELTE_INT32 BeginConnect();	
	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);
	//������Ϣ
	ELTE_INT32 SendMsg(const ELTE_USHORT& usMsgType, CXml& reqXml, const ELTE_INT32& bBodyExist);
	//��������
	ELTE_INT32 ConnectAgain();
	//�Ͽ�����
	ELTE_INT32 DisConnect();
	//��ȡ���������¼�����
	HANDLE GetEventHandle() {return m_EventConnectReq;};
	//�������ӻ�Ӧ������
	ELTE_VOID SetPacketData(const PACKET_DATA& packetData);
	//��ȡ���ӻ�Ӧ������
	const PACKET_DATA& GetPacketData() const {return m_connectReqPacketData;};
	//���ӷ�����֤
	ELTE_INT32 Connect();
	//���� ���ϡ��������������߳�
	ELTE_INT32 StartSendConnectMsg();
	//ֹͣ ���ϡ��������������߳�
	ELTE_INT32 StopSendConnectMsg();
	//��Ϣ�����߳�
	static ELTE_UINT32 __stdcall ConnectMsgThread(ELTE_VOID* param);
private:
	//�ͷ���Դ
	ELTE_VOID CloseResource();
	//��ȡ���к�
	ELTE_UINT32 GetSeqId();
	//���ӷ���
	ELTE_INT32 ConnectSSL(const ELTE_INT32& bConnectAgain = FALSE);
	//����
	ELTE_INT32 KeepAlive();
	//����������������
	ELTE_VOID DoRevMsg();

private:
	ELTE_UINT32  m_uiPort;
	std::string  m_strHost;
	ELTE_UINT32  m_socket;
	CUserMgr*    m_pUserMgr;
	ELTE_UINT32  m_uiSeqId;
	MUTEX_HANDLE m_MutexSeqId;
	HANDLE       m_EventConnectReq;
	PACKET_DATA  m_connectReqPacketData;
	ELTE_UINT32 m_connectMsgThreadId;
	HANDLE      m_connectMsgThread;
	ELTE_INT32  m_bStopConMsgThtread;
};
#endif //__SSL_SOCKET_H__
