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
#include "MediaPlayer.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"
#include "AppContextMgr.h"
#include "eLTE_PlayerCtrl.h"


#define IVS_MP_LOG_PATH (".\\ivs_mp_log\\")

// ��ʼ����־
BOOL MediaPlayer::m_bInit = FALSE;

MediaPlayer::MediaPlayer(void)
	: m_ulNetChannel(0)
	, m_ulPalyerChannel(0)	
	, m_bStartPlayer(FALSE)
	, m_snapshotFormat(0)
	, m_savePath("")
	, m_bAllowPlaySound(FALSE)
	, m_strResId("")
	, m_strLocalIP("")
	, m_strRemoteIP("")
	, m_uiLocalVideoPort(0)
	, m_uiLocalAudioPort(0)
	, m_uiRemoteVideoPort(0)
	, m_uiRemoteAudioPort(0)
{

}


MediaPlayer::~MediaPlayer(void)
{

}

int MediaPlayer::InitPlayerParam(IVS_CHAR* pLocalIP, IVS_UINT32 uiLocalVideoPort, IVS_UINT32 uiLocalAudioPort)
{
	LOG_TRACE();
	INFO_PARAM2(uiLocalVideoPort, uiLocalAudioPort);
	
	// ����ж�
	if (NULL == pLocalIP)
	{
		LOG_RUN_ERROR("pLocalIP is null.");
		return eLTE_ERR_INVALID_PARAM;
	}

	// ��ֹͣ����
	(void)StopPlayer();

	// ����IP����
	m_strLocalIP = pLocalIP;

	// netsource init
	IVS_INT32 iRet = IVS_SUCCEED;
	if (!m_bInit)
	{
		iRet = IVS_NETSOURCE_Init(IVS_MP_LOG_PATH);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_NETSOURCE_Init failed. (%d)", iRet);
			return iRet;
		}
	}

	// ��ȡ����ͨ��
	iRet = IVS_NETSOURCE_GetChannel(&m_ulNetChannel);
	if (IVS_SUCCEED != iRet)
	{
		LOG_RUN_ERROR("IVS_NETSOURCE_GetChannel failed. (%d)", iRet);
		m_ulNetChannel = 0;
		return iRet;
	}

	// player init
	if (!m_bInit)
	{
		iRet = IVS_PLAY_Init(IVS_MP_LOG_PATH);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_PLAY_Init failed. (%d)", iRet);
			return iRet;
		}
		m_bInit = TRUE;
	}

	// ��ȡ���õ�ý��ͨ����
	iRet = IVS_PLAY_GetChannel(&m_ulPalyerChannel);
	if (IVS_SUCCEED != iRet)
	{
		LOG_RUN_ERROR("IVS_PLAY_GetChannel failed. (%d)", iRet);
		m_ulPalyerChannel = 0;
		return iRet;
	}

	// ����ý�����ص�����
	IVS_NETSOURCE_SetDataCallBack(m_ulNetChannel, NetDateCallBack, (IVS_VOID *)this);
	// �����쳣�ص�����
	IVS_NETSOURCE_SetExceptionCallBack(NetExceptionCallBack, (IVS_VOID *)this);

	// ����Э�����ͣ�1-UDP 2-TCP��Ĭ��Ϊ1
	iRet = IVS_NETSOURCE_SetProtocolType(m_ulNetChannel, PROTOCOL_RTP_OVER_UDP);
	if (IVS_SUCCEED != iRet)
	{
		INFO_PARAM2(m_ulNetChannel, PROTOCOL_RTP_OVER_UDP);
		LOG_RUN_ERROR("IVS_NETSOURCE_SetProtocolType failed. (%d)", iRet);
		return iRet;
	}

	// ���ñ��ؽ���������Ip��ַ
	iRet = IVS_NETSOURCE_SetLocalRecvStreamIP(m_ulNetChannel, pLocalIP);
	if (IVS_SUCCEED != iRet)
	{
		INFO_PARAM2(m_ulNetChannel, pLocalIP);
		LOG_RUN_ERROR("IVS_NETSOURCE_SetLocalRecvStreamIP failed. (%d)", iRet);
		return iRet;
	}

	// ���ñ��ؽ��������Ķ˿ں�
	iRet = IVS_NETSOURCE_SetLocalRecvStreamPort(m_ulNetChannel, uiLocalVideoPort, uiLocalAudioPort);
	if (IVS_SUCCEED != iRet)
	{
		INFO_PARAM3(m_ulNetChannel, uiLocalVideoPort, uiLocalAudioPort);
		LOG_RUN_ERROR("IVS_NETSOURCE_SetLocalRecvStreamPort failed. (%d)", iRet);
	}

	return iRet;
}

