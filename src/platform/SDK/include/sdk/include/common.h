/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    appcontext.h
Author:      Wu yu wei
Created:     2012/07/10 10:00
Description:
*********************************************************************/
#ifndef DC_COMMON_H
#define DC_COMMON_H

#define WAIT_TIMER 5	//事件检测时间间隔ms，DTS2013030603146
#define REG_TIMER  500*10	    //注册时间间隔ms
#define EXPIRE_TIME 3600            //SIP Expire time

#define EXPIRE_CHECK_TIME 2            //定时器检测时间
#define SIP_KEEPALIVE_TIME 60           //每60s做一次SIP Keepalive检测

/*audio payload type*/
#define AMR_PAYLOAD_TYPE 114
#define AMR_PAYLOAD_TYPE_475 115
#define PCMU_PAYLOAD_TYPE 0
#define PCMA_PAYLOAD_TYPE 8
#define TEL_PAYLOAD_TYPE 101

/*video payload type*/
#define H264_PAYLOAD_TYPE 102

#define AMR_MODE    "mode-set="
#define AMR_475_MODE   0
#define AMR_1220_MODE  7
#define INVALID_RSC   -1

/*txt_ptt_req :: type*///清SDK告警 [-Wwrite-strings
#define PTT_REQ_TYPE_REQUEST  const_cast<char *>("request")
#define PTT_REQ_TYPE_ACCEPT   const_cast<char *>("accept")
#define PTT_REQ_TYPE_WAITING  const_cast<char *>("waiting")
#define PTT_REQ_TYPE_REJECT   const_cast<char *>("reject")
#define PTT_REQ_TYPE_RLEASE  const_cast<char *>("release")

#define EMERGENCY_CALL  const_cast<char *>("emergency_call")
#define PTT_STATUS_IND  const_cast<char *>("Session_Ind")

#define VIDEO_DISPATCH_TYPE  const_cast<char *>("videodispatch")
#define VIDEO_DIAL_TYPE   const_cast<char *>("videocall")
#define VIDEO_UPLOAD_TYPE  const_cast<char *>("videoupload")

/*discreet listening*/
#define DISCREET_LISTEN_CALL  const_cast<char *>("discreet_listening_call") 

/*half_duplex*/
#define HALFDUPLEX_PTP_CALL  const_cast<char *>("halfduplex_ptp")

/*txt_ptt_release :: reason*/
#define PTT_RELEASE_TIMEOUT  const_cast<char *>("timeout")
#define PTT_RELEASE_FORCE   const_cast<char *>("force")
#define PTT_RELEASE_IDLE  const_cast<char *>("idle")
#define PTT_RELEASE_ACTIVE  const_cast<char *>("active")

/*SDS config info*/
//#define DC_SDS_PATH  "data/mms";
#define INBOX        "inbox.xml"
#define OUTBOX       "outbox.xml"

#define DC_SDS_HTTP_PROTOCOL "https";  /* 3.0  https, 2.1 http*/
#define DC_SDS_HTTPSERVER_PATH "upload";
#define DC_SDS_HTTPSERVER_PORT "9091"; /* https @9091, http @9090*/

/*XMPP 心跳检测周期*/
#define XMPP_HEART_BEAT_CHECK_PERIOD   120

/*断链检测周期*/
#define XMPP_CONNECT_CHECK_PERIOD   5

/*No ack检测周期*/
#define XMPP_NO_ACK_CHECK_PERIOD   10

/*MIC 热插拔检测周期*/
#define SERIAL_CHECK_PERIOD   1

/*iconv转码路径最大长度*/
#define MAX_PATH_LENGTH   1000

/*inconv转码字符最大长度*/
#define MAX_CHARACTOR_LENGTH   2000

/* max number of patch group*/
#define MAX_PATCH_GRP_NUM 16

