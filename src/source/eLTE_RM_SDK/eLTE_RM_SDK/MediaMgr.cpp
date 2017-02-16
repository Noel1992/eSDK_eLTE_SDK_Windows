/*
Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
	   eSDK is licensed under the Apache License, Version 2.0 (the "License");
	   you may not use this file except in compliance with the License.
	   You may obtain a copy of the License at
	
	       http://www.apache.org/licenses/LICENSE-2.0

	
	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.

*/
#include "stdafx.h"
#include "MediaMgr.h"
//log manage class 
#include "eLTE_Log.h"
//Error Code define class
#include "eLTE_Error.h"
#include "eLTE_Xml.h"
//user manage class 
#include "UserMgr.h"
//public function interface class 
#include "eLTE_Tool.h"
//Json definition class 
#include "json.h"
#include "XMLProcess.h"

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

ELTE_INT32 CMediaMgr::StartRealPlay(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam) const
{
	LOG_TRACE();
	if(NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_NULL_POINTER;
	}

	VIDEO_PARAM videoParam;
	ELTE_INT32 iRet=XMLProcess::GetXml_StartRealPlay_Req(pVideoParam, videoParam);
	if(eLTE_SDK_ERR_SUCCESS!=iRet)
	{
		LOG_RUN_ERROR("GetXml_StartRealPlay_Req Failed");
		return iRet;
	}

	//播放参数
	Json::Value paramNode;
	paramNode["fmt"] = videoParam.VideoFormat;
	paramNode["camera"] = videoParam.CameraType;
	paramNode["mute"] = videoParam.MuteType;
	paramNode["confirm"] =videoParam.UserConfirmType;

	Json::Value value;
	Json::FastWriter writer;

	value["cmd"] = "000007";
	value["opt"] = "monitor";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_STARTREALPLAY);
	value["isdn"] = pResourceID;
	value["param"] = paramNode;
	value["session"] = m_pUserMgr->GetSessionId();
	std::string ptzReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	iRet = socket.SendMsg(ptzReqStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("StartRealPlay req failed.");
		return iRet;
	}

	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_PTZCONTROL));
	iRet = m_pUserMgr->WaitObject(WAIT_OBJECT_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetCheckStr("");
		return iRet;
	}

	const std::string& strPacketData = m_pUserMgr->GetPacketData();
	::ResetEvent(m_pUserMgr->GetEventHandle());
	iRet = eLTE_Tool::String2Int(strPacketData.c_str());
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
	//登出请求报文
	Json::Value value;
	Json::FastWriter writer;

	value["cmd"] = "000009";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_PTZCONTROL);
	value["opt"] = "ptz";
	value["isdn"] = pResourceID;
	value["act"] = eLTE_Tool::UInt2String(iPTZControlCode);
	value["value"] = eLTE_Tool::UInt2String(iPTZControlValue);
	value["session"] = m_pUserMgr->GetSessionId();
	std::string ptzReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ptzReqStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("Send get ptz req failed.");
		return iRet;
	}
	
	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_PTZCONTROL));
	iRet = m_pUserMgr->WaitObject(WAIT_OBJECT_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetCheckStr("");
		return iRet;
	}

	const std::string& strPacketData = m_pUserMgr->GetPacketData();
	::ResetEvent(m_pUserMgr->GetEventHandle());
	iRet = eLTE_Tool::String2Int(strPacketData.c_str());
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

	Json::Value value;
	Json::FastWriter writer;

	value["cmd"] = "000007";
	value["opt"] = "close";
	value["seq"] = eLTE_Tool::UInt2String(SEQ_STOPREALPLAY);
	value["isdn"] = pResourceID;
	value["session"] = m_pUserMgr->GetSessionId();
	std::string ptzReqStr = writer.write(value);

	//发送消息
	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	MutexLocker Locker(m_pUserMgr->GetMutexHandle());
	ELTE_INT32 iRet = socket.SendMsg(ptzReqStr);
	if(eLTE_SDK_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("StopRealPlay req failed.");
		return iRet;
	}

	m_pUserMgr->SetCheckStr(eLTE_Tool::UInt2String(SEQ_STOPREALPLAY));
	iRet = m_pUserMgr->WaitObject(WAIT_OBJECT_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		m_pUserMgr->SetCheckStr("");
		return iRet;
	}
	const std::string& strPacketData = m_pUserMgr->GetPacketData();
	::ResetEvent(m_pUserMgr->GetEventHandle());
	iRet = eLTE_Tool::String2Int(strPacketData.c_str());
	return iRet;
}
