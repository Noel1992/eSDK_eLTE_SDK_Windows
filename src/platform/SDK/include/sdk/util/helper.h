/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    helper.h
Author:      Wu yu wei
Created:     2012/07/08 10:00
Description: 帮助类
*********************************************************************/
#ifndef VALIDATIONHELPER_H
#define VALIDATIONHELPER_H


#include "osipparser2/osip_message.h"
#include "operationmanager.h"
#include "uiparameter.h"
#include "sdk_export.h"
#include <map>

#if !defined(WIN32) && !defined(_WIN32)
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
#endif

namespace ubp
{
namespace platform
{
namespace timer
{
class TimerDispatcher;
}
}
}
using namespace ubp::platform::timer;
class KeepAliveTimeoutHandler;
struct eXosip_t;

typedef enum
{
    ENCRYPTION_GETKEY_FROMCENTER = 0,   //有中心加密
    ENCRYPTION_GETKEY_FROMLOCAL = 1,    //无中心加密
    NO_ENCRYPTION_CARD                  //无加密卡
}ENCRYPTION_KEYFROM_t;

#define CHECK_IFNULL_RETURN(A, B) \
    if((A) == NULL) return B;


typedef std::map<MSISDN, DC_LIST> TasklistMap;
typedef std::map<MSISDN, DC_LIST>::iterator TaskListMapIterator;

/**
    辅助校验输入
 **/

class SDK_IMPORT_EXPORT Helper
{
public:
    Helper();

    //print sip message
    static void printSipMessage(osip_message_t * sip, const char * type);


    // Sip初始化, Media 协议栈初始化
    static int protocol_init();
    static int protocol_fini();
    // Change Sip UA
    static void change_sip_ua(const char * sipua);

    /*
      Sip注册/去注册
      expires：过期时间
    */
    static int real_send_register(eXosip_t* ExContext,const char *username, const char *serverip,
                                  const char *sipport,int expires);
    static int real_send_register(int expires);

    static void clean_env();
    static void clean_invalid_reg();
    static long start_register_timer();
    static int find_reg_by_username(char *username);//username "sip:UserName@ServerIp:SipPort"
    static void remove_reg_by_username(eXosip_t * excontext,char *username);//username "sip:UserName@ServerIp:SipPort"
    /* 校验调度员权限 */
    static ACE_INT32 checkDcPriority(MSISDN, int);
    /* 启动Sip心跳检测Task */
    static KeepAliveTimeoutHandler* start_keepalive_timer();

    //异步调用invoke Operation
    static ACE_INT32 scheduleAsynchOperation(ResourceInfo *res_info, Operation_t op_type, void* para=0);
    //同步调用invoke Operation
    static ACE_INT32 execsynchOperation(ResourceInfo *res_info, Operation_t op_type, void* para=0);
    static void read_Dcconf(std::string filename);

    /* 设置音频jitter补偿值 */
    static void setAudioJittComp(int jitter);
    static int getAudioJittComp();

    /* 设置视频jitter补偿值 */
    static void setVideoJittComp(int jitter);
    static int getVideoJittComp();

    /*added by lwx201687 begin */
    static void setVideocontinuDropPFrame(int contin_p_frame);
    static int getVideocontinuDropPFrame();
    static void setVideoMiniDropPFrameNum(int min_drop_frame_num);
    static int getVideoMiniDropPFrameNum();
    /*added by lwx201687 end */

    /* added by lwx201687 20160523 begin */
    static void setVideoMiniLoassRate(int min_loss_rate);
    static int getVideoMiniLoassRate();
    /* added by lwx201687 20160523 end */

    /* lijinglai add for rtprtx b */
    static void setVideoRtpRtxMaxTransTime(int max_trans_time);
    static int getVideoRtpRtxMaxTransTime();
    static void setVideoRtpRtxCongestThd(int congest_thd);
    static int getVideoRtpRtxCongestThd();
    static void setVideoRtpRtxCongestTimerThd(int congest_timer_thd);
    static int getVideoRtpRtxCongestTimerThd();
    static void setVideoRtpRtxCongestDelayThd(int congest_delay_thd);
    static int getVideoRtpRtxCongestDelayThd();
    static void setVideoRtpRtxCongestDelayTimerThd(int congest_delay_timer_thd);
    static int getVideoRtpRtxCongestDelayTimerThd();
    static void setVideoRtpRtxSumJitterParam(int sumjitter_param);
    static int getVideoRtpRtxSumJitterParam();
    static void setVideoRtpRtxMaxCheckTime(int max_check_time);
    static int getVideoRtpRtxMaxCheckTime();
    /* lijinglai add for rtprtx e */

    /* 设置串口值 */
    static void setSCom(std::string com);
    static std::string getSCom();

    /* 设置max_audio_channel */
    static void setMaxAudioChannel(int max);
    static int getMaxAudioChannel();

    /* 设置max_video_num */
    static void setMaxVideoNum(int max);
    static int getMaxVideoNum();


    /* Osip超时时间 */
    static int GetOsipOutTimes() ;
    static void SetOsipOutTimes(int val) ;

    static bool getVideoStatistics(MSISDN resid, VideoStatistics_t& struVideoStaInfo, bool bBitRateFlag);

    static ubp::platform::timer::TimerDispatcher* getTimerDispatcher();

    static int getTaskId(MSISDN physicalResid);

    static void setResource(std::string resource);

    /* 设置获取通用字符集 */
    static std::string getUniChrSet();
    static void setUniChrSet(std::string ChrSet);

