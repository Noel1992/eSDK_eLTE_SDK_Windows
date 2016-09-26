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
author      :    yubinbin wx200189
created     :    2014/07/01
description :    调度台操作管理类。包括注册、注销、视频回传业务逻辑处理。对应eLTE SDK内的OperationManager类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_OPERATION_MGR_H__
#define __ELTE_OCX_OPERATION_MGR_H__

#include "DataType.h"


//////////////////////////////////////////////////////////////////////////
// 视频回传参数
struct STVideoParam
{
	std::string strFmtValue;
	std::string strCameraType;
	std::string strUserCfmType;
	std::string strMuteType;

	STVideoParam& operator = (const STVideoParam& v)
	{
		if (this != &v)
		{
			strFmtValue = v.strFmtValue;
			strCameraType = v.strCameraType;
			strUserCfmType = v.strUserCfmType;
			strMuteType = v.strMuteType;
		}

		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
class OperationMgr
{
public:
	static OperationMgr& Instance();

private:
	OperationMgr(void);
	~OperationMgr(void);

public:
	// 调度台注册
	int DcLogin(const std::string& userId, const std::string& passwd, 
		const std::string& serverIP, const std::string& localIP, const std::string& sipPort);
	// 启动SDK处理业务
	int DcStartRunning();
	// 调度台注销
	int DcLogout(const std::string& userId);
	// 触发状态上报
	int DcTriggerStatusReport(bool bEnableStatusReport) const;
	// 调度台发起视频回传
	int DcVideoMonitor(int iResId, Video_UL_parameter& param, bool bNeedSave = true);
	// 调度台挂断视频回传
	int DcVideoHangup(int iResId, bool bNeedSave = true);
	// 订阅群组
	int DcSubscribeGroup(const std::string& groupId, unsigned long ulEableSubscribeGroup) const;
	// 创建动态重组
	int DcCreateDGNA(DGNA_parameter& param) const;
	// 删除动态重组
	int DcCancelDGNA(int iResId) const;
	// 创建临时组
	int DcCreateTEMPG(DGNA_parameter& param) const;
	// 申请一个临时组号
	int DcGetNextTempGroupID() const;
	//发起语音点呼
	int DcP2PDial(int iResId) const;
	//接收语音点呼
	int DcP2PRecv(int iResId) const;
	//拒接语音点呼
	int DcP2PReject(int iResId) const;
	//挂断语音点呼
	int DcP2PHangup(int iResId) const;
	//加入群组
	int DcJoinGroup(int iResId) const;
	//订阅并自动加入群组
	int DcSubJoinGroup(int iResId) const;
	//发起组呼（固定和临时组呼）或抢权
	int DcPTTDial(int iResId) const;
	//释放话权
	int DcPTTRelease(int iResId) const;
	//退出组呼
	int DcPTTHangup(int iResId) const;
	//发起紧急组呼
	int DcPTTEmergency(int iResId) const;
	//调度台执行静音
	int DcVolMute(int iResId, Mute_parameter& param) const;
	//调度台取消静音
	int DcVolUnMute(int iResId, Mute_parameter& param) const;
	//调度台强拆组呼
	int GroupBreakoff(int iResId) const;
	//调度台强拆点呼
	int P2PBreakoff(int iResId) const;
	//抢话（强拆点呼+发起新点呼）
	int P2PBreakin(int iResId) const;
	//静音控制
	int MuteControl(int iResId, int param) const;
	//订阅/去订阅GIS终端
	int GISSubscribe(int iResId, GIS_parameter& param) const;
	//发短信
	int SDSSendMessage(int iResId, SDS_parameter& param) const;

	// 用于工具栏操作
public:
	// 设置配置文件参数
	void SetParam(const std::string& strFrontFormat, const std::string& strBackFormat);
	// 切换摄像头
	int DcVideoReverse(int iResId) const;
	// 获取是否开启伴音
	std::string GetMuteType(int iResId);

	// 登陆登出参数
private:
	// 用户ID
	std::string m_userId;
	// 用户密码
	std::string m_passwd;
	// 本地IP
	std::string m_localIP;
	// 服务器IP
	std::string m_serverIP;
	// SIP端口
	std::string m_sipPort;

	// 用于工具栏操作
public:
	// 默认前置摄像头视频格式
	std::string m_strFrontFormat;
	// 默认后置摄像头视频格式
	std::string m_strBackFormat;

public:
	// 登陆成功标示
	BOOL m_bLoginSuccess;
	// eLTE_PlayerCtrl类指针
	COleControl* m_pCeLTE_PlayerCtrl;

	// 视频回传资源列表
public:
	std::map<int, STVideoParam> m_mapResource;

	// 切换视频线程
public:
	// 视频关闭信号
	static HANDLE m_hHangupEvent;
	// 切换视频线程
	static unsigned int __stdcall VideoReverseRun(void* lparam);
};

#endif
