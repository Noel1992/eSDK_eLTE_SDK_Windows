
#pragma warning(disable: 4251)
#ifndef PROVISION_HELPER_H
#define PROVISION_HELPER_H

#include <list>
#include <map>
#include <vector>
#include <string>
#include "sdk/include/sdk_export.h"

namespace Json{
class Value;
}

class ACE_Null_Mutex;
class ProvListener;
class UserStatusIndicator;
class GrpDspInfo;
class ProvChangeIndicator;
class DisplayManager;
typedef GrpDspInfo GroupStatusIndicator;
class LicenseListener;
typedef int MSISDN;

#define ALIAS_MAX_LENGTH 64
#define STMSG_MAX_LENGTH 64

typedef enum {
    ENUM_UE=0,
    ENUM_MOBILE_STATION=1

}Ue_Model_t;


typedef enum{
    ENUM_UE_VERSION=0
}Ue_Version_t;


typedef enum{
    ENUM_UE_NULL=0,
    ENUM_UE_QCIF=1,
    ENUM_UE_CIF=2,
    ENUM_UE_D1=3,
    ENUM_UE_720p=4,
    ENUM_UE_1080P=5
}Ue_Ability_t;


typedef enum {
    DISPATCHER = 0,     /** Dispatcher user type*/
    FIXEDCAMERA = 1,    /** fixed camera user type*/
    PSTNUSER = 2,       /** PSTN Gateway user type */
    TETRAUSER = 3,      /** TETRA Gateway user type */
    PLMNUSER = 4,       /** PLMN Gateway user type */
    EXTERNALPTT = 5,    /** External PTT user type */
    SDKUSER = 6,        /** SDK_GW Gateway user type*/
    APPUSER = 7,        /** APP in public network user type */
    UELOGINUSER = 8,    /** UE login user type */
    PTTUSER = 9,        /** LTE PTT user type */
    ALLTYPEUSER = 50,   /** all users */
    DECUSER = 100,      /** video decoder user type */
    OTHERUSER = 255     /** reserved */
}User_Category_t;

typedef enum {
    LOCAL_USER =0,      /** local eMDC user */
    EXTERNAL_USER = 1   /** external eMDC user */

}User_Location_t;

typedef enum {
    GRP_ALLBROADCAST =0,        /** network broadcasting group type*/
    GRP_GENERAL = 1,            /** common group type */
    GRP_AREABROADCAST = 2,      /** area broadcasting group type */
    GRP_EXTERNAL=8,             /** external eMDC group type */
    GRP_DYNAMICGRP = 9,         /** dynamic group type */
    GRP_ALLTYPE   = 10          /** all groups */
}Group_Category_t;

typedef enum {
    MEMBER_GROUP = 0,
    MEMBER_USER = 1

}Group_Member_t;

typedef enum {
    PREV_BREAKOFF =1,           /** break off */
    PREV_BREAKIN = 2,           /** break in */
    PREV_LISTEN = 4,            /** Discrete listen */
    PREV_MONITOR = 8,           /** video surveillance */
    PREV_SDSEXPORT = 16,        /** SDS export */
    PREV_ENVLISTEN = 32,        /** environment listen */
    PREV_VWALL = 64,            /** video on wall */
    PREV_VIMONITOR_NO_CONFIRM = 128,         /** video without confirm */
    PREV_GIS_SUB = 256,             /** GIS subscribe */
    PREV_GIS_TRACK = 512,           /** GIS track query */
    PREV_LOCAL_RECAUDIO = 1024,     /** DC audio recording */
    PREV_LOCAL_RECVIDEO = 2048,     /** DC video recording  */
    PREV_RECLISTVIEW = 4096,        /** DC query record files */
    PREV_RECREPLAY = 8192,          /** DC playback record files */
    PREV_RECDOWNLOAD = 16384        /** DC download record files */

}Dc_Previlege_t;

typedef int Prov_Result_t;  /** provision result: 0=success， negative=failure */

typedef enum {
    GISEVENT_PERSONDOWN =1, /** Person down event */
    GISEVENT_EMERGENCY =2   /** emergency event */
}Gis_Event_t;

