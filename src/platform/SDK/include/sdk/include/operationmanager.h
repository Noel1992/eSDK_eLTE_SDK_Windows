/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    operationmanager.h
Author:      Wu yu wei
Created:     2012/07/26 16:00
Description: SDK provided API to start a dispatching operation
*********************************************************************/
#ifndef OPERATIONMANAGER_H
#define OPERATIONMANAGER_H

#include "sdk_export.h"
#include "dc/dc_module_error.h"
class OperationManager;
class ResourceInfo;

#define OP_OK_ACCEPTED UBP_SUCCESS


typedef enum
{
    DC_LOGIN,           /** DC login */
    DC_LOGOUT,          /** DC logout */
    DC_EXIT,            /** reserved */
    PTT_DIAL,           /** start PTT */
    PTT_RELEASE,        /** release PTT */
    PTT_HANGUP,         /** hangup PTT */
    SIP_INIT,           /** reserved */
    P2P_DIALOUT,        /** start a P2P audio call */
    P2P_RECV,           /** anwser a P2P audio call */
    P2P_HANGUP,         /** hangup a P2P audio call */
    P2P_REJECT,         /** reject a P2P audio call */
    P2P_VIDEO_DIAL,     /** start a P2P video call */
    P2P_VIDEO_RECV,     /** answer a P2P video call or video surveillance */
    P2P_VIDEO_HANGUP,   /** hangup a P2P video call or video surveillance */
    P2P_VIDEO_MONITOR,  /** start a video surveillance */
    P2P_VIDEO_DISPATCH, /** start a video dispatch */
    P2P_VIDEO_STARTCIRCDISPLAY, /** start a video circulate */
    P2P_VIDEO_STOPCIRCDISPLAY,  /** stop a video circulate */
    GRP_SUB,            /** subscribe a group */
    GRP_UNSUB,          /** unsubscribe a group */
    GRP_JOIN,           /** late-entry a group */
    GRP_LEAVE,          /** reserved */
    GRP_BREAKOFF,       /** break off a group call */
    GRP_QUERY,          /** query user affiliated group */
    P2P_BREAKOFF,       /** break off a P2P call */
    P2P_BREAKIN,        /** break in a user, AKA interrupt */
    DC_PZT_CONCTROL,    /** camera PTZ control */
    VOL_MUTE,           /** mute a resource */
    VOL_UNMUTE,         /** unmute a resource */
    BATCH_CFG,          /** DC subscribe and late-entry a group */
    SDS_SEND,           /** send a short/multimedia message */
    SDS_EXPORT,         /** export short/multimedia message */
    REC_START,          /** start recording */
    REC_STOP,           /** stop recording */
    VWALL_START,        /** project video on wall */
    VWALL_STOP,         /** stop video on wall */
    DL_START,           /** start Discrete Listen */
    DL_STOP,            /** stop Discrete Listen */
    GIS_SUBSCRIBE,      /** subscribe a mobile for GIS report */
    DGNA_CREATE,        /** create a dynamic group */
    DGNA_CANCEL,        /** cancel a dynmaic group */
    TIC_DIALOUT,        /** dial a Telephone-Interconnect-Call */
    TIC_HANGUP,         /** hangup a Telephone-Interconnect-Call */
    TEMPGRP_CREATE,         /** create a temporary group */
    TEMP_PTT,               /** start PTT in a temporary group */
    P2P_VIDEO_REJECT,       /** reject a video call or video surveillance */
    P2P_VIDEO_PREVIEW,      /** preview video stream on GIS */
    P2P_VIDEO_SWITCHBACK,   /** switch video from GIS to DC */
    P2P_ENVIRONMENT_LISTEN, /** start environment listen */
    P2P_VIDEO_RECV_PARA,    /** answer video surveillance with negotiated parameter */
    TEMPGRP_DELETE,         /** delete a temporary group */
    MUTE_CONTROL,           /** mute control a resource */
    GRP_SUB_BATCH,          /** subscribe groups in a batch */
    GRP_UNSUB_BATCH,        /** unsubscribe groups in a batch */
    VOLUME_CONTROL,         /** volume control a resource */
    P2P_TRANSFER,           /** transfer a P2P call */
    P2P_VIDEO_DISPATCH_DELETE,  /** delete a user from video-dispatch */
    DOWNLOAD_ATTACH_FILE,       /** download attach file */
    PCHGRP_CREATE,              /** create a patch group */
    PCHGRP_CANCEL,              /** cancel a patch group*/
    PCHGRP_ADD,                 /** add members to a patch group */
    PCHGRP_DEL,                 /** delete members from a patch group */
    PTT_EMERGENCY,              /** start emergency PTT */

    ADD_HOTLINE,                /** add hotline */
    DELE_HOTLINE,               /** delete hotline */
    ADD_PRESET_MESSAGE,         /** add predefined status message */
    MOD_PRESET_MESSAGE,         /** modify predefined status message */
    DEL_PRESET_MESSAGE,         /** delete predefined status message */
    QUERY_USER2UE_BINDINFO,     /** query the terminal on which a user logs */
    MOD_PSW,                    /** modify DC password */
    VIDEO_CLOSE,                /** close all gis video*/

    /*---------------added from TTR4.0-----------------*/
    P2P_RINGBACK,                   /** ringback from SDK_GW */
    VERSION_CHECK,                  /** version check between client and server */
    EMERGENCY_STATUS_CONFIRM,       /** emergency acknowledge */
    TEMPUSER_JOIN_GRPCALL,          /** phone patch for a PTT group */
    GIS_CONFIG,                     /** config GIS report conditions for a UE */
    P2PDIAL_REINVITE,               /** start a SIP reinvite for a audio call */
    P2PVIDEO_REINVITE,              /** start a SIP reinvite for a video call */
    MRSRECFILE_DOWNLOAD,            /** start downloading a record file */
    DGNA_EDIT,                      /** modify a dynamic group members */
    QUERY_DEVICEINFO,               /** query device binding information */
    QUERY_RESOURCEINFO,             /** query resource information cross multi-zones*/
    P2P_HALFDPX_DIAL,               /** push-to-talk a half duplex p2p call */
    P2P_HALFDPX_RELEASE,            /** release push-to-talk for a half duplex p2p call */
    USER_SUB_BATCH,                 /** subscribe mobile user or camera resources in a batch */
    USER_UNSUB_BATCH,               /** unsubscribe mobile user or camera resources in a batch */
    QUERY_GROUPINFO,                /** query groups information cross multi-zones*/
    P2P_VIDEO_PUSH,                  /** push a video from someone, there're some differences from P2P_VIDEO_MONITOR. This can choose to set from who.*/
    CANCEL_DOWNLOAD_MRSRECFILE,     /** cancel downloading a record file*/
    P2P_VIDEO_HANGUP_WITHREASON     /** Reserved. if want to hangup a video, please use  P2P_VIDEO_HANGUP*/
}Operation_t;

