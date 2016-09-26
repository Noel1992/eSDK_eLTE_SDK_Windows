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
description :    �������Ͷ����ࡣ�����ڲ�ʹ�õ�ö�ٺͽṹ��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_OCX_DATA_TYPE_H__
#define __ELTE_OCX_DATA_TYPE_H__


//////////////////////////////////////////////////////////////////////////
typedef int eLTE_RET;

// eLTE_Player �����붨��
// 213��eSDK��ʶ, 5101��eLTE_Player��ʶ�� 51xx���Ժ�eLTE SDK
typedef enum
{
	eLTE_ERR_SUCCESS = 0,					// �ɹ�

	//2135101001 - 2135101200 Normal ����
	eLTE_ERR_FAILED = 2135101001,			// ͨ��ʧ��
	eLTE_ERR_INVALID_PARAM,					// �Ƿ����
	eLTE_ERR_LOAD_TYPE,						// ocx load���Ͳ�֧��
	eLTE_ERR_NOT_LOGIN,						// û�е�½
	eLTE_ERR_WAIT_LOGIN_RESULT,				// �ȴ���¼����У�����û�е�¼�ɹ�

	//2135101201 - 2135101300 ����������
	eLTE_ERR_VIDEO_STATE = 2135101201,		// ��Ƶ���� ״̬�����磺���ڲ����У����ܲ��ţ�
	eLTE_ERR_AUDIO_OPEN,					// ��Ƶ���� �豸��ʧ��
	eLTE_ERR_AUDIO_CLOSE,					// ��Ƶ���� �豸�ر�ʧ��
	eLTE_ERR_AUDIO_RESET,					// ��Ƶ���� �豸����ʧ��
	eLTE_ERR_AUDIO_PREPARE,					// ��Ƶ���� Ԥ����������ʧ��
	eLTE_ERR_AUDIO_UNPREPARE,				// ��Ƶ���� ���Ԥ����������ʧ��
	eLTE_ERR_AUDIO_WRITE,					// ��Ƶ���� ������������ʧ��
	eLTE_ERR_AUDIO_SET_VOLUME,				// ��Ƶ���� ��������ʧ��
	eLTE_ERR_AUDIO_GET_VOLUME,				// ��Ƶ���� ��ȡ����ʧ��
	eLTE_ERR_AUDIO_OUT_BUFFER,				// ��Ƶ���� ���Ż�������	
	eLTE_ERR_AUDIO_DECODE,					// ��Ƶ���� ����ʧ��

	//2135101301 - 2135101400 �ļ���������
	eLTE_ERR_CONFIG_FILE_LOAD = 2135101301,	// ����ini�����ļ�ʧ��
	eLTE_ERR_CONFIG_FILE_READ,				// ��ȡini�����ļ�ʧ��
	eLTE_ERR_CONFIG_FILE_WRITE,				// д��ini�����ļ�ʧ��
	eLTE_ERR_LOG_FILE_OPEN,					// ��/������־�ļ�ʧ��

	//2135101401 - 2135101500 XML��������
	eLTE_ERR_XML_FIND_ELEM = 2135101401,	// ����XML�ڵ�ʧ��
	eLTE_ERR_XML_GET_ELEM_VALUE				// ��ȡXML�ڵ�ʧ��

} eLTE_RET_ERROR_CODE_E;

//////////////////////////////////////////////////////////////////////////
// �汾����
typedef enum
{
	eLTE_VERSION_INVALID = 0,		// �Ƿ��汾
	eLTE_VERSION_OCX,				// OCX �汾
	eLTE_VERSION_SDK				// SDK �汾
} eLTE_VERSION_TYPE_E;

//////////////////////////////////////////////////////////////////////////
// ��������
typedef enum
{
	eLTE_LANG_INVALID = 0,			// �Ƿ�����
	eLTE_LANG_zh_CN,				// ����
	eLTE_LANG_en_US,				// Ӣ��
	eLTE_LANG_es_ES					// ��������
} eLTE_LANG_CATEGORY_E;

//////////////////////////////////////////////////////////////////////////
// eLTE_Player ����ģʽ
typedef enum
{
	eLTE_LOAD_INVALID = 0,
	eLTE_LOAD_OPERATE_MGR,			// ����Ҫ���ز��ִ�й������ʱʹ��1
	eLTE_LOAD_MEDIA_PLAYER			// ����Ϊ������ģʽʹ��ʱʹ��2
} eLTE_LOAD_TYPE_E;

//////////////////////////////////////////////////////////////////////////
// ץͼ����
typedef enum
{
	eLTE_SNAPSHOT_INVALID = 0,
	eLTE_SNAPSHOT_JPG,				// JPG ��ʽ
	eLTE_SNAPSHOT_BMP				// BMP ��ʽ
} eLTE_SNAPSHOT_FORMAT_E;

//////////////////////////////////////////////////////////////////////////
// �����������¼�
typedef enum
{
	eLTE_PLAYER_HIDE = 0,					// OCX ��������������
	eLTE_PLAYER_SHOW,						// OCX ������������ʾ
	eLTE_PLAYER_FULLSCREEN,					// OCX ������ȫ��
	eLTE_PLAYER_NORMALSCREEN,				// OCX �������˳�ȫ��
	eLTE_PLAYER_MUTE,						// OCX ����������
	eLTE_PLAYER_UNMUTE,						// OCX ������ȡ������
	eLTE_PLAYER_REVERSE,					// OCX �������л���ͷ(XML����Ϊ��Ƶ����)
	eLTE_PLAYER_SNAPSHOT,					// OCX ������ץͼ(XML����ΪͼƬ����·��)
	eLTE_PLAYER_POSITION,					// OCX ����������䶯(XML����Ϊ����)
	eLTE_PLAYER_TITLE						// OCX ���������ڱ���(XML����Ϊ���ڱ���)
} eLTE_PLAYER_EVENT_ID_E;

