#ifndef LOGGER_H
#define LOGGER_H

#include "platform/modules/trace/ubp_trace.hpp"
#include "platform/modules/error/ubp_error.hpp"

#include "ubp_securityc.hpp"


/************************** RTP begin ********************************************/
/*****   RTP   ******/
static log4cxx::LoggerPtr rtp_logger(log4cxx::Logger::getLogger("DC.RTP"));
#define UBP_RTP_DEBUG(A)  LOG4CXX_DEBUG(rtp_logger, A)
#define UBP_RTP_INFO(A)   LOG4CXX_INFO(rtp_logger, A)
#define UBP_RTP_WARN(A)   LOG4CXX_WARN(rtp_logger, A)
#define UBP_RTP_ERROR(A)  LOG4CXX_ERROR(rtp_logger, A)
#define UBP_RTP_FATAL(A)  LOG4CXX_FATAL(rtp_logger, A)
/************************** SIP end *******************************************/


/************************** SIP begin ********************************************/
/*****   SIP   ******/
static log4cxx::LoggerPtr sip_logger(log4cxx::Logger::getLogger("DC.SIP"));
#define UBP_SIP_DEBUG(A)  LOG4CXX_DEBUG(sip_logger, A)
#define UBP_SIP_INFO(A)   LOG4CXX_INFO(sip_logger, A)
#define UBP_SIP_WARN(A)   LOG4CXX_WARN(sip_logger, A)
#define UBP_SIP_ERROR(A)  LOG4CXX_ERROR(sip_logger, A)
#define UBP_SIP_FATAL(A)  LOG4CXX_FATAL(sip_logger, A)
/************************** SIP end *******************************************/

/*****   GW    ******/
static log4cxx::LoggerPtr gw_logger(log4cxx::Logger::getLogger("GW"));
#define UBP_GW_DEBUG(A) LOG4CXX_DEBUG(gw_logger, A)
#define UBP_GW_INFO(A) LOG4CXX_INFO(gw_logger, A)
#define UBP_GW_WARN(A) LOG4CXX_WARN(gw_logger, A)
#define UBP_GW_ERROR(A) LOG4CXX_ERROR(gw_logger, A)
#define UBP_GW_FATAL(A) LOG4CXX_FATAL(gw_logger, A)

/************************** DC begin ********************************************/

/*****   DC    ******/
static log4cxx::LoggerPtr dc_logger(log4cxx::Logger::getLogger("DC"));
#define UBP_DC_DEBUG(A) LOG4CXX_DEBUG(dc_logger, A)
#define UBP_DC_INFO(A) LOG4CXX_INFO(dc_logger, A)
#define UBP_DC_WARN(A) LOG4CXX_WARN(dc_logger, A)
#define UBP_DC_ERROR(A) LOG4CXX_ERROR(dc_logger, A)
#define UBP_DC_FATAL(A) LOG4CXX_FATAL(dc_logger, A)


/*****   DC.GUI  begin  ******/
static log4cxx::LoggerPtr dc_gui_logger(log4cxx::Logger::getLogger("DC.GUI"));
#define UBP_GUI_DEBUG(A) LOG4CXX_DEBUG(dc_gui_logger, A)
#define UBP_GUI_INFO(A) LOG4CXX_INFO(dc_gui_logger, A)
#define UBP_GUI_WARN(A) LOG4CXX_WARN(dc_gui_logger, A)
#define UBP_GUI_ERROR(A) LOG4CXX_ERROR(dc_gui_logger, A)
#define UBP_GUI_FATAL(A) LOG4CXX_FATAL(dc_gui_logger, A)
#define UBP_GUI_DEBUG3(RES,OP,MSG) LOG4CXX_DEBUG(dc_gui_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_GUI_INFO3(RES,OP,MSG) LOG4CXX_INFO(dc_gui_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_GUI_ERROR3(RES,OP,MSG) LOG4CXX_ERROR(dc_gui_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_GUI_FATAL3(RES,OP,MSG) LOG4CXX_FATAL(dc_gui_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)

