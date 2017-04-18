/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    UIParameter.h
Author:      zhangjiansheng
Created:     2012/07/26 16:00
Description: Define specific parameters for different dispatching operations
*********************************************************************/
#pragma warning(disable: 4251)
#ifndef UIPARAMETER_H
#define UIPARAMETER_H

#include <list>
#include "resourceinfo.h"
#include "common.h"
#include "sdk_export.h"

namespace Json{
class Value;
}

/**********************PTZ command begin*************************/
#define LIGHT_PWRON		2	/* light power on */
#define WIPER_PWRON		3	/* wiper power on */
#define FAN_PWRON		4	/* fan power on */
#define HEATER_PWRON            5       /* heater power on */
#define AUX_PWRON1		6	/* AUX1 power on */
#define AUX_PWRON2		7	/* AUX2 power on */
#define SET_PRESET		8	/* set preset */
#define CLE_PRESET		9	/* clear preset */

#define ZOOM_IN			11	/* zoom in */
#define ZOOM_OUT		12	/* zoom out */
#define FOCUS_NEAR              13      /* focus near */
#define FOCUS_FAR               14      /* focus far */
#define IRIS_OPEN               15      /* iris open */
#define IRIS_CLOSE              16      /* iris close */

#define TILT_UP			21	/* tilt up */
#define TILT_DOWN		22	/* tilt down */
#define PAN_LEFT		23	/* pan left */
#define PAN_RIGHT		24	/* pan right */
#define UP_LEFT			25	/* up and left */
#define UP_RIGHT		26	/* up and right */
#define DOWN_LEFT		27	/* down and left */
#define DOWN_RIGHT		28	/* down and right */
#define PAN_AUTO		29	/* pan automatically */

#define FILL_PRE_SEQ            30	/* reserved */
#define SET_SEQ_DWELL           31	/* reserved */
#define SET_SEQ_SPEED           32	/* reserved */
#define CLE_PRE_SEQ             33	/* reserved */
#define STA_MEM_CRUISE          34	/* reserved */
#define STO_MEM_CRUISE          35	/* reserved */
#define RUN_CRUISE		36	/* reserved */
#define RUN_SEQ			37	/* reserved */
#define STOP_SEQ		38	/* reserved */
#define GOTO_PRESET		39	/* goto a preset */
#define PTZ_STOP_CMD            99      /* stop PTZ command */

/**********************PTZ end*************************/

typedef struct SDK_IMPORT_EXPORT BaseParam{
    virtual bool parse(const Json::Value& ){return false;}
    virtual ~BaseParam(){;}

    virtual bool serialize(Json::Value& value){return false;}
	
    std::string datalog_str();

}BaseParam;

/**
  \brief Operation_t::DC_PZT_CONCTROL parameters
  \param ptz_control_code: PZT control code, defined in MACRO
  \param ptz_control_value: PTZ control value (normally a speed value)
*/
typedef struct SDK_IMPORT_EXPORT FVG_control : public BaseParam
{
    unsigned int ptz_control_code;
    unsigned int ptz_control_value;
    virtual bool parse(const Json::Value& param);
    virtual ~FVG_control(){;}
    virtual bool serialize(Json::Value &);
}FVG_control;

/**
  \brief Operation_t::P2P_VIDEO_MONITOR parameters
  \param fmtvalue: video format，can be “D1”、“CIF”、“QCIF”、“1080P”、“720P”
  \param cameratype: “0”=front camera； “1”=back camera
  \param user_confirm_type: “0”=receiver don't need confirm；“1”=receiver need confirm
  \param mutetype: “0”= with audio；“1”=without audio
  \param ptztype: RESERVED
*/
typedef struct SDK_IMPORT_EXPORT Video_UL_parameter : public BaseParam
{
    char * fmtvalue;
    char * cameratype;
    char * user_confirm_type;
    char * mutetype;
    char * ptztype;
    Video_UL_parameter():ptztype(NULL){}
    virtual bool parse(const Json::Value& param);
    virtual ~Video_UL_parameter(){;}
    virtual bool serialize(Json::Value &);
}Video_UL_parameter;

