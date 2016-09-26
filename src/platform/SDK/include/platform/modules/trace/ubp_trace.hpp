/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      ubp_trace.hpp
Author:        XiaoLongguang
Created:       2012/07/5 9:23
Description:
*********************************************************************/

#ifndef  PLATFORM_MODULES_TRACE_UBP_TRACE_HPP_
#define  PLATFORM_MODULES_TRACE_UBP_TRACE_HPP_

#include "ace/Null_Mutex.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Singleton.h"
#include "log4cxx/logger.h"
#include "platform/modules/util/no_copy.hpp"

namespace ubp
{
namespace platform
{
namespace trace
{
class TraceObject;
}
}
};

#define UBP_FUNC_TRACE(MODULE,FUNC) ubp::platform::trace::TraceObject obj(MODULE,FUNC)
#define UBP_FUNC_INFO(MODULE,FUNC) ubp::platform::trace::TraceObject obj(MODULE,FUNC,false)

#define UBP_LOGGER_DEBUG(MODULE,LOG) LOG4CXX_DEBUG(log4cxx::Logger::getLogger(MODULE),LOG)
#define UBP_LOGGER_INFO(MODULE,LOG) LOG4CXX_INFO(log4cxx::Logger::getLogger(MODULE),LOG)
#define UBP_LOGGER_WARN(MODULE,LOG) LOG4CXX_WARN(log4cxx::Logger::getLogger(MODULE),LOG)
#define UBP_LOGGER_ERROR(MODULE,LOG) LOG4CXX_ERROR(log4cxx::Logger::getLogger(MODULE),LOG)
#define UBP_LOGGER_FATAL(MODULE,LOG) LOG4CXX_FATAL(log4cxx::Logger::getLogger(MODULE),LOG)

/*****  COMMON    ******/
#define UBP_DEBUG(A) LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), A)
#define UBP_INFO(A) LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), A)
#define UBP_WARN(A) LOG4CXX_WARN(log4cxx::Logger::getRootLogger(), A)
#define UBP_ERROR(A) LOG4CXX_ERROR(log4cxx::Logger::getRootLogger(), A)
#define UBP_FATAL(A) LOG4CXX_FATAL(log4cxx::Logger::getRootLogger(), A)

#ifdef WIN32
#ifdef UBP_BUILD_PLATFORM_TRACE_DLL
#define TRACE_IMPORT_EXPORT __declspec(dllexport)
#define TRACE_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
  template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#else
#define TRACE_IMPORT_EXPORT __declspec(dllimport)
#define TRACE_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
  extern template class SINGLETON_TYPE <CLASS, LOCK>;
#endif
#else
#define TRACE_IMPORT_EXPORT
#define TRACE_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif

namespace ubp
{
namespace platform
{
namespace trace
{
class TRACE_IMPORT_EXPORT UBPTrace : private ubp::platform::util::NonCopyable
{
    friend class ACE_Singleton<UBPTrace, ACE_Null_Mutex>;

  public:
    UBPTrace(void);
    ~UBPTrace(void);
    ACE_INT32 Init(const char *logname = NULL);

    //ACE_INT32 SetConsole(bool need_console);
    //ACE_INT32 SetLevel(const char *level);
    //ACE_INT32 SetMaxFileSize(const char *file_size = "20000KB");
    //ACE_INT32 SetMaxBackupIndex(int max_backup_index = 20);

  private:
    std::string log_name_;
    char env_process_name_[64];
    char env_process_id_[64];
};

//////////////////////////////////////////////////////////////////////////

class TraceObject
{
  public:
    TraceObject(const char *logger_name, const char *obj_name, bool debugLevel = true)
      : logger_name_(logger_name), object_name_(obj_name), debugLevel_(debugLevel)
    {
      if(debugLevel_)
      {
        LOG4CXX_DEBUG(log4cxx::Logger::getLogger(logger_name_),
                      object_name_ << " Entered...")
      }
      else
      {
        LOG4CXX_INFO(log4cxx::Logger::getLogger(logger_name_),
                     object_name_ << " Entered...")
      }
    }

    ~TraceObject()
    {
      if(debugLevel_)
      {
        LOG4CXX_DEBUG(log4cxx::Logger::getLogger(logger_name_),
                      object_name_ << " Exited...")
      }
      else
      {
        LOG4CXX_INFO(log4cxx::Logger::getLogger(logger_name_),
                     object_name_ << " Exited...")
      }
    }
  private:
    ::std::string logger_name_;
    ::std::string object_name_;
    bool debugLevel_;
};

//////////////////////////////////////////////////////////////////////////

typedef ACE_Unmanaged_Singleton<UBPTrace, ACE_Null_Mutex> UBPTrace_T;
TRACE_IMPORT_EXPORT_SINGLETON_DECLARE(
  ACE_Unmanaged_Singleton, UBPTrace, ACE_Null_Mutex)


} //namespace trace
} //namespace platform
} //namespace ubp



#endif //PLATFORM_MODULES_TRACE_UBP_TRACE_HPP_
