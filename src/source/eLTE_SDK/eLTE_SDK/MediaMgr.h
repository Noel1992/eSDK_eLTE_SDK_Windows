/********************************************************************
filename		: 	MediaMgr.h
author			:	zWX229156
created		    :	2015/1/12
description	    :	媒体功能管理类（获取录像文件列表、开始播放实时视频、开始云台控制以及停止播放实时视频）
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/12 初始版本
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

	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//获取录像文件列表
	ELTE_INT32 GetUserRECFileInfoList(const ELTE_CHAR* pQueryXml, ELTE_CHAR** pRspXml) const;

	//开始播放实时视频
	ELTE_INT32 StartRealPlay(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam) const;

	// 开始云台控制
	ELTE_INT32 PTZControl(const ELTE_CHAR* pResourceID, ELTE_UINT32 iPTZControlCode, ELTE_UINT32 iPTZControlValue) const;

	// 停止播放实时视频
	ELTE_INT32 StopRealPlay(const ELTE_CHAR* pResourceID) const;

	// 开始视频分发
	ELTE_INT32 StartVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam) const;

	// 停止视频分发
	ELTE_INT32 StopVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam) const;

	// 接受视频回传
	ELTE_INT32 RecvVideoPlay(const ELTE_CHAR* pResourceID) const;

	// 视频上墙
	ELTE_INT32 VWallStart(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam) const;

	//视频下墙
	ELTE_INT32 VWallStop(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVWallStopParam) const;

	//电话呼叫
	ELTE_INT32 TelephoneDial(const ELTE_CHAR* pTelNumber) const;

	//挂断电话呼叫
	ELTE_INT32 TelephoneHangup(const ELTE_CHAR* pTelNumber) const;

	//发起缜密侦听
	ELTE_INT32 StartDiscreetListen(const ELTE_CHAR* pResourceID) const;

	//停止缜密侦听
	ELTE_INT32 StopDiscreetListen(const ELTE_CHAR* pResourceID) const;

	//开始环境侦听
	ELTE_INT32 StartEnvironmentListen(const ELTE_CHAR* pResourceID) const;
	
	
private:
	CUserMgr*    m_pUserMgr;
};

#endif // __MEDIA_MGR_H__
