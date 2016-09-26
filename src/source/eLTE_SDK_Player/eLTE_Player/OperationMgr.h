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
	std::string strVideoType;

	STVideoParam& operator = (const STVideoParam& v)
	{
		if (this != &v)
		{
			strFmtValue = v.strFmtValue;
			strCameraType = v.strCameraType;
			strUserCfmType = v.strUserCfmType;
			strMuteType = v.strMuteType;
			strVideoType = v.strVideoType;
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
	// 调度台发起视频回传
	int DcVideoMonitor(int iResId, const STVideoParam& param, bool bNeedSave = true);
	// 调度台挂断视频回传
	int DcVideoHangup(int iResId, bool bNeedSave = true);

	// 用于工具栏操作
public:
	// 设置配置文件参数
	void SetParam(const std::string& strFrontFormat, const std::string& strBackFormat);
	// 切换摄像头
	int DcVideoReverse(int iResId) const;
	// 获取是否开启伴音
	std::string GetMuteType(int iResId);
	// 获取视频类型
	std::string GetVideoType(int iResId);

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
