/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    id_generate.hpp
  Author:      Wu Gang
  Created:     2013/03/4 11:37
  Description:

*********************************************************************/
#ifndef UBP_PLATFORM_UTIL_ID_GENERATE_HPP_
#define UBP_PLATFORM_UTIL_ID_GENERATE_HPP_

#include "ace/Basic_Types.h"
#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "platform_util_def.hpp"

namespace ubp
{
namespace platform
{
namespace util
{

class UBP_PLATFORM_UTIL_IMPORT_EXPORT IDGenerate
{
  public:
    IDGenerate(ACE_UINT64 workerId, ACE_UINT64 datacenterId);

    virtual ~IDGenerate();

    ACE_INT32 Init();

    ACE_UINT64 GetNextId();

  protected:
    ACE_UINT64 TimeGen() ;

    void SequenceGen();

    ACE_UINT64 TillNextMillis(ACE_UINT64 lastTimestamp);

  private:
    ACE_UINT64 workerId_;
    ACE_UINT64 datacenterId_;
    ACE_UINT64 sequence_;
    ACE_UINT64 lastTimestamp_;
    ACE_UINT64 lastId_;
    ACE_Recursive_Thread_Mutex rwMutex_;
};

}//namespace util
}//namespace platform
}//namespace ubp

#endif // UBP_PLATFORM_UTIL_ID_GENERATE_HPP_