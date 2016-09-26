/********************************************************************
filename		: 	MediaMgr.h
author			:	zWX229156
created		    :	2015/10/14
description	    :	ý�幦�ܹ����ࣨ��ʼ����ʵʱ��Ƶ����ʼ��̨�����Լ�ֹͣ����ʵʱ��Ƶ��
copyright		:	Copyright (C) 2015-2017
history			:	2015/10/14 ��ʼ�汾
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

	//��ʼ����ʵʱ��Ƶ
	ELTE_INT32 StartRealPlay(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam) const;

	// ��ʼ��̨����
	ELTE_INT32 PTZControl(const ELTE_CHAR* pResourceID, ELTE_UINT32 iPTZControlCode, ELTE_UINT32 iPTZControlValue) const;

	// ֹͣ����ʵʱ��Ƶ
	ELTE_INT32 StopRealPlay(const ELTE_CHAR* pResourceID) const;

private:
	CUserMgr*    m_pUserMgr;
};

#endif // __MEDIA_MGR_H__
