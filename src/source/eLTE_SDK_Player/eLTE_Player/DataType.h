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
filename    :    DataType.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    数据类型定义类。包括内部使用的枚举和结构体
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_DATA_TYPE_H__
#define __ELTE_OCX_DATA_TYPE_H__

#include <list>

//////////////////////////////////////////////////////////////////////////
typedef int eLTE_RET;

// eLTE_Player 返回码定义
// 213是eSDK标识, 5101是eLTE_Player标识， 51xx是以后eLTE SDK
typedef enum
{
	eLTE_ERR_SUCCESS = 0,					// 成功

	//2135101001 - 2135101200 普通错误
	eLTE_ERR_FAILED = 2135101001,			// 通用失败
	eLTE_ERR_INVALID_PARAM,					// 非法入参
	eLTE_ERR_LOAD_TYPE,						// ocx load类型不支持
	eLTE_ERR_NOT_LOGIN,						// 没有登陆
	eLTE_ERR_WAIT_LOGIN_RESULT,				// 等待登陆结果中，或者没有登陆成功
	eLTE_ERR_TIMEOUT,						// 等待超时
	eLTE_ERR_LOG_INIT,						// 日志初始化失败

	//2135101201 - 2135101300 播放器错误
	eLTE_ERR_VIDEO_STATE = 2135101201,		// 视频播放 状态错误（如：正在播放中，不能播放）
	eLTE_ERR_AUDIO_OPEN,					// 音频播放 设备打开失败
	eLTE_ERR_AUDIO_CLOSE,					// 音频播放 设备关闭失败
	eLTE_ERR_AUDIO_RESET,					// 音频播放 设备重置失败
	eLTE_ERR_AUDIO_PREPARE,					// 音频播放 预播声音数据失败
	eLTE_ERR_AUDIO_UNPREPARE,				// 音频播放 清除预播声音数据失败
	eLTE_ERR_AUDIO_WRITE,					// 音频播放 播发声音数据失败
	eLTE_ERR_AUDIO_SET_VOLUME,				// 音频播放 设置音量失败
	eLTE_ERR_AUDIO_GET_VOLUME,				// 音频播放 获取音量失败
	eLTE_ERR_AUDIO_OUT_BUFFER,				// 音频播放 播放缓存已满	
	eLTE_ERR_AUDIO_DECODE,					// 音频播放 解码失败

	//2135101301 - 2135101400 文件操作错误
	eLTE_ERR_CONFIG_FILE_LOAD = 2135101301,	// 加载ini配置文件失败
	eLTE_ERR_CONFIG_FILE_READ,				// 读取ini配置文件失败
	eLTE_ERR_CONFIG_FILE_WRITE,				// 写入ini配置文件失败
	eLTE_ERR_LOG_FILE_OPEN,					// 打开/创建日志文件失败

	//2135101401 - 2135101500 XML操作错误
	eLTE_ERR_XML_FIND_ELEM = 2135101401,	// 查找XML节点失败
	eLTE_ERR_XML_GET_ELEM_VALUE				// 获取XML节点失败

} eLTE_RET_ERROR_CODE_E;

//////////////////////////////////////////////////////////////////////////
// 版本类型
typedef enum
{
	eLTE_VERSION_INVALID = 0,		// 非法版本
	eLTE_VERSION_OCX,				// OCX 版本
	eLTE_VERSION_SDK				// SDK 版本
} eLTE_VERSION_TYPE_E;

//////////////////////////////////////////////////////////////////////////
// 语言类型
typedef enum
{
	eLTE_LANG_INVALID = 0,			// 非法语言
	eLTE_LANG_zh_CN,				// 中文
	eLTE_LANG_en_US,				// 英文
	eLTE_LANG_es_ES					// 西班牙语
} eLTE_LANG_CATEGORY_E;

//////////////////////////////////////////////////////////////////////////
// eLTE_Player 加载模式
typedef enum
{
	eLTE_LOAD_INVALID = 0,
	eLTE_LOAD_OPERATE_MGR,			// 当需要加载插件执行管理操作时使用1
	eLTE_LOAD_MEDIA_PLAYER			// 当作为播放器模式使用时使用2
} eLTE_LOAD_TYPE_E;

