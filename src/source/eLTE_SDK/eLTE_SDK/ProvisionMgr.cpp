#include "stdafx.h"
#include "ProvisionMgr.h"
#include "eLTE_Error.h"
#include "eLTE_Log.h"
#include "eLTE_Xml.h"
#include "UserMgr.h"

CProvisionMgr::CProvisionMgr()
{
	m_pUserMgr = NULL;
}

CProvisionMgr::~CProvisionMgr()
{
	m_pUserMgr = NULL;
}

ELTE_INT32 CProvisionMgr::ProvisionManagerInit(const ELTE_CHAR* pServerIP,const ELTE_CHAR* pUserID) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("ServerIP");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pServerIP);
	(void)reqXml.AddElem("UserID");
	(void)reqXml.SetElemValue(pUserID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_PROVISIONMANAGERINIT_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::ProvisionManagerInitMRS(const ELTE_CHAR* pServerIP) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("ServerIP");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pServerIP);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_PROVISIONMANAGERINITMRS_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::ProvisionManagerExit() const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	//发送消息
	CXml reqXml;
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_PROVISIONMANAGEREXIT_REQ, reqXml, FALSE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_VOID CProvisionMgr::SetUserMgr(CUserMgr* pUserMgr)
{
	LOG_TRACE();
	if(NULL == pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}
	m_pUserMgr = pUserMgr; 
}

ELTE_INT32 CProvisionMgr::GetDcGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcGroups) const
{
	LOG_TRACE();

	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CONSTRUCT_XML_HEAD("UserID",pUserID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETDCGROUPS_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	
	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pDcGroups = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pDcGroups)
		{
			LOG_RUN_ERROR("New DcGroups failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pDcGroups, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pDcGroups, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pDcGroups = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetPatchGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pPatchGroups) const
{
	LOG_TRACE();

	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CONSTRUCT_XML_HEAD("UserID",pUserID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETPATCHGROUPS_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}

	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pPatchGroups = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pPatchGroups)
		{
			LOG_RUN_ERROR("New PatchGroups failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pPatchGroups, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pPatchGroups, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pPatchGroups = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetPatchGroupInfo(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupInfo) const
{
	LOG_TRACE();

	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CONSTRUCT_XML_HEAD("GroupID",pPatchGroupId);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETPATCHGROUPINFO_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}

	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pPatchGroupInfo = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pPatchGroupInfo)
		{
			LOG_RUN_ERROR("New PatchGroupInfo failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pPatchGroupInfo, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pPatchGroupInfo, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pPatchGroupInfo = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetGroupMemberByPatchId(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupMembers) const
{
	LOG_TRACE();

	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CONSTRUCT_XML_HEAD("GroupID",pPatchGroupId);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETPATCHGROUPMEMBERS_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}

	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pPatchGroupMembers = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pPatchGroupMembers)
		{
			LOG_RUN_ERROR("New PatchGroupMembers failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pPatchGroupMembers, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pPatchGroupMembers, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pPatchGroupMembers = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetDcUsers(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcUsers) const
{
	LOG_TRACE();

	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CONSTRUCT_XML_HEAD("UserID",pUserID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETDCUSERS_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	
	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pDcUsers = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pDcUsers)
		{
			LOG_RUN_ERROR("New DcUsers failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pDcUsers, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pDcUsers, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pDcUsers = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetGroupUsers(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupUsers) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CONSTRUCT_XML_HEAD("GroupID",pGroupID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETGROUPUSERS_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	
	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pGroupUsers = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pGroupUsers)
		{
			LOG_RUN_ERROR("New GroupUsers failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pGroupUsers, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pGroupUsers, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pGroupUsers = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetGroupInfo(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupInfo) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CONSTRUCT_XML_HEAD("GroupID",pGroupID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETGROUPINFO_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	
	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pGroupInfo = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pGroupInfo)
		{
			LOG_RUN_ERROR("New GroupInfo failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pGroupInfo, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pGroupInfo, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pGroupInfo = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetUserInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pUserInfo) const
{
	LOG_TRACE();

	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CONSTRUCT_XML_HEAD("UserID",pUserID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETUSERINFO_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	
	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pUserInfo = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pUserInfo)
		{
			LOG_RUN_ERROR("New UserInfo failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pUserInfo, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pUserInfo, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pUserInfo = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetDcInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcInfo) const
{
	LOG_TRACE();

	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CONSTRUCT_XML_HEAD("UserID",pUserID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETDCINFO_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	
	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pDcInfo = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pDcInfo)
		{
			LOG_RUN_ERROR("New DcInfo failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pDcInfo, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pDcInfo, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pDcInfo = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}


ELTE_INT32 CProvisionMgr::GetDcVWallIDList(ELTE_CHAR** pVWallIDList) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	
	CXml reqXml;

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	//无内容，sendMsg传入参数改为FALSE
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETVWALLLIST_REQ, reqXml, FALSE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}

	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pVWallIDList = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pVWallIDList)
		{
			LOG_RUN_ERROR("New DcUsers failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pVWallIDList, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pVWallIDList, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pVWallIDList = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetGisSubscription(const ELTE_CHAR* pResourceID, ELTE_CHAR** pGisQuerySubList) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CONSTRUCT_XML_HEAD("ResourceID",pResourceID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETGISSUBSCRIPTION_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}

	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pGisQuerySubList = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pGisQuerySubList)
		{
			LOG_RUN_ERROR("New GisQuerySubList failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pGisQuerySubList, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pGisQuerySubList, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pGisQuerySubList = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CProvisionMgr::GetUserSpecificGISCfg(const ELTE_CHAR* pResourceID, ELTE_CHAR** pUeGisCfg) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CONSTRUCT_XML_HEAD("ResourceID",pResourceID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETUSERSPECIFICGISCFG_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}

	WAIT_SERVER_RSP();
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;

	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pUeGisCfg = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pUeGisCfg)
		{
			LOG_RUN_ERROR("New UeGisCfg failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pUeGisCfg, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pUeGisCfg, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pUeGisCfg = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

