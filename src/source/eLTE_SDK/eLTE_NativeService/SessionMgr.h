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
	// ����SessionID��18λguid + 14λʱ��
	int NewSessionID();
	// ��ȡSessionID
	const std::string& GetSessionID() const;
	// ����SessionID
	void ClearSessionID();
	// ���ʼ
	int KeepAliveStart();
	// ����ˢ��
	void KeepAliveFreshen();
	// �������
	void KeepAliveStop();

private:
	static unsigned __stdcall KeepAliveThread(void* param);

private:
	std::string m_strSessionID;
	HANDLE m_hKeepAliveThread;
	HANDLE m_hMutex;
	HANDLE m_hStopEvent;
	unsigned int m_uiTimeOutCount;	// ���ʱ����
};

#endif