//////////////////////////////////////////////////////////////////////////
// 抓图类型
typedef enum
{
	eLTE_SNAPSHOT_INVALID = 0,
	eLTE_SNAPSHOT_JPG,				// JPG 格式
	eLTE_SNAPSHOT_BMP				// BMP 格式
} eLTE_SNAPSHOT_FORMAT_E;

//////////////////////////////////////////////////////////////////////////
// eLTE_Player消息ID定义
#define WM_ELTE_CUSTOM_MSG_BEGIN	(WM_USER+100)
#define WM_ELTE_POST_EVENT			(WM_ELTE_CUSTOM_MSG_BEGIN+1)	// 事件消息ID
#define WM_ELTE_POST_PLAYER_EVENT	(WM_ELTE_CUSTOM_MSG_BEGIN+2)	// 播放器窗体事件消息ID
#define WM_ELTE_EXIT_FULL_SCREEN	(WM_ELTE_CUSTOM_MSG_BEGIN+3)	// 退出全屏消息ID
#define WM_ELTE_INVALID_PARENTWND	(WM_ELTE_CUSTOM_MSG_BEGIN+4)	// 重绘父窗口CImageButton中使用
#define WM_ELTE_IMG_BTN_CLICKED		(WM_ELTE_CUSTOM_MSG_BEGIN+5)	// CImageButton Clicked消息
#define WM_ELTE_VIDEO_EXIT			(WM_ELTE_CUSTOM_MSG_BEGIN+6)	// CVideoPane 关闭消息
#define WM_ELTE_CUSTOM_MSG_END		(WM_ELTE_CUSTOM_MSG_BEGIN+100)

//////////////////////////////////////////////////////////////////////////
// 播放器窗体事件
typedef enum
{
	eLTE_PLAYER_HIDE = 0,					// OCX 播放器窗体隐藏
	eLTE_PLAYER_SHOW,						// OCX 播放器窗体显示
	eLTE_PLAYER_FULLSCREEN,					// OCX 播放器全屏
	eLTE_PLAYER_NORMALSCREEN,				// OCX 播放器退出全屏
	eLTE_PLAYER_MUTE,						// OCX 播放器静音
	eLTE_PLAYER_UNMUTE,						// OCX 播放器取消静音
	eLTE_PLAYER_REVERSE,					// OCX 播放器切换镜头(XML参数为视频参数)
	eLTE_PLAYER_SNAPSHOT,					// OCX 播放器抓图(XML参数为图片保存路径)
	eLTE_PLAYER_POSITION,					// OCX 播放器坐标变动(XML参数为坐标)
	eLTE_PLAYER_TITLE						// OCX 播放器窗口标题(XML参数为窗口标题)
} eLTE_PLAYER_EVENT_ID_E;

typedef struct
{
	// videoparam
	std::string strFmtValue;
	std::string strCameraType;
	std::string strUserCfmType;
	std::string strMuteType;

	// snapshot
	std::string strSnapshotPath;

	// position
	int iLeft;
	int iTop;
	int iWidth;
	int iHeight;

	// title
	std::string strTitle;
} PlayerEventParamInfo;

//////////////////////////////////////////////////////////////////////////
// 事件类型
typedef enum
{
	EVENT_NOTIFY_USER_STATUS = 0,			// 设备状态变化事件通知
	EVENT_NOTIFY_P2P_VIDEO_CALL_STATUS,		// 开始实况事件通知
	EVENT_NOTIFY_RESOURCE_STATUS,			// 群组关系状态变化事件通知
	EVENT_NOTIFY_PROVISION_CHANGE,			// 设备属性配置变更通知事件
	EVENT_NOTIFY_PROVISION_ALLRESYNC,		// 自动下载配置数据通知事件
	EVENT_NOTIFY_P2P_CALL_STATUS,			// 点呼状态变化事件
	EVENT_NOTIFY_GROUP_STATUS,				// 组呼状态变化事件
	EVENT_NOTIFY_MODULE_STATUS,				// 模块组件状态事件
	EVENT_NOTIFY_GIS_REPORT,                // 终端GIS信息上报事件
	EVENT_NOTIFY_GIS_STATUS,                // 终端订阅状态上报事件
	EVENT_NOTIFY_SDS_REPORT,                // 短信上报事件
	EVENT_NOTIFY_SDS_RETCODE                // 短信发送是否成功事件
} eLTE_EVENT_ID_E;

