/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:   util_trace.hpp
  Author:     Wu Gang
  Created:    2012/09/09 14:21
  Description:

*********************************************************************/

#ifndef UBP_PLATFORM_MOUDLES_UTIL_TRACE_HPP_
#define UBP_PLATFORM_MOUDLES_UTIL_TRACE_HPP_

#include "platform/modules/trace/ubp_trace.hpp"
#include "platform/modules/error/ubp_error.hpp"

#define UTIL_DEBUG(LOG) UBP_LOGGER_DEBUG("PLATFORM.UTIL", LOG)
#define UTIL_INFO(LOG) UBP_LOGGER_INFO("PLATFORM.UTIL", LOG)
#define UTIL_WARN(LOG)  UBP_LOGGER_WARN("PLATFORM.UTIL", LOG)
#define UTIL_ERROR(LOG)  UBP_LOGGER_ERROR("PLATFORM.UTIL", LOG)
#define UTIL_FATAL(LOG)  UBP_LOGGER_FATAL("PLATFORM.UTIL", LOG)

#endif //UBP_PLATFORM_MOUDLES_UTIL_TRACE_HPP_

