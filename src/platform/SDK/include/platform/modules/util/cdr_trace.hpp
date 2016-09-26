/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    cdr_trace.hpp
Author:      lijing
Created:     2013/05/21 15:53
Description:

*********************************************************************/
#ifndef    UBP_PLATFORM_CDR_TRACE_HPP_
#define    UBP_PLATFORM_CDR_TRACE_HPP_

#include <stdio.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ace/Basic_Types.h>
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Singleton.h"
#include "log4cxx/logger.h"
#include "platform/modules/util/no_copy.hpp"


#ifdef WIN32
#ifdef UBP_CDR_EXPORT
#define UBP_CDR_TRACE_IMPORT_EXPORT __declspec(dllexport)
#else//NOT UBP_CDR__EXPORT
#define UBP_CDR_TRACE_IMPORT_EXPORT __declspec(dllimport)
#endif//UBP_CDR_EXPORT
#else//NOT WIN32
#define UBP_CDR_TRACE_IMPORT_EXPORT
#endif//WIN32


namespace ubp
{
	namespace platform
	{
		namespace util
		{
			class UBP_CDR_TRACE_IMPORT_EXPORT CDRTrace : private ubp::platform::util::NonCopyable
			{
			protected:
				CDRTrace();
			public:
				static CDRTrace &instance();
				virtual ACE_INT32 Trace(std::string NODEID,std::string SVCTYPE, std::string SESSIONID, std::string CALLER, std::string CALLEE, std::string GROUPID, std::string STARTTIME, std::string ENDTIME, std::string RESULT,std::string CAUSE)=0;
				virtual ~CDRTrace();
			private:
			};

#define CDR_TRACE(NODEID,SVCTYPE,SESSIONID,CALLER,CALLEE,STARTTIME,ENDTIME,RESULT,CAUSE) \
			do \
			{ \
					CDRTraceAdp::WriteTrace( NODEID,SVCTYPE,SESSIONID,CALLER,CALLEE,STARTTIME,ENDTIME,RESULT,CAUSE );\
			}while(0);

			class UBP_CDR_TRACE_IMPORT_EXPORT CDRTraceAdp : private ubp::platform::util::NonCopyable	
			{
			private:
				CDRTraceAdp();
				ACE_INT32 LoadCfg();
			public:
				static CDRTraceAdp * instance();    
				virtual ACE_INT32 Trace(std::string NODEID,std::string SVCTYPE, std::string SESSIONID, std::string CALLER, std::string CALLEE, std::string STARTTIME, std::string ENDTIME, std::string RESULT,std::string CAUSE);
				virtual ~CDRTraceAdp();
				/* The following is added to enable polling timer task*/
				static void WriteTrace( std::string NODEID,std::string SVCTYPE, std::string SESSIONID, std::string CALLER, std::string CALLEE, std::string STARTTIME, std::string ENDTIME, std::string RESULT,std::string CAUSE);
			protected:
				boost::posix_time::ptime start_time_;
				boost::posix_time::time_duration cycle_time_;
				boost::posix_time::time_period next_time_period_;
				boost::thread * thread_ptr_;
				static CDRTraceAdp * cdr_trace_ptr_;  
				void PollingThread( void );
				ACE_INT32 Rollover( void );
				/*This is a  luxuriant delimeter*/
			private:
				std::string cdr_path_;
				std::map<std::string, std::string> cfg_;
				ACE_Recursive_Thread_Mutex trace_mutex_;
				
			};

		}//namespace util
	}//namespace platform
}//namespace ubp

#endif // UBP_PLATFORM_CDR_TRACE_HPP_