enum Rec_Call_Dir_t
{
    ENUM_CALLER,    /** caller */
    ENUM_CALLEE     /** callee */
};

/**
  \brief call type condition when query recording files
*/
enum Rec_Call_Type_t
{
    ENUM_REC_UNKNOW_CALL_TYPE = -1,     /** not set, shall be avoided */
    ENUM_REC_P2P_AUDIO = 0,             /** peer to peer audio */
    ENUM_REC_P2P_VIDEO = 1,             /** peer to peer video */
    ENUM_REC_VIDEO_UPLOAD = 2,          /** video suveillance */
    ENUM_REC_PTT = 3,                   /** trunking group audio */
    ENUM_REC_BELOW_RESERVED = 4,        /** below are all reserved for internal use only */
    ENUM_REC_VIDEO_SCHEDULE = 5,        /** video recording by schedule */
    ENUM_REC_VIDEO_UEOFFLINE = 6        /** video recording by UE offline */
};

enum Rec_Media_Type_t
{
    ENUM_MEDIA_AUDIO,
    ENUM_MEDIA_VIDEO
};

/**
 \brief define resource types in eMDC, being used in query condition
*/
typedef enum {
    RES_GROUP,
    RES_USER,
    RES_CAMERA,
    RES_DEC,
    RES_DC,
    RES_UEABILITY,
    RES_PATCHGROUP,
    RES_DYNAMICGROUP,
    RES_USER_INGROUP,
    RES_MEMBER_INPATCH,
    RES_MEMBER_INDGNA,
    RES_VPN,
    RES_EXTERNALUSER,
    RES_ALONEUSER,
    RES_STMSG,
    RES_GISIPC,
    RES_GISSUB,
    RES_USERREGISTERINFO,
    RES_IPCATTRIBUTE,
    RES_VIDEOLOSSRETRANSMISSION
}DC_RESOURCE_t;
/**
  \brief User Register Infomation
*/
typedef enum{
    PROXY_UNREGISTERED = 0,
    PROXY_REGISTER = 1
}USER_REGISTERINFO_t;
/**
  \brief MRS node types, add in TTR4.0
*/
typedef enum{
    BDC =1,
    VGW =2,
    MRS =3,
    ECN =4,
    AFEE =5,
    UBPNODE_OTHER = 6,
    SDS = 7
}UBP_NODE_t;
/**
  \brief define this for External_Isdn
*/
typedef enum
{
    Group = 0,
    User = 1,
    unknown = 10
}UserType_t;

typedef enum{
    UNKNOWN = 0, //not configureBox Camera
    DomeCamera = 1,
    BoxCamera = 2
}CameraShape_t;

/**
 \brief a dummy class used as BASE for condition and return value in ProvisionManager API
*/
class SDK_IMPORT_EXPORT BaseInfo
{
public:
    virtual ~BaseInfo(){;}
    /**
     \brief serialize into json text format
    */
    virtual const char* serialize(Json::Value& )=0;
    /**
     \brief de_serialize from json text format
    */
    virtual bool parse(const Json::Value&){return false;}
    //[todo]move to cpp
    std::string datalog_str();    
};
typedef BaseInfo BaseCondition;

class SDK_IMPORT_EXPORT NoCondition : public BaseCondition
{
    virtual const char* serialize(Json::Value& ){return NULL;}
};

typedef std::vector<BaseInfo*> BaseInfoVec;

/**
    \brief setup emergency group  call
*/
class SDK_IMPORT_EXPORT ProvEmgrpmber : public BaseCondition
{
public:
    int EmencygrpID;
    int Emencygrptime;
    ProvEmgrpmber ();
    ProvEmgrpmber (const ProvEmgrpmber& emgrpid);
    void operator =(const ProvEmgrpmber& emgrpid);
    void update(const ProvEmgrpmber& emgrpid);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &root);
};
typedef std::map<MSISDN,ProvEmgrpmber*> ProvEmgrpmberMap;