class SDK_IMPORT_EXPORT OperationManager
{
public:
    OperationManager();    
    /**
    \brief Start a dispatching operation, without additional parameters
    \param res_info: The resource on which this dispatching operation is performed
    \param op_type: The dispatching operation type defined by Operation_t Enum

    \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
    \note: for most dispatching operations, the return value of UBP_SUCCESS only means the operation is valid and accepted by SDK
    , and after that more status or events are notified through callback interface in DisplayManager
    **/
    ACE_INT32 invokeOp(ResourceInfo * res_info, Operation_t op_type);


    /**
    \brief Start a disptaching operation, with additional parameters passed by a generic void*
    \param res_info: The resource on which this dispatching operation is performed
    \param multi_para: The additional parameters, the actual data structure depends on op_type and shall be one defined in uiparameter.h
    \param op_type: The dispatching operation type defined by Operation_t Enum

    \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
    \note: for most dispatching operations, the return value of UBP_SUCCESS only means the operation is valid and accepted by SDK
    , and after that more status or events are notified through callback interface in DisplayManager
    **/
    ACE_INT32 invokeOp_multipara(ResourceInfo * res_info, void * multi_para, Operation_t op_type);
};

class SDK_IMPORT_EXPORT OperationManager_T
{
public:
    /// Global access point to the Singleton.
    static OperationManager *instance (void);
};
#endif // OPERATIONMANAGER_H
