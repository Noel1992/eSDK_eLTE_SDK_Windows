/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    mediachannel_ms2.h
Author:      Wang Kai
Created:     2012/09/12 14:50
Description:  基于MediaStream2(in Linphone)的 VoiceChannel 和 VideoChannel 实现类
              主要借助于 MediaStream2中的audiostream 和 videostream 结构及相应的函数来实现
               并通过MediaManager 接口提供的VideoRenderer 和 VideoCapturer 来和GUI层交互
*********************************************************************/

#ifndef VOICECHANNEL_MS2_H
#define VOICECHANNEL_MS2_H

#pragma warning(disable: 4251)

#include "mediastreamer2/mediastream.h"
#include <string>
#include "media_export.h"


struct RecordStream;

/*added by tanghailang*/
class MEDIA_IMPORT_EXPORT SoundCardManager
{
public:
	SoundCardManager();
	~SoundCardManager();

	void ReloadCardManager();

	MSSndCard * GetDefaultCaptureCard();
	
	//[TDTECH-jinglai.li-20151209]{
	//add "speaker_id" for select output devices
	MSSndCard * GetPlaybackCard(const char* speaker_id = NULL);
	//[TDTECH-jinglai.li-20151209]}

	/* test for start audio detect b*/
	MSSndCardManager &GetMSSndCardManager() {return card_manager;}
	/* test for start audio detect e*/

private:
	MSSndCardManager card_manager;
};



class MEDIA_IMPORT_EXPORT VoiceChannelMS2
      	  
{
 public:        //抽象接口定义
  VoiceChannelMS2(int rid_);
  virtual ~VoiceChannelMS2();

  //关闭或打开喇叭（speaker）
  void SetPlayout(bool playout);

  //关闭或打开麦克风（MIC）
  virtual bool SetSend(bool send);

  //打开或关闭录音
  void SetRecord(bool record,std::string filepath);
  
  //Audio channel buildup and teardown
  
  //[TDTECH-jinglai.li-20151208]{
  //add "speaker_id" for select output devices
  int BuildAudio(int local_port, const char *remote_ip,
                             int remote_port, int payload, const char *fmtp,
                             int jitter, bool ec, int bitrate, const char *local_ip = NULL,const char* speaker_id = NULL);
  //[TDTECH-jinglai.li-20151208]{
  void TeardownAudio();

  //[TDTECH-jinglai.li-20151208]{
  //add "speaker_id" for select output devices
  int BuildAudioRecording(int local_port, const char *remote_ip,
                             int remote_port, int payload, const char *fmtp,
                             int jitter, bool ec, int bitrate,std::string filepath, const char *local_ip = NULL,const char* speaker_id = NULL);
  
  
  //[TDTECH-jinglai.li-20151208]}
  //local ring start and stop
  virtual void StartRing(bool bIncomingCall);
  virtual void StopRing();

  //麦克风并发控制
  void AttachMIC();
  void DetachMIC();

  AudioStream* GetAudioStream(){return audio_stream_;}
  void SetPlaybackGainDB(float gaindb);
  float GetPlaybackGainDB();

  bool ConfigRecord(bool is_start,RtpRecvDataCallBackSession* info);

  //void fill_stats(DcCallLog *log);

  // added in TTR4.0
  //to stop_NAT_probe ---added by wangshiwei
  void stopAudio_NAT_probe();
  //audio NAT穿越场景发送首包
  int sendAudio_probePacket(unsigned int SSRC,int send_probepacket_num,int gap_time);

  //[TDTECH-jinglai.li-20151202]{
  // add for encryption
  void setEncrypTionType(int encrypType);
  void setEncrypTionKeyIndex(int encrypIndex);
  void enableEncryption();
  void disableEncryption();
  //[TDTECH-jinglai.li-20151202]{
  
 private:       //基于MediaStream2的实现定义

  int rid;  //resource ID
  
  float gain_;      //当前设定的音量

  AudioStream *audio_stream_;
  RingStream* ring_stream_;

  SoundCardManager *manager;    /*added by tanghailang*/

  //媒体面参数，在媒体面修改时需要引用
  int pt_;
    
  //[TDTECH-jinglai.li-20151202]{
  // add for encryption
  int mEncryptionType;
  int mEncryptionIndex;
  int mEnableEncryption;
  //[TDTECH-jinglai.li-20151202]{

  /* test for start audio detect b*/
#ifndef _BYPASS_MEDIA
  ms_thread_t m_DetectAudioOutThreadId;
	ms_thread_t m_DetectAudioInThreadId;
#endif

  bool ec_;
  bool playing_;    //是否播放， muteSpeaker
  bool sending_;    //是否采集， muteMic
  bool recording_;  //是否在录音
  
#ifndef _BYPASS_MEDIA
   std::string mSpeakerId;
#endif
  /* test for start audio detect e*/

  /* test for start audio detect b*/
#ifndef _BYPASS_MEDIA
  bool mDetectAORuning;
  bool mDetectAIRuning;
#endif
  /* test for start audio detect e*/
  
  //added in TTR4.0
  bool bStopNATProbe;				  //NAT	穿透是否定时发送首包标志，true停止发送,false继续发送


public:
  int UpdateAudio_StartRecord(std::string filepath);
  int UpdateAudio_StopRecord();
  int Send_DTMF(char dtmf);
  void UserConfigAudio(bool send_,bool play_);

  /* test for start audio detect b*/
#ifndef _BYPASS_MEDIA
  SoundCardManager *GetSoundCardManger(){return manager; }
  bool GetDetectAORuningState(){return mDetectAORuning;}
  void SetDetectAORuningState(bool state){mDetectAORuning = state;}
  static void *DetectAudioOutFunc(void* instance);

  bool GetDetectAIRuningState(){return mDetectAIRuning;}
  void SetDetectAIRuningState(bool state){mDetectAIRuning = state;}
  static void *DetectAudioInFunc(void* instance);
  
#endif
  /* test for start audio detect e*/

protected:
	static void AudioChannelRtpCallback(struct _RtpSession *session, ...);
	static void MSFilterNotifyFunc_SoundReader(void *userdata , unsigned int id, void *arg);
	//added from TTR4.0
	static void AudioChannelStopNATProbeCallBack(void *session);
};
#endif // VOICECHANNEL_MS2_H
