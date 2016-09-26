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


//////////////////////////////////////////////////////////////////////////
typedef int eLTE_RET;

// eLTE_Player 返回码定义
// 213是eSDK标识, 5101是eLTE_Player标识， 51xx是以后eLTE SDK
typedef enum
{
	eLTE_ERR_SUCCESS = 0,					// 成功

	//2135101001 - 2135101200 Normal 错误
	eLTE_ERR_FAILED = 2135101001,			// 通用失败
	eLTE_ERR_INVALID_PARAM,					// 非法入参
	eLTE_ERR_LOAD_TYPE,						// ocx load类型不支持
	eLTE_ERR_NOT_LOGIN,						// 没有登陆
	eLTE_ERR_WAIT_LOGIN_RESULT,				// 等待登录结果中，或者没有登录成功

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


//////////////////////////////////////////////////////////////////////////

typedef enum
{
	/*-------------------以下信息元素用于点呼，包括视频点呼；见p2pcallstatus_indicator.cpp-----------*/
	CALLSTATUS =0, //点呼状态
	CALLERINFO =1, //主叫人
	CALLEEINFO =2, //被叫人

	/*--------------------以下信息元素用于组呼，见grpcallstatus_indicator.cpp----------------*/
	GRPCALLSTATUS =3, //组呼状态
	GRPCALLSPERKER =4, //组呼主讲人
	FLOORSTATUS =5,    //话权状态指示

	/*--------------------以下信息元素用于Emergency call， 见emergency_indicator.cpp----------------*/
	EMERGENCYCALL =6,  //Emergency call指示
	EMERGENCYCALLER =7,  //Emergency call人

	/*--------------------以下信息元素用于资源状态管理,见resourcestatus_indicator.cpp----------------*/
	RESORCEID =8,              //资源ID
	DEVICEID =9,               //视频分栏时的窗口号
	RESSELECTSTATUS =10,        //资源选中状态指示， 用于多GUI联动
	RESASSIGNSTATUS =11,        //资源指派状态指示， 即调度台是否加入该资源组
	RESTXSTATUS =12,            //资源占用状态指示
	RESLOGGINGSTATUS =13,       //资源录音状态指示
	RESDLSTATUS =14,            //资源监听状态指示
	RESREGSTATUS_PROXY =15,     //资源注册状态指示, 代理注册(所有有线user ，终端集群注册)
	RESREGSTATUS_DIRECT =16,    //资源注册状态指示, 直接注册(终端业务注册，如视频)
	ATTACHINGGROUP =17,         //资源加入的组号

	/*--------------------以下信息元素用于user 状态管理;见userstatus_indicator.cpp----------------*/
	USERSTATUS =18,		//user 状态指示， 一般由BCC通知
	USERGPSSTATUS =19,       //user GPS状态指示
	USERDLSTATUS =20,            //user 监听状态指示
	USERDGNASTATUS =21,      //user 动态重组状态
	VIDEODISPATCHSTATUS =22,     //视频分发状态
	MUTESTATUS =23,          //静音状态
	GRPPATCHSTATUS =24  //派接操作状态
}BundleKey;

typedef enum {
	RESOURCE_STATUS_UNKNOWN = 4000,
	STATUS_SELECTED = 4001,    //资源选中状态
	STATUS_UNSELECTED = 4002,  //资源去选中状态

	STATUS_ASSIGNED = 4003,    //资源指派状态-
	STATUS_DEASSIGNED = 4004,  //资源去指派状态-

	STATUS_LISTENING = 4005,   //资源监听中状态
	STATUS_LISTENEND = 4006,   //资源监听结束状态

	STATUS_LOGGING = 4007,      //资源录音中状态
	STATUS_NOLOGGING = 4008,    //资源未设置录音

	STATUS_INPATCH = 4009,      //资源被派接
	STATUS_OUTPATCH = 4010,     //资源解除派接

	STATUS_REGOK = 4011,        //资源注册
	STATUS_REGFAIL = 4012,      //资源去注册
	STATUS_FORBIDEN = 4013,     //资源未授权 500
	STATUS_NOT_FOUND = 4014,    //资源不存在 404
	STATUS_TEMP_UNAVAILABLE = 4015, //resource not available now 480
	STATUS_RES_CONFILCT = 4016, //resource conflict 450
	STATUS_UNAUTHORIZED = 4017, //未鉴权 401
	STATUS_LICENSE_LIMIT = 4018, //License受限 503
	STATUS_PASSWORD_WRONG = 4019, //密码错误 403

	//下面四个状态的顺序很重要， 因为 isValidTransition（）需要参考顺序
	STATUS_INVITING = 4020,    //the resource is initializing a call
	STATUS_RINGING = 4021,      //the resource is ringing
	STATUS_TXMTING = 4022,     //the resource is talking/transimitting in a call
	STATUS_TXMTIDLE = 4023,    //the resource is idle

	STATUS_DGNAOK = 4024,       //dynamic regrouping OK
	STATUS_DGNAFAIL = 4025,      //dynamic regrouping fail
	STATUS_DGNA_UEFAIL = 4026,   //dynamic regrouping one UE fail
	STATUS_GROUP_ACTIVATED = 4027,
	STATUS_GROUP_DEACTIVATED = 4028,
	STATUS_MUTE = 4029,        //sound mute
	STATUS_UNMUTE = 4030,       //sound unmute
	STATUS_GRPPATCH_CREATEOK = 4031,  //派接组创建OK
	STATUS_GRPPATCH_CREATEFAIL = 4032, //派接组创建失败
	STATUS_GRPPATCH_CANCELOK = 4033, //派接组取消OK
	STATUS_GRPPATCH_CANCELFAIL = 4034, //派接组取消失败
	STATUS_GRPPATCH_ADDOK = 4035, //派接组添加memberOK
	STATUS_GRPPATCH_ADDFAIL = 4036, //派接组添加memberFAIL
	STATUS_GRPPATCH_DELOK = 4037, //派接组删除memberOK
	STATUS_GRPPATCH_DELFAIL = 4038,  //派接组删除memberFAIL

	STATUS_ACCOUNT_LOCKED = 4060 // 账号被锁定

} res_status_value_t;

typedef enum {
	PTT_IND_STATUS_UNKNOWN=1000,
	PTT_IND_STATUS_RECEIVED,
	PTT_IND_STATUS_REQUEST,
	PTT_IND_STATUS_ACCEPT,
	PTT_IND_STATUS_WAITING,
	PTT_IND_STATUS_REJECT,
	PTT_IND_STATUS_SNATCH,
	PTT_IND_STATUS_HANGUP_OK,
	PTT_IND_STATUS_CLOSE_OK,
	PTT_IND_STATUS_CANCEL_OK,
	PTT_IND_STATUS_PREMPTED,
	/*--------媒体面变化的通知-------------*/
	PTT_IND_STATUS_TX_BEGIN,        //组呼话权变化
	PTT_IND_STATUS_TX_IDLE,         //组呼Right to speak
	PTT_IND_STATUS_MEDIAPLANE_ERROR,
	PTT_IND_STATUS_EMERGENCY_BEGIN,
	PTT_IND_STATUS_EMERGENCY_UPDATE,
	PTT_IND_STATUS_LICENSE_LIMIT,
	PTT_IND_STATUS_SIGNAL_ERROR = 1017
} Speaker_Status_t;

typedef enum {
	P2P_IND_STATUS_UNKNOWN=2000,
	/*--------信令面状态--------*/
	P2P_IND_STATUS_INITIATED,
	P2P_IND_STATUS_RECEIVED,
	P2P_IND_STATUS_ANSWERED,
	P2P_IND_STATUS_EMERGENCY,
	P2P_IND_STATUS_PROCEEDING,
	P2P_IND_STATUS_RINGING,
	P2P_IND_STATUS_ACKED,
	P2P_IND_STATUS_RELEASED,
	P2P_IND_STATUS_HANGUPED,
	P2P_IND_STATUS_HANGUPED_ACTIVE,
	P2P_IND_STATUS_CANCELLED,
	P2P_IND_STATUS_PREMPTED,
	P2P_IND_STATUS_RELEASED_BUSY,
	P2P_IND_STATUS_BREAKIN_FAIL,
	P2P_IND_STATUS_BREAKIN_SUCCESS,
	P2P_IND_STATUS_PROHIBITED,
	P2P_IND_STATUS_REMOTE_NOANSWER,
	P2P_IND_STATUS_REMOTE_NOTFOUND,
	P2P_IND_STATUS_LICENSE_LIMIT,
	/*--------媒体面状态--------*/
	P2P_IND_STATUS_MEDIAPLANE_ERROR,
	P2P_IND_STATUS_MEDIAPLANE_MUTE,
	P2P_IND_STATUS_MEDIAPLANE_UNMUTE,
	P2P_IND_STATUS_REMOTE_NOTSUPPORTED,
	P2P_IND_STATUS_REMOTE_NOTCONNECTED,

	P2P_IND_STATUS_PICKEDUP = 2040,
} P2pcall_Status_t;

typedef enum {
	P2Pvideocall_IND_STATUS_UNKNOWN=3000,
	/*------------信令面状态-----------*/
	P2Pvideocall_IND_STATUS_INITIATED,
	P2Pvideocall_IND_STATUS_RECEIVED,
	P2Pvideocall_IND_STATUS_ANSWERED,
	P2Pvideocall_IND_STATUS_PROCEEDING,
	P2Pvideocall_IND_STATUS_RINGING,
	P2Pvideocall_IND_STATUS_ACKED,
	P2Pvideocall_IND_STATUS_RELEASED,
	P2Pvideocall_IND_STATUS_HANGUPED,
	P2Pvideocall_IND_STATUS_HANGUPED_ACTIVE,
	P2Pvideocall_IND_STATUS_CANCELLED,
	P2Pvideocall_IND_STATUS_MONITOR,
	P2Pvideocall_IND_STATUS_PREMPTED,
	P2Pvideocall_IND_STATUS_RELEASED_BUSY,
	P2Pvideocall_IND_STATUS_PROHIBITED,
	P2Pvideocall_IND_STATUS_REMOTE_NOANSWER,
	P2Pvideocall_IND_STATUS_REMOTE_NOTFOUND,
	P2Pvideocall_IND_STATUS_LICENSE_LIMIT,
	P2Pvideocall_IND_STATUS_EMERGENCY,
	/*------------媒体面状态-----------*/
	P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_ERROR,  //视频RTP流建立失败
	P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_NOAUDIO, //视频伴音流建立失败
	P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED,
	P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED
} P2pvideocall_Status_t;

typedef enum {
	SDS_MODULE = 1,   //短信业务模块
	SMS_MODULE,       //彩信业务模块
	AUDIO_MODULE,    //语音业务模块
	VIDEO_MODULE,  //视频业务模块
	GIS_MODULE,  //GIS业务模块
	SIP_MODULE,  //信令通道模块
	RTP_MODULE,   //媒体通道模块
	RING_MODULE,   //响铃 add by lwj
	SVC_MODULE, //调度机进程
	UNKOWN_MODULE = 999
} dc_module_t;

typedef enum {
	RESOURCE_STATUS_NORMAL = 0, //状态正常
	SDS_OVERFLOW,    //短信满
	SDS_DISCONNECT,  //DC和短信服务器断链
	SDS_CONNECT,     //DC和短信服务器连接
	SDS_SEND_SUCCESS, //短信发送成功
	SDS_SEND_FAIL, //短信发送失败
	FOLDER_IS_EMPTY,  //导出文件夹为空
	GIS_LICESNCE_UPDATE,  //GIS License变更
	GIS_LICENSCE_EXPIREDAY, //license 即将过期提醒，只剩7天时通知，正数为还剩n天，负数为已经过期n天
	SIP_DISCONNECT,       //DC和BCC断链
	SIP_RECONNECT,       //DC和BCC恢复
	SIP_KEEPALIVE_REJECT, //BCC拒绝DC的心跳请求
	ATTACH_UPLOAD_FAIL,   //附件上传失败
	ATTACH_DOWNLOAD_FAIL,  //附件下载失败
	SIP_START_RUNNING,     //BCC 启动
	KICK_OFF,      //BCC 踢出调度台通知,因为有相同账号登录
	ASYN_CALLBACK_RETURN,   //异步调用返回值
	PASSWORD_CHANGE,    //调度员密码修改
	MODULE_DISCONNECT,
	MODULE_RECONNECT,
	SIP_KA_FAILONCE,       //DC和BCC断链一次
	SIP_KA_OKONCE,       //DC和BCC恢复一次
	USER_DELETE,
	UNKOWN_STATUS = 999
} module_status_t;

typedef enum
{
	breakoff,
	insert,
	SubGroup,
	DeSubGroup,
	doTransfer,
	doBreakIn,
	doPTZ,
	Dispatch,
	Dispatchdelete,
	VwallStop,
	VwallStart,
	DeAssignDgna,
	invalid_type,
	UNKOWN_MSGTP = 999
}call_back_msg_type;

#define WM_ELTE_CUSTOM_MSG_BEGIN	(WM_USER+100)
#define WM_RECEIVED			        (WM_ELTE_CUSTOM_MSG_BEGIN+1)	// 对方来电
#define WM_VIDEORECEIVED            (WM_ELTE_CUSTOM_MSG_BEGIN+2)    // 终端视频回传
#define WM_SHOW_COMMING_VIDEO       (WM_ELTE_CUSTOM_MSG_BEGIN+3)    // 接收对方请求视频回传或分发
//#define WM_			        (WM_ELTE_CUSTOM_MSG_BEGIN+1)	// 对方来电
#define WM_ELTE_CUSTOM_MSG_END		(WM_ELTE_CUSTOM_MSG_BEGIN+100)

#endif
