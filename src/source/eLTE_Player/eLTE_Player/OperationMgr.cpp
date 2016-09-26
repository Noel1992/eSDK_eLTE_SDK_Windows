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
#include "OperationMgr.h"
#include "ProvisionMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"


HANDLE OperationMgr::m_hHangupEvent = NULL;
OperationMgr::OperationMgr(void)
	: m_userId("")
	, m_passwd("")
	, m_localIP("")
	, m_serverIP("")
	, m_sipPort("")
	, m_strFrontFormat("")
	, m_strBackFormat("")
	, m_bLoginSuccess(FALSE)
	, m_pCeLTE_PlayerCtrl(NULL)
{
	m_mapResource.clear();
	m_hHangupEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}


OperationMgr::~OperationMgr(void)
{
	try
	{
		m_pCeLTE_PlayerCtrl = NULL;
		m_mapResource.clear();
	}
	catch (...)
	{
		m_pCeLTE_PlayerCtrl = NULL;
	}
}


OperationMgr& OperationMgr::Instance()
{
	static OperationMgr s_OperationMgr;
	return s_OperationMgr;
}

// ����̨ע��
int OperationMgr::DcLogin(const std::string& userId, const std::string& passwd, 
	const std::string& serverIP, const std::string& localIP, const std::string& sipPort)
{
	LOG_TRACE();
	INFO_PARAM4(userId, serverIP, localIP, sipPort);

	// ���У��
	if (!m_userId.empty())
	{
		LOG_RUN_ERROR("Please logout first.");
		return eLTE_ERR_FAILED;
	}

	// ��ȡSDKʵ��ָ��
	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_ERR_FAILED;
	}
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ���õ���̨״̬
	DcState* pDC = pAC->getConsoleState();
	if (NULL == pDC)
	{
		LOG_RUN_ERROR("getConsoleState failed.");
		return eLTE_ERR_FAILED;
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
		return eLTE_ERR_FAILED;
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

	// ���汾������
	m_userId = userId;
	m_passwd = passwd;
	m_localIP = localIP;
	m_serverIP = serverIP;
	m_sipPort = sipPort;

	return eLTE_ERR_SUCCESS;
}

// ����SDK����ҵ��
int OperationMgr::DcStartRunning()
{
	LOG_TRACE();

	// У���Ƿ��½
	if (m_userId.empty())
	{
		// û�е�½���ǳ��ɹ�����ҪstartRunning
		return eLTE_ERR_SUCCESS;
	}

	if (m_bLoginSuccess)
	{
		return eLTE_ERR_SUCCESS;
	}

	// ��½�ɹ���ʾ
	m_bLoginSuccess = TRUE;

	// ��ȡSDKʵ��ָ��
	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ����SDK����ҵ��
	int iRet = pAC->startRunning(m_serverIP.c_str());
	if (0 != iRet)
	{
		LOG_RUN_ERROR("startRunning failed. (%d)", iRet);
		return iRet;
	}

	return eLTE_ERR_SUCCESS;
}

// ����̨ע��
int OperationMgr::DcLogout(const std::string& userId)
{
	LOG_TRACE();
	INFO_PARAM1(userId);

	// ���У��
	if (m_userId != userId)
	{
		INFO_PARAM2(userId, m_userId);
		LOG_RUN_ERROR("Please login first.");
		return eLTE_ERR_NOT_LOGIN;
	}

	// ״̬У��
	if (!m_bLoginSuccess)
	{
		LOG_RUN_ERROR("Please wait login result.");
		return eLTE_ERR_WAIT_LOGIN_RESULT;
	}

	// ��½�ɹ���ʾ
	m_bLoginSuccess = FALSE;

	// ��ȡSDKʵ��ָ��
	ApplicationContext * pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_ERR_FAILED;
	}
	OperationManager * pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
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
		return eLTE_ERR_FAILED;
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

	// �����ر���
	m_userId.clear();
	m_passwd.clear();
	m_localIP.clear();
	m_serverIP.clear();
	m_sipPort.clear();

	return iRet;
}

// ����״̬�ϱ�
int OperationMgr::DcTriggerStatusReport(bool bEnableStatusReport) const
{
	LOG_TRACE();
	INFO_PARAM1(bEnableStatusReport);

	ApplicationContext* pAC = ApplicationContext::getInstance();
	if (NULL == pAC)
	{
		LOG_RUN_ERROR("ApplicationContext get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// �����ϱ�״̬
	pAC->triggerStatusReport(bEnableStatusReport);

	return eLTE_ERR_SUCCESS;
}

// ����̨ȡ������
int OperationMgr::DcVolUnMute(int iResId, Mute_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, param.task_type, param.call_type);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ��Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VOL_UNMUTE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara VOL_UNMUTE failed. (%d)", iRet);
	}

	// �ͷ���Ƶ��Դ
	delete pResInfo;

	return iRet;
}

