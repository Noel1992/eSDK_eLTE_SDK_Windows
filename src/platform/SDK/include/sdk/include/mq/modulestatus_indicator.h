/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    modulestatus_indicator.h
Author:      zhangjiansheng
Created:     2013/03/21 10:36
Description: module level notifications from SDK to upper layer
*********************************************************************/

#pragma warning(disable: 4251)
#ifndef MODULESTATUS_INDICATOR_H
#define MODULESTATUS_INDICATOR_H

#include <string>
#include "sdk_export.h"

namespace Json{
 class Value;
}

typedef enum {
    SDS_MODULE = 1,         /// short data service module
    SMS_MODULE = 2,         /// multimedia message service module
    AUDIO_MODULE = 3,       /// audio module
    VIDEO_MODULE = 4,       /// video module
    GIS_MODULE = 5,         /// GIS module
    SIP_MODULE = 6,         /// SIP signalling module
    RTP_MODULE = 7,         /// RTP streaming module
    RING_MODULE = 8,        /// Ring module
    SVC_MODULE = 9,         /// MDC
    UNKOWN_MODULE = 999
} dc_module_t;

typedef enum {
    RESOURCE_STATUS_NORMAL = 0,         /// reserved
    SDS_OVERFLOW = 1,                   /// short message inbox overflow
    SDS_DISCONNECT = 2,                 /// short message service disconnected
    SDS_CONNECT = 3,                    /// short message service connected
    SDS_SEND_SUCCESS = 4,               /// reserved
    SDS_SEND_FAIL = 5,                  /// reserved
    FOLDER_IS_EMPTY = 6,                /// reserved
    GIS_LICESNCE_UPDATE = 7,            /// internal use only
    GIS_LICENSCE_EXPIREDAY = 8,         /// internal use only
    SIP_DISCONNECT = 9,                 /// SIP link disconnected
    SIP_RECONNECT = 10,                 /// SIP link recovered
    SIP_KEEPALIVE_REJECT = 11,          /// SIP link keepalive failed
    ATTACH_UPLOAD_FAIL = 12,            /// attachment upload failed
    ATTACH_DOWNLOAD_FAIL = 13,          /// attachment download failed
    SIP_START_RUNNING = 14,             /// MDC restarted (for MRS only)
    KICK_OFF = 15,                      /// DC is kick-off
    ASYN_CALLBACK_RETURN = 16,          /// Asynchronous event notification, see "call_back_msg_type" below
    PASSWORD_CHANGE = 17,               /// DC password changed
    MODULE_DISCONNECT = 18,             /// Server not connected
    MODULE_RECONNECT = 19,              /// Server connected
    SIP_KA_FAILONCE = 20,               /// internal use only
    SIP_KA_OKONCE = 21,                 /// internal use only
    USER_DELETE = 22,                   /// internal use only
    CERT_AUTH_FAIL = 23,                /// internal use only
    ATTACH_UPLOAD_FAIL_LOST = 24,       /// attachment upload fail due to file lost
    ATTACH_UPLOAD_FIAL_UNAUTHORIZED = 25,   /// upload failed due to unauthorized
    ATTACH_DOWNLOAD_FIAL_UNAUTHORIZED = 26, /// download failed due to unauthorized
    ATTACH_UPLOAD_FAIL_SECTION = 27,        ///Resume broken transfer
    ATTACH_DOWNLOAD_FAIL_SECTION = 28,      ///Resume broken downloads
    ATTACH_REQUEST_URL_ERROR = 29,          ///request url is illegal
    ATTACH_ACCESS_FORBIDDEN = 30,           ///upload/download attach forbidden,maybe password is wrong
    ATTACH_NOTFOUND = 31,                   ///attach was not found in SDS Server
    ATTACH_INTERNAL_ERROR = 32,             ///SDS Server internal error
    ATTACH_SERVER_SPACE_FULL = 33,          ///SDS Server space full
    PROGRESS_RECORD_FILE = 34,              ///progress details when downloading file.
    PROGRESS_FILE_RESULT = 35,              ///result for downing file
	GIS_MONITOR = 36,
    UNKOWN_STATUS = 999
} module_status_t;

typedef enum
{
    breakoff,
    insert,
    SubGroup,
    DeSubGroup,
    doTransfer,
    doBreakIn,
    doPTZ,
    Dispatch,
    Dispatchdelete,
    VwallStop,
    VwallStart,
    DeAssignDgna,
    invalid_type,
    QueryUeBindInfo,
    ModPswInfo,
    VerCheck, 
    DeviceQuery,
    TempUserJoinGroup,
    UNKOWN_MSGTP = 999
}call_back_msg_type;


class NestedModuleIndicator;

class SDK_IMPORT_EXPORT DCModuleStatusIndicator
{
public:
    DCModuleStatusIndicator(MSISDN resid);
    virtual ~DCModuleStatusIndicator();
    std::string datalog_str();

    void setModuleStatus(module_status_t status_value);
    void setResID(MSISDN resid);

    void setModuleType(dc_module_t module_type);

    void setModulePara(const Json::Value& value);

    void setCallBackBizType(call_back_msg_type msg_biz_type);

    module_status_t getModuleStatus();
    dc_module_t getModuleType();

    /** out_para for ModuleStatusIndicator **/
    std::string getModulePara(Json::Value & out_para);
    std::string getModulePara();

    MSISDN getResid();
    call_back_msg_type getCallBackBizType();

    virtual const char* serialize(Json::Value& value);
    virtual bool parse(const Json::Value& value);

 private:
    NestedModuleIndicator * _impl;

};
#endif // MODULESTATUS_INDICATOR_H
