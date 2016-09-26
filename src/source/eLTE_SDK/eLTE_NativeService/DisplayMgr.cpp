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

#include "stdafx.h"
#include "DisplayMgr.h"
#include "AppContextMgr.h"
#include "OperationMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "XMLProcess.h"
#include "Logic_Queue.h"
#include "SSL_Socket.h"
#include "SessionMgr.h"
#include "mq/p2pvideocallstatus_indicator.h"
#include "mq/sds_message_indicator.h"
#include "mq/provchange_indicator.h"
#include "mq/grpcallstatus_indicator.h"
#include "mq/p2pcallstatus_indicator.h"
#include "mq/modulestatus_indicator.h"
#include "sdk_helper.h"


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

DisplayMgr::DisplayMgr(void)
{

}


DisplayMgr::~DisplayMgr(void)
{

}

//֪ͨ�����״̬�����Ϣ
void DisplayMgr::notifyGroupStatus(GrpDspInfo* pGrpInfo)
{
	LOG_TRACE();

	if (NULL == pGrpInfo)
	{
		LOG_RUN_ERROR("pGrpInfo is null.");
		return;
	}

	// ��ȡxml�ַ���
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyGroupStatus(pGrpInfo, xmlStr);
	delete pGrpInfo;
	pGrpInfo = NULL;
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyGroupStatus failed.");
		return;
	}

	// �����߼��������ݲ������߼�����
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_GROUPCALLSTATUS_REQ, xmlStr, iRet);
}

//֪ͨ�û���Ⱥ����Դ��״̬�����Ϣ
void DisplayMgr::notifyResourceStatus(ResourceStatusIndicator* pResourceStatus)
{
	LOG_TRACE();

	if (NULL == pResourceStatus)
	{
		LOG_RUN_ERROR("pResourceStatus is null.");
		return;
	}

	// ��ȡxml�ַ���
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyResourceStatus(pResourceStatus, xmlStr);
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyResourceStatus failed.");
		delete pResourceStatus;
		pResourceStatus = NULL;
		return;
	}

	// �����߼��������ݲ������߼�����
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_RESOURCESTATUS_REQ, xmlStr, iRet);

	// ��½�ɹ�������SDK����ҵ��
	if ((RESREGSTATUS_PROXY == pResourceStatus->getStatusType() || RESREGSTATUS_DIRECT == pResourceStatus->getStatusType())
		&& STATUS_REGOK == pResourceStatus->getStatusValue())
	{
		// ����SDK����ҵ��
		(void)OperationMgr::Instance().StartRunning();
		//��¼�ɹ��������¼ʧ����Ϣ
		SDK_Helper::clean_invalid_reg();
	}
	delete pResourceStatus;
	pResourceStatus = NULL;
}

//֪ͨ��Ե���е�״̬�����Ϣ
void DisplayMgr::notifyP2pcallStatus(P2pcallStatusIndicator* pCallStatus)
{
	LOG_TRACE();

	if (NULL == pCallStatus)
	{
		LOG_RUN_ERROR("pCallStatus is null.");
		return;
	}

	// ��ȡxml�ַ���
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyP2pcallStatus(pCallStatus, xmlStr);
	delete pCallStatus;
	pCallStatus = NULL;
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyP2pcallStatus failed.");
		return;
	}

	// �����߼��������ݲ������߼�����
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_P2PCALLSTATUS_REQ, xmlStr, iRet);
}

//֪ͨ��Ƶ���е�״̬�����Ϣ��������Ƶ�ش�������ҪYCʵ��
void DisplayMgr::notifyP2pvideocallStatus(P2pvideocallStatusIndicator* pCallStatus)
{
	LOG_TRACE();

	if (NULL == pCallStatus)
	{
		LOG_RUN_ERROR("pCallStatus is null.");
		return;
	}

	// ��ȡxml�ַ���
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyP2pvideocallStatus(pCallStatus, xmlStr);
	delete pCallStatus;
	pCallStatus = NULL;
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyP2pvideocallStatus failed.");
		return;
	}

	// �����߼��������ݲ������߼�����
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_P2PVIDEOCALLSTATUS_REQ, xmlStr, iRet);
}

//֪ͨ�û���Ⱥ����Դ��״̬�����Ϣ,��ҪYCʵ��
void DisplayMgr::notifyUserStatus(ResourceStatusIndicator* pResourceStatus)
{
	LOG_TRACE();

	if (NULL == pResourceStatus)
	{
		LOG_RUN_ERROR("pResourceStatus is null.");
		return;
	}

	// ��ȡxml�ַ���
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyUserStatus(pResourceStatus, xmlStr);
	delete pResourceStatus;
	pResourceStatus = NULL;
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyUserStatus failed.");
		return;
	}

	// �����߼��������ݲ������߼�����
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_USERSTATUS_REQ, xmlStr, iRet);
}