// ����ִ̨�о���
int OperationMgr::DcVolMute(int iResId, Mute_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM3(iResId, param.task_type, param.call_type);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ��Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, VOL_MUTE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara VOL_MUTE failed. (%d)", iRet);
	}

	// �ͷ���Ƶ��Դ
	delete pResInfo;

	return iRet;
}

// ����/ȥ����GIS�ն�
int OperationMgr::GISSubscribe(int iResId, GIS_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM4(iResId, (int)param.subtype, param.msg_body, param.subscriber);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ��Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	param.msg_body = eLTE_Tool::ANSIToUTF8(param.msg_body);

	// ������Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, GIS_SUBSCRIBE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara GIS_SUBSCRIBE failed. (%d)", iRet);
	}

	// �ͷ���Ƶ��Դ
	delete pResInfo;

	return iRet;
}

// ������
int OperationMgr::SDSSendMessage(int iResId, SDS_parameter& param) const
{
	LOG_TRACE();
	INFO_PARAM4(iResId, param.sdstype, param.msg_body, param.receiver);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ���������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
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

// ����ִ̨�п���
int OperationMgr::MuteControl(int iResId, int param) const
{
	LOG_TRACE();
	INFO_PARAM2(iResId, param);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ��Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, MUTE_CONTROL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara MUTE_CONTROL failed. (%d)", iRet);
	}

	// �ͷ���Ƶ��Դ
	delete pResInfo;

	return iRet;
}

// ����̨������Ƶ�ش�
int OperationMgr::DcVideoMonitor(int iResId, Video_UL_parameter& param, bool bNeedSave)
{
	LOG_TRACE();
	INFO_PARAM5(iResId, param.fmtvalue, param.cameratype, param.mutetype, param.user_confirm_type);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ��Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, P2P_VIDEO_MONITOR);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara P2P_VIDEO_MONITOR failed. (%d)", iRet);
	}

	// �ͷ���Ƶ��Դ
	delete pResInfo;

	// ������Ƶ�ش���Դ�б�
	if (OP_OK_ACCEPTED == iRet && bNeedSave)
	{
		std::map<int, STVideoParam>::iterator itor = m_mapResource.find(iResId);
		if (m_mapResource.end() != itor)
		{
			STVideoParam& videoParam = itor->second;
			videoParam.strFmtValue = param.fmtvalue;
			videoParam.strCameraType = param.cameratype;
			videoParam.strUserCfmType = param.user_confirm_type;
			videoParam.strMuteType = param.mutetype;
		}
		else
		{
			STVideoParam videoParam;
			videoParam.strFmtValue = param.fmtvalue;
			videoParam.strCameraType = param.cameratype;
			videoParam.strUserCfmType = param.user_confirm_type;
			videoParam.strMuteType = param.mutetype;
			(void)m_mapResource.insert(std::make_pair(iResId, videoParam));
		}
	}

	return iRet;
}

// ����̨�Ҷ���Ƶ�ش�
int OperationMgr::DcVideoHangup(int iResId, bool bNeedSave)
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Ƶ��Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// �Ҷ���Ƶ�ش�
	Op_Return_t iRet = pOM->invokeOp(pResInfo, P2P_VIDEO_HANGUP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_VIDEO_HANGUP failed. (%d)", iRet);	
	}

	// �ͷ���Ƶ��Դ
	delete pResInfo;

	// ����Ƶ�ش���Դ�б���ɾ��
	if (OP_OK_ACCEPTED == iRet && bNeedSave)
	{
		std::map<int, STVideoParam>::iterator itor = m_mapResource.find(iResId);
		if (m_mapResource.end() != itor)
		{
			m_mapResource.erase(itor);
		}
	}

	return iRet;
}

// �л�����ͷ
int OperationMgr::DcVideoReverse(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, VideoReverseRun, (void*)iResId, CREATE_SUSPENDED, NULL);

	if (NULL == hThread)
	{
		LOG_RUN_ERROR("_beginthreadex failed.");
		return eLTE_ERR_FAILED;
	}
	ResumeThread(hThread);
	CloseHandle(hThread);

	return eLTE_ERR_SUCCESS;
}

