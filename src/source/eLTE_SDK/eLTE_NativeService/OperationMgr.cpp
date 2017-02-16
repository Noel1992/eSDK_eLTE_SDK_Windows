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
#include "OperationMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"
#include "eLTE_Tool.h"
#include "eLTE_Message.h"

#define GET_SDK_POINTER												\
	OperationManager* pOM = OperationManager_T::instance();			\
	if (NULL == pOM)												\
	{																\
		LOG_RUN_ERROR("OperationManager_T get instance failed.");	\
		return eLTE_SVC_ERR_NULL_POINTER;							\
	}																\


OperationMgr::OperationMgr(void)
	: m_userId("")
	, m_passwd("")
	, m_localIP("")
	, m_serverIP("")
	, m_sipPort("")
	, m_bLoginSuccess(FALSE)
{

}


OperationMgr::~OperationMgr(void)
{
	try
	{
		m_userId = "";
		m_passwd = "";
		m_localIP = "";
		m_serverIP = "";
		m_sipPort = "";
		m_bLoginSuccess = FALSE;
	}
	catch (...)
	{

	}
}


OperationMgr& OperationMgr::Instance()
{
	static OperationMgr s_OperationMgr;
	return s_OperationMgr;
}

// ����̨ע��
int OperationMgr::Login(const std::string& userId, const std::string& passwd, 
	const std::string& serverIP, const std::string& localIP, const std::string& sipPort)
{
	LOG_TRACE();

	// ״̬У��
	if (m_bLoginSuccess)
	{
		LOG_RUN_WARN("Already login.");
		return eLTE_SVC_ERR_SUCCESS;
	}

	// ��ȡSDKʵ��ָ��
	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// ���õ���̨״̬
	DcState* pDC = pAC->getConsoleState();
	if (NULL == pDC)
	{
		LOG_RUN_ERROR("getConsoleState failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	int iDcId = eLTE_Tool::String2Int(userId);
	pDC->setDCID(iDcId);
	pDC->setUser(userId.c_str());
	pDC->setPwd(passwd.c_str());
	pDC->setServerIP(serverIP.c_str());
	pDC->setLocalIP(localIP.c_str());
	pDC->setSipPort(sipPort.c_str());

	// ���õ�¼��Դ
	ResourceInfo* pResInfo = new ResourceInfo(iDcId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}
	pResInfo->setUserName(userId.c_str());
	pResInfo->setPassWord(passwd.c_str());
	pResInfo->setServerIp(serverIP.c_str());
	pResInfo->setLocalIp(localIP.c_str());
	pResInfo->setSipPort(sipPort.c_str());

	// ����̨ע��
	Op_Return_t iRet = pOM->invokeOp(pResInfo, DC_LOGIN);
	if (OP_OK_ACCEPTED != iRet)
	{
		// �ͷŵ�½��Դ
		delete pResInfo;
		LOG_RUN_ERROR("invokeOp DC_LOGIN failed. (%d)", iRet);
		return iRet;
	}

	// �ͷŵ�½��Դ
	delete pResInfo;
	pResInfo = NULL;
	// ���汾������
	m_userId = userId;
	m_passwd = passwd;
	m_localIP = localIP;
	m_serverIP = serverIP;
	m_sipPort = sipPort;

	return eLTE_SVC_ERR_SUCCESS;//lint !e438
}

// ����̨ע��
int OperationMgr::Logout(const std::string& userId)
{
	LOG_TRACE();
	INFO_PARAM1(userId);

	// ״̬У��
	if (m_userId.empty())
	{
		return eLTE_SVC_ERR_SUCCESS;
	}

	// ���У��
	if (m_userId != userId)
	{
		INFO_PARAM2(userId, m_userId);
		LOG_RUN_ERROR("Please login first.");
		return eLTE_SVC_ERR_NOT_LOGIN;
	}

	// ״̬У��
	if (!m_bLoginSuccess)
	{
		LOG_RUN_ERROR("Please wait login result.");
		return eLTE_SVC_ERR_WAIT_LOGIN_RESULT;
	}

	// ��½�ɹ���ʾ
	m_bLoginSuccess = FALSE;

	// ��ȡSDKʵ��ָ��
	ApplicationContext * pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	OperationManager * pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// ֹͣSDK����ҵ��
	int sRet = pAC->stopRunning();
	if (0 != sRet)
	{
		LOG_RUN_ERROR("stopRunning failed. (%d)", sRet);
		return sRet;
	}

	// ���õǳ���Դ
	ResourceInfo* pResInfo = new ResourceInfo(eLTE_Tool::String2Int(userId));
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}
	pResInfo->setUserName(userId.c_str());
	pResInfo->setPassWord(m_passwd.c_str());
	pResInfo->setServerIp(m_serverIP.c_str());
	pResInfo->setLocalIp(m_localIP.c_str());
	pResInfo->setSipPort(m_sipPort.c_str());

	// ����̨ע��
	Op_Return_t iRet = pOM->invokeOp(pResInfo, DC_LOGOUT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp DC_LOGOUT failed. (%d)", iRet);
	}

	// �ͷŵǳ���Դ
	delete pResInfo;
	pResInfo = NULL;
	// �����ر���
	m_userId.clear();
	m_passwd.clear();
	m_localIP.clear();
	m_serverIP.clear();
	m_sipPort.clear();

	return iRet;//lint !e438
}

// ����SDK����ҵ��
int OperationMgr::StartRunning()
{
	LOG_TRACE();

	// У���Ƿ��½
	if (m_userId.empty())
	{
		// û�е�½���ǳ��ɹ�����ҪstartRunning
		return eLTE_SVC_ERR_SUCCESS;
	}

	if (m_bLoginSuccess)
	{
		return eLTE_SVC_ERR_SUCCESS;
	}

	// ��½�ɹ���ʾ
	m_bLoginSuccess = TRUE;

	// ��ȡSDKʵ��ָ��
	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// ����SDK����ҵ��
	int iRet = pAC->startRunning(m_serverIP.c_str());
	if (0 != iRet)
	{
		LOG_RUN_ERROR("startRunning failed. (%d)", iRet);
		return iRet;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

// ����״̬�ϱ�
int OperationMgr::TriggerStatusReport(unsigned int uiEnableStatusReport) const
{
	LOG_TRACE();
	INFO_PARAM1(uiEnableStatusReport);

	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// �����ϱ�״̬
	bool bEnable = ((FALSE == uiEnableStatusReport) ? false : true);
	pAC->triggerStatusReport(bEnable);

	return eLTE_SVC_ERR_SUCCESS;
}

// ����̨������Ƶ�ش�
int OperationMgr::VideoMonitor(int iResId, Video_UL_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM5(iResId, param.fmtvalue, param.cameratype, param.mutetype, param.user_confirm_type);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ������Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, P2P_VIDEO_MONITOR);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara P2P_VIDEO_MONITOR failed. (%d)", iRet);
	}
	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ������Ƶ�ش�
int OperationMgr::RecvVideoPlay(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ������Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_VIDEO_RECV);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_VIDEO_RECV failed. (%d)", iRet);
	}
	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����ֹ̨ͣ��Ƶ�ַ�
int OperationMgr::StopVideoDispatch(int iResId, Video_Dispatch_Delete& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, param.resId, param.userId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ��ʼ��Ƶ�ַ�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, P2P_VIDEO_DISPATCH_DELETE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara P2P_VIDEO_DISPATCH_DELETE failed. (%d)", iRet);
	}
	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����̨��ʼ��Ƶ�ַ�
int OperationMgr::VideoDispatch(int iResId, Video_Dispatch_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, param.dispatch_num, param.fmtvalue);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ��ʼ��Ƶ�ַ�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, P2P_VIDEO_DISPATCH);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara P2P_VIDEO_DISPATCH failed. (%d)", iRet);
	}
	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����̨�Ҷ���Ƶ�ش�
