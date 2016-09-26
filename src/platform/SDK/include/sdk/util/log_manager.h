/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    log_manager.h
Author:      zhangjiansheng
Created:     2013/02/26 10:00
Description: 日志管理类
*********************************************************************/
#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <ace/Basic_Types.h>
#include"operationmanager.h"
#include "displaymanager.h"
#include "license_control.h"
#include "sdk_export.h"

/*zmq 对应的日志操作类型*/
typedef enum
{
    ADD_HOTLINE_LOG_TYPE = 1,
    DELE_HOTLINE_LOG_TYPE,
    MODIFY_PASSWORD_LOG_TYPE
}ZMQ_LOG_TYPE;
/**
    辅助校验输入
 **/
class SDK_IMPORT_EXPORT log_manager
{
public:
    log_manager();
    static void init();
    static int record_op_log(Operation_t op_type,ACE_INT32 op_result,MSISDN op_objid);

private:

    static int log_AddOp(ACE_INT32 op_type,ACE_INT32 op_level,std::string op_desc,ACE_INT32 op_result,MSISDN op_objid);
};

class OpLog_t
{
public:
    std::string op_name;
    std::string op_desc;//
};

//display_manager_logwrapper 负责把displaymanager所有通知的事件记录到DATALOG
class display_manager_logwrapper : public DisplayManager
{
public:
    display_manager_logwrapper();
    virtual ~display_manager_logwrapper();
    virtual void notifyGroupStatus(GrpDspInfo * grpInfo);

    virtual void notifyResourceStatus(ResourceStatusIndicator * resourceStatus);

    virtual void notifyP2pcallStatus(P2pcallStatusIndicator * callStatus);

    virtual void notifyP2pvideocallStatus(P2pvideocallStatusIndicator * callStatus);

    virtual void notifyUserStatus(ResourceStatusIndicator * resourceStatus);
    virtual void notifyUserSdsStatus(SdsMessageIndicator * sdsmsg);
    virtual void notifyModuleStatus(DCModuleStatusIndicator * moduleStatus);


    virtual void notifySMS(SdsMessageIndicator * sdsmsg);


    virtual void notifyProvisionChange(ProvChangeIndicator* );


    virtual void notifyProvisionAllResync(ProvChangeIndicator* );


    virtual void notifyStatusAllResync(ResourceStatusIndicator* );

    void setDisplayManager(DisplayManager* dm) {innerDM = dm;}

private:
    DisplayManager* innerDM; //目前只支持wrapper一个displaymanager
};

//license_contorl_logwrapper 负责把license_contorl所有通知的事件记录到DATALOG
class license_contorl_logwrapper : public LicenseControl
{
public:
    license_contorl_logwrapper();
    virtual ~license_contorl_logwrapper();
    virtual bool is_license_accept(ServInfo *Sinfo);

    void setLicenseControl(LicenseControl* lc) {innerLC = lc;}

private:
    LicenseControl* innerLC; //目前只支持wrapper一个license_contorl
};


//connection_contorl_logwrapper 负责把connection_contorl所有通知的事件记录到DATALOG
class Connection_contorl_logwrapper : public ConnectionControl
{
public:
    Connection_contorl_logwrapper();
    virtual ~Connection_contorl_logwrapper();
    virtual bool get_Connection_info(ServInfo *Sinfo,ConnInfo *Cinfo);
    void setConnectionControl(ConnectionControl* cc) {innerCC = cc;}
    ConnectionControl* getConnectionControl() {return innerCC;}
private:
    ConnectionControl* innerCC; //目前只支持wrapper一个connection_contorl
};

typedef std::map<Operation_t, OpLog_t> OpMap;//操作码和操作信息体对应关系

typedef std::map<Operation_t, int> OptypeMap;//操作码和zmq日志操作类型对应关系

typedef std::map<Operation_t, OpLog_t>::const_iterator log_iterator;
typedef std::map<Operation_t, int>::const_iterator Optype_iterator;

