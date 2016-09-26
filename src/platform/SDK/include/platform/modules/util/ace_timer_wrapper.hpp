#ifndef    UBP_PLATFORM_UTIL_ACE_TIMER_WRAPPER_HPP_
#define    UBP_PLATFORM_UTIL_ACE_TIMER_WRAPPER_HPP_
/********************************************************************
  Copyright 2014,TD-Tech. Co., Ltd.
  Filename:    ace_timer_wrapper.hpp
  Author:      HuangChunlun
  Created:     2014/11/19 15:15
  Description:

*********************************************************************/
#include "ace/Log_Msg.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"
#include <ace/Basic_Types.h>
#include "ace/Timer_Heap_T.h"
#include <ace/Activation_Queue.h>
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Timer_Heap.h"

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> ActiveTimer;

namespace ubp
{
namespace platform
{
namespace util
{
class AceTimerWrapper: public ACE_Event_Handler
{
public:
	AceTimerWrapper(  );
	virtual ~AceTimerWrapper(void);
	//计时器到期后运行的回调函数
	virtual int handle_timeout(const ACE_Time_Value& , const void *act /* = 0 */);
	virtual ACE_INT32 Init( const time_t abs_time, const time_t interval );
	virtual ACE_INT32 TimerId(void){	return timer_id_; }
protected:
	ACE_INT32 timer_id_;
	ActiveTimer sync_timer_;
	ACE_Timer_Heap syncTimerHeap_;
	ACE_thread_t sync_tid_;
};

}//namespace util
}//namespace platform
}//namespace ubp

#endif