/********************************setup pttlinkcamera begin by ywx43679****************************/

class SDK_IMPORT_EXPORT ProvPTTLinkCamera : public BaseCondition
{
public:
    std::string PttIsdn;    //ptt user Isdn
    std::string CameraId;      //bindding camera ID
    ProvPTTLinkCamera ();
    ProvPTTLinkCamera (const ProvPTTLinkCamera& pttlinkcamera);
    void operator =(const ProvPTTLinkCamera& pttlinkcamera);
    void update(const ProvPTTLinkCamera& pttlinkcamera);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &root);
};

typedef std::map<std::string,ProvPTTLinkCamera*> ProvPTTLinkCameraMap;

/******************************setup pttlinkcamera end by ywx43679************************/

/**
    \brief mobile terminal capability information
*/
class SDK_IMPORT_EXPORT ProvUEAblity : public BaseInfo
{
public:
    MSISDN userid;                  /** user ID */
    Ue_Model_t UEModel;             /** model type */
    Ue_Version_t UESoftwareVersion; /** softare version */
    Ue_Version_t UAppVersion;       /** APP version */
    int PreCamera;                  /** front camera resolution */
    int PostCamera;                 /** post camera resolution */
    int ExtCamera;                  /** usb camera resolution */

    ProvUEAblity();
    ProvUEAblity(const ProvUEAblity& uety);
    void operator =(const ProvUEAblity& uety);
    void update(const ProvUEAblity& uety);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &root);

};

typedef std::map<MSISDN,ProvUEAblity*> ProvUEAbilityMap;

/**
    \brief condition when query recording files
*/
class SDK_IMPORT_EXPORT RECQueryInfo : public BaseInfo
{
public:
    Rec_Call_Type_t call_type;  /** call type, see definition of Rec_Call_Type_t */
    MSISDN caller;              /** caller number */
    MSISDN callee;		/** callee number*/
    MSISDN resourceid;          /** resourceid, such as group id, camera id, caller or callee id */
    std::string start_sec;	/** recording start time, "yyyy-mm-dd hh:mm:ss" */
    std::string end_sec;	/** recording end time */
    RECQueryInfo();
    std::string toConditionStr();

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value&);
};
/**
    \brief condition when query recording tasks
*/
class SDK_IMPORT_EXPORT RECQueryTaskInfo : public RECQueryInfo
{
public:
    std::string dcId;           /** user ID*/
    std::string strcaller;      /** caller number*/
    std::string strcallee;      /** callee number*/
    std::string strresourceid;  /** camera id*/
    std::string groupid;        /** group id*/
    std::string joinid;         /** current speaking id*/
    MSISDN taskNum;             /** wish to query the numbers of tasks*/
    MSISDN conditionNum;        /** query condition numbers, you can ignore it*/
    MSISDN offset;              /** start querying position*/
    RECQueryTaskInfo();
    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);
};
/**
    \brief condition when query recording files
*/
class SDK_IMPORT_EXPORT RECQueryFileInTask : public RECQueryInfo
{
public:
    std::string uid;            /** user ID*/
    std::string taskId;         /** task ID, this should be set by 'UserRecTaskInfo'*/
    MSISDN offset;              /** start querying position*/
    MSISDN fileNum;             /** wish to query the numbers of files*/
    RECQueryFileInTask();
    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);
};
class SDK_IMPORT_EXPORT RECQueryPttInfo : public RECQueryInfo
{
public:
    std::string dcid;           /** user ID*/
    std::string taskId;         /** task ID, this should be set by 'UserRecTaskInfo'*/
    RECQueryPttInfo();
    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);
};
class SDK_IMPORT_EXPORT RECQueryUEInfo : public RECQueryInfo
{
public:
    std::string dcid;           /** user ID*/
    MSISDN offset;              /** start querying position*/
    MSISDN filenum;             /** wish to query the numbers of files*/
    RECQueryUEInfo();
    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);
};
class SDK_IMPORT_EXPORT GISTrackQueryInfo : public BaseInfo    //add by wsw
{
public:
    std::string begintime;      /** query GisTrack start time, which is milliseconds from 1970-01-01 00::00*/
    std::string endtime;        /** query GisTrack end time, which is milliseconds from 1970-01-01 00::00*/
    unsigned int maxcount;      /** max count about query gisTrack number*/
    unsigned int policy;        /** query policy: 0 means the oldest track points;1 means the newest track points;2 means the sample track points */
    unsigned int limit;         /** limit server return track points, it was set 1000 in code*/
    unsigned int checkpoint;    /** start querying position*/
    MSISDN isdn;                /** terminal ID, which reported GPS infomation*/
    MSISDN dcid;                /** DConsole ID*/

