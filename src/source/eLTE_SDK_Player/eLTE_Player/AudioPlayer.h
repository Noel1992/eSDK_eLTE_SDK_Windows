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

#ifndef __ELTE_OCX_AUDIO_PLAYER_H__
#define __ELTE_OCX_AUDIO_PLAYER_H__

// PCM播放
#include "OpencoreAmrnb.h"
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")


//////////////////////////////////////////////////////////////////////////
#define AUDIO_FRAME_DATA_SZ  512         //音频帧尺寸, FIX ME!!!!
#define WAVE_BUFF_SZ    (sizeof(WAVEHDR) + AUDIO_FRAME_DATA_SZ)
#define WAVE_RING_SZ    1024

typedef struct
{
	BOOL     bInUse;
	WAVEHDR  Hdr;
	char     WaveData[AUDIO_FRAME_DATA_SZ];
} WAVE_OUT_BUF;

//////////////////////////////////////////////////////////////////////////

class AudioPlayer
{
public:
	AudioPlayer(void);
	~AudioPlayer(void);

public:
	// 初始化播放
	int WaveOutInit();
	// 退出播放
	int WaveOutExit();
	// 播放PCM
	int WaveOutPlay(char* pAmr, unsigned int len);
	// 设置播放音量
	int WaveOutSetVolume(DWORD dwVolume);
	// 获取播放音量
	int WaveOutGetVolume(DWORD& dwVolume);
	// 初始化音量
	int WaveOutInitVolume(DWORD dwVolume);

private:
	// waveOut回调函数
	static void CALLBACK WaveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

private:
	// 波形音频输出句柄
	HWAVEOUT m_hWaveOut;
	// 波形音频的格式
	WAVEFORMATEX m_waveformat;

	// 播放缓存
	WAVE_OUT_BUF m_WaveOutBuf[WAVE_RING_SZ];
	unsigned int m_uIdxToWrite;
	unsigned int m_uIdxToRead;

	// 音量
	DWORD m_dwVolume;

	// 系统原始音量
	DWORD m_orgVolume;

private:
	// 解码类
	OpencoreAmrnb m_OpencoreAmrnb;
};

#endif
