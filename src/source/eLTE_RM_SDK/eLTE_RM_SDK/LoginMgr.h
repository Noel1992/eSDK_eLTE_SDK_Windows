/********************************************************************
filename		: 	LoginMgr.h
author			:	zWX229156
created		    :	2015/1/5
description	    :	�û���¼��
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/5 ��ʼ�汾
*********************************************************************/

#ifndef __LOGIN_MGR_H__
#define __LOGIN_MGR_H__

#include "eLTE_Types.h"
//#include "eLTE_Message.h"
#include <string>

class CUserMgr;//lint !e763
class CLoginMgr
{
public:
	CLoginMgr();
	~CLoginMgr();

	//��¼
	ELTE_INT32 Login(const ELTE_CHAR* pUserId, const ELTE_CHAR* pPasswd, const ELTE_CHAR* pServerIP, const ELTE_CHAR* pLocalIP, const ELTE_UINT32& sipPort) const;

	//ע��
	ELTE_INT32 Logout(const ELTE_CHAR* pUserId) const;

	//����״̬�ϱ�
	ELTE_INT32 TriggerStatusReport(const ELTE_INT32& iEnableStatusReport) const;

	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

private:
	CUserMgr*    m_pUserMgr;
};

#endif // __LOGIN_MGR_H__
