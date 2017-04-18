/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    resourcestatus_indicator.h
Author:      Kai Wang
Created:     2012/08/08 16:36
Description: Dispatch Resource Status Notifications
             Normally it's a combination of Status Type and Status Value,
             For some status type, a subclass indicator is notified
*********************************************************************/
#pragma warning(disable: 4251)
#ifndef RESOURCESTATUS_INDICATOR_H
#define RESOURCESTATUS_INDICATOR_H

#include "resourceinfo.h"
#include "uieventlist.h"
#include <list>
#include <vector>
#include "sdk_export.h"

namespace Json{
 class Value;
}

typedef  BundleKey resource_status_t;

/**
   define  status values for dispatching resources such as group, user, camera,etc
**/
typedef enum {
    RESOURCE_STATUS_UNKNOWN = 4000,
    STATUS_SELECTED = 4001,         /** reserved */
    STATUS_UNSELECTED = 4002,       /** reserved */

    STATUS_ASSIGNED = 4003,         /** group is assigned in SDK*/
    STATUS_DEASSIGNED = 4004,       /** group is deassigned in SDK*/

    STATUS_LISTENING = 4005,        /** user is being listened */
    STATUS_LISTENEND = 4006,        /** user is not being listend*/

    STATUS_LOGGING = 4007,          /** reserved*/
    STATUS_NOLOGGING = 4008,        /** reserved*/

    STATUS_INPATCH = 4009,          /** reserved*/
    STATUS_OUTPATCH = 4010,         /** reserved*/

    STATUS_REGOK = 4011,            /** user is registered*/
    STATUS_REGFAIL = 4012,          /** user register fail or not registered */
    STATUS_FORBIDEN = 4013,         /** user register fail, not allowed */
    STATUS_NOT_FOUND = 4014,        /** user register fail, not found */
    STATUS_TEMP_UNAVAILABLE = 4015, /** user register fail, locked */
    STATUS_RES_CONFILCT = 4016,     /** user register fail, conflict*/
    STATUS_UNAUTHORIZED = 4017,     /** register is to be authenticated */
    STATUS_LICENSE_LIMIT = 4018,    /** register fail, license limited */
    STATUS_PASSWORD_WRONG = 4019,   /** register fail, wrong password */

    STATUS_INVITING = 4020,         /** the resource is initializing a call*/
    STATUS_RINGING = 4021,          /** the resource is ringing */
    STATUS_TXMTING = 4022,          /** the resource is talking/transimitting in a call */
    STATUS_TXMTIDLE = 4023,         /** the resource is idle */

    STATUS_DGNAOK = 4024,           /** dynamic regrouping OK */
    STATUS_DGNAFAIL = 4025,         /** dynamic regrouping fail */
    STATUS_DGNA_UEFAIL = 4026,      /** dynamic regrouping partial OK, with some UEs fail */
    STATUS_GROUP_ACTIVATED = 4027,  /** group is activated, PTT is going on */
    STATUS_GROUP_DEACTIVATED = 4028,/** group is deactivated, PTT closed */
    STATUS_MUTE = 4029,                 /** the resource is muted */
    STATUS_UNMUTE = 4030,               /** the resource is unmuted */

    STATUS_GRPPATCH_CREATEOK = 4031,    /** create patch group OK */
    STATUS_GRPPATCH_CREATEFAIL = 4032,  /** create patch group fail */
    STATUS_GRPPATCH_CANCELOK = 4033,    /** cancel patch group OK */
    STATUS_GRPPATCH_CANCELFAIL = 4034,  /** cancel patch group fail */
    STATUS_GRPPATCH_ADDOK = 4035,       /** add member to patch group OK */
    STATUS_GRPPATCH_ADDFAIL = 4036,     /** add member to patch group fail */
    STATUS_GRPPATCH_DELOK = 4037,       /** delete member from patch group OK */
    STATUS_GRPPATCH_DELFAIL = 4038,     /** delete member from patch group fail */
    STATUS_VIDEO_CONTROL = 4039,        /** video control*/
    STATUS_HALFVOICE_ACTIVATED = 4040,  /** the resource is active, Half Voice is going on */
    STATUS_HALFVOICE_CLOSED = 4041,     /** the resource is deactive, Half Voice is closed */
    STATUS_ACCOUNT_LOCKED = 4060        /** DC account is locked */

} res_status_value_t;

