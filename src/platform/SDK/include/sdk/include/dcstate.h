/********************************************************************

Copyright 2012,TD-Tech. Co., Ltd.

Filename:    dcstate.h

Author:      Wu yu wei

Created:     2012/07/07 10:00

Description: Define different API and data structure for SDK_DC

*********************************************************************/

#ifndef DCSTATE_H

#define DCSTATE_H

#include "uiparameter.h"
#include "mq/resourcestatus_indicator.h"
#include <map>
#include <memory>
#include <string>
#include "displaymanager.h"
#include "sdk_export.h"

/**
  \brief User Status Value
*/
typedef enum {
    INVITING = 0,
    RINGING = 1,
    TALKING = 2,
    CLOSING = 3
}User_Status_Value;

/**
  \brief Call Type Value
*/
typedef enum {
    UNREG =0,
    REG =1,
    VOICEP2P =2,
    VIDEOP2P =3,
    UPLOADP2P =4,
    VIDEODISP =5,
    VIDEOONWALL=6,
    VOICEP2G = 7,
    ABMIENCELISTENP2P = 8,
    HALFVIOCEP2P=9
}Call_Type_t;

/**
  \brief User Call Status data structure
*/
typedef struct User_Call_Status{
    res_status_value_t userStatus;
    int peerUserID;
    Call_Type_t calltype;
    int calldir;
    int is_encrypt;
    std::string peerNumber;

    int speakID; //speak isdn in half voice call
    std::string speakName;  //speak name in half voice call

    User_Call_Status* clone() const;
}User_Call_Status;

/**
  \brief User Registration Status data structure
*/
typedef struct {
    res_status_value_t regResult;
    resource_status_t regType;
}Res_Reg_Status;

/**
  \brief Group Status data structure
*/
typedef struct {
    MSISDN grpid;
    bool bCallActivated;
    int callPriority;
    MSISDN speaker;
    int is_encrypt;
    std::string speaker_str;
}Group_Status;

typedef enum {
    PROXY,
    DIRECT
}Reg_Type_t;

class GroupInfo;
class DcState_Imp;

/**
 \brief Group subscribing data structure
 \param bMuted: whether the group is muted
 \param bRecording: whether the group is in recording
 \param bDiscretListening: whether the group is in listening
 \param bCallActivated: whether the group is active
 \param bTempGroup: whether the group is a temp group
 \param setupDcId: creating DC of temp group or dynamic group
 \param speakerID: speaker assigned for this group
*/
class SDK_IMPORT_EXPORT SubscribeInfo {

public:
    SubscribeInfo();
    ~SubscribeInfo();
    bool bSelected;     //reserved

    bool bMuted;

    int grpPriority;

    bool bRecording;

    bool bDiscretListening;

    bool bCallActivated;

    bool bTempGroup;

    DGNA_parameter* tempGroupInfo;

    MSISDN setupDcId;

    bool bPTTReleaseLater;

    MSISDN OrigDcID;

    int calcPttPriority();

    float volumn;

    char speakerID[256];
};


/**
 \brief Camera subscribing data structure
 \param bMuted: whether the camera is muted
 \param camera: video channel, front camera or back camera
 \param bOnWall: whether the camera is projected on a video wall
*/
class SDK_IMPORT_EXPORT VideoInfo {

public:
    bool bMuted;

    int camera;

    bool bOnWall;

    int ptz;     
};


/**
 \brief User joining Group data structure
 \param userID: mobile ISDN
 \param is_joined_grp: whether the mobile is joining a group call
 \param grpID:	Group ID mobile is joining
*/
typedef struct user_inGrp_info

{

    MSISDN userID;

    bool is_joined_grp;

    MSISDN grpID;

}user_inGrp_info;

class SDK_IMPORT_EXPORT DcState

{
public:
    static DcState* getInstance();

    /**
     \brief set login required information of DC
     \param dcid : ID of the DC
     \param password: password of the DC
     \param serverIP : IP address of eMDC server
     \param sipPort: SIP port of eMDC server
     \param localIP : IP address of DC machine
     \param lang: language, such as "zh","en",etc
     \note  This API replace several setXXX() APIs
    */
    virtual void set_login_info(MSISDN dcid,const char* password,const char* serverIP, int sipPort, const char* localIP,const char* lang );