    GISTrackQueryInfo();

    virtual const char* serialize(Json::Value &value); /** serialize into json text format*/
    virtual bool parse(const Json::Value&);    /** de_serialize from json text format*/
};

/**
    \brief result when query recording files
*/
class SDK_IMPORT_EXPORT UserRecFileInfo : public BaseInfo
{
public:
    int call_type;              /** call type,see definition of Rec_Call_Type_t */
    MSISDN caller;              /** caller number */
    MSISDN callee;		/** callee number*/
    MSISDN resourceid;          /** resourceid, such as group id, camera id, caller or callee id */
    std::string start_sec;	/** recording start time, "yyyy-mm-dd hh:mm:ss" */
    std::string end_sec;	/** recording end time */
    std::string urlFTP;         /** URL for HTTP, note: the name is urlFTP but actually value is URL of http */
    std::string urlRTSP;        /** URL for RTSP */

    UserRecFileInfo();

    virtual const char* serialize(Json::Value &value);    /** serialize into text format*/
    virtual bool parse(const Json::Value&);
};

typedef std::list<UserRecFileInfo*> UserRecFileInfoList;

/**
  \brief result when query recording tasks
  \note different query type return different results, so please handle these results by query type
*/
class SDK_IMPORT_EXPORT UserRecTaskInfo : public BaseInfo
{
public:
    MSISDN ret;                 /** return 0 means success,return 1 menas failed*/
    MSISDN queryType;           /** query type,see definition of Rec_Call_Type_t*/
    MSISDN taskTotalNum;        /** total record tasks numbers*/
    MSISDN taskNum;             /** current record task counts*/
    std::string  taskId;        /** record task ID*/
    std::string  callType;      /** call type*/
    std::string  caller;        /** caller number */
    std::string  callee;        /** callee number */
    std::string  startTime;     /** recording start time, "yyyy-mm-dd hh:mm:ss" */
    std::string  endTime;       /** recording end time */
    std::string  emergency;     /** emergency flag: 0 means usual,1 means emergency*/
    std::string  groupNo;       /** group number*/
    std::string  ipcIndex;      /** local camera ID */
    std::string  nodeId;        /** node ID which include 'ipcIndex'*/
    std::string  recordUeId;    /** resource terminal ID*/
    MSISDN startTimeSec;        /** recording start time, seconds from 1970-01-01 00:00:00 */
    MSISDN endTimeSec;          /** recording end time, seconds from 1970-01-01 00:00:00 */
    UserRecTaskInfo();
    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value&);
};
typedef std::vector<UserRecTaskInfo*> UserRecTaskInfoList;

/**
  \brief result when query recording files in one recording task
*/
class SDK_IMPORT_EXPORT UserRecFileInTask : public BaseInfo
{
public:
    std::string dcid;               /** DConsole ID*/
    std::string taskid;             /** task ID*/
    MSISDN fileTotalNum;            /** total number of record files in one task*/
    MSISDN fileNum;                 /** current record file counts*/
    MSISDN errorcode;               /** error info*/
    std::string fileId;             /** only when query type is 'ENUM_REC_VIDEO_UEOFFLINE' it means the name of uploaded record file, others mean session ID*/
    std::string fileSeqNo;
    std::string beginTime;          /** recording start time, "yyyy-mm-dd hh:mm:ss" */
    std::string endTime;            /** recording end time */
    std::string downloadHttpUrl;    /** URL for HTTP,this is useless,please use 'downloadUrlFor40'*/
    std::string downloadUrlFor40;   /** URL for HTTP, note: you can download record file from eMRS*/
    std::string playHttpUrl;        /** URL for HTTP, note: you can play the record files which is in eMRS by using it*/
    std::string rtspUrl;            /** URL for RTSP */
    MSISDN beginTimeSec;            /** recording start time, seconds from 1970-01-01 00:00:00 */
    MSISDN endTimeSec;              /** recording end time, seconds from 1970-01-01 00:00:00 */
    UserRecFileInTask();
    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value&);
};
typedef std::vector<UserRecFileInTask*> UserRecFileInTaskInfoList;

