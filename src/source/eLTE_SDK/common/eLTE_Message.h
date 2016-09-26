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
filename    :    eLTE_Message.h
author      :    yubinbin wx200189
created     :    2015/01/12
description :    对服务端和客户端的通信报文进行定义(服务端和客户端公共文件)
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 初始版本
*********************************************************************/

#ifndef __ELTE_MESSAGE_H__
#define __ELTE_MESSAGE_H__


// 常量定义
#define PROTOCOL_VERSION						0x0100				// 协议版本，当前版本号为0x0100
#define INVALID_MSG_TYPE						(UINT16)(-1)		// 消息类型，非法类型
#define XML_CONTEXT_TYPE						0x0001				// 字段类型定义，“0x0001”为xml文本字段

#define BUFFER_SIZE								1024				// SSL_read一次最多读取字节数

#define RSP_MSG_FLAG							0xF000				// respond消息标志

#define SESSIONID_LENGTH						32					// sessionid长度

#define KEEPALIVE_TIME							6					// 保活时间  单位：秒
#define MAX_TIMEOUT_COUNT						5					// 最大超时次数
#define SERVER_IP                               "127.0.0.1"
//////////////////////////////////////////////////////////////////////////
// 消息头
class PACKET_HEAD
{
public:
	UINT16 ProtocolVersion;
	UINT16 MsgType;
	UINT32 SeqID;
	char SessionID[SESSIONID_LENGTH];
	INT32  RspCode;
	UINT32 Type;
	UINT32 PacketLength;

	PACKET_HEAD()
		: ProtocolVersion(PROTOCOL_VERSION)
		, MsgType(INVALID_MSG_TYPE)
		, SeqID(0)
		, RspCode(0)
		, Type(XML_CONTEXT_TYPE)
		, PacketLength(0)
	{
		eSDK_MEMSET(SessionID, 0, SESSIONID_LENGTH);
	}
	~PACKET_HEAD()
	{
		try
		{
			ProtocolVersion = PROTOCOL_VERSION;
			MsgType = INVALID_MSG_TYPE;
			SeqID = 0;
			RspCode = 0;
			Type = XML_CONTEXT_TYPE;
			PacketLength = 0;
			eSDK_MEMSET(SessionID, 0, SESSIONID_LENGTH);
		}
		catch(...){}
	}
	PACKET_HEAD(const PACKET_HEAD& head)
		: ProtocolVersion(head.ProtocolVersion)
		, MsgType(head.MsgType)
		, SeqID(head.SeqID)
		, RspCode(head.RspCode)
		, Type(head.Type)
		, PacketLength(head.PacketLength)
	{
		eSDK_MEMCPY(SessionID, sizeof(SessionID), head.SessionID, SESSIONID_LENGTH);
	}
};

// 消息体
class PACKET_BODY
{
public:
	char* Value;

	PACKET_BODY()
		: Value(NULL)
	{
	}
	~PACKET_BODY()
	{
		// 析构时不释放内存，要么很容易出现野指针（如：临时PACKET_BODY变量, 队列出队等）
		// PACKET_BODY的内存，程序员自己注意释放

		//try
		//{
		//	if (NULL != Value)
		//	{
		//		delete[] Value;
				Value = NULL;
		//	}
		//}
		//catch(...){}
	}
	PACKET_BODY(const PACKET_BODY& body)
		: Value(body.Value)
	{
	}
};

// 消息结构
class PACKET_DATA : public PACKET_HEAD, public PACKET_BODY
{
public:
	// ssl
	void* ssl;

	PACKET_DATA()
		: ssl(NULL)
	{
	}
	~PACKET_DATA()
	{
		try
		{
			ssl = NULL;
		}
		catch(...){}
	}
	PACKET_DATA(const PACKET_DATA& data)
		: PACKET_HEAD(data)
		, PACKET_BODY(data)
		, ssl(data.ssl)
	{
	}
};

