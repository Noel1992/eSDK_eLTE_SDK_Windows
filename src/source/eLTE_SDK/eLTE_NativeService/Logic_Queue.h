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
filename    :    Logic_Queue.h
author      :    yubinbin wx200189
created     :    2015/01/12
description :    �߼����й����࣬����ѻص���Ϣ�Ϳͻ���������Ϣ�����е���ʽ���д���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 ��ʼ�汾
*********************************************************************/

#ifndef __LOGIC_QUEUE_H__
#define __LOGIC_QUEUE_H__

#include <queue>
#include "openssl/ssl.h"
#include "eLTE_Message.h"


// �����߼��������ݲ������߼�����
#define PUSH_LOGIC_QUEUE(msgType, xmlStr, iRet) {\
	if (xmlStr.empty())\
	{\
	QUEUE_DATA _data;\
	_data.ProtocolVersion = PROTOCOL_VERSION;\
	_data.MsgType = msgType;\
	_data.SeqID = SSL_Socket::Instance().GetSeqNum();\
	eSDK_MEMCPY(_data.SessionID, sizeof(_data.SessionID), SessionMgr::Instance().GetSessionID().c_str(), SESSIONID_LENGTH);\
	_data.RspCode = iRet;\
	_data.Type = XML_CONTEXT_TYPE;\
	_data.PacketLength = PACKET_HEAD_SIZE;\
	_data.ssl = SSL_Socket::Instance().GetOptSSL();\
	_data.Value = NULL;\
	iRet = Logic_Queue::Instance().Push(_data);\
	if (eLTE_SVC_ERR_SUCCESS != iRet)\
		{\
		LOG_RUN_ERROR("Logic_Queue Push failed.");\
		delete[] _data.Value;\
		}\
	}\
	else\
	{\
	QUEUE_DATA _data;\
	_data.ProtocolVersion = PROTOCOL_VERSION;\
	_data.MsgType = msgType;\
	_data.SeqID = SSL_Socket::Instance().GetSeqNum();\
	_data.RspCode = iRet;\
	_data.Type = XML_CONTEXT_TYPE;\
	_data.PacketLength = PACKET_HEAD_SIZE+xmlStr.size();\
	_data.ssl = SSL_Socket::Instance().GetOptSSL();\
	_data.Value = new char[xmlStr.size()+1];\
	if (NULL == _data.Value)\
		{\
		LOG_RUN_DEBUG("New queue data buf failed.");\
		return;\
		}\
		eSDK_MEMSET(_data.Value, 0, xmlStr.size()+1);\
		eSDK_MEMCPY(_data.Value, xmlStr.size()+1, xmlStr.c_str(), xmlStr.size());\
		iRet = Logic_Queue::Instance().Push(_data);\
		if (eLTE_SVC_ERR_SUCCESS != iRet)\
		{\
		LOG_RUN_ERROR("Logic_Queue Push failed.");\
		delete[] _data.Value;\
		}\
	}\
}

#define CONSTRUCT_AND_SEND_MESSAGE() \
	PACKET_BODY body;											\
	size_t _size = xmlStr.size();								\
	body.Value = new char[_size+1];								\
	if (NULL == body.Value)										\
	{															\
		LOG_RUN_ERROR("new memory failed.");					\
		head.RspCode = eLTE_SVC_ERR_MEMORY_APPLY;				\
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);				\
		return;													\
	}															\
	eSDK_MEMSET(body.Value, 0, _size+1);						\
	eSDK_MEMCPY(body.Value, _size+1, xmlStr.c_str(), _size);	\
	head.RspCode = iRet;										\
	head.PacketLength += _size;									\
	SSL_WRITE_SAFE(_ssl, &head, PACKET_HEAD_SIZE, body.Value);	\
	SSL_WRITE_SAFE(_ssl, body.Value, _size, body.Value);		\
	if(NULL != body.Value)										\
	{															\
		delete [] body.Value;									\
		body.Value = NULL;										\
	}															\