/**
  \brief result when query recording PTT files
*/
class SDK_IMPORT_EXPORT UserRecPttInfo : public BaseInfo
{
public:
    std::string dcid;               /** DConsole ID*/
    std::string taskid;             /** task ID*/
    std::string eventIdx;           /** event index*/
    MSISDN isRelease;               /** release flag, true means released*/
    std::string owner;              /** current speaking ID*/
    MSISDN timeSec;                 /** the owner begin snatching group_right time*/
    MSISDN timeUsec;
    MSISDN releas_time_sec;         /** the owner begin releasing group_right time*/
    MSISDN releas_time_usec;
    MSISDN is_emergency;              /** indicate the current group call is emergency or not,0:not emergency,1:emergency, default is 0*/
    UserRecPttInfo();
    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value&);
};
typedef std::vector<UserRecPttInfo*> UserRecPttInfoList;

/**
  \brief result when to get UBPNode Infomation
*/
class SDK_IMPORT_EXPORT UBPNodeInfo :public BaseInfo
{
public:
    MSISDN NodeID;          /** node ID*/
    std::string NodeName;   /** node name*/
    UBP_NODE_t NodeType;    /** node type, see definition of 'UBP_NODE_t' */
    std::string AddrIPV4;   /** Address of IPV4*/
    std::string OuterAddrIPV4; /** Address of IPV4 for NAT*/
    MSISDN SipPort;         /** port for SIP*/
    MSISDN ISDN;            /** User number*/
    MSISDN OuterPort1;      /** when NodeType = MRS，表示MRS在公网侧的Http端口，用于UE视频录制文件上传。
                                NodeType = SDS，表示SDS在公网侧的XMPP端口。
                                其它类型，不需要填写*/
    MSISDN OuterPort2;      /** NodeType = SDS，表示SDS在公网侧的HTTPS端口（短彩信附件下载端口）。
                                NodeType = MRS，表示MRS在公网侧的RTSP端口，用于DC播放文件。
                                其它类型，不需要填写*/
    UBPNodeInfo();
    UBPNodeInfo(const UBPNodeInfo& ubpnodeinfo);
    virtual const char* serialize(Json::Value &);
    virtual bool parse(const Json::Value &);
};
typedef std::vector<UBPNodeInfo*> UBPNodeInfoList;

/**
  \brief result when query recording file which uploaded from terminal
*/
class SDK_IMPORT_EXPORT UserRecUEInfo : public BaseInfo
{
public:
    std::string dcid;               /** DConsole ID*/
    MSISDN fileTotalNum;            /** total number of record files */
    std::string fileid;             /** session ID*/
    std::string downloadHttpUrl;    /** URL for HTTP,this is useless,please use 'downloadUrlFor40'*/
    std::string downloadUrlFor40;   /** URL for HTTP, note: you can download record file from eMRS*/
    std::string playHttpUrl;        /** URL for HTTP, note: you can play the record files which is in eMRS by using it*/
    std::string rtspUrl;            /** URL for RTSP */
    std::string start_sec;          /** recording start time, "yyyy-mm-dd hh:mm:ss" */
    std::string end_sec;            /** recording end time, "yyyy-mm-dd hh:mm:ss" */
    std::string recordUeId;         /** resource terminal ID*/
    UserRecUEInfo();
    virtual const char* serialize(Json::Value &);
    virtual bool parse(const Json::Value &);
};
typedef std::vector<UserRecUEInfo*>UserRecUEInfoList;