/*****   DC.STS  begin  ******/
static log4cxx::LoggerPtr dc_sts_logger(log4cxx::Logger::getLogger("DC.STS"));
#define UBP_STS_DEBUG(A) LOG4CXX_DEBUG(dc_sts_logger, A)
#define UBP_STS_INFO(A) LOG4CXX_INFO(dc_sts_logger, A)
#define UBP_STS_WARN(A) LOG4CXX_WARN(dc_sts_logger, A)
#define UBP_STS_ERROR(A) LOG4CXX_ERROR(dc_sts_logger, A)
#define UBP_STS_FATAL(A) LOG4CXX_FATAL(dc_sts_logger, A)
#define UBP_STS_DEBUG3(RES,OP,MSG) LOG4CXX_DEBUG(dc_sts_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_STS_INFO3(RES,OP,MSG) LOG4CXX_INFO(dc_sts_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_STS_ERROR3(RES,OP,MSG) LOG4CXX_ERROR(dc_sts_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)

//define proxyserver version
static log4cxx::LoggerPtr dc_proxy_logger(log4cxx::Logger::getLogger("DC.PROXY"));
#define UBP_PROXY_SERVER_ERROR(A) LOG4CXX_ERROR(dc_proxy_logger, "===PROXY_SERVER <" << A )
#define UBP_PROXY_SERVER_INFO(A) LOG4CXX_INFO(dc_proxy_logger, "===PROXY_SERVER <" << A )
#define UBP_PROXY_SERVER_WARN(A) LOG4CXX_WARN(dc_proxy_logger, "===PROXY_SERVER <" << A )
#define UBP_PROXY_SERVER_DEBUG(A) LOG4CXX_DEBUG(dc_proxy_logger, "===PROXY_SERVER <" << A )


//检查CONDITION是否为true,如果true，记录错误信息并返回
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define GUI_CHECK_CONDITION_RETURN(CONDITION,ERRMSG) \
    CHECK_CONDITION_RETURN(UBP_GUI_ERROR,CONDITION,ERRMSG)

//检查CONDITION是否为true,如果true，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define GUI_CHECK_CONDITION_RETURN_FAIL(CONDITION,ERRMSG) \
    CHECK_CONDITION_RETURN_FAIL(UBP_GUI_ERROR,CONDITION,ERRMSG)

//检查CONDITION是否为true,如果true，记录错误码信息并返回错误码
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERROR_CODE-失败返回的错误码
#define GUI_CHECK_CONDITION_RETURN_ERRCODE(CONDITION,ERROR_CODE) \
    CHECK_CONDITION_RETURN_ERRCODE(UBP_GUI_ERROR,CONDITION,ERROR_CODE)

//检查CONDITION是否为true,如果true，记录错误信息并返回NULL
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define GUI_CHECK_CONDITION_RETURN_NULL(CONDITION,ERROR_CODE) \
    CHECK_CONDITION_RETURN_NULL(UBP_GUI_ERROR,CONDITION,ERROR_CODE)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define GUI_CHECK_VAL_RETURN(CHECK_VAL,ERRMSG) \
    CHECK_VAL_RETURN(UBP_GUI_ERROR,CHECK_VAL,ERRMSG)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define GUI_CHECK_VAL_RETURN_FAIL(CHECK_VAL,ERRMSG)\
    CHECK_VAL_RETURN_FAIL(UBP_GUI_ERROR,CHECK_VAL,ERRMSG)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回NULL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define GUI_CHECK_VAL_RETURN_NULL(CHECK_VAL,ERRMSG) \
    CHECK_VAL_RETURN_NULL(UBP_GUI_ERROR,CHECK_VAL,ERRMSG)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误码信息并返回错误码
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERROR_CODE-失败返回的错误码
#define GUI_CHECK_VAL_RETURN_ERRCODE(CHECK_VAL,ERRMSG) \
    CHECK_VAL_RETURN_ERRCODE(UBP_GUI_ERROR,CHECK_VAL,ERRMSG)

//检查指针是否空，如果空，记录错误信息并返回
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define GUI_CHECK_NULL_RETURN(PTR,ERRMSG) \
    CHECK_NULL_RETURN(UBP_GUI_ERROR,PTR,ERRMSG)

//检查指针是否空，如果空，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define GUI_CHECK_NULL_RETURN_FAIL(PTR,ERRMSG) \
    CHECK_NULL_RETURN_FAIL(UBP_GUI_ERROR,PTR,ERRMSG)

//检查指针是否空，如果空，记录错误信息并返回NULL
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define GUI_CHECK_NULL_RETURN_NULL(PTR,ERRMSG) \
    CHECK_NULL_RETURN_NULL(UBP_GUI_ERROR,PTR,ERRMSG)

//检查指针是否空，如果空，记录错误码信息并返回错误码
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define GUI_CHECK_NULL_RETURN_ERRCODE(PTR,ERROR_CODE) \
    CHECK_NULL_RETURN_ERRCODE(UBP_GUI_ERROR,PTR,ERROR_CODE)

