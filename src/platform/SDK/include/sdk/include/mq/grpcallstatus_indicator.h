/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    uidspinfo.h
Author:      Wu yu wei
Created:     2012/08/01 14:00
Description:
*********************************************************************/
#pragma warning(disable: 4251)
#ifndef UIDSPINFO_H
#define UIDSPINFO_H

#include "sdk_export.h"
#include "common.h"
#include <string>
namespace Json {
    class Value;
}

/**
   \brief define event types notified for group call service
**/
typedef enum {
    PTT_IND_STATUS_UNKNOWN = 1000,                      /** reserved*/
    PTT_IND_STATUS_RECEIVED = 1001,                     /** a group call request is received*/
    PTT_IND_STATUS_REQUEST = 1002,                      /** reserved */
    PTT_IND_STATUS_ACCEPT = 1003,                       /** a floor request is accepted*/
    PTT_IND_STATUS_WAITING = 1004,                      /** reserved */
    PTT_IND_STATUS_REJECT = 1005,                       /** a floor request is rejected*/
    PTT_IND_STATUS_SNATCH = 1006,                       /** a group call is started*/
    PTT_IND_STATUS_HANGUP_OK = 1007,                    /** DC leaves the group call*/
    PTT_IND_STATUS_CLOSE_OK = 1008,                     /** a group call is closed*/
    PTT_IND_STATUS_CANCEL_OK = 1009,                    /** a group call request is cancelled*/
    PTT_IND_STATUS_PREMPTED = 1010,                     /** a group call is prempted in SDK*/

    PTT_IND_STATUS_TX_BEGIN = 1011,                     /** somebody is talking in the group*/
    PTT_IND_STATUS_TX_IDLE = 1012,                      /** nobody is talking in the group*/
    PTT_IND_STATUS_MEDIAPLANE_ERROR = 1013,             /** group call setup failed */
    PTT_IND_STATUS_EMERGENCY_BEGIN = 1014,              /** a emergency group call request is received*/
    PTT_IND_STATUS_EMERGENCY_UPDATE = 1015,             /** a group call is becoming emergency group call*/
    PTT_IND_STATUS_LICENSE_LIMIT = 1016,                /** out of license*/
    PTT_IND_STATUS_SIGNAL_ERROR = 1017,                  /** SIP error, used in SDK_GW*/
    PTT_IND_STATUS_RECORDING = 1018
} Speaker_Status_t;

class GrpCallInfo;

/**
   \brief define indicator object details notified for group call service
**/
class SDK_IMPORT_EXPORT GrpDspInfo
{
public:
    GrpDspInfo(MSISDN grpId);
    GrpDspInfo(GrpDspInfo& clone);
    virtual ~GrpDspInfo();
    std::string datalog_str();

    void setGrpId(MSISDN grpId);
    void setSpStatus(Speaker_Status_t status);
    void setSpeaker(MSISDN speaker);
    void setSpeakerName(std::string name);
    void setPorts(int localPort, int remotePort);
    void setRemoteIP(unsigned long IP);
    void setSoundPtype(Sound_t Ptype); 
    Speaker_Status_t getSpStatus();
    std::string getSpeakerName();
    MSISDN getSpeaker();
    MSISDN getGrpId();

    unsigned long getRemoteIP();
    int getLocalPort();
    int getRemotePort();
    Sound_t getSoundPtype();

    //add from v3.0

    void setCallID(unsigned long callid);
    unsigned long getCallID();
    void setSignalError(int code);
    int getSignalError();
    void setFromTo(const char* from, const char* to);
    const char* getFromString();
    const char* getToString();
    unsigned long getLocalIP();
    int getUri(){return _uri;}
    void setLocalIP(unsigned long IP);

    virtual const char* serialize_speaker(Json::Value &value);
    virtual const char* serialize(Json::Value& value);
    virtual bool parse(const Json::Value& value);

    void setAudioSSRC(unsigned long ssrc);
    unsigned long getAudioSSRC();

    void setSpeaker(std::string speaker);
    std::string getSpeakerStr();
    void setUri(int uri){_uri = uri;}
    //record file url
    void setRecPlayURL();
    void setRecDownloadURL();
    char* getRecPlayURL();
    char* getRecDownloadURL();

private:
    MSISDN _grpId;                              /// group number
    Speaker_Status_t _sp_status;                /// status
    MSISDN _speaker;                            /// speaker number
    std::string str_speaker;                    /// speaker number (in string format)

    int theLocalPort;                           /// local RTP port
    int theRemotePort;                          /// remote RTP port
    unsigned long theRemoteIP;                  /// remote IP
    Sound_t soundpldtype;                       /// sound type
    std::string _speakerName;                   /// speaker name

    //added from v3.0
    unsigned long theCallID;                    /// call_id
    int signalError;                            /// SIP error code
    char fromString[MAX_TELENUMBER_LENGTH];     /// from
    char toString[MAX_TELENUMBER_LENGTH];       /// to
    unsigned long theLocalIP;                   /// local IP

    //added from TTR4.0
    unsigned long audio_ssrc;
    char playUrl[MAX_URL_LENGTH];
    char downloadUrl[MAX_URL_LENGTH];
    int _uri;

};

typedef GrpDspInfo GroupStatusIndicator;

#endif // UIDSPINFO_H
