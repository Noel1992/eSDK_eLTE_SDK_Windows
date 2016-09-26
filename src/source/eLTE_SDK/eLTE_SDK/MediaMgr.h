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
	ELTE_INT32 TelephoneDial(const ELTE_CHAR* pTelNumber) const;

	//�Ҷϵ绰����
	ELTE_INT32 TelephoneHangup(const ELTE_CHAR* pTelNumber) const;

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
