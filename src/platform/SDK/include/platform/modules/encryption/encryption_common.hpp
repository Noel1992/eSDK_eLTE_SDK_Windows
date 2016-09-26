/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      encryption_common.hpp
Author:        XiaoLongguang
Created:       2012/11/24 09:01
Description:
*********************************************************************/

#ifndef UBP_PLATFORM_MODULES_ENCRYPTION_COMMON_HPP_
#define UBP_PLATFORM_MODULES_ENCRYPTION_COMMON_HPP_

#include "platform/modules/trace/ubp_trace.hpp"
// 定义此宏是为了解决WINDOWS平台单实例类对象在dll和exe中并非同一实例的bug 
// dll中需要export，使用时需要import 

#ifdef WIN32
#ifdef ENCRYPTION_EXPORT
#define ENCRYPTION_IMPORT_EXPORT __declspec(dllexport)
#else//NOT ENCRYPTION_EXPORT
#define ENCRYPTION_IMPORT_EXPORT __declspec(dllimport)
#endif//ENCRYPTION_EXPORT
#else//NOT WIN32
#define ENCRYPTION_IMPORT_EXPORT
#endif//WIN32

#define ENCRYPTION_FUN_TRACE(FUNC) UBP_FUNC_TRACE("PLATFORM.ENCRYPTION",FUNC)
#define ENCRYPTION_FUN_INFO(FUNC) UBP_FUNC_INFO("PLATFORM.ENCRYPTION",FUNC)
#define UBP_ENCRYPTION_DEBUG(LOG) UBP_LOGGER_DEBUG("PLATFORM.ENCRYPTION", LOG)
#define UBP_ENCRYPTION_INFO(LOG) UBP_LOGGER_INFO("PLATFORM.ENCRYPTION", LOG)
#define UBP_ENCRYPTION_WARN(LOG) UBP_LOGGER_WARN("PLATFORM.ENCRYPTION", LOG)
#define UBP_ENCRYPTION_ERROR_LOG(LOG) UBP_LOGGER_ERROR("PLATFORM.ENCRYPTION", LOG)
#define UBP_ENCRYPTION_FATAL(LOG) UBP_LOGGER_FATAL("PLATFORM.ENCRYPTION", LOG)


#endif  //UBP_PLATFORM_MODULES_ENCRYPTION_COMMON_HPP_