int MediaPlayer::UninitPlayer()
{
	LOG_TRACE();
	
	(void)StopPlayer();

	if (m_bInit)
	{
		IVS_PLAY_UnInit();
		IVS_NETSOURCE_UnInit();
		m_bInit = FALSE;
	}

	if (m_bAllowPlaySound)
	{
		(void)StopAudio();
	}

	return eLTE_ERR_SUCCESS;
}

int MediaPlayer::StartPlayer(IVS_CHAR* pRemoteIP, IVS_UINT32 uiRemoteVideoPort, IVS_UINT32 uiRemoteAudioPort, HWND hVideoPlayer)
{
	LOG_TRACE();
	INFO_PARAM3(uiRemoteVideoPort, uiRemoteAudioPort, hVideoPlayer);
	
	// ����ж�
	if (NULL == pRemoteIP)
	{
		LOG_RUN_ERROR("pRemoteIP is null.");
		return eLTE_ERR_INVALID_PARAM;
	}
	if (NULL == hVideoPlayer)
	{
		LOG_RUN_ERROR("hVideoPlayer is null.");
		return eLTE_ERR_INVALID_PARAM;
	}

	// �Ƿ��Ѿ�����
	if (m_bStartPlayer)
	{
		LOG_RUN_INFO("Is playing. please stop player first.");
		return eLTE_ERR_VIDEO_STATE;
	}

	// ����IP����
	m_strRemoteIP = pRemoteIP;
	if(CeLTE_PlayerCtrl::m_ulBypass)
	{
		// ���������������߳�;
		IVS_INT32 iRet = IVS_NETSOURCE_OpenNetStream(m_ulNetChannel, pRemoteIP, uiRemoteVideoPort, uiRemoteAudioPort);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_NETSOURCE_OpenNetStream failed. (%d)", iRet);
			(void)IVS_NETSOURCE_FreeChannel(m_ulNetChannel);
			(void)IVS_PLAY_FreeChannel(m_ulPalyerChannel);
			m_ulPalyerChannel = 0;
			m_ulNetChannel = 0;
			return iRet;
		}

		// ��������
		iRet = IVS_NETSOURCE_StartRecvStream(m_ulNetChannel);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_NETSOURCE_StartRecvStream failed. (%d)", iRet);
			(void)IVS_NETSOURCE_CloseNetStream(m_ulNetChannel);
			(void)IVS_NETSOURCE_FreeChannel(m_ulNetChannel);
			(void)IVS_PLAY_FreeChannel(m_ulPalyerChannel);
			m_ulPalyerChannel = 0;
			m_ulNetChannel = 0;
			return iRet;
		}

		MEDIA_ATTR AttrAudio = {0};
		AttrAudio.rtpmap = PAY_LOAD_TYPE_PCMA;
		AttrAudio.type   = AUDIO_DEC_G711U;
		AttrAudio.samplerate = 8000;
		MEDIA_ATTR AttrVideo = {0};
		AttrVideo.rtpmap = PAY_LOAD_TYPE_H264;
		AttrVideo.type   = VIDEO_DEC_H264;
		AttrVideo.samplerate = 90000;
		// ����ý����Ϣ
		iRet = IVS_PLAY_SetMediaAttribute(m_ulPalyerChannel, &AttrVideo, &AttrAudio);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_PLAY_SetMediaAttribute failed. (%d)", iRet);
			(void)IVS_NETSOURCE_CloseNetStream(m_ulNetChannel);
			(void)IVS_NETSOURCE_FreeChannel(m_ulNetChannel);
			(void)IVS_PLAY_FreeChannel(m_ulPalyerChannel);
			m_ulPalyerChannel = 0;
			m_ulNetChannel = 0;
			return iRet;
		}

		// ����ͨ��
		iRet = IVS_PLAY_OpenStream(m_ulPalyerChannel, SERVICE_TYPE_REALVIDEO, SOURCE_RTP);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_PLAY_OpenStream failed. (%d)", iRet);
			(void)IVS_NETSOURCE_CloseNetStream(m_ulNetChannel);
			(void)IVS_NETSOURCE_FreeChannel(m_ulNetChannel);
			(void)IVS_PLAY_FreeChannel(m_ulPalyerChannel);
			m_ulPalyerChannel = 0;
			m_ulNetChannel = 0;
			return iRet;
		}

		// ��������
		iRet = IVS_PLAY_StartPlay(m_ulPalyerChannel);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_PLAY_StartPlay failed. (%d)", iRet);
			(void)IVS_NETSOURCE_CloseNetStream(m_ulNetChannel);
			(void)IVS_NETSOURCE_FreeChannel(m_ulNetChannel);
			(void)IVS_PLAY_CloseStream(m_ulPalyerChannel);
			(void)IVS_PLAY_FreeChannel(m_ulPalyerChannel);
			m_ulPalyerChannel = 0;
			m_ulNetChannel = 0;
			return iRet;
		}
		// ���ò��Ŵ��ھ��
		iRet = IVS_PLAY_SetPlayWindow(m_ulPalyerChannel, hVideoPlayer);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_PLAY_SetPlayWindow failed. (%d)", iRet);
			(void)IVS_NETSOURCE_CloseNetStream(m_ulNetChannel);
			(void)IVS_NETSOURCE_FreeChannel(m_ulNetChannel);
			(void)IVS_PLAY_CloseStream(m_ulPalyerChannel);
			(void)IVS_PLAY_StopPlay(m_ulPalyerChannel);
			(void)IVS_PLAY_FreeChannel(m_ulPalyerChannel);
			m_ulPalyerChannel = 0;
			m_ulNetChannel = 0;
			return iRet;
		}
		// ��ͨ���ʹ���������Ƶ��ʾ������1Ϊԭʼ�ߴ磬2Ϊȫ��
		iRet = IVS_PLAY_SetDisplayAspectRatio(m_ulPalyerChannel, 1);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_PLAY_SetDisplayAspectRatio failed. (%d)", iRet);
			(void)IVS_NETSOURCE_CloseNetStream(m_ulNetChannel);
			(void)IVS_NETSOURCE_FreeChannel(m_ulNetChannel);
			(void)IVS_PLAY_CloseStream(m_ulPalyerChannel);
			(void)IVS_PLAY_StopPlay(m_ulPalyerChannel);
			(void)IVS_PLAY_FreeChannel(m_ulPalyerChannel);
			m_ulPalyerChannel = 0;
			m_ulNetChannel = 0;
			return iRet;
		}
	}
	m_bStartPlayer = TRUE;

	return eLTE_ERR_SUCCESS;
}