unsigned int __stdcall OperationMgr::VideoReverseRun(void* lparam)
{
	LOG_TRACE();

	int iResId = (int)lparam;

	// ������Ƶ�ش���Դ
	std::map<int, STVideoParam>::iterator itor = OperationMgr::Instance().m_mapResource.find(iResId);
	if (OperationMgr::Instance().m_mapResource.end() == itor)
	{
		LOG_RUN_ERROR("Not found the resourceID in m_mapResource.");
		return eLTE_ERR_FAILED;
	}

	// �Ҷ���Ƶ�ش�
	int iRet = OperationMgr::Instance().DcVideoHangup(itor->first, false);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("DcVideoHangup failed.");
		return iRet;//lint !e732
	}

	// �ȴ��Ҷϳɹ�
	static const int WAIT_TIME_EVENT = 10000;
	DWORD dwRet = WaitForSingleObject(m_hHangupEvent, WAIT_TIME_EVENT);
	if (WAIT_TIMEOUT == dwRet)
	{
		LOG_RUN_ERROR("DcVideoHangup timeout.");
		return eLTE_ERR_TIMEOUT;
	}

	// ������Ƶ�ش�����
	STVideoParam& videoParam = itor->second;
	videoParam.strCameraType = ("0" == videoParam.strCameraType) ? "1" : "0";//�л�����ͷ
	videoParam.strUserCfmType = "0";//Ĭ�ϲ���Ҫȷ��
	std::string strFormat = ("0" == videoParam.strCameraType) ? OperationMgr::Instance().m_strFrontFormat : OperationMgr::Instance().m_strBackFormat;//ʹ�������ļ��е���Ƶ��ʽ
	if (strFormat.empty())
	{
		strFormat = videoParam.strFmtValue;
	}
	videoParam.strFmtValue = strFormat;

	Video_UL_parameter param;
	param.fmtvalue = (char *)strFormat.c_str();//lint !e1773
	param.cameratype = (char *)videoParam.strCameraType.c_str();//lint !e1773
	param.user_confirm_type = (char *)videoParam.strUserCfmType.c_str();//lint !e1773
	param.mutetype = (char *)videoParam.strMuteType.c_str();//lint !e1773

	// ������Ƶ�ش�
	iRet = OperationMgr::Instance().DcVideoMonitor(itor->first, param, false);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("DcVideoMonitor failed.");
	}
	else
	{
		// �������¼�����
		PlayerEventParamInfo* pInfo = new PlayerEventParamInfo;
		if (NULL != pInfo)//lint !e774
		{
			pInfo->strFmtValue = videoParam.strFmtValue;
			pInfo->strCameraType = videoParam.strCameraType;
			pInfo->strUserCfmType = videoParam.strUserCfmType;
			pInfo->strMuteType = videoParam.strMuteType;
			// �������л���ͷ�¼�
			if (NULL != OperationMgr::Instance().m_pCeLTE_PlayerCtrl)
			{
				OperationMgr::Instance().m_pCeLTE_PlayerCtrl->PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_REVERSE, (LPARAM)pInfo);
			}
		}
	}//lint !e593

	return iRet;//lint !e732
}

// ��ȡ�Ƿ�������
std::string OperationMgr::GetMuteType(int iResId)
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	std::string strMuteType = "-1";

	// ������Ƶ�ش���Դ
	std::map<int, STVideoParam>::iterator itor = m_mapResource.find(iResId);
	if (m_mapResource.end() != itor)
	{
		strMuteType = itor->second.strMuteType;
	}

	return strMuteType;
}

// ����Ⱥ��
int OperationMgr::DcSubscribeGroup(const std::string& groupId, unsigned long ulEableSubscribeGroup) const
{
	LOG_TRACE();
	INFO_PARAM2(groupId, ulEableSubscribeGroup);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ����Ⱥ����Դ
	ResourceInfo* pResInfo = new ResourceInfo(eLTE_Tool::String2Int(groupId));
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("pResInfo is null.");
		return eLTE_ERR_FAILED;
	}

	// ȥ����Ⱥ��
	Op_Return_t iRet;
	if (0 == ulEableSubscribeGroup)
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

	return iRet;
}

// ������̬����
int OperationMgr::DcCreateDGNA(DGNA_parameter& param) const
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
		return eLTE_ERR_INVALID_PARAM;
	}
	if (param.priority < MIN_PRIORITY_VAR || param.priority > MAX_PRIORITY_VAR)
	{
		LOG_RUN_ERROR("priority is greater than 15 or less than 1.");
		return eLTE_ERR_INVALID_PARAM;
	}
	if (param.maxPeriod < MIN_PERIOD_VAR || param.maxPeriod > MAX_PERIOD_VAR)
	{
		LOG_RUN_ERROR("maxPeriod is greater than 65535 or less than 1.");
		return eLTE_ERR_INVALID_PARAM;
	}

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	int iDcID = eLTE_Tool::String2Int(m_userId);
	ResourceInfo* pResInfo = new ResourceInfo(iDcID);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ������̬����
	int iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, DGNA_CREATE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara DGNA_CREATE failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