/**
  \brief Operation_t::P2P_VIDEO_RECV_PARA parameters
  \param fmt: video format, a enum value
  \param mute: “0”= with audio；“1”=without audio
*/
typedef struct SDK_IMPORT_EXPORT Video_Negotiate_parameter : public BaseParam
{
    Fmt_type fmt;
    int mute;
    int ptz;
    virtual bool parse(const Json::Value& param);
    virtual ~Video_Negotiate_parameter(){;}
    virtual bool serialize(Json::Value &);
}Video_Negotiate_parameter;


#define VIDEO_DISPATCH_NUM   32
typedef std::list<MSISDN> DISP_LIST;

/**
  \brief Operation_t::P2P_VIDEO_DISPATCH parameters
  \param fmtvalue: video format can be "NO","D1"、"CIF"、"QCIF"、"1080P"、"720P"
  \param dispatch_num: Video source ID
  \param dstviewerlist: ID List of receivers(viewers)
*/
typedef struct  SDK_IMPORT_EXPORT Video_Dispatch_parameter : public BaseParam
{
    const char * fmtvalue;
    MSISDN   dispatch_num;
    DISP_LIST  dstviewerlist;
    int  channel;

    virtual bool parse(const Json::Value& param);
    virtual ~Video_Dispatch_parameter(){;}
    virtual bool serialize(Json::Value &);
}Video_Dispatch_parameter;

/**
  \brief Operation_t::P2P_VIDEO_DISPATCH_DELETE parameters
  \param resId: video source ID
  \param userId: video receiver(viewer) ID
*/
typedef struct SDK_IMPORT_EXPORT Video_Dispatch_Delete : public BaseParam
{
    int  resId;
    int  userId;
    virtual bool parse(const Json::Value& param);
    virtual ~Video_Dispatch_Delete(){;}
    virtual bool serialize(Json::Value &);
}Video_Dispatch_Delete;

/**
  \brief Operation_t::P2P_TRANSFER parameters
  \param dcid: ID of dispatch who helps transfer
  \param speakerid: Originator user ID
  \param objectid: target user ID
*/
typedef struct SDK_IMPORT_EXPORT transfer_parameter : public BaseParam
{
    int   dcid;
    const char *speakerid;
    const char *objectid;
    virtual bool parse(const Json::Value& param);
    virtual ~transfer_parameter(){;}
    virtual bool serialize(Json::Value &);
}transfer_parameter;

typedef std::list<MSISDN> DC_LIST;

/**
  \brief Operation_t::P2P_VIDEO_STARCIRCDISPLAY parameters
  \param TimerPeriod: time period for each video source
  \param Displaylist: video source list
*/
typedef struct SDK_IMPORT_EXPORT Video_CircDisplay_parameter: public BaseParam
{
    int   TimerPeriod;
    DC_LIST  Displaylist;
    virtual bool parse(const Json::Value& param);
    virtual ~Video_CircDisplay_parameter(){;}
    virtual bool serialize(Json::Value &);
}Video_CircDisplay_parameter;

typedef std::list<std::string> AttachFileList;	/** Attach file list */

/**
* \brief Describes SDS message types.
*/
typedef enum{
    MsgNormal	= 1,			/** normal message */
    MsgStatus	= 2,			/** status message */
    MsgAlert	= 3,			/** alert message */
    MsgMMS      = 4,			/** multi-media message */
    MsgDrop     = 5,			/** man-down message */
    MsgCommand	= 6,			/** command from dispatcher */
    MsgACK      = 0xAA,			/** Acknowledgement */
    MsgSUBGIS	= 0x07,			/** GIS subscribe message*/
    MsgSUBGIS_ACK	= 17,		/** GIS subscribe ack message */
    MsgUNSUBGIS         = 8,		/** GIS unsubscribe message */
    MsgUNSUBGIS_ACK	= 18		/** GIS unsubscribe ack message */
}SDSMSG_TYPE_t;


