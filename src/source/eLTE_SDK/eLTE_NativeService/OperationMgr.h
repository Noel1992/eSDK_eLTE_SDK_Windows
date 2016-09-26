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


class OperationMgr
{
public:
	static OperationMgr& Instance();

private:
	OperationMgr(void);
	~OperationMgr(void);

public:
	// ����̨ע��
	int Login(const std::string& userId, const std::string& passwd, 
		const std::string& serverIP, const std::string& localIP, const std::string& sipPort);
	// ����̨ע��
	int Logout(const std::string& userId);
	// ����SDK����ҵ��
	int StartRunning();
	// ����״̬�ϱ�
	int TriggerStatusReport(unsigned int uiEnableStatusReport) const;
	// ����̨������Ƶ�ش�
	int VideoMonitor(int iResId, Video_UL_parameter& param) const;
	// ����̨�Ҷ���Ƶ�ش�
	int VideoHangup(int iResId) const;
	// ��̨����
	int PTZControl(int iResId, FVG_control& control) const;
	// ����Ⱥ��
	int SubscribeGroup(int groupId, unsigned int uiEableSubscribeGroup) const;
	// ������̬����
	int CreateDynamicGroup(DGNA_parameter& param) const;
	// ɾ����̬����
	int CancelDynamicGroup(int iResId) const;
	// ������ʱ��
	int CreateTempGroup(DGNA_parameter& param) const;
	// ����һ����ʱ���
	int GetNextTempGroupID() const;
	// �����������
	int P2PDial(int iResId) const;
	// �����˫�����
	int P2PHalfDpxDial(int iResId) const;
	// �ͷŰ�˫�����
	int P2PHalfDpxRelease(int iResId) const;
	// �����������
	int P2PRecv(int iResId) const;
	// �ܽ��������
	int P2PReject(int iResId) const;
	// �Ҷ��������
	int P2PHangup(int iResId) const;
	// ����̨ǿ����
	int P2PBreakoff(int iResId) const;
	// ������ǿ����+�����µ����
	int P2PBreakin(int iResId) const;
	// ���Ĳ��Զ�����Ⱥ��
	int SubJoinGroup(int groupId) const;
	// ����������̶�����ʱ���������Ȩ
	int PTTDial(int groupId) const;
	// �ͷŻ�Ȩ
	int PTTRelease(int groupId) const;
	// �˳����
	int PTTHangup(int groupId) const;
	// ����������
	int PTTEmergency(int groupId) const;
	// ����̨ǿ�����
	int GroupBreakoff(int groupId) const;
	// ����ִ̨�о���
	int VolMute(int iResId, Mute_parameter& param) const;
	// �����ն˵�GIS������Ϣ
	int SetUserSpecificGISCfg(int resId, UEGisCfg_parameter& param) const;
	// ����̨ȡ������
	int VolUnMute(int iResId, Mute_parameter& param) const;
	// ����/ȥ����GIS�ն�
	int GISSubscribe(int iResId, GIS_parameter& param) const;
	// ���Ͷ�/����
	int SDSSend(const int& iResId, SDS_parameter& param) const;
	// ��ʼ��Ƶ�ַ�
	int VideoDispatch(int iResId, Video_Dispatch_parameter& param) const;
	// ֹͣ��Ƶ�ַ�
	int StopVideoDispatch(int iResId, Video_Dispatch_Delete& param) const;
	// ������Ƶ�ش�
	int RecvVideoPlay(int iResId) const;

	// ����̨������Ƶ��ǽ
	int VWallStart(int iResId, VWallStart_parameter& param) const;
	// ��Ƶ��ǽ
	int VWallStop(int iResId, VWallStop_parameter& param) const;
	// ���µ���ǽID״̬
	int UpdateDcVWallIDState(VWallInfoList* const &m_WallInfoList) const;

	// ����PSTN/PLMN�绰���� 
	int TelephoneDial(const std::string& PhoneNum) const;

	// �Ҷ�PSTN/PLMN�绰���� 
	int TelephoneHangup(const std::string& PhoneNum) const;

	// ���ܼ��� 
	int DiscreetListenOpera(int ResourceID, Operation_t OperationType) const;
	
	int P2PTransfer(const int iResId, transfer_parameter& param) const;

	// �����ɽ��顢�����ɽ����Ա��ɾ���ɽ����Ա
	int OperatePatchGroup(PCHGRP_Para& param, const unsigned short& uOperateType) const;

	// ɾ���ɽ���
	int CancelPatchGroup(int iResId) const;

	//�޸Ķ�̬Ⱥ��
	int ModifyDynamicGroup(DGNA_Modify_parameter& param) const;

	// ����̨��������ʱ�û�
	int TempUserJoinGroup(const int& resid, const PhonePatch_parameter& param) const;

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
	// ��½�ɹ���ʾ
	BOOL m_bLoginSuccess;
};

#endif
