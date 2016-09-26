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
description :    �������Ͷ���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 ��ʼ�汾
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
	/*-------------------������ϢԪ�����ڵ����������Ƶ�������p2pcallstatus_indicator.cpp-----------*/
	CALLSTATUS =0, //���״̬
	CALLERINFO =1, //������
	CALLEEINFO =2, //������

	/*--------------------������ϢԪ�������������grpcallstatus_indicator.cpp----------------*/
	GRPCALLSTATUS =3, //���״̬
	GRPCALLSPERKER =4, //���������
	FLOORSTATUS =5,    //��Ȩ״ָ̬ʾ

	/*--------------------������ϢԪ�����ڽ������У� ��emergency_indicator.cpp----------------*/
	EMERGENCYCALL =6,  //��������ָʾ
	EMERGENCYCALLER =7,  //����������

	/*--------------------������ϢԪ��������Դ״̬����,��resourcestatus_indicator.cpp----------------*/
	RESORCEID =8,              //��ԴID
	DEVICEID =9,               //��Ƶ����ʱ�Ĵ��ں�
	RESSELECTSTATUS =10,        //��Դѡ��״ָ̬ʾ�� ���ڶ�GUI����
	RESASSIGNSTATUS =11,        //��Դָ��״ָ̬ʾ�� ������̨�Ƿ�������Դ��
	RESTXSTATUS =12,            //��Դռ��״ָ̬ʾ
	RESLOGGINGSTATUS =13,       //��Դ¼��״ָ̬ʾ
	RESDLSTATUS =14,            //��Դ����״ָ̬ʾ
	RESREGSTATUS_PROXY =15,     //��Դע��״ָ̬ʾ, ����ע��(���������û����ն˼�Ⱥע��)
	RESREGSTATUS_DIRECT =16,    //��Դע��״ָ̬ʾ, ֱ��ע��(�ն�ҵ��ע�ᣬ����Ƶ)
	ATTACHINGGROUP =17,         //��Դ��������

	/*--------------------������ϢԪ�������û�״̬����;��userstatus_indicator.cpp----------------*/
	USERSTATUS =18,		//�û�״ָ̬ʾ�� һ����BCC֪ͨ
	USERGPSSTATUS =19,       //�û�GPS״ָ̬ʾ
	USERDLSTATUS =20,            //�û�����״ָ̬ʾ
	USERDGNASTATUS =21,      //�û���̬����״̬
	VIDEODISPATCHSTATUS =22,     //��Ƶ�ַ�״̬
	MUTESTATUS =23,          //����״̬
	GRPPATCHSTATUS =24  //�ɽӲ���״̬
}BundleKey;

typedef enum {
	RESOURCE_STATUS_UNKNOWN = 4000,
	STATUS_SELECTED = 4001,    //��Դѡ��״̬
	STATUS_UNSELECTED = 4002,  //��Դȥѡ��״̬

	STATUS_ASSIGNED = 4003,    //��Դָ��״̬-
	STATUS_DEASSIGNED = 4004,  //��Դȥָ��״̬-

	STATUS_LISTENING = 4005,   //��Դ������״̬
	STATUS_LISTENEND = 4006,   //��Դ��������״̬

	STATUS_LOGGING = 4007,      //��Դ¼����״̬
	STATUS_NOLOGGING = 4008,    //��Դδ����¼��

	STATUS_INPATCH = 4009,      //��Դ���ɽ�
	STATUS_OUTPATCH = 4010,     //��Դ����ɽ�

	STATUS_REGOK = 4011,        //��Դע��
	STATUS_REGFAIL = 4012,      //��Դȥע��
	STATUS_FORBIDEN = 4013,     //��Դδ��Ȩ 500
	STATUS_NOT_FOUND = 4014,    //��Դ������ 404
	STATUS_TEMP_UNAVAILABLE = 4015, //��Դ��ʱ������ 480
	STATUS_RES_CONFILCT = 4016, //��Դ��ͻ 450
	STATUS_UNAUTHORIZED = 4017, //δ��Ȩ 401
	STATUS_LICENSE_LIMIT = 4018, //License���� 503
	STATUS_PASSWORD_WRONG = 4019, //������� 403

	//�����ĸ�״̬��˳�����Ҫ�� ��Ϊ isValidTransition������Ҫ�ο�˳��
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
	STATUS_GRPPATCH_CREATEOK = 4031,  //�ɽ��鴴��OK
	STATUS_GRPPATCH_CREATEFAIL = 4032, //�ɽ��鴴��ʧ��
	STATUS_GRPPATCH_CANCELOK = 4033, //�ɽ���ȡ��OK
	STATUS_GRPPATCH_CANCELFAIL = 4034, //�ɽ���ȡ��ʧ��
	STATUS_GRPPATCH_ADDOK = 4035, //�ɽ�����ӳ�ԱOK
	STATUS_GRPPATCH_ADDFAIL = 4036, //�ɽ�����ӳ�ԱFAIL
	STATUS_GRPPATCH_DELOK = 4037, //�ɽ���ɾ����ԱOK
	STATUS_GRPPATCH_DELFAIL = 4038,  //�ɽ���ɾ����ԱFAIL

	STATUS_ACCOUNT_LOCKED = 4060 // �˺ű�����

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
	/*--------ý����仯��֪ͨ-------------*/
	PTT_IND_STATUS_TX_BEGIN,        //�����Ȩ�仯
	PTT_IND_STATUS_TX_IDLE,         //�����Ȩ����
	PTT_IND_STATUS_MEDIAPLANE_ERROR,
	PTT_IND_STATUS_EMERGENCY_BEGIN,
	PTT_IND_STATUS_EMERGENCY_UPDATE,
	PTT_IND_STATUS_LICENSE_LIMIT
} Speaker_Status_t;

