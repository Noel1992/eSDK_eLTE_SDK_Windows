
#ifndef LICENSE_CONTROL_H
#define LICENSE_CONTROL_H

#include "sdk_export.h"
#include <string>
#include "common.h"

/**
 \brief define service types for license control and connection control
 */
typedef enum
{
    DIAL,           /** P2P audio service */
    VIDEO_P2P,      /** P2P video service */
    VIDEO_UPLOAD,   /** video surveillance service */
    PTT,            /** PTT group service */
    UNDEFINED_TP    /** reserved */
}Service_type;

/**
 \brief define call priority
 */
typedef enum
{
    EMERGENCY,
    NORMAL
}Priority_type;

/**
  \brief define a service detail information, transferred between SDK and 3rd-APP for LicenseControl or ConnectionControl
*/
class SDK_IMPORT_EXPORT ServInfo
{
public:
    ServInfo(MSISDN caller, MSISDN callee);
    ServInfo(const ServInfo& sInfo);

    std::string datalog_str();

    void setServiceType(Service_type stype);
    void setPriType(Priority_type ctype);
    void setFmt(Fmt_type FmtVlaue);
    void setMute(int MuteValue);
    void setCallId(MSISDN callid);

    MSISDN getCaller();
    MSISDN getCallee();
    Service_type getServiceType();
    Priority_type getPriType();
    Fmt_type getFmt();
    int getMute();
    MSISDN getCallId();

private:
    MSISDN m_Caller;
    MSISDN m_Callee;
    Service_type m_stype;
    Priority_type m_ctype;
    Fmt_type m_ftype;
    int m_mute;
    MSISDN m_callid;
};

/**
  \brief license control callback interface
*/
class SDK_IMPORT_EXPORT LicenseControl
{
public:

    /**
     \brief check license ok or not
     \param Sinfo: service detail information to be checked
     \return true means OK，false means NOT OK
    */
    virtual bool is_license_accept(ServInfo *Sinfo) =0;

};

/**
  \brief define a connection detail information, transferred between SDK and 3rd-APP for ConnectionControl
*/
class SDK_IMPORT_EXPORT ConnInfo
{
public:
    ConnInfo();
    ConnInfo(char lip[]);
    void setLocalIp(char *localip);
    void setAudioport(unsigned int aport);
    void setVideoport(unsigned int vport);

    char * getLocalIp();
    unsigned int getAudioport();
    unsigned int getVideoport();
    std::string datalog_str();

private:
    char m_LocalIp[20];
    unsigned int m_audioport;
    unsigned int m_videoport;

};

/**
  \brief connection control callback interface
*/
class SDK_IMPORT_EXPORT ConnectionControl
{
public:

    /**
     \brief check connection control from 3rd-party APP
     \param Sinfo: input parameter, carry the details of a Service
     \param Cinfo: output parameter, carry the details of connection info allocated for this service
     \return true means OK and connection infor is carried out by Cinfo，false means NOT OK
     \note 3rd APP can implement a ConnectionControl interface and register it to SDK by invoking ApplicationContext::setConnectionControl()
            in this way 3rd APP can control how RTP connection parameters (such as media IP address and port)are allocated
     */
    virtual bool get_Connection_info(ServInfo *Sinfo,ConnInfo *Cinfo) =0;

};


#endif // LICENSE_CONTROL_H
