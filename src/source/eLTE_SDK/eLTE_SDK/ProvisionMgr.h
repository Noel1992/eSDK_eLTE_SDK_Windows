/********************************************************************
filename		: 	ProvisionMgr.h
author			:	zWX229156
created		    :	2015/1/6
description	    :	调度台配置管理类
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/6 初始版本
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

	//配置管理初始化
	ELTE_INT32 ProvisionManagerInit(const ELTE_CHAR* pServerIP,const ELTE_CHAR*pUserID) const;

	//媒体服务器初始化
	ELTE_INT32 ProvisionManagerInitMRS(const ELTE_CHAR* pServerIP) const;

	//退出配置管理
	ELTE_INT32 ProvisionManagerExit() const;

	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//获取调度台的群组列表
	ELTE_INT32 GetDcGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcGroups) const;

	//获取调度台用户列表
	ELTE_INT32 GetDcUsers(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcUsers) const;

	//获取一个群组成员列表
	ELTE_INT32 GetGroupUsers(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupUsers) const;
	
	//获取指定调度台的所有派接组列表
	ELTE_INT32 GetPatchGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pPatchGroups) const;

	//获取指定派接组管理的成员列表
	ELTE_INT32 GetGroupMemberByPatchId(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupMembers) const;

	//获取派接组属性信息
	ELTE_INT32 GetPatchGroupInfo(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupInfo) const;

	//获取一个群组详细信息
	ELTE_INT32 GetGroupInfo(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupInfo) const;

	//获取一个用户详细信息
	ELTE_INT32 GetUserInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pUserInfo) const;
	
	//获取一个调度台详细信息
	ELTE_INT32 GetDcInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcInfo) const;

	//获取视频上墙ID列表
	ELTE_INT32 GetDcVWallIDList(ELTE_CHAR** pVWallIDList) const;

	//查询终端的GIS配置信息
	ELTE_INT32 GetUserSpecificGISCfg(const ELTE_CHAR* pResourceID, ELTE_CHAR** pUeGisCfg) const;

	//获取调度台订阅的终端列表
	ELTE_INT32 GetGisSubscription(const ELTE_CHAR* pResourceID, ELTE_CHAR** pGisQuerySubList) const;

private:
	CUserMgr*   m_pUserMgr;
};

#endif // __PROVISION_MGR_H__