typedef enum {
	P2P_IND_STATUS_UNKNOWN=2000,
	/*--------������״̬--------*/
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
	/*--------ý����״̬--------*/
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
	/*------------������״̬-----------*/
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
	/*------------ý����״̬-----------*/
	P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_ERROR,  //��ƵRTP������ʧ��
	P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_NOAUDIO, //��Ƶ����������ʧ��
	P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED,
	P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED
} P2pvideocall_Status_t;

typedef enum {
	SDS_MODULE = 1,   //����ҵ��ģ��
	SMS_MODULE,       //����ҵ��ģ��
	AUDIO_MODULE,    //����ҵ��ģ��
	VIDEO_MODULE,  //��Ƶҵ��ģ��
	GIS_MODULE,  //GISҵ��ģ��
	SIP_MODULE,  //����ͨ��ģ��
	RTP_MODULE,   //ý��ͨ��ģ��
	RING_MODULE,   //���� add by lwj
	SVC_MODULE, //���Ȼ�����
	UNKOWN_MODULE = 999
} dc_module_t;

typedef enum {
	RESOURCE_STATUS_NORMAL = 0, //״̬����
	SDS_OVERFLOW,    //������
	SDS_DISCONNECT,  //DC�Ͷ��ŷ���������
	SDS_CONNECT,     //DC�Ͷ��ŷ���������
	SDS_SEND_SUCCESS, //���ŷ��ͳɹ�
	SDS_SEND_FAIL, //���ŷ���ʧ��
	FOLDER_IS_EMPTY,  //�����ļ���Ϊ��
	GIS_LICESNCE_UPDATE,  //GIS License���
	GIS_LICENSCE_EXPIREDAY, //license �����������ѣ�ֻʣ7��ʱ֪ͨ������Ϊ��ʣn�죬����Ϊ�Ѿ�����n��
	SIP_DISCONNECT,       //DC��BCC����
	SIP_RECONNECT,       //DC��BCC�ָ�
	SIP_KEEPALIVE_REJECT, //BCC�ܾ�DC����������
	ATTACH_UPLOAD_FAIL,   //�����ϴ�ʧ��
	ATTACH_DOWNLOAD_FAIL,  //��������ʧ��
	SIP_START_RUNNING,     //BCC ����
	KICK_OFF,      //BCC �߳�����̨֪ͨ,��Ϊ����ͬ�˺ŵ�¼
	ASYN_CALLBACK_RETURN,   //�첽���÷���ֵ
	PASSWORD_CHANGE,    //����Ա�����޸�
	MODULE_DISCONNECT,
	MODULE_RECONNECT,
	SIP_KA_FAILONCE,       //DC��BCC����һ��
	SIP_KA_OKONCE,       //DC��BCC�ָ�һ��
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