class SDK_IMPORT_EXPORT ProvGisCfg : public BaseInfo
{
public:
    MSISDN userid;                  /** mobile ISDN */
    unsigned int reportSwitch;      /** reserved */
    unsigned int reportPeriod;      /** minimum report period */
    unsigned int reportCapability;  /** reserved */
    unsigned int reportDistance;    /** minimum report distance */

    ProvGisCfg();
    ProvGisCfg(const ProvGisCfg& gc);
    void operator =(const ProvGisCfg& gc);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value&);
};
typedef std::vector<ProvGisCfg*> ProvGisCfgList;

class SDK_IMPORT_EXPORT ProvGisIpc : public BaseInfo
{
public:
    MSISDN userid;
    double latitude;
    double longitude;
    double altitude;
    std::string address;

    ProvGisIpc();
    ProvGisIpc(const ProvGisIpc& gisipc);
    void operator =(const ProvGisIpc& gisipc);
    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value&);
};
typedef std::vector<ProvGisIpc*> ProvGisIpcList;

class SDK_IMPORT_EXPORT SdsStMsgCfg : public BaseInfo
{
public:
    int id;
    std::string stmsg;

    SdsStMsgCfg();
    SdsStMsgCfg(const SdsStMsgCfg& dc);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);
};
typedef std::vector<SdsStMsgCfg*> SdsStMsgCfgList;

/**
    \brief Dispatch Console information
*/
class SDK_IMPORT_EXPORT DcProperty : public BaseInfo
{
public:
    MSISDN dcid;        /** DC ID*/
    int priviledge;     /** DC priviledge, see definition in Dc_Previlege_t */
    int role;           /** reserved */

    std::string Alias;  /** DC alias */

    DcProperty();
    DcProperty(const DcProperty& dc);
    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);
};
typedef std::vector<DcProperty*> DcList;

/**
    \brief Range information
*/

class SDK_IMPORT_EXPORT RangeInfo :public BaseInfo
{
public:
    int rangid;         /** range ID */
    int Rangecategory;  /** reserved */
    MSISDN From;        /** range start */
    MSISDN To;          /** range end */
    std::string Alias;  /** range alias */

    RangeInfo();
    RangeInfo(const RangeInfo& range);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);
};
typedef std::vector<RangeInfo*> DcRanges;

class SDK_IMPORT_EXPORT Hotline : public BaseInfo
{
public:
    MSISDN DcId;
    int HotIndex;
    MSISDN Number;
    std::string Alias;

    Hotline();
    Hotline(const Hotline& user);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);
};
typedef std::vector<Hotline*> DcHotlines;

class SDK_IMPORT_EXPORT TerminalLinkage : public BaseInfo
{
public:
    MSISDN DcId;
    int    CallNum;
    int    LinkageNum;
    int    videoOnWallId;
    int    chnNo;

    TerminalLinkage(){CallNum = 0;LinkageNum = 0;videoOnWallId = 0;DcId = 0;chnNo = 0;}
    TerminalLinkage(const TerminalLinkage& user);

    virtual const char* serialize(Json::Value &);
    virtual bool parse(const Json::Value &);
};
typedef std::vector<TerminalLinkage*> DcTerminalLinkage;

/**
    \brief VPN information
*/
class SDK_IMPORT_EXPORT VpnInfo : public BaseInfo
{
public:

    int VpnId;              /** VPN ID */
    std::string VpnName;    /** VPN alias */

    VpnInfo();
    VpnInfo(const VpnInfo& vpn);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);
};
typedef std::vector<VpnInfo*> VpnList;


class SDK_IMPORT_EXPORT VpnRangeInfo : public BaseInfo
{
public:
    int VpnId;
    MSISDN From;
    MSISDN To;

    VpnRangeInfo();
    VpnRangeInfo(const VpnRangeInfo& range);

    virtual const char* serialize(Json::Value &);
    virtual bool parse(const Json::Value &);
};
typedef std::vector<VpnRangeInfo*> VpnRanges;

