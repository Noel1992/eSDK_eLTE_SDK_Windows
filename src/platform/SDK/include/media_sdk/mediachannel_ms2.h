/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    mediachannel_ms2.h
Author:      Wang Kai
Created:     2012/09/12 14:50
Description:  基于MediaStream2(in Linphone)的 VoiceChannel 和 VideoChannel 实现类
              主要借助于 MediaStream2中的audiostream 和 videostream 结构及相应的函数来实现
               并通过MediaManager 接口提供的VideoRenderer 和 VideoCapturer 来和GUI层交互
*********************************************************************/

#ifndef MEDIACHANNEL_MS2_H
#define MEDIACHANNEL_MS2_H

#include "mediamanager.h"
#include "mediastreamer2/mediastream.h"
#include "media_export.h"
//#include "platform/modules/timer/ubp_timer.hpp"


#include "rec_context_itf.h"
using ubp::dc::rec::RecFileSessionInfo;

class VoiceChannelMS2;
class RecFileSessionInfo;

class MEDIA_IMPORT_EXPORT VideoChannelMS2
        //      : public VideoMediaChannel
{
public:     //抽象接口定义
    static VideoChannelMS2* new_VideoChannel(int ridLogical,int ridPhysical);
    virtual ~VideoChannelMS2();

    void SetVideoRender(VideoRenderer* render);
    void SetMediaManager(MediaManager* mm_);
	/*added by lwx201687 begin*/
    /*int BuildVideo(int local_port, const char *remote_ip,
                   int remote_port, int payload, const char *fmtp,
                   int jitter, MSWebCam* cam,VideoStreamDir dir,const char * local_ip = NULL);*/

	int BuildVideo(int local_port, const char *remote_ip,
                   int remote_port, int payload, const char *fmtp,
                   int jitter, MSWebCam* cam,VideoStreamDir dir,const char * local_ip = NULL,int continous_p_frames = 0,int min_drop_p_framenumber = 0,int min_loss_rate = 5);
	/*added by lwx201687 end*/
    VideoStream* GetVideoStream(){return video_stream_; }
    RtpProfile* GetRtpProfile(){return m_profile; }

    void TeardownVideo();
    /* lijinglai add for rtprtx b */
	void SetVideoRtpRtxParam(struct VideoStream_RtpRtx &param);
	/* lijinglai add for rtprtx e */

    static void X264_init(void);

    void emptyVideoStream();
    void emptyVideoQueue();
    bool StartRecording(VoiceChannelMS2* companyVoice, RecFileSessionInfo* info);  //如果无伴音， 则 companyVoice == NULL

    void StopRecording(void);
    void Deactive()
    {
        active_ = false;
    }

    int GetVideoLossrate();
    int GetVideoFramerate();
    float GetVideoBitrate();

    void InitAccompanyAudio(VoiceChannelMS2 *channel);    /*added by tanghailang*/
    void UnInitAccompanyAudio();                          /*added by tanghailang*/

    static void VideoChannelRtpCallback(struct _RtpSession *, ...);

    //video NAT穿越场景发送首包
    int sendVideo_probePacket(unsigned int SSRC , int send_probePacket_num,int gap_time);
    //to stop_NAT_probe ---added by wangshiwei
    void stopVideo_NAT_probe();

	//[TDTECH-jinglai.li-20151202]{
    // add for encryption
    void setEncrypTionType(int encrypType);
    void setEncrypTionKeyIndex(int encrypIndex);
    void enableEncryption();
    void disableEncryption();
    //[TDTECH-jinglai.li-20151202]{
    
private:       //基于MediaStream2的实现定义
    int ridPhysical_;             //本channel对应的捕获输入时的resourceID
    int ridLogical_;              //本channel对应的绘制输出时的resourceID

	//[TDTECH-jinglai.li-20151202]{
    // add for encryption
    int mEncryptionType;
    int mEncryptionIndex;
    int mEnableEncryption;
   //[TDTECH-jinglai.li-20151202]{

#ifndef _BYPASS_MEDIA
    bool is_recording_; //标志当前是否已经启动录音
    VoiceChannelMS2* voice_channel_; //伴音的Channel，录像时有用
    int rec_session_id_;
    //added in TTR4.0
    bool bStopNATProbe;				  //NAT	穿透是否定时发送首包标志，true停止发送,false继续发送
#endif
protected:
    VideoRenderer *renderer_;  //外部注入的render
    VideoStream* video_stream_;     //视频通道
    MediaManager* mm;
    RtpProfile * m_profile;
    //OrtpEvQueue * m_queue;   rmv m_queue mod by wugang,wangkai
    bool active_;
    VideoChannelMS2(int ridLogical,int ridPhysical);

    void ConfigVideo(VideoStream* video,VideoStreamDir dir);
    void ConfigRender(int renderID,VideoStream * video);
    void SetVideoParams();
    bool ConfigRecord(bool is_start,RtpRecvDataCallBackSession* info);
    static void VideoChannelRenderCallback(void *user_pointer, const MSPicture *local_view, const MSPicture *remote_view);
    static void VideoChannelYuvRenderCallback(void *user_pointer, const MSPicture *local_view, const MSPicture *remote_view, const void *time_stat);
	static void VideoChannelStopCallback(void *session);//added by wangshiwei

	//[TDTECH-jinglai.li-20150314]{
	static void VideoChannelFileFullCallBack(void *user_pointer,const void *filename);
	//[TDTECH-jinglai.li-20150314]}
	//[TDTECH-jinglai.li-20151229]{
	static void VideoChannelRecvFirstDataCallback(void *session);
	//[TDTECH-jinglai.li-20151229]}
	
    static void set_default_pps(VideoStream * video);

    static void VideoChannelErrorEventCallback(void *user_pointer,int errorno, const char* errorinfo);
};


class MEDIA_IMPORT_EXPORT VideoStream_Stopper
{
public:
    VideoStream_Stopper();

    void stopVideoStream(VideoChannelMS2* channel);

private:

};

/* lijinglai add for rtprtx b */
struct VideoStream_RtpRtx
{
	int rtprtx_enable;
	int sumjitter_param;
	uint32_t congest_thd;
  	uint32_t congestdelay_thd;
  	uint32_t congest_timer_thd;
  	uint32_t congestdelay_timer_thd;
  	uint32_t  rtprtx_ssrc;
	uint32_t  rtprtx_max_trans_time;
	uint32_t  rtprtx_max_check_time;
};
/* lijinglai add for rtprtx e */
#endif // MEDIACHANNEL_MS2_H
