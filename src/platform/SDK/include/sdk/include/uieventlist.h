/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    uieventlist.h
Author:      Wu yu wei
Created:     2012/07/13 10:00
Description: 定义APP通知GUI的所有信息元素
             如何显示或反应由GUI自主决定
*********************************************************************/

#ifndef UIEVENTLIST_H
#define UIEVENTLIST_H

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
    GRPPATCHSTATUS =24,  //派接操作状态
    TMPGRPMUTESTATUS =25  //临时组静音通知
}BundleKey;

#endif // UIEVENTLIST_H