#define RESET_PACKET(_data){\
	_data.ProtocolVersion = PROTOCOL_VERSION;\
	_data.MsgType = INVALID_MSG_TYPE;\
	_data.SeqID = 0;\
	eSDK_MEMSET(_data.SessionID, 0, SESSIONID_LENGTH);\
	_data.RspCode = 0;\
	_data.Type = XML_CONTEXT_TYPE;\
	_data.PacketLength = 0;\
	_data.Value = NULL;\
	_data.ssl = NULL;\
}

#define RELEASE_PACKET(_data){\
	if (NULL != _data.Value)\
	{\
		delete[] _data.Value;\
		_data.Value = NULL;\
	}\
	_data.ProtocolVersion = PROTOCOL_VERSION;\
	_data.MsgType = INVALID_MSG_TYPE;\
	_data.SeqID = 0;\
	eSDK_MEMSET(_data.SessionID, 0, SESSIONID_LENGTH);\
	_data.RspCode = 0;\
	_data.Type = XML_CONTEXT_TYPE;\
	_data.PacketLength = 0;\
	_data.ssl = NULL;\
}

#define PACKET_HEAD_SIZE						(sizeof(PACKET_HEAD))

//////////////////////////////////////////////////////////////////////////
// 消息类型
// 通用接口定义			REQ: 0x0000 - 0x00FF RSP: 0xF000 - 0xF0FF
#define ELTE_SERVICE_CONNECT_REQ				0x0001				// 连接服务
#define ELTE_SERVICE_CONNECT_RSP				0xF001
#define ELTE_SERVICE_DISCONNECT_REQ				0x0002				// 断开服务
#define ELTE_SERVICE_DISCONNECT_RSP				0xF002
#define ELTE_SERVICE_KEEPALIVE_REQ				0x0003				// 保活
#define ELTE_SERVICE_KEEPALIVE_RSP				0xF003

// 用户管理接口定义		REQ: 0x0100 - 0x02FF RSP: 0xF100 - 0xF2FF
#define ELTE_SERVICE_LOGIN_REQ					0x0101				// 登录
#define ELTE_SERVICE_LOGIN_RSP					0xF101
#define ELTE_SERVICE_LOGOUT_REQ					0x0102				// 注销
#define ELTE_SERVICE_LOGOUT_RSP					0xF102

#define ELTE_SERVICE_USERSTATUS_REQ				0x0103				// 设备状态变化消息
#define ELTE_SERVICE_USERSTATUS_RSP				0xF103
#define ELTE_SERVICE_MODULESTATUS_REQ			0x0104				// 模块状态变化消息
#define ELTE_SERVICE_MODULESTATUS_RSP			0xF104
#define ELTE_SERVICE_PROVISIONALLRESYNC_REQ		0x0119				// 自动下载配置数据完成消息
#define ELTE_SERVICE_PROVISIONALLRESYNC_RSP		0xF119

