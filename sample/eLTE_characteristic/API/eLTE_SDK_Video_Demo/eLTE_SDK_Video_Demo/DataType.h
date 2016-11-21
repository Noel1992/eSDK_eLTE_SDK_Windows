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
author      :    zhangziqian zwx229156
created     :    2016/05/30
description :    data type define class.Enumeration and structure including internal use.
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2016/05/30 initial version
*********************************************************************/

#ifndef __DEMO_DATA_TYPE_H__
#define __DEMO_DATA_TYPE_H__

#include <map>
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

struct UserInfo
{
	CString UserID;
	CString UserCategory;
	CString UserPriority;
	CString UserName;
};

typedef std::map<int, UserInfo*> UserInfoList;

//////////////////////////////////////////////////////////////////////////
typedef enum
{
	//P2P
	CALLSTATUS =0, 
	CALLERINFO =1, 
	CALLEEINFO =2, 

	//PTT
	GRPCALLSTATUS =3, 
	GRPCALLSPERKER =4, 
	FLOORSTATUS =5,

	//Emergency
	EMERGENCYCALL =6,  
	EMERGENCYCALLER =7,  

	//Resource
	RESORCEID =8,              
	DEVICEID =9,              
	RESSELECTSTATUS =10,        
	RESASSIGNSTATUS =11,        
	RESTXSTATUS =12,            //Occupy
	RESLOGGINGSTATUS =13,       //Record
	RESDLSTATUS =14,            //Listen
	RESREGSTATUS_PROXY =15,     //Register
	RESREGSTATUS_DIRECT =16,    //
	ATTACHINGGROUP =17,         //

	//User
	USERSTATUS =18,		
	USERGPSSTATUS =19,       
	USERDLSTATUS =20,            
	USERDGNASTATUS =21,      
	VIDEODISPATCHSTATUS =22,     
	MUTESTATUS =23,          
	GRPPATCHSTATUS =24  
}BundleKey;

typedef enum {
	RESOURCE_STATUS_UNKNOWN = 4000,
	STATUS_SELECTED = 4001,    
	STATUS_UNSELECTED = 4002,  

	STATUS_ASSIGNED = 4003,    
	STATUS_DEASSIGNED = 4004,  

	STATUS_LISTENING = 4005,   
	STATUS_LISTENEND = 4006,   

	STATUS_LOGGING = 4007,      //Record
	STATUS_NOLOGGING = 4008,    //

	STATUS_INPATCH = 4009,      //
	STATUS_OUTPATCH = 4010,     //

	STATUS_REGOK = 4011,       
	STATUS_REGFAIL = 4012,      
	STATUS_FORBIDEN = 4013,    
	STATUS_NOT_FOUND = 4014,   
	STATUS_TEMP_UNAVAILABLE = 4015, 
	STATUS_RES_CONFILCT = 4016, 
	STATUS_UNAUTHORIZED = 4017,
	STATUS_LICENSE_LIMIT = 4018, 
	STATUS_PASSWORD_WRONG = 4019, 

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
	STATUS_GRPPATCH_CREATEOK = 4031,  //patch group create ok
	STATUS_GRPPATCH_CREATEFAIL = 4032, //patch group create fail
	STATUS_GRPPATCH_CANCELOK = 4033, //patch group cancel ok
	STATUS_GRPPATCH_CANCELFAIL = 4034, //patch group cancel faile
	STATUS_GRPPATCH_ADDOK = 4035, //patch group add member ok
	STATUS_GRPPATCH_ADDFAIL = 4036, //patch group add member fail
	STATUS_GRPPATCH_DELOK = 4037, //patch group del member ok
	STATUS_GRPPATCH_DELFAIL = 4038,  //patch group del member fail

	STATUS_ACCOUNT_LOCKED = 4060 // 

} ResStatusValueType;

typedef enum {
	P2Pvideocall_IND_STATUS_UNKNOWN=3000,
	/*------------Signaling status-----------*/
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
	/*------------Media status-----------*/
	P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_ERROR,  //Video RTP stream creation failed 
	P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_NOAUDIO, //Failure to establish audio streaming of video
	P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED,
	P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED,

	P2Pvideocall_IND_STATUS_SIGNAL_ERROR = 3023,            //SIP error for GW-mode
	P2Pvideocall_IND_STATUS_VIDEO_CONTROL = 3024,            //PTZ control for GW-mode

	P2Pvideocall_IND_STATUS_PICKEDUP = 3040            //picked up
} P2PVideoCallStatusType;

typedef enum 
{
	DCUSERS_RESID = 0,
	DCUSERS_STATUS,
	DCUSERS_NAME,
	DCUSERS_CATEGORY,
	DCUSERS_PRIORITY
} DCUSERS_COLUMN_E;

//res image index
typedef enum
{
	Img_camera_offline = 0,
	Img_camera_online,
	Img_dispatcher_offline,
	Img_dispatcher_online,
	Img_gwuser_offline,
	Img_gwuser_online,
	Img_pttuser_offline,
	Img_pttuser_online
} IMAGE_LIST_E;
typedef enum
{
	ID_MENU_BEGIN = 3000,
	ID_MENU_STARTREALPLAY,
	ID_MENU_STOPREALPLAY,
	ID_MENU_SNAPSHOT,
	ID_MENU_MUTE,
	ID_MENU_UNMUTE,
	ID_MENU_END
} NMRCLICK_MENU_LIST;

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

typedef std::map<CString, CString> VideoChannelList;

//////////////////////////////////////////////////////////////////////////
#define WM_INIT             (WM_USER+100)
#define WM_START_REAL_PLAY	(WM_USER+101)
#define WM_INCOMMING_VIDEO  (WM_USER+102)

#endif