#define GUI_PRINT_MSG(MsgDebugStr) \
    do { UBP_GUI_DEBUG("++++++ print msg ++++++"); \
         UBP_GUI_DEBUG(MsgDebugStr); \
    }while(0)
/*****   DC.GUI  end  ******/


/*****   DC.SDK  begin  ******/
static log4cxx::LoggerPtr dc_sdk_logger(log4cxx::Logger::getLogger("DC.SDK"));
#define UBP_SDK_DEBUG(A) LOG4CXX_DEBUG(dc_sdk_logger, A)
#define UBP_SDK_INFO(A) LOG4CXX_INFO(dc_sdk_logger, A)
#define UBP_SDK_WARN(A) LOG4CXX_WARN(dc_sdk_logger, A)
#define UBP_SDK_ERROR(A) LOG4CXX_ERROR(dc_sdk_logger, A)
#define UBP_SDK_FATAL(A) LOG4CXX_FATAL(dc_sdk_logger, A)
#define UBP_SDK_INFO3(RES,OP,MSG) LOG4CXX_INFO(dc_sdk_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_SDK_DEBUG3(RES,OP,MSG) LOG4CXX_DEBUG(dc_sdk_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_SDK_ERROR3(RES,OP,MSG) LOG4CXX_ERROR(dc_sdk_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_SDK_WARN3(RES,OP,MSG) LOG4CXX_WARN(dc_sdk_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)

//检查CONDITION是否为true,如果true，记录错误信息并返回
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define SDK_CHECK_CONDITION_RETURN(CONDITION,ERRMSG) \
    CHECK_CONDITION_RETURN(UBP_SDK_ERROR,CONDITION,ERRMSG)

//检查CONDITION是否为true,如果true，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define SDK_CHECK_CONDITION_RETURN_FAIL(CONDITION,ERRMSG) \
    CHECK_CONDITION_RETURN_FAIL(UBP_SDK_ERROR,CONDITION,ERRMSG)

//检查CONDITION是否为true,如果true，记录错误码信息并返回错误码
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERROR_CODE-失败返回的错误码
#define SDK_CHECK_CONDITION_RETURN_ERRCODE(CONDITION,ERROR_CODE) \
    CHECK_CONDITION_RETURN_ERRCODE(UBP_SDK_ERROR,CONDITION,ERROR_CODE)

//检查CONDITION是否为true,如果true，记录错误信息并返回NULL
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define SDK_CHECK_CONDITION_RETURN_NULL(CONDITION,ERROR_CODE) \
    CHECK_CONDITION_RETURN_NULL(UBP_SDK_ERROR,CONDITION,ERROR_CODE)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define SDK_CHECK_VAL_RETURN(CHECK_VAL,ERRMSG) \
    CHECK_VAL_RETURN(UBP_SDK_ERROR,CHECK_VAL,ERRMSG)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define SDK_CHECK_VAL_RETURN_FAIL(CHECK_VAL,ERRMSG)\
    CHECK_VAL_RETURN_FAIL(UBP_SDK_ERROR,CHECK_VAL,ERRMSG)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回NULL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define SDK_CHECK_VAL_RETURN_NULL(CHECK_VAL,ERRMSG) \
    CHECK_VAL_RETURN_NULL(UBP_SDK_ERROR,CHECK_VAL,ERRMSG)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误码信息并返回错误码
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERROR_CODE-失败返回的错误码
#define SDK_CHECK_VAL_RETURN_ERRCODE(CHECK_VAL,ERRMSG) \
    CHECK_VAL_RETURN_ERRCODE(UBP_SDK_ERROR,CHECK_VAL,ERRMSG)

//检查指针是否空，如果空，记录错误信息并返回
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define SDK_CHECK_NULL_RETURN(PTR,ERRMSG) \
    CHECK_NULL_RETURN(UBP_SDK_ERROR,PTR,ERRMSG)

//检查指针是否空，如果空，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define SDK_CHECK_NULL_RETURN_FAIL(PTR,ERRMSG) \
    CHECK_NULL_RETURN_FAIL(UBP_SDK_ERROR,PTR,ERRMSG)

//检查指针是否空，如果空，记录错误信息并返回NULL
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define SDK_CHECK_NULL_RETURN_NULL(PTR,ERRMSG) \
    CHECK_NULL_RETURN_NULL(UBP_SDK_ERROR,PTR,ERRMSG)