/*REC :: type,Call-Info的格式定义如下
Call-Info: <0>;type=call_type;id1=user1;id2=user2
其中type为本次通话类型：single_voice_call表示语音点呼，single_video_call表示视频点呼，
single_emergency_call表示语音紧急点呼，group_call表示组呼，
group_emergency_call表示紧急组呼，video_upload表示视频回传。
*/
#define REC_P2P_CALL   const_cast<char*>("single_voice_call")
#define REC_P2P_VIDEO_CALL  const_cast<char*>("single_video_call")
#define REC_P2P_EMERGENCY_CALL  const_cast<char*>("single_emergency_call")
#define REC_GROUP_EMERGENCY_CALL const_cast<char*>("group_emergency_call")
#define REC_GROUP_CALL const_cast<char*>("group_call")
#define REC_VIDEO_UPLOAD const_cast<char*>("videoupload")

/*
分辨率fmt：1080P、720P、D1、CIF、QCIF
INVITE中的语义：
fmt = D1，只填写1个表示只需要D1分辨率，其它分辨率都不要 —— 2.1终端、3.0服务器、3.0终端
fmt = D1，只填写1个表示低于D1的分辨率都支持，即支持D1/CIF/QCIF —— 2.1服务器
fmt = 720P/D1，填写多个表示只支持明确携带的，即只支持720P/D1，不支持CIF/QCIF —— 3.0服务器、3.0终端
分辨率协商后，从交集中选择Invite里携带的排在最前面的分辨率为准，在200OK中携带给对端，如果协商没有交集，接收Invite方直接拒绝，调度机支持转码。
*/
/*3.0对于固定摄像头缺省分辨率如下*/
#define DEFAULT_FIX_CEMERA_FMT const_cast<char*>("1080P/720P/D1/CIF/QCIF")

#define FMT_SEPARATOR_SYMBOL const_cast<char*>("/")

/*DC调度台只有在视频回传界面可以手动触发和停止录像，其它场景都只能触发和停止手动录音*/
//录制类型
#define REC_VOICE_TYPE    0 //录音
#define REC_VIDEO_TYPE    1 //录像

//录制业务类型
#define REC_P2P_TYPE             0  //点对点业务
#define REC_GROUP_TYPE           1  //集群业务
#define REC_UPOALD_VIDEO_TYPE    2  //视频上传业务

#define SIP_OK 200


//清SDK告警 [-Wwrite-strings
#define CALLINFO_EXT_TYPE  const_cast<char *>("type")
#define CALLINFO_EXT_ID1  const_cast<char *>("id1")
#define CALLINFO_EXT_ID2  const_cast<char *>("id2")
#define CALLINFO_EXT_VPN1  const_cast<char *>("vpn1")
#define CALLINFO_EXT_VPN2  const_cast<char *>("vpn2")
#define CALLINFO_EXT_GRPSTATUS  const_cast<char *>("groupstatus")
#define CALLINFO_EXT_STATUS  const_cast<char *>("status")
#define CALLINFO_EXT_SPEAKER  const_cast<char *>("speaker")
#define CALLINFO_EXT_SPEAKERNAME  const_cast<char *>("speakername")
#define CALLINFO_EXT_MUTE const_cast<char *>("mute")
#define CALLINFO_EXT_USRCONFIRM const_cast<char *>("user_confirm")
#define CALLINFO_EXT_FMT const_cast<char *>("fmt")
#define CALLINFO_EXT_CAMERA const_cast<char *>("camera")
#define CALLINFO_EXT_CHANNEL const_cast<char *>("channel")
#define CALLINFO_EXT_RESULT const_cast<char *>("result")
#define CALLINFO_EXT_GROUPCALL const_cast<char *>("group_call")
#define CALLINFO_EXT_PRIORITY const_cast<char *>("priority")
#define CALLINFO_EXT_CALLTYPE const_cast<char *>("calltype")
#define CALLINFO_EXT_TEMPGROUPCALL const_cast<char *>("temp_groupcall")
#define CALLINFO_EXT_EMERGENCY const_cast<char *>("emergency_callInd")
#define CALLINFO_EXT_MDCSTART const_cast<char *>("mdc_start")
#define CALLINFO_EXT_KICKOFF const_cast<char *>("kickoff")
#define CALLINFO_EXT_IP const_cast<char *>("newip")
#define CALLINFO_EXT_REASON const_cast<char *>("reason")
#define CALLINFO_EXT_LOGRETRYTIME const_cast<char *>("loginretyrtime")
#define CALLINFO_EXT_LOGUNLOCKTIME const_cast<char *>("loginunlocktime")
#define CALLINFO_EXT_APPLYID const_cast<char *>("applyid") 

