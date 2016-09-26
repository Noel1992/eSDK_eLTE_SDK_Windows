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

#ifndef __ELTE_SESSION_MGR_H__
#define __ELTE_SESSION_MGR_H__


class SessionMgr
{
protected:
	SessionMgr(void);
	~SessionMgr(void);

public:
	static SessionMgr& Instance();
	// 创建SessionID，18位guid + 14位时间
	int NewSessionID();
	// 获取SessionID
	const std::string& GetSessionID() const;
	// 清理SessionID
	void ClearSessionID();
	// 保活开始
	int KeepAliveStart();
	// 保活刷新
	void KeepAliveFreshen();
	// 保活结束
	void KeepAliveStop();

private:
	static unsigned __stdcall KeepAliveThread(void* param);

private:
	std::string m_strSessionID;
	HANDLE m_hKeepAliveThread;
	HANDLE m_hMutex;
	HANDLE m_hStopEvent;
	unsigned int m_uiTimeOutCount;	// 保活超时次数
};

#endif
