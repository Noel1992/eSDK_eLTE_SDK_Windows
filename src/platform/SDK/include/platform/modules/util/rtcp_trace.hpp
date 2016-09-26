/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    
  Author:      
  Created:     
  Description:

*********************************************************************/
#ifndef    UBP_PLATFORM_RTCP_TRACE_HPP_
#define    UBP_PLATFORM_RTCP_TRACE_HPP_

#include <stdio.h>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include "boost/smart_ptr/detail/spinlock.hpp"
#include <boost/thread/recursive_mutex.hpp>

//linux使用c++11
#if !defined(WIN32)
#include <atomic>
typedef std::atomic_long rtp_atomic_long;
#else
typedef long rtp_atomic_long;
#endif

#include <ace/Basic_Types.h>
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Singleton.h"
#include "log4cxx/logger.h"
#include "platform/modules/util/no_copy.hpp"

#ifdef WIN32
#ifdef UBP_RTCP_EXPORT
#define UBP_RTCP_TRACE_IMPORT_EXPORT __declspec(dllexport)
#else//NOT UBP_RTCP__EXPORT
#define UBP_RTCP_TRACE_IMPORT_EXPORT __declspec(dllimport)
#endif//UBP_RTCP_EXPORT
#else//NOT WIN32
#define UBP_RTCP_TRACE_IMPORT_EXPORT
#endif//WIN32


namespace ubp
{
namespace platform
{
namespace util
{
////info:const std::vector<std::pair<std::string, std::string> > &aInfo
//pair.first是字段名,pair.second是字段值
//aInfo[0]是文件名,first固定填"file_name", second填文件名
#define RTCP_INIT(fileinfos) \
  do \
  { \
    RTCPTraceAdp::instance().InitFileInfos(fileinfos); \
  }while(0);

#define RTCP_TRACE(info) \
  do \
  { \
    RTCPTraceAdp::instance().Trace(info); \
  }while(0);

struct t_rtcp_conf
{
  rtp_atomic_long switch_;
};

class UBP_RTCP_TRACE_IMPORT_EXPORT RTCPTraceAdp : private ubp::platform::util::NonCopyable
{
private:
  RTCPTraceAdp();
  ACE_INT32 LoadCfg();
public:
  static RTCPTraceAdp &instance();
  ACE_INT32 InitFileInfos(const std::vector<std::pair<std::string, std::string> > &aFileInfo);
  ACE_INT32 InitLogger(const std::string &aFileName);
  std::string GetTime();
  void ExtendBuf(int& bufsize ,int bufindex, int valuesize,std::vector<char>&buf);
  ACE_INT32 Trace(const std::vector<std::pair<std::string, std::string> > &aInfo);

  void Run();
  virtual ~RTCPTraceAdp();
private:
  std::string rtcp_path_;
  std::map<std::string, std::string> cfg_temp_;
  std::map<std::string, std::string> cfg_;
  std::map<std::string, bool> file_init_flag;
  boost::recursive_mutex lock_logger_;
  t_rtcp_conf varia_cfg;
  boost::thread thread_;
  boost::recursive_mutex cfg_mutex_;
  rtp_atomic_long running_;
};

//////////////////////////////////////////////////////////////////////////

//typedef ACE_Unmanaged_Singleton<RTCPTraceEx, ACE_Null_Mutex> RTCPTrace_T;
//RTCP_TRACE_IMPORT_EXPORT_SINGLETON_DECLARE(
//  ACE_Unmanaged_Singleton, RTCPTraceEx, ACE_Null_Mutex)

}//namespace util
}//namespace platform
}//namespace ubp

#endif // UBP_PLATFORM_RTCP_TRACE_HPP_