int MediaPlayer::StopPlayer()
{
	LOG_TRACE();
	if (m_bStartPlayer)
	{
		if (CeLTE_PlayerCtrl::m_ulBypass)
		{
			(void)IVS_NETSOURCE_CloseNetStream(m_ulNetChannel);
			(void)IVS_NETSOURCE_FreeChannel(m_ulNetChannel);
			(void)IVS_PLAY_CloseStream(m_ulPalyerChannel);
			(void)IVS_PLAY_StopPlay(m_ulPalyerChannel);
			(void)IVS_PLAY_FreeChannel(m_ulPalyerChannel);
			m_ulPalyerChannel = 0;
			m_ulNetChannel = 0;
		}
	}
	if (m_bAllowPlaySound)
	{
		(void)StopAudio();
	}
	m_bStartPlayer = FALSE;
	return eLTE_ERR_SUCCESS;
}

// ������Ƶ
int MediaPlayer::PlayAudio()
{
	LOG_TRACE();

	// ��Ƶ��������ʼ��
	int iRet = 0; 
	if(CeLTE_PlayerCtrl::m_ulBypass)
	{
		iRet = m_AudioPlayer.WaveOutInit();
		if (eLTE_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("WaveOutInit failed.");
			return iRet;
		}
	}
	else
	{
		// ������������
		Mute_parameter param;
		param.task_type = false;
		param.call_type = 2;//��Ƶ�ش�

		// ִ�о���
		iRet = OperationMgr::Instance().DcVolUnMute(eLTE_Tool::String2Int(m_strResId), param);
		if (eLTE_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Do dc vol unmute failed.");
			return iRet;
		}
	}	

	m_bAllowPlaySound = TRUE;

	return eLTE_ERR_SUCCESS;
}

// ֹͣ��Ƶ
int MediaPlayer::StopAudio()
{
	LOG_TRACE();

	// �Ƚ�ֹwaveOutWrite��Ƶ
	m_bAllowPlaySound = FALSE;
	int iRet = 0;
	if(CeLTE_PlayerCtrl::m_ulBypass)
	{
		// ��Ƶ�������˳�
		iRet = m_AudioPlayer.WaveOutExit();
		if (eLTE_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("WaveOutInit failed.");
		}
	}
	else
	{
		// ������������
		Mute_parameter param;
		param.task_type = true;
		param.call_type = 2;//��Ƶ�ش�

		// ִ�о���
		iRet = OperationMgr::Instance().DcVolMute(eLTE_Tool::String2Int(m_strResId), param);
		if (eLTE_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Do dc vol mute failed.");
			return iRet;
		}
	}

	return iRet;
}

