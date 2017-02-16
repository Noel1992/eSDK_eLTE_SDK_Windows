/*Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
eSDK is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
		http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

#include "stdafx.h"
#include "OperationMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "eLTE_Xml.h"
#include "eLTE_Tool.h"
#include "UserMgr.h"

COperationMgr::COperationMgr()
{
	m_pUserMgr = NULL;
}

COperationMgr::~COperationMgr()
{
	m_pUserMgr = NULL;
}

ELTE_VOID COperationMgr::SetUserMgr(CUserMgr* pUserMgr)
{
	LOG_TRACE();
	if(NULL == pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}
	m_pUserMgr = pUserMgr; 
}

ELTE_INT32 COperationMgr::UnSubscribeGroup(const ELTE_CHAR* pGroupID) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("GroupID");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pGroupID);
	(void)reqXml.AddElem("EnableSubscribeGroup");
	(void)reqXml.SetElemValue("0");

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_SUBSCRIBEGROUP_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::SubJoinGroup(const ELTE_CHAR* pGroupID) const
{
	LOG_TRACE();
	SET_GROUP_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_SUBJOINGROUP_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::CreateDynamicGroup(const ELTE_CHAR* pDGNAParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pDGNAParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pDGNAParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_CREATEDYNAMICGROUP_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::CancelDynamicGroup(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	SET_RESOURCE_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_CANCELDYNAMICGROUP_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::OperatePatchGroup(const ELTE_CHAR* pPatchGroupParam, const ELTE_USHORT& uOperateType) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pPatchGroupParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pPatchGroupParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(uOperateType, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::CancelPatchGroup(const ELTE_CHAR* pPatchGroupID) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("PatchGroupID");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pPatchGroupID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_CANCELPATCHGROUP_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::GetTempGroupID(ELTE_INT32* iGroupID) const
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
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETTEMPGROUPID_REQ, reqXml, FALSE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	iRet = m_pUserMgr->WaitObject(WAIT_OBJECT_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		CServerMgr& serverMgr = const_cast<CServerMgr&>(m_pUserMgr->GetServerMgr());
		if(!serverMgr.ServerIsRunning() || 0 != m_pUserMgr->GetServerStatus())
		{
			m_pUserMgr->SetServerStatus(0);
			return eLTE_SDK_ERR_SERVER_NOT_RUNNING;
		}
		return iRet;
	}
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();
	iRet = packetData.RspCode;
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		::ResetEvent(m_pUserMgr->GetEventHandle());
		return iRet;
	}
	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		ELTE_CHAR* pBuf = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == pBuf)
		{
			LOG_RUN_ERROR("New buf failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(pBuf, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(pBuf, uiDataLen + 1, packetData.Value, uiDataLen);
		CXml rspXml;
		if(!rspXml.Parse(pBuf))
		{
			LOG_RUN_ERROR("Parse rspXml failed, rspXml is %s.", pBuf);
			delete [] pBuf;
			pBuf = NULL;
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_XML_PARSE;//lint !e438
		}
		if(!rspXml.FindElemEx("Content/GroupID"))
		{
			LOG_RUN_ERROR("Find 'Content/GroupID' failed, rspXml is %s.", pBuf);
			delete [] pBuf;
			pBuf = NULL;
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_XML_FIND_ELEM;//lint !e438
		}
		std::string strGroupID = rspXml.GetElemValue();
		if (strGroupID.empty())
		{
			*iGroupID = 0;
		}
		else
		{
			*iGroupID = eLTE_Tool::String2Int(strGroupID);
		}
		delete [] pBuf;
		pBuf = NULL;
	}//lint !e438
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::CreateTempGroup(const ELTE_CHAR* pTEMPGParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pTEMPGParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pTEMPGParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_CREATETEMPGROUP_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::P2PDial(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	SET_RESOURCE_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_P2PDIAL_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::P2PHalfDpxRelease(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	SET_RESOURCE_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_P2PHALFDPXRELEASE_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::P2PHalfDpxDial(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	SET_RESOURCE_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_P2PHALFDPXDIAL_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::P2PRecv(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	SET_RESOURCE_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_P2PRECV_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::P2PReject(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	SET_RESOURCE_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_P2PREJECT_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::P2PHangup(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	SET_RESOURCE_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_P2PHANGUP_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::P2PBreakoff(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	SET_RESOURCE_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_P2PBREAKOFF_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::P2PBreakin(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	SET_RESOURCE_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_P2PBREAKIN_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::PTTDial(const ELTE_CHAR* pGroupID) const
{
	LOG_TRACE();
	SET_GROUP_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_PTTDIAL_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::PTTRelease(const ELTE_CHAR* pGroupID) const
{
	LOG_TRACE();
	SET_GROUP_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_PTTRELEASE_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::PTTHangup(const ELTE_CHAR* pGroupID) const
{
	LOG_TRACE();
	SET_GROUP_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_PTTHANGUP_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::PTTEmergency(const ELTE_CHAR* pGroupID) const
{
	LOG_TRACE();
	SET_GROUP_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_PTTEMERGENCY_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::GroupBreakoff(const ELTE_CHAR* pGroupID) const
{
	LOG_TRACE();
	SET_GROUP_XML_HEAD();

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GROUPBREAKOFF_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::VolMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pMuteParam) const
{
	PARSE_MUTE_XML();
	(void)reqXml.IntoElem();
	(void)reqXml.AddElemBeforeCurNode("ResourceID");
	(void)reqXml.SetElemValue(pResourceID);

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_VOLUMEMUTE_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::VolUnMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pMuteParam) const
{
	PARSE_MUTE_XML();
	(void)reqXml.IntoElem();
	(void)reqXml.AddElemBeforeCurNode("ResourceID");
	(void)reqXml.SetElemValue(pResourceID);

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_VOLUMEUNMUTE_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 COperationMgr::GISSubscribe(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pGISParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pGISParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pGISParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GISSUBSCRIBE_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

//设置终端的GIS配置信息
ELTE_INT32 COperationMgr::SetGisParam(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pUeGisCfg) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pUeGisCfg))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pUeGisCfg);
		return eLTE_SDK_ERR_XML_PARSE;
	}
	if(!reqXml.FindElem("Content"))
	{
		LOG_RUN_ERROR("Find 'Content' failed, reqXml is %s.", pUeGisCfg);
		return eLTE_SDK_ERR_XML_FIND_ELEM;
	}
	(void)reqXml.IntoElem();
	if(!reqXml.FindElem("UeGisCfgInfo"))
	{
		LOG_RUN_ERROR("Find 'UeGisCfgInfo' failed, reqXml is %s.", pUeGisCfg);
		return eLTE_SDK_ERR_XML_FIND_ELEM;
	}
	(void)reqXml.IntoElem();
	(void)reqXml.AddElemBeforeCurNode("ResourceID");
	(void)reqXml.SetElemValue(pResourceID);

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_SETUSERSPECIFICGISCFG_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

//修改动态重组
ELTE_INT32 COperationMgr::ModifyDynamicGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pDynamicGroupInfo) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pDynamicGroupInfo))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pDynamicGroupInfo);
		return eLTE_SDK_ERR_XML_PARSE;
	}
	if(!reqXml.FindElem("Content"))
	{
		LOG_RUN_ERROR("Find 'Content' failed, reqXml is %s.", pDynamicGroupInfo);
		return eLTE_SDK_ERR_XML_FIND_ELEM;
	}
	(void)reqXml.IntoElem();
	if(!reqXml.FindElem("DynamicGroupInfo"))
	{
		LOG_RUN_ERROR("Find 'DynamicGroupInfo' failed, reqXml is %s.", pDynamicGroupInfo);
		return eLTE_SDK_ERR_XML_FIND_ELEM;
	}
	(void)reqXml.IntoElem();
	(void)reqXml.AddElemBeforeCurNode("ResourceID");
	(void)reqXml.SetElemValue(pResourceID);

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_MODIFYDYNAMICGROUP_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

// ELTE_INT32 COperationMgr::TempUserJoinGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pPhonePatchParam) const
// {
// 	LOG_TRACE();
// 	if(NULL == m_pUserMgr)
// 	{
// 		LOG_RUN_ERROR("UserMgr is null.");
// 		return eLTE_SDK_ERR_NULL_POINTER;
// 	}
// 	CXml reqXml;
// 	if(!reqXml.Parse(pPhonePatchParam))
// 	{
// 		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pPhonePatchParam);
// 		return eLTE_SDK_ERR_XML_PARSE;
// 	}
// 	if(!reqXml.FindElem("Content"))
// 	{
// 		LOG_RUN_ERROR("Find 'Content' failed, reqXml is %s.", pPhonePatchParam);
// 		return eLTE_SDK_ERR_XML_FIND_ELEM;
// 	}
// 	(void)reqXml.IntoElem();
// 	if(!reqXml.FindElem("PhonePatchParam"))
// 	{
// 		LOG_RUN_ERROR("Find 'PhonePatchParam' failed, reqXml is %s.", pPhonePatchParam);
// 		return eLTE_SDK_ERR_XML_FIND_ELEM;
// 	}
// 	(void)reqXml.IntoElem();
// 	(void)reqXml.AddElemBeforeCurNode("ResourceID");
// 	(void)reqXml.SetElemValue(pResourceID);
// 
// 	ELTE_UINT32 xmlLen = 0;
// 	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));
// 
// 	//发送消息
// 	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
// 	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
// 	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_TEMPUSERJOINGROUP_REQ, reqXml, TRUE);
// 	if(eLTE_SDK_ERR_SUCCESS != iRet) 
// 	{
// 		return iRet;
// 	}
// 	WAIT_SERVER_RSP();
// 
// 	iRet = m_pUserMgr->GetPacketData().RspCode;
// 	::ResetEvent(m_pUserMgr->GetEventHandle());
// 	return iRet;
// }

ELTE_INT32 COperationMgr::SDSSendMessage(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pSDSParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	//特殊字符转换
	std::string xmlStr = pSDSParam;
	std::string xmlDst;
	std::string::size_type iStart = xmlStr.find("<MsgBody>");
	if(std::string::npos != iStart)
	{
		std::string::size_type iEnd = xmlStr.find("</MsgBody>", iStart);
		if(std::string::npos != iEnd)
		{
			std::string strTemp;
			iStart += strlen("<MsgBody>");
			eLTE_Tool::ReplaceString(xmlStr.substr(iStart, iEnd - iStart), strTemp, "&", "&amp;");
			eLTE_Tool::ReplaceString(strTemp, strTemp, "<", "&lt;");
			xmlDst = xmlStr.substr(0, iStart) + strTemp + xmlStr.substr(iEnd, xmlStr.size() - iEnd);
		}
	}

	CXml reqXml;
	if(!reqXml.Parse(xmlDst.c_str()))
	{
		LOG_RUN_ERROR("ReqXml parse failed.");
		return eLTE_SDK_ERR_XML_PARSE;
	}

	if(!reqXml.FindElem("Content"))
	{
		LOG_RUN_ERROR("Find 'Content' failed.");
		return eLTE_SDK_ERR_XML_FIND_ELEM;
	}
	(void)reqXml.IntoElem();
	if(!reqXml.FindElem("SDSType"))
	{
		LOG_RUN_ERROR("Find 'SDSType' failed.");
		return eLTE_SDK_ERR_XML_FIND_ELEM;
	}
	(void)reqXml.AddElemBeforeCurNode("ResourceID");
	(void)reqXml.SetElemValue(pResourceID);
	if(!reqXml.FindElem("Receiver"))
	{
		LOG_RUN_ERROR("Find 'Receiver' failed.");
		return eLTE_SDK_ERR_XML_FIND_ELEM;
	}

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_SDSSEND_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}


ELTE_INT32 COperationMgr::P2PTransfer(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pP2PTransferParam)const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pP2PTransferParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pP2PTransferParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}
	
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_P2PTRANSFER_REQ, reqXml, TRUE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	WAIT_SERVER_RSP();

	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());  
	return iRet;
}
