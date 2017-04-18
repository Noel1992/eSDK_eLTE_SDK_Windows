#ifndef GWSTATE_H
#define GWSTATE_H

#include "sdk/gwapi/service_observer.h"
#include "sdk/include/sdk_export.h"
#include <string>
#include "sdk/include/common.h"

using namespace LTE::TRUNK::SDK;
class QueryManager;
class Gw_DisplayManager;
class Gw_GISReportManager;

class SDK_IMPORT_EXPORT GWState
{
public:
    static GWState* getInstance();

    /**
    \brief set login required info for gateway node login to the system
    \param gwid: ID of this gateway node, shall be configured at MDC web client
    \param password: password for gwid
    \param serverIP: MDC address this gateway node connects
    \param serverPort: MDC SIP port this gateway node connects
    \param localIP: local SIP IP this gateway node uses to connect MDC
    \param localPort: local SIP port this gateway node uses to connect MDC
    */
    virtual void set_login_info(MSISDN gwid,const char* password,const char* serverIP, int serverPort, const char* localIP,int localPort);
    /**
    \brief set audio prefer list for this gateway node
    \note support 4 audio payload types at most,
    and if one payload type is AMR , you shall also need designate AMR mode
    */
    virtual void set_audio_prefer(Payload_t first,Payload_t second,Payload_t third,Payload_t fourth,PRIORITY_t amr_mode=NO_AMR);
    /**
    \brief set IP address and port range for RTP of this gateway node
    \param localip4RTP: IP address for RTP, can be different from localIP for SIP (in the case separating media from signal)
    \param port_start: RTP start port for RTP streams on this SDK_GW
    \param audio_stop: RTP stop port for RTP streams on this SDK_GW
    */
    virtual void set_rtp_info(const char* localip4RTP,unsigned short port_start,unsigned short port_stop);

    /**
    \brief Register observer classes for each service
    \note There is one service observer for each service, please register observers for those services your application need
    */
    virtual void registerObserver(VideoServiceObserver* obs);
    virtual void registerObserver(AudioServiceObserver* obs);
    virtual void registerObserver(GroupServiceObserver* obs);
    virtual void registerObserver(MessageServiceObserver* obs);
    virtual void registerObserver(GisServiceObserver* obs);
    virtual void registerObserver(QueryServiceObserver* obs);
    virtual void registerObserver(ModuleStatusObserver* obs);
    virtual void registerObserver(ResourceStatusObserver* obs);
    /**
    \brief logon/logoff with gwid,
    \note  gwid and passowrd shall be first provided in GWState::set_login_info()
    */
    virtual int logonGW();
    virtual int logoffGW();
    /**
    \brief logon/logoff for this gateway served user,
    \note  the userid shall be confiured as FixedUser at MDC web client, 
    if you want set different logon type, can invoke setLogonType(LogOn_Proxy_t type) in sdk_helper.h
    */
    virtual int logon_proxy(MSISDN _userid,const char* _password);
    virtual int logoff_proxy(MSISDN _userid,const char* _password);

    const char* show_state();
    const char* show_calls();
    bool isVideoExist(MSISDN uri);

private:
    GWState();
};


#endif // GWSTATE_H
