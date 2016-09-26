/*Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
eSDK is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
		http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

#include "stdafx.h"
#include "eLTE_Timer.h"
#include "eLTE_Error.h"
#include "eLTE_Log.h"
#include "eLTE_Xml.h"
#include "Mmsystem.h"
#include "UserMgr.h"
#pragma comment(lib, "Winmm.lib")
#define   Accuracy           1     //系统允许的分辨率最小值 

CTimer::CTimer()
{
	m_uTimerID = 0;
	m_uDelayTime = Accuracy;
	m_uTimerType = 0;
	m_pUserMgr = NULL;
	m_EventTimer = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}


CTimer::~CTimer()
{
	try
	{
		(void)StopTimer();
	}
	catch (...)
	{
		
	}
	m_uDelayTime = Accuracy;
	m_uTimerType = 0;
	m_pUserMgr = NULL;
	if(m_EventTimer)
	{
		CloseHandle(m_EventTimer); 
		m_EventTimer = NULL;
	}
	RELEASE_PACKET(m_packetData);
}

ELTE_VOID CTimer::SetTimerType(const ELTE_UINT32& timerType)
{
	m_uTimerType = timerType;
}

ELTE_VOID CTimer::SetTimer(ELTE_UINT32 tick)
{
	LOG_TRACE();
	if(m_uTimerID)
	{
		timeKillEvent(m_uTimerID);
		m_uTimerID = 0;
	}

	if(0 == tick)
	{
		//默认200ms发送数据
		tick = 200;
	}

	TIMECAPS   timecaps; 
	ELTE_UINT32 TimerAccuracy = Accuracy; 

	//从系统获得关于定时器服务能力的信息， 
	//分辨率不能超出系统许可值（1到16毫秒） 
	if   (timeGetDevCaps(&timecaps,sizeof(TIMECAPS))==TIMERR_NOERROR) 
		TimerAccuracy = min(max(timecaps.wPeriodMin,Accuracy),timecaps.wPeriodMax); 

	timeBeginPeriod(TimerAccuracy); 
	m_uDelayTime = tick;

	m_uTimerID = timeSetEvent(m_uDelayTime, TimerAccuracy, TimeProc, (DWORD_PTR)this, TIME_PERIODIC);
	LOG_RUN_DEBUG("TimerId is %d.", m_uTimerID);
	timeEndPeriod(TimerAccuracy); 
}

ELTE_VOID __stdcall CTimer::TimeProc(
	UINT uID,      
	UINT uMsg,     
	DWORD_PTR dwUser,  
	DWORD dw1,     
	DWORD dw2      
	)
{
	if(dwUser)
	{
		CTimer* timer = (CTimer*) dwUser;
		ELTE_INT32 iRet = timer->DoProcess();
		if(eLTE_SDK_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("Timer do process failed.TimerID:%d,iRet:%d.",timer->m_uTimerID,iRet);
		}
	}
}

ELTE_INT32 CTimer::DoProcess()
{
	LOG_TRACE();
	if (NULL == m_pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return eLTE_SDK_ERR_FAILED;
	}
	//请求xml
	CXml reqXml;

	SSL_Socket& socket = const_cast<SSL_Socket&>(m_pUserMgr->GetSSLSocket());
	ELTE_INT32 iRet = socket.SendMsg(ELTE_SERVICE_KEEPALIVE_REQ, reqXml, FALSE);
	if(eLTE_SDK_ERR_SUCCESS != iRet) 
	{
		return iRet;
	}
	iRet = (ELTE_INT32)::WaitForSingleObject(m_EventTimer, (ELTE_INT32)WAIT_OBJECT_TIME);
	if (eLTE_SDK_ERR_SUCCESS != iRet)
	{
		CServerMgr& serverMgr = const_cast<CServerMgr&>(m_pUserMgr->GetServerMgr());
		if(!serverMgr.ServerIsRunning() || 0 != m_pUserMgr->GetServerStatus())
		{
			if (m_pUserMgr)
			{
				m_pUserMgr->SetServerStatus(0);
			}
			return eLTE_SDK_ERR_SERVER_NOT_RUNNING;
		}
		return iRet;
	}
	iRet = m_packetData.RspCode;
	::ResetEvent(m_EventTimer);
	return iRet;
}

ELTE_UINT32 CTimer::GetDelayTime() const
{
	return m_uDelayTime;
}

ELTE_VOID CTimer::SetDelayTime(const ELTE_UINT32& delayTime)
{
	m_uDelayTime = delayTime;
}

ELTE_INT32 CTimer::StopTimer()
{
	LOG_TRACE();
	if(0 != m_uTimerID)
	{
		LOG_RUN_DEBUG("TimerId is %d.", m_uTimerID);
		timeKillEvent(m_uTimerID);
		m_uTimerID = 0;
	}
	return eLTE_SDK_ERR_SUCCESS;
}

ELTE_VOID CTimer::SetUserMgr(CUserMgr* pUserMgr)
{
	LOG_TRACE();
	if(NULL == pUserMgr)
	{
		LOG_RUN_ERROR("UserMgr is null.");
		return;
	}
	m_pUserMgr = pUserMgr;
}

ELTE_VOID CTimer::SetPacketData(const PACKET_DATA& packetData)
{
	RELEASE_PACKET(m_packetData);
	m_packetData = packetData;
}