//检查指针是否空，如果空，记录错误码信息并返回错误码
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define SDK_CHECK_NULL_RETURN_ERRCODE(PTR,ERROR_CODE) \
    CHECK_NULL_RETURN_ERRCODE(UBP_SDK_ERROR,PTR,ERROR_CODE)

#define SDK_PRINT_MSG(MsgDebugStr) \
    do { UBP_SDK_DEBUG("++++++ print msg ++++++"); \
         UBP_SDK_DEBUG(MsgDebugStr); \
    }while(0)
/*****   DC.SDK  end  ******/


/*****   DC.DATA    ******/
static log4cxx::LoggerPtr dc_data_logger(log4cxx::Logger::getLogger("DC.DATA"));
#define UBP_DC_DATALOG(RES,OP,MSG) LOG4CXX_DEBUG(dc_data_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)

/*****   DC.APP  begin  ******/
static log4cxx::LoggerPtr dc_app_logger(log4cxx::Logger::getLogger("DC.APP"));
#define UBP_APP_DEBUG(A) LOG4CXX_DEBUG(dc_app_logger, A)
#define UBP_APP_INFO(A) LOG4CXX_INFO(dc_app_logger, A)
#define UBP_APP_WARN(A) LOG4CXX_WARN(dc_app_logger, A)
#define UBP_APP_ERROR(A) LOG4CXX_ERROR(dc_app_logger, A)
#define UBP_APP_FATAL(A) LOG4CXX_FATAL(dc_app_logger, A)
#define UBP_APP_INFO3(RES,OP,MSG) LOG4CXX_INFO(dc_app_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_APP_DEBUG3(RES,OP,MSG) LOG4CXX_DEBUG(dc_app_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_APP_ERROR3(RES,OP,MSG) LOG4CXX_ERROR(dc_app_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)
#define UBP_APP_WARN3(RES,OP,MSG) LOG4CXX_WARN(dc_app_logger, "<ISDN:" << RES << "," <<OP<<">" <<MSG)

//检查CONDITION是否为true,如果true，记录错误信息并返回
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define APP_CHECK_CONDITION_RETURN(CONDITION,ERRMSG) \
    CHECK_CONDITION_RETURN(UBP_APP_ERROR,CONDITION,ERRMSG)

//检查CONDITION是否为true,如果true，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define APP_CHECK_CONDITION_RETURN_FAIL(CONDITION,ERRMSG) \
    CHECK_CONDITION_RETURN_FAIL(UBP_APP_ERROR,CONDITION,ERRMSG)

//检查CONDITION是否为true,如果true，记录错误码信息并返回错误码
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERROR_CODE-失败返回的错误码
#define APP_CHECK_CONDITION_RETURN_ERRCODE(CONDITION,ERROR_CODE) \
    CHECK_CONDITION_RETURN_ERRCODE(UBP_APP_ERROR,CONDITION,ERROR_CODE)

//检查CONDITION是否为true,如果true，记录错误信息并返回NULL
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define APP_CHECK_CONDITION_RETURN_NULL(CONDITION,ERROR_CODE) \
    CHECK_CONDITION_RETURN_NULL(UBP_APP_ERROR,CONDITION,ERROR_CODE)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define APP_CHECK_VAL_RETURN(CHECK_VAL,ERRMSG) \
    CHECK_VAL_RETURN(UBP_APP_ERROR,CHECK_VAL,ERRMSG)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define APP_CHECK_VAL_RETURN_FAIL(CHECK_VAL,ERRMSG)\
    CHECK_VAL_RETURN_FAIL(UBP_APP_ERROR,CHECK_VAL,ERRMSG)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回NULL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define APP_CHECK_VAL_RETURN_NULL(CHECK_VAL,ERRMSG) \
    CHECK_VAL_RETURN_NULL(UBP_APP_ERROR,CHECK_VAL,ERRMSG)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误码信息并返回错误码
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERROR_CODE-失败返回的错误码
#define APP_CHECK_VAL_RETURN_ERRCODE(CHECK_VAL,ERRMSG) \
    CHECK_VAL_RETURN_ERRCODE(UBP_APP_ERROR,CHECK_VAL,ERRMSG)

//检查指针是否空，如果空，记录错误信息并返回
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define APP_CHECK_NULL_RETURN(PTR,ERRMSG) \
    CHECK_NULL_RETURN(UBP_APP_ERROR,PTR,ERRMSG)

//检查指针是否空，如果空，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define APP_CHECK_NULL_RETURN_FAIL(PTR,ERRMSG) \
    CHECK_NULL_RETURN_FAIL(UBP_APP_ERROR,PTR,ERRMSG)

