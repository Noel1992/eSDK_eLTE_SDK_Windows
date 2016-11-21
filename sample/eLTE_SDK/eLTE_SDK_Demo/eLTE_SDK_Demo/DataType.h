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
description :    Public type definition
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 Initial version
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
	/*-------------------The following information elements are used to point to call, include video call; see p2pcallstatus_indicator.cpp-----------*/
	CALLSTATUS =0, //call status
	CALLERINFO =1, // caller
	CALLEEINFO =2, // callee

	/*--------------------The following information elements are used for group call, see grpcallstatus_indicator.cpp----------------*/
	GRPCALLSTATUS =3, // group call status
	GRPCALLSPERKER =4, // group call speaker
	FLOORSTATUS =5,    //Power status indicator

	/*--------------------The following information elements are used for emergency calls, see emergency_indicator.cpp----------------*/
	EMERGENCYCALL =6,  //emergency call status
	EMERGENCYCALLER =7,  //emergency caller

	/*--------------------The following information elements are used for resource status management,see resourcestatus_indicator.cpp----------------*/
	RESORCEID =8,              // resource ID
	DEVICEID =9,               //the window ID of Video column
	RESSELECTSTATUS =10,        //Resource selected status indicator,For multi GUI linkage
	RESASSIGNSTATUS =11,        //Resource assignment status indicator, that is, whether or not the dispatching station joins the resource group
	RESTXSTATUS =12,            //Resource occupancy status indicator
	RESLOGGINGSTATUS =13,       //Resource logging status indicator
	RESDLSTATUS =14,            //Resource monitor status indicator
	RESREGSTATUS_PROXY =15,     //Resource register status indicator, Agent registration(All cable users,Terminal cluster registry)
	RESREGSTATUS_DIRECT =16,    //Resource register status indicator, Direct registration(Terminal business registration,like video)
	ATTACHINGGROUP =17,         //Join the group of resources

	/*--------------------The following information elements are used for user status management;see userstatus_indicator.cpp----------------*/
	USERSTATUS =18,		//User status indicator,normally notified by BCC
	USERGPSSTATUS =19,       //user GPS status indicator
	USERDLSTATUS =20,            //User monitoring status indicator
	USERDGNASTATUS =21,      //User dynamic state
	VIDEODISPATCHSTATUS =22,     //Video distribution status
	MUTESTATUS =23,          //mute status
	GRPPATCHSTATUS =24  //group patch status
}BundleKey;

typedef enum {
	RESOURCE_STATUS_UNKNOWN = 4000,
	STATUS_SELECTED = 4001,    //Resource selected state
	STATUS_UNSELECTED = 4002,  //Resource unselected state

	STATUS_ASSIGNED = 4003, // group is assigned in SDK
	STATUS_DEASSIGNED = 4004, // group is deassigned in SDK

	STATUS_LISTENING = 4005, // user is being listened 
	STATUS_LISTENEND = 4006, // user is not being listened

	STATUS_LOGGING = 4007, // reserved
	STATUS_NOLOGGING = 4008, // reserved

	STATUS_INPATCH = 4009, // reserved
	STATUS_OUTPATCH = 4010, // reserved

	STATUS_REGOK = 4011, // user is registered
	STATUS_REGFAIL = 4012, // user register fail or not registered 
	STATUS_FORBIDEN = 4013, // user register fail, not allowed 
	STATUS_NOT_FOUND = 4014, // user register fail, not found 
	STATUS_TEMP_UNAVAILABLE = 4015, // user register fail, locked 
	STATUS_RES_CONFILCT = 4016, // user register fail, conflict
	STATUS_UNAUTHORIZED = 4017, // register is to be authenticated 
	STATUS_LICENSE_LIMIT = 4018, // register fail, license limited 
	STATUS_PASSWORD_WRONG = 4019, // register fail, wrong password 

	STATUS_INVITING = 4020, // the resource is initializing a call
	STATUS_RINGING = 4021, // the resource is ringing 
	STATUS_TXMTING = 4022, // the resource is talking/transimitting in a call 
	STATUS_TXMTIDLE = 4023, // the resource is idle 

	STATUS_DGNAOK = 4024, // dynamic regrouping OK 
	STATUS_DGNAFAIL = 4025, // dynamic regrouping fail 
	STATUS_DGNA_UEFAIL = 4026, // dynamic regrouping partial OK, with some UEs fail 
	STATUS_GROUP_ACTIVATED = 4027, // group is activated, PTT is going on 
	STATUS_GROUP_DEACTIVATED = 4028,// group is deactivated, PTT closed 
	STATUS_MUTE = 4029, // the resource is muted 
	STATUS_UNMUTE = 4030, // the resource is unmuted 

	STATUS_GRPPATCH_CREATEOK = 4031, // create patch group OK 
	STATUS_GRPPATCH_CREATEFAIL = 4032, // create patch group fail 
	STATUS_GRPPATCH_CANCELOK = 4033, // cancel patch group OK 
	STATUS_GRPPATCH_CANCELFAIL = 4034, // cancel patch group fail 
	STATUS_GRPPATCH_ADDOK = 4035, // add member to patch group OK 
	STATUS_GRPPATCH_ADDFAIL = 4036, // add member to patch group fail 
	STATUS_GRPPATCH_DELOK = 4037, // delete member from patch group OK 
	STATUS_GRPPATCH_DELFAIL = 4038, // delete member from patch group fail 
	STATUS_VIDEO_CONTROL = 4039, // video control
	STATUS_HALFVOICE_ACTIVATED = 4040, // the resource is active, Half Voice is going on 
	STATUS_HALFVOICE_CLOSED = 4041, // the resource is deactive, Half Voice is closed 
	STATUS_ACCOUNT_LOCKED = 4060 // DC account is locked 

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
	/*--------Notice of change in media-------------*/
	PTT_IND_STATUS_TX_BEGIN,        //Group call right change
	PTT_IND_STATUS_TX_IDLE,         //Group call right idle
	PTT_IND_STATUS_MEDIAPLANE_ERROR,
	PTT_IND_STATUS_EMERGENCY_BEGIN,
	PTT_IND_STATUS_EMERGENCY_UPDATE,
	PTT_IND_STATUS_LICENSE_LIMIT
} Speaker_Status_t;