// ɾ����̬����
int OperationMgr::DcCancelDGNA(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ɾ����̬����
	int iRet = pOM->invokeOp_multipara(pResInfo, NULL, DGNA_CANCEL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara DGNA_CANCEL failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

// ������ʱ��
int OperationMgr::DcCreateTEMPG(DGNA_parameter& param) const
{
	LOG_TRACE();

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	int iDcId = eLTE_Tool::String2Int(m_userId);
	ResourceInfo* pResInfo = new ResourceInfo(iDcId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ������ʱ��
	int iRet = pOM->invokeOp_multipara(pResInfo, (void*)&param, TEMPGRP_CREATE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp_multipara TEMPGRP_CREATE failed. (%d)");
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

// ����һ����ʱ���
int OperationMgr::DcGetNextTempGroupID() const
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

//�����������
int OperationMgr::DcP2PDial(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// �����������
	int iRet = pOM->invokeOp(pResInfo, P2P_DIALOUT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_DIALOUT failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//�����������
int OperationMgr::DcP2PRecv(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// �����������
	int iRet = pOM->invokeOp(pResInfo, P2P_RECV);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_RECV failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//�ܽ��������
int OperationMgr::DcP2PReject(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// �ܽ��������
	int iRet = pOM->invokeOp(pResInfo, P2P_REJECT);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_REJECT failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//�Ҷ��������
int OperationMgr::DcP2PHangup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// �Ҷ��������
	int iRet = pOM->invokeOp(pResInfo, P2P_HANGUP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_HANGUP failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//����Ⱥ��
int OperationMgr::DcJoinGroup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ����Ⱥ��
	int iRet = pOM->invokeOp(pResInfo, GRP_JOIN);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp GRP_JOIN failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//���Ĳ��Զ�����Ⱥ��
int OperationMgr::DcSubJoinGroup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ���Ĳ��Զ�����Ⱥ��
	int iRet = pOM->invokeOp(pResInfo, BATCH_CFG);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp BATCH_CFG failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//����������̶�����ʱ���������Ȩ
int OperationMgr::DcPTTDial(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ����������̶�����ʱ���������Ȩ
	int iRet = pOM->invokeOp(pResInfo, PTT_DIAL);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_DIAL failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//�ͷŻ�Ȩ
int OperationMgr::DcPTTRelease(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// �ͷŻ�Ȩ
	int iRet = pOM->invokeOp(pResInfo, PTT_RELEASE);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_RELEASE failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//�˳����
int OperationMgr::DcPTTHangup(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// �˳����
	int iRet = pOM->invokeOp(pResInfo, PTT_HANGUP);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_HANGUP failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//����������
int OperationMgr::DcPTTEmergency(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ����������
	int iRet = pOM->invokeOp(pResInfo, PTT_EMERGENCY);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp PTT_EMERGENCY failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//����̨ǿ�����
int OperationMgr::GroupBreakoff(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ǿ�����
	int iRet = pOM->invokeOp(pResInfo, GRP_BREAKOFF);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp GRP_BREAKOFF failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//����̨ǿ����
int OperationMgr::P2PBreakoff(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ǿ����
	int iRet = pOM->invokeOp(pResInfo, P2P_BREAKOFF);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_BREAKOFF failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

//������ǿ����+�����µ����
int OperationMgr::P2PBreakin(int iResId) const
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	// ��ȡSDKʵ��ָ��
	OperationManager* pOM = OperationManager_T::instance();
	if (NULL == pOM)
	{
		LOG_RUN_ERROR("OperationManager_T get instance failed.");
		return eLTE_ERR_FAILED;
	}

	// ������Դ
	ResourceInfo* pResInfo = new ResourceInfo(iResId);
	if (NULL == pResInfo)//lint !e774
	{
		LOG_RUN_ERROR("new ResourceInfo failed.");
		return eLTE_ERR_FAILED;
	}

	// ����
	int iRet = pOM->invokeOp(pResInfo, P2P_BREAKIN);
	if (OP_OK_ACCEPTED != iRet)
	{
		LOG_RUN_ERROR("invokeOp P2P_BREAKIN failed. (%d)", iRet);
	}

	// �ͷ���Դ
	delete pResInfo;

	return iRet;
}

void OperationMgr::SetParam(const std::string& strFrontFormat, const std::string& strBackFormat)
{
	m_strFrontFormat = strFrontFormat;
	m_strBackFormat = strBackFormat;
}