static OpLog_t DC_LOGIN_t = {"login","DC user login"};
static OpLog_t DC_LOGOUT_t = {"logout","DC user logout"};
static OpLog_t DC_EXIT_t = {"exit","DC user exit"};
static OpLog_t PTT_DIAL_t = {"press PTT","DC user launch a ptt request for speak in a PTT call"};
static OpLog_t PTT_RELEASE_t = {"release PTT","DC user launch a ptt release request in a PTT call"};
static OpLog_t PTT_HANGUP_t = {"exit from group call","A group member exit from the ongoing group call."};
static OpLog_t SIP_INIT_t = {"init sip stack","init sip stack"};
static OpLog_t P2P_DIALOUT_t = {"point-to-point call","DC user launch a point-to-point call"};
static OpLog_t P2P_RECV_t = {"answering a call","DC user answer an incoming call for ringing"};
static OpLog_t P2P_HANGUP_t = {"hanging up a call","DC user hang up an incoming call"};
static OpLog_t P2P_REJECT_t = {"reject a call","DC user reject an incoming call"};
static OpLog_t P2P_VIDEO_DIAL_t = {"video call","DC user launch a video call"};
static OpLog_t P2P_VIDEO_RECV_t = {"answering a video upload","DC user answer a video upload"};
static OpLog_t P2P_VIDEO_HANGUP_t = {"hanging up a video upload","DC user hang up a video upload"};
static OpLog_t P2P_VIDEO_MONITOR_t = {"video monitor","DC user launch a video upload"};
static OpLog_t P2P_VIDEO_DISPATCH_t = {"video dispatch","DC user launch a video dispatch"};
static OpLog_t P2P_VIDEO_STARTCIRCDISPLAY_t = {"start video circle task","DC user start a video circle display task"};
static OpLog_t P2P_VIDEO_STOPCIRCDISPLAY_t = {"stop video circle task","DC user stop a video circle display task"};
static OpLog_t GRP_SUB_t = {"subscription to group","DC user launch a subscribe group request"};
static OpLog_t GRP_UNSUB_t = {"cancel subscription to group","DC user cancel subscription to group"};
static OpLog_t GRP_JOIN_t = {"late entry","A group member may fail to join the group call upon the initiation and join the call later."};
static OpLog_t GRP_LEAVE_t = {"exit from group call","DC user exit from the ongoing group call."};
static OpLog_t GRP_BREAKOFF_t = {"forced release group call service","DC user force to release group call service"};
static OpLog_t GRP_QUERY_t = {"query current group","query current group where the DC user is located."};
static OpLog_t P2P_BREAKOFF_t = {"release a conversation of an intra-office user ","DC user force to release conversation of an intra-office user for some reasons."};
static OpLog_t P2P_BREAKIN_t = {"break-in p2p service","DC user break in the conversation between two common users."};
static OpLog_t DC_PZT_CONCTROL_t = {"PTZ(pan tilt zoom)","Wall or ceiling mounted cameras that are able to move their lenses horizontally (pan) and vertically (tilt), as well as change their focal length between near and far (zoom in/zoom out)."};
static OpLog_t VOL_MUTE_t = {"mute voice","mute voice, DC user who participate in the conference subsequently cannot speak."};
static OpLog_t VOL_UNMUTE_t = {"cancel mute voice","DC user who have the speak right and do not mute actively can speak after conference mute is cancelled."};
static OpLog_t BATCH_CFG_t = {"batch configuration","configure a batch of data or tasks."};
static OpLog_t SDS_SEND_t = {"send short message","send short message,a short message contains texts, digits, or binary non-text data."};
static OpLog_t SDS_EXPORT_t = {"export sds log or attach","export sds log or attach for restore."};
static OpLog_t REC_START_t = {"start recording","start recording,a function for recording the speech or video of a user into a file."};
static OpLog_t REC_STOP_t = {"stop recording","stop recording,a function for recording the speech or video of a user into a file."};
static OpLog_t VWALL_START_t = {"dispatch video to video wall","dispatch video to video wall,a video wall consists of multiple computer monitors, video projectors, or television sets tiled together contiguously or overlapped in order to form one large screen."};
static OpLog_t VWALL_STOP_t = {"close video from video wall","close video from video wall,a video wall consists of multiple computer monitors, video projectors, or television sets tiled together contiguously or overlapped in order to form one large screen."};
static OpLog_t DL_START_t = {"start discreet listening","start discreet listening,a kind of service which allows a special user to listen to other users in the radio without being recognized."};
static OpLog_t DL_STOP_t = {"stop discreet listening","stop discreet listening,a kind of service which allows a special user to listen to other users in the radio without being recognized."};
static OpLog_t GIS_SUBSCRIBE_t = {"subscription to GIS","DC user launch a subscribe gis request to teminal."};
static OpLog_t DGNA_CREATE_t = {"perform DGNA","perform DGNA, Dynamic Group Number Assignment (DGNA) refers to regrouping users dynamically at any moment."};
static OpLog_t DGNA_CANCEL_t = {"cancel DGNA","cancel DGNA, Dynamic Group Number Assignment (DGNA) refers to regrouping users dynamically at any moment."};
static OpLog_t TIC_DIALOUT_t = {"Telephone-Interconnect-Call","DC user launch a Telephone-Interconnect-Call."};
static OpLog_t TIC_HANGUP_t = {"hang up Telephone-Interconnect-Call","DC user hang up a Telephone-Interconnect-Call."};
static OpLog_t TEMPGRP_CREATE_t = {"Create tempGroup ","The PTT subscriber selects several subscribers to form a group."};
static OpLog_t TEMP_PTT_t = {"Initiates a tempGroup call","Invites the other members to join the group session,When the session is over, the system will delete the tempGroup automatically."};
static OpLog_t P2P_VIDEO_REJECT_t = {"Reject a video upload ","DC user Reject a video upload."};
static OpLog_t P2P_VIDEO_SWITCHBACK_t = {"video Switch from GIS to DC ","Switching receiving video signals from GIS to DC"};
static OpLog_t P2P_VIDEO_PREVIEW_t = {"Preview on GIS","A function that allows users to view scenes of video on GIS"};
static OpLog_t P2P_ENVIRONMENT_LISTEN_t = {"start environment listening","a kind of service which allows a special user to listen environment voice of other users in the radio without being recognized."};
static OpLog_t P2P_VIDEO_RECV_PARA_t = {"answering a video upload","DC user answer a video upload"};
static OpLog_t TEMPGRP_DELETE_t = {"Delete a tempGroup","DC user delete a tempGroup"};
static OpLog_t MUTE_CONTROL_t = {"Mute a group or user"};
static OpLog_t GRP_SUB_BATCH_t = {"subscribe multiple groups in a batch"};
static OpLog_t GRP_UNSUB_BATCH_t = {"unsubscribe multiple groups in a batch"};
static OpLog_t VOLUME_CONTROL_t = {"change volume of a group or user"};
static OpLog_t P2P_TRANSFER_t = {"transfer call from one user to another by Console"};
static OpLog_t P2P_VIDEO_DISPATCH_DELETE_t = {"delete a user from video dispatch list"};
static OpLog_t DOWNLOAD_ATTACH_FILE_t = {"download attachment of a multimedia message"};
static OpLog_t PCHGRP_CREAGE_t = {"create a patch group"};
static OpLog_t PCHGRP_CANCEL_t = {"delete a patch group"};
static OpLog_t PCHGRP_ADD_t = {"add new members to a patch group"};
static OpLog_t PCHGRP_DEL_t = {"delete members from a patch group"};
static OpLog_t PTT_EMERGENCY_t = {"start an emergency group call"};
static OpLog_t ADD_HOTLINE_t = {"add a hotline#","DC user add a hotline#"};
static OpLog_t DELE_HOTLINE_t = {"delete a hotline#","DC user delete a hotline#"};
static OpLog_t ADD_PRESET_MESSAGE_t = {"add a preset message#","DC user add a preset message#"};
static OpLog_t MOD_PRESET_MESSAGE_t = {"modify a preset message#","DC user modify a preset message#"};
static OpLog_t DEL_PRESET_MESSAGE_t = {"delete a preset message#","DC user delete a preset message#"};
static OpLog_t MODIFY_PASSWORD_t = {"modify password#","DC user modify password#"};
static OpLog_t VIDEO_CLOSE_t = {"close all gis video#","DC close all gis video#"};


#endif // LOG_MANAGER_H
