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
#include "eLTE_PlayerCtrl.h"


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

// ����̨������Ƶ�ش�
int OperationMgr::DcVideoMonitor(int iResId, const STVideoParam& param, bool bNeedSave)
{
	LOG_TRACE();
	INFO_PARAM5(iResId, param.strFmtValue, param.strCameraType, param.strMuteType, param.strUserCfmType);

	// ������Ƶ�ش���Դ�б�
	if (bNeedSave)
	{
		std::map<int, STVideoParam>::iterator itor = m_mapResource.find(iResId);
		if (m_mapResource.end() != itor)
		{
			STVideoParam& videoParam = itor->second;
			videoParam = param;
		}
		else
		{
			STVideoParam videoParam;
			videoParam = param;
			(void)m_mapResource.insert(std::make_pair(iResId, videoParam));
		}
	}

	return eLTE_ERR_SUCCESS;
}

// ����̨�Ҷ���Ƶ�ش�
int OperationMgr::DcVideoHangup(int iResId, bool bNeedSave)
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	ELTE_INT32 iRet = ELTE_SDK_StopRealPlay(eLTE_Tool::Int2String(iResId).c_str());

	// ����Ƶ�ش���Դ�б���ɾ��
	if (eLTE_ERR_SUCCESS == iRet && bNeedSave)
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

	//EP820��֧�������л�ǰ��������ͷ����ȴ�����
	Sleep(2000);

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

	// ������Ƶ�ش�
	std::string strVideoParam;
	strVideoParam.append("<Content>");
	strVideoParam.append("<VideoParam>");
	strVideoParam.append("<VideoFormat>");
	strVideoParam.append(videoParam.strFmtValue);
	strVideoParam.append("</VideoFormat>");
	strVideoParam.append("<CameraType>");
	strVideoParam.append(videoParam.strCameraType);
	strVideoParam.append("</CameraType>");
	strVideoParam.append("<UserConfirmType>");
	strVideoParam.append(videoParam.strUserCfmType);
	strVideoParam.append("</UserConfirmType>");
	strVideoParam.append("<MuteType>");
	strVideoParam.append(videoParam.strMuteType);
	strVideoParam.append("</MuteType>");
	strVideoParam.append("</VideoParam>");
	strVideoParam.append("</Content>");
	iRet = ELTE_SDK_StartRealPlay(eLTE_Tool::Int2String(itor->first).c_str(), strVideoParam.c_str());

	if(eLTE_ERR_SUCCESS == iRet)
	{
		iRet = OperationMgr::Instance().DcVideoMonitor(itor->first, videoParam, false);
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
		if (NULL != OperationMgr::Instance().m_pCeLTE_PlayerCtrl)
		{
			
		}
	}

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

// ��ȡ��Ƶ����
std::string OperationMgr::GetVideoType(int iResId)
{
	LOG_TRACE();
	INFO_PARAM1(iResId);

	std::string strVideoType = "-1";

	// ������Ƶ�ش���Դ
	std::map<int, STVideoParam>::iterator itor = m_mapResource.find(iResId);
	if (m_mapResource.end() != itor)
	{
		strVideoType = itor->second.strVideoType;
	}

	return strVideoType;
}

void OperationMgr::SetParam(const std::string& strFrontFormat, const std::string& strBackFormat)
{
	m_strFrontFormat = strFrontFormat;
	m_strBackFormat = strBackFormat;
}