// 设备管理接口定义		REQ: 0x0300 - 0x04FF RSP: 0xF300 - 0xF4FF
#define ELTE_SERVICE_TRIGGERSTATUSREPORT_REQ	0x0301				// 触发状态上报
#define ELTE_SERVICE_TRIGGERSTATUSREPORT_RSP	0xF301
#define ELTE_SERVICE_PROVISIONMANAGERINIT_REQ	0x0302				// 配置管理初始化
#define ELTE_SERVICE_PROVISIONMANAGERINIT_RSP	0xF302
#define ELTE_SERVICE_PROVISIONMANAGEREXIT_REQ	0x0303				// 退出配置管理
#define ELTE_SERVICE_PROVISIONMANAGEREXIT_RSP	0xF303
#define ELTE_SERVICE_GETDCGROUPS_REQ			0x0304				// 获取调度台群组列表
#define ELTE_SERVICE_GETDCGROUPS_RSP			0xF304
#define ELTE_SERVICE_GETDCUSERS_REQ				0x0305				// 获取调度台用户列表
#define ELTE_SERVICE_GETDCUSERS_RSP				0xF305
#define ELTE_SERVICE_GETGROUPUSERS_REQ			0x0306				// 获取一个群组成员列表
#define ELTE_SERVICE_GETGROUPUSERS_RSP			0xF306
#define ELTE_SERVICE_GETGROUPINFO_REQ			0x0307				// 获取一个群组详细信息
#define ELTE_SERVICE_GETGROUPINFO_RSP			0xF307
#define ELTE_SERVICE_GETUSERINFO_REQ			0x0308				// 获取一个用户详细信息
#define ELTE_SERVICE_GETUSERINFO_RSP			0xF308
#define ELTE_SERVICE_GETDCINFO_REQ				0x0309				// 获取一个调度台详细信息
#define ELTE_SERVICE_GETDCINFO_RSP				0xF309
#define ELTE_SERVICE_SUBSCRIBEGROUP_REQ			0x030A				// 订阅群组
#define ELTE_SERVICE_SUBSCRIBEGROUP_RSP			0xF30A
#define ELTE_SERVICE_GETUSERRECFILEINFOLIST_REQ	0x030B				// 获取录像文件列表
#define ELTE_SERVICE_GETUSERRECFILEINFOLIST_RSP	0xF30B
#define ELTE_SERVICE_CREATEDYNAMICGROUP_REQ		0x030C				// 创建动态重组
#define ELTE_SERVICE_CREATEDYNAMICGROUP_RSP		0xF30C
#define ELTE_SERVICE_CANCELDYNAMICGROUP_REQ		0x030D				// 删除动态重组
#define ELTE_SERVICE_CANCELDYNAMICGROUP_RSP		0xF30D
#define ELTE_SERVICE_GETTEMPGROUPID_REQ			0x030E				// 获取临时组号
#define ELTE_SERVICE_GETTEMPGROUPID_RSP			0xF30E
#define ELTE_SERVICE_CREATETEMPGROUP_REQ		0x030F				// 创建临时组
#define ELTE_SERVICE_CREATETEMPGROUP_RSP		0xF30F

#define ELTE_SERVICE_RESOURCESTATUS_REQ			0x0310				// 群组关系状态变化消息
#define ELTE_SERVICE_RESOURCESTATUS_RSP			0xF310
#define ELTE_SERVICE_PROVISIONCHANGE_REQ		0x0311				// 设备属性配置变更消息
#define ELTE_SERVICE_PROVISIONCHANGE_RSP		0xF311

#define ELTE_SERVICE_PROVISIONMANAGERINITMRS_REQ		0x0312			// 媒体服务器初始化消息
#define ELTE_SERVICE_PROVISIONMANAGERINITMRS_RSP		0xF312
#define ELTE_SERVICE_SUBJOINGROUP_REQ				0x0313				// 订阅并自动加入群组
#define ELTE_SERVICE_SUBJOINGROUP_RSP				0xF313
#define ELTE_SERVICE_GISSUBSCRIBE_REQ				0x0314				// 订阅/去订阅GIS终端
#define ELTE_SERVICE_GISSUBSCRIBE_RSP				0xF314

#define ELTE_SERVICE_SDSSEND_REQ				    0x0315				// 发送短/彩信
#define ELTE_SERVICE_SDSSEND_RSP				    0xF315

#define ELTE_SERVICE_GETVWALLLIST_REQ				0x0316				// 获取视频上墙ID
#define ELTE_SERVICE_GETVWALLLIST_RSP				0xF316

#define ELTE_SERVICE_GETVWALLSTATE_REQ				0x0317				// 获取视频上墙ID状态
#define ELTE_SERVICE_GETVWALLSTATE_RSP				0xF317