//////////////////////////////////////////////////////////////////////////
// �¼�����
typedef enum
{
	EVENT_NOTIFY_USER_STATUS = 0,			// �豸״̬�仯�¼�֪ͨ
	EVENT_NOTIFY_P2P_VIDEO_CALL_STATUS,		// ��ʼʵ���¼�֪ͨ
	EVENT_NOTIFY_RESOURCE_STATUS,			// Ⱥ���ϵ״̬�仯�¼�֪ͨ
	EVENT_NOTIFY_PROVISION_CHANGE,			// �豸�������ñ��֪ͨ�¼�
	EVENT_NOTIFY_PROVISION_ALLRESYNC,		// �Զ�������������֪ͨ�¼�
	EVENT_NOTIFY_P2P_CALL_STATUS,			// ���״̬�仯�¼�
	EVENT_NOTIFY_GROUP_STATUS,				// ���״̬�仯�¼�
	EVENT_NOTIFY_MODULE_STATUS,				// ģ�����״̬�¼�
	EVENT_NOTIFY_GIS_REPORT,                // �ն�GIS��Ϣ�ϱ��¼�
	EVENT_NOTIFY_GIS_STATUS,                // �ն˶���״̬�ϱ��¼�
	EVENT_NOTIFY_SDS_REPORT,                // �����ϱ��¼�
	EVENT_NOTIFY_SDS_RETCODE                // ���ŷ����Ƿ�ɹ��¼�
} eLTE_EVENT_ID_E;


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

	/*--------------------������ϢԪ������Emergency call�� ��emergency_indicator.cpp----------------*/
	EMERGENCYCALL =6,  //Emergency callָʾ
	EMERGENCYCALLER =7,  //Emergency call��

	/*--------------------������ϢԪ��������Դ״̬����,��resourcestatus_indicator.cpp----------------*/
	RESORCEID =8,              //��ԴID
	DEVICEID =9,               //��Ƶ����ʱ�Ĵ��ں�
	RESSELECTSTATUS =10,        //��Դѡ��״ָ̬ʾ�� ���ڶ�GUI����
	RESASSIGNSTATUS =11,        //��Դָ��״ָ̬ʾ�� ������̨�Ƿ�������Դ��
	RESTXSTATUS =12,            //��Դռ��״ָ̬ʾ
	RESLOGGINGSTATUS =13,       //��Դ¼��״ָ̬ʾ
	RESDLSTATUS =14,            //��Դ����״ָ̬ʾ
	RESREGSTATUS_PROXY =15,     //��Դע��״ָ̬ʾ, ����ע��(��������user ���ն˼�Ⱥע��)
	RESREGSTATUS_DIRECT =16,    //��Դע��״ָ̬ʾ, ֱ��ע��(�ն�ҵ��ע�ᣬ����Ƶ)
	ATTACHINGGROUP =17,         //��Դ��������

	/*--------------------������ϢԪ������user ״̬����;��userstatus_indicator.cpp----------------*/
	USERSTATUS =18,		//user ״ָ̬ʾ�� һ����BCC֪ͨ
	USERGPSSTATUS =19,       //user GPS״ָ̬ʾ
	USERDLSTATUS =20,            //user ����״ָ̬ʾ
	USERDGNASTATUS =21,      //user ��̬����״̬
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
	STATUS_TEMP_UNAVAILABLE = 4015, //resource not available now 480
	STATUS_RES_CONFILCT = 4016, //resource conflict 450
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
	STATUS_GRPPATCH_ADDOK = 4035, //�ɽ������memberOK
	STATUS_GRPPATCH_ADDFAIL = 4036, //�ɽ������memberFAIL
	STATUS_GRPPATCH_DELOK = 4037, //�ɽ���ɾ��memberOK
	STATUS_GRPPATCH_DELFAIL = 4038,  //�ɽ���ɾ��memberFAIL

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
	PTT_IND_STATUS_TX_IDLE,         //���Right to speak
	PTT_IND_STATUS_MEDIAPLANE_ERROR,
	PTT_IND_STATUS_EMERGENCY_BEGIN,
	PTT_IND_STATUS_EMERGENCY_UPDATE,
	PTT_IND_STATUS_LICENSE_LIMIT,
	PTT_IND_STATUS_SIGNAL_ERROR = 1017
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

#define WM_ELTE_CUSTOM_MSG_BEGIN	(WM_USER+100)
#define WM_RECEIVED			        (WM_ELTE_CUSTOM_MSG_BEGIN+1)	// �Է�����
#define WM_VIDEORECEIVED            (WM_ELTE_CUSTOM_MSG_BEGIN+2)    // �ն���Ƶ�ش�
#define WM_SHOW_COMMING_VIDEO       (WM_ELTE_CUSTOM_MSG_BEGIN+3)    // ���նԷ�������Ƶ�ش���ַ�
//#define WM_			        (WM_ELTE_CUSTOM_MSG_BEGIN+1)	// �Է�����
#define WM_ELTE_CUSTOM_MSG_END		(WM_ELTE_CUSTOM_MSG_BEGIN+100)

#endif
