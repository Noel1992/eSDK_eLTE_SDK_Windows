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
#include "ConfigMgr.h"
#include "MediaPlayer.h"
#include "VideoPane.h"
#include "OperationMgr.h"
#include "eLTE_Tool.h"
#include "eLTE_Log.h"


// 配置文件读取返回值检测
#define CHECK_READ_CONFIG_FILE(x)			\
	if (0 >= (int)x)						\
	{										\
		return eLTE_ERR_CONFIG_FILE_READ;	\
	}


ConfigMgr::ConfigMgr(void)
	: m_snapshotFormat(0)
	, m_savePath("")
	, m_skinType(0)
	, m_strLogoPath(_T(""))
	, m_ulVolume(0)
	, m_strFrontFormat("")
	, m_strBackFormat("")
{
}


ConfigMgr::~ConfigMgr(void)
{
}

eLTE_RET ConfigMgr::Start(MediaPlayer& mediaPlayer, CVideoPane& videoPane, OperationMgr& operationMgr)
{
	// 相对路径转绝对路径
	eLTE_Tool::RelativePathToAbsolute(m_savePath);
	eLTE_Tool::RelativePathToAbsoluteOcx(m_strLogoPath);

	// 设置MediaPlayer抓图参数
	mediaPlayer.InitParam(m_snapshotFormat, m_savePath);

	// 设置音频播放音量
	mediaPlayer.InitParam(m_ulVolume);

	// 设置CVideoPane皮肤
	videoPane.SetParam(m_skinType, m_strLogoPath);

	// 设置OperationMgr参数
	operationMgr.SetParam(m_strFrontFormat, m_strBackFormat);

	return eLTE_ERR_SUCCESS;
}

eLTE_RET ConfigMgr::Init()
{
	// 读取配置文件
	eLTE_RET iRet = LoadConfigFile();
	if (eLTE_ERR_SUCCESS != iRet)
	{
		return iRet;
	}

	return eLTE_ERR_SUCCESS;
}

eLTE_RET ConfigMgr::Exit()
{
	m_snapshotFormat = 0;
	m_savePath = "";
	m_skinType = 0;
	m_strLogoPath = _T("");
	m_ulVolume = 0;
	m_strFrontFormat = "";
	m_strBackFormat = "";

	return eLTE_ERR_SUCCESS;
}

eLTE_RET ConfigMgr::LoadConfigFile()
{
	LOG_TRACE();
	CString szPath = eLTE_Tool::GetOcxPath();
	if (!eLTE_Tool::IsExistPath(szPath))
	{
		return eLTE_ERR_CONFIG_FILE_LOAD;
	}	
	szPath.Append(CONFIG_FILE_NAME);

	TCHAR tchValue[LENGTH_LONG] = {0};

	// snapshot_config
	DWORD dwRet = GetPrivateProfileString(SECTION_SNAPSHOT_CONFIG, _T("snapshotFormat"), _T(""), tchValue, LENGTH_SHORT, szPath);
	CHECK_READ_CONFIG_FILE(dwRet);
	m_snapshotFormat = eLTE_Tool::WString2Int(tchValue);
	if(eLTE_SNAPSHOT_JPG != m_snapshotFormat && eLTE_SNAPSHOT_BMP != m_snapshotFormat)
	{		
		LOG_RUN_ERROR("SnapShotFormat %d is invalid, set default %d.", m_snapshotFormat, eLTE_SNAPSHOT_JPG);
		m_snapshotFormat = eLTE_SNAPSHOT_JPG;
	}

	memset(tchValue, 0, sizeof(TCHAR)*LENGTH_LONG);
	dwRet = GetPrivateProfileString(SECTION_SNAPSHOT_CONFIG, _T("savePath"), _T(""), tchValue, LENGTH_LONG, szPath);
	CHECK_READ_CONFIG_FILE(dwRet);
	m_savePath = eLTE_Tool::UnicodeToANSI(tchValue);

	// skin_config
	memset(tchValue, 0, sizeof(TCHAR)*LENGTH_LONG);
	dwRet = GetPrivateProfileString(SECTION_SKIN_CONFIG, _T("skinType"), _T(""), tchValue, LENGTH_SHORT, szPath);
	CHECK_READ_CONFIG_FILE(dwRet);
	m_skinType = eLTE_Tool::WString2Int(tchValue);
	if(eLTE_SKIN_BLACK != m_skinType && eLTE_SKIN_WHITE != m_skinType)
	{
		LOG_RUN_ERROR("SkinType %d is invalid, set default %d.", m_skinType, eLTE_SKIN_BLACK);
		m_skinType = eLTE_SKIN_BLACK;
	}

	memset(tchValue, 0, sizeof(TCHAR)*LENGTH_LONG);
	dwRet = GetPrivateProfileString(SECTION_SKIN_CONFIG, _T("logoPath"), _T(""), tchValue, LENGTH_LONG, szPath);
	CHECK_READ_CONFIG_FILE(dwRet);
	m_strLogoPath = tchValue;

	// audio_config
	memset(tchValue, 0, sizeof(TCHAR)*LENGTH_LONG);
	dwRet = GetPrivateProfileString(SECTION_AUDIO_CONFIG, _T("volume"), _T(""), tchValue, LENGTH_SHORT, szPath);
	CHECK_READ_CONFIG_FILE(dwRet);
	m_ulVolume = eLTE_Tool::WString2UInt(tchValue);
	if(m_ulVolume > 100)
	{
		LOG_RUN_ERROR("Volume %d is invalid, set default 100.", m_skinType);
		m_ulVolume = 100;
	}

	// video_param
	memset(tchValue, 0, sizeof(TCHAR)*LENGTH_LONG);
	dwRet = GetPrivateProfileString(SECTION_VIDEO_PARAM, _T("frontFormat"), _T(""), tchValue, LENGTH_SHORT, szPath);
	CHECK_READ_CONFIG_FILE(dwRet);
	m_strFrontFormat = eLTE_Tool::UnicodeToANSI(tchValue);

	memset(tchValue, 0, sizeof(TCHAR)*LENGTH_LONG);
	dwRet = GetPrivateProfileString(SECTION_VIDEO_PARAM, _T("backFormat"), _T(""), tchValue, LENGTH_SHORT, szPath);
	CHECK_READ_CONFIG_FILE(dwRet);
	m_strBackFormat = eLTE_Tool::UnicodeToANSI(tchValue);

	return eLTE_ERR_SUCCESS;
}
