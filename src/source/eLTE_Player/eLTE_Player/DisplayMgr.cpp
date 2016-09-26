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

#include "StdAfx.h"
#include "DisplayMgr.h"
#include "DataType.h"
#include "AppContextMgr.h"
#include "OperationMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"
#include "mq/resourcestatus_indicator.h"//lint !e451
#include "mq/p2pvideocallstatus_indicator.h"
#include "mq/sds_message_indicator.h"
#include "mq/provchange_indicator.h"
#include "mq/grpcallstatus_indicator.h"
#include "mq/p2pcallstatus_indicator.h"
#include "mq/modulestatus_indicator.h"


DisplayMgr::DisplayMgr(void)
	: m_pCeLTE_PlayerCtrl(NULL)
{
}


DisplayMgr::~DisplayMgr(void)
{
	m_pCeLTE_PlayerCtrl = NULL;
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

	int iGroupID = pGrpInfo->getGrpId();
	int iGroupCallStatus = pGrpInfo->getSpStatus();
	int iSpeaker = pGrpInfo->getSpeaker();
	INFO_PARAM3(iGroupID, iGroupCallStatus, iSpeaker);

	// �����¼���Ϣ
	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		NotifyGroupStatusInfo* pNotifyGroupStatusInfo = new NotifyGroupStatusInfo;
		if (NULL == pNotifyGroupStatusInfo)//lint !e774
		{
			LOG_RUN_ERROR("pNotifyGroupStatusInfo is null.");
			return;
		}

		pNotifyGroupStatusInfo->iGroupID = iGroupID;
		pNotifyGroupStatusInfo->iGroupCallStatus = iGroupCallStatus;
		pNotifyGroupStatusInfo->iSpeaker = iSpeaker;

		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_GROUP_STATUS, (LPARAM)pNotifyGroupStatusInfo);
	}
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

	int iResId = pResourceStatus->getResId();
	int iStatusType = pResourceStatus->getStatusType();
	int iStatusValue = pResourceStatus->getStatusValue();
	int iAttachingGrp = pResourceStatus->getAttachingGrp();
	INFO_PARAM4(iResId, iStatusType, iStatusValue, iAttachingGrp);

	// �����¼���Ϣ
	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		NotifyResourceStatusInfo* pNotifyResourceStatusInfo = new NotifyResourceStatusInfo;
		if (NULL == pNotifyResourceStatusInfo)//lint !e774
		{
			LOG_RUN_ERROR("pNotifyResourceStatusInfo is null.");
			return;
		}

		// ��ȫ���� begin
		if ((RESREGSTATUS_PROXY == iStatusType || RESREGSTATUS_DIRECT == iStatusType) && STATUS_PASSWORD_WRONG == iStatusValue)
		{
			// "�˺Ŵ���"��"�������" => "�˺Ż����������"
			iStatusValue = STATUS_NOT_FOUND;
		}
		// ��ȫ���� end

		pNotifyResourceStatusInfo->iResourceID = iResId;
		pNotifyResourceStatusInfo->iStatusType = iStatusType;
		pNotifyResourceStatusInfo->iStatusValue = iStatusValue;
		pNotifyResourceStatusInfo->iAttachingGroup = iAttachingGrp;

		// ��̬�����cause�ֶ�
		if (USERDGNASTATUS == iStatusType)
		{
			DGNAStatusIndicator *pDGNAStatusIndicator = dynamic_cast<DGNAStatusIndicator*>(pResourceStatus);
			if (NULL == pDGNAStatusIndicator)
			{
				LOG_RUN_ERROR("pDGNAStatusIndicator is null.");
				delete pNotifyResourceStatusInfo;
				return;
			}
			int iCause = pDGNAStatusIndicator->getCause();
			INFO_PARAM1(iCause);
			pNotifyResourceStatusInfo->iCause = iCause;
		}

		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_RESOURCE_STATUS, (LPARAM)pNotifyResourceStatusInfo);
	}

	// ��½�ɹ�������SDK����ҵ��
	if ((RESREGSTATUS_PROXY == iStatusType || RESREGSTATUS_DIRECT == iStatusType)
		&& STATUS_REGOK == iStatusValue)
	{
		// ����SDK����ҵ��
		(void)OperationMgr::Instance().DcStartRunning();
	}
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

	int iCallStatus = pCallStatus->getStatus();
	int iCaller = pCallStatus->getCaller();
	int iCallee = pCallStatus->getCallee();
	INFO_PARAM3(iCallStatus, iCaller, iCallee);

	// �����¼���Ϣ
	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		NotifyP2pcallStatusInfo* pNotifyP2pcallStatusInfo = new NotifyP2pcallStatusInfo;
		if (NULL == pNotifyP2pcallStatusInfo)//lint !e774
		{
			LOG_RUN_ERROR("pNotifyP2pcallStatusInfo is null.");
			return;
		}
		pNotifyP2pcallStatusInfo->iCallStatus = iCallStatus;
		pNotifyP2pcallStatusInfo->iCaller = iCaller;
		pNotifyP2pcallStatusInfo->iCallee = iCallee;

		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_P2P_CALL_STATUS, (LPARAM)pNotifyP2pcallStatusInfo);
	}
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

	int iCallStatus = pCallStatus->getStatus();
	int iLocalAudioPort = pCallStatus->getlocalaudioport();
	int iLocalVideoPort = pCallStatus->getlocalvideoport();
	int iRemoteAudioPort = pCallStatus->getremoteaudioport();
	int iRemoteVideoPort = pCallStatus->getremotevideoport();
	unsigned long iRemoteIp = pCallStatus->getRemoteIp();
	int iCallee = pCallStatus->getCallee();
	int iCaller = pCallStatus->getCaller();
	int iSoundMute = pCallStatus->getSoundMute();
	int iChannel = pCallStatus->getChannel();
	int iUserConfirm = pCallStatus->getUserCfm();
	int iCamera = pCallStatus->getCamera();
	int iSoundPtype = pCallStatus->getSoundPtype();
	int iVideoFormatType = pCallStatus->getFmt();

	INFO_PARAM3(iCallStatus, iCaller, iCallee);
	INFO_PARAM6(iRemoteIp, iChannel, iLocalAudioPort, iLocalVideoPort, iRemoteAudioPort, iRemoteVideoPort);
	INFO_PARAM5(iSoundMute, iUserConfirm, iCamera, iVideoFormatType, iSoundPtype);

	// �����¼���Ϣ
	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		NotifyP2pvideocallStatusInfo* pNotifyP2pvideocallStatusInfo = new NotifyP2pvideocallStatusInfo;
		if (NULL == pNotifyP2pvideocallStatusInfo)//lint !e774
		{
			LOG_RUN_ERROR("pNotifyP2pvideocallStatusInfo is null.");
			return;
		}
		pNotifyP2pvideocallStatusInfo->iCallStatus = iCallStatus;
		pNotifyP2pvideocallStatusInfo->iLocalAudioPort = iLocalAudioPort;
		pNotifyP2pvideocallStatusInfo->iLocalVideoPort = iLocalVideoPort;
		pNotifyP2pvideocallStatusInfo->iRemoteAudioPort = iRemoteAudioPort;
		pNotifyP2pvideocallStatusInfo->iRemoteVideoPort = iRemoteVideoPort;
		pNotifyP2pvideocallStatusInfo->strRemoteIp = eLTE_Tool::GetIPByUIntValue(iRemoteIp);
		pNotifyP2pvideocallStatusInfo->iCallee = iCallee;
		pNotifyP2pvideocallStatusInfo->iCaller = iCaller;
		pNotifyP2pvideocallStatusInfo->iSoundMute = iSoundMute;
		pNotifyP2pvideocallStatusInfo->iChannel = iChannel;
		pNotifyP2pvideocallStatusInfo->iUserConfirm = iUserConfirm;
		pNotifyP2pvideocallStatusInfo->iCamera = iCamera;
		pNotifyP2pvideocallStatusInfo->iSoundPtype = iSoundPtype;
		pNotifyP2pvideocallStatusInfo->iVideoFormatType = iVideoFormatType;

		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_P2P_VIDEO_CALL_STATUS, (LPARAM)pNotifyP2pvideocallStatusInfo);
	}
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

	int iStatusType = pResourceStatus->getStatusType();
	int iStatusValue = pResourceStatus->getStatusValue();
	int iResId = pResourceStatus->getResId();
	INFO_PARAM3(iResId, iStatusType, iStatusValue);
	int iPeerID = 0;
	int iDirection = 0;
	int iCallType = 0;

	if (USERSTATUS == iStatusType)
	{
		UserStatusIndicator* pUserStatus = dynamic_cast<UserStatusIndicator*>(pResourceStatus);
		if (NULL == pUserStatus)
		{
			LOG_RUN_ERROR("pUserStatus is null.");
			return;
		}
		else
		{
			iPeerID = pUserStatus->getPeerUser();
			iDirection = pUserStatus->getDirection();
			iCallType = pUserStatus->getCallType();
		}
	}
	
	// �����¼���Ϣ
	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		NotifyUserStatusInfo* pNotifyUserStatusInfo = new NotifyUserStatusInfo;
		if (NULL == pNotifyUserStatusInfo)//lint !e774
		{
			LOG_RUN_ERROR("pNotifyUserStatusInfo is null.");
			return;
		}
		pNotifyUserStatusInfo->iUserID = iResId;
		pNotifyUserStatusInfo->iStatusType = iStatusType;
		pNotifyUserStatusInfo->iStatusValue = iStatusValue;
		pNotifyUserStatusInfo->iPeerID = iPeerID;
		pNotifyUserStatusInfo->iDirection = iDirection;
		pNotifyUserStatusInfo->iCallType = iCallType;

		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_USER_STATUS, (LPARAM)pNotifyUserStatusInfo);
	}
}