// 事件结构体
typedef struct  
{
	int iUserID;				// 资源ID
	int iStatusType;			// 状态类型，固定为USERSTATUS
	int iStatusValue;			// 状态取值
	int iPeerID;				// 事件类型
	int iDirection;				// “1”表示主叫，“2”表示被叫
	int iCallType;				// 资源类型
} NotifyUserStatusInfo;			// 设备状态变化事件通知消息结构体

typedef struct
{
	int iCallStatus;			// 事件类型
	int iLocalAudioPort;		// 本地语音RTP端口
	int iLocalVideoPort;		// 本地视频端口
	int iRemoteAudioPort;		// 远端语音RTP端口
	int iRemoteVideoPort;		// 远端视频端口
	std::string strRemoteIp;	// 对端IP地址
	int iCallee;				// 被叫
	int iCaller;				// 主叫
	int iSoundMute;				// 伴音
	int iChannel;				// 传输Channel
	int iUserConfirm;			// 用户确认
	int iCamera;				// 资源ID
	int iSoundPtype;			//
	int iVideoFormatType;		// 视频格式
} NotifyP2pvideocallStatusInfo;	// 开始实况事件通知消息结构

typedef struct  
{
	int iCallStatus;			// 事件类型
	int iCaller;				// 主叫
	int iCallee;				// 被叫
} NotifyP2pcallStatusInfo;		// 点呼状态变化事件

typedef struct  
{
	int iGroupID;				// 组号
	int iGroupCallStatus;		// 事件类型
	int iSpeaker;				// 主讲人（可能为空）
} NotifyGroupStatusInfo;		// 组呼状态变化事件

typedef struct  
{
	int iResourceID;			// 资源ID
	int iStatusType;			// 资源状态类型
	int iStatusValue;			// 资源状态值
	int iAttachingGroup;		// 用户当前加入的组号
	int iCause;					// 当type为USERDGNASTATUS时才有
} NotifyResourceStatusInfo;		// 群组关系状态变化事件通知消息结构体

typedef struct  
{
	int iResourceID;			// 资源ID
	int iModuleType;			// 模块类型
	int iModuleStatus;			// 模块状态
	int iCallBackMsgType;		// 回调消息类型
	std::string strModulePara;	// 模块状态对应消息参数
} NotifyModuleStatusInfo;

typedef struct
{
	int iChangeType;			// 类型
	int iChangeMO;				// 取值
	void* pInfo;				// 下面MO_xxx_Info结构体
} NotifyProvisionChangeInfo;	// 设备属性配置变更通知事件结构体

// ProvChangeIndicator_PTTUser
typedef struct
{
	int iUserID;
	int iUserCategory;			// 包括PTTuser， Camera, FixedLine, etc
	int iUserPriority;
	std::string strUserName;
	int iVpnID;
	int iVpnOut;
	int iVpnIn;
	int iLocation;
} MO_PTTUser_Info;

// ProvChangeIndicator_FixedUser
typedef MO_PTTUser_Info MO_FixedUser_Info;

// ProvChangeIndicator_DCAttribute
typedef struct
{
	int iDcID;
	int iPriviledge;
	int iRole;					// supervisor, operator
	std::string strAlias;
} MO_DCAttribute_Info;

// ProvChangeIndicator_Group
typedef struct 
{
	int iGroupID;
	int iGroupCategory;			// 全网， 区域， 静态， 动态， etc
	int iGroupPriority;
	std::string strGroupName;
	int iGroupState;			// 0=disable, 1=enable
	int iVpnID;
	int iSetupDcID;				// creator, valid if it's dynamic or temp group
} MO_Group_Info;

// ProvChangeIndicator_GrpUserInfo
typedef struct  
{
	int iUserID;				// 根据membertype，可能为静态用户或组 ID
	int iGroupID;				// 可能为静态组或动态组
	int iUserPriorityInGroup;
	int iMemberType;			// 1=普通用户成员， 0=动态重组中的组成员
} MO_GroupUser_Info;