#define CONSTRUCT_AND_WRITE_GROUPUSERS_REQ() \
	PACKET_HEAD head(data);	\
	head.MsgType |= RSP_MSG_FLAG;	\
	head.PacketLength = PACKET_HEAD_SIZE;	\
	int groupId;	\
	int iRet = XMLProcess::GetXml_GetGroupUsers_Req(data.Value, groupId);	\
	if (eLTE_SVC_ERR_SUCCESS != iRet)	\
	{	\
		LOG_RUN_ERROR("GetXml_GetGroupUsers_Req failed.");	\
		head.RspCode = iRet;	\
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);	\
		return;	\
	}	\

#define CONSTRUCT_AND_WRITE_RESOURCEID_REQ() \
	PACKET_HEAD head(data);	\
	head.MsgType |= RSP_MSG_FLAG;	\
	head.PacketLength = PACKET_HEAD_SIZE;	\
	std::string ResourceID;	\
	VWallStop_parameter param;	\
	int iRet = XMLProcess::GetXml_GetResourceID_Req(data.Value, ResourceID);	\
	if (eLTE_SVC_ERR_SUCCESS != iRet)	\
	{	\
		LOG_RUN_ERROR("GetXml_StartDiscreetListen_Req failed.");	\
		head.RspCode = iRet;	\
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);	\
		return;	\
	}	\

#define CONSTRUCT_AND_WRITE_VOL_HEAD() \
	PACKET_HEAD head(data);	\
	head.MsgType |= RSP_MSG_FLAG;	\
	head.PacketLength = PACKET_HEAD_SIZE;	\
	int resId;	\
	int callType;	\
	int iRet = XMLProcess::GetXml_VolControl_Req(data.Value, resId, callType);	\
	if (eLTE_SVC_ERR_SUCCESS != iRet)	\
	{	\
		LOG_RUN_ERROR("GetXml_VolControl_Req failed.");	\
		head.RspCode = iRet;	\
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);	\
		return;	\
	}	\

#define CONSTRUCT_AND_WRITE_RESOURCE_HEAD() \
	PACKET_HEAD head(data);	\
	head.MsgType |= RSP_MSG_FLAG;	\
	head.PacketLength = PACKET_HEAD_SIZE;	\
	int resId;	\
	int iRet = XMLProcess::Common_GetResourceID(data.Value, resId);	\
	if (eLTE_SVC_ERR_SUCCESS != iRet)	\
	{	\
		LOG_RUN_ERROR("Common_GetResourceID failed.");	\
		head.RspCode = iRet;	\
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);	\
		return;	\
	}	\

#define CONSTRUCT_AND_WRITE_GROUP_HEAD() \
	PACKET_HEAD head(data);	\
	head.MsgType |= RSP_MSG_FLAG;	\
	head.PacketLength = PACKET_HEAD_SIZE;	\
	int groupId;	\
	int iRet = XMLProcess::Common_GetGroupID(data.Value, groupId);	\
	if (eLTE_SVC_ERR_SUCCESS != iRet)	\
	{	\
	LOG_RUN_ERROR("Common_GetGroupID failed.");	\
	head.RspCode = iRet;	\
	SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);	\
	return;	\
	}	\

#define CONSTRUCT_AND_WRITE_DCGROUPS_HEAD() \
	PACKET_HEAD head(data);	\
	head.MsgType |= RSP_MSG_FLAG;	\
	head.PacketLength = PACKET_HEAD_SIZE;	\
	int userId;	\
	int iRet = XMLProcess::GetXml_GetDcGroups_Req(data.Value, userId);	\
	if (eLTE_SVC_ERR_SUCCESS != iRet)	\
	{	\
	LOG_RUN_ERROR("GetXml_GetDcGroups_Req failed.");	\
	head.RspCode = iRet;	\
	SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);	\
	return;	\
	}	\

// �߼���������
typedef PACKET_DATA QUEUE_DATA;

