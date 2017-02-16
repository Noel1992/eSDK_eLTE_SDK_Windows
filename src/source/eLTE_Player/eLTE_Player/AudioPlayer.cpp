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
#include "AudioPlayer.h"
#include "eLTE_Log.h"


AudioPlayer::AudioPlayer(void)
	: m_hWaveOut(NULL)
	, m_uIdxToWrite(0)
	, m_uIdxToRead(0)
	, m_dwVolume(0)
	, m_orgVolume(0)
{
	eSDK_MEMSET(&m_waveformat, 0, sizeof(m_waveformat));
	eSDK_MEMSET(m_WaveOutBuf, 0, sizeof(WAVE_OUT_BUF)*WAVE_RING_SZ);
}


AudioPlayer::~AudioPlayer(void)
{
	if (NULL != m_hWaveOut)
	{
		(void)waveOutClose(m_hWaveOut);
		m_hWaveOut = NULL;
	}
	m_orgVolume = 0;
}


// 初始化播放
int AudioPlayer::WaveOutInit()
{
	LOG_TRACE();

	// 初始化opencore-amr音频转码
	int iRet = m_OpencoreAmrnb.Init();
	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("OpencoreAmrnb init failed.");
		return iRet;
	}

	if (NULL == m_hWaveOut)
	{
		// 设置音频参数
		eSDK_MEMSET(&m_waveformat, 0, sizeof(m_waveformat));
		m_waveformat.wFormatTag = WAVE_FORMAT_PCM;	// 音频格式
		m_waveformat.nChannels = 1;					// 1单声道，2立体声
		m_waveformat.nSamplesPerSec = 8000;			// 样本频率8.0 KHz
		m_waveformat.nAvgBytesPerSec = 16000;		// 平均数据传输率,PCM nAvgBytesPerSec = nSamplesPerSec
		m_waveformat.nBlockAlign = 2;				// 以字节为单位设置块对齐, (nChannels*wBitsPerSample)/8
		m_waveformat.wBitsPerSample = 16;			// 样本的位深
		m_waveformat.cbSize = 0;					// 额外信息的大小, PCM忽略此值

		// 打开音频设备
		MMRESULT mRet = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_waveformat, (DWORD_PTR)WaveOutProc, (DWORD_PTR)this, CALLBACK_FUNCTION);//lint !e620
		if (MMSYSERR_NOERROR != mRet)  
		{
			LOG_RUN_ERROR("waveOutOpen failed. (%d)", mRet);
			return eLTE_ERR_AUDIO_OPEN;  
		}

		iRet = (int)waveOutGetVolume(m_hWaveOut, &m_orgVolume);
		if (eLTE_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("waveOutGetVolume failed.");
			return iRet;
		}
		// 设置音量
		iRet = WaveOutSetVolume(m_dwVolume);
		if (eLTE_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("WaveOutSetVolume failed.");
			return iRet;
		}
	}

	return eLTE_ERR_SUCCESS;
}

// 退出播放
int AudioPlayer::WaveOutExit()
{
	LOG_TRACE();

	// 关闭音频设备
	if (NULL != m_hWaveOut)
	{
		MMRESULT mRet = waveOutReset(m_hWaveOut);
		if (MMSYSERR_NOERROR != mRet)  
		{
			LOG_RUN_ERROR("waveOutReset failed. (%d)", mRet);
			return eLTE_ERR_AUDIO_RESET;  
		}

		// 等待播放结束
		while (m_uIdxToRead != m_uIdxToWrite)
		{
			mRet = waveOutUnprepareHeader(m_hWaveOut, &m_WaveOutBuf[m_uIdxToRead].Hdr, sizeof(WAVEHDR));
			if (MMSYSERR_NOERROR != mRet)  
			{
				LOG_RUN_ERROR("waveOutUnprepareHeader failed. (%d)", mRet);
				return eLTE_ERR_AUDIO_UNPREPARE;  
			}

			m_WaveOutBuf[m_uIdxToRead].bInUse = FALSE;
			m_uIdxToRead++;
			m_uIdxToRead = m_uIdxToRead % WAVE_RING_SZ;
		}

		//设回原始系统音量
		if(0 == m_orgVolume)
		{
			m_orgVolume = 100;
			if(!WaveOutGetVolume(m_orgVolume))
			{
				return eLTE_ERR_FAILED;
			}
		}

		mRet = waveOutSetVolume(m_hWaveOut, m_orgVolume);
		if (eLTE_ERR_SUCCESS != mRet)
		{
			LOG_RUN_ERROR("waveOutSetVolume failed.");
			return (int)mRet;
		}

		mRet = waveOutClose(m_hWaveOut);
		if (MMSYSERR_NOERROR != mRet)  
		{
			LOG_RUN_ERROR("waveOutClose failed. (%d)", mRet);
			return eLTE_ERR_AUDIO_CLOSE;
		}
		m_hWaveOut = NULL;
	}

	// 退出opencore-amr音频转码
	int iRet = m_OpencoreAmrnb.Exit();
	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("OpencoreAmrnb Exit failed. (%d)", iRet);
	}

	return iRet;
}