#define CALLINFO_EXT_ZMQ_SUB_PORT const_cast<char *>("zmq_sub_port")
#define CALLINFO_EXT_ZMQ_PORT const_cast<char *>("zmq_port")
#define VIA_RECEIVED const_cast<char *>("received")
#define CALLINFO_EXT_PTZ const_cast<char *>("PTZCapability")



#define STRING_ZERO const_cast<char *>("0")

#define STRING_COLON const_cast<char *>(":")

#define STRING_SEMICOLON const_cast<char *>(";")

#define STRING_OPEN_ANGLE const_cast<char* >("<")
#define STRING_CLOSE_ANGLE const_cast<char* >(">")
#define STRING_AT const_cast<char* >("@")
#define STRING_SPACE const_cast<char*>(" ")

#define CALLINFO_EXT_MIX  const_cast<char *>("mix_flag")
#define CALLINFO_EXT_SWITCH1  const_cast<char *>("switch1")
#define CALLINFO_EXT_SWITCH2  const_cast<char *>("switch2")

//视频格式类型
typedef enum
{
    V_CIF,
    V_QCIF,
    V_D1,
    V_720P,
    V_1080P,
    INVALID_FMT
}Fmt_type;

//音频格式类型
typedef enum {
  invalid_ptype = -1,
  amr_475_ptype,
  amr_1220_ptype,
  pcmu_ptype,
  pcma_ptype
} Sound_t;

typedef enum {
    AMR_NB = AMR_PAYLOAD_TYPE,

    PCM_U = PCMU_PAYLOAD_TYPE,
    PCM_A = PCMA_PAYLOAD_TYPE,
    TEL_EVENT = TEL_PAYLOAD_TYPE,
    NO_PAYLOAD = INVALID_RSC
} Payload_t;

typedef enum
{
    AMR_475, //4.75k优先
    AMR_1220, //12.2k优先
    NO_AMR    //不用AMR
}PRIORITY_t;
typedef enum
{
    USUAL_PROXY = 0,        //usual logon
    SPECIAL_PROXY = 1,      //special logon,it's for disney
    UNKNOWN_LOGON
}LogOn_Proxy_t;
typedef enum
{
    NATIVE_BRIDGE =0,
    UNKNOWN_VIDEO_BRIDGE
}VIDEO_BRIDGE_t;

typedef enum {
    CONTROL_CODE_MUTE,   	///video play-back control type
    CONTROL_CODE_WALL,		///video on-wall control type
    CONTROL_CODE_PTZ,		///PTZ control type
	UNKNOWN_VIDEO_CONTROL = 999
}VideoControl_t;
#define MAX_TELENUMBER_LENGTH 40    //maxium telephone number length, on GUI is constrained to 32 digits
#define LOGIN_FIELD_LENG  20        //field length used in resourceinfo
#define MAX_URL_LENGTH 255

#define TDTECH_CONSOLE_UA const_cast<char *>("#Dispatch Console 4.0")
#define SDK_CONSOLE_UA const_cast<char *>("#SDK Console 4.0")
#define SDK_GATEWAY_UA const_cast<char *>("#SDK GateWay 4.0")
#define SIP_GATEWAY_UA const_cast<char *>("#SIP GateWay 4.0")
#define SDK_RM_UA const_cast<char *>("#SDK_RM Console 4.0")

//去定义stun_udp里的bool定义
#ifdef bool
#undef bool
#endif
#ifdef true
#undef true
#endif
#ifdef false
#undef false
#endif

#endif // DC_COMMON_H
