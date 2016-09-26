// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#ifndef ELTE_RM_SDK_EXPORTS
#define ELTE_RM_SDK_EXPORTS
#endif

typedef enum
{
	/*-------------------以下信息元素用于点呼，包括视频点呼；见p2pcallstatus_indicator.cpp-----------*/
	CALLSTATUS =0, //点呼状态
	CALLERINFO =1, //主叫人
	CALLEEINFO =2, //被叫人

	/*--------------------以下信息元素用于组呼，见grpcallstatus_indicator.cpp----------------*/
	GRPCALLSTATUS =3, //组呼状态
	GRPCALLSPERKER =4, //组呼主讲人
	FLOORSTATUS =5,    //话权状态指示

	/*--------------------以下信息元素用于紧急呼叫， 见emergency_indicator.cpp----------------*/
	EMERGENCYCALL =6,  //紧急呼叫指示
	EMERGENCYCALLER =7,  //紧急呼叫人

	/*--------------------以下信息元素用于资源状态管理,见resourcestatus_indicator.cpp----------------*/
	RESORCEID =8,              //资源ID
	DEVICEID =9,               //视频分栏时的窗口号
	RESSELECTSTATUS =10,        //资源选中状态指示， 用于多GUI联动
	RESASSIGNSTATUS =11,        //资源指派状态指示， 即调度台是否加入该资源组
	RESTXSTATUS =12,            //资源占用状态指示
	RESLOGGINGSTATUS =13,       //资源录音状态指示
	RESDLSTATUS =14,            //资源监听状态指示
	RESREGSTATUS_PROXY =15,     //资源注册状态指示, 代理注册(所有有线用户，终端集群注册)
	RESREGSTATUS_DIRECT =16,    //资源注册状态指示, 直接注册(终端业务注册，如视频)
	ATTACHINGGROUP =17,         //资源加入的组号

	/*--------------------以下信息元素用于用户状态管理;见userstatus_indicator.cpp----------------*/
	USERSTATUS =18,		//用户状态指示， 一般由BCC通知
	USERGPSSTATUS =19,       //用户GPS状态指示
	USERDLSTATUS =20,            //用户监听状态指示
	USERDGNASTATUS =21,      //用户动态重组状态
	VIDEODISPATCHSTATUS =22,     //视频分发状态
	MUTESTATUS =23,          //静音状态
	GRPPATCHSTATUS =24  //派接操作状态
}BundleKey;

/**
   定义资源状态指示数据结构
**/
typedef  BundleKey resource_status_t;

typedef enum {
    RESOURCE_STATUS_UNKNOWN = 4000,
    STATUS_SELECTED = 4001,    //资源选中状态
    STATUS_UNSELECTED = 4002,  //资源去选中状态

    STATUS_ASSIGNED = 4003,    //资源指派状态-
    STATUS_DEASSIGNED = 4004,  //资源去指派状态-

    STATUS_LISTENING = 4005,   //资源监听中状态
    STATUS_LISTENEND = 4006,   //资源监听结束状态

    STATUS_LOGGING = 4007,      //资源录音中状态
    STATUS_NOLOGGING = 4008,    //资源未设置录音

    STATUS_INPATCH = 4009,      //资源被派接
    STATUS_OUTPATCH = 4010,     //资源解除派接

    STATUS_REGOK = 4011,        //资源注册
    STATUS_REGFAIL = 4012,      //资源去注册
    STATUS_FORBIDEN = 4013,     //资源未授权 500
    STATUS_NOT_FOUND = 4014,    //资源不存在 404
    STATUS_TEMP_UNAVAILABLE = 4015, //资源暂时不可用 480
    STATUS_RES_CONFILCT = 4016, //资源冲突 450
    STATUS_UNAUTHORIZED = 4017, //未鉴权 401
    STATUS_LICENSE_LIMIT = 4018, //License受限 503
    STATUS_PASSWORD_WRONG = 4019, //密码错误 403

    //下面四个状态的顺序很重要， 因为 isValidTransition（）需要参考顺序
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
    STATUS_GRPPATCH_CREATEOK = 4031,  //派接组创建OK
    STATUS_GRPPATCH_CREATEFAIL = 4032, //派接组创建失败
    STATUS_GRPPATCH_CANCELOK = 4033, //派接组取消OK
    STATUS_GRPPATCH_CANCELFAIL = 4034, //派接组取消失败
    STATUS_GRPPATCH_ADDOK = 4035, //派接组添加成员OK
    STATUS_GRPPATCH_ADDFAIL = 4036, //派接组添加成员FAIL
    STATUS_GRPPATCH_DELOK = 4037, //派接组删除成员OK
    STATUS_GRPPATCH_DELFAIL = 4038,  //派接组删除成员FAIL

    STATUS_ACCOUNT_LOCKED = 4060 // 账号被锁定

} res_status_value_t;

typedef enum {
	//查询
	SEQ_CONNECT_REQUEST,         // 接入请求
	SEQ_LOGIN,                   // 用户登录
	SEQ_LOGOUT,                  // 用户登出



	SEQ_GETDCUSERS = 25,         // 查询调度台所有用户
	SEQ_GETDCGROUPS,             // 查询调度台所有组
	SEQ_GETGROUPUSERS,           // 查询组里用户
	SEQ_GETUSERINFO,             // 查询用户信息
	SEQ_GETDCINFO,               // 查询调度台信息
	SEQ_GETGROUPINFO,            // 查询组信息



	SEQ_STARTREALPLAY = 50,      // 开始视频回传
	SEQ_STOPREALPLAY,           // 停止视频回传
	SEQ_PTZCONTROL,              // 云台控制









} excute_seq_value_t;

// TODO: reference additional headers your program requires here
