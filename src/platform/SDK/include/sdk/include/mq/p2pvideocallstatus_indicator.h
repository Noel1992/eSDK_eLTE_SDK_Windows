#ifndef P2PVIDEOCALL_STATUSINDICATOR_H
#define P2PVIDEOCALL_STATUSINDICATOR_H

#include "sdk_export.h"
#include "common.h"
#include <string>

namespace Json{
 class Value;
}


/**
   \brief define event types notified for video service
**/
typedef enum {
    P2Pvideocall_IND_STATUS_UNKNOWN = 3000,

    P2Pvideocall_IND_STATUS_INITIATED = 3001,               /** reserved, */
    P2Pvideocall_IND_STATUS_RECEIVED = 3002,                /** a video call request received from remote end*/
    P2Pvideocall_IND_STATUS_ANSWERED = 3003,                /** a video call request is answered by remote end*/
    P2Pvideocall_IND_STATUS_PROCEEDING = 3004,              /** 100trying received */
    P2Pvideocall_IND_STATUS_RINGING = 3005,                 /** 180ringing received */
    P2Pvideocall_IND_STATUS_ACKED = 3006,                   /** a video call answer is acked by remote end*/
    P2Pvideocall_IND_STATUS_RELEASED = 3007,                /** reserved, used internal for 5s-later final release*/
    P2Pvideocall_IND_STATUS_HANGUPED = 3008,                /** the video call is hanguped from remote end*/
    P2Pvideocall_IND_STATUS_HANGUPED_ACTIVE = 3009,         /** the video call is being hanguped from DC*/
    P2Pvideocall_IND_STATUS_CANCELLED = 3010,               /** a video request is cancelled*/
    P2Pvideocall_IND_STATUS_MONITOR = 3011,                 /** a video surveillance request is received from remote end*/
    P2Pvideocall_IND_STATUS_PREMPTED = 3012,                /** reserved, */
    P2Pvideocall_IND_STATUS_RELEASED_BUSY = 3013,           /** remote end is busy*/
    P2Pvideocall_IND_STATUS_PROHIBITED = 3014,              /** no permissions*/
    P2Pvideocall_IND_STATUS_REMOTE_NOANSWER = 3015,         /** no answer from remote end */
    P2Pvideocall_IND_STATUS_REMOTE_NOTFOUND = 3016,         /** not found the remote resource */
    P2Pvideocall_IND_STATUS_LICENSE_LIMIT = 3017,           /** out of license */
    P2Pvideocall_IND_STATUS_EMERGENCY = 3018,               /** a emergency video-call is received */

    P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_ERROR = 3019,      /** video stream setup failed*/
    P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_NOAUDIO = 3020,    /** audio subchannel setup failed*/
    P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED = 3021,         /** remote end not registered*/
    P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED = 3022,         /** remote end doesn't support the video format in the request*/
    P2Pvideocall_IND_STATUS_SIGNAL_ERROR = 3023,                /** SIP signal error, used in SDK_GW mode*/
    P2Pvideocall_IND_STATUS_VIDEO_CONTROL = 3024,               /** for SDK_GW, or VGW */
    P2Pvideocall_IND_STATUS_PICKEDUP = 3040,                     /** the video request is answered by SDK_GW*/
    P2Pvideocall_IND_STATUS_DISPATCH = 3041,                     /** the video is dispatched to this SDK_GW*/
    P2Pvideocall_IND_STATUS_BEARER_RELEASED = 3042,              /** video monitor bearer released*/
    P2Pvideodisp_IND_STATUS_BEARER_RELEASED = 3043,               /** video dispatch bearer released*/
    P2Pvideodisp_IND_STATUS_CALLER_UNSUPPORT = 3044,              /** caller unsupport*/
    P2Pvideodisp_IND_STATUS_CALLEE_UNSUPPORT = 3045,              /** callee unsupport*/
    P2Pvideodisp_IND_STATUS_CIPHER_NOT_AVAILABLE = 3046,          /** cipher not available*/
    P2Pvideodisp_IND_STATUS_MISCELLANEOUS_WARNING = 3047,          /** Miscellaneous warning*/
    P2Pvideocall_IND_STATUS_REMOTE_PROHIBITED = 3048,               /** remote no permissions*/
    P2Pvideocall_IND_STATUS_DECFAILED = 3049                        /** start decode failed*/

} P2pvideocall_Status_t;


