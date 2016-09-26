#include "stdafx.h"
#include "ProvisionMgr.h"
//Error Code define class
#include "eLTE_Error.h"
//log manage class 
#include "eLTE_Log.h"
//#include "eLTE_Xml.h"
//user manage class 
#include "UserMgr.h"
//Json definition class 
#include "json.h"
//public function interface class 
#include "eLTE_Tool.h"

#define WAIT_CHECK_TIME 30000

CProvisionMgr::CProvisionMgr()
{
	m_pUserMgr = NULL;
}

CProvisionMgr::~CProvisionMgr()
{
	m_pUserMgr = NULL;
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
	//登出请求报文
	Json::Value value,para;
	Json::FastWriter writer;
	para["mot"] = "0";
	para["dcid"] = pUserID;
	para["vpnid"] = "-1";
	para["cat"] = "-1";

	value["cmd"] = "010006";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_GETDCGROUPS);
	value["session"] = m_pUserMgr->GetSessionId();
	value["param"] = para;
	std::string dcGroupsReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(dcGroupsReqStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Send get dc groups req failed.");
		return iRet;
	}
	
	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_GETDCGROUPS));
	LOG_RUN_INFO("CProvisionMgr::GetDcGroups : wait single!");
	iRet = m_pUserMgr->WaitObject(WAIT_CHECK_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetCheckStr("");
		return iRet;
	}
	const std::string& strPacketData = m_pUserMgr->GetPacketData();
	ELTE_UINT32 uiDataLen = strPacketData.length();
	if(uiDataLen > 0)
	{
		*pDcGroups = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pDcGroups)
		{
			LOG_RUN_ERROR("New DcGroups failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		memset(*pDcGroups, 0x00, uiDataLen + 1);
		memcpy(*pDcGroups, strPacketData.c_str(), uiDataLen);
	}
	else
	{
		*pDcGroups = NULL;
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

	//登出请求报文
	Json::Value value,para;
	Json::FastWriter writer;
	para["mot"] = "1";
	para["dcid"] = pUserID;
	para["vpnid"] = "-1";
	para["cat"] = "-1";

	value["cmd"] = "010006";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_GETDCUSERS);
	value["session"] = m_pUserMgr->GetSessionId();
	value["param"] = para;
	std::string dcUsersReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(dcUsersReqStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Send get dc users req failed.");
		return iRet;
	}

	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_GETDCUSERS));
	LOG_RUN_INFO("CProvisionMgr::GetDcUsers : wait single!");
	::ResetEvent(m_pUserMgr->GetEventHandle());
	iRet = m_pUserMgr->WaitObject(WAIT_CHECK_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetCheckStr("");
		return iRet;
	}
	const std::string& strPacketData = m_pUserMgr->GetPacketData();
	ELTE_UINT32 uiDataLen = strPacketData.length();
	if(uiDataLen > 0)
	{
		*pDcUsers = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pDcUsers)
		{
			LOG_RUN_ERROR("New DcUsers failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		memset(*pDcUsers, 0x00, uiDataLen + 1);
		memcpy(*pDcUsers, strPacketData.c_str(), uiDataLen);
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

	//登出请求报文
	Json::Value value,para;
	Json::FastWriter writer;
	para["mot"] = "8";
	para["dcid"] = m_pUserMgr->GetUserID();
	para["vpnid"] = "-1";
	para["cat"] = pGroupID;

	value["cmd"] = "010006";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_GETGROUPUSERS);
	value["session"] = m_pUserMgr->GetSessionId();
	value["param"] = para;
	std::string groupUsersReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(groupUsersReqStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Send get dc users req failed.");
		return iRet;
	}

	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_GETGROUPUSERS));
	iRet = m_pUserMgr->WaitObject(WAIT_CHECK_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetCheckStr("");
		return iRet;
	}
	const std::string& strPacketData = m_pUserMgr->GetPacketData();
	ELTE_UINT32 uiDataLen = strPacketData.length();
	if(uiDataLen > 0)
	{
		*pGroupUsers = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pGroupUsers)
		{
			LOG_RUN_ERROR("New GroupUsers failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		memset(*pGroupUsers, 0x00, uiDataLen + 1);
		memcpy(*pGroupUsers, strPacketData.c_str(), uiDataLen);
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

	//登出请求报文
	Json::Value value,para;
	Json::FastWriter writer;
	para["mot"] = "0";
	para["id"] = pGroupID;
	para["id2"] = "-1";

	value["cmd"] = "010016";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_GETGROUPINFO);
	value["session"] = m_pUserMgr->GetSessionId();
	value["param"] = para;
	std::string groupInfoReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	//异步锁
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(groupInfoReqStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Send get group info req failed.");
		return iRet;
	}

	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_GETGROUPINFO));
	iRet = m_pUserMgr->WaitObject(WAIT_CHECK_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetCheckStr("");
		return iRet;
	}
	const std::string& strPacketData = m_pUserMgr->GetPacketData();
	ELTE_UINT32 uiDataLen = strPacketData.length();
	if(uiDataLen > 0)
	{
		*pGroupInfo = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pGroupInfo)
		{
			LOG_RUN_ERROR("New GroupInfo failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		memset(*pGroupInfo, 0x00, uiDataLen + 1);
		memcpy(*pGroupInfo, strPacketData.c_str(), uiDataLen);
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

	//登出请求报文
	Json::Value value,para;
	Json::FastWriter writer;
	para["mot"] = "1";
	para["id"] = pUserID;
	para["id2"] = "-1";

	value["cmd"] = "010016";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_GETUSERINFO);
	value["session"] = m_pUserMgr->GetSessionId();
	value["param"] = para;
	std::string userInfoReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(userInfoReqStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Send get user info req failed.");
		return iRet;
	}

	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_GETUSERINFO));
	iRet = m_pUserMgr->WaitObject(WAIT_CHECK_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetCheckStr("");
		return iRet;
	}
	const std::string& strPacketData = m_pUserMgr->GetPacketData();
	ELTE_UINT32 uiDataLen = strPacketData.length();
	if(uiDataLen > 0)
	{
		*pUserInfo = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pUserInfo)
		{
			LOG_RUN_ERROR("New UserInfo failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		memset(*pUserInfo, 0x00, uiDataLen + 1);
		memcpy(*pUserInfo, strPacketData.c_str(), uiDataLen);
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

	//登出请求报文
	Json::Value value,para;
	Json::FastWriter writer;
	para["mot"] = "4";
	para["id"] = pUserID;
	para["id2"] = "-1";

	value["cmd"] = "010016";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_GETDCINFO);
	value["session"] = m_pUserMgr->GetSessionId();
	value["param"] = para;
	std::string dcInfoReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(dcInfoReqStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Send get dc info req failed.");
		return iRet;
	}

	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_GETDCINFO));
	iRet = m_pUserMgr->WaitObject(WAIT_CHECK_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetCheckStr("");
		return iRet;
	}
	const std::string& strPacketData = m_pUserMgr->GetPacketData();
	ELTE_UINT32 uiDataLen = strPacketData.length();
	if(uiDataLen > 0)
	{
		*pDcInfo = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pDcInfo)
		{
			LOG_RUN_ERROR("New UserInfo failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		memset(*pDcInfo, 0x00, uiDataLen + 1);
		memcpy(*pDcInfo, strPacketData.c_str(), uiDataLen);
	}
	else
	{
		*pDcInfo = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}