//֪ͨ�û�״̬����Ϣ�ķ������ʵ��
void DisplayMgr::notifyUserSdsStatus(SdsMessageIndicator* pSdsMessage)
{
	if (NULL == pSdsMessage)
	{
		LOG_RUN_ERROR("pSdsMessage is null.");
		return;
	}
	// ��ȡxml�ַ���
	std::string xmlStr;
	// ɸѡ��Ϣ
	bool bSdsDirection = pSdsMessage->getSdsDirection();
	if (!bSdsDirection)
	{
		int iRet = XMLProcess::SetXml_NotifyUserSdsStatusReport(pSdsMessage, xmlStr);
		delete pSdsMessage;
		pSdsMessage = NULL;
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifySDSReport failed.");
			return;
		}

		// �����߼��������ݲ������߼�����
		PUSH_LOGIC_QUEUE(ELTE_SERVICE_SDSREPORT_REQ, xmlStr, iRet);
	}
	else
	{
		LOG_RUN_ERROR("pSdsMessage is null.");
	}
}

//ģ�����״̬�¼�
void DisplayMgr::notifyModuleStatus(DCModuleStatusIndicator* pModuleStatus)
{
	//LOG_TRACE();

	if (NULL == pModuleStatus)
	{
		LOG_RUN_ERROR("pModuleStatus is null.");
		return;
	}

	// ɸѡ��Ϣ
	//int iModuleStatus = pModuleStatus->getModuleStatus();
	/*if (KICK_OFF != iModuleStatus && PASSWORD_CHANGE != iModuleStatus && USER_DELETE != iModuleStatus)
	{
	return;
	}*/

	// ��ȡxml�ַ���
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyModuleStatus(pModuleStatus, xmlStr);
	delete pModuleStatus;
	pModuleStatus = NULL;
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("SetXml_NotifyModuleStatus failed.");
		return;
	}

	// �����߼��������ݲ������߼�����
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_MODULESTATUS_REQ, xmlStr, iRet);
}

//��������Ϣ�ϱ���GUI
void DisplayMgr::notifySMS(SdsMessageIndicator* pSdsMessage)
{
	if (NULL == pSdsMessage)
	{
		LOG_RUN_ERROR("pSdsMessage is null.");
		return;
	}
	// ��ȡxml�ַ���
	std::string xmlStr;
	// ɸѡ��Ϣ
	bool bSdsDirection = pSdsMessage->getSdsDirection();
	if (!bSdsDirection)
	{
		int iRet = XMLProcess::SetXml_NotifySDSReport(pSdsMessage, xmlStr);
		delete pSdsMessage;
		pSdsMessage = NULL;
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifySDSReport failed.");
			return;
		}

		// �����߼��������ݲ������߼�����
		PUSH_LOGIC_QUEUE(ELTE_SERVICE_SDSREPORT_REQ, xmlStr, iRet);
	}
	else
	{
		int iRet = XMLProcess::SetXml_NotifySDSSendStatus(pSdsMessage, xmlStr);
		delete pSdsMessage;
		pSdsMessage = NULL;
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetXml_NotifySDSSendStatus failed.");
			return;
		}

		// �����߼��������ݲ������߼�����
		PUSH_LOGIC_QUEUE(ELTE_SERVICE_SDSSENDSTATUS_REQ, xmlStr, iRet);
	}
}

//֪ͨ���ö���ı����Ϣ�� ����Ϊһ����ָ̬�룬 ʹ������Ҫdowncast Ϊ��Ӧ�ı������
void DisplayMgr::notifyProvisionChange(ProvChangeIndicator* pProvChange)
{
	LOG_TRACE();

	if (NULL == pProvChange)
	{
		LOG_RUN_ERROR("pProvChange is null.");
		return;
	}

	// ��ȡxml�ַ���
	std::string xmlStr;
	int iRet = XMLProcess::SetXml_NotifyProvisionChange(pProvChange, xmlStr);
	delete pProvChange;
	pProvChange = NULL;
	if (eLTE_SVC_ERR_SUCCESS != iRet)
	{
		LOG_RUN_WARN("SetXml_NotifyProvisionChange failed.");
		return;
	}

	// �����߼��������ݲ������߼�����
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_PROVISIONCHANGE_REQ, xmlStr, iRet);
}

//֪ͨ���ö����ȫͬ���� ����Ϊһ����ָ̬�룬 ʹ������Ҫdowncast Ϊ��Ӧ�ı������
void DisplayMgr::notifyProvisionAllResync(ProvChangeIndicator* pProvChangeIndicator)
{
	LOG_TRACE();

	int iRet = eLTE_SVC_ERR_SUCCESS;
	std::string xmlStr("");

	if(NULL != pProvChangeIndicator)
	{
		delete pProvChangeIndicator;
		pProvChangeIndicator = NULL;
	}

	// �����߼��������ݲ������߼�����
	PUSH_LOGIC_QUEUE(ELTE_SERVICE_PROVISIONALLRESYNC_REQ, xmlStr, iRet);
}

//֪ͨ״̬�����ȫͬ���� ����Ϊһ����ָ̬�룬 ʹ������Ҫdowncast Ϊ��Ӧ�ı������
void DisplayMgr::notifyStatusAllResync(ResourceStatusIndicator* pResourceStatusIndicator)
{
	if(NULL != pResourceStatusIndicator)
	{
		delete pResourceStatusIndicator;
		pResourceStatusIndicator = NULL;
	}
}
