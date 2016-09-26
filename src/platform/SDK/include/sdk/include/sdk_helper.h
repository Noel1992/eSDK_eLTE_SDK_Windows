#ifndef SDK_HELPER_H
#define SDK_HELPER_H

#include "sdk_export.h"
#include "platform/modules/timer/ubp_timer.hpp"
#include "uiparameter.h"
#include "dcstate.h"

using namespace ubp::platform::timer;
class ResourceInfo;


typedef std::map<MSISDN, DC_LIST> TasklistMap;
typedef std::map<MSISDN, DC_LIST>::iterator TaskListMapIterator;


class SDK_IMPORT_EXPORT SDK_Helper {
public:
    // Change Sip UA
    static void change_sip_ua(const char * sipua);

    static void clean_invalid_reg();
    
    static int checkDcPriority(int , int);

    //异步调用invoke Operation
    static int scheduleAsynchOperation(ResourceInfo *res_info, int op_type, void* para=0);
    //同步调用invoke Operation
    static int execsynchOperation(ResourceInfo *res_info, int op_type, void* para=0);

    static bool getVideoStatistics(MSISDN resid, VideoStatistics_t& struVideoStaInfo, bool bBitRateFlag);

    static ubp::platform::timer::TimerDispatcher* getTimerDispatcher();

    static int getTaskId(MSISDN physicalResid);

    static void setResource(std::string resource);

    static int getCUrrentListener(bool isPTTtype);

    //TIC related utilities
    static bool isTicResource(MSISDN resid);

    static bool isTicNumber(const char* number);

    static MSISDN getOutboundTicResource();

    static const Res_Reg_Status* findUserRegStatus(MSISDN iUeUd, Reg_Type_t type);

    /**
     \brief send SIP informessage, it's like:Call-Info: <xxxxx>;name1=value1; name2=value2;
     \param callid: Call-info id
     \return: Only return 0 means successfully
    */
    static int SendSipInfoMsg(MSISDN callid, char *name1,char* value1,char *name2,char* value2);
    /**
      \brief set the logon type
    */
    static void setLogonType(LogOn_Proxy_t type);
    static LogOn_Proxy_t getLogonType();
    /**
     \brief set video loss retransfer info
     \param status: 0: not support, 1: support,default is 0
     \param rtxtime: original RTP max cache time
    */
    static void setVideoLossRetransInfo(int status, int rtxtime);

    //for camera circle upload
    static TasklistMap& listMap();

};

#endif // SDK_HELPER_H