//检查指针是否空，如果空，记录错误信息并返回NULL
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define APP_CHECK_NULL_RETURN_NULL(PTR,ERRMSG) \
    CHECK_NULL_RETURN_NULL(UBP_APP_ERROR,PTR,ERRMSG)

//检查指针是否空，如果空，记录错误码信息并返回错误码
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define APP_CHECK_NULL_RETURN_ERRCODE(PTR,ERROR_CODE) \
    CHECK_NULL_RETURN_ERRCODE(UBP_APP_ERROR,PTR,ERROR_CODE)

#define APP_PRINT_MSG(MsgDebugStr) \
    do { UBP_APP_DEBUG("++++++ print msg ++++++"); \
         UBP_APP_DEBUG(MsgDebugStr); \
    }while(0)

/*****   DC.APP  end  ******/

#define INJECT_EXCEPTION \
    int* eee=NULL; (*eee)++;


/************************** DC end   *******************************************/

//BELOW ARE SOME MACROS FOR SECURE PROGRAMMING, USE MICROSOFT VERSION ON WINDOWS, AND USE SELF-MADE VERSION ON LINUX
//MACRO FOR SNPRINTF: IN WINDOWS WE USE _TRUNCATE FOR MAXCOUNT PARAMETER
#ifdef WIN32
    #define UBP_SAFE_SNPRINTF_S(BUFFER,SIZE,FORMAT,...)\
    _snprintf_s(BUFFER,SIZE,_TRUNCATE,FORMAT,##__VA_ARGS__);
#else
    #define UBP_SAFE_SNPRINTF_S(BUFFER,SIZE,FORMAT,...)\
    snprintf_s(BUFFER,SIZE,SIZE,FORMAT,##__VA_ARGS__);
#endif

//MACRO FOR MEMSET: USE SECUREZEROMEMORY IN WINDOWS, MAKE SURE YOU SET THE MEMORY TO ZERO!
//#ifdef WIN32
//    #define UBP_SAFE_MEMSET_S(BUFFER,CHAR,SIZE)\
//    SecureZeroMemory(BUFFER,SIZE);
//#else
    #define UBP_SAFE_MEMSET_S(BUFFER,CHAR,SIZE)\
    memset_s(BUFFER,SIZE,CHAR,SIZE);
//#endif

//MACRO FOR STRNCPY, MAKE SURE YOU USE SIZEOF(BUFFER) OR LENGTH(SRC)+1 FOR THE SIZE PARAMETER, NOT LENGTH(SRC)!
#ifdef WIN32
    #define UBP_SAFE_STRNCPY_S(BUFFER,SRC,SIZE)\
    strncpy_s(BUFFER,SIZE,SRC,SIZE);
#else
    #define UBP_SAFE_STRNCPY_S(BUFFER,SRC,SIZE)\
    strncpy_s(BUFFER,SIZE,SRC,SIZE);
#endif

#ifdef WIN32
    #define UBP_SAFE_STRNCAT_S(BUFFER,SRC,SIZE)\
    strncat_s(BUFFER,SIZE,SRC,SIZE);
#else
    #define UBP_SAFE_STRNCAT_S(BUFFER,SRC,SIZE)\
    strncat_s(BUFFER,SIZE,SRC,SIZE);
#endif

#ifdef WIN32
    #define UBP_SAFE_MEMCPY_S(BUFFER,SRC,SIZE)\
    memcpy_s(BUFFER,SIZE,SRC,SIZE);
#else
    #define UBP_SAFE_MEMCPY_S(BUFFER,SRC,SIZE)\
    memcpy_s(BUFFER,SIZE,SRC,SIZE);
#endif

#define UBP_SAFE_MEMMOVE_S(DSTBUF,DSTSIZE,SRCBUF,CPYSIZE)\
    memmove_s(DSTBUF,DSTSIZE,SRCBUF,CPYSIZE);

/*
#ifdef __cplusplus
extern "C"
{
#endif
inline int UBP_SAFE_SNPRINTF_S (char *buffer, size_t maxlen, const char *format, ... )
{
    int result;
    va_list ap;
    va_start (ap, format);
# if defined (WIN32)
    result =   _snprintf_s (buffer, maxlen, _TRUNCATE,format,ap);
# else
    result =  snprintf_s (buffer, maxlen, format, ap);
#endif
    va_end (ap);
    return result;
}
#ifdef __cplusplus
}
#endif

*/

#endif // LOGGER_H
