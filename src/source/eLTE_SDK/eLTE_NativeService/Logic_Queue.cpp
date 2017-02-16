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
#include <process.h>
#include "Logic_Queue.h"
#include "SSL_Socket.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "XMLProcess.h"
#include "OperationMgr.h"
#include "ProvisionMgr.h"
#include "SessionMgr.h"
#include "eLTE_Tool.h"
#include "ConfigMgr.h"
#include "eLTE_Message.h"

#define SSL_CHECK(data)					\
	LOG_TRACE();						\
	SSL* _ssl = (SSL*)data.ssl;			\
	if (NULL == _ssl)					\
	{									\
		LOG_RUN_INFO("_ssl is null.");	\
		return;							\
	}									\

#define SSL_WRITE(ssl, buf, size) {\
	int iWriteSize = SSL_write(ssl, buf, (int)(size));\
	if ((int)(size) != iWriteSize)\
	{\
		LOG_RUN_ERROR("SSL_write failed. bufsize = %d, writesize = %d", (size), iWriteSize);\
		return;\
	}\
}

#define SSL_WRITE_SAFE(ssl, buf, size, mem) {\
	int iWriteSize = SSL_write(ssl, buf, (int)(size));\
	if ((int)(size) != iWriteSize)\
	{\
		LOG_RUN_ERROR("SSL_write failed. bufsize = %d, writesize = %d", (size), iWriteSize);\
		if (NULL != mem)\
		{\
			delete[] mem;\
			mem = NULL;\
		}\
		return;\
	}\
}


Logic_Queue::Logic_Queue(void)
	: m_hRun(NULL)
	, m_hMutex(NULL)
{

}

Logic_Queue::~Logic_Queue(void)
{
	try
	{
		Clean();
	}
	catch(...)
	{
		
	}
}//lint !e1579

Logic_Queue& Logic_Queue::Instance()
{
	static Logic_Queue s_Logic_Queue;
	return s_Logic_Queue;
}

