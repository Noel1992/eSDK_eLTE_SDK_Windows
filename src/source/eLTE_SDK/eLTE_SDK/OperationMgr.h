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

/********************************************************************
filename    :    OperationMgr.h
author      :    zWX229156
created     :    2015/01/12
description :    操作管理类（进行群组的相关操作）
copyright   :    Copyright (c) 2015-2017 Huawei Tech.Co.,Ltd
history     :    2015/01/12 初始版本
*********************************************************************/

#ifndef __OPERATION_MGR_H__
#define __OPERATION_MGR_H__

#include "eLTE_Types.h"

class CUserMgr;//lint !e763
class COperationMgr
{
public:
	COperationMgr();
	~COperationMgr();

public:
	//设置用户管理类
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//取消订阅群组
	ELTE_INT32 UnSubscribeGroup(const ELTE_CHAR* pGroupID) const;

	//订阅并自动加入群组
	ELTE_INT32 SubJoinGroup(const ELTE_CHAR* pGroupID) const;

	//创建动态群组
	ELTE_INT32 CreateDynamicGroup(const ELTE_CHAR* pDGNAParam) const;

	//删除动态群组
	ELTE_INT32 CancelDynamicGroup(const ELTE_CHAR* pResourceID) const;

	//获取临时组号
	ELTE_INT32 GetTempGroupID(ELTE_INT32* iGroupID) const;

	//创建临时组
	ELTE_INT32 CreateTempGroup(const ELTE_CHAR* pTEMPGParam) const;

	//发起语音点呼
	ELTE_INT32 P2PDial(const ELTE_CHAR* pResourceID) const;
	
	//发起半双工点呼
	ELTE_INT32 P2PHalfDpxDial(const ELTE_CHAR* pResourceID) const;

	//释放半双工点呼
	ELTE_INT32 P2PHalfDpxRelease(const ELTE_CHAR* pResourceID) const;

	//接收语音点呼
	ELTE_INT32 P2PRecv(const ELTE_CHAR* pResourceID) const;

	//拒接语音点呼
	ELTE_INT32 P2PReject(const ELTE_CHAR* pResourceID) const;

	//挂断语音点呼
	ELTE_INT32 P2PHangup(const ELTE_CHAR* pResourceID) const;

	//调度台强拆点呼
	ELTE_INT32 P2PBreakoff(const ELTE_CHAR* pResourceID) const;

	//抢话（强拆点呼+发起新点呼）
	ELTE_INT32 P2PBreakin(const ELTE_CHAR* pResourceID) const;

	//发起组呼（固定和临时组呼）或抢权
	ELTE_INT32 PTTDial(const ELTE_CHAR* pGroupID) const;

	//释放话权
	ELTE_INT32 PTTRelease(const ELTE_CHAR* pGroupID) const;

	//退出组呼
	ELTE_INT32 PTTHangup(const ELTE_CHAR* pGroupID) const;

	//发起紧急组呼
	ELTE_INT32 PTTEmergency(const ELTE_CHAR* pGroupID) const;

	//调度台强拆组呼
	ELTE_INT32 GroupBreakoff(const ELTE_CHAR* pGroupID) const;

	//调度台执行静音
	ELTE_INT32 VolMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pMuteParam) const;

	//调度台取消静音
	ELTE_INT32 VolUnMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pUnMuteParam) const;

	//订阅/去订阅GIS终端
	ELTE_INT32 GISSubscribe(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pGISParam) const;

	//发送短信/彩信
	ELTE_INT32 SDSSendMessage(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pSDSParam) const;

	//发起人工转接
	ELTE_INT32 P2PTransfer(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pP2PTransferParam) const;

	//创建派接组、增加派接组成员、删除派接组成员
	ELTE_INT32 OperatePatchGroup(const ELTE_CHAR* pPatchGroupParam, const ELTE_USHORT& uOperateType) const;

	//删除派接组
	ELTE_INT32 CancelPatchGroup(const ELTE_CHAR* pPatchGroupID) const;

	//设置终端的GIS配置信息
	ELTE_INT32 SetGisParam(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pUeGisCfg) const;

	//修改动态重组
	ELTE_INT32 ModifyDynamicGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pDynamicGroupInfo) const;

	//调度台组呼添加临时用户
	ELTE_INT32 TempUserJoinGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pPhonePatchParam) const;

private:
	CUserMgr*    m_pUserMgr;
};

#endif //__OPERATION_MGR_H__
