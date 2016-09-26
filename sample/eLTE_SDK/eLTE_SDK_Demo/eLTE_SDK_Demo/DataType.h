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
created     :    2015/01/16
description :    公共类型定义
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 初始版本
*********************************************************************/

#ifndef __DEMO_DATA_TYPE_H__
#define __DEMO_DATA_TYPE_H__

#include <vector>
#include <string>
#include <list>

//////////////////////////////////////////////////////////////////////////
#define CHECK_API_RETURN_VOID(iRet, api) {\
	if (0 != iRet)\
	{\
		CString strMsg;\
		strMsg.Format(_T("%s failed. (%d)"), api, iRet);\
		if(-40096 == iRet) \
		{\
			strMsg.Append(_T(". network connect timeout"));\
		}\
		MessageBox(strMsg);\
		return;\
	}\
}

#define CHECK_API_RETURN(iRet, api) {\
	if (0 != iRet)\
	{\
		CString strMsg;\
		strMsg.Format(_T("%s failed. (%d)"), api, iRet);\
		MessageBox(strMsg);\
	}\
}

#define CHECK_API_RETURN_RET(iRet, api, ret) {\
	if (0 != iRet)\
	{\
		CString strMsg;\
		strMsg.Format(_T("%s failed. (%d)"), api, iRet);\
		MessageBox(strMsg);\
		return ret;\
	}\
}
//////////////////////////////////////////////////////////////////////////
static CString IntToStr(int value)
{
	CString str;
	str.Format(_T("%d"), value);
	return str;
}

static CString UIntToStr(unsigned int value)
{
	CString str;
	str.Format(_T("%d"), value);
	return str;
}

struct GroupInfo
{
	CString GroupID;
	CString GroupCategory;
	CString GroupPriority;
	CString GroupName;
	CString GroupCreator;
	CString GroupIndex;
	CString GroupVPNID;
};

struct UserInfo
{
	CString UserID;
	CString UserCategory;
	CString UserPriority;
	CString UserName;
};

struct GroupUserInfo
{
	CString UserID;
	CString GroupID;
	CString UserPriorityInGroup;
	CString MemberType;
	CString MemberGroupId;
};

typedef std::vector<GroupInfo*> GroupInfoList;
typedef std::vector<UserInfo*> UserInfoList;
typedef std::vector<GroupUserInfo*> GroupUserInfoList;

struct DcInfo
{
	CString DcID;
	CString Priviledge;
	CString Role;
	CString Alias;
};

struct VideoParamInfo
{
	CString strFormat;
	CString strCamera;
	CString strConfirm;
	CString strMute;

	VideoParamInfo()
	{
		strFormat = _T("");
		strCamera = _T("");
		strConfirm = _T("");
		strMute = _T("");
	}

	VideoParamInfo& operator = (const VideoParamInfo& v)
	{
		strFormat = v.strFormat;
		strCamera = v.strCamera;
		strConfirm = v.strConfirm;
		strMute = v.strMute;
		return *this;
	}

	void Reset()
	{
		strFormat = _T("");
		strCamera = _T("");
		strConfirm = _T("");
		strMute = _T("");
	}
};

struct DispatchParamInfo
{
	CString strResourceId;
	CString strFormat;
	std::list<CString> strDstViewerList;

	DispatchParamInfo()
	{
		strResourceId = _T("");
		strFormat = _T("");
		strDstViewerList.clear();
	}

	DispatchParamInfo& operator = (const DispatchParamInfo& v)
	{
		strFormat = v.strFormat;
		strDstViewerList = v.strDstViewerList;
		return *this;
	}

	void Reset()
	{
		strResourceId = _T("");
		strFormat = _T("");
		strDstViewerList.clear();
	}
};

struct UnDispatchParamInfo
{
	CString strResId;
	CString strUsrId;

	UnDispatchParamInfo()
	{
		strResId = _T("");
		strUsrId = _T("");
	}

	UnDispatchParamInfo& operator = (const UnDispatchParamInfo& v)
	{
		strUsrId = v.strUsrId;
		return *this;
	}

