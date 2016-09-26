#include "stdafx.h"
#include "MediaMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "eLTE_Xml.h"
#include "UserMgr.h"
#include "eLTE_Tool.h"
#include "SharedMemoryMgr.h"

CMediaMgr::CMediaMgr()
{
	m_pUserMgr = NULL;
}

CMediaMgr::~CMediaMgr()
{
	m_pUserMgr = NULL;
}

ELTE_VOID CMediaMgr::SetUserMgr(CUserMgr* pUserMgr)
{
	LOG_TRACE();
	if(NULL == pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}
	m_pUserMgr = pUserMgr; 
}

ELTE_INT32 CMediaMgr::GetUserRECFileInfoList(const ELTE_CHAR* pQueryXml, ELTE_CHAR** pRspXml) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pQueryXml))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pQueryXml);
		return eLTE_SDK_ERR_XML_PARSE;
	}

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_GETUSERRECFILEINFOLIST_REQ, reqXml, TRUE);
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
	ELTE_UINT32 uiDataLen = packetData.PacketLength - PACKET_HEAD_SIZE;
	if(uiDataLen > 0)
	{
		*pRspXml = new ELTE_CHAR[uiDataLen + 1];
		if(NULL == *pRspXml)
		{
			LOG_RUN_ERROR("New RspXml failed.");
			::ResetEvent(m_pUserMgr->GetEventHandle());
			return eLTE_SDK_ERR_CREATE_OBJECT;
		}
		eSDK_MEMSET(*pRspXml, 0x00, uiDataLen + 1);
		eSDK_MEMCPY(*pRspXml, uiDataLen + 1, packetData.Value, uiDataLen);
	}
	else
	{
		*pRspXml = NULL;
	}
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CMediaMgr::StartRealPlay(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pVideoParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pVideoParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}
	if(!reqXml.FindElem("Content"))
	{
		LOG_RUN_ERROR("Find 'Content' failed, reqXml is %s.", pVideoParam);
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
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_STARTREALPLAY_REQ, reqXml, TRUE);
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
	::ResetEvent(m_pUserMgr->GetEventHandle());
	if(eLTE_SDK_ERR_SUCCESS == iRet && !CUserMgr::m_iBypass)
	{
		iRet = SharedMemoryMgr::Instance().CreateSharedMemory(pResourceID);
		if(eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("CreateSharedMemory failed.");
		}
	}
	return iRet;
}

ELTE_INT32 CMediaMgr::RecvVideoPlay(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("ResourceID");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pResourceID);

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_RECVVIDEOPLAY_REQ, reqXml, TRUE);
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
	::ResetEvent(m_pUserMgr->GetEventHandle());
	if(eLTE_SDK_ERR_SUCCESS == iRet && !CUserMgr::m_iBypass)
	{
		iRet = SharedMemoryMgr::Instance().CreateSharedMemory(pResourceID);
		if(eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("CreateSharedMemory failed.");
		}
	}
	return iRet;
}

ELTE_INT32 CMediaMgr::PTZControl(const ELTE_CHAR* pResourceID, ELTE_UINT32 iPTZControlCode, ELTE_UINT32 iPTZControlValue) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("ResourceID");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pResourceID);
	(void)reqXml.AddElem("PTZControlCode");
	(void)reqXml.SetElemValue(eLTE_Tool::UInt2String(iPTZControlCode).c_str());
	(void)reqXml.AddElem("PTZControlValue");
	(void)reqXml.SetElemValue(eLTE_Tool::UInt2String(iPTZControlValue).c_str());

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_PTZCONTROL_REQ, reqXml, TRUE);
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
	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CMediaMgr::StopRealPlay(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("ResourceID");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pResourceID);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_STOPREALPLAY_REQ, reqXml, TRUE);
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
	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	if(eLTE_SDK_ERR_SUCCESS == iRet && !CUserMgr::m_iBypass)
	{
		SharedMemoryMgr::Instance().DeleteSharedMemory(pResourceID);
	}
	return iRet;
}