#define ELTE_SERVICE_CREATEPATCHGROUP_REQ		    0x0318				// 创建派接组
#define ELTE_SERVICE_CREATEPATCHGROUP_RSP		    0xF318
#define ELTE_SERVICE_CANCELPATCHGROUP_REQ		    0x0319				// 删除派接组
#define ELTE_SERVICE_CANCELPATCHGROUP_RSP		    0xF319
#define ELTE_SERVICE_ADDPATCHMEMBER_REQ		        0x031A				// 增加派接组成员
#define ELTE_SERVICE_ADDPATCHMEMBER_RSP		        0xF31A
#define ELTE_SERVICE_DELETEPATCHMEMBER_REQ		    0x031B				// 删除派接组成员
#define ELTE_SERVICE_DELETEPATCHMEMBER_RSP		    0xF31B
#define ELTE_SERVICE_GETPATCHGROUPS_REQ             0x031C              // 获取派接组
#define ELTE_SERVICE_GETPATCHGROUPS_RSP             0xF31C
#define ELTE_SERVICE_GETPATCHGROUPMEMBERS_REQ       0x031D              // 获取派接组的成员
#define ELTE_SERVICE_GETPATCHGROUPMEMBERS_RSP       0xF31D
#define ELTE_SERVICE_GETPATCHGROUPINFO_REQ          0x031E              // 获取派接组的属性信息
#define ELTE_SERVICE_GETPATCHGROUPINFO_RSP          0xF31E

#define ELTE_SERVICE_GETUSERSPECIFICGISCFG_REQ      0x031F              // 查询终端的GIS配置信息
#define ELTE_SERVICE_GETUSERSPECIFICGISCFG_RSP      0xF31F

#define ELTE_SERVICE_SETUSERSPECIFICGISCFG_REQ      0x0320              // 设置终端的GIS配置信息
#define ELTE_SERVICE_SETUSERSPECIFICGISCFG_RSP      0xF320

#define ELTE_SERVICE_GETGISSUBSCRIPTION_REQ         0x0321              // 获取调度台订阅的终端列表
#define ELTE_SERVICE_GETGISSUBSCRIPTION_RSP         0xF321

#define ELTE_SERVICE_MODIFYDYNAMICGROUP_REQ         0x0322              // 修改动态重组
#define ELTE_SERVICE_MODIFYDYNAMICGROUP_RSP         0xF322

#define ELTE_SERVICE_TEMPUSERJOINGROUP_REQ         0x0323              // 组呼添加临时用户
#define ELTE_SERVICE_TEMPUSERJOINGROUP_RSP         0xF323


// 媒体接口定义			REQ: 0x0500 - 0x07FF RSP: 0xF500 - 0xF7FF
#define ELTE_SERVICE_STARTREALPLAY_REQ			0x0501				// 开始播放实时视频
#define ELTE_SERVICE_STARTREALPLAY_RSP			0xF501
#define ELTE_SERVICE_PTZCONTROL_REQ				0x0502				// 开始云台控制
#define ELTE_SERVICE_PTZCONTROL_RSP				0xF502
#define ELTE_SERVICE_STOPREALPLAY_REQ			0x0503				// 停止播放实时视频
#define ELTE_SERVICE_STOPREALPLAY_RSP			0xF503



#define ELTE_SERVICE_P2PVIDEOCALLSTATUS_REQ		0x0504				// 实时视频状态消息
#define ELTE_SERVICE_P2PVIDEOCALLSTATUS_RSP		0xF504
#define ELTE_SERVICE_P2PCALLSTATUS_REQ			0x0505				// 点呼状态消息
#define ELTE_SERVICE_P2PCALLSTATUS_RSP			0xF505
#define ELTE_SERVICE_GROUPCALLSTATUS_REQ		0x0506				// 组呼状态消息
#define ELTE_SERVICE_GROUPCALLSTATUS_RSP		0xF506

