/********************************************************************
filename		: 	MediaMgr.h
author			:	zWX229156
created		    :	2015/1/12
description	    :	ý�幦�ܹ����ࣨ��ȡ¼���ļ��б���ʼ����ʵʱ��Ƶ����ʼ��̨�����Լ�ֹͣ����ʵʱ��Ƶ��
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/12 ��ʼ�汾
*********************************************************************/

#ifndef __MEDIA_MGR_H__
#define __MEDIA_MGR_H__

#include "eLTE_Types.h"

#define GET_PACKAGE_DATA_MEMORY()	\
	WAIT_SERVER_RSP();	\
	const PACKET_DATA& packetData = m_pUserMgr->GetPacketData();	\
	iRet = packetData.RspCode;	\
	::ResetEvent(m_pUserMgr->GetEventHandle());	\
	if(eLTE_SDK_ERR_SUCCESS == iRet && !CUserMgr::m_iBypass)	\
	{	\
		iRet = SharedMemoryMgr::Instance().CreateSharedMemory(pResourceID);	\
		if(eLTE_SDK_ERR_SUCCESS != iRet)	\
		{	\
			LOG_RUN_ERROR("CreateSharedMemory failed.");	\
		}	\
	}	\

#define CONSTRUCT_XML(nodeName,nodeValue) \
	CXml reqXml;	\
	(void)reqXml.AddElem("Content");	\
	(void)reqXml.AddChildElem(nodeName);	\
	(void)reqXml.IntoElem();	\
	(void)reqXml.SetElemValue(nodeValue);	\

#define WAIT_SERVER_RSP() \
	iRet = m_pUserMgr->WaitObject(WAIT_OBJECT_TIME);								\
	if (eLTE_SDK_ERR_SUCCESS != iRet)												\
	{																				\
		CServerMgr& serverMgr = const_cast<CServerMgr&>(m_pUserMgr->GetServerMgr());\
		if(!serverMgr.ServerIsRunning() || 0 != m_pUserMgr->GetServerStatus())		\
		{																			\
			m_pUserMgr->SetServerStatus(0);											\
			return eLTE_SDK_ERR_SERVER_NOT_RUNNING;									\
		}																			\
		return iRet;																\
	}																				\

#define SET_MEDIA_XML(param) \
	if(NULL == m_pUserMgr)	\
	{	\
		LOG_RUN_ERROR("UserMgr is null.");	\
		return eLTE_SDK_ERR_NULL_POINTER;	\
	}	\
	CXml reqXml;	\
	if(!reqXml.Parse(param))	\
	{	\
		LOG_RUN_ERROR("ReqXml parse failed, parameter is %s.", param);	\
		return eLTE_SDK_ERR_XML_PARSE;	\
	}	\
	if(!reqXml.FindElem("Content"))	\
	{	\
		LOG_RUN_ERROR("Find 'Content' failed, reqXml is %s.", param);	\
		return eLTE_SDK_ERR_XML_FIND_ELEM;	\
	}	\
	(void)reqXml.IntoElem();	\
	(void)reqXml.AddElemBeforeCurNode("ResourceID");	\
	(void)reqXml.SetElemValue(pResourceID);	\

class CUserMgr;//lint !e763
class CMediaMgr
{
public:
	CMediaMgr();
	~CMediaMgr();

	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//��ȡ¼���ļ��б�
	ELTE_INT32 GetUserRECFileInfoList(const ELTE_CHAR* pQueryXml, ELTE_CHAR** pRspXml) const;

	//��ʼ����ʵʱ��Ƶ
	ELTE_INT32 StartRealPlay(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam) const;

	// ��ʼ��̨����
	ELTE_INT32 PTZControl(const ELTE_CHAR* pResourceID, ELTE_UINT32 iPTZControlCode, ELTE_UINT32 iPTZControlValue) const;

	// ֹͣ����ʵʱ��Ƶ
	ELTE_INT32 StopRealPlay(const ELTE_CHAR* pResourceID) const;

	// ��ʼ��Ƶ�ַ�
	ELTE_INT32 StartVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam) const;

	// ֹͣ��Ƶ�ַ�
	ELTE_INT32 StopVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam) const;

	// ������Ƶ�ش�
	ELTE_INT32 RecvVideoPlay(const ELTE_CHAR* pResourceID) const;

	// ��Ƶ��ǽ
	ELTE_INT32 VWallStart(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam) const;

	//��Ƶ��ǽ
	ELTE_INT32 VWallStop(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVWallStopParam) const;

	//�绰����
//	ELTE_INT32 TelephoneDial(const ELTE_CHAR* pTelNumber) const;

	//�Ҷϵ绰����
//	ELTE_INT32 TelephoneHangup(const ELTE_CHAR* pTelNumber) const;

	//������������
	ELTE_INT32 StartDiscreetListen(const ELTE_CHAR* pResourceID) const;

	//ֹͣ��������
	ELTE_INT32 StopDiscreetListen(const ELTE_CHAR* pResourceID) const;

	//��ʼ��������
	ELTE_INT32 StartEnvironmentListen(const ELTE_CHAR* pResourceID) const;
	
	
private:
	CUserMgr*    m_pUserMgr;
};

#endif // __MEDIA_MGR_H__
