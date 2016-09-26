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


class OperationMgr
{
public:
	static OperationMgr& Instance();

private:
	OperationMgr(void);
	~OperationMgr(void);

public:
	// 调度台注册
	int Login(const std::string& userId, const std::string& passwd, 
		const std::string& serverIP, const std::string& localIP, const std::string& sipPort);
	// 调度台注销
	int Logout(const std::string& userId);
	// 启动SDK处理业务
	int StartRunning();
	// 触发状态上报
	int TriggerStatusReport(unsigned int uiEnableStatusReport) const;
	// 调度台发起视频回传
	int VideoMonitor(int iResId, Video_UL_parameter& param) const;
	// 调度台挂断视频回传
	int VideoHangup(int iResId) const;
	// 云台控制
	int PTZControl(int iResId, FVG_control& control) const;
	// 订阅群组
	int SubscribeGroup(int groupId, unsigned int uiEableSubscribeGroup) const;
	// 创建动态重组
	int CreateDynamicGroup(DGNA_parameter& param) const;
	// 删除动态重组
	int CancelDynamicGroup(int iResId) const;
	// 创建临时组
	int CreateTempGroup(DGNA_parameter& param) const;
	// 申请一个临时组号
	int GetNextTempGroupID() const;
	// 发起语音点呼
	int P2PDial(int iResId) const;
	// 发起半双工点呼
	int P2PHalfDpxDial(int iResId) const;
	// 释放半双工点呼
	int P2PHalfDpxRelease(int iResId) const;
	// 接收语音点呼
	int P2PRecv(int iResId) const;
	// 拒接语音点呼
	int P2PReject(int iResId) const;
	// 挂断语音点呼
	int P2PHangup(int iResId) const;
	// 调度台强拆点呼
	int P2PBreakoff(int iResId) const;
	// 抢话（强拆点呼+发起新点呼）
	int P2PBreakin(int iResId) const;
	// 订阅并自动加入群组
	int SubJoinGroup(int groupId) const;
	// 发起组呼（固定和临时组呼）或抢权
	int PTTDial(int groupId) const;
	// 释放话权
	int PTTRelease(int groupId) const;
	// 退出组呼
	int PTTHangup(int groupId) const;
	// 发起紧急组呼
	int PTTEmergency(int groupId) const;
	// 调度台强拆组呼
	int GroupBreakoff(int groupId) const;
	// 调度台执行静音
	int VolMute(int iResId, Mute_parameter& param) const;
	// 设置终端的GIS配置信息
	int SetUserSpecificGISCfg(int resId, UEGisCfg_parameter& param) const;
	// 调度台取消静音
	int VolUnMute(int iResId, Mute_parameter& param) const;
	// 订阅/去订阅GIS终端
	int GISSubscribe(int iResId, GIS_parameter& param) const;
	// 发送短/彩信
	int SDSSend(const int& iResId, SDS_parameter& param) const;
	// 开始视频分发
	int VideoDispatch(int iResId, Video_Dispatch_parameter& param) const;
	// 停止视频分发
	int StopVideoDispatch(int iResId, Video_Dispatch_Delete& param) const;
	// 接受视频回传
	int RecvVideoPlay(int iResId) const;

	// 调度台发起视频上墙
	int VWallStart(int iResId, VWallStart_parameter& param) const;
	// 视频下墙
	int VWallStop(int iResId, VWallStop_parameter& param) const;
	// 更新电视墙ID状态
	int UpdateDcVWallIDState(VWallInfoList* const &m_WallInfoList) const;

	// 发起PSTN/PLMN电话呼叫 
	int TelephoneDial(const std::string& PhoneNum) const;

	// 挂断PSTN/PLMN电话呼叫 
	int TelephoneHangup(const std::string& PhoneNum) const;

	// 缜密监听 
	int DiscreetListenOpera(int ResourceID, Operation_t OperationType) const;
	
	int P2PTransfer(const int iResId, transfer_parameter& param) const;

	// 创建派接组、增加派接组成员、删除派接组成员
	int OperatePatchGroup(PCHGRP_Para& param, const unsigned short& uOperateType) const;

	// 删除派接组
	int CancelPatchGroup(int iResId) const;

	//修改动态群组
	int ModifyDynamicGroup(DGNA_Modify_parameter& param) const;

	// 调度台组呼添加临时用户
	int TempUserJoinGroup(const int& resid, const PhonePatch_parameter& param) const;

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
	// 登陆成功标示
	BOOL m_bLoginSuccess;
};

#endif