int MediaPlayer::LocalSnapshot(const std::string& strResId, std::string& strSnapshotPath)
{
	LOG_TRACE();	

	strSnapshotPath = GetSnapshotPath(strResId);
	INFO_PARAM2(m_snapshotFormat, strSnapshotPath);
	int iRet = 0;
	if(CeLTE_PlayerCtrl::m_ulBypass)
	{
		iRet = IVS_PLAY_LocalSnapshot(m_ulPalyerChannel, strSnapshotPath.c_str(), (IVS_ULONG)m_snapshotFormat);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_PLAY_LocalSnapshot failed. (%d)", iRet);
			strSnapshotPath = "";
		}
	}
	else
	{
		MediaMgr* pMediaMgr = AppContextMgr::Instance().GetMediaMgr();
		if (NULL != pMediaMgr)
		{
			VideoRender* pVideoRender = dynamic_cast<VideoRender*>(pMediaMgr->getRender(eLTE_Tool::String2Int(m_strResId)));
			if (NULL != pVideoRender)
			{
				iRet = pVideoRender->LocalSnapshot(strSnapshotPath, m_snapshotFormat);
			}
		}
	}

	return iRet;
}

// ��ȡ��ԴID
int MediaPlayer::GetMediaPlayerResID() const
{
	int iResId = 0;
	if (!m_strResId.empty())
	{
		iResId = eLTE_Tool::String2Int(m_strResId);
	}

	return iResId;
}

void MediaPlayer::InitParam(int snapshotFormat, const std::string& savePath)
{
	LOG_TRACE();
	INFO_PARAM2(snapshotFormat, savePath);

	m_snapshotFormat = snapshotFormat;
	m_savePath = savePath;
}

void MediaPlayer::InitParam(DWORD dwVolume)
{
	LOG_TRACE();
	INFO_PARAM1(dwVolume);

	(void)m_AudioPlayer.WaveOutInitVolume(dwVolume);
}

// ����ץͼ����
void MediaPlayer::SetMediaPlayerResID(const std::string& strResId)
{
	LOG_TRACE();
	INFO_PARAM1(strResId);
	
	m_strResId = strResId;
}

// ���ò��Ų����������ǹ������л���Ƶ���������IPʹ���ϴο�����Ƶʱʹ�õ�IP��
void MediaPlayer::SetPlayerParam(IVS_UINT32 uiLocalVideoPort, IVS_UINT32 uiLocalAudioPort,
	IVS_UINT32 uiRemoteVideoPort, IVS_UINT32 uiRemoteAudioPort)
{
	LOG_TRACE();
	INFO_PARAM4(uiLocalVideoPort, uiLocalAudioPort, uiRemoteVideoPort, uiRemoteAudioPort);

	m_uiLocalVideoPort = uiLocalVideoPort;
	m_uiLocalAudioPort = uiLocalAudioPort;
	m_uiRemoteVideoPort = uiRemoteVideoPort;
	m_uiRemoteAudioPort = uiRemoteAudioPort;
}

// ��ʼ��������
int MediaPlayer::StartPlayer(HWND hVideoPlayer)
{
	LOG_TRACE();

	// ��ʼ�����Ų���
	int iRet = 0;
	if(CeLTE_PlayerCtrl::m_ulBypass)
	{
		iRet = InitPlayerParam((IVS_CHAR*)m_strLocalIP.c_str(), m_uiLocalVideoPort, m_uiLocalAudioPort);//lint !e1773
		if (eLTE_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("InitPlayerParam failed.");
			return iRet;
		}
	}

	// ��ʼ����
	iRet = StartPlayer((IVS_CHAR*)m_strRemoteIP.c_str(), m_uiRemoteVideoPort, m_uiRemoteAudioPort, hVideoPlayer);//lint !e1773
	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("StartPlayer failed.");
	}

	return iRet;
}

// ����ץ�ĵ���ͼƬ
int MediaPlayer::LocalSnapshot(std::string& strSnapshotPath)
{
	LOG_TRACE();

	int iRet = LocalSnapshot(m_strResId, strSnapshotPath);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("LocalSnapshot failed.");
	}

	return iRet;
}