int OperationMgr::VideoHangup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �Ҷ���Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_VIDEO_HANGUP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_VIDEO_HANGUP failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ��̨����
int OperationMgr::PTZControl(int iResId, FVG_control& control) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, control.ptz_control_code, control.ptz_control_value);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ������Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&control, DC_PZT_CONCTROL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara DC_PZT_CONCTROL failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����Ⱥ��
int OperationMgr::SubscribeGroup(int groupId, unsigned int uiEableSubscribeGroup) const
{
	LOG_TRACE();
	INFO_PARAM2(groupId, uiEableSubscribeGroup);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ����Ⱥ����Դ
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("pResInfo is null.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ȥ����Ⱥ��
	Op_Return_t iRet;
	if (0 == uiEableSubscribeGroup)
	{
		iRet = pOM->invokeOp(pResInfo, GRP_UNSUB);
		if (OP_OK_ACCEPTED != iRet)
		{
			LOG_RUN_ERROR("invokeOp GRP_UNSUB failed. (%d)", iRet);
		}
	}
	// ����Ⱥ��
	else
	{
		iRet = pOM->invokeOp(pResInfo, GRP_SUB);
		if (OP_OK_ACCEPTED != iRet)
		{
			LOG_RUN_ERROR("invokeOp GRP_SUB failed. (%d)", iRet);
		}
	}

	// �ͷ�Ⱥ����Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ������̬����
int OperationMgr::CreateDynamicGroup(DGNA_parameter& param) const
{
	LOG_TRACE();

	// ����У��
	static const int MAX_ALIAS_LEN = 32;
	static const int MIN_PRIORITY_VAR = 1;
	static const int MAX_PRIORITY_VAR = 15;
	static const int MIN_PERIOD_VAR = 1;
	static const int MAX_PERIOD_VAR = 65535;
	if (param.alias.empty() || param.alias.size() > MAX_ALIAS_LEN)
	{
		LOG_RUN_ERROR("alias is empty or length is greater than 32.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}
	if (param.priority < MIN_PRIORITY_VAR || param.priority > MAX_PRIORITY_VAR)
	{
		LOG_RUN_ERROR("priority is greater than 15 or less than 1.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}
	if (param.maxPeriod < MIN_PERIOD_VAR || param.maxPeriod > MAX_PERIOD_VAR)
	{
		LOG_RUN_ERROR("maxPeriod is greater than 65535 or less than 1.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	int iDcID = eLTE_Tool::String2Int(m_userId);
	ResourceInfo* pResInfo = new ResourceInfo(iDcID);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ������̬����
	int iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, DGNA_CREATE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara DGNA_CREATE failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ɾ����̬����
int OperationMgr::CancelDynamicGroup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ɾ����̬����
	int iRet = pOM->invokeOp_multipara(pResInfo, NULL, DGNA_CANCEL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara DGNA_CANCEL failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// �����ɽ��顢�����ɽ����Ա��ɾ���ɽ����Ա
int OperationMgr::OperatePatchGroup(PCHGRP_Para& param, const unsigned short& uOperateType) const
{
	LOG_TRACE();
	INFO_PARAM3(param.DcId, param.PatchGroupId, param.PatchGroupName);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(param.DcId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}
	int iRet = 0;
	// ������̬����
	switch(uOperateType)
	{
		case ELTE_SERVICE_CREATEPATCHGROUP_REQ:
			{
				iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, PCHGRP_CREATE);
				if (OP_OK_ACCEPTED != iRet)
				{
					LOG_RUN_ERROR("invokeOp_multipara PCHGRP_CREATE failed. (%d)", iRet);
				}
			}
			break;
		case ELTE_SERVICE_ADDPATCHMEMBER_REQ:
			{
				iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, PCHGRP_ADD);
				if (OP_OK_ACCEPTED != iRet)
				{
					LOG_RUN_ERROR("invokeOp_multipara PCHGRP_ADD failed. (%d)", iRet);
				}
			}
			break;
		case ELTE_SERVICE_DELETEPATCHMEMBER_REQ:
			{
				iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, PCHGRP_DEL);
				if (OP_OK_ACCEPTED != iRet)
				{
					LOG_RUN_ERROR("invokeOp_multipara PCHGRP_DEL failed. (%d)", iRet);
				}
			}
			break;
		default:
			{
				iRet = eLTE_SVC_ERR_FAILED;
				LOG_RUN_ERROR("Invalid MsgType: %x", uOperateType);
			}
			break;
	}
	
	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ɾ���ɽ���
int OperationMgr::CancelPatchGroup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ɾ����̬����
	int iRet = pOM->invokeOp(pResInfo, PCHGRP_CANCEL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PCHGRP_CANCEL failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ������ʱ��
int OperationMgr::CreateTempGroup(DGNA_parameter& param) const
{
	LOG_TRACE();

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	int iDcId = eLTE_Tool::String2Int(m_userId);
	ResourceInfo* pResInfo = new ResourceInfo(iDcId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ������ʱ��
	int iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, TEMPGRP_CREATE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara TEMPGRP_CREATE failed.");
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����һ����ʱ���
int OperationMgr::GetNextTempGroupID() const
{
	LOG_TRACE();
	int iGroupID = 0;

	// ��ȡ��ʱ���
	DcState* pDcState = DcState::getInstance();
	if (NULL == pDcState)
	{
		LOG_RUN_ERROR("DcState get instance failed.");
		return iGroupID;
	}
	iGroupID = pDcState->getNextTempGroupID();

	return iGroupID;
}

// �����˫�����
int OperationMgr::P2PHalfDpxDial(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �����������
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_HALFDPX_DIAL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_HALFDPX_DIAL failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// �ͷŰ�˫�����
int OperationMgr::P2PHalfDpxRelease(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �����������
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_HALFDPX_RELEASE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_HALFDPX_RELEASE failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// �����������
int OperationMgr::P2PDial(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �����������
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_DIALOUT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_DIALOUT failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// �����������
int OperationMgr::P2PRecv(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �����������
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_RECV);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_RECV failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// �ܽ��������
int OperationMgr::P2PReject(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �ܽ��������
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_REJECT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_REJECT failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// �Ҷ��������
int OperationMgr::P2PHangup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �Ҷ��������
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_HANGUP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_HANGUP failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����̨ǿ����
int OperationMgr::P2PBreakoff(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ����̨ǿ����
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_BREAKOFF);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_BREAKOFF failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ������ǿ����+�����µ����
int OperationMgr::P2PBreakin(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ������ǿ����+�����µ����
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_BREAKIN);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_BREAKIN failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ���Ĳ��Զ�����Ⱥ��
int OperationMgr::SubJoinGroup(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ���Ĳ��Զ�����Ⱥ��
	Op_Return_t iRet = pOM->invokeOp(pResInfo, BATCH_CFG);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp BATCH_CFG failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����������̶�����ʱ���������Ȩ
int OperationMgr::PTTDial(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ����������̶�����ʱ���������Ȩ
	Op_Return_t iRet = pOM->invokeOp(pResInfo, PTT_DIAL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_DIAL failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// �ͷŻ�Ȩ
int OperationMgr::PTTRelease(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �ͷŻ�Ȩ
	Op_Return_t iRet = pOM->invokeOp(pResInfo, PTT_RELEASE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_RELEASE failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// �˳����
int OperationMgr::PTTHangup(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �˳����
	Op_Return_t iRet = pOM->invokeOp(pResInfo, PTT_HANGUP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_HANGUP failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����������
int OperationMgr::PTTEmergency(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ����������
	Op_Return_t iRet = pOM->invokeOp(pResInfo, PTT_EMERGENCY);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_EMERGENCY failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����̨ǿ�����
int OperationMgr::GroupBreakoff(int groupId) const
{
	LOG_TRACE();
	INFO_PARAM1(groupId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(groupId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ����̨ǿ�����
	Op_Return_t iRet = pOM->invokeOp(pResInfo, GRP_BREAKOFF);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp GRP_BREAKOFF failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����ִ̨�о���
int OperationMgr::VolMute(int iResId, Mute_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ����ִ̨�о���
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VOL_MUTE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp VOL_MUTE failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

int OperationMgr::SetUserSpecificGISCfg(int resId, UEGisCfg_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM1(resId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(resId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ����̨�޸��ն˵�GIS����
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, GIS_CONFIG);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp GIS_CONFIG failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

int OperationMgr::ModifyDynamicGroup(DGNA_Modify_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM2(param.dcId, param.grpId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(param.grpId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// �޸Ķ�̬����
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, DGNA_EDIT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp DGNA_EDIT failed. (%d)", iRet);	
	}
	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����̨ȡ������
int OperationMgr::VolUnMute(int iResId, Mute_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ����̨ȡ������
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VOL_UNMUTE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp VOL_UNMUTE failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ����/ȥ����GIS�ն�
int OperationMgr::GISSubscribe(int iResId, GIS_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	param.msg_body = eLTE_Tool::ANSIToUTF8(param.msg_body);

	// ����/ȥ����GIS�ն�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, GIS_SUBSCRIBE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp GIS_SUBSCRIBE failed. (%d)", iRet);	
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

int OperationMgr::SDSSend(const int& iResId, SDS_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, param.sdstype, param.receiver);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ���������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	param.msg_body = eLTE_Tool::ANSIToUTF8(param.msg_body);

	// ���Ͷ���
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, SDS_SEND);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara SDS_SEND failed. (%d)", iRet);
	}

	// �ͷŶ�����Դ
	delete pResInfo;
	pResInfo=NULL;
	return iRet;//lint !e438
}

// ����̨������Ƶ�ش�
int OperationMgr::VWallStart(int iResId, VWallStart_parameter& param) const
{
	LOG_TRACE();

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;
	
	//��ȡ��ԴID
//	iResId = param.DstObjId;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}
	// ������Ƶ��ǽ
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VWALL_START);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara VWALL_START failed. (%d)", iRet);
	}
	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// ��Ƶ��ǽ
int OperationMgr::VWallStop(int iResId, VWallStop_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId,param.DstObjId, param.SrcObjId);

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ��ʼ��Ƶ��ǽ
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VWALL_STOP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara VWALL_STOP failed. (%d)", iRet);
	}
	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

//���µ���ǽID״̬
int OperationMgr::UpdateDcVWallIDState(VWallInfoList* const &m_WallInfoList) const
{
	LOG_TRACE();
	if (NULL == m_WallInfoList)
	{
		LOG_RUN_ERROR("m_WallInfoList is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}
	
	// ��ȡSDKʵ��ָ��
	DcState* pDcState = DcState::getInstance();
	if (NULL == pDcState)
	{
		LOG_RUN_ERROR("DcState get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	VWallInfoList::const_iterator it_b = m_WallInfoList->begin();
	VWallInfoList::const_iterator it_e = m_WallInfoList->end();

	LOG_RUN_INFO("list size:%d",m_WallInfoList->size());
	User_Call_Status* m_WallIdState = NULL;
	for (VWallInfoList::const_iterator itor = it_b; it_e != itor; itor++)
	{
		VWallInfo* const &pVWallIDList = *itor;
		if (NULL == pVWallIDList)
		{
			LOG_RUN_ERROR("pVWallIDList is null.");
			return eLTE_SVC_ERR_NULL_POINTER;
		}

		m_WallIdState = pDcState->findUserStatus(pVWallIDList->userid,VIDEOONWALL);		
		
		if (NULL != m_WallIdState)
		{
			LOG_RUN_INFO("UpdateDcVWallIDState ID:%d state:%d",pVWallIDList->userid, m_WallIdState->userStatus);
			pVWallIDList->Status = m_WallIdState->userStatus;
		}				
	}		
	
	m_WallIdState = NULL;	
	return eLTE_SVC_ERR_SUCCESS;//lint !e438
}
// 
// //����PSTN/PLMN�绰���� 
// int OperationMgr::TelephoneDial(const std::string& PhoneNum) const
// {
// 	LOG_TRACE();
// 
// 	// ��ȡSDKʵ��ָ��
// 	GET_SDK_POINTER;
// 
// 	//���ⲿPSTN/PLMN�绰�������,ResourceID�̶�Ϊ99911000
// 	int	iResId = 99911000;
// 
// 	// ������Դ
// 	TIResourceInfo* pTIResInfo = new TIResourceInfo(iResId, PhoneNum);
// 	if (NULL == pTIResInfo)//lint !e774
// 	{
// 		LOG_RUN_ERROR("new TIResourceInfo failed.");
// 		return eLTE_SVC_ERR_MEMORY_APPLY;
// 	}
// 	
// 	// ����PSTN/PLMN�绰���� 
// 	Op_Return_t iRet = pOM->invokeOp(pTIResInfo, TIC_DIALOUT);
// 	if (OP_OK_ACCEPTED != iRet)
// 	{
// 		LOG_RUN_ERROR("invokeOp TelephoneDial failed. (%d)", iRet);
// 	}
// 	// �ͷ���Դ
// 	delete pTIResInfo;
// 	pTIResInfo = NULL;
// 	return iRet;//lint !e438
// }
// 
// //�Ҷ�PSTN/PLMN�绰���� 
// int OperationMgr::TelephoneHangup(const std::string& PhoneNum) const
// {
// 	LOG_TRACE();
// 
// 	// ��ȡSDKʵ��ָ��
// 	GET_SDK_POINTER;
// 
// 	//���ⲿPSTN/PLMN�绰�������,ResourceID�̶�Ϊ99911000
// 	int	iResId = 99911000;
// 
// 	// ������Դ
// 	TIResourceInfo* pTIResInfo = new TIResourceInfo(iResId, PhoneNum);
// 	if (NULL == pTIResInfo)//lint !e774
// 	{
// 		LOG_RUN_ERROR("new TIResourceInfo failed.");
// 		return eLTE_SVC_ERR_MEMORY_APPLY;
// 	}
// 	
// 	// �Ҷ�PSTN/PLMN�绰����
// 	Op_Return_t iRet = pOM->invokeOp(pTIResInfo, TIC_HANGUP);
// 	if (OP_OK_ACCEPTED != iRet)
// 	{
// 		LOG_RUN_ERROR("invokeOp TelephoneHangup failed. (%d)", iRet);
// 	}
// 	// �ͷ���Դ
// 	delete pTIResInfo;
// 	pTIResInfo = NULL;
// 	return iRet;//lint !e438
// }

//���ܼ���
int OperationMgr::DiscreetListenOpera(int ResourceID, Operation_t OperationType) const
{
	LOG_TRACE();

	// ��ȡSDKʵ��ָ��
	GET_SDK_POINTER;
	
	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(ResourceID);
	if (NULL == pResInfo)
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	// ��ʼ���ܼ���
	Op_Return_t iRet = pOM->invokeOp(pResInfo, OperationType);
	LOG_RUN_INFO("DiscreetListenOpera: %d-%d",pResInfo->resId,OperationType);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp StartDiscreetListen failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}

// 
// int OperationMgr::TempUserJoinGroup(const int& resid, const PhonePatch_parameter& param) const
// {
// 	LOG_TRACE();
// 	INFO_PARAM3(resid, param.DCID, param.UserID);
// 
// 	//��ȡҵ��������ȫ��Ψһʵ��
// 	OperationManager* pOM = OperationManager_T::instance();
// 	if (NULL == pOM)
// 	{
// 		LOG_RUN_ERROR("OperationManager_T get instance failed.");
// 		return eLTE_SVC_ERR_NULL_POINTER;
// 	}
// 
// 	// ������Դ
// 	ResourceInfo* pResInfo = new ResourceInfo(resid);
// 	if (NULL == pResInfo)//lint !e774
// 	{
// 		LOG_RUN_ERROR("new ResourceInfo failed.");
// 		return eLTE_SVC_ERR_MEMORY_APPLY;
// 	}
// 
// 	//�����˹�ת��ҵ��ӿ�
// 	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo,(void*)&param, TEMPUSER_JOIN_GRPCALL);
// 	if (OP_OK_ACCEPTED != iRet)
// 	{
// 		LOG_RUN_ERROR("invokeOp_multipara TEMPUSER_JOIN_GRPCALL failed. (%d)", iRet);
// 	}
// 
// 	//�ͷ���Դ
// 	delete pResInfo;
// 	pResInfo = NULL;
// 	return iRet;//lint !e438
// }

int OperationMgr::P2PTransfer(const int iResId, transfer_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM4(iResId, param.dcid, param.speakerid, param.objectid);

	//��ȡҵ��������ȫ��Ψһʵ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_SVC_ERR_MEMORY_APPLY;
	}

	//�����˹�ת��ҵ��ӿ�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo,(void*)&param, P2P_TRANSFER);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara P2PTransfer failed. (%d)", iRet);
	}

	//�ͷ���Դ
	delete pResInfo;
	pResInfo = NULL;
	return iRet;//lint !e438
}