	void Reset()
	{
		strResId = _T("");
		strUsrId = _T("");
	}
};

struct DGNAParamInfo
{
	CString GroupID;
	CString DcID;
	CString Alias;
	CString Priority;
	CString MaxPeriod;
	CString GroupList[8];
	CString UserList[200];
};

struct ModifyDGNAParamInfo
{
	CString GroupID;
	CString DcID;
	CString AddList[199];
	CString DelList[199];
};

struct PCHGRPParamInfo
{
	CString DcID;
	CString GroupID;
	CString Alias;
	CString GroupList[20];
};

typedef std::list<CString> AttachFileList;

struct SDSParamInfo
{
	CString sdstype;
	CString msgBody;
	CString receiver;
	AttachFileList files;
	CString msgId;
};

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

	/*--------------------以下信息元素用于紧急呼叫， 见emergency_indicator.cpp----------------*/
	EMERGENCYCALL =6,  //紧急呼叫指示
	EMERGENCYCALLER =7,  //紧急呼叫人

	/*--------------------以下信息元素用于资源状态管理,见resourcestatus_indicator.cpp----------------*/
	RESORCEID =8,              //资源ID
	DEVICEID =9,               //视频分栏时的窗口号
	RESSELECTSTATUS =10,        //资源选中状态指示， 用于多GUI联动
	RESASSIGNSTATUS =11,        //资源指派状态指示， 即调度台是否加入该资源组
	RESTXSTATUS =12,            //资源占用状态指示
	RESLOGGINGSTATUS =13,       //资源录音状态指示
	RESDLSTATUS =14,            //资源监听状态指示
	RESREGSTATUS_PROXY =15,     //资源注册状态指示, 代理注册(所有有线用户，终端集群注册)
	RESREGSTATUS_DIRECT =16,    //资源注册状态指示, 直接注册(终端业务注册，如视频)
	ATTACHINGGROUP =17,         //资源加入的组号

	/*--------------------以下信息元素用于用户状态管理;见userstatus_indicator.cpp----------------*/
	USERSTATUS =18,		//用户状态指示， 一般由BCC通知
	USERGPSSTATUS =19,       //用户GPS状态指示
	USERDLSTATUS =20,            //用户监听状态指示
	USERDGNASTATUS =21,      //用户动态重组状态
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
	STATUS_TEMP_UNAVAILABLE = 4015, //资源暂时不可用 480
	STATUS_RES_CONFILCT = 4016, //资源冲突 450
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
	STATUS_GRPPATCH_ADDOK = 4035, //派接组添加成员OK
	STATUS_GRPPATCH_ADDFAIL = 4036, //派接组添加成员FAIL
	STATUS_GRPPATCH_DELOK = 4037, //派接组删除成员OK
	STATUS_GRPPATCH_DELFAIL = 4038,  //派接组删除成员FAIL

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
	PTT_IND_STATUS_TX_IDLE,         //组呼话权空闲
	PTT_IND_STATUS_MEDIAPLANE_ERROR,
	PTT_IND_STATUS_EMERGENCY_BEGIN,
	PTT_IND_STATUS_EMERGENCY_UPDATE,
	PTT_IND_STATUS_LICENSE_LIMIT
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
	P2P_IND_STATUS_HALFDUPLEX_BEGIN = 2041,         /** half duplex p2p call request received */
	P2P_IND_STATUS_TX_BEGIN = 2042,                 /** half_duplex p2p call transmit begin */
	P2P_IND_STATUS_TX_IDLE = 2043,                   /** half_duplex p2p call transmit idle */
	P2P_IND_STATUS_TX_VOICE = 2044                   /** half_duplex p2p call transmit voice */
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

//////////////////////////////////////////////////////////////////////////
#define WM_INCOMMING_CALL	(WM_USER+100)
#define WM_START_REAL_PLAY	(WM_USER+101)
#define WM_INCOMMING_VIDEO  (WM_USER+102)
#define WM_INIT             (WM_USER+103)

#endif