/**
  \brief define Group detail information,including static group and dynamic group
*/
class SDK_IMPORT_EXPORT GroupInfo : public BaseInfo
{
public:
    MSISDN grpid;                   /** group ID */
    Group_Category_t grpcategory;   /** group type, can be broadcasting，common，dynamic, etc */
    int grppriority;                /** group priority */
    std::string grpname;            /** group alais */
    int grpstate;                   /** group state: 0=disable, 1=enable */
    int vpnid;                      /** group VPN */
    MSISDN setupDcId;               /** group creator, valid if it's dynamic or temp group */
    GroupInfo();
    GroupInfo(const GroupInfo& user);
    void update(const GroupInfo& grp);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);

};

typedef std::vector<GroupInfo*> DcGroups;
typedef std::map<MSISDN, GroupInfo*> DcGroupsMap_T;

/**
  \brief define User detail information, including fixed user and PTT user
*/
class SDK_IMPORT_EXPORT UserInfo : public BaseInfo
{
public:
    MSISDN userid;                  /** user ID */
    User_Category_t usercategory;   /** user type, can be PTTuser， Camera, FixedLine, etc */
    int userpriority;               /** user priority */
    std::string username;           /** user alias */
    int vpnid ;                     /** user VPN */
    int vpnout;                     /** user call restriction, VPN out */
    int vpnin;                      /** user call restriction, VPN in */
    User_Location_t location;       /** user location, can be Local or External */
    UserInfo();
    UserInfo(const UserInfo& user);
    void update(const UserInfo& user);

    virtual  const char* serialize(Json::Value &value);
    virtual  bool parse(const Json::Value &root);
};

typedef std::vector<UserInfo*> DcUsers;
typedef std::map<MSISDN, UserInfo*> DcUsersMap_T;

class SDK_IMPORT_EXPORT PhotoGrpUserInfo :public BaseInfo
{
public:
    MSISDN dmgid;
    MSISDN userid;
    MSISDN grpid;

    PhotoGrpUserInfo();
    PhotoGrpUserInfo(const PhotoGrpUserInfo& user);

    virtual  const char* serialize(Json::Value &value);
    virtual  bool parse(const Json::Value &root);

};

/**
 \brief Users in Group Information
*/
class SDK_IMPORT_EXPORT GrpUserInfo : public BaseInfo
{
public:
    MSISDN userid;                      /** member ID, per membertype，can be group ID or user ID */
    MSISDN grpid;                       /** group ID, can be common group or dynamic group */
    int userpriorityInGrp;              /** user priority in group */
    Group_Member_t memberType;          /** member type ,1=user in common group， 0=group in dynamic group */

    GrpUserInfo();
    GrpUserInfo(const GrpUserInfo& user);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &root);

};

/**
 \brief Patch Group Information
*/
class SDK_IMPORT_EXPORT PatchGroupInfo : public BaseInfo
{
public:
    MSISDN GroupNumber;     /** patch group ID */
    MSISDN SetupDcId;       /** patch group creator */
    int PGPriority;         /** patch group priority */
    int DcPatchIndex;       /** patch group index in DC */
    std::string PGName;     /** patch group alias */
    MSISDN VPNID;           /** patch group VPN */

    PatchGroupInfo();
    PatchGroupInfo(const PatchGroupInfo& info);

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &root);
};

/**
 \brief members in patch group information
*/
class SDK_IMPORT_EXPORT PatchGroupMemberInfo : public BaseInfo
{
public:
    MSISDN GroupNumber;     /** patch group ID */
    MSISDN MemberGroupId;   /** member group ID */

    PatchGroupMemberInfo();
    PatchGroupMemberInfo(const PatchGroupMemberInfo& info);

    virtual const char* serialize(Json::Value &value);
    virtual bool  parse(const Json::Value &root);
};


typedef std::vector<RangeInfo*> DcRanges;

typedef std::vector <GrpUserInfo*> GrpUserList;

typedef std::vector<UserStatusIndicator*> UserStatusList;
typedef std::vector<GroupStatusIndicator*> GroupStatusList;