/**
  \brief define Properties in SDS message

*/
const std::string EXMPP_MSG_TYPE	= "MsgType";
const std::string EXMPP_MSG_TEXT	= "MsgText";
const std::string MMS_FILE_NAME		= "attach";
const std::string EXMPP_INTEGER		= "integer";
const std::string EXMPP_STRING		= "string";
const std::string EXMPP_MSG_USER        = "3rdUser";
const std::string EXMPP_RETURN_CODE     = "ReturnCode";
const std::string EXMPP_CALLEE_IN_ACK   = "Callee";
const std::string ACK_STRING            = "ACK";
const std::string EXMPP_MSG_GROUP       = "MsgGroup";
const std::string EXMPP_EXTENDINFO      = "ExtendInfo";

//MAX SIZE for segemented message
const unsigned long MSG_MAX_SIZE	= 4096;

/**
    \brief SDS message types definition
*/
const std::string EXMPP_MSG_TYPE_NORMAL		= "0001";
const std::string EXMPP_MSG_TYPE_STATUS		= "0002";
const std::string EXMPP_MSG_TYPE_ALERT		= "0003";
const std::string EXMPP_MSG_TYPE_MMS		= "0004";
const std::string EXMPP_MSG_TYPE_DROP		= "0005";
const std::string EXMPP_MSG_TYPE_ACK		= "0010";
const std::string EXMPP_MSG_TYPE_FAIL		= "0011";
const std::string EXMPP_MSG_TYPE_CONFIRM_STATUS = "0009";

const std::string EXMPP_SUCCESS		                = "0"; //sending success
const std::string EXMPP_FAIL_NO_ACK		        = "0xff"; //NO ACK
/**
 \brief cause value in SDS FAIL message
*/
const std::string EXMPP_FAIL_SERVICE_UNSUPORT		= "1"; //service not support
const std::string EXMPP_FAIL_VPN_LIMIT		        = "2"; //VPN restricted
const std::string EXMPP_FAIL_TARGET_NOT_FOUND		= "3"; //target mobile not found

/**
 \brief GIS Msg Type definition
*/
const std::string EXMPP_MSG_TYPE_GISRPT		= "0006";
const std::string EXMPP_MSG_TYPE_SUBGIS         = "07";
const std::string EXMPP_MSG_TYPE_SUBGIS_ACK	= "17";
const std::string EXMPP_MSG_TYPE_UNSUBGIS       = "08";
const std::string EXMPP_MSG_TYPE_UNSUBGIS_ACK	= "18";
const std::string EXMPP_MSG_TYPE_SUBGIS_TOGW = "09";
const std::string EXMPP_MSG_TYPE_UNSUBGIS_TOGW = "10";
const std::string EXMPP_MSG_TYPE_ENCRYPT_GISRPT = "0106";
const std::string EXMPP_MSG_TYPE_EMERGENCY_ENCRYPT_GISRPT   = "0206";

/**
  \brief Operation_t::SDS_SEND parameters
  \param sdstype: SDS message types
  \param msg_body: SDS message body
  \param receiver: receiver ID of this message
  \param files: Attach file path
  \param msgid: message id，can be designated by sender，if not set SDK will assign one

  \note: for multi-media message，sender shall designate a msgid, and carry the same msgid in http post URL
*/
typedef struct SDK_IMPORT_EXPORT SDS_parameter: public BaseParam
{
    std::string sdstype;
    std::string msg_body;
    std::string receiver;
    AttachFileList files;
    std::string msgid;
    virtual bool parse(const Json::Value& param);
    virtual ~SDS_parameter(){;}
}SDS_parameter;

