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

/********************************************************************
filename    :    MediaPlayer.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    媒体播放类。使用IVS MP模块实现本OCX的视频播放功能
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_MEDIA_PLAYER_H__
#define __ELTE_OCX_MEDIA_PLAYER_H__

#include "AudioPlayer.h"


//////////////////////////////////////////////////////////////////////////
struct FIXED_HEADER
{
	// byte 0
	unsigned char csrc_len  : 4;	// expect 0
	unsigned char extension : 1;	// expect 0
	unsigned char padding   : 1;	// expect 0
	unsigned char version   : 2;	// expect 2
	// byte 1
	unsigned char payload	: 7;	//
	unsigned char marker	: 1;	// expect 1
	// bytes 2, 3
	unsigned short seq_no;

	// bytes 4-7
	unsigned int timestamp;

	// bytes 8-11
	unsigned int ssrc;				// stream number is used here
};

// eLTE AMR
#define PAY_LOAD_TYPE_ELTE_AMR	114
//////////////////////////////////////////////////////////////////////////

class MediaPlayer
{
public:
	MediaPlayer(void);
	~MediaPlayer(void);

public:
	// 初始化播放器
	int InitPlayerParam(IVS_CHAR* pLocalIP, IVS_UINT32 uiLocalVideoPort, IVS_UINT32 uiLocalAudioPort);
	// 去初始化播放器
	int UninitPlayer();
	// 开始播放
	int StartPlayer(IVS_CHAR* pRemoteIP, IVS_UINT32 uiRemoteVideoPort, IVS_UINT32 uiRemoteAudioPort, HWND hVideoPlayer);
	// 停止播放
	int StopPlayer();
	// 播放音频
	int PlayAudio();
	// 停止音频
	int StopAudio();
	// 本地抓拍单张图片
	int LocalSnapshot(const std::string& strResId, std::string& strSnapshotPath);
	// 初始化抓图参数
	void InitParam(int snapshotFormat, const std::string& savePath);
	// 初始化伴音音量
	void InitParam(DWORD dwVolume);

	// 用于工具栏操作
public:
	// 设置抓图参数
	void SetMediaPlayerResID(const std::string& strResId);
	// 设置播放参数（这里是工具栏切换视频操作，IP使用上次开启视频时使用的IP）
	void SetPlayerParam(IVS_UINT32 uiLocalVideoPort, IVS_UINT32 uiLocalAudioPort,
						IVS_UINT32 uiRemoteVideoPort, IVS_UINT32 uiRemoteAudioPort);
	// 初始化播放器
	int StartPlayer(HWND hVideoPlayer);
	// 本地抓拍单张图片
	int LocalSnapshot(std::string& strSnapshotPath);
	// 获取资源ID
	int GetMediaPlayerResID() const;
	// 设置m_bAllowPlaySound
	void SetAllowPlaySound(const BOOL& bAllowPlaySound) {m_bAllowPlaySound = bAllowPlaySound;};

public:
	// 媒体流回调
	// pBuf		回调数据
	// uiSize	数据长度
	// pUser	用户数据
	static IVS_VOID CALLBACK NetDateCallBack(IVS_CHAR* pBuf, IVS_UINT32 uiSize, IVS_VOID* pUser);

	// 异常回调函数
	// ulChannel 通道号
	// iMsgType 异常消息类型
	// pParam 对应异常的信息，可扩展为结构体指针
	// pUser 用户数据
	static IVS_VOID CALLBACK NetExceptionCallBack(IVS_ULONG ulChannel, IVS_INT32 iMsgType, IVS_VOID* pParam, IVS_VOID* pUser);

private:
	// 获取抓图名
	std::string GetSnapshotPath(const std::string& strResId) const;
	// 音频判断
	BOOL IsNetDateAudio(char* pBuf, size_t bufSize, char* &pAmr, size_t& amrSize) const;

private:
	// 网络收流句柄
	IVS_ULONG m_ulNetChannel;
	// 播放器句柄
	IVS_ULONG m_ulPalyerChannel;
	// 播放开始标志
	BOOL m_bStartPlayer;
	// 抓图格式
	int m_snapshotFormat;
	// 抓图保存路径
	std::string m_savePath;
	// 允许播放音频，用于关闭音频
	BOOL m_bAllowPlaySound;
	// 初始化标志
	static BOOL m_bInit;

private:
	// 音频播放类
	AudioPlayer m_AudioPlayer;

	// 用于工具栏操作
private:
	// 视频资源ID
	std::string m_strResId;
	// 本地IP
	std::string m_strLocalIP;
	// 远程IP
	std::string m_strRemoteIP;
	// 本地视频端口
	IVS_UINT32 m_uiLocalVideoPort;
	// 本地音频端口
	IVS_UINT32 m_uiLocalAudioPort;
	// 远程视频端口
	IVS_UINT32 m_uiRemoteVideoPort;
	// 远程音频端口
	IVS_UINT32 m_uiRemoteAudioPort;
};

#endif
