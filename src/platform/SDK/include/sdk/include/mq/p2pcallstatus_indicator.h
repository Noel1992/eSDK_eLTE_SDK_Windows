/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    p2pcallstatus_indicator.h
Author:      Kai Wang
Created:     2012/08/014 16:36
Description: Responsible for point-to-point call status notification from SDK to upper app

*********************************************************************/

#ifndef P2PCALLSTATUS_INDICATOR_H
#define P2PCALLSTATUS_INDICATOR_H

#include "sdk_export.h"
#include "common.h"
#include <string>

namespace Json{
class Value;
}

/**
   define event types notified for point-to-point calls
**/
typedef enum {
    P2P_IND_STATUS_UNKNOWN = 2000,

    P2P_IND_STATUS_INITIATED = 2001,                /** reserved, */
    P2P_IND_STATUS_RECEIVED = 2002,                 /** a voice call request is received from remote end*/
    P2P_IND_STATUS_ANSWERED = 2003,                 /** a voice call is answered by remote end*/
    P2P_IND_STATUS_EMERGENCY = 2004,                /** an emergency voice conversation is started*/
    P2P_IND_STATUS_PROCEEDING = 2005,               /** 100trying is received */
    P2P_IND_STATUS_RINGING = 2006,                  /** 180ring is received, remote end is ringing*/
    P2P_IND_STATUS_ACKED = 2007,                    /** an anwser is acked by remote end*/
    P2P_IND_STATUS_RELEASED = 2008,                 /** reserved, internal use only */
    P2P_IND_STATUS_HANGUPED = 2009,                 /** a voice call is terminated by remote end*/
    P2P_IND_STATUS_HANGUPED_ACTIVE = 2010,          /** a voice call is being terminated by DC*/
    P2P_IND_STATUS_CANCELLED = 2011,                /** a voice call request is cancelled*/
    P2P_IND_STATUS_PREMPTED = 2012,                 /** a voice call is prempted */
    P2P_IND_STATUS_RELEASED_BUSY = 2013,            /** remote busy*/
    P2P_IND_STATUS_BREAKIN_FAIL = 2014,             /** break-in failed*/
    P2P_IND_STATUS_BREAKIN_SUCCESS = 2015,          /** break-in succeeded*/
    P2P_IND_STATUS_PROHIBITED = 2016,               /** no permissions*/
    P2P_IND_STATUS_REMOTE_NOANSWER = 2017,          /** remote end no answer*/
    P2P_IND_STATUS_REMOTE_NOTFOUND = 2018,          /** remote end not found*/
    P2P_IND_STATUS_LICENSE_LIMIT = 2019,            /** out of license*/

    P2P_IND_STATUS_MEDIAPLANE_ERROR = 2020,         /** audio stream setup fail*/
    P2P_IND_STATUS_MEDIAPLANE_MUTE = 2021,          /** audio is muted*/
    P2P_IND_STATUS_MEDIAPLANE_UNMUTE = 2022,        /** audio is unmuted*/
    P2P_IND_STATUS_REMOTE_NOTSUPPORTED = 2023,      /** remote end doesn't support the required audio format */
    P2P_IND_STATUS_REMOTE_NOTCONNECTED = 2024,      /** remote end not registered*/
    P2P_IND_STATUS_TRANSFER_SUCCESS = 2025,         /** voice transfer succeeded*/
    P2P_IND_STATUS_TRANSFER_FAIL = 2026,            /** voice transfer failed*/
    P2P_IND_STATUS_TRANSFER = 2027,
    P2P_IND_STATUS_SIGNAL_ERROR = 2028,             /** SIP error, used in SDK_GW */
    P2P_IND_STATUS_EMERGENCY_BEGIN = 2029,          /** an emergency voice call request is received from remote end*/
    P2P_IND_STATUS_RING_START_180 = 2030,           /** reserved, internal use only*/
    P2P_IND_STATUS_RING_START_183 = 2031,           /** reserved, internal use only*/
    P2P_IND_STATUS_RING_STOP = 2032,                /** reserved, internal use only*/

    P2P_IND_STATUS_PICKEDUP = 2040,                 /** callee picked up  event*/
    P2P_IND_STATUS_HALFDUPLEX_BEGIN = 2041,         /** half duplex p2p call request received */
    P2P_IND_STATUS_TX_BEGIN = 2042,                 /** half_duplex p2p call transmit begin */
    P2P_IND_STATUS_TX_IDLE = 2043,                  /** half_duplex p2p call transmit idle */
    P2P_IND_STATUS_TX_VOICE = 2044,                 /** half_duplex p2p call transmit voice */
    P2P_IND_STATUS_FC_ACCEPTED= 2045,               /** half_duplex p2p floor accepted */
    P2P_IND_STATUS_FC_REJECTED = 2046,              /** half_duplex p2p floor rejected */
    P2P_IND_STATUS_ENVIR_ANSWERED = 2047,            /** environment listen answered*/
    P2P_IND_STATUS_CALLER_UNSUPPORT = 2048,          /** caller unsupport*/
    P2P_IND_STATUS_CALLEE_UNSUPPORT = 2049,          /** callee unsupport*/
    P2P_IND_STATUS_CIPHER_NOT_AVAILABLE = 2050,          /** cipher not available*/
    P2P_IND_STATUS_MISCELLANEOUS_WARNING = 2051,          /** Miscellaneous warning*/
    P2P_IND_STATUS_REMOTE_PROHIBITED = 2052               /** remote no permissions*/

} P2pcall_Status_t;


