///////////////////////////////////////////////////////////////////////
//Copyright 2012,TD-Tech. Co., Ltd.
//Filename:    ubp_error.hpp
//Author:      Wu Gang
//Created:     2012/08/21 10:08
//Description:
///////////////////////////////////////////////////////////////////////
#ifndef  UBP_PLATFORM_MODULES_ERROR_UBP_ERROR_HPP_
#define  UBP_PLATFORM_MODULES_ERROR_UBP_ERROR_HPP_

#include "ace/Basic_Types.h"
#include "platform/modules/sca/ubp_svc_env.hpp"
#include "error_module.hpp"

///////////////////////////////////////////////////////////////////////
// UBP全局错误码定义范围如下：
// Common  : 0     ~ -999
// Platform:-1000  ~ -9999   
// OM      :-10000 ~ -19999   
// BCC     :-20000 ~ -29999
// BDC     :-30000 ~ -39999
// DC      :-40000 ~ -49999
// MRS     :-50000 ~ -59999
// VGW     :-60000 ~ -69999
// SDS     :-70000 ~ -79999
///////////////////////////////////////////////////////////////////////

//各个子系统分配如下：
//1、Platform（-1000 ~ - 9999）
//    |--MQ    : -1000 ~ -1099
//    |--SCA   : -1100 ~ -1199
//    |--FRM   : -1200 ~ -1299
//    |--Thread: -1300 ~ -1399
//    |--DAO   : -1400 ~ -2999

//2、OM（-10000 ~ -19999）
//    |--CM : -10000 ~ -10099
//    |--FM : -10100 ~ -10199
//    |--LM : -10200 ~ -10299
//    |--MON :-10300 ~ -10399
//    |--LIC :-10400 ~ -10499

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
//For example : 
//static const ACE_INT32 ERR_UBP_MQ_XXX1 = -1000;

//////////////////////////////////////////////////////////////////////////
//ErrorInfo for UBP common : 0 ~ -999
//////////////////////////////////////////////////////////////////////////
static const ACE_INT32 UBP_SUCCESS = 0;
static const ACE_INT32 UBP_RETRY = -1;
static const ACE_INT32 UBP_FAIL = -999;

//////////////////////////////////////////////////////////////////////////
//检查CONDITION是否为true,如果true，记录错误信息并返回
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define CHECK_CONDITION_RETURN(ERR_LOGGER,CONDITION,ERRMSG)\
    do{if(true==(CONDITION)){\
    ERR_LOGGER("Error:"<<ERRMSG); \
    return;}\
    } while (0)

//检查CONDITION是否为true,如果true，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define CHECK_CONDITION_RETURN_FAIL(ERR_LOGGER,CONDITION,ERRMSG)\
    do{if(true==(CONDITION)){\
    ERR_LOGGER("Error:"<<ERRMSG); \
    return UBP_FAIL;}\
    } while (0)

//检查CONDITION是否为true,如果true，记录错误信息并返回NULL
//LOGGER-Trace logger;CONDITION-要检查条件；ERRMSG-错误信息
#define CHECK_CONDITION_RETURN_NULL(ERR_LOGGER,CONDITION,ERRMSG)\
    do{if(true==(CONDITION)){\
    ERR_LOGGER("Error:"<<ERRMSG); \
    return NULL;}\
    } while (0)