typedef std::map<MSISDN, PatchGroupInfo*> PatchGroupsMap;
typedef std::vector<PatchGroupInfo*> PatchGroupsList;
typedef std::vector<PatchGroupMemberInfo*> PatchGroupMembers;

/**
    \brief video output channel information (AKA video wall)
    \note VWallInfo is a subclass of UserInfo, each video output channel has a virtual MSISDN
          please use UserInfo::userid to get the MSISDN number
*/
class SDK_IMPORT_EXPORT VWallInfo:public UserInfo
{
public:
    VWallInfo();
    VWallInfo(const VWallInfo& vwall);

    int DevIndex;       /** decoder device index in the network*/
    int ChnNo;          /** video output channel index on this device */
    std::string Alias;  /** decoder alias */
    int Status;         /** decoder status 0=disable, 1=enable */

    virtual  const char* serialize(Json::Value &value);
    virtual  bool parse(const Json::Value &root);

};
typedef std::vector<VWallInfo*> VWallInfoList;

/**
    \brief return information for querying GIS subscription
*/
class SDK_IMPORT_EXPORT GisQuerySub :public BaseInfo
{
public:
    std::string UserId;     /** subscribed User ID */
    std::string UserName;   /** subscribed User Name */

    GisQuerySub();
    GisQuerySub(const GisQuerySub& gis);
    void operator =(const GisQuerySub& gis);
    virtual const char* serialize(Json::Value &value);
};
typedef std::vector<GisQuerySub*> GisQuerySubList;
typedef std::vector<GisQuerySub*>::iterator GisQuerySubListIter;
/**
    \brief get user proxy register information
*/
class SDK_IMPORT_EXPORT UserRegister : public BaseInfo
{
public:
    std::string userid;                 /** user ID */
    USER_REGISTERINFO_t registerinfo;   /** user register infomation, "0" means user unregistered,"1" means user registered */

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &);

};
typedef std::vector<UserRegister*> UserRegisterList;

/**
  \brief get external isdn information
*/
class SDK_IMPORT_EXPORT External_Isdn :public BaseInfo
{
public:
    External_Isdn();
    External_Isdn(const External_Isdn& exisdn);
    ~External_Isdn();
    std::string isdn;  /** user id or group id*/
    std::string name;  /** user name or group name*/
    int vpnid;  /** user or group belonged vpnid*/
    UserType_t usertype;  /** 0:group, 1:user*/
    Group_Category_t gcategory;
    User_Category_t ucategory;

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &root);
};
typedef std::vector<External_Isdn*> External_Isdn_List;
typedef std::vector<External_Isdn*>::iterator External_Isdn_ListIter;


/**
   \brief get user proxy register information
*/
class SDK_IMPORT_EXPORT CameraAttribute : public BaseInfo
{
public:
    CameraAttribute();
    CameraAttribute(const CameraAttribute& cameraattr);
    ~CameraAttribute();
    std::string cameraid;       /** camera ID */
    CameraShape_t camerashape;  /** CAMERA shape */

    virtual const char* serialize(Json::Value &value);
    virtual bool parse(const Json::Value &){return true;}

};
typedef std::vector<CameraAttribute*> CameraAttributeList;

/**
  \brief get the value whcih can tell you whether camera support viedo loss retransmission or not
*/
 class SDK_IMPORT_EXPORT VideoLossRetransmissionInfo : public BaseInfo
 {
 public:
     VideoLossRetransmissionInfo();
     ~VideoLossRetransmissionInfo();
     int vlrid;    /** index number*/
     int Status;   /** value 0: not support, 1: support,default is 0*/
     int Rtxtime;  /** default value: 1000 millisecond, maybe from 1 to 3000 millisecond*/
     virtual const char* serialize(Json::Value &value){return NULL;}
     virtual bool parse(const Json::Value &){return true;}
 };
typedef std::vector<VideoLossRetransmissionInfo*> VideoLossRetransmissionInfoList;


#endif // PROVISION_HELPER_H
