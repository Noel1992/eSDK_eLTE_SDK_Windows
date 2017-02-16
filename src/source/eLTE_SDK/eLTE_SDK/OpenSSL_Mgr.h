/********************************************************************
filename		: 	OpenSSL_Mgr.h
author			:	zWX229156
created		    :	2015/1/7
description	    :	ssl�����ࣨ����ctx��sslʵ�������Լ��ͷţ���������ݵĽ��ܡ��洢�Լ�����
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/7 ��ʼ�汾
*********************************************************************/
#ifndef __OPENSSL_MGR_H__
#define __OPENSSL_MGR_H__

#include <openssl/ssl.h>
#include <queue>
#include "eLTE_Types.h"
#include "eLTE_Message.h"
#include "mutex_locker.h"

class CUserMgr;//lint !e763
class OpenSSL_Mgr
{
private:
	OpenSSL_Mgr();
	~OpenSSL_Mgr();

public:
	//��ȡʵ��
	static OpenSSL_Mgr& Instance();
	//��ʼ��ctx
	ELTE_INT32 Init_SSL_CTX();
	//�ͷ�ctx�Լ�ssl
	ELTE_VOID Uninit_SSL_CTX();
	//����ssl
	ELTE_INT32 Connect_SSL(ELTE_INT32 socketfd);
	//�������������߳�
	ELTE_INT32 StartRevMsg();
	//���������߳�
	static ELTE_UINT32 __stdcall RevMsgThread(ELTE_VOID* param);
	//ֹͣ���������߳�
	ELTE_INT32 StopRevMsg();
	//������Ϣ�����߳�
	ELTE_INT32 StartProMsg();
	//��Ϣ�����߳�
	static ELTE_UINT32 __stdcall ProMsgThread(ELTE_VOID* param);
	//ֹͣ��Ϣ�����߳�
	ELTE_INT32 StopProMsg();
	//��������
	ELTE_INT32 SendMsg(const ELTE_VOID* pData, const ELTE_UINT32& uiLength);
	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);
private:
	//���ܷ���������������
	ELTE_VOID DoRevMsg();
	//��Ϣ����
	ELTE_VOID DoProMsg();
	//�ͷ���Դ
	ELTE_VOID CloseResource();
	//�ַ���Ϣ
	ELTE_VOID DispatchMsg(PACKET_DATA& Packet);
	//�ͷ�����
	ELTE_VOID CleanMsgQuene();
	//�����¼���packetdata
	ELTE_VOID ProcessPacketData(const PACKET_DATA& packetData);

	//��Ϣ�ַ��������͵�Ȧ���Ӷ��ж�
	ELTE_INT32 CheckDispatchMsg(PACKET_DATA& Packet, ELTE_UINT32 Type);


private:
	SSL_CTX* m_ctx;
	SSL* m_ssl;
	HANDLE m_revThread;
	ELTE_UINT32 m_revThreadId;
	ELTE_INT32 m_bStopRevThtread;
	std::queue<PACKET_DATA> m_msgQueue;
	MUTEX_HANDLE m_MutexMsgQueue;
	HANDLE m_proMsgThread;
	ELTE_UINT32 m_proMsgThreadId;
	ELTE_INT32  m_bStopProMsgThtread;
	MUTEX_HANDLE m_MutexSSL;
	CUserMgr* m_pUserMgr;
};

#endif