int Logic_Queue::Init()
{
	LOG_TRACE();
	if (NULL != m_hRun)
	{
		LOG_RUN_ERROR("The thread is exist.");
		return eLTE_SVC_ERR_FAILED;
	}

	if (NULL != m_hMutex)
	{
		LOG_RUN_ERROR("The mutex is exist.");
		return eLTE_SVC_ERR_FAILED;
	}

	// ����������
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	if (NULL == m_hMutex)
	{
		LOG_RUN_ERROR("Create mutex failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	// �����߼��߳�
	m_hRun = (HANDLE)_beginthreadex(NULL, 0, Run, NULL, CREATE_SUSPENDED, NULL);
	if (NULL == m_hRun)
	{
		LOG_RUN_ERROR("Create thread failed.");
		return eLTE_SVC_ERR_FAILED;
	}
	ResumeThread(m_hRun);

	return eLTE_SVC_ERR_SUCCESS;
}

void Logic_Queue::Uninit()
{
	LOG_TRACE();
	if (NULL != m_hRun)
	{
		const int iTimeOut = 5000;
		DWORD dwRet = WaitForSingleObject(m_hRun, iTimeOut);
		if (WAIT_TIMEOUT == dwRet)
		{
			LOG_RUN_ERROR("Wait Run thread stop failed.");
		}
		CloseHandle(m_hRun);
		m_hRun = NULL;
	}

	Clean();
}

void Logic_Queue::Clean()
{
	if (NULL != m_hRun)
	{
		CloseHandle(m_hRun);
		m_hRun = NULL;
	}
	if (NULL != m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
	while (!m_logicQueue.empty())
	{
		QUEUE_DATA& _data = m_logicQueue.front();
		if (NULL != _data.Value)
		{
			delete[] _data.Value;
		}
		m_logicQueue.pop();
	}
}

int Logic_Queue::Push(const QUEUE_DATA& data)
{
	LOG_TRACE();
	if (NULL == m_hMutex)
	{
		LOG_RUN_ERROR("m_hMutex is null, please init logic queue first.");
		return eLTE_SVC_ERR_FAILED;
	}

	// ���������߼�����
	(void)WaitForSingleObject(m_hMutex, INFINITE);
	m_logicQueue.push(data);
	ReleaseMutex(m_hMutex);

	return eLTE_SVC_ERR_SUCCESS;
}

unsigned __stdcall Logic_Queue::Run(void*)
{
	LOG_TRACE();
	while (!SSL_Socket::Instance().m_bStopSvc)
	{
		if (!Instance().m_logicQueue.empty())
		{
			// ����
			if (NULL != Instance().m_hMutex)
			{				
				(void)WaitForSingleObject(Instance().m_hMutex, INFINITE);
			}
			// ���߼���������������
			std::queue<QUEUE_DATA>::reference _front = Instance().m_logicQueue.front();
			QUEUE_DATA _data(_front);
			// ���ݳ���
			Instance().m_logicQueue.pop();
			// ����
			if (NULL != Instance().m_hMutex)
			{
				ReleaseMutex(Instance().m_hMutex);
			}

			// �ص���Ϣ����
			if (ELTE_SERVICE_USERSTATUS_REQ == _data.MsgType
				|| ELTE_SERVICE_MODULESTATUS_REQ == _data.MsgType
				|| ELTE_SERVICE_PROVISIONALLRESYNC_REQ == _data.MsgType
				|| ELTE_SERVICE_RESOURCESTATUS_REQ == _data.MsgType
				|| ELTE_SERVICE_PROVISIONCHANGE_REQ == _data.MsgType
				|| ELTE_SERVICE_P2PVIDEOCALLSTATUS_REQ == _data.MsgType
				|| ELTE_SERVICE_P2PCALLSTATUS_REQ == _data.MsgType
				|| ELTE_SERVICE_GROUPCALLSTATUS_REQ == _data.MsgType
				|| ELTE_SERVICE_GISREPORT_REQ == _data.MsgType
				|| ELTE_SERVICE_GISSTATUS_REQ == _data.MsgType
				|| ELTE_SERVICE_SDSSENDSTATUS_REQ == _data.MsgType
				|| ELTE_SERVICE_SDSREPORT_REQ == _data.MsgType)
			{
				Instance().CallBackMsg(_data);
			}
			// �ص���ϢRespond
			else if (ELTE_SERVICE_USERSTATUS_RSP == _data.MsgType
				|| ELTE_SERVICE_MODULESTATUS_RSP == _data.MsgType
				|| ELTE_SERVICE_PROVISIONALLRESYNC_RSP == _data.MsgType
				|| ELTE_SERVICE_RESOURCESTATUS_RSP == _data.MsgType
				|| ELTE_SERVICE_PROVISIONCHANGE_RSP == _data.MsgType
				|| ELTE_SERVICE_P2PVIDEOCALLSTATUS_RSP == _data.MsgType
				|| ELTE_SERVICE_P2PCALLSTATUS_RSP == _data.MsgType
				|| ELTE_SERVICE_GROUPCALLSTATUS_RSP == _data.MsgType
				|| ELTE_SERVICE_GISREPORT_RSP == _data.MsgType
				|| ELTE_SERVICE_GISSTATUS_RSP == _data.MsgType)
			{
				Instance().RespondMsg(_data);
			}
			// ҵ�������Ϣ
			else
			{
				Instance().OperationMsg(_data);
			}

			if (NULL != _data.Value)
			{
				delete[] _data.Value;
			}
		}
		else
		{
			// �ȴ�5����
			Sleep(5);
		}
	}

	return 0;
}

void Logic_Queue::OperationMsg(const QUEUE_DATA& data) const
{
	// SessionIDУ��
	if (ELTE_SERVICE_CONNECT_REQ != data.MsgType)
	{
		char sessionid[SESSIONID_LENGTH+1] = {0};
		eSDK_MEMCPY(sessionid, sizeof(sessionid), data.SessionID, SESSIONID_LENGTH);
		if (0 != strcmp(sessionid, SessionMgr::Instance().GetSessionID().c_str()))
		{
			LOG_RUN_ERROR("Invalid Session ID. MsgType=%d, client sessionID=%s", data.MsgType, sessionid);
			SSL* _ssl = (SSL*)data.ssl;
			if (NULL == _ssl)
			{
				LOG_RUN_INFO("_ssl is null.");
				return;
			}
			if (SSL_ST_OK == SSL_state(_ssl))
			{
				PACKET_HEAD head(data);
				head.MsgType |= RSP_MSG_FLAG;
				head.PacketLength = PACKET_HEAD_SIZE;
				head.RspCode = eLTE_SVC_ERR_INVALID_SESSIONID;

				// ������Ϣͷ
				SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			}
			else
			{
				// ssl״̬����ȷ���п��������жϣ�����������
				LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
			}
		}
	}

	// ͨ�ýӿ�
	if (/*ELTE_INTERFACE_UNIVERSAL_REQ_BEGIN <= data.MsgType &&*/ ELTE_INTERFACE_UNIVERSAL_REQ_END >= data.MsgType)
	{
		InterfaceUniversal(data);
	}
	// �û�����ӿ�
	else if (ELTE_INTERFACE_USERMGR_REQ_BEGIN <= data.MsgType && ELTE_INTERFACE_USERMGR_REQ_END >= data.MsgType)
	{
		InterfaceUserMgr(data);
	}
	// �豸����ӿ�
	else if (ELTE_INTERFACE_DEVICEMGR_REQ_BEGIN <= data.MsgType && ELTE_INTERFACE_DEVICEMGR_REQ_END >= data.MsgType)
	{
		InterfaceDeviceMgr(data);
	}
	// ý��ӿ�
	else if (ELTE_INTERFACE_MEDIAMGR_REQ_BEGIN <= data.MsgType && ELTE_INTERFACE_MEDIAMGR_REQ_END >= data.MsgType)
	{
		InterfaceMediaMgr(data);
	}
	// �Ƿ��ӿ�
	else
	{
		LOG_RUN_ERROR("Invalid MsgType: %x", data.MsgType);
	}
}

void Logic_Queue::CallBackMsg(const QUEUE_DATA& data) const
{
	LOG_TRACE();
	LOG_RUN_DEBUG("CallBack Msg: %x", data.MsgType);
	SSL* _ssl = (SSL*)data.ssl;
	if (NULL == _ssl)
	{
		LOG_RUN_INFO("_ssl is null.");
		return;
	}
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ��ȡ��Ϣͷ
		const PACKET_HEAD* _head = &data;
		// ������Ϣͷ
		SSL_WRITE(_ssl, _head, PACKET_HEAD_SIZE);
		// ���Ͱ���
		if (NULL != data.Value && data.PacketLength > PACKET_HEAD_SIZE)
		{
			int size = (int)(data.PacketLength-PACKET_HEAD_SIZE);
			SSL_WRITE(_ssl, data.Value, size);
		}
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

void Logic_Queue::RespondMsg(const QUEUE_DATA& data) const
{
	LOG_TRACE();
	LOG_RUN_DEBUG("Respond Msg: %x", data.MsgType);
	if (eLTE_SVC_ERR_SUCCESS != data.RspCode)
	{
		LOG_RUN_ERROR("Client respond the callback msg faild. msgType = %x", data.MsgType);
	}
}

// ͨ�ýӿ�
void Logic_Queue::InterfaceUniversal(const QUEUE_DATA& data) const
{
	switch (data.MsgType)
	{
		// ���ӷ���
	case ELTE_SERVICE_CONNECT_REQ:
		{
			Connect(data);
		}
		break;
		// �Ͽ�����
	case ELTE_SERVICE_DISCONNECT_REQ:
		{
			Disconnect(data);
		}
		break;
		// ����
	case ELTE_SERVICE_KEEPALIVE_REQ:
		{
			KeepAlive(data);
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid MsgType: %x", data.MsgType);
		}
		break;
	}
}

// �û�����ӿ�
void Logic_Queue::InterfaceUserMgr(const QUEUE_DATA& data) const
{
	switch (data.MsgType)
	{
		// ��¼
	case ELTE_SERVICE_LOGIN_REQ:
		{
			Login(data);
		}
		break;
		// ע��
	case ELTE_SERVICE_LOGOUT_REQ:
		{
			Logout(data);
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid MsgType: %x", data.MsgType);
		}
		break;
	}
}

// �豸����ӿ�
void Logic_Queue::InterfaceDeviceMgr(const QUEUE_DATA& data) const
{
	switch (data.MsgType)
	{
		// ����״̬�ϱ�
	case ELTE_SERVICE_TRIGGERSTATUSREPORT_REQ:
		{
			TriggerStatusReport(data);
		}
		break;
		// ���ù����ʼ��
	case ELTE_SERVICE_PROVISIONMANAGERINIT_REQ:
		{
			ProvisionInit(data);
		}
		break;
		// ý���������ʼ��
	case ELTE_SERVICE_PROVISIONMANAGERINITMRS_REQ:
		{
			ProvisionInitMRS(data);
		}
		break;
		// �˳����ù���
	case ELTE_SERVICE_PROVISIONMANAGEREXIT_REQ:
		{
			ProvisionExit(data);
		}
		break;
		// ��ȡ����̨Ⱥ���б�
	case ELTE_SERVICE_GETDCGROUPS_REQ:
		{
			GetDcGroups(data);
		}
		break;
		// ��ȡ�ɽ���������Ϣ
	case ELTE_SERVICE_GETPATCHGROUPINFO_REQ:
		{
			GetPatchGroupInfo(data);
		}
		break;
		// ��ѯ�ն˵�GIS������Ϣ
	case ELTE_SERVICE_GETUSERSPECIFICGISCFG_REQ:
		{
			GetUserSpecificGISCfg(data);
		}
		break;
		// ��ȡ����̨���ĵ��ն��б�
	case ELTE_SERVICE_GETGISSUBSCRIPTION_REQ:
		{
			GetGisSubscription(data);
		}
		break;
		// �����ն˵�GIS������Ϣ
	case ELTE_SERVICE_SETUSERSPECIFICGISCFG_REQ:
		{
			SetUserSpecificGISCfg(data);
		}
		break;
		// ��ȡ����̨��������ɽ���
	case ELTE_SERVICE_GETPATCHGROUPS_REQ:
		{
			GetPatchGroups(data);
		}
		break;
		// ��ȡ�ɽ���ĳ�Ա
	case ELTE_SERVICE_GETPATCHGROUPMEMBERS_REQ:
		{
			GetGroupMemberByPatchId(data);
		}
		break;
		// ��ȡ����̨�û��б�
	case ELTE_SERVICE_GETDCUSERS_REQ:
		{
			GetDcUsers(data);
		}
		break;
		// ��ȡһ��Ⱥ���Ա�б�
	case ELTE_SERVICE_GETGROUPUSERS_REQ:
		{
			GetGroupUsers(data);
		}
		break;
		// ��ȡһ��Ⱥ����ϸ��Ϣ
	case ELTE_SERVICE_GETGROUPINFO_REQ:
		{
			GetGroupInfo(data);
		}
		break;
		// ��ȡһ���û���ϸ��Ϣ
	case ELTE_SERVICE_GETUSERINFO_REQ:
		{
			GetUserInfo(data);
		}
		break;
		// ��ȡһ������̨��ϸ��Ϣ
	case ELTE_SERVICE_GETDCINFO_REQ:
		{
			GetDcInfo(data);
		}
		break;
		// ����Ⱥ��
	case ELTE_SERVICE_SUBSCRIBEGROUP_REQ:
		{
			SubscribeGroup(data);
		}
		break;
		// ���Ĳ��Զ�����Ⱥ��
	case ELTE_SERVICE_SUBJOINGROUP_REQ:
		{
			SubJoinGroup(data);
		}
		break;
		// ��ȡ¼���ļ��б�
	case ELTE_SERVICE_GETUSERRECFILEINFOLIST_REQ:
		{
			GetUserRECFileInfoList(data);
		}
		break;
		// ������̬����
	case ELTE_SERVICE_CREATEDYNAMICGROUP_REQ:
		{
			CreateDynamicGroup(data);
		}
		break;
		// ɾ����̬����
	case ELTE_SERVICE_CANCELDYNAMICGROUP_REQ:
		{
			CancelDynamicGroup(data);
		}
		break;
		// �޸Ķ�̬Ⱥ��
	case ELTE_SERVICE_MODIFYDYNAMICGROUP_REQ:
		{
			ModifyDynamicGroup(data);
		}
		break;
		// ��������ʱ�û�
// 	case ELTE_SERVICE_TEMPUSERJOINGROUP_REQ:
// 		{
// 			TempUserJoinGroup(data);
// 		}
// 		break;
		// �����ɽ��顢�����ɽ����Ա��ɾ���ɽ����Ա
	case ELTE_SERVICE_CREATEPATCHGROUP_REQ:
	case ELTE_SERVICE_ADDPATCHMEMBER_REQ:
	case ELTE_SERVICE_DELETEPATCHMEMBER_REQ:
		{
			OperatePatchGroup(data);
		}
		break;
		// ɾ���ɽ���
	case ELTE_SERVICE_CANCELPATCHGROUP_REQ:
		{
			CancelPatchGroup(data);
		}
		break;
		// ��ȡ��ʱ���
	case ELTE_SERVICE_GETTEMPGROUPID_REQ:
		{
			GetTempGroupID(data);
		}
		break;
		// ������ʱ��
	case ELTE_SERVICE_CREATETEMPGROUP_REQ:
		{
			CreateTempGroup(data);
		}
		break;
		// ����/ȥ����GIS�ն�
	case ELTE_SERVICE_GISSUBSCRIBE_REQ:
		{
			GISSubscribe(data);
		}
		break;
	case ELTE_SERVICE_SDSSEND_REQ:
		{
			SDSSendMessage(data);
		}
		break;
	case ELTE_SERVICE_GETVWALLLIST_REQ:
		{
			GetDcVWallIDList(data);
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid MsgType: %x", data.MsgType);
		}
		break;
	}
}

// ý��ӿ�
void Logic_Queue::InterfaceMediaMgr(const QUEUE_DATA& data) const
{
	switch (data.MsgType)
	{
			// ��ʼ��Ƶ�ַ�
		case ELTE_SERVICE_STARTDISPATCHVIDEO_REQ:
			{
				StartVideoDispatch(data);
			}
			break;
			// ֹͣ��Ƶ�ַ�
		case ELTE_SERVICE_STOPDISPATCHVIDEO_REQ:
			{
				StopVideoDispatch(data);
			}
			break;
			// ��ʼ����ʵʱ��Ƶ
		case ELTE_SERVICE_STARTREALPLAY_REQ:
			{
				StartRealPlay(data);
			}
			break;
			// ������Ƶ�ش�
		case ELTE_SERVICE_RECVVIDEOPLAY_REQ:
			{
				RecvVideoPlay(data);
			}
			break;
			// ��ʼ��̨����
		case ELTE_SERVICE_PTZCONTROL_REQ:
			{
				PTZControl(data);
			}
			break;
			// ֹͣ����ʵʱ��Ƶ
		case ELTE_SERVICE_STOPREALPLAY_REQ:
			{
				StopRealPlay(data);
			}
			break;
			// �����������
		case ELTE_SERVICE_P2PDIAL_REQ:
			{
				P2PDial(data);
			}
			break;
			// �����˫���������
		case ELTE_SERVICE_P2PHALFDPXDIAL_REQ:
			{
				P2PHalfDpxDial(data);
			}
			break;
			// �ͷŰ�˫���������
		case ELTE_SERVICE_P2PHALFDPXRELEASE_REQ:
			{
				P2PHalfDpxRelease(data);
			}
			break;
			// �����������
		case ELTE_SERVICE_P2PRECV_REQ:
			{
				P2PRecv(data);
			}
			break;
			// �ܽ��������
		case ELTE_SERVICE_P2PREJECT_REQ:
			{
				P2PReject(data);
			}
			break;
			// �Ҷ��������
		case ELTE_SERVICE_P2PHANGUP_REQ:
			{
				P2PHangup(data);
			}
			break;
			// ����̨ǿ����
		case ELTE_SERVICE_P2PBREAKOFF_REQ:
			{
				P2PBreakoff(data);
			}
			break;
			// ������ǿ����+�����µ����
		case ELTE_SERVICE_P2PBREAKIN_REQ:
			{
				P2PBreakin(data);
			}
			break;
			// ����������̶�����ʱ���������Ȩ
		case ELTE_SERVICE_PTTDIAL_REQ:
			{
				PTTDial(data);
			}
			break;
			// �ͷŻ�Ȩ
		case ELTE_SERVICE_PTTRELEASE_REQ:
			{
				PTTRelease(data);
			}
			break;
			// �˳����
		case ELTE_SERVICE_PTTHANGUP_REQ:
			{
				PTTHangup(data);
			}
			break;
			// ����������
		case ELTE_SERVICE_PTTEMERGENCY_REQ:
			{
				PTTEmergency(data);
			}
			break;
			// ����̨ǿ�����
		case ELTE_SERVICE_GROUPBREAKOFF_REQ:
			{
				GroupBreakoff(data);
			}
			break;
			// ִ�о�����������������Ƶ�ش���
		case ELTE_SERVICE_VOLUMEMUTE_REQ:
			{
				VolMute(data);
			}
			break;
			// ȡ��������������������Ƶ�ش���
		case ELTE_SERVICE_VOLUMEUNMUTE_REQ:
			{
				VolUnMute(data);
			}
			break;
			//��ʼ��Ƶ��ǽ
		case ELTE_SERVICE_VWALLSTART_REQ:
			{
				StartVWall(data);
			}
			break;
			//ֹͣ��Ƶ��ǽ
		case ELTE_SERVICE_VWALLSTOP_REQ:
			{			
				VWallStop(data);
			}
			break;			
// 			//����PSTN/PLMN�绰����
// 		case ELTE_SERVICE_TELEPHONEDIAL_REQ:
// 			{			
// 				TelephoneDial(data);
// 			}
// 			break; 			
// 			//�Ҷ�PSTN/PLMN�绰����
// 		case ELTE_SERVICE_TELEPHONEHANGUP_REQ:
// 			{			
// 				TelephoneHangup(data);
// 			}
// 			break;
			//�������ܼ���
		case ELTE_SERVICE_STARTDISCREETLISTEN_REQ:
			{			
				StartDiscreetListen(data);
			}
			break;
			//ֹͣ���ܼ���
		case ELTE_SERVICE_STOPDISCREETLISTEN_REQ:
			{
				StopDiscreetListen(data);
			}
			break;
			//������������
		case ELTE_SERVICE_STARTENVIRLISTEN_REQ:
			{
				StartEnvironmentListen(data);
			}
			break;
			//�˹�ת��
		case ELTE_SERVICE_P2PTRANSFER_REQ:
			{
				P2PTransfer(data);
			}
			break;
			//Ĭ��
		default:
			{
				LOG_RUN_ERROR("Invalid MsgType: %x", data.MsgType);
			}
			break;
		}
}

// ���ӷ���
void Logic_Queue::Connect(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		std::string connectpasswd;
		int iRet = XMLProcess::GetXml_Connect_Req(data.Value, connectpasswd);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_Connect_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// У�������Ƿ���ȷ������ȷ�ͶϿͻ�������
		std::string strIniFile = eLTE_Tool::GetServicePath();
		strIniFile.append(CONFIG_FILE_NAME);

		// ҵ����
		char sessId[SESSIONID_LENGTH+1] = {0};
		eSDK_MEMCPY(sessId, sizeof(sessId), head.SessionID, SESSIONID_LENGTH);
		// sessionidΪ��.   1.��һ�ε�½ 2.���߱���ʱ�������µ�½
		if (0 == strcmp(sessId, ""))
		{
			// 1.��һ�ε�½
			if ("" == SessionMgr::Instance().GetSessionID())
			{
				// ����SessionID
				iRet = SessionMgr::Instance().NewSessionID();
				if (eLTE_SVC_ERR_SUCCESS != iRet)
				{
					LOG_RUN_ERROR("NewSessionID failed.");
					head.RspCode = iRet;
					// ������Ϣͷ
					SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
					return;
				}

				// ����SessionID
				head.RspCode = iRet;
				eSDK_MEMCPY(head.SessionID, sizeof(head.SessionID), SessionMgr::Instance().GetSessionID().c_str(), SESSIONID_LENGTH);

				// ��������
				(void)SessionMgr::Instance().KeepAliveStart();
			}
			// 2.���߱���ʱ�������µ�½����Ҫ�ͻ��������������
			else
			{
				head.RspCode = eLTE_SVC_ERR_SERVICE_EXPIRE;
			}
		}
		// sessionid���ڷ����¼��sessionid. ��Ϊ��������
		else if (0 == strcmp(sessId, SessionMgr::Instance().GetSessionID().c_str()))
		{
			// �߼�����.........����ֱ�ӷ��سɹ��������������
			head.RspCode = eLTE_SVC_ERR_SUCCESS;
		}
		// sessionidΪ�ǿշǷ�ֵ.
		else
		{
			LOG_RUN_ERROR("Client Session ID is invalid value.");
			head.RspCode = eLTE_SVC_ERR_INVALID_SESSIONID;
			eSDK_MEMSET(head.SessionID, 0, SESSIONID_LENGTH);
		}

		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �Ͽ�����
void Logic_Queue::Disconnect(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;
		head.RspCode = eLTE_SVC_ERR_SUCCESS;

		// ֹͣ����
		SessionMgr::Instance().KeepAliveStop();

		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}

	// ֹͣ����
	SSL_Socket::Instance().m_bStopSvc = true;
}

// ����
void Logic_Queue::KeepAlive(const QUEUE_DATA& data) const
{
	//LOG_TRACE();
	SSL* _ssl = (SSL*)data.ssl;
	if (NULL == _ssl)
	{
		LOG_RUN_INFO("_ssl is null.");
		return;
	}
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// ˢ�±���ʱ��
		SessionMgr::Instance().KeepAliveFreshen();
		head.RspCode = eLTE_SVC_ERR_SUCCESS;

		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��¼
void Logic_Queue::Login(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		std::string userId;
		std::string passwd;
		std::string serverIP;
		std::string localIP;
		std::string sipPort;
		int iRet = XMLProcess::GetXml_Login_Req(data.Value, userId, passwd, serverIP, localIP, sipPort);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_Login_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().Login(userId, passwd, serverIP, localIP, sipPort);
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ע��
void Logic_Queue::Logout(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		std::string userId;
		int iRet = XMLProcess::GetXml_Logout_Req(data.Value, userId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_Logout_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().Logout(userId);
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ����״̬�ϱ�
void Logic_Queue::TriggerStatusReport(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		unsigned int uiEnable = 0;
		int iRet = XMLProcess::GetXml_TriggerStatusReport_Req(data.Value, uiEnable);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_TriggerStatusReport_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().TriggerStatusReport(uiEnable);
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ���ù����ʼ��
void Logic_Queue::ProvisionInit(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		std::string serverIP;
		int userId;
		int iRet = XMLProcess::GetXml_ProvisionManagerInit_Req(data.Value, serverIP, userId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_ProvisionManagerInit_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = ProvisionMgr::Instance().Init(serverIP, userId);
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ý���������ʼ��
void Logic_Queue::ProvisionInitMRS(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		std::string serverIP;
		int iRet = XMLProcess::GetXml_ProvisionManagerInitMRS_Req(data.Value, serverIP);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_ProvisionManagerInitMRS_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = ProvisionMgr::Instance().InitMRS(serverIP);
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �˳����ù���
void Logic_Queue::ProvisionExit(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ҵ����
		int iRet = ProvisionMgr::Instance().Exit();
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡ����̨Ⱥ���б�
void Logic_Queue::GetDcGroups(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_DCGROUPS_HEAD();

		// ҵ����
		DcGroups* pGroups = NULL;
		iRet = ProvisionMgr::Instance().GetDcGroups(userId, pGroups);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pGroups)
		{
			LOG_RUN_ERROR("GetDcGroups failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetDcGroups_Rsp(pGroups, xmlStr);
		//release pGroups
		if (pGroups)
		{
			DeleteVecotor(pGroups->begin(), pGroups->end());
			delete pGroups;
			pGroups = NULL;
		}
		
		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetDcGroups_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡ�ɽ���ĳ�Ա�б�
void Logic_Queue::GetGroupMemberByPatchId(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		CONSTRUCT_AND_WRITE_GROUPUSERS_REQ();

		// ҵ����
		PatchGroupMembers* pPatchGroupMembers = NULL;
		iRet = ProvisionMgr::Instance().GetGroupMemberByPatchId(groupId, pPatchGroupMembers);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pPatchGroupMembers)
		{
			LOG_RUN_ERROR("GetGroupMemberByPatchId failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetGroupMemberByPatchId_Rsp(pPatchGroupMembers, xmlStr);
		//release pPatchGroupMembers
		if (pPatchGroupMembers)
		{
			DeleteVecotor(pPatchGroupMembers->begin(), pPatchGroupMembers->end());
			delete pPatchGroupMembers;
			pPatchGroupMembers = NULL;
		}

		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetGroupMemberByPatchId_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �����ն˵�GIS������Ϣ
void Logic_Queue::SetUserSpecificGISCfg(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		unsigned resId;
		unsigned reportPeriod;
		unsigned reportDistance;
		int iRet = XMLProcess::GetXml_SetUserSpecificGISCfg_Req(data.Value, resId, reportPeriod, reportDistance);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_SetUserSpecificGISCfg_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		UEGisCfg_parameter param;
		param.userid = resId;
		param.reportDistance = reportDistance;
		param.reportPeriod = reportPeriod;
		iRet = OperationMgr::Instance().SetUserSpecificGISCfg(resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SetUserSpecificGISCfg failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ѯ�ն˵�GIS������Ϣ
void Logic_Queue::GetUserSpecificGISCfg(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		std::string resId;
		int iRet = XMLProcess::GetXml_GetResourceID_Req(data.Value, resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_GetResourceID_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		ProvGisCfg* pProvGisCfg = NULL;
		iRet = ProvisionMgr::Instance().GetUserSpecificGISCfg(eLTE_Tool::String2Int(resId), pProvGisCfg);
		
		// ����xml�ַ���
		std::string xmlStr;

		//������ն�û��gis������Ϣ,��pProvGisCfgΪNULL
		if(NULL == pProvGisCfg)
		{
			xmlStr = "<Content><UeGisCfgInfo></UeGisCfgInfo></Content>";
		}

		else
		{
			iRet = XMLProcess::SetXml_GetUserSpecificGISCfg_Rsp(pProvGisCfg, xmlStr);
			//release pProvGisCfg
			if (pProvGisCfg)
			{
				delete pProvGisCfg;
				pProvGisCfg = NULL;
			}

			if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
			{
				LOG_RUN_ERROR("SetXml_GetUserSpecificGISCfg_Rsp failed.");
				head.RspCode = iRet;
				// ������Ϣͷ
				SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
				return;
			}
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡ����̨���ĵ��ն��б�
void Logic_Queue::GetGisSubscription(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_RESOURCE_HEAD();

		// ҵ����
		GisQuerySubList* pGisQuerySubList = NULL;
		iRet = ProvisionMgr::Instance().GetGisSubscription(resId, pGisQuerySubList);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pGisQuerySubList)
		{
			LOG_RUN_ERROR("GetGisSubscription failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetGisSubscription_Rsp(pGisQuerySubList, xmlStr);
		if(pGisQuerySubList)
		{
			DeleteVecotor(pGisQuerySubList->begin(), pGisQuerySubList->end());
			delete pGisQuerySubList;
			pGisQuerySubList = NULL;
		}
		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetGisSubscription_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡ�ɽ���������Ϣ
void Logic_Queue::GetPatchGroupInfo(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		CONSTRUCT_AND_WRITE_GROUPUSERS_REQ();

		// ҵ����
		PatchGroupInfo* pPatchGroupInfo = NULL;
		iRet = ProvisionMgr::Instance().GetPatchGroupInfo(groupId, pPatchGroupInfo);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pPatchGroupInfo)
		{
			LOG_RUN_ERROR("GetPatchGroupInfo failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetPatchGroupInfo_Rsp(pPatchGroupInfo, xmlStr);
		//release pPatchGroupInfo
		if (pPatchGroupInfo)
		{
			delete pPatchGroupInfo;
			pPatchGroupInfo = NULL;
		}

		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetPatchGroupInfo_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡ����̨�ɽ����б�
void Logic_Queue::GetPatchGroups(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_DCGROUPS_HEAD();

		// ҵ����
		PatchGroupsList* pPatchGroupsList = NULL;
		iRet = ProvisionMgr::Instance().GetPatchGroups(userId, pPatchGroupsList);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pPatchGroupsList)
		{
			LOG_RUN_ERROR("GetPatchGroups failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetPatchGroups_Rsp(pPatchGroupsList, xmlStr);
		//release pPatchGroupsList
		if (pPatchGroupsList)
		{
			DeleteVecotor(pPatchGroupsList->begin(), pPatchGroupsList->end());
			delete pPatchGroupsList;
			pPatchGroupsList = NULL;
		}

		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetPatchGroups_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡ����̨�û��б�
void Logic_Queue::GetDcUsers(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int userId;
		int iRet = XMLProcess::GetXml_GetDcUsers_Req(data.Value, userId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_GetDcUsers_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		DcUsers* pUsers = NULL;
		iRet = ProvisionMgr::Instance().GetDcUsers(userId, pUsers);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pUsers)
		{
			LOG_RUN_ERROR("GetDcUsers failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetDcUsers_Rsp(pUsers, xmlStr);
		//release pUsers
		if(pUsers)
		{
			DeleteVecotor(pUsers->begin(), pUsers->end());
			delete pUsers;
			pUsers = NULL;
		}
		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetDcUsers_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡһ��Ⱥ���Ա�б�
void Logic_Queue::GetGroupUsers(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		CONSTRUCT_AND_WRITE_GROUPUSERS_REQ();

		// ҵ����
		GrpUserList* pGrpUserList = NULL;
		iRet = ProvisionMgr::Instance().GetGroupUsers(groupId, pGrpUserList);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pGrpUserList)
		{
			LOG_RUN_ERROR("GetGroupUsers failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetGroupUsers_Rsp(pGrpUserList, xmlStr);
		//release pGrpUserList
		if(pGrpUserList)
		{
			DeleteVecotor(pGrpUserList->begin(), pGrpUserList->end());
			delete pGrpUserList;
			pGrpUserList = NULL;
		}
		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetGroupUsers_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡһ��Ⱥ����ϸ��Ϣ
void Logic_Queue::GetGroupInfo(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int groupId;
		int iRet = XMLProcess::GetXml_GetGroupInfo_Req(data.Value, groupId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_GetGroupInfo_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		GroupInfo* pGroupInfo = NULL;
		iRet = ProvisionMgr::Instance().GetGroupInfo(groupId, pGroupInfo);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pGroupInfo)
		{
			LOG_RUN_ERROR("GetGroupInfo failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetGroupInfo_Rsp(pGroupInfo, xmlStr);
		if(pGroupInfo)
		{
			delete pGroupInfo;
			pGroupInfo = NULL;
		}
		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetGroupInfo_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡһ���û���ϸ��Ϣ
void Logic_Queue::GetUserInfo(const QUEUE_DATA& data) const
{
	SSL_CHECK(data)
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int userId;
		int iRet = XMLProcess::GetXml_GetUserInfo_Req(data.Value, userId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_GetUserInfo_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		UserInfo* pUserInfo = NULL;
		iRet = ProvisionMgr::Instance().GetUserInfo(userId, pUserInfo);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pUserInfo)
		{
			LOG_RUN_ERROR("GetUserInfo failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetUserInfo_Rsp(pUserInfo, xmlStr);
		if(pUserInfo)
		{
			delete pUserInfo;
			pUserInfo = NULL;
		}
		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetUserInfo_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡһ������̨��ϸ��Ϣ
void Logic_Queue::GetDcInfo(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int userId;
		int iRet = XMLProcess::GetXml_GetDcInfo_Req(data.Value, userId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_GetDcInfo_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		DcProperty* pProperty = NULL;
		iRet = ProvisionMgr::Instance().GetDcInfo(userId, pProperty);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pProperty)
		{
			LOG_RUN_ERROR("GetDcInfo failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetDcInfo_Rsp(pProperty, xmlStr);
		if(pProperty)
		{
			delete pProperty;
			pProperty = NULL;
		}
		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetDcInfo_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ����Ⱥ��
void Logic_Queue::SubscribeGroup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int groupId;
		unsigned int uiEnable;
		int iRet = XMLProcess::GetXml_SubscribeGroup_Req(data.Value, groupId, uiEnable);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_SubscribeGroup_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().SubscribeGroup(groupId, uiEnable);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SubscribeGroup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡ¼���ļ��б�
void Logic_Queue::GetUserRECFileInfoList(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		RECQueryInfo query;
		int iRet = XMLProcess::GetXml_GetUserRECFileInfoList_Req(data.Value, query);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_GetUserRECFileInfoList_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		UserRecFileInfoList* pInfoList = NULL;
		iRet = ProvisionMgr::Instance().GetUserRECFileInfoList(&query, pInfoList);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == pInfoList)
		{
			LOG_RUN_ERROR("GetUserRECFileInfoList failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetUserRECFileInfoList_Rsp(pInfoList, xmlStr);
		if(pInfoList)
		{
			DeleteVecotor(pInfoList->begin(), pInfoList->end());
			delete pInfoList;
			pInfoList = NULL;
		}
		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetUserRECFileInfoList_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ������̬����
void Logic_Queue::CreateDynamicGroup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		DGNA_parameter param;
		int iRet = XMLProcess::GetXml_CreateDynamicGroup_Req(data.Value, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_CreateDynamicGroup_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().CreateDynamicGroup(param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("CreateDynamicGroup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ɾ����̬����
void Logic_Queue::CancelDynamicGroup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		int iRet = XMLProcess::GetXml_CancelDynamicGroup_Req(data.Value, resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_CancelDynamicGroup_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().CancelDynamicGroup(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("CancelDynamicGroup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �޸Ķ�̬����
void Logic_Queue::ModifyDynamicGroup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		DGNA_Modify_parameter param;
		int iRet = XMLProcess::GetXml_ModifyDynamicGroup_Req(data.Value, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_ModifyDynamicGroup_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().ModifyDynamicGroup(param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("ModifyDynamicGroup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��������ʱ�û�
// void Logic_Queue::TempUserJoinGroup(const QUEUE_DATA& data) const
// {
// 	SSL_CHECK(data);
// 	if (SSL_ST_OK == SSL_state(_ssl))
// 	{
// 		// ������Ϣͷ
// 		PACKET_HEAD head(data);
// 		head.MsgType |= RSP_MSG_FLAG;
// 		head.PacketLength = PACKET_HEAD_SIZE;
// 
// 		// xml����
// 		int resid;
// 		PhonePatch_parameter param;
// 		int iRet = XMLProcess::GetXml_TempUserJoinGroup_Req(data.Value, resid, param);
// 		if (eLTE_SVC_ERR_SUCCESS != iRet)
// 		{
// 			LOG_RUN_ERROR("GetXml_TempUserJoinGroup_Req failed.");
// 			head.RspCode = iRet;
// 			// ������Ϣͷ
// 			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
// 			return;
// 		}
// 
// 		// ҵ����
// 		iRet = OperationMgr::Instance().TempUserJoinGroup(resid, param);
// 		if (eLTE_SVC_ERR_SUCCESS != iRet)
// 		{
// 			LOG_RUN_ERROR("TempUserJoinGroup failed.");
// 		}
// 		head.RspCode = iRet;
// 		// ������Ϣͷ
// 		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
// 	}
// 	else
// 	{
// 		// ssl״̬����ȷ���п��������жϣ�����������
// 		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
// 	}
// }

// �����ɽ��顢�����ɽ����Ա��ɾ���ɽ����Ա
void Logic_Queue::OperatePatchGroup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		PCHGRP_Para param;
		int iRet = XMLProcess::GetXml_OperatePatchGroup_Req(data.Value, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_OperatePatchGroup_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().OperatePatchGroup(param, data.MsgType);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("OperatePatchGroup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ɾ���ɽ���
void Logic_Queue::CancelPatchGroup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		int iRet = XMLProcess::GetXml_CancelPatchGroup_Req(data.Value, resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_CancelPatchGroup_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().CancelPatchGroup(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("CancelPatchGroup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ȡ��ʱ���
void Logic_Queue::GetTempGroupID(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// ҵ����
		int iGroupId = OperationMgr::Instance().GetNextTempGroupID();
		if (0 > iGroupId)
		{
			LOG_RUN_ERROR("GetNextTempGroupID failed.");
			head.RspCode = iGroupId;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		int iRet = XMLProcess::SetXml_GetTempGroupID_Rsp(iGroupId, xmlStr);
		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetTempGroupID_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ������ʱ��
void Logic_Queue::CreateTempGroup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		DGNA_parameter param;
		int iRet = XMLProcess::GetXml_CreateTempGroup_Req(data.Value, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_CreateTempGroup_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().CreateTempGroup(param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("CreateTempGroup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ʼ��Ƶ�ַ�
void Logic_Queue::StartVideoDispatch(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		VIDEO_DISPATCH_PARAM param;
		int iRet = XMLProcess::GetXml_StartVideoDispatch_Req(data.Value, resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_StartVideoDispatch_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		Video_Dispatch_parameter videoDispatchParam;
		videoDispatchParam.channel = param.channel;
		videoDispatchParam.dispatch_num = param.dispatch_num;
		videoDispatchParam.dstviewerlist = param.dstviewerlist;
		videoDispatchParam.fmtvalue = param.fmtvalue.c_str();

		// ҵ����
		iRet = OperationMgr::Instance().VideoDispatch(resId, videoDispatchParam);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("VideoDispatch failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ֹͣ��Ƶ�ַ�
void Logic_Queue::StopVideoDispatch(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		Video_Dispatch_Delete param;
		int iRet = XMLProcess::GetXml_StopVideoDispatch_Req(data.Value, resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_StopVideoDispatch_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().StopVideoDispatch(resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("StopVideoDispatch failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ������Ƶ�ش�
void Logic_Queue::RecvVideoPlay(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		int iRet = XMLProcess::GetXml_RecvVideoPlay_Req(data.Value, resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_RecvVideoPlay_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		iRet = OperationMgr::Instance().RecvVideoPlay(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("RecvVideoPlay failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ʼ����ʵʱ��Ƶ
void Logic_Queue::StartRealPlay(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		VIDEO_PARAM param;
		int iRet = XMLProcess::GetXml_StartRealPlay_Req(data.Value, resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_StartRealPlay_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		Video_UL_parameter vParam;
		vParam.fmtvalue = (char*)param.VideoFormat.c_str();//lint !e1773
		vParam.cameratype = (char*)param.CameraType.c_str();//lint !e1773
		vParam.user_confirm_type = (char*)param.UserConfirmType.c_str();//lint !e1773
		vParam.mutetype = (char*)param.MuteType.c_str();//lint !e1773

		iRet = OperationMgr::Instance().VideoMonitor(resId, vParam);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("VideoMonitor failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ʼ��̨����
void Logic_Queue::PTZControl(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		FVG_control control;
		int iRet = XMLProcess::GetXml_PTZControl_Req(data.Value, resId, control);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_PTZControl_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().PTZControl(resId, control);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("PTZControl failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ֹͣ����ʵʱ��Ƶ
void Logic_Queue::StopRealPlay(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		int iRet = XMLProcess::GetXml_StopRealPlay_Req(data.Value, resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_StopRealPlay_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().VideoHangup(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("VideoHangup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �ͷŰ�˫�����
void Logic_Queue::P2PHalfDpxRelease(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_RESOURCE_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().P2PHalfDpxRelease(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("P2PHalfDpxRelease failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �����˫�����
void Logic_Queue::P2PHalfDpxDial(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_RESOURCE_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().P2PHalfDpxDial(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("P2PHalfDpxDial failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �����������
void Logic_Queue::P2PDial(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_RESOURCE_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().P2PDial(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("P2PDial failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �����������
void Logic_Queue::P2PRecv(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_RESOURCE_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().P2PRecv(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("P2PRecv failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �ܽ��������
void Logic_Queue::P2PReject(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_RESOURCE_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().P2PReject(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("P2PReject failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �Ҷ��������
void Logic_Queue::P2PHangup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_RESOURCE_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().P2PHangup(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("P2PHangup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ������ǿ����+�����µ����
void Logic_Queue::P2PBreakin(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_RESOURCE_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().P2PBreakin(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("P2PBreakin failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ǿ����
void Logic_Queue::P2PBreakoff(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_RESOURCE_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().P2PBreakoff(resId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("P2PBreakoff failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ���Ĳ��Զ�����Ⱥ��
void Logic_Queue::SubJoinGroup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_GROUP_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().SubJoinGroup(groupId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SubJoinGroup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ����������̶�����ʱ���������Ȩ
void Logic_Queue::PTTDial(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_GROUP_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().PTTDial(groupId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("PTTDial failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �ͷŻ�Ȩ
void Logic_Queue::PTTRelease(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_GROUP_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().PTTRelease(groupId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("PTTRelease failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// �˳����
void Logic_Queue::PTTHangup(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_GROUP_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().PTTHangup(groupId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("PTTHangup failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ����������
void Logic_Queue::PTTEmergency(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_GROUP_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().PTTEmergency(groupId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("PTTEmergency failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ǿ�����
void Logic_Queue::GroupBreakoff(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		CONSTRUCT_AND_WRITE_GROUP_HEAD();

		// ҵ����
		iRet = OperationMgr::Instance().GroupBreakoff(groupId);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GroupBreakoff failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ִ�о�����������������Ƶ�ش���
void Logic_Queue::VolMute(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		CONSTRUCT_AND_WRITE_VOL_HEAD();

		// ҵ����
		Mute_parameter param;
		param.task_type = true;
		param.call_type = callType;
		iRet = OperationMgr::Instance().VolMute(resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("VolMute failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ȡ��������������������Ƶ�ش���
void Logic_Queue::VolUnMute(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		CONSTRUCT_AND_WRITE_VOL_HEAD();

		// ҵ����
		Mute_parameter param;
		param.task_type = false;
		param.call_type = callType;
		iRet = OperationMgr::Instance().VolUnMute(resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("VolUnMute failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

//���Ͷ�/����
void Logic_Queue::SDSSendMessage(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		//����SDS����
		SDS_parameter paramSDS;

		std::string strResId;

		// �������Ͷ���Ϣxml����ֵ�ṹ��
		int iRet = XMLProcess::GetXml_SDSSend_Req(data.Value, strResId, paramSDS);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_SDSSend_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().SDSSend(eLTE_Tool::String2Int(strResId), paramSDS);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("SDSSend failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ����/ȥ����GIS�ն�
void Logic_Queue::GISSubscribe(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int subType;
		std::string strResourceList("");
		std::string strSubscriber("");
		int iRet = XMLProcess::GetXml_GISSubscribe_Req(data.Value, subType, strResourceList, strSubscriber);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_GISSubscribe_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		GIS_parameter param;
		param.subtype = (SDSMSG_TYPE_t)subType;
		param.msg_body = strResourceList;
		param.subscriber = strSubscriber;
		iRet = OperationMgr::Instance().GISSubscribe(0, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GISSubscribe failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

// ��ʼ��Ƶ��ǽ
void Logic_Queue::StartVWall(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);

	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		VWALL_PARAM param;		
		int iRet = XMLProcess::GetXml_StartVWall_Req(data.Value, resId, param);

		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_StartVWall_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		VWallStart_parameter vParam;

		vParam.DstObjId = param.DstObjId;//lint !e1773
		vParam.SrcObjId = param.SrcObjId;//lint !e1773
		vParam.StrFmt = (char*)param.StrFmt.c_str();//lint !e1773


		iRet = OperationMgr::Instance().VWallStart(resId, vParam);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("VWallStart failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

void Logic_Queue::GetDcVWallIDList(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);
	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// ҵ����
		VWallInfoList* m_WallInfoList = NULL;
		int iRet = ProvisionMgr::Instance().GetDcVWallIDList(m_WallInfoList);
		if (eLTE_SVC_ERR_SUCCESS != iRet || NULL == m_WallInfoList)
		{
			LOG_RUN_ERROR("GetDcVWallIDList failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}
		//������ƵǽID״̬
		iRet = OperationMgr::Instance().UpdateDcVWallIDState(m_WallInfoList);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("UpdateDcVWallIDState failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ����xml�ַ���
		std::string xmlStr;
		iRet = XMLProcess::SetXml_GetDcVWallIDList_Rsp(m_WallInfoList, xmlStr);

		//LOG_RUN_ERROR("xmlStr rusult : %s",xmlStr);

		if(m_WallInfoList)
		{
			DeleteVecotor(m_WallInfoList->begin(), m_WallInfoList->end());
			delete m_WallInfoList;
			m_WallInfoList = NULL;
		}

		if (eLTE_SVC_ERR_SUCCESS != iRet || xmlStr.empty())
		{
			LOG_RUN_ERROR("SetXml_GetDcVWallIDList_Rsp failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		CONSTRUCT_AND_SEND_MESSAGE();
	}//lint !e438
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}



// ��Ƶ��ǽ
void Logic_Queue::VWallStop(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);

	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		// xml����
		int resId;
		VWallStop_parameter param;		
		int iRet = XMLProcess::GetXml_VWallStop_Req(data.Value, resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_VWallStop_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}

		// ҵ����
		iRet = OperationMgr::Instance().VWallStop(resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("VWallStop failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}
// 
// 	// ����PSTN/PLMN�绰����
// void Logic_Queue::TelephoneDial(const QUEUE_DATA& data) const
// {
// 	SSL_CHECK(data);
// 
// 	if (SSL_ST_OK == SSL_state(_ssl))
// 	{
// 		// ������Ϣͷ
// 		PACKET_HEAD head(data);
// 		head.MsgType |= RSP_MSG_FLAG;
// 		head.PacketLength = PACKET_HEAD_SIZE;
// 
// 		// xml����
// 		std::string PhoneNumStr;
// 		VWallStop_parameter param;
// 		int iRet = XMLProcess::GetXml_TelephoneDial_Req(data.Value, PhoneNumStr);
// 		if (eLTE_SVC_ERR_SUCCESS != iRet)
// 		{
// 			LOG_RUN_ERROR("GetXml_TelephoneDial_Req failed.");
// 			head.RspCode = iRet;
// 			// ������Ϣͷ
// 			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
// 			return;
// 		}
// 
// 		// ҵ����
// 		iRet = OperationMgr::Instance().TelephoneDial(PhoneNumStr);
// 		if (eLTE_SVC_ERR_SUCCESS != iRet)
// 		{
// 			LOG_RUN_ERROR("TelephoneDial failed.");
// 		}
// 		head.RspCode = iRet;
// 		// ������Ϣͷ
// 		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
// 	}
// 	else
// 	{
// 		// ssl״̬����ȷ���п��������жϣ�����������
// 		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
// 	}
// }
// 
// // �Ҷ�PSTN/PLMN�绰����
// void Logic_Queue::TelephoneHangup(const QUEUE_DATA& data) const
// {
// 	SSL_CHECK(data);
// 
// 	if (SSL_ST_OK == SSL_state(_ssl))
// 	{
// 		// ������Ϣͷ
// 		PACKET_HEAD head(data);
// 		head.MsgType |= RSP_MSG_FLAG;
// 		head.PacketLength = PACKET_HEAD_SIZE;
// 
// 		// xml����
// 		std::string PhoneNumStr;
// 		VWallStop_parameter param;
// 		int iRet = XMLProcess::GetXml_TelephoneHangup_Req(data.Value, PhoneNumStr);
// 		if (eLTE_SVC_ERR_SUCCESS != iRet)
// 		{
// 			LOG_RUN_ERROR("GetXml_TelephoneHangup_Req failed.");
// 			head.RspCode = iRet;
// 			// ������Ϣͷ
// 			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
// 			return;
// 		}
// 
// 		// ҵ����
// 		iRet = OperationMgr::Instance().TelephoneHangup(PhoneNumStr);
// 		if (eLTE_SVC_ERR_SUCCESS != iRet)
// 		{
// 			LOG_RUN_ERROR("TelephoneDial failed.");
// 		}
// 		head.RspCode = iRet;
// 		// ������Ϣͷ
// 		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
// 	}
// 	else
// 	{
// 		// ssl״̬����ȷ���п��������жϣ�����������
// 		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
// 	}
// }

// �������ܼ���
void Logic_Queue::StartDiscreetListen(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);

	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		CONSTRUCT_AND_WRITE_RESOURCEID_REQ();

		// ҵ����
		iRet = OperationMgr::Instance().DiscreetListenOpera(eLTE_Tool::String2Int(ResourceID), DL_START);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Start DiscreetListen failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

void Logic_Queue::StopDiscreetListen(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);

	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		CONSTRUCT_AND_WRITE_RESOURCEID_REQ();

		// ҵ����
		iRet = OperationMgr::Instance().DiscreetListenOpera(eLTE_Tool::String2Int(ResourceID), DL_STOP);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Start DiscreetListen failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

void Logic_Queue::StartEnvironmentListen(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);

	if (SSL_ST_OK == SSL_state(_ssl))
	{
		// ������Ϣͷ
		CONSTRUCT_AND_WRITE_RESOURCEID_REQ();

		// ҵ����
		iRet = OperationMgr::Instance().DiscreetListenOpera(eLTE_Tool::String2Int(ResourceID), P2P_ENVIRONMENT_LISTEN);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Start DiscreetListen failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}

void Logic_Queue::P2PTransfer(const QUEUE_DATA& data) const
{
	SSL_CHECK(data);

	if (SSL_ST_OK == SSL_state(_ssl))
	{
		//������Ϣͷ
		PACKET_HEAD head(data);
		head.MsgType |= RSP_MSG_FLAG;
		head.PacketLength = PACKET_HEAD_SIZE;

		int resId;
		transfer_parameter param;
		int iRet = XMLProcess::GetXml_P2PTransfer_Req(data.Value, resId, param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("GetXml_P2PTransfer_Req failed.");
			head.RspCode = iRet;
			// ������Ϣͷ
			SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
			return;
		}
		//����OperationManger��ʵ�������ҵ������ӿ�
		iRet = OperationMgr::Instance().P2PTransfer(resId,param);
		if (eLTE_SVC_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("P2PTransfer failed.");
		}
		head.RspCode = iRet;
		// ������Ϣͷ
		SSL_WRITE(_ssl, &head, PACKET_HEAD_SIZE);
	}
	else
	{
		// ssl״̬����ȷ���п��������жϣ�����������
		LOG_RUN_INFO("SSL state is not ok, don't send the data. msgType = %x", data.MsgType);
	}
}