#define ELTE_SERVICE_P2PDIAL_REQ			0x0507			// 发起语音点呼
#define ELTE_SERVICE_P2PDIAL_RSP				0xF507
#define ELTE_SERVICE_P2PRECV_REQ			0x0508			// 接收语音点呼
#define ELTE_SERVICE_P2PRECV_RSP			0xF508
#define ELTE_SERVICE_P2PREJECT_REQ		0x0509			// 拒接语音点呼
#define ELTE_SERVICE_P2PREJECT_RSP			0xF509
#define ELTE_SERVICE_P2PHANGUP_REQ		0x050A	 		// 挂断语音点呼
#define ELTE_SERVICE_P2PHANGUP_RSP		0xF50A
#define ELTE_SERVICE_P2PBREAKOFF_REQ	0x050B			// 调度台强拆点呼
#define ELTE_SERVICE_P2PBREAKOFF_RSP	0xF50B
#define ELTE_SERVICE_P2PBREAKIN_REQ		0x050C		// 抢话（强拆点呼+发起新点呼）
#define ELTE_SERVICE_P2PBREAKIN_RSP		0xF50C

#define ELTE_SERVICE_PTTDIAL_REQ					0x050D		// 发起组呼（固定和临时组呼）或抢权
#define ELTE_SERVICE_PTTDIAL_RSP					0xF50D
#define ELTE_SERVICE_PTTRELEASE_REQ			0x050E			// 释放话权
#define ELTE_SERVICE_PTTRELEASE_RSP			0xF50E
#define ELTE_SERVICE_PTTHANGUP_REQ			0x050F			// 退出组呼
#define ELTE_SERVICE_PTTHANGUP_RSP			0xF50F
#define ELTE_SERVICE_PTTEMERGENCY_REQ		0x0510			// 发起紧急组呼
#define ELTE_SERVICE_PTTEMERGENCY_RSP		0xF510
#define ELTE_SERVICE_GROUPBREAKOFF_REQ	0x0511			// 调度台强拆组呼
#define ELTE_SERVICE_GROUPBREAKOFF_RSP	0xF511

#define ELTE_SERVICE_VOLUMEMUTE_REQ		0x0512			// 执行静音（点呼、组呼、视频回传）
#define ELTE_SERVICE_VOLUMEMUTE_RSP			0xF512
#define ELTE_SERVICE_VOLUMEUNMUTE_REQ	0x0513			// 取消静音（点呼、组呼、视频回传）
#define ELTE_SERVICE_VOLUMEUNMUTE_RSP	0xF513

#define ELTE_SERVICE_GISREPORT_REQ		0x0514				// GIS消息
#define ELTE_SERVICE_GISREPORT_RSP		0xF514
#define ELTE_SERVICE_GISSTATUS_REQ		0x0515				// GIS状态消息
#define ELTE_SERVICE_GISSTATUS_RSP		0xF515

#define ELTE_SERVICE_SDSREPORT_REQ		0x0516				// 接受短彩信消息
#define ELTE_SERVICE_SDSREPORT_RSP		0xF516
#define ELTE_SERVICE_SDSSENDSTATUS_REQ  0x0517				// 发送短彩信状态消息
#define ELTE_SERVICE_SDSSENDSTATUS_RSP  0xF517

#define ELTE_SERVICE_STARTDISPATCHVIDEO_REQ	    0x0518				// 开始视频分发
#define ELTE_SERVICE_STARTDISPATCHVIDEO_RSP		0xF518
#define ELTE_SERVICE_STOPDISPATCHVIDEO_REQ	    0x0519				// 停止视频分发
#define ELTE_SERVICE_STOPDISPATCHVIDEO_RSP		0xF519


#define ELTE_SERVICE_RECVVIDEOPLAY_REQ          0x051A              // 接受视频回传
#define ELTE_SERVICE_RECVVIDEOPLAY_RSP          0xF51A              

#define ELTE_SERVICE_VWALLSTART_REQ				0x051B				//视频上墙
#define ELTE_SERVICE_VWALLSTART_RSP				0xF51B

