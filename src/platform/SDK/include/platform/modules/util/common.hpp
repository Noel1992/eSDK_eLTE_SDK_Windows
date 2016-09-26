/********************************************************************
	Copyright 2012,TD-Tech. Co., Ltd.
	Filename: 	   common.hpp
	Author:		   Wu Gang
    Created:       2012/07/11 10:10
	Description: 
*********************************************************************/
#ifndef UBP_PLATFORM_UTIL_COMMON_HPP_ 
#define UBP_PLATFORM_UTIL_COMMON_HPP_

#include "ace/Thread_Mutex.h"
#include "ace/Atomic_Op_T.h"

typedef ACE_Atomic_Op<ACE_Recursive_Thread_Mutex, ACE_INT32> SafeInt;
typedef ACE_Atomic_Op<ACE_Recursive_Thread_Mutex, bool> SafeBool;

#endif //UBP_PLATFORM_UTIL_COMMON_HPP_