/**
   define callstatus indicator details for point-to-point calls
**/
class SDK_IMPORT_EXPORT P2pcallStatusIndicator
{
public:
    P2pcallStatusIndicator(MSISDN caller, MSISDN callee);
    P2pcallStatusIndicator(P2pcallStatusIndicator& copy);
    virtual ~P2pcallStatusIndicator();

    std::string datalog_str();

    void setStatus(P2pcall_Status_t status);
    void setPeers(MSISDN caller, MSISDN callee);
    void setBreakins(MSISDN inserter, MSISDN targeter);

    void setPorts(int localPort, int remotePort);
    void setRemoteIP(unsigned long IP);
    void setSoundPtype(Sound_t Ptype);
    MSISDN getCaller();
    MSISDN getCallee();
    MSISDN getInserter();
    MSISDN getTargeter();

    P2pcall_Status_t getStatus();
    unsigned long getRemoteIP();
    int getLocalPort();
    int getRemotePort();
    Sound_t getSoundPtype();

    //add from v3.0

    void setTransfers(const char* transfer, MSISDN targeter);
    char* getTransfer();

    void setDiscreetListen(MSISDN Listenee); 
    int getDiscreetListenee(); 

    void setCallID(unsigned long callid);
    unsigned long getCallID();
    void setSignalError(int code);
    int getSignalError();
    void setFromTo(const char* from, const char* to);
    const char* getFromString();
    const char* getToString();
    void setLocalIP(unsigned long IP);
    unsigned long getLocalIP();

    virtual const char* serialize(Json::Value& value);
    virtual bool parse(const Json::Value& value);

    /** added from TTR4.0 */
    void setAudioSSRC(unsigned long ssrc);
    unsigned long getAudioSSRC();
    void setSpeaker(const char* speaker);
    char* getSpeaker();

    void setSubType(int type);//获取通话类型
    int getSubType();

    //record file url
    void setRecPlayURL();
    void setRecDownloadURL();
    char* getRecPlayURL();
    char* getRecDownloadURL();

private:
    P2pcall_Status_t theStatus;     //status value
    MSISDN theCaller;               //caller in MSISDN format
    MSISDN theCallee;               //callee in MSISDN format
    MSISDN theInserter;             //inserter in MSISDN format
    MSISDN theTargeter;             //interrupted or transferred target ID in MSISDN format
    MSISDN theListenee;             //listenee for Discreet Listen. 
    char theTransfer[MAX_TELENUMBER_LENGTH];//call transfer target in string format
    int theLocalPort;               //RTP local port
    int theRemotePort;              //RTP remote port
    unsigned long theRemoteIP;      //RTP remote IP address
    Sound_t soundpldtype;           //sound payload type

    //add from v3.0
    unsigned long theCallID;    //SIP Call ID
    int signalError;            //SIP error code
    char fromString[MAX_TELENUMBER_LENGTH]; //SIP from in string format
    char toString[MAX_TELENUMBER_LENGTH];   //SIP to in string format
    unsigned long theLocalIP;   //RTP local IP address

    /** added from TTR4.0 */
    unsigned long audio_ssrc;                   /**  ssrc */
    char theSpeaker[MAX_TELENUMBER_LENGTH];     /** speaker when P2P_IND_STATUS_TX_BEGIN */
    int subtype; //0=full-duplex, 1=half-duplex
    char playUrl[MAX_URL_LENGTH];
    char downloadUrl[MAX_URL_LENGTH];
};

#endif // P2PCALLSTATUS_INDICATOR_H
