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
filename		: 	eLTE_SDK.h
author			:	zWX229156
created		    :	2015/1/5
description		:	定义 DLL 应用程序的导出函数
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/5 初始版本
*********************************************************************/

#ifndef __ELTE_SDK_H__
#define __ELTE_SDK_H__

#include "eLTE_Types.h"

// 这里提供DLL导出接口
#ifdef WIN32
#ifdef  ELTE_SDK_EXPORTS
#define ELTE_SDK_API __declspec(dllexport)
#else
#define ELTE_SDK_API __declspec(dllimport)
#endif
#else
#define ELTE_SDK_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	/**************************************************************************
	* name			: ELTE_SDK_SetLogPath
	* description   : 设置日志路径，请使用绝对路径方便查看日志文件
	* input			: pLogFilePath 日志保存路径
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetLogPath(const ELTE_CHAR* pLogFilePath);

	/**************************************************************************
	* name			: ELTE_SDK_SetLogLevel	
	* description   : 设置日志级别
	* input			: iLogLevel 日志级别
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetLogLevel(ELTE_UINT32 iLogLevel);

	/**************************************************************************
	* name			: ELTE_SDK_ReleaseBuffer
	* description   : 释放SDK内部申请的内存
	* input			: pBuffer 内存指针
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL  ELTE_SDK_ReleaseBuffer(ELTE_CHAR* pBuffer);

	/**************************************************************************
	* name			: ELTE_SDK_GetVersion
	* description   : 获取SDK版本号
	* input			: pVersion 获取SDK版本号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetVersion(ELTE_CHAR** pVersion);

	/**************************************************************************
	* name			: ELTE_SDK_Init
	* description   : 初始化SDK
	* input			: N/A
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_Init(ELTE_INT32 iMediaBypass);

	/**************************************************************************
	* name			: ELTE_SDK_Cleanup
	* description   : 释放SDK
	* input			: N/A
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_Cleanup();

	/**************************************************************************
	* name			: ELTE_SDK_SetEventCallBack
	* description   : 设置消息回调函数
	* input			: fEventCallBack 用户设置的事件回调函数，为空表示取消设置的回调函数
	                  pUserData      用户数据
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetEventCallBack(EventCallBack fEventCallBack, ELTE_VOID* pUserData);

	/**************************************************************************
	* name			: ELTE_SDK_SetYUVFrameCallBack
	* description   : 设置视频数据回调函数
	* input			: fYUVFrameCallBack 用户设置的视频数据回调函数，为空表示取消设置的回调函数
	                  pUserData      用户数据
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetYUVFrameCallBack(YUVFrameCallBack fYUVFrameCallBack, ELTE_VOID* pUserData);

	/**************************************************************************
	* name			: ELTE_SDK_Login
	* description   : 登录
	* input			: pUserID         调度员账号
					  pPWD            调度员密码
					  pServerIP       调度机IP地址
	                  pLocalIP        本地主机IP地址
					  uiServerSIPPort 调度机SIP服务端口
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_Login(const ELTE_CHAR* pUserID, const ELTE_CHAR* pPWD, const ELTE_CHAR* pServerIP, const ELTE_CHAR* pLocalIP, ELTE_UINT32 uiServerSIPPort);

	/**************************************************************************
	* name			: ELTE_SDK_Logout
	* description   : 注销
	* input			: pUserID 调度员账号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_Logout(const ELTE_CHAR* pUserID);

	/**************************************************************************
	* name			: ELTE_SDK_TriggerStatusReport
	* description   : 触发状态上报
	* input			: iEnableStatusReport 是否开启状态上报
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_TriggerStatusReport(ELTE_INT32 iEnableStatusReport);

	/**************************************************************************
	* name			: ELTE_SDK_ProvisionManagerInitMRS
	* description   : 媒体服务器初始化
	* input			: pServerIP 媒体服务器IP地址
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_ProvisionManagerInitMRS(const ELTE_CHAR* pServerIP);

	/**************************************************************************
	* name			: ELTE_SDK_GetDcGroups
	* description   : 获取调度台群组列表
	* input			: pUserID 调度台的用户号
	* output		: pDcGroups 该调度台所属的所有群组列表
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcGroups);

	/**************************************************************************
	* name			: ELTE_SDK_GetDcUsers
	* description   : 获取调度台用户列表
	* input			: pUserID 调度台的用户号
	* output		: pDcUsers 该调度台所管理的所有用户列表
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcUsers(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcUsers);

	/**************************************************************************
	* name			: ELTE_SDK_GetGroupUsers
	* description   : 获取一个群组成员列表
	* input			: pGroupID 群组号
	* output		: pGroupUsers 该群组的所有成员列表
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetGroupUsers(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupUsers);

	/**************************************************************************
	* name			: ELTE_SDK_GetGroupInfo
	* description   : 获取一个群组详细信息
	* input			: pGroupID 群组号
	* output		: pGroupInfo 该群组的详细信息
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetGroupInfo(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupInfo);

	/**************************************************************************
	* name			: ELTE_SDK_GetUserInfo
	* description   : 获取一个用户详细信息
	* input			: pUserID 调度员账号
	* output		: pUserInfo 该用户的详细信息
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetUserInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pUserInfo);

	/**************************************************************************
	* name			: ELTE_SDK_GetDcInfo
	* description   : 获取一个用户详细信息
	* input			: pUserID 调度台的用户号
	* output		: pDcInfo 该调度台详细信息
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcInfo);

	/**************************************************************************
	* name			: ELTE_SDK_UnSubscribeGroup
	* description   : 取消订阅群组
	* input			: pGroupID              群组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_UnSubscribeGroup(const ELTE_CHAR* pGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_SubJoinGroup
	* description   : 订阅并自动加入群组
	* input			: pGroupID  群组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SubJoinGroup(const ELTE_CHAR* pGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_CreateDynamicGroup
	* description   : 创建动态群组
	* input			: pDGNAParam 动态组参数信息
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_CreateDynamicGroup(const ELTE_CHAR* pDGNAParam);

	/**************************************************************************
	* name			: ELTE_SDK_CancelDynamicGroup
	* description   : 删除动态群组
	* input			: pResourceID 动态组组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_CancelDynamicGroup(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_GetTempGroupID
	* description   : 获取临时组号
	* input			: N/A
	* output		: iGroupID 临时组组号
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetTempGroupID(ELTE_INT32* iGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_CreateTempGroup
	* description   : 创建临时组
	* input			: pTEMPGParam 临时组参数信息
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_CreateTempGroup(const ELTE_CHAR* pTEMPGParam);

	/**************************************************************************
	* name			: ELTE_SDK_GetUserRECFileInfoList
	* description   : 获取录像文件列表
	* input			: pQueryXml 查询录像条件Xml
	* output		: pRspXml   查询录像文件结果列表
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetUserRECFileInfoList(const ELTE_CHAR* pQueryXml, ELTE_CHAR** pRspXml);

	/**************************************************************************
	* name			: ELTE_SDK_StartRealPlay
	* description   : 开始播放实时视频
	* input			: pResourceID 资源ID
	                  pVideoParam 媒体参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StartRealPlay(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam);

	/**************************************************************************
	* name			: ELTE_SDK_SetPlayWindow
	* description   : 设置播放实时视频窗口
	* input			: pResourceID 资源ID
	                  pVideoParam 窗口句柄
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetPlayWindow(const ELTE_CHAR* pResourceID, ELTE_VOID* hWnd);

	/**************************************************************************
	* name			: ELTE_SDK_LocalSnapshot
	* description   : 抓拍视频画面
	* input			: pResourceID 资源ID
	                  pSnapshotPath 图片保存路径+文件名
					  ulSnapshotFormat 抓图格式，1 JPG，2 BMP
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_LocalSnapshot(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pSnapshotPath, ELTE_ULONG ulSnapshotFormat);

	/**************************************************************************
	* name			: ELTE_SDK_SetPlayWindowSize
	* description   : 设置播放实时视频窗口的大小
	* input			: pResourceID 资源ID
	                  ulWidth			宽度
					  ulHeight		    高度
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetPlayWindowSize(const ELTE_CHAR* pResourceID, ELTE_ULONG ulWidth, ELTE_ULONG ulHeight);

	/**************************************************************************
	* name			: ELTE_SDK_PTZControl
	* description   : 开始云台控制
	* input			: pResourceID      带云台的摄像机资源ID
	                  iPTZControlCode  云台控制命令
					  iPTZControlValue 云台控制命令对应的参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_PTZControl(const ELTE_CHAR* pResourceID, ELTE_UINT32 iPTZControlCode, ELTE_UINT32 iPTZControlValue);

	/**************************************************************************
	* name			: ELTE_SDK_StopRealPlay
	* description   : 停止播放实时视频
	* input			: pResourceID      带云台的摄像机资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StopRealPlay(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_P2PDial
	* description   : 发起语音点呼
	* input			: pResourceID  资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_P2PDial(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_P2PHalfDpxDial
	* description   : 释放半双工点呼
	* input			: pResourceID  点呼的对端资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_P2PHalfDpxRelease(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_P2PHalfDpxDial
	* description   : 发起半双工点呼
	* input			: pResourceID  点呼的对端资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_P2PHalfDpxDial(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_P2PRecv
	* description   : 接收语音点呼
	* input			: pResourceID  资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_P2PRecv(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_P2PReject
	* description   : 拒接语音点呼
	* input			: pResourceID  资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_P2PReject(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_P2PHangup
	* description   : 挂断语音点呼
	* input			: pResourceID  资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_P2PHangup(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_P2PBreakoff
	* description   : 调度台强拆点呼
	* input			: pResourceID  资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_P2PBreakoff(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_P2PBreakin
	* description   : 抢话（强拆点呼+发起新点呼）
	* input			: pResourceID  资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_P2PBreakin(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_PTTDial
	* description   : 发起组呼（固定和临时组呼）或抢权
	* input			: pGroupID  群组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_PTTDial(const ELTE_CHAR* pGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_PTTRelease
	* description   : 释放话权
	* input			: pGroupID  群组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_PTTRelease(const ELTE_CHAR* pGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_PTTHangup
	* description   : 退出组呼
	* input			: pGroupID  群组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_PTTHangup(const ELTE_CHAR* pGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_PTTEmergency
	* description   : 发起紧急组呼
	* input			: pGroupID  群组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_PTTEmergency(const ELTE_CHAR* pGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_GroupBreakoff
	* description   : 调度台强拆组呼
	* input			: pGroupID  群组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GroupBreakoff(const ELTE_CHAR* pGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_VolMute
	* description   : 调度台执行静音
	* input			: pResourceID 资源ID
	                  pMuteParam 静音参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_VolMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pMuteParam);

	/**************************************************************************
	* name			: ELTE_SDK_VolUnMute
	* description   : 调度台取消静音
	* input			: pResourceID 资源ID
						pUnMuteParam 取消静音参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_VolUnMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pUnMuteParam);

	/**************************************************************************
	* name			: ELTE_SDK_GISSubscribe
	* description   : 订阅/去订阅GIS终端
	* input			: pResourceID 资源ID
	                  pGISParam GIS参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GISSubscribe(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pGISParam);

	/**************************************************************************
	* name			: ELTE_SDK_SDSSendMessage
	* description   : 发送短/彩信
	* input			: pResourceID 资源ID
	                  pSDSParam 短/彩信参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SDSSendMessage(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pSDSParam);

	/**************************************************************************
	* name			: ELTE_SDK_StartVideoDispatch
	* description   : 开始视频分发
	* input			: pResourceID 资源ID
	                  pVideoDispatchParam 视频分发参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StartVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam);

	/**************************************************************************
	* name			: ELTE_SDK_StopVideoDispatch
	* description   : 停止视频分发
	* input			: pResourceID 资源ID
	                  pVideoDispatchParam 视频分发参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StopVideoDispatch(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoDispatchParam);

	/**************************************************************************
	* name			: ELTE_SDK_RecvVideoPlay
	* description   : 调度台接收视频回传
	* input			: pResourceID 资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_RecvVideoPlay(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_StartVWall
	* description   : 发起视频上墙
	* input			: pResVWallID 资源ID
	                  pVWallStartParam 视频上墙参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_VWallStart(const ELTE_CHAR* pResVWallID, const ELTE_CHAR* pVWallStartParam);

	/**************************************************************************
	* name			: ELTE_SDK_GetDcVWallIDList
	* description   : 获取视频上墙ID列表
	* input			: DstObjId 视频上墙的目的地，为一个视频通道
					  pVWallIDList 视频上墙目的ID列表
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcVWallIDList(ELTE_CHAR** pVWallIDList);

	/**************************************************************************
	* name			: ELTE_SDK_VWallStop
	* description   : 终止视频下墙
	* input			: pResVWallID 视频上墙的源，为一个摄像头
	                  pVWallStopParam 视频下墙参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_VWallStop(const ELTE_CHAR* pResVWallID, const ELTE_CHAR* pVWallStopParam);

	//C60新添加接口
	/**************************************************************************
	* name			: ELTE_SDK_TelephoneDial
	* description   : 发起PSTN/PLMN电话呼叫
	* input			: pTelNumber PSTN/PLMN网络的对端电话号码
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_TelephoneDial(const ELTE_CHAR* pTelNumber);

	/**************************************************************************
	* name			: ELTE_SDK_TelephoneHangup
	* description   : 停止PSTN/PLMN电话呼叫
	* input			: pTelNumber PSTN/PLMN网络的对端电话号码
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_TelephoneHangup(const ELTE_CHAR* pTelNumber);

	/**************************************************************************
	* name			: ELTE_SDK_StartDiscreetListen
	* description   : 发起缜密监听
	* input			: pResourceID 资源对象，可以是群组或用户
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StartDiscreetListen(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_StopDiscreetListen
	* description   : 停止缜密监听
	* input			: pResourceID 资源对象，可以是群组或用户
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StopDiscreetListen(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_StartEnvironmentListen
	* description   : 发起环境侦听
	* input			: pResourceID 资源对象，可以是群组或用户
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StartEnvironmentListen(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_P2PTransfer
	* description   : 发起人工转接
	* input			: pResourceID 资源对象，一般是发起人工转接的调度员
					  pP2PTransferParam 人工转接参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_P2PTransfer(const ELTE_CHAR* pResourceID,const ELTE_CHAR* pP2PTransferParam);

	/**************************************************************************
	* name			: ELTE_SDK_CreatePatchGroup
	* description   : 创建派接组
	* input			: pP2PTransferParam 派接组参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_CreatePatchGroup(const ELTE_CHAR* pPatchGroupParam);

	/**************************************************************************
	* name			: ELTE_SDK_CancelPatchGroup
	* description   : 取消已创建派接组
	* input			: pPatchGroupID 资源对象，一般是创建派接组的组号。
									通过notifyResourceStatus读出的组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_CancelPatchGroup(const ELTE_CHAR* pPatchGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_AddPatchGroupMember
	* description   : 增加派接组成员
	* input			: pPatchGroupParam 派接组参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_AddPatchGroupMember(const ELTE_CHAR* pPatchGroupParam);

	/**************************************************************************
	* name			: ELTE_SDK_DeletePatchGroupMember
	* description   : 删除派接组成员
	* input			: pPatchGroupParam 派接组参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_DeletePatchGroupMember(const ELTE_CHAR* pPatchGroupParam);

	/**************************************************************************
	* name			: ELTE_SDK_GetPatchGroups
	* description   : 获取指定调度台的所有派接组列表
	* input			: pDcUser 调度台用户
					  pPatchGroups 该调度台管理的派接组信息		
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetPatchGroups(const ELTE_CHAR* pDcUser, ELTE_CHAR** pPatchGroups);

	/**************************************************************************
	* name			: ELTE_SDK_GetGroupMemberByPatchId
	* description   : 获取指定派接组管理的成员列表
	* input			: pPatchGroupId 派接组号
					  pPatchGroupMembers 该派接组成员信息		
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetGroupMemberByPatchId(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupMembers);

	/**************************************************************************
	* name			: ELTE_SDK_GetPatchGroupInfo
	* description   : 获取指定派接组属性信息
	* input			: pPatchGroupId 派接组号
					  pPatchGroupInfo 该派接的属性信息		
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetPatchGroupInfo(const ELTE_CHAR* pPatchGroupId, ELTE_CHAR** pPatchGroupInfo);

	/**************************************************************************
	* name			: ELTE_SDK_GetUserSpecificGISCfg
	* description   : 查询终端的GIS配置信息
	* input			: pResourceID 终端ID	
	* output		: pUeGisCfg GIS配置信息
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	//ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetUserSpecificGISCfg(const ELTE_CHAR* pResourceID, ELTE_CHAR** pUeGisCfg);

	/**************************************************************************
	* name			: ELTE_SDK_SetGisParam
	* description   : 设置终端的GIS配置信息
	* input			: pResourceID 终端ID
					  pUeGisCfg GIS配置信息
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	//ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetGisParam(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pUeGisCfg);

	/**************************************************************************
	* name			: ELTE_SDK_GetGisSubscription
	* description   : 获取调度台订阅GIS上报的终端列表
	* input			: pResourceID 调度台ID	
	* output		: pGisQuerySubList 订阅GIS上报的终端列表
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetGisSubscription(const ELTE_CHAR* pResourceID, ELTE_CHAR** pGisQuerySubList);

	/**************************************************************************
	* name			: ELTE_SDK_ModifyDynamicGroup
	* description   : 修改动态群组
	* input			: pResourceID 创建动态组的调度台 ID
	                  pDynamicGroupInfo 动态组修改信息
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_ModifyDynamicGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pDynamicGroupInfo);

	/**************************************************************************
	* name			: ELTE_SDK_TempUserJoinGroup
	* description   : 组呼添加临时用户
	* input			: pResourceID 普通组或动态组ID
	                  pPhonePatchParam 动态组修改信息
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_TempUserJoinGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pPhonePatchParam);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