/**
  \brief Operation_t::GIS_SUB parameters
  \param subtype: GIS message type , MsgSUBGIS or MsgUNSUBGIS
  \param msg_body: terminal ID list，delimited by ";"; OR you can set it with "all",then eSDK will take it as SUB/UNSUB ALL terminals!
  \param subscriber: see note below

  \note: in TTR2.0 "subscriber" is formatted as the string of "subscribe@broadcast.SERVER_IP", e.g."subscribe@broadcast.10.1.1.1";
         from TTR v2.1, it can be empty
*/
typedef struct  SDK_IMPORT_EXPORT GIS_parameter: public BaseParam
{
    SDSMSG_TYPE_t subtype;
    std::string msg_body;
    std::string subscriber;
    virtual bool parse(const Json::Value& param);
    virtual ~GIS_parameter(){;}
    virtual bool serialize(Json::Value &);
} GIS_parameter;


typedef enum
{
    ON_LINE, //on_line
    OFF_LINE //off_line
}MSG_STATE_t;

/**
  \brief Operation_t::SDS_EXPORT parameters
  \param exportPath: path for the export file
*/
typedef struct SDK_IMPORT_EXPORT SDS_export_parameter: public BaseParam
{
    std::string exportpath;

}SDS_export_parameter;

/**
  \brief Operation_t::REC_START parameters
  \param task_type: 0=audio recording, 1=video recording
  \param call_type: 0=peer-to-peer audio, 1=PTT, 2=video
*/
typedef struct SDK_IMPORT_EXPORT Record_parameter: public BaseParam{
    unsigned int task_type;
    unsigned int call_type;
    virtual bool parse(const Json::Value& param);
    virtual ~Record_parameter(){;}
    virtual bool serialize(Json::Value &);
}Record_parameter;

/**
  \brief Operation_t::VWALL_START parameters
  \param DstObjId:  ID for the video output channel
  \param SrcObjId:  ID for video source which is to be projected
  \param StrFmt: Reserved
  \note: for video output channel list, please invoke ProvisionManager::getVWallsClone()
*/
typedef struct SDK_IMPORT_EXPORT VWallStart_parameter : public BaseParam
{
    MSISDN DstObjId ;
    MSISDN SrcObjId ;
    std::string StrFmt ;

    virtual bool parse(const Json::Value& param);
    virtual ~VWallStart_parameter(){;}
    virtual bool serialize(Json::Value &);
}VWallStart_parameter;


/**
  \brief Operation_t::VWALL_STOP parameters
  \param DstObjId:  ID for the video output channel
  \param SrcObjId:  ID for video source which is already projected and to be stopped
  \note:
*/
typedef struct SDK_IMPORT_EXPORT VWallStop_parameter : public BaseParam
{
    MSISDN DstObjId ;
    MSISDN SrcObjId ;

    virtual bool parse(const Json::Value& param);
    virtual ~VWallStop_parameter(){;}
    virtual bool serialize(Json::Value &);
}VWallStop_parameter;

//define structure for dynamic group and temporary group
#define MAX_GROUP_IN_DGNA 8
#define MAX_UE_IN_DGNA 200

/**
  \brief Operation_t::DGNA_CREATE parameters
  \param grpId: dynamic group ID, you can set it to zero to let system assign one ID
  \param dcId: ID of the creating DC
  \param alias: alias of this dynamic group
  \param priority: priority of this dynamic group
  \param maxPeriod: maxium period one speaker can talk
  \param grpList: group member vector, where 0 means the end of the vector
  \param userList: UE member list, where 0 means the end of the vector
  \note: when the grpList or userList doesn't reach max limit, please fill 0 at the end of the list
*/
typedef struct SDK_IMPORT_EXPORT DGNA_parameter : public BaseParam
{
    MSISDN grpId ;
    MSISDN dcId ;
    std::string alias;
    int priority;
    int maxPeriod;
    MSISDN grpList[MAX_GROUP_IN_DGNA];
    MSISDN userList[MAX_UE_IN_DGNA];

    virtual bool parse(const Json::Value& param);
    virtual ~DGNA_parameter(){;}
    virtual bool serialize(Json::Value &);
}DGNA_parameter;

typedef struct {
    int framerate;
    int packetlossrate;
    float bitrate;
} VideoStatistics_t;

