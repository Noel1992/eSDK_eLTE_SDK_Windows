#include "stdafx.h"
#include "eLTE_RM_Timer.h"
//SSL manage class
#include "OpenSSL_Mgr.h"
//log manage class 
#include "eLTE_Log.h"

//initial static variable
int RMTimer::iTaskNum = 0;
//int RMTimer::m_iHeartBeatCounter = 0;
//int RMTimer::m_iReConnectCounter = 0;
unsigned int RMTimer::m_uTimerIDArray[TIMERNUM]={0};


/**************************************************************************
* name       : SetTimer
* description: ������ʱ��
* input      : tick
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void RMTimer::SetTimer(int iID, int tick)
{
	LOG_TRACE();
	
	TIMECAPS   timecaps; 
	int TimerAccuracy = 1; 	

	switch(iID)
	{
	case RECONNECT_TIMER:
		{
			if (m_uTimerIDArray[RECONNECT_TIMER] != IDLE_TIMER)
			{
				LOG_RUN_ERROR("Heart beat timer is already running!");
// 				if (RET_SUCCESS != StopTimer(iID))
// 				{
// 					LOG_RUN_ERROR("eLTE_RM_Timer::Reconnect StopTimer error!");
// 				}

				return;
			}
			else
			{
//				m_iReConnectCounter++;
				LOG_RUN_INFO("reconnect Timer start .");
			}
		}
		break;
	case HEARTBEAT_TIMER:
		{
			if (m_uTimerIDArray[HEARTBEAT_TIMER] != IDLE_TIMER)
			{
				LOG_RUN_ERROR("Reconnect timer is already running!");
// 				if (RET_SUCCESS != StopTimer(iID))
// 				{
// 					LOG_RUN_ERROR("eLTE_RM_Timer::Reconnect StopTimer error!");
// 				}
				
				return;
			}
			else
			{
				//m_iHeartBeatCounter++;
				LOG_RUN_INFO("heart beat Timer start.");
			}
		}
		break;
	default:
		;
	}
	
	//��ϵͳ��ù��ڶ�ʱ��������������Ϣ�� 
	//�ֱ��ʲ��ܳ���ϵͳ���ֵ��1��16���룩 
	if   (timeGetDevCaps(&timecaps,sizeof(TIMECAPS))==TIMERR_NOERROR)
	{
		TimerAccuracy = min(max(timecaps.wPeriodMin,1),timecaps.wPeriodMax); 
	}

	timeBeginPeriod(TimerAccuracy);
	if (MAX_TASK_NUM >= iID)
	{
		m_uTimerIDArray[iID] = timeSetEvent(tick, TimerAccuracy, TimeProc, (DWORD_PTR)this, TIME_PERIODIC);	
		setTaskNum(iID);
		LOG_RUN_INFO("start Timer ID:%d",m_uTimerIDArray[iID]);
	}
	
	timeEndPeriod(TimerAccuracy); 
}

/**************************************************************************
* name       : TimeProc
* description: ��ʱ��ִ�к���
* input      : uID,uMsg,dwUser,dw1,dw2
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void __stdcall RMTimer::TimeProc(
	UINT uID,      
	UINT uMsg,     
	DWORD_PTR dwUser,  
	DWORD dw1,
	DWORD dw2      
	)
{
	int iRet = 0;
	if(dwUser)
	{		
		RMTimer* timer = (RMTimer*) dwUser;
		if (RET_SUCCESS != timer->DoProcess())
		{
			LOG_RUN_ERROR("Timer DoProcess error! error Code:%d",iRet);
		}

	}
}

/**************************************************************************
* name       : DoProcess
* description: ִ��������
* input      : NA
* output     : 0
* return     : NA
* remark     : NA
**************************************************************************/
int RMTimer::DoProcess()
{
	LOG_TRACE();
	int iRet = 0;
	switch (iTaskNum)
	{
	case RECONNECT_TIMER:
		{
			iRet = OpenSSL_Mgr::Instance().SSL_ReConnect();
			//Reconnect
			if (RET_SUCCESS == iRet)
			{
				//stop reconnect timer
				if (RET_SUCCESS != StopTimer(iTaskNum))
				{
					LOG_RUN_ERROR("eLTE_RM_Timer::Reconnect StopTimer error!");
				}

			}
		}
		break;
	case HEARTBEAT_TIMER:
		{
			iRet = OpenSSL_Mgr::Instance().SSL_HeartBeat();
			//Heart beat 
			if (RET_SUCCESS != iRet)
			{
				//stop heart beat timer
				if (RET_SUCCESS != StopTimer(iTaskNum))
				{
					LOG_RUN_ERROR("Timer Stop warning: stop timer error or timer stop running");
				}
				
				//start reconnect timer
				SetTimer(RECONNECT_TIMER, RECONNECT_POLLING_TIME);
			}
		}
		break;
	default:
		;
	}

	return iRet;
}

/**************************************************************************
* name       : StopTimer
* description: ֹͣ��ʱ��
* input      : NA
* output     : 0
* return     : NA
* remark     : NA
**************************************************************************/
int RMTimer::StopTimer(int iID)
{
	LOG_TRACE();	

	switch (iID)
	{
	case RECONNECT_TIMER:
		{
			if (m_uTimerIDArray[RECONNECT_TIMER] == IDLE_TIMER)
			{
				LOG_RUN_ERROR("no Reconnect timer is running!");
				return RET_FAILED;
			}
			else
			{				
//				m_iReConnectCounter--;
				LOG_RUN_INFO("reconnect Timer end.");
			}
		}
		break;
	case HEARTBEAT_TIMER:
		{
			if (m_uTimerIDArray[HEARTBEAT_TIMER] == IDLE_TIMER)
			{
				LOG_RUN_ERROR("no heart beat timer is running!");
				return RET_FAILED;
			}
			else
			{
				//m_iHeartBeatCounter--;
				LOG_RUN_INFO("heart beat Timer end.");
			}
		}
		break;
	default:
		;
	}


	if(IDLE_TIMER != m_uTimerIDArray[iID])
	{
		LOG_RUN_INFO("kill Timer ID:%d",m_uTimerIDArray[iID]);
		timeKillEvent(m_uTimerIDArray[iID]);
		m_uTimerIDArray[iID] = IDLE_TIMER;		
	}
	return RET_SUCCESS;
}

RMTimer& RMTimer::Instance()
{
	static RMTimer s_RMTimer;
	return s_RMTimer;
}