    /* 设置获取本地字符集 */
    static std::string getLocalChrSet();
    static void setLocalChrSet(std::string ChrSet);
    //DTS2014011604149,提供当前监听对象的查询接口
    /*******************************add by lwj****************************************/
    /*
      振铃与回铃
      */
    static int ringing(std::string pathfile);
    static int ringback();
    static void ringstop(int ringID);
    static int beepshort();
    static bool isMicAttached();

    static int  AlarmPlay();
    static void AlarmStop(int rsid);

    /*********************************************************************************/
    static int getCUrrentListener(bool isPTTtype);

    //TIC related utilities
    static bool isTicResource(MSISDN resid);
    static bool isTicNumber(const char* number);
    static MSISDN getOutboundTicResource();
    static MSISDN getGWResource();

    static bool isTDTechConsole();

    static int getTDTechConsoleVersion();
    static void setTDTechConsoleVersion(int version);

    /* 加密字符串 */
    static bool encryptContent(const char * src, char * encOut);
    /* 解密字符串 */
    static bool decryptContent(const char * src, char * decOut);

    /* 加密字符串 */
    static bool encryptByDPAPI(BYTE * src, const DWORD src_size, BYTE * decOut, DWORD *out_size);
    static void encryptContentByDPAPI(char *src, char *out_put);
    /* 解密字符串 */
    static bool decryptByDPAPI(BYTE * src, const DWORD src_size, BYTE * decOut, DWORD *out_size);
    static void decryptContentByDPAPI(char *src, char *out_put);
    static bool decryptStringByDPAPI(const char * src,char * decOut);
    /* 字符转换 */
    static char StrToHEX(char c);
    static void HexValueToString(unsigned char *pucInput, int udwLen, char *pszDststring);
    static void HexString2Value(char *pcSrcStr, unsigned char *pucTargetValu, unsigned int ucStrLen);

    static void MyHandleError(char *s);

    /* 获取当前Work Key */
    static std::string GetCurrentWKey();
    /* 设置加密workKey */
    static void setEncriptedCurrentKey(const char * enckey);

    /* 模糊化字串的敏感信息, 适配安全红线整改需求  */
    static std::string blurStr(const std::string & src);


    /* 设置set_sds_mms */
    static void setsdsmms(int sdsmmstime);
    static int getsdsmms();

    /*设置sms_max_num*/
    static void setMaxSMSNum(int sms_max_num);
    static int getMaxSMSNum();

    /*设置mms_max_num*/
    static void setMaxMMSNum(int sms_max_num);
    static int getMaxMMSNum();

    static std::string getAppDataPath();
    static void setAppDataPath(std::string path);

    static std::string getTempPath();
    static void setTempPath(std::string path);
    static void add_callinfo_grouprelease(osip_message_t * message,int uri_id);
    static void closeGroup(MSISDN speaker,MSISDN group);
    static void queryUBPNode4NAT();
    static unsigned int str2unint(const std::string src);
    static std::string number2str(const int number);
    //向某个ip发送简单的UDP包
    static int sendudp(char *desip,int port,char *message);
    //监听某个协商好的端口来获取需要的数据
    static int recvudp(int port , std::string &recvmessage);
    //保存被缜密侦听的目标号码
    static void setDiscreet_listenee(const int discreet_listenee);
    //获取当前被缜密侦听的目标号码
    static int getDiscreet_listenee();
    //设置当前发送nat首包的次数
    static void setProbePacketNum(int num);
    static int getProbePacketNum();
    //设置当前发送rtsp 首包的次数
    static void setRtspProbePacketNum(int num);
    static int getRtspProbePacketNum();
    //设置发送nat的间隔
    static void setProbePacket_gaptime(int gap_time);
    static int getProbePacket_gaptime();
    //设置发送rtsp首包的时间间隔
    static void setRtspProbePacket_gap_time(int rtsp_gap_time);
    static int getRtspProbePacket_gaptime();
    //DTS2016052504400, DTS2016052504149
    static void setLoadFileTimeout(int filetimeout);
    static int getLoadFileTimeout();

    //调度台加密情况下业务发起时需要在sip消息中加特定的字段，详见TTR4.0_CRN04533 2.3.5.2.2章节
    static std::string make_multiparty_4_encrypt_invite(std::string sdp, std::string cmmsg,bool encryptflag);
    //设置组呼时是否是加密,每次业务操作时都需要进行设置
    static void setGroupEncryptFlag(bool encryption_card);
    static bool getGroupEncryptFlag();
    //设置点呼时是否是加密,每次业务操作时都需要进行设置
    static void setDialEncryptFlag(bool encryption_card);
    static bool getDialEncryptFlag();
    //设置视频监控时是否是加密,每次业务操作时都需要进行设置
    static void setCameraEncryptFlag(bool encryption_card);
    static bool getCameraEncryptFlag();
    //在调度台登陆一开始需要设置 有无中心加密
    static void setEncryptionType(ENCRYPTION_KEYFROM_t keyfrom);
    static ENCRYPTION_KEYFROM_t getEncrypType();
    static int SendSipInfoMsg(MSISDN callid, char *name1,char* value1,char *name2,char* value2);

    static void setLogonType(LogOn_Proxy_t type);
    static LogOn_Proxy_t getLogonType();
    //截取mark1到mark2之间的字符串
    static void splitStr(std::string& srcStr, char* mark1, char* mark2);

    //设置视频丢包重传参数
    static void setVideoLossRetransInfo(int status , int rtxtime);
    static void getVideoLossRetransInfo(int* status , int* rtxtime);

    static void GenerateRandNumber(std::string& ori_ssrc, std::string& rtx_ssrc);

    static TasklistMap& listMap();

private:
    /* 从str中摘除"phrase":"aaaa"，以...替代*/
    static void extractStr(std::string & str, const char * phrase);

    static int bytes_size(const char * bytes);
};

#endif // VALIDATIONHELPER_H