    virtual void setSipPort(const std::string &org);

    virtual const char* getSipPort();

    virtual void setSipLocalPort(const std::string &org);

    virtual const char* getSipLocalPort();

    virtual void setServerIP(const std::string &org);

    virtual std::string& getServerIP();

    virtual void setLocalIP(const std::string &org);

    /**
     \brief set local RTP address
     \param localrtp : IP address for local RTP
     \note  This API is to customize RTP negotiation, can be used together with updatePortRange() to customize your local RTP IP and port
    */
    virtual void setLocalIP4RTP(const std::string &localrtp);

    virtual std::string& getLocalIP();
    virtual std::string& getLocalIP4RTP();

    virtual void setPwd(const std::string &org);

    virtual std::string& getPwd();

    virtual void setUser(const std::string &org);

    virtual std::string& getUser();

    virtual void setLogin(bool login);

    virtual bool isLogin();

    virtual void resetState();

    virtual std::string & getLang();

    virtual void setLang(const std::string &org);

    virtual int getDCID(void);

    virtual void setDCID(int id);


    virtual SubscribeInfo * findSubscribe(MSISDN grpid) ;

    virtual std::auto_ptr<VideoInfo> findVideoInfo(MSISDN resID);

    virtual MSISDN getNextTempGroupID();

    virtual int unsubscribeTempGroup(MSISDN grpid);
    virtual bool isTempGroup(MSISDN grpid);

    virtual int getDCVPN(void);

    virtual User_Call_Status* findUserStatus(MSISDN userid, Call_Type_t calltype);
    virtual Res_Reg_Status* findUserRegStatus(MSISDN userid, Reg_Type_t regtype);

    /*---------------------ADDED FROM TTR3.0--------------------------*/

    /**
     \brief customize RTP port range
     \param start: start port number when SDK allocate RTP port
     \param end: end port number when SDK allocate RTP port
     \note  for some scenarios, 3rd-DC can customize RTP port ranges
    */
    virtual void updatePortRange(unsigned int start, unsigned int end);
    /**
     \brief logon/logoff
     \note  used together with set_login_info()，to replace TTR2.x ResourceInfo+DC_LOGIN for DC Login
    */
    virtual int logonDC();
    virtual int logoffDC();

    /**
     \brief assign a speaker for a group
     \param grpid: Group MSISDN
     \param speakerid: speaker ID, shall be one emuerated by enumSpeakers()
     \note  The voice from this group will be mixed and played in the assigned speaker
    */
    virtual int assignSpeaker4Group(MSISDN grpid, const char* speakerid);

    /**
     \brief assign a speaker for individual call on Dispatcher console
     \param speakerid: speaker ID, shall be one emuerated by enumSpeakers()
     \note  The voice from individual call will be mixed and played in the assigned speaker
    */
    virtual int assignSpeaker4Individual(const char* speakerid);

    /**
      \brief enumerate all speakers on the Dispatch PC
      \param speakers: an array of char*, the array shall be allocated by caller and be large enough (array size shall be larger than the count of speakers!)
      \return: the number of speakers found in this enumeration
      \note: first use this API to enumerate all speaker and save the ID for each speaker, then use assignSpeaker4XXX() API to assign speaker for different calls
    */
    virtual int enumSpeakers(char* speakers[]);

    /**
      \brief:Reserved
      \note:
    */
    virtual void addSubscribedGroup(MSISDN grpid,SubscribeInfo*);
    /**
      \brief:find whether exist p2p call at present
      \note: NOT include Emergency call, if want to find all active p2pcall, you can call 'findAllActiveP2pCall'.
    */
    virtual bool findActiveP2pCall();

    /**
      \brief:find whether exist p2p call at present
      \note: include Emergency call
    */
    virtual bool findAllActiveP2pCall();


private:
    static DcState* proxy;
    static DcState_Imp* inner;
};


#endif // DCSTATE_H

