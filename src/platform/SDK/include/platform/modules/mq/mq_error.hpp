/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    mq_error.hpp
Author:      Wu Gang
Created:     2012/08/21 10:08
Description:
*********************************************************************/
#ifndef  UBP_PLATFORM_MODULES_ERROR_MQ_ERROR_HPP_
#define  UBP_PLATFORM_MODULES_ERROR_MQ_ERROR_HPP_

#include "platform/modules/error/ubp_error.hpp"

///////////////////////////////////////////////////////////////////////
//platform：-1000 ~ - 9999 
//  |--MQ     : -1000 ~ -1099
//  |--SCA    : -1100 ~ -1199
//  |--FRM    : -1200 ~ -1299
//  |--Thread : -1300 ~ -1399
//  |--DAO    : -1400 ~ -2999
//For example : 
//static const ACE_INT32 ERR_UBP_MQ_XXX1 = -1000;
///////////////////////////////////////////////////////////////////////

//MQ:-1000 ~ -1099
static const ACE_INT32 ERR_UBP_MQ_INIT = -1000;
static const ACE_INT32 ERR_UBP_MQ_DISCONNECT = -1001;
static const ACE_INT32 ERR_UBP_MQ_REG_REPONSER = -1002;  //注册Reponser错误
static const ACE_INT32 ERR_UBP_MQ_SEND_MSG = -1003;  //send message error

static const ACE_INT32 ERR_UBP_MQ_INIT_ASYNSOCKET = -1004;  //init asyn socket error
static const ACE_INT32 ERR_UBP_MQ_CONSUMER_CONNECT = -1005;  //consumer连接失败
static const ACE_INT32 ERR_UBP_MQ_CONSUMER_SUBSCRIBE = -1006;  //consumer订阅失败
static const ACE_INT32 ERR_UBP_MQ_INIT_LOCALSVC = -1007;  //consumer订阅失败
static const ACE_INT32 ERR_UBP_MQ_INIT_REPSPONSE = -1008;  //Init reponse server error
static const ACE_INT32 ERR_UBP_MQ_INIT_DEALER = -1009;  //Init dealer server error
static const ACE_INT32 ERR_UBP_MQ_INIT_PUBLISHER = -1010;  //Init publisher server error
static const ACE_INT32 ERR_UBP_MQ_GET_DESTSVC = -1011;  //Get dest svc info error
static const ACE_INT32 ERR_UBP_MQ_INIT_CONSUMER = -1012;//Init consumer error


#endif //UBP_PLATFORM_MODULES_ERROR_MQ_ERROR_HPP_