/**
   \brief define indicator object details notified for video service
**/
class SDK_IMPORT_EXPORT P2pvideocallStatusIndicator
{
public:
    P2pvideocallStatusIndicator(MSISDN caller, MSISDN callee);
    P2pvideocallStatusIndicator(P2pvideocallStatusIndicator& copy);
    virtual ~P2pvideocallStatusIndicator();
    std::string datalog_str();

    void setStatus(P2pvideocall_Status_t status);
    void setPeers(MSISDN caller, MSISDN callee);
    void setPorts(int local_audio_port,int local_video_port, int remote_audio_port, int remote_video_port);
    void setRemoteIp(unsigned long Remote_ip);
    void setSoundMute(int muteflag);
    void setChannel(int ChannelId);
    void setUri(MSISDN UriId);
    void setUserCfm(int cfmid);
    void setCamera(int Cameraid);
    void setSoundPtype(Sound_t Ptype); 
    void setFmt(Fmt_type FmtVlaue);

    MSISDN getCaller();
    MSISDN getCallee();
    int getlocalaudioport();
    int getlocalvideoport();
    int getremoteaudioport();
    int getremotevideoport();
    unsigned long getRemoteIp();
    int getSoundMute();
    int getChannel();
    MSISDN getUri();
    int getUserCfm();
    int getCamera();
    Sound_t getSoundPtype();
    Fmt_type getFmt();
    P2pvideocall_Status_t getStatus();

    //add from v3.0
    unsigned long getCallID();
    void setCallID(unsigned long);
    void setSignalError(int code);
    int getSignalError();
    void setFromTo(const char* from, const char* to);
    const char* getFromString();
    const char* getToString();
    void setLocalIp(unsigned long local_ip);
    unsigned long getLocalIp();

    //added from TTR3.0
    virtual const char* serialize(Json::Value& value);    //serialize into text format
    virtual bool parse(const Json::Value& value);

    void setAudioSSRC(unsigned long audioSSRC);
    void setVideoSSRC(unsigned long videoSSRC);
    unsigned long getAudioSSRC();
    unsigned long getVideoSSRC();
    //added from TTR4.0
    void setEncryptFlag(bool encrypt);
    bool getEncryptFlag();
	void setCause(int cause);
    int getCause();

    //record file url
    void setRecPlayURL();
    void setRecDownloadURL();
    char* getRecPlayURL();
    char* getRecDownloadURL();

    void setPTZFlag(int ptz);
    int getPTZFlag(); //0 or 1: support,2:not support

    //set retransfer payload type
    void setRetransferPT(int pt){rts_pt = pt;}
    int getRetransferPT(){return rts_pt;}
    void setRetransferSSRC(unsigned long rtsssrc){rts_ssrc=rtsssrc;}
    unsigned long getRetransferSSRC(){return rts_ssrc;}
    void setOriSSRC(unsigned long orissrc){ori_ssrc = orissrc;}
    unsigned long getOriSSRC(){return ori_ssrc;}

    bool isP2PVideoDial();
    void setP2PVideoDialFalg(bool);

private:
    MSISDN theCaller;
    MSISDN theCallee;
    P2pvideocall_Status_t theStatus;

    //SDP related information
    //for the case of media_bypass, upper layer need handle media setup
    int theLocal_port_audio;
    int theLocal_port_video;
    int theRemote_port_audio;
    int theRemote_port_video;
    unsigned long theRemote_ip_video;
    int sound_mute;
    MSISDN uri;
    int channel;
    int usercfm;
    int Camera;
    Sound_t soundpldtype;
    Fmt_type Fmt;

    //added from v3.0
    unsigned long cid;
    int signalError;
    char fromString[MAX_TELENUMBER_LENGTH];
    char toString[MAX_TELENUMBER_LENGTH];
    unsigned long theLocal_ip;

    //added from v4.0
    unsigned long audio_ssrc; //audio ssrc
    unsigned long video_ssrc; //video ssrc
    bool encryptflag;
    int videoBye_cause;
    char playUrl[MAX_URL_LENGTH];
    char downloadUrl[MAX_URL_LENGTH];
    VideoControl_t control_type;
    int control_value;
    int m_CameraPTZ;//2:not support, 0 or 1: support
    int rts_pt;     //retransfer payloadtype
    unsigned long rts_ssrc;   //retransfer ssrc
    unsigned long ori_ssrc;   //original ssrc
    bool m_videodialflag;     //true means p2p video dial, false means not
};

#endif // P2PVIDEOCALLSTATUS_INDICATOR_H