ELTE_INT32 CMediaMgr::StartVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pVideoDispatchParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pVideoDispatchParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}
	if(!reqXml.FindElem("Content"))
	{
		LOG_RUN_ERROR("Find 'Content' failed, reqXml is %s.", pVideoDispatchParam);
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
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_STARTDISPATCHVIDEO_REQ, reqXml, TRUE);
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
	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CMediaMgr::StopVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pVideoDispatchParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pVideoDispatchParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}
	if(!reqXml.FindElem("Content"))
	{
		LOG_RUN_ERROR("Find 'Content' failed, reqXml is %s.", pVideoDispatchParam);
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
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_STOPDISPATCHVIDEO_REQ, reqXml, TRUE);
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
	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

//视频上墙
ELTE_INT32 CMediaMgr::VWallStart(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CXml reqXml;
	if(!reqXml.Parse(pVideoParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pVideoParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}

	if(!reqXml.FindElem("Content"))
	{
		LOG_RUN_ERROR("Find 'Content' failed, reqXml is %s.", pVideoParam);
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
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_VWALLSTART_REQ, reqXml, TRUE);
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
	::ResetEvent(m_pUserMgr->GetEventHandle());

	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("GetPacketData failed.");
	}
	return iRet;
}

ELTE_INT32 CMediaMgr::VWallStop(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVWallStopParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}
	CXml reqXml;
	if(!reqXml.Parse(pVWallStopParam))
	{
		LOG_RUN_ERROR("ReqXml parse failed, param is %s.", pVWallStopParam);
		return eLTE_SDK_ERR_XML_PARSE;
	}
	if(!reqXml.FindElem("Content"))
	{
		LOG_RUN_ERROR("Find 'Content' failed, reqXml is %s.", pVWallStopParam);
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
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_VWALLSTOP_REQ, reqXml, TRUE);
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
	iRet = m_pUserMgr->GetPacketData().RspCode;
	::ResetEvent(m_pUserMgr->GetEventHandle());
	return iRet;
}

ELTE_INT32 CMediaMgr::TelephoneDial(const ELTE_CHAR* pTelNumber) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	//构造xml
	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("TelNumber");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pTelNumber);

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_TELEPHONEDIAL_REQ, reqXml, TRUE);
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
	::ResetEvent(m_pUserMgr->GetEventHandle());

	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("GetPacketData failed.");
	}
	return iRet;
}

ELTE_INT32 CMediaMgr::TelephoneHangup(const ELTE_CHAR* pTelNumber) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("TelNumber");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pTelNumber);

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_TELEPHONEHANGUP_REQ, reqXml, TRUE);
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
	::ResetEvent(m_pUserMgr->GetEventHandle());

	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("GetPacketData failed.");
	}
	return iRet;
}

ELTE_INT32 CMediaMgr::StartDiscreetListen(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("ResourceID");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pResourceID);
	
	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_STARTDISCREETLISTEN_REQ, reqXml, TRUE);
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
	::ResetEvent(m_pUserMgr->GetEventHandle());

	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("GetPacketData failed.");
	}
	return iRet;
}

ELTE_INT32 CMediaMgr::StopDiscreetListen(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("ResourceID");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pResourceID);

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_STOPDISCREETLISTEN_REQ, reqXml, TRUE);
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
	::ResetEvent(m_pUserMgr->GetEventHandle());

	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("GetPacketData failed.");
	}
	return iRet;
}

ELTE_INT32 CMediaMgr::StartEnvironmentListen(const ELTE_CHAR* pResourceID) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	CXml reqXml;
	(void)reqXml.AddElem("Content");
	(void)reqXml.AddChildElem("ResourceID");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pResourceID);

	ELTE_UINT32 xmlLen = 0;
	LOG_RUN_INFO("ReqXml is %s.", reqXml.GetXMLStream(xmlLen));

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_STARTENVIRLISTEN_REQ, reqXml, TRUE);
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
	::ResetEvent(m_pUserMgr->GetEventHandle());

	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("GetPacketData failed.");
	}
	return iRet;
}