typedef enum {
	P2P_IND_STATUS_UNKNOWN=2000,
	/*--------Signalling surface state--------*/
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
	/*--------Media surface state--------*/
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
	/*------------Signalling surface state-----------*/
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
	/*------------Media surface state-----------*/
	P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_ERROR,  //Video RTP stream build failed
	P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_NOAUDIO, //Video audio stream establishment failure
	P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED,
	P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED
} P2pvideocall_Status_t;

typedef enum {
	SDS_MODULE = 1,   //Short message service module
	SMS_MODULE,       //SMS module
	AUDIO_MODULE,    //Audio service module
	VIDEO_MODULE,  //Video service module
	GIS_MODULE,  //GIS service module
	SIP_MODULE,  //Signaling channel module
	RTP_MODULE,   //Media channel module
	RING_MODULE,   //ring add by lwj
	SVC_MODULE, //Scheduling machine process
	UNKOWN_MODULE = 999
} dc_module_t;

typedef enum {
	RESOURCE_STATUS_NORMAL = 0, //status normal
	SDS_OVERFLOW,    //message full
	SDS_DISCONNECT,  //DC disconnect with message server
	SDS_CONNECT,     //DC connected with message server
	SDS_SEND_SUCCESS, //message send success
	SDS_SEND_FAIL, //message send failed
	FOLDER_IS_EMPTY,  //export folder is empty
	GIS_LICESNCE_UPDATE,  //GIS License update
	GIS_LICENSCE_EXPIREDAY, //license imminent expiration remind,
	//Notice there are only 7 days left for n days, positive, negative for n days has expired
	SIP_DISCONNECT,       //DC disconnected with BCC
	SIP_RECONNECT,       //DC reconnect with BCC
	SIP_KEEPALIVE_REJECT, //BCC reject keepalive request from DC
	ATTACH_UPLOAD_FAIL,   //Attachment upload failed
	ATTACH_DOWNLOAD_FAIL,  //Attachment download failed
	SIP_START_RUNNING,     //BCC start running
	KICK_OFF,      //BCC kick off notification,Because the same account login somewhere else
	ASYN_CALLBACK_RETURN,   //Asynchronous call return value
	PASSWORD_CHANGE,    //Dispatcher password change
	MODULE_DISCONNECT,
	MODULE_RECONNECT,
	SIP_KA_FAILONCE,       //DC disconnect with BCC once
	SIP_KA_OKONCE,       //DC reconnect with BCC once
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
