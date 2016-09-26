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

/********************************************************************
filename    :    OperationMgr.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    ����̨���������ࡣ����ע�ᡢע������Ƶ�ش�ҵ���߼�������ӦeLTE SDK�ڵ�OperationManager��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_OCX_OPERATION_MGR_H__
#define __ELTE_OCX_OPERATION_MGR_H__

#include "DataType.h"


//////////////////////////////////////////////////////////////////////////
// ��Ƶ�ش�����
struct STVideoParam
{
	std::string strFmtValue;
	std::string strCameraType;
	std::string strUserCfmType;
	std::string strMuteType;

	STVideoParam& operator = (const STVideoParam& v)
	{
		if (this != &v)
		{
			strFmtValue = v.strFmtValue;
			strCameraType = v.strCameraType;
			strUserCfmType = v.strUserCfmType;
			strMuteType = v.strMuteType;
		}

		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
class OperationMgr
{
public:
	static OperationMgr& Instance();

private:
	OperationMgr(void);
	~OperationMgr(void);

public:
	// ����̨ע��
	int DcLogin(const std::string& userId, const std::string& passwd, 
		const std::string& serverIP, const std::string& localIP, const std::string& sipPort);
	// ����SDK����ҵ��
	int DcStartRunning();
	// ����̨ע��
	int DcLogout(const std::string& userId);
	// ����״̬�ϱ�
	int DcTriggerStatusReport(bool bEnableStatusReport) const;
	// ����̨������Ƶ�ش�
	int DcVideoMonitor(int iResId, Video_UL_parameter& param, bool bNeedSave = true);
	// ����̨�Ҷ���Ƶ�ش�
	int DcVideoHangup(int iResId, bool bNeedSave = true);
	// ����Ⱥ��
	int DcSubscribeGroup(const std::string& groupId, unsigned long ulEableSubscribeGroup) const;
	// ������̬����
	int DcCreateDGNA(DGNA_parameter& param) const;
	// ɾ����̬����
	int DcCancelDGNA(int iResId) const;
	// ������ʱ��
	int DcCreateTEMPG(DGNA_parameter& param) const;
	// ����һ����ʱ���
	int DcGetNextTempGroupID() const;
	//�����������
	int DcP2PDial(int iResId) const;
	//�����������
	int DcP2PRecv(int iResId) const;
	//�ܽ��������
	int DcP2PReject(int iResId) const;
	//�Ҷ��������
	int DcP2PHangup(int iResId) const;
	//����Ⱥ��
	int DcJoinGroup(int iResId) const;
	//���Ĳ��Զ�����Ⱥ��
	int DcSubJoinGroup(int iResId) const;
	//����������̶�����ʱ���������Ȩ
	int DcPTTDial(int iResId) const;
	//�ͷŻ�Ȩ
	int DcPTTRelease(int iResId) const;
	//�˳����
	int DcPTTHangup(int iResId) const;
	//����������
	int DcPTTEmergency(int iResId) const;
	//����ִ̨�о���
	int DcVolMute(int iResId, Mute_parameter& param) const;
	//����̨ȡ������
	int DcVolUnMute(int iResId, Mute_parameter& param) const;
	//����̨ǿ�����
	int GroupBreakoff(int iResId) const;
	//����̨ǿ����
	int P2PBreakoff(int iResId) const;
	//������ǿ����+�����µ����
	int P2PBreakin(int iResId) const;
	//��������
	int MuteControl(int iResId, int param) const;
	//����/ȥ����GIS�ն�
	int GISSubscribe(int iResId, GIS_parameter& param) const;
	//������
	int SDSSendMessage(int iResId, SDS_parameter& param) const;

	// ���ڹ���������
public:
	// ���������ļ�����
	void SetParam(const std::string& strFrontFormat, const std::string& strBackFormat);
	// �л�����ͷ
	int DcVideoReverse(int iResId) const;
	// ��ȡ�Ƿ�������
	std::string GetMuteType(int iResId);

	// ��½�ǳ�����
private:
	// �û�ID
	std::string m_userId;
	// �û�����
	std::string m_passwd;
	// ����IP
	std::string m_localIP;
	// ������IP
	std::string m_serverIP;
	// SIP�˿�
	std::string m_sipPort;

	// ���ڹ���������
public:
	// Ĭ��ǰ������ͷ��Ƶ��ʽ
	std::string m_strFrontFormat;
	// Ĭ�Ϻ�������ͷ��Ƶ��ʽ
	std::string m_strBackFormat;

public:
	// ��½�ɹ���ʾ
	BOOL m_bLoginSuccess;
	// eLTE_PlayerCtrl��ָ��
	COleControl* m_pCeLTE_PlayerCtrl;

	// ��Ƶ�ش���Դ�б�
public:
	std::map<int, STVideoParam> m_mapResource;

	// �л���Ƶ�߳�
public:
	// ��Ƶ�ر��ź�
	static HANDLE m_hHangupEvent;
	// �л���Ƶ�߳�
	static unsigned int __stdcall VideoReverseRun(void* lparam);
};

#endif