#define ELTE_SERVICE_VWALLSTOP_REQ          0x051C              // 停止视频上墙
#define ELTE_SERVICE_VWALLSTOP_RSP          0xF51C

#define ELTE_SERVICE_TELEPHONEDIAL_REQ			0x051D			// 发起PSTN/PLMN电话呼叫
#define ELTE_SERVICE_TELEPHONEDIAL_RSP			0xF51D

#define ELTE_SERVICE_TELEPHONEHANGUP_REQ			0x051E			// 挂断PSTN/PLMN电话呼叫
#define ELTE_SERVICE_TELEPHONEHANGUP_RSP			0xF51E

#define ELTE_SERVICE_STARTDISCREETLISTEN_REQ			0x051F			// 发起缜密监听
#define ELTE_SERVICE_STARTDISCREETLISTEN_RSP			0xF51F

#define ELTE_SERVICE_STOPDISCREETLISTEN_REQ			0x0520			// 停止缜密监听
#define ELTE_SERVICE_STOPDISCREETLISTEN_RSP			0xF520

#define ELTE_SERVICE_STARTENVIRLISTEN_REQ			0x0521			// 发起环境侦听
#define ELTE_SERVICE_STARTENVIRLISTEN_RSP			0xF521

#define ELTE_SERVICE_P2PTRANSFER_REQ			0x0522			    // 发起人工转接
#define ELTE_SERVICE_P2PTRANSFER_RSP			0xF522

#define ELTE_SERVICE_P2PHALFDPXDIAL_REQ			0x0523			   // 发起半双工点呼
#define ELTE_SERVICE_P2PHALFDPXDIAL_RSP			0xF523

#define ELTE_SERVICE_P2PHALFDPXRELEASE_REQ		0x0524			   // 释放半双工点呼
#define ELTE_SERVICE_P2PHALFDPXRELEASE_RSP		0xF524

//////////////////////////////////////////////////////////////////////////
// 消息类型范围定义
// 通用接口定义			REQ: 0x0000 - 0x00FF RSP: 0xF000 - 0xF0FF
#define ELTE_INTERFACE_UNIVERSAL_REQ_BEGIN		0x0000
#define ELTE_INTERFACE_UNIVERSAL_REQ_END		0x00FF
#define ELTE_INTERFACE_UNIVERSAL_RSP_BEGIN		0xF000
#define ELTE_INTERFACE_UNIVERSAL_RSP_END		0xF0FF

// 用户管理接口定义		REQ: 0x0100 - 0x02FF RSP: 0xF100 - 0xF2FF
#define ELTE_INTERFACE_USERMGR_REQ_BEGIN		0x0100
#define ELTE_INTERFACE_USERMGR_REQ_END			0x02FF
#define ELTE_INTERFACE_USERMGR_RSP_BEGIN		0xF100
#define ELTE_INTERFACE_USERMGR_RSP_END			0xF2FF

// 设备管理接口定义		REQ: 0x0300 - 0x04FF RSP: 0xF300 - 0xF4FF
#define ELTE_INTERFACE_DEVICEMGR_REQ_BEGIN		0x0300
#define ELTE_INTERFACE_DEVICEMGR_REQ_END		0x04FF
#define ELTE_INTERFACE_DEVICEMGR_RSP_BEGIN		0xF300
#define ELTE_INTERFACE_DEVICEMGR_RSP_END		0xF4FF

// 媒体接口定义			REQ: 0x0500 - 0x07FF RSP: 0xF500 - 0xF7FF
#define ELTE_INTERFACE_MEDIAMGR_REQ_BEGIN		0x0500
#define ELTE_INTERFACE_MEDIAMGR_REQ_END			0x07FF
#define ELTE_INTERFACE_MEDIAMGR_RSP_BEGIN		0xF500
#define ELTE_INTERFACE_MEDIAMGR_RSP_END			0xF7FF

#endif