class SDK_IMPORT_EXPORT ResourceStatusIndicator: public ResourceInfo
{
public:
    ResourceStatusIndicator(MSISDN resourceid);
    virtual ~ResourceStatusIndicator();
    ResourceStatusIndicator(ResourceStatusIndicator& copy);
    //void ResourceStatusIndicator::operator=(ResourceStatusIndicator& copy);

    std::string datalog_str();
    void setStatus(resource_status_t status_type, res_status_value_t status_value);
    void setAttachingGrp(MSISDN _grpID);
    resource_status_t getStatusType();
    res_status_value_t getStatusValue();
    MSISDN getAttachingGrp();

    //added from TTR3.0
    virtual bool serialize(Json::Value&);    //serialize into text format
    virtual bool parse(const Json::Value& value);
private:

    resource_status_t status_type;      //status type
    res_status_value_t status_value;    //status value for this type


    //status_type = RESTXSTATUS， STATUS_DGNAFAIL
    MSISDN attaching_grp;                  //user in this group

};

/// when status_type = USERSTATUS 时， use below derived class
#define MAX_PEERNUM_LEN 64
class SDK_IMPORT_EXPORT UserStatusIndicator: public ResourceStatusIndicator
{
public:
    UserStatusIndicator(MSISDN resourceid);
    std::string datalog_str();
    virtual ~UserStatusIndicator();
    UserStatusIndicator(UserStatusIndicator& copy);
    //UserStatusIndicator::operator=(UserStatusIndicator& copy);

    void setPeerUser(MSISDN peerid){_peerid=peerid;}
    void setDirection(int dir){_dir=dir;}
    void setCallType(int calltype){_calltype=calltype;}
    MSISDN getPeerUser(){return _peerid;}
    int getDirection(){return _dir;}
    int getCallType(){return _calltype;}
    void setEncrypt(int type){_encrypt=type;}
    int getEncrypt(){return _encrypt;}

    //added from TTR3.0
    const char* getPeerNumber(){return _peernumber;}
    void setPeerNumber(const char* peer);
    //added from TTR4.0, used in half voice call
    void setSpeakID(MSISDN speakid){ _speakid = speakid;}
    MSISDN getSpeakID(){return _speakid;}

    void setSpeakName(const char* speakname);
    const char* getSpeakName(){return _speakname;}
    virtual bool serialize(Json::Value&);    //serialize into text format
    virtual bool parse(const Json::Value& value);

    void setCallerCause(const std::string& callerCause );
    void setCalleeCause(const std::string& calleeCause );
    std::string getCallerCause() const;
    std::string getCalleeCause() const;
protected:
    MSISDN _peerid;
    int _dir;       // 1=caller， 2=callee
    int _calltype;  // 1= p2paudio, 2=p2pvideo, 3=videoupload

    //added from TTR3.0
    int _encrypt;
    char _peernumber[MAX_PEERNUM_LEN];  //peer telephone number,present in telephone_interconnect_call

    int _speakid; //used in Half Voice Call
    char _speakname[MAX_PEERNUM_LEN];//used in Half Voice Call
    std::string m_callerCause;
    std::string m_calleeCause;
};

//when status_type = VIDEOSTATUSSTATUS ,use below derived class
class SDK_IMPORT_EXPORT VideoDispatchStatusIndicator: public UserStatusIndicator
{
public:
    VideoDispatchStatusIndicator(MSISDN resourceid);
    VideoDispatchStatusIndicator(VideoDispatchStatusIndicator& copy);
    virtual ~VideoDispatchStatusIndicator();

    MSISDN getCamID();
    void setCamID(MSISDN camID);

    //added from TTR3.0
    virtual bool serialize(Json::Value&);    //serialize into text format
    virtual bool parse(const Json::Value& value);
private:
    MSISDN _camID;
};

//when status_type = USERDGNASTATUS ,use below derived class
class SDK_IMPORT_EXPORT DGNAStatusIndicator: public ResourceStatusIndicator
{
public:
    DGNAStatusIndicator(MSISDN resourceid);
    virtual ~DGNAStatusIndicator();   
    DGNAStatusIndicator(DGNAStatusIndicator& copy);
    //DGNAStatusIndicator::operator=(DGNAStatusIndicator& copy);

    int getCause();
    void setCause(int cause);

    //added from TTR3.0
    virtual bool serialize(Json::Value&);    //serialize into text format
    virtual bool parse(const Json::Value& value);
private:
    int _cause;  //DGNA failure cause
};

