/*********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    ubp_assert.hpp
  Author:      wugang wwx21870
  Created:     2013/12/5 20:11
  Description: assert只在DEVELOP版本有效

*********************************************************************/

#ifndef UBP_ASSERT_HPP_
#define UBP_ASSERT_HPP_

#ifdef EAPP_DEVELOP
#include <assert.h>
#define UBP_ASSERT(A) assert(A)
#else
#include <iostream>
//#include "platform/modules/trace/ubp_trace.hpp"
//#define UBP_ASSERT_ERROR(LOG) UBP_LOGGER_ERROR("UBP_ASSERT", LOG)
#define UBP_ASSERT(A) \
{ \
	if ( !(A) ) \
	{ \
		std::cerr << "UBP_ASSERT " << #A << " File:" << __FILE__ << " Line:" << __LINE__ << std::endl; \
	} \
}
#endif

#endif //UBP_ASSERT_HPP_