// 播放PCM
int AudioPlayer::WaveOutPlay(char* pAmr, unsigned int len)
{
	// 入参判断
	if (NULL == pAmr)
	{
		LOG_RUN_ERROR("pAmr is null.");
		return eLTE_ERR_INVALID_PARAM;
	}
	if (AUDIO_FRAME_DATA_SZ < len)
	{
		LOG_RUN_ERROR("len is overlong.");
		return eLTE_ERR_INVALID_PARAM;
	}

	// 状态校验
	if (NULL == m_hWaveOut)
	{
		LOG_RUN_ERROR("Please WaveOutInit first.");
		return eLTE_ERR_AUDIO_OPEN;
	}

	// 缓存满了
	if (m_WaveOutBuf[m_uIdxToWrite].bInUse)
	{
		LOG_RUN_ERROR("RingBuffer is full.");
		return eLTE_ERR_AUDIO_OUT_BUFFER;
	}

	// AMR 转换为 WAV
	char* pWav = (char*)m_OpencoreAmrnb.DecodeAmrToWav((unsigned char*)pAmr);
	if (NULL == pWav)
	{
		LOG_RUN_ERROR("DecodeAmrToWav failed.");
		return eLTE_ERR_AUDIO_DECODE;
	}

	// 播放
	WAVE_OUT_BUF* pWaveOutBuf = &m_WaveOutBuf[m_uIdxToWrite];
	if (pWaveOutBuf->Hdr.dwFlags & WHDR_PREPARED)
	{
		(void)waveOutUnprepareHeader(m_hWaveOut, &pWaveOutBuf->Hdr, sizeof(WAVEHDR));
	}

	pWaveOutBuf->Hdr.lpData = pWaveOutBuf->WaveData;
	pWaveOutBuf->Hdr.dwBufferLength = WAV_BUFFER_SIZE;
	pWaveOutBuf->Hdr.dwBytesRecorded = 0;
	pWaveOutBuf->Hdr.dwUser =  0;
	pWaveOutBuf->Hdr.dwFlags = 0; 
	pWaveOutBuf->Hdr.dwLoops = 0; 
	pWaveOutBuf->Hdr.lpNext = NULL;
	pWaveOutBuf->Hdr.reserved = 0;
	eSDK_MEMCPY(pWaveOutBuf->Hdr.lpData, WAV_BUFFER_SIZE, pWav, WAV_BUFFER_SIZE);

	int iRet = eLTE_ERR_SUCCESS;
	MMRESULT mRet = waveOutPrepareHeader(m_hWaveOut, &pWaveOutBuf->Hdr, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != mRet)
	{
		LOG_RUN_ERROR("waveOutPrepareHeader failed. (%d)", mRet);
		iRet = eLTE_ERR_AUDIO_PREPARE;
	}

	pWaveOutBuf->bInUse = TRUE;
	mRet = waveOutWrite(m_hWaveOut, &pWaveOutBuf->Hdr, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != mRet)
	{              
		LOG_RUN_ERROR("waveOutWrite failed. (%d)", mRet);
		iRet = eLTE_ERR_AUDIO_WRITE;
	}

	m_uIdxToWrite++;
	m_uIdxToWrite = m_uIdxToWrite % WAVE_RING_SZ;

	return iRet;
}

// 设置播放音量
int AudioPlayer::WaveOutSetVolume(DWORD dwVolume)
{
	LOG_TRACE();
	INFO_PARAM1(dwVolume);
	if (NULL == m_hWaveOut)
	{
		LOG_RUN_ERROR("Please WaveOutInit first.");
		return eLTE_ERR_AUDIO_OPEN;
	}

	dwVolume = (ULONG)(((float)0xFFFF / 100) * (float)dwVolume);
	dwVolume = (dwVolume << 16) + dwVolume;

	MMRESULT mRet = waveOutSetVolume(m_hWaveOut, dwVolume);
	if (MMSYSERR_NOERROR != mRet)
	{              
		LOG_RUN_ERROR("waveOutSetVolume failed. (%d)", mRet);
		return eLTE_ERR_AUDIO_SET_VOLUME;
	}

	m_dwVolume = dwVolume;
	return eLTE_ERR_SUCCESS;
}

// 获取播放音量
int AudioPlayer::WaveOutGetVolume(DWORD& dwVolume)
{
	LOG_TRACE();

	if (NULL == m_hWaveOut)
	{
		LOG_RUN_ERROR("Please WaveOutInit first.");
		return eLTE_ERR_AUDIO_OPEN;
	}

	MMRESULT mRet = waveOutGetVolume(m_hWaveOut, &dwVolume);
	if (MMSYSERR_NOERROR != mRet)
	{              
		LOG_RUN_ERROR("waveOutGetVolume failed. (%d)", mRet);
		return eLTE_ERR_AUDIO_GET_VOLUME;
	}

	dwVolume = (dwVolume >> 16) * 100 / 0xFFFF;

	INFO_PARAM1(dwVolume);
	m_dwVolume = dwVolume;
	return eLTE_ERR_SUCCESS;
}

// 初始化音量
int AudioPlayer::WaveOutInitVolume(DWORD dwVolume)
{
	LOG_TRACE();
	INFO_PARAM1(dwVolume);

	m_dwVolume = dwVolume;

	return eLTE_ERR_SUCCESS;
}

// waveOut回调函数
void CALLBACK AudioPlayer::WaveOutProc(HWAVEOUT /*hwo*/, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR /*dwParam1*/, DWORD_PTR /*dwParam2*/)
{
	if (WOM_DONE != uMsg)
	{
		return;
	}

	AudioPlayer* pAudioPlayer = (AudioPlayer*)dwInstance;
	if (NULL == pAudioPlayer)
	{
		LOG_RUN_ERROR("pAudioPlayer is null.");
		return;
	}

	WAVE_OUT_BUF* pWaveOutBuf = &pAudioPlayer->m_WaveOutBuf[pAudioPlayer->m_uIdxToRead];
	if (!(pWaveOutBuf->Hdr.dwFlags & WHDR_DONE))
	{
		LOG_RUN_ERROR("WAVEHDR.dwFlags is not WHDR_DONE.");
	}

	pWaveOutBuf->bInUse = FALSE;
	pAudioPlayer->m_uIdxToRead++;
	pAudioPlayer->m_uIdxToRead = pAudioPlayer->m_uIdxToRead % WAVE_RING_SZ;
}