std::string MediaPlayer::GetSnapshotPath(const std::string& strResId) const
{
	CTime time = CTime::GetCurrentTime();
	CString szTime = time.Format(_T("%Y%m%d%H%M%S"));
	std::string strSnapshotName = strResId;
	strSnapshotName.append("_");
	strSnapshotName.append(eLTE_Tool::UnicodeToANSI(szTime.GetBuffer()));

	std::string strSnapshotPath = m_savePath;
	if(!CeLTE_PlayerCtrl::m_ulBypass)
	{
		size_t index = m_savePath.find_last_of("\\");
		std::string strSavePath;
		if(std::string::npos != index)
		{
			strSavePath = m_savePath.substr(0, index);
		}
		if(!eLTE_Tool::CreateMultipleDirectory(strSavePath))
		{
			LOG_RUN_ERROR("CreateMultipleDirectory %s failed.", strSavePath.c_str());
		}
	}
	strSnapshotPath.append(strSnapshotName);
	if (eLTE_SNAPSHOT_JPG == m_snapshotFormat)
	{
		strSnapshotPath.append(".jpg");
	}
	else if (eLTE_SNAPSHOT_BMP == m_snapshotFormat)
	{
		strSnapshotPath.append(".bmp");
	}
	else
	{
		INFO_PARAM1(m_snapshotFormat);
		LOG_RUN_ERROR("Invalid snapshot format.");
	}

	return strSnapshotPath;
}

// ��Ƶ�ж�
BOOL MediaPlayer::IsNetDateAudio(char* pBuf, size_t bufSize, char* &pAmr, size_t& amrSize) const
{
	if (NULL == pBuf)
	{
		LOG_RUN_ERROR("pBuf is null.");
		return FALSE;
	}

	if (sizeof(FIXED_HEADER) > bufSize)
	{
		LOG_RUN_ERROR("bufSize is too small.");
		return FALSE;
	}

	FIXED_HEADER *phdr = (FIXED_HEADER *)pBuf;//lint !e826
	if (NULL == phdr)//lint !e774
	{
		LOG_RUN_ERROR("phdr is null.");
		return FALSE;
	}

	// amr
	if (PAY_LOAD_TYPE_ELTE_AMR == phdr->payload)
	{
		// ��ȥAMR֡�е�RTPͷ��С��12B
		size_t headerLen = sizeof(FIXED_HEADER) + phdr->csrc_len*4;
		if (headerLen >= bufSize)
		{
			return FALSE;
		}
		amrSize = bufSize - headerLen;
		pAmr = pBuf + headerLen;

		// �ټ�ȥAMR�е�Payload header��С��1B
		amrSize -= 1;
		pAmr += 1;

		return TRUE;
	}

	return FALSE;
}

IVS_VOID CALLBACK MediaPlayer::NetDateCallBack(IVS_CHAR* pBuf, IVS_UINT32 uiSize, IVS_VOID* pUser)
{
	// ��ȡ�ص������������;
	MediaPlayer* pMediaPlayer = (MediaPlayer*)pUser;
	if (NULL == pMediaPlayer)
	{
		LOG_RUN_ERROR("MediaPlayer is null.");
		return;
	}

	// ��Ƶ����
	size_t amrSize;
	char* pAmr;
	if (pMediaPlayer->IsNetDateAudio(pBuf, uiSize, pAmr, amrSize))
	{
		if (pMediaPlayer->m_bAllowPlaySound)
		{
			// ������Ƶ
			int iRet = pMediaPlayer->m_AudioPlayer.WaveOutPlay(pAmr, amrSize);
			if (eLTE_ERR_SUCCESS != iRet)
			{
				LOG_RUN_ERROR("WaveOutPlay failed.");
			}
		}
	}
	// ��Ƶ����
	else
	{
		// �򲥷ſ�����RTP��;
		IVS_INT32 iRet = IVS_PLAY_InputRtpData(pMediaPlayer->m_ulPalyerChannel, pBuf, uiSize);
		if (IVS_SUCCEED != iRet)
		{
			LOG_RUN_ERROR("IVS_PLAY_InputRtpData failed. (%d)", iRet);
		}
	}
}

IVS_VOID CALLBACK MediaPlayer::NetExceptionCallBack(IVS_ULONG ulChannel, IVS_INT32 iMsgType, IVS_VOID* /*pParam*/, IVS_VOID* /*pUser*/)
{
	// ���û������쳣�¼��ϱ�����ʱ�Ȱ��쳣д�뵽��־�С�
	LOG_RUN_WARN("Net exception: Type = %d, Channel = %d", iMsgType, ulChannel);
}