//检查CONDITION是否为true,如果true，记录错误码信息并返回错误码
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERROR_CODE-失败返回的错误码
#define CHECK_CONDITION_RETURN_ERRCODE(ERR_LOGGER,CONDITION,ERROR_CODE)\
    do{if(true==(CONDITION)){\
    const ubp::platform::error::ErrorInfo* errordetail =\
    ubp::platform::frame::ModuleContext::CreateContext()->\
    ModuleRef<ubp::platform::error::ErrorModule>()->GetErrorInfo(ERROR_CODE);\
    if(NULL!=errordetail){\
    ERR_LOGGER("Error:"<< ERROR_CODE<< ","<<errordetail->message_);}\
    return ERROR_CODE;}\
    } while (0)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define CHECK_VAL_RETURN(ERR_LOGGER,CHECK_VAL,ERRMSG)\
    do{if(UBP_SUCCESS!=CHECK_VAL){\
    ERR_LOGGER("Error:"<<ERRMSG); \
    return;}\
    } while (0)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define CHECK_VAL_RETURN_FAIL(ERR_LOGGER,CHECK_VAL,ERRMSG)\
    do{if(UBP_SUCCESS!=CHECK_VAL){\
    ERR_LOGGER("Error:"<<ERRMSG); \
    return UBP_FAIL;}\
    } while (0)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误信息并返回NULL
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERRMSG-错误信息
#define CHECK_VAL_RETURN_NULL(ERR_LOGGER,CHECK_VAL,ERRMSG)\
    do{if(UBP_SUCCESS!=CHECK_VAL){\
    ERR_LOGGER("Error:"<<ERRMSG); \
    return NULL;}\
    } while (0)

//检查VAL是否等于UBP_SUCCESS，如果不是，记录错误码信息并返回错误码
//LOGGER-Trace logger;CHECK_VAL-要检查的值；ERROR_CODE-失败返回的错误码
#define CHECK_VAL_RETURN_ERRCODE(ERR_LOGGER,CHECK_VAL,ERROR_CODE)\
    do{if(UBP_SUCCESS!=CHECK_VAL){\
    const ubp::platform::error::ErrorInfo* errordetail =\
    ubp::platform::frame::ModuleContext::CreateContext()->\
    ModuleRef<ubp::platform::error::ErrorModule>()->GetErrorInfo(ERROR_CODE);\
    if(NULL!=errordetail){\
    ERR_LOGGER("Error:"<< ERROR_CODE<< ","<<errordetail->message_);}\
    return ERROR_CODE;}\
    } while (0)

//检查指针是否空，如果空，记录错误信息并返回
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define CHECK_NULL_RETURN(ERR_LOGGER,PTR,ERRMSG)\
    do{if(NULL==PTR){\
    ERR_LOGGER("Null pointer:"<<ERRMSG); \
    return;}\
    } while (0)

//检查指针是否空，如果空，记录错误信息并返回UBP_FAIL
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define CHECK_NULL_RETURN_FAIL(ERR_LOGGER,PTR,ERRMSG)\
    do{if(NULL==PTR){\
    ERR_LOGGER("Null pointer:"<<ERRMSG); \
    return UBP_FAIL;}\
    } while (0)

//检查指针是否空，如果空，记录错误信息并返回NULL
//LOGGER-Trace logger;PTR-要检查的指针；ERRMSG-错误信息
#define CHECK_NULL_RETURN_NULL(ERR_LOGGER,PTR,ERRMSG)\
    do{if(NULL==PTR){\
    ERR_LOGGER("Null pointer:"<<ERRMSG); \
    return NULL;}\
    } while (0)

//检查指针是否为NULL，如果是空指针，记录错误码信息并返回错误码
//LOGGER-Trace logger;PTR-要检查的指针;ERROR_CODE-失败返回的错误码
#define CHECK_NULL_RETURN_ERRCODE(ERR_LOGGER,PTR,ERROR_CODE) \
    do { if( NULL == PTR) { \
    const ubp::platform::error::ErrorInfo* errordetail =\
    ubp::platform::frame::ModuleContext::CreateContext()->\
    ModuleRef<ubp::platform::error::ErrorModule>()->GetErrorInfo(ERROR_CODE);\
    if(NULL!=errordetail){\
    ERR_LOGGER("Error:"<< ERROR_CODE<< ","<<errordetail->message_);}\
    return ERROR_CODE;}\
    } while (0)

#endif //UBP_PLATFORM_MODULES_ERROR_UBP_ERROR_HPP_