//֪ͨ�û�״̬����Ϣ�ķ������ʵ��
void DisplayMgr::notifyUserSdsStatus(SdsMessageIndicator* )
{
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
	int iModuleStatus = pModuleStatus->getModuleStatus();
	/*if (KICK_OFF != iModuleStatus && PASSWORD_CHANGE != iModuleStatus && USER_DELETE != iModuleStatus)
	{
	return;
	}*/

	int iResourceID = pModuleStatus->getResid();
	int iModuleType = pModuleStatus->getModuleType();
	int iCallBackMsgType = pModuleStatus->getCallBackBizType();
	std::string strModulePara = pModuleStatus->getModulePara();
	INFO_PARAM5(iResourceID, iModuleType, iModuleStatus, iCallBackMsgType, strModulePara);

	// �����¼���Ϣ
	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		NotifyModuleStatusInfo* pNotifyModuleStatusInfo = new NotifyModuleStatusInfo;
		if (NULL == pNotifyModuleStatusInfo)//lint !e774
		{
			LOG_RUN_ERROR("pNotifyUserStatusInfo is null.");
			return;
		}
		pNotifyModuleStatusInfo->iResourceID = iResourceID;
		pNotifyModuleStatusInfo->iModuleType = iModuleType;
		pNotifyModuleStatusInfo->iModuleStatus = iModuleStatus;
		pNotifyModuleStatusInfo->iCallBackMsgType = iCallBackMsgType;
		pNotifyModuleStatusInfo->strModulePara = strModulePara;

		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_MODULE_STATUS, (LPARAM)pNotifyModuleStatusInfo);
	}
}