//-------------------------------added from TTR3.0-----------------------------------
/**
  \brief Operation_t::MUTE_CONTROL parameters
  \param task_type: false=unmute, true=mute
  \param call_type: 0=peer-to-peer audio, 1=PTT, 2=video
*/
typedef struct SDK_IMPORT_EXPORT Mute_parameter: public BaseParam
{
    bool task_type;
    int call_type;
    virtual bool serialize(Json::Value &);
}Mute_parameter;

typedef enum
{
    SINGLE_UNMUTE,
    SINGLE_MUTE,
    ALL_UNMUTE,
    ALL_MUTE
}MUTE_type_t;


//define structure for patch group
#define MAX_MEMBER_IN_PATCH 20

/**
  \brief Operation_t::PCHGRP_CREATE/PCHGRP_ADD/PCHGRP_DEL parameters
  \param DcId: ID of the creating DC
  \param PatchGroupId: ID of this patch group,can be 0 when creating
  \param PatchGroupName: alias of the patch group
  \param memberList: member list, 0 means the end of the list, NOT FOR PCHGRP_CREATE
  \note when create a patch group please set memberList as empty, instead please fill memberList when edit patch group
*/
typedef struct SDK_IMPORT_EXPORT PCHGRP_Para: public BaseParam{
    MSISDN DcId;
    MSISDN PatchGroupId;
    std::string PatchGroupName;
    MSISDN memberList[MAX_MEMBER_IN_PATCH];

    //PCHGRP_Para();

    virtual bool parse(const Json::Value& param);
    virtual ~PCHGRP_Para(){;}
    //virtual bool serialize(Json::Value &value);
}PCHGRP_Para;

/**
  \brief Operation_t::DOWNLOAD_ATTACH_FILE parameters
  \param msgid: msgid for the MMS
  \param user: ID of the user who helps download attachemnt
  \param attach: the path which uses to save attachmen and name it
*/
typedef struct SDK_IMPORT_EXPORT Attach_parameter:public BaseParam{
    std::string msgid;
    std::string user;
    std::string attach;
}Attach_parameter;

/**
  \brief Operation_t::TIC_DIAL parameters
  \param teleNumber: the target telephone number for the Telephone_Interconnect_Call
  \note: if DC dial a LTE mobile, please use P2P_DIAL; if DC dial a public telephone please use TIC_DIAL
*/
typedef struct SDK_IMPORT_EXPORT TIC_Dial_parameter : public BaseParam
{
    char teleNumber[MAX_TELENUMBER_LENGTH];

    virtual bool parse(const Json::Value& ){return false;}
    virtual ~TIC_Dial_parameter(){;}
}TIC_Dial_parameter;

/**
  \brief Operation_t::TIC_DIAL parameters
  \param teleNumber: the target telephone number for the Telephone_Interconnect_Call
  \note: if DC dial a LTE mobile, please use P2P_DIAL; if DC dial a public telephone please use TIC_DIAL
*/
typedef struct SDK_IMPORT_EXPORT PTT_Dial_parameter : public BaseParam
{
    char applyID[MAX_TELENUMBER_LENGTH];

    virtual bool parse(const Json::Value& );
    virtual ~PTT_Dial_parameter(){;}
    virtual bool serialize(Json::Value &);
}PTT_Dial_parameter;

typedef struct SDK_IMPORT_EXPORT User2Ue_parameter: public BaseParam
{
    std::string ObjId;
    virtual bool parse(const Json::Value& );
}User2Ue_parameter;

typedef struct SDK_IMPORT_EXPORT ModPsw_parameter: public BaseParam    
{
    std::string ISDN;
    std::string username;
    std::string oldpwd;
    std::string newpwd;
    int acttype;
    int usercategory;
    virtual bool parse(const Json::Value& ){return false;}
}ModPsw_parameter;

//-------------------------------added from TTR4.0-----------------------------------
typedef struct SDK_IMPORT_EXPORT PhonePatch_parameter: public BaseParam
{
    MSISDN DCID;
    char UserID[MAX_TELENUMBER_LENGTH];
    virtual bool parse(const Json::Value& ){return false; }
}PhonePatch_parameter;