// ProvChangeIndicator_RangeList
typedef struct
{
	int iRangeID;
	int iRangeCategory;			// group range, user range,
	int iFrom;
	int iTo;
	std::string strAlias;
} MO_RangeList_Info;

// ProvChangeIndicator_DCRange
typedef struct 
{
	int iRangeID;
	int iDcID;
} MO_DCRanges_Info;

// ProvChangeIndicator_HotlineCfg
typedef struct  
{
	int iDcID;
	int iHotIndex;
	int iNumber;
	std::string strAlias;
} MO_HotlineCfg_Info;

// ProvChangeIndicator_StMsgCfg
typedef struct  
{
	int iID;
	std::string strStMsg;
} MO_StMsgCfg_Info;

	//MO_DECUSER

// ProvChangeIndicator_GisCfg
typedef struct  
{
	int iUserID;
	unsigned int uiReportSwitch;// 上报开关, 0=关闭， 1=开启
	unsigned int uiReportPeriod;
	unsigned int uiReportCapability;
	unsigned int uiReportDistance;// 支持GIS终端上报
} MO_GisCfg_Info;

// ProvChangeIndicator_GisIpc
typedef struct
{
	int iUserID;
	double dLatitude;
	double dLongitude;
	double dAltitude;
	std::string strAddress;
} MO_GisIpc_Info;

// GISReportIndicator
typedef std::map<int, int> AckStateMap;

typedef struct
{
	std::string strSender;
	std::string strTime;
	std::string strAltitude;
	std::string strLatitude;
	std::string strLongtitude;
	AckStateMap ackStateMap;
	int iTriggerCode;
	int iReportState;
} GisReportInfo;

typedef std::list<std::string> RecvfileList;	/** Receiver list */
typedef struct  
{
	std::string strType;
	std::string strContent;
	std::string strFrom;
	RecvfileList listMmsFileName;
	std::string strSubject;
	std::string strDate;
	std::string strTime;
	std::string strRetCode;
}SdsMessageInfo;

/**
   定义群组显示数据结构
**/
typedef enum {
        P2Pvideocall_IND_STATUS_UNKNOWN = 3000,
/*------------信令面状态-----------*/
    P2Pvideocall_IND_STATUS_INITIATED = 3001,
    P2Pvideocall_IND_STATUS_RECEIVED = 3002,
    P2Pvideocall_IND_STATUS_ANSWERED = 3003,
    P2Pvideocall_IND_STATUS_PROCEEDING = 3004,
    P2Pvideocall_IND_STATUS_RINGING = 3005,
    P2Pvideocall_IND_STATUS_ACKED = 3006,
    P2Pvideocall_IND_STATUS_RELEASED = 3007,
    P2Pvideocall_IND_STATUS_HANGUPED = 3008,
    P2Pvideocall_IND_STATUS_HANGUPED_ACTIVE = 3009,
    P2Pvideocall_IND_STATUS_CANCELLED = 3010,
    P2Pvideocall_IND_STATUS_MONITOR = 3011,
    P2Pvideocall_IND_STATUS_PREMPTED = 3012,
    P2Pvideocall_IND_STATUS_RELEASED_BUSY = 3013,
    P2Pvideocall_IND_STATUS_PROHIBITED = 3014,
    P2Pvideocall_IND_STATUS_REMOTE_NOANSWER = 3015,
    P2Pvideocall_IND_STATUS_REMOTE_NOTFOUND = 3016,
    P2Pvideocall_IND_STATUS_LICENSE_LIMIT = 3017,
    P2Pvideocall_IND_STATUS_EMERGENCY = 3018,
	/*------------媒体面状态-----------*/
    P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_ERROR = 3019,  //视频RTP流建立失败
    P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_NOAUDIO = 3020, //视频伴音流建立失败
    P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED = 3021,
    P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED = 3022,
    P2Pvideocall_IND_STATUS_SIGNAL_ERROR = 3023,            //SIP error for GW-mode
    P2Pvideocall_IND_STATUS_VIDEO_CONTROL = 3024,            //PTZ control for GW-mode

    P2Pvideocall_IND_STATUS_PICKEDUP = 3040            //本端已摘机

} P2pvideocall_Status_t;

#endif
