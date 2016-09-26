/********************************************************************
filename		: 	ServerMgr.h
author			:	zWX229156
created		    :	2015/1/8
description	    :	��������ࣨ������������ȡ�����ļ������ӷ����Լ��Ͽ�����
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/8 ��ʼ�汾
*********************************************************************/

#ifndef __SERVER_MGR_H__
#define __SERVER_MGR_H__

#include "eLTE_Types.h"
#include "eLTE_Message.h"
#include <string>

class CUserMgr;//lint !e763
class CServerMgr
{
public:
	CServerMgr();
	~CServerMgr();

	//������Դ
	ELTE_INT32 Init(ELTE_INT32 iMediaBypass);

	//�ͷ���Դ
	ELTE_INT32 CleanUp() const;

	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//��ȡ��ʼ����־
	ELTE_INT32 GetInitValue() const {return m_bInit;};

	//��������
	ELTE_VOID StartServer() const;

	//ֹͣ����
	ELTE_VOID StopServer() const;

	//�жϷ���״̬
	ELTE_INT32 ServerIsRunning() const;

private:
	CUserMgr*   m_pUserMgr;
	ELTE_INT32  m_bInit;
	ELTE_INT32  m_bMediaBypass;
};

#endif // __SERVER_MGR_H__
