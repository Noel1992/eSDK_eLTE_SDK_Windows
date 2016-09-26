/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    mediachannel_ms2.h
Author:      Wang Kai
Created:     2012/09/12 14:50
Description:  基于MediaStream2(in Linphone)的 VoiceChannel 和 VideoChannel 实现类
              主要借助于 MediaStream2中的audiostream 和 videostream 结构及相应的函数来实现
               并通过MediaManager 接口提供的VideoRenderer 和 VideoCapturer 来和GUI层交互
*********************************************************************/

#ifndef RINGING_DC_H
#define RINGING_DC_H

#include <string>
#include "media_sdk/media_export.h"
//#include "media_sdk/ring_stream_itf.h"
#include "platform/modules/timer/ubp_timer.hpp"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "sdk_export.h"

typedef enum {      //添加新的码时请严格按照错误码排序
    START_FROM_ZERO = 0,
    FAIL_CODE_403,
    FAIL_CODE_404,
    FAIL_CODE_451,
    FAIL_CODE_480,
    FAIL_CODE_486,
    FAIL_CODE_488,
    FAIL_CODE_183, //183回铃音
    FAIL_CODE_ML_WAIT //多路点呼等待
}Sip_Fail_Code;

enum
{
    MEDIA_RES_START = 1000,
    RES_RING_FILE,
    RES_RINGBACK_FILE,
    RES_BEEPSHORT_FILE,
    RES_SDS_RING_FILE,
    RES_ALARM_FILE
};
class SDK_IMPORT_EXPORT RingStreamWrapper
{
    /*
      振铃与回铃
      */
public:
  //  static RingStreamMS2* ringing();        //做被叫时振铃
    static int  ringing(std::string ringing_file);        //做被叫时振铃
    static int  ringing();        //做被叫时振铃
  //  static RingStreamMS2* ringback();       //做主叫时回铃
    static int  ringback();       //做主叫时回铃
    static int ringFailTone(int failCode); //做主叫时播放失败提示音

    static void  ringstop(int rsid);
    static int  beepshort();
    static bool isMicAttached();

    static int  ringingSDS(); //SDS sucess ring
    static void setSmstipSwitchValue(int switchValue);   //设置sms短信提示音打开
    static bool isSmsSwitchOpen();        //检查sms短信提示音是否打开
private:
    static bool btSmstoneOpen;

};

class SDK_IMPORT_EXPORT NewMediaAlarmPlayer
{
public:
        static int  AlarmPlay();
        static void AlarmStop(int rsid);
};

//FailTone_Ringer负责播放和结束呼叫失败的提示音
//一般用于点呼
class SDK_IMPORT_EXPORT FailTone_Ringer : public ubp::platform::timer::UbpTimerHandler
{
    friend class ACE_Singleton<FailTone_Ringer, ACE_Null_Mutex>;
public:
    ACE_Recursive_Thread_Mutex* getLock(){return lock;}

    static void ringFailTone(MSISDN resid,int failCode); //做主叫时播放失败提示音
    static int ringFailTone_4_Recv183(MSISDN resid,int failCode); //收到183振铃
    static void stopFailTone(MSISDN resid);           //停止该资源上的失败提示音
    int handle_timeout(const ACE_Time_Value &current_time,const void *act = 0);
    static void stopCurrentFailTone();

private:
    MSISDN currentResID;            //当前正在播放失败提示音的资源ID， 任何时候只有一个失败提示音
    int ringID;
  //  RingStreamMS2* r_;              //当前的RingStream
//    long timerID;
    FailTone_Ringer();
    ACE_Recursive_Thread_Mutex* lock;
};

typedef ACE_Singleton<FailTone_Ringer, ACE_Null_Mutex> FailTone_Ringer_T;

#endif // VOICECHANNEL_MS2_H
