/********************************************************************
filename		: 	ProvisionMgr.h
author			:	zWX229156
created		    :	2015/1/6
description	    :	����̨���ù�����
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/6 ��ʼ�汾
*********************************************************************/

#ifndef __PROVISION_MGR_H__
#define __PROVISION_MGR_H__

#include "eLTE_Types.h"
//#include "eLTE_Message.h"
#include <string>

class CUserMgr;//lint !e763
class CProvisionMgr
{
public:
	CProvisionMgr();
	~CProvisionMgr();

	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//��ȡ����̨��Ⱥ���б�
	ELTE_INT32 GetDcGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcGroups) const;

	//��ȡ����̨�û��б�
	ELTE_INT32 GetDcUsers(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcUsers) const;

	//��ȡһ��Ⱥ���Ա�б�
	ELTE_INT32 GetGroupUsers(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupUsers) const;

	//��ȡһ��Ⱥ����ϸ��Ϣ
	ELTE_INT32 GetGroupInfo(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupInfo) const;

	//��ȡһ���û���ϸ��Ϣ
	ELTE_INT32 GetUserInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pUserInfo) const;
	
	//��ȡһ������̨��ϸ��Ϣ
	ELTE_INT32 GetDcInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcInfo) const;

private:
	CUserMgr*   m_pUserMgr;
};

#endif // __PROVISION_MGR_H__
