#ifndef GWSTATE_IMP_H
#define GWSTATE_IMP_H

#include "sdk/gwapi/service_observer.h"
#include "sdk/include/sdk_export.h"
#include "sdk/include/mq/resourcestatus_indicator.h"
#include <string>
#include "sdk/include/common.h"
#include "ace/Future.h"

using namespace LTE::TRUNK::SDK;

class SDK_IMPORT_EXPORT GWState_Imp 
{
public:
    GWState_Imp();

    void set_login_info(MSISDN _gwid,const char* _password,const char* _serverIP, int _server_sipPort, const char* localIP,int _local_sipPort);
    void set_audio_prefer(Payload_t first,Payload_t second,Payload_t third,Payload_t fourth,PRIORITY_t amr_mode=NO_AMR);
    void set_rtp_info(const char* localip4RTP,unsigned short start,unsigned short stop);

    int logonGW();
    int logoffGW();
    int logon_proxy(MSISDN _userid,const char* _password);
    int logoff_proxy(MSISDN _userid,const char* _password);

    MSISDN getGWID(){return gwid;}
    const char* getUserName(){return userName.c_str();}
    const char* getPassWord(){return passWord.c_str();}

    const char* getSipPort(){return remote_sipport.c_str();}
    const char* getSipLocalPort(){return local_sipport.c_str();}
    const char* getServerIP(){return serverip.c_str();}
    const char* getLocalIP(){return localip.c_str();}
    PRIORITY_t get_amr_priority();
    const char* getLocalIP4RTP(){return localip4RTP.c_str();}

    void signalLoginStatus(bool bSuccess, res_status_value_t cause);
    res_status_value_t waitLoginStatus();

    //show and debug facility
    const char* show_state();
    const char* show_calls();
    bool isVideoExist(MSISDN uri);
private:
    MSISDN gwid;
    std::string userName;
    std::string passWord;

    //Control Plane Configuration
    std::string serverip;   //serverip for SIP (serverip for RTP is negotiated by SDP)
    std::string remote_sipport; //remote sipport
    std::string local_sipport; //local sipport
    std::string localip;        //localip for SIP

    //Media Plane Configuration
    std::string localip4RTP;    //localip for RTP
    Payload_t first_audio;        //RTP offer audio preference 1
    Payload_t second_audio;       //RTP offer audio preference 2
    Payload_t third_audio;        //RTP offer audio preference 3
    Payload_t fourth_audio;       //RTP offer audio preference 4

    PRIORITY_t amr_priority;


    ACE_Future<res_status_value_t> regResult;
};

#endif // GWSTATE_IMP_H