//��������Ϣ�ϱ���GUI
/************************************************************************/
/* 
SdsMessageIndicatorָ�룬һ���������Ϣ���£�

Type����Ϣ���͡�
Content����Ϣ���ݡ�
From�������ˡ�
Mmsfilename�������ļ�����
Subject����Ϣ���⡣
Direction�� =false����ʾ���յ�����
Date�����ڡ�
Time��ʱ�䡣
StatusCode��״̬���ŵ�״̬�루��ͨ���Ų����ã���

����
������̨�յ����Ż����ʱ��ͨ���ص���������֪ͨ�ϲ�Ӧ�á���ʹ��getSdsDirection()==false �����ˡ��յ��Ķ��š�
*/
/************************************************************************/
void DisplayMgr::notifySMS(SdsMessageIndicator* pSdsMessage)
{
	//LOG_TRACE();
	if (NULL == pSdsMessage)
	{
		LOG_RUN_ERROR("pSdsMessage is null.");
		return;
	}

	// ɸѡ��Ϣ

	bool bSdsDirection = pSdsMessage->getSdsDirection();
	if (!bSdsDirection)
	{
		// �����¼���Ϣ
		if (NULL != m_pCeLTE_PlayerCtrl)
		{
			SdsMessageInfo* pNotifySdsMessageInfo = new SdsMessageInfo;
			if (NULL == pNotifySdsMessageInfo)//lint !e774
			{
				LOG_RUN_ERROR("pNotifySdsMessageInfo is null.");
				return;
			}

			pNotifySdsMessageInfo->strType = pSdsMessage->getSdsType();
			pNotifySdsMessageInfo->strContent = pSdsMessage->getSdsContent();
			if(!pNotifySdsMessageInfo->strContent.empty())
			{
				pNotifySdsMessageInfo->strContent = eLTE_Tool::UTF8ToANSI(pNotifySdsMessageInfo->strContent);
			}
			pNotifySdsMessageInfo->strFrom = pSdsMessage->getSdsFrom();
			pNotifySdsMessageInfo->listMmsFileName = pSdsMessage->getSdsFilename();
			pNotifySdsMessageInfo->strSubject = pSdsMessage->getSdsSubject();
			pNotifySdsMessageInfo->strSubject = eLTE_Tool::UTF8ToANSI(pNotifySdsMessageInfo->strSubject);
			pNotifySdsMessageInfo->strDate = pSdsMessage->getSdsDate();
			pNotifySdsMessageInfo->strTime = pSdsMessage->getSdsTime();
			//��ͨ���Ų�����
			if(EXMPP_MSG_TYPE_NORMAL != pNotifySdsMessageInfo->strType)
			{
				pNotifySdsMessageInfo->strStatusCode = eLTE_Tool::UInt2String(pSdsMessage->getStatusCode());
			}
			else
			{
				pNotifySdsMessageInfo->strStatusCode = "";
			}

			::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
				(WPARAM)EVENT_NOTIFY_SDS_REPORT, (LPARAM)pNotifySdsMessageInfo);
		}
	}
	else
	{
		// �����¼���Ϣ
		if (NULL != m_pCeLTE_PlayerCtrl)
		{
			SdsMessageInfo* pNotifypSdsStatus = new SdsMessageInfo;
			if (NULL == pNotifypSdsStatus)//lint !e774
			{
				LOG_RUN_ERROR("pNotifypSdsStatus is null.");
				return;
			}

			pNotifypSdsStatus->strSubject = pSdsMessage->getSdsSubject();
			pNotifypSdsStatus->strRetCode = pSdsMessage->getRetCode();

			::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
				(WPARAM)EVENT_NOTIFY_SDS_RETCODE, (LPARAM)pNotifypSdsStatus);
		}
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

	// �����¼���Ϣ
	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		NotifyProvisionChangeInfo* pNotifyProvisionChangeInfo = new NotifyProvisionChangeInfo;
		if (NULL == pNotifyProvisionChangeInfo)//lint !e774
		{
			LOG_RUN_ERROR("pNotifyProvisionChangeInfo is null.");
			return;
		}
		pNotifyProvisionChangeInfo->iChangeType = pProvChange->getChangeType();
		pNotifyProvisionChangeInfo->iChangeMO = pProvChange->getChangeMO();
		pNotifyProvisionChangeInfo->pInfo = NULL;

		switch (pNotifyProvisionChangeInfo->iChangeMO)
		{
		case MO_PTTUser:
			{
				ProvChangeIndicator_PTTUser* pPTTUser = dynamic_cast<ProvChangeIndicator_PTTUser*>(pProvChange);
				if (NULL == pPTTUser)
				{
					LOG_RUN_ERROR("pPTTUser is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}

				MO_PTTUser_Info* pInfo = new MO_PTTUser_Info; //PClint Info:No.774, new ��ָ��ز�Ϊ�� 
				pInfo->iUserID = pPTTUser->userid;
				pInfo->iUserCategory = pPTTUser->usercategory;
				pInfo->iUserPriority = pPTTUser->userpriority;
				pInfo->strUserName = pPTTUser->username;
				pInfo->iVpnID = pPTTUser->vpnid;
				pInfo->iVpnOut = pPTTUser->vpnout;
				pInfo->iVpnIn = pPTTUser->vpnin;
				pInfo->iLocation = pPTTUser->location;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_FixedUser:
			{
				ProvChangeIndicator_FixedUser* pFixedUser = dynamic_cast<ProvChangeIndicator_FixedUser*>(pProvChange);
				if (NULL == pFixedUser)
				{
					LOG_RUN_ERROR("pFixedUser is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}

				MO_FixedUser_Info* pInfo = new MO_FixedUser_Info;
				pInfo->iUserID = pFixedUser->userid;
				pInfo->iUserCategory = pFixedUser->usercategory;
				pInfo->iUserPriority = pFixedUser->userpriority;
				pInfo->strUserName = pFixedUser->username;
				pInfo->iVpnID = pFixedUser->vpnid;
				pInfo->iVpnOut = pFixedUser->vpnout;
				pInfo->iVpnIn = pFixedUser->vpnin;
				pInfo->iLocation = pFixedUser->location;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_DCAttribute:
			{
				ProvChangeIndicator_DCAttribute* pDCAttribute = dynamic_cast<ProvChangeIndicator_DCAttribute*>(pProvChange);
				if (NULL == pDCAttribute)
				{
					LOG_RUN_ERROR("pDCAttribute is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}
				
				MO_DCAttribute_Info* pInfo = new MO_DCAttribute_Info;
				pInfo->iDcID = pDCAttribute->dcid;
				pInfo->iPriviledge = pDCAttribute->priviledge;
				pInfo->iRole = pDCAttribute->role;
				pInfo->strAlias = pDCAttribute->Alias;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_Group:
			{
				ProvChangeIndicator_Group* pGroup = dynamic_cast<ProvChangeIndicator_Group*>(pProvChange);
				if (NULL == pGroup)
				{
					LOG_RUN_ERROR("pGroup is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}

				MO_Group_Info* pInfo = new MO_Group_Info;
				pInfo->iGroupID = pGroup->grpid;
				pInfo->iGroupCategory = pGroup->grpcategory;
				pInfo->iGroupPriority = pGroup->grppriority;
				pInfo->strGroupName = pGroup->grpname;
				pInfo->iGroupState = pGroup->grpstate;
				pInfo->iVpnID = pGroup->vpnid;
				pInfo->iSetupDcID = pGroup->setupDcId;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_GroupPTTUser:
		case MO_GroupFixedUser:
			{
				ProvChangeIndicator_GrpUserInfo* pGroupUserInfo = dynamic_cast<ProvChangeIndicator_GrpUserInfo*>(pProvChange);
				if (NULL == pGroupUserInfo)
				{
					LOG_RUN_ERROR("pGroupUserInfo is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}

				MO_GroupUser_Info* pInfo = new MO_GroupUser_Info;
				pInfo->iUserID = pGroupUserInfo->userid;
				pInfo->iGroupID = pGroupUserInfo->grpid;
				pInfo->iUserPriorityInGroup = pGroupUserInfo->userpriorityInGrp;
				pInfo->iMemberType = pGroupUserInfo->memberType;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_RangeList:
			{
				ProvChangeIndicator_RangeList* pRangeList = dynamic_cast<ProvChangeIndicator_RangeList*>(pProvChange);
				if (NULL == pRangeList)
				{
					LOG_RUN_ERROR("pRangeList is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}
				
				MO_RangeList_Info* pInfo = new MO_RangeList_Info;
				pInfo->iRangeID = pRangeList->rangid;
				pInfo->iRangeCategory = pRangeList->Rangecategory;
				pInfo->iFrom = pRangeList->From;
				pInfo->iTo = pRangeList->To;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_DCRanges:
			{
				ProvChangeIndicator_DCRange* pDCRange = dynamic_cast<ProvChangeIndicator_DCRange*>(pProvChange);
				if (NULL == pDCRange)
				{
					LOG_RUN_ERROR("pDCRange is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}
			
				MO_DCRanges_Info* pInfo = new MO_DCRanges_Info;
				pInfo->iRangeID = pDCRange->rangid;
				pInfo->iDcID = pDCRange->dcid;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_HOTLINECFG:
			{
				ProvChangeIndicator_HotlineCfg* pHotlineCfg = dynamic_cast<ProvChangeIndicator_HotlineCfg*>(pProvChange);
				if (NULL == pHotlineCfg)
				{
					LOG_RUN_ERROR("pHotlineCfg is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}

				MO_HotlineCfg_Info* pInfo = new MO_HotlineCfg_Info;
				pInfo->iDcID = pHotlineCfg->DcId;
				pInfo->iHotIndex = pHotlineCfg->HotIndex;
				pInfo->iNumber = pHotlineCfg->Number;
				pInfo->strAlias = pHotlineCfg->Alias;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_STMSGCFG:
			{
				ProvChangeIndicator_StMsgCfg* pStMsgCfg = dynamic_cast<ProvChangeIndicator_StMsgCfg*>(pProvChange);
				if (NULL == pStMsgCfg)
				{
					LOG_RUN_ERROR("pStMsgCfg is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}
				
				MO_StMsgCfg_Info* pInfo = new MO_StMsgCfg_Info;
				pInfo->iID = pStMsgCfg->id;
				pInfo->strStMsg = pStMsgCfg->stmsg;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_DECUSER:
			{
				// û���ҵ���Ӧ��
				//LOG_RUN_ERROR("not found MO_DECUSER.");
				delete pNotifyProvisionChangeInfo;
				return;
			}
			//break;//lint -e527
		case MO_GISCFG:
			{
				ProvChangeIndicator_GisCfg* pGisCfg = dynamic_cast<ProvChangeIndicator_GisCfg*>(pProvChange);
				if (NULL == pGisCfg)
				{
					LOG_RUN_ERROR("pGisCfg is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}

				MO_GisCfg_Info* pInfo = new MO_GisCfg_Info;
				pInfo->iUserID = pGisCfg->userid;
				pInfo->uiReportSwitch = pGisCfg->reportSwitch;
				pInfo->uiReportPeriod = pGisCfg->reportPeriod;
				pInfo->uiReportCapability = pGisCfg->reportCapability;
				pInfo->uiReportDistance = pGisCfg->reportDistance;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		case MO_GISIPC:
			{
				ProvChangeIndicator_GisIpc* pGisIpc = dynamic_cast<ProvChangeIndicator_GisIpc*>(pProvChange);
				if (NULL == pGisIpc)
				{
					LOG_RUN_ERROR("pGisIpc is null.");
					delete pNotifyProvisionChangeInfo;
					return;
				}
			
				MO_GisIpc_Info* pInfo = new MO_GisIpc_Info;
				pInfo->iUserID = pGisIpc->userid;
				pInfo->dLatitude = pGisIpc->latitude;
				pInfo->dLongitude = pGisIpc->longitude;
				pInfo->dAltitude = pGisIpc->altitude;
				pInfo->strAddress = pGisIpc->address;

				pNotifyProvisionChangeInfo->pInfo = pInfo;
			}
			break;
		default:
			{
				// ���������¼��ص�
				// û���ҵ���Ӧ��
				delete pNotifyProvisionChangeInfo;
				return;
			}
		}

		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_PROVISION_CHANGE, (LPARAM)pNotifyProvisionChangeInfo);
	}
}

//֪ͨ���ö����ȫͬ���� ����Ϊһ����ָ̬�룬 ʹ������Ҫdowncast Ϊ��Ӧ�ı������
void DisplayMgr::notifyProvisionAllResync(ProvChangeIndicator* )
{
	LOG_TRACE();

	if (NULL != m_pCeLTE_PlayerCtrl)
	{
		::PostMessage(m_pCeLTE_PlayerCtrl->GetSafeHwnd(), WM_ELTE_POST_EVENT,
			(WPARAM)EVENT_NOTIFY_PROVISION_ALLRESYNC, 0);
	}
}

//֪ͨ״̬�����ȫͬ���� ����Ϊһ����ָ̬�룬 ʹ������Ҫdowncast Ϊ��Ӧ�ı������
void DisplayMgr::notifyStatusAllResync(ResourceStatusIndicator* )
{

}

// ����eLTE_PlayerCtrl���
void DisplayMgr::SetCeLTE_PlayerCtrl(COleControl* pCeLTE_PlayerCtrl)
{
	m_pCeLTE_PlayerCtrl = pCeLTE_PlayerCtrl;
}