//when status_type = GRPCALLSTATUS ,use below derived class
class SDK_IMPORT_EXPORT GrpResourceStatusIndicator: public ResourceStatusIndicator
{
public:
    GrpResourceStatusIndicator(MSISDN resourceid);
    virtual ~GrpResourceStatusIndicator();
    GrpResourceStatusIndicator(GrpResourceStatusIndicator& copy);
    //void GrpResourceStatusIndicator::operator=(GrpResourceStatusIndicator& copy);

    MSISDN getSpeakerID();
    std::string getSpeakerName();
    int getEncrypt(){return _encrypt;}

    void setSpeakerID(MSISDN);
    void setSpeakerName(std::string);
    void setEncrypt(int type){_encrypt=type;}

    //added from TTR3.0
    virtual bool serialize(Json::Value&);    //serialize into text format
    virtual bool parse(const Json::Value& value);

    std::string getSpeaker(); //hwd
    void setSpeaker(std::string); //hwd

private:
    MSISDN _speakerID;
    std::string _speakerName;
    int _encrypt;
    std::string speaker; //hwd
};



typedef std::list<MSISDN> MUTE_LIST;

//when status_type = MUTESTATUS 或 status_type = UNMUTESTATUS时， use below derived class
class SDK_IMPORT_EXPORT MuteStatusIndicator: public ResourceStatusIndicator
{
public:
    MuteStatusIndicator(MSISDN resourceid);
    virtual ~MuteStatusIndicator();
    MuteStatusIndicator(MuteStatusIndicator& copy);

    void setP2PUserLst(MUTE_LIST p2plist){_p2plist=p2plist;}
    void setGroupLst(MUTE_LIST grouplist){_grouplist=grouplist;}
    void setVideoUserLst(MUTE_LIST videolist){_videolist=videolist;}

    MUTE_LIST getP2PUserLst(){return _p2plist;}
    MUTE_LIST getGroupLst(){return _grouplist;}
    MUTE_LIST getVideoUserLst(){return _videolist;}

    //added from TTR3.0
    virtual bool serialize(Json::Value&);    //serialize into text format
    virtual bool parse(const Json::Value& value);
private:

    MUTE_LIST _p2plist;   //  usr for p2p
    MUTE_LIST _grouplist;   // GroupId
    MUTE_LIST _videolist;  //  usr for videoupload or videodispatch
};

//when status_type = GRPPATCHSTATUS ,use below derived class
class SDK_IMPORT_EXPORT GroupPatchStatusIndicator: public ResourceStatusIndicator
{
public:
    GroupPatchStatusIndicator(MSISDN resourceid);
    virtual ~GroupPatchStatusIndicator();
    GroupPatchStatusIndicator(GroupPatchStatusIndicator& copy);

    int getCause();
    int getMember();
    void setCause(int cause);
    void setMember(int member);

    //added from TTR3.0
    virtual bool serialize(Json::Value&);    //serialize into text format
    virtual bool parse(const Json::Value& value);
private:
    int _cause;         /// fail cause
    int _memberId;      /// member Id
};
//when status_type = VIDEOCONTROL ,use below derived class
class SDK_IMPORT_EXPORT VideoControlStatusIndicator: public ResourceStatusIndicator
{
public:
    VideoControlStatusIndicator(MSISDN resourceid);
    virtual ~VideoControlStatusIndicator();
    VideoControlStatusIndicator(VideoControlStatusIndicator& copy);

    virtual bool serialize(Json::Value&);    //serialize into text format
    virtual bool parse(const Json::Value& value);

	//0: without MDC ;other: with MDC
    void setVideoBridgeType(VIDEO_BRIDGE_t type);
    VIDEO_BRIDGE_t getVideoBridgeType();

	void setVideoBridgeDstID(int wallid);
	/**
		\brief get the Wall ID for ON-Wall or OFF-Wall
	*/
    int getVideoBridgeDstID();

	/**
		\brief set VIDEOCONTROL status type and value
		if VideoControl_t is CONTROL_CODE_WALL, then value=0 means ON-Wall, value = 1 means OFF-Wall
	*/
	void setVideoControlStatus(VideoControl_t controlType ,int value);
	int getVideoControlValue();
	VideoControl_t getVideoControlType();
private:
	int wallID;
	VideoControl_t control_type;
	int control_value;
	VIDEO_BRIDGE_t videoBridge_type;
    
};
#endif // RESOURCESTATUS_INDICATOR_H