class Logic_Queue
{
private:
	Logic_Queue(void);
	~Logic_Queue(void);

public:
	static Logic_Queue& Instance();
	int Init();
	void Uninit();
	int Push(const QUEUE_DATA& data);
	
private:
	void Clean();
	static unsigned __stdcall Run(void* param);
	// ������Ϣ�߼�����
	void OperationMsg(const QUEUE_DATA& data) const;
	// �ص���Ϣ�߼�����
	void CallBackMsg(const QUEUE_DATA& data) const;
	// �ص���ϢRespond�߼�����
	void RespondMsg(const QUEUE_DATA& data) const;

private:
	// ͨ�ýӿ�
	void InterfaceUniversal(const QUEUE_DATA& data) const;
	// �û�����ӿ�
	void InterfaceUserMgr(const QUEUE_DATA& data) const;
	// �豸����ӿ�
	void InterfaceDeviceMgr(const QUEUE_DATA& data) const;
	// ý��ӿ�
	void InterfaceMediaMgr(const QUEUE_DATA& data) const;

	// ҵ����ӿ�
private:
	// ���ӷ���
	void Connect(const QUEUE_DATA& data) const;
	// �Ͽ�����
	void Disconnect(const QUEUE_DATA& data) const;
	// ����
	void KeepAlive(const QUEUE_DATA& data) const;
	// ��¼
	void Login(const QUEUE_DATA& data) const;
	// ע��
	void Logout(const QUEUE_DATA& data) const;
	// ����״̬�ϱ�
	void TriggerStatusReport(const QUEUE_DATA& data) const;
	// ���ù����ʼ��
	void ProvisionInit(const QUEUE_DATA& data) const;
	// ý���������ʼ��
	void ProvisionInitMRS(const QUEUE_DATA& data) const;
	// �˳����ù���
	void ProvisionExit(const QUEUE_DATA& data) const;
	// ��ȡ����̨Ⱥ���б�
	void GetDcGroups(const QUEUE_DATA& data) const;
	// ��ȡ����̨�û��б�
	void GetDcUsers(const QUEUE_DATA& data) const;
	// ��ȡһ��Ⱥ���Ա�б�
	void GetGroupUsers(const QUEUE_DATA& data) const;
	// ��ȡһ��Ⱥ����ϸ��Ϣ
	void GetGroupInfo(const QUEUE_DATA& data) const;
	// ��ȡһ���û���ϸ��Ϣ
	void GetUserInfo(const QUEUE_DATA& data) const;
	// ��ȡһ������̨��ϸ��Ϣ
	void GetDcInfo(const QUEUE_DATA& data) const;
	// ����Ⱥ��
	void SubscribeGroup(const QUEUE_DATA& data) const;
	// ��ȡ¼���ļ��б�
	void GetUserRECFileInfoList(const QUEUE_DATA& data) const;
	// ������̬����
	void CreateDynamicGroup(const QUEUE_DATA& data) const;
	// ɾ����̬����
	void CancelDynamicGroup(const QUEUE_DATA& data) const;
	// �����ɽ��顢�����ɽ����Ա��ɾ���ɽ����Ա
	void OperatePatchGroup(const QUEUE_DATA& data) const;
	// ɾ���ɽ���
	void CancelPatchGroup(const QUEUE_DATA& data) const;
	// ��ȡ����̨�ɽ����б�
	void GetPatchGroups(const QUEUE_DATA& data) const;
	// ��ȡ�ɽ���ĳ�Ա�б�
	void GetGroupMemberByPatchId(const QUEUE_DATA& data) const;
	// ��ȡ�ɽ���������Ϣ
	void GetPatchGroupInfo(const QUEUE_DATA& data) const;
	// ��ȡ��ʱ���
	void GetTempGroupID(const QUEUE_DATA& data) const;
	// ������ʱ��
	void CreateTempGroup(const QUEUE_DATA& data) const;
	// ��ʼ����ʵʱ��Ƶ
	void StartRealPlay(const QUEUE_DATA& data) const;	
	// ��ʼ��̨����
	void PTZControl(const QUEUE_DATA& data) const;
	// ֹͣ����ʵʱ��Ƶ
	void StopRealPlay(const QUEUE_DATA& data) const;
	// �����������
	void P2PDial(const QUEUE_DATA& data) const;
	// �����˫�����
	void P2PHalfDpxDial(const QUEUE_DATA& data) const;
	// �ͷŰ�˫�����
	void P2PHalfDpxRelease(const QUEUE_DATA& data) const;
	// �����������
	void P2PRecv(const QUEUE_DATA& data) const;
	// �ܽ��������
	void P2PReject(const QUEUE_DATA& data) const;
	// �Ҷ��������
	void P2PHangup(const QUEUE_DATA& data) const;
	// ������ǿ����+�����µ����
	void P2PBreakin(const QUEUE_DATA& data) const;
	// ǿ����
	void P2PBreakoff(const QUEUE_DATA& data) const;
	// ���Ĳ��Զ�����Ⱥ��
	void SubJoinGroup(const QUEUE_DATA& data) const;
	// ����������̶�����ʱ���������Ȩ
	void PTTDial(const QUEUE_DATA& data) const;
	// �ͷŻ�Ȩ
	void PTTRelease(const QUEUE_DATA& data) const;
	// �˳����
	void PTTHangup(const QUEUE_DATA& data) const;
	// ����������
	void PTTEmergency(const QUEUE_DATA& data) const;
	// ǿ�����
	void GroupBreakoff(const QUEUE_DATA& data) const;
	// ִ�о�����������������Ƶ�ش���
	void VolMute(const QUEUE_DATA& data) const;
	// ȡ��������������������Ƶ�ش���
	void VolUnMute(const QUEUE_DATA& data) const;
	// ����/ȥ����GIS�ն�
	void GISSubscribe(const QUEUE_DATA& data) const;
	// ���Ͷ�/����
	void SDSSendMessage(const QUEUE_DATA& data) const;
	// ��ʼ��Ƶ�ַ�
	void StartVideoDispatch(const QUEUE_DATA& data) const;
	// ֹͣ��Ƶ�ַ�
	void StopVideoDispatch(const QUEUE_DATA& data) const;
	// ������Ƶ�ش�
	void RecvVideoPlay(const QUEUE_DATA& data) const;
	// ��ʼ��Ƶ��ǽ
	void StartVWall(const QUEUE_DATA& data) const;
	//ֹͣ��Ƶ��ǽ
	void StopVWall(const QUEUE_DATA& data) const;
	//��ȡ��Ƶ��ǽID�б�
	void GetDcVWallIDList(const QUEUE_DATA& data) const;
	//��ѯ�ն˵�GIS������Ϣ
	void GetUserSpecificGISCfg(const QUEUE_DATA& data) const;
	//�����ն˵�GIS������Ϣ
	void SetUserSpecificGISCfg(const QUEUE_DATA& data) const;
	//��ȡ����̨���ĵ��ն��б�
	void GetGisSubscription(const QUEUE_DATA& data) const;
	//�޸Ķ�̬����
	void ModifyDynamicGroup(const QUEUE_DATA& data) const;

	// ��������ʱ�û�
//	void TempUserJoinGroup(const QUEUE_DATA& data) const;

	//����PSTN/PLMN�绰����
//	void TelephoneDial(const QUEUE_DATA& data) const;
	//�Ҷ�PSTN/PLMN�绰����
//	void TelephoneHangup(const QUEUE_DATA& data) const;
	//�������ܼ���
	void StartDiscreetListen(const QUEUE_DATA& data) const;
	//ֹͣ���ܼ���
	void StopDiscreetListen(const QUEUE_DATA& data) const;
	//ֹͣ��������
	void StartEnvironmentListen(const QUEUE_DATA& data) const;
	//�����˹�ת��
	void P2PTransfer(const QUEUE_DATA& data) const;

	// ��ʼ����Ƶ¼��
	void StartRecord(const QUEUE_DATA& data) const;
	//ֹͣ����Ƶ¼��
	void StopRecord(const QUEUE_DATA& data) const;
private:
	std::queue<QUEUE_DATA> m_logicQueue;
	HANDLE m_hRun;
	HANDLE m_hMutex;

};

#endif