/**
  \brief Operation_t::EMERGENCY_STATUS_CONFIRM parameters
  \param ueid: terminal ID
  \param groupid: group ID
  \param msg_body: message content which is optional paramer
  \param receiver: const string "statusServer"
  \param sdstype: consult SDS message types definition
  \param msgid: unique message ID which is optional paramer
  \note:
*/
typedef struct SDK_IMPORT_EXPORT SDS_ConfirmEmergStatus_parameter: public BaseParam
{
    std::string ueid;
    std::string groupid;
    std::string msg_body;
    std::string receiver;
    std::string sdstype;
    std::string msgid;
    virtual bool parse(const Json::Value& );
    virtual bool serialize(Json::Value &);
    virtual ~SDS_ConfirmEmergStatus_parameter(){;}
}SDS_ConfirmEmergStatus_parameter;

/**
  \brief Operation_t::GIS_CFG parameters
  \param userid : ID of the terminal
  \param reportPeriod: minimum report period, shall be in {0,1,2,5,10,15,30,60}
  \param reportDistance: minium moving distance between two consecutive report
*/
typedef struct SDK_IMPORT_EXPORT UEGisCfg_parameter: public BaseParam
{
    MSISDN userid;
    unsigned int reportPeriod;
    unsigned int reportDistance;

    virtual bool parse(const Json::Value& param);
    virtual ~UEGisCfg_parameter(){;}
    virtual bool serialize(Json::Value &);
}UEGisCfg_parameter;

typedef struct SDK_IMPORT_EXPORT ReInvite_parameter: public BaseParam
{
    char LocalIP[128];
    int AudioPort;
    int VideoPort;
    virtual bool parse(const Json::Value& ){return false; }
}ReInvite_parameter;

typedef struct SDK_IMPORT_EXPORT DownLoad_MrsRecFile_parameter:public BaseParam
{
    std::string downloadHttpURL;
    std::string savePath;
    virtual bool parse(const Json::Value &){return false;}
    virtual bool serialize(Json::Value &);
    virtual ~DownLoad_MrsRecFile_parameter(){;}
}DownLoad_MrsRecFile_parameter;

/**
  \brief Operation_t::DGNA_EDIT parameters
  \param grpId :dynamic group ID
  \param dcId: ID of the creating DC
  \param addList: add member list,0 means the end if less than MAXIUM
  \param delList: delete UE member list,0 means the end if less than MAXIUM
*/
typedef struct SDK_IMPORT_EXPORT DGNA_Modify_parameter : public BaseParam
{
    MSISDN grpId ;
    MSISDN dcId ;
    MSISDN addList[MAX_UE_IN_DGNA];
    MSISDN delList[MAX_UE_IN_DGNA];

    virtual bool parse(const Json::Value& param);
    virtual bool serialize(Json::Value &);
    virtual ~DGNA_Modify_parameter(){;}
}DGNA_Modify_parameter;

typedef struct SDK_IMPORT_EXPORT QueryDevInfo_parameter: public BaseParam
{
    MSISDN dcId;
    std::string StartUserId;
    std::string EndUserId;

    virtual bool parse(const Json::Value& ){return false;}
    virtual bool serialize(Json::Value &);
}QueryDevInfo_parameter;


typedef std::list<std::string> DownloadUrlList;	/** download mrs rec file url list */
typedef std::list<std::string> SavePathList;
typedef struct SDK_IMPORT_EXPORT CancelDownload_MrsRecFile_parameter : public BaseParam
{
    DownloadUrlList download_url_list;    //download url list
    SavePathList save_path_list;
    MSISDN number; //the number of cancel dwonloading, default is 0
}CancelDownload_MrsRecFile_parameter;

 /**
   \brief Operation_t::P2P_VIDEO_HANGUP_WITHREASON parameters
   \param cause 
   \note: It was reserved for INTERNAL use only
  */
typedef struct SDK_IMPORT_EXPORT HangUpReason : public BaseParam
{
  int cause;
  HangUpReason():cause(200){}
}HangUpReason;



#endif // UIPARAMETER_H
