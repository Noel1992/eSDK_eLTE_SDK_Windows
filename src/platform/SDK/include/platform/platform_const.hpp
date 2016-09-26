/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    platform_const.hpp
  Author:      Wu Gang
  Created:     2012/07/24 14:41
  Description: 定义平台的常量

*********************************************************************/

#ifndef UBP_PLATFORM_CONST_HPP__
#define UBP_PLATFORM_CONST_HPP__

/********************************************************************
   MODULE_ID范围定义:
   平台： 0 ~ 999；
   BCC : 1000~1999;
   BDC : 2000~2999;
*********************************************************************/
static const ACE_UINT32 MODULE_ID_UBP_PLATFORM_MQ = 1;
static const ACE_UINT32 MODULE_ID_UBP_PLATFORM_TRACE = 2;

/********************************************************************
   MODULE_Name定义:按照命名空间的约定命名
*********************************************************************/
static const char *MODULE_NAME_UBP_PLATFORM_MQ = "UBP.PLATFORM.MQ";
static const char *MODULE_NAME_UBP_PLATFORM_TRACE = "UBP.PLATFORM.TRACE";

/********************************************************************
   Message_ID范围定义:
   平台： 0 ~ 9999
   BCC : 10000 ~19999
   BDC : 20000 ~29999
*********************************************************************/
static const ACE_UINT32 MESSAGE_ID_UBP_PLATFORM_TEST = 0;

#endif // UBP_PLATFORM_CONST_HPP__