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
#include <string>

#define CONSTRUCT_XML_HEAD(nodeName,nodeValue) \
	CXml reqXml;	\
	(void)reqXml.AddElem("Content");	\
	(void)reqXml.AddChildElem(nodeName);	\
	(void)reqXml.IntoElem();	\
	(void)reqXml.SetElemValue(nodeValue);	\

	
#define WAIT_SERVER_RSP() \
	iRet = m_pUserMgr->WaitObject(WAIT_OBJECT_TIME);									\
	if (eLTE_SDK_ERR_SUCCESS != iRet)													\
	{																					\
		CServerMgr& serverMgr = const_cast<CServerMgr&>(m_pUserMgr->GetServerMgr());	\
		if(!serverMgr.ServerIsRunning() || 0 != m_pUserMgr->GetServerStatus())			\
		{																				\
			m_pUserMgr->SetServerStatus(0);												\
			return eLTE_SDK_ERR_SERVER_NOT_RUNNING;										\
		}																				\
		return iRet;																	\
	}																					\


class CUserMgr;//lint !e763
class CProvisionMgr
{
public:
	CProvisionMgr();
	~CProvisionMgr();

	//���ù����ʼ��
	ELTE_INT32 ProvisionManagerInit(const ELTE_CHAR* pServerIP,const ELTE_CHAR*pUserID) const;

	//ý���������ʼ��
	ELTE_INT32 ProvisionManagerInitMRS(const ELTE_CHAR* pServerIP) const;

	//�˳����ù���
	ELTE_INT32 ProvisionManagerExit() const;

	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//��ȡ����̨��Ⱥ���б�
	ELTE_INT32 GetDcGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcGroups) const;

	//��ȡ����̨�û��б�
	ELTE_INT32 GetDcUsers(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcUsers) const;

	//��ȡһ��Ⱥ���Ա�б�
	ELTE_INT32 GetGroupUsers(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupUsers) const;
	
	//��ȡָ������̨�������ɽ����б�
	ELTE_INT32 GetPatchGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pPatchGroups) const;

	//��ȡָ���ɽ������ĳ�Ա�б�
	ELTE_INT32 GetGroupMemberByPatchId(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupMembers) const;

	//��ȡ�ɽ���������Ϣ
	ELTE_INT32 GetPatchGroupInfo(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupInfo) const;

	//��ȡһ��Ⱥ����ϸ��Ϣ
	ELTE_INT32 GetGroupInfo(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupInfo) const;

	//��ȡһ���û���ϸ��Ϣ
	ELTE_INT32 GetUserInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pUserInfo) const;
	
	//��ȡһ������̨��ϸ��Ϣ
	ELTE_INT32 GetDcInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcInfo) const;

	//��ȡ��Ƶ��ǽID�б�
	ELTE_INT32 GetDcVWallIDList(ELTE_CHAR** pVWallIDList) const;

	//��ѯ�ն˵�GIS������Ϣ
	ELTE_INT32 GetUserSpecificGISCfg(const ELTE_CHAR* pResourceID, ELTE_CHAR** pUeGisCfg) const;

	//��ȡ����̨���ĵ��ն��б�
	ELTE_INT32 GetGisSubscription(const ELTE_CHAR* pResourceID, ELTE_CHAR** pGisQuerySubList) const;

private:
	CUserMgr*   m_pUserMgr;
};

#endif // __PROVISION_MGR_H__
