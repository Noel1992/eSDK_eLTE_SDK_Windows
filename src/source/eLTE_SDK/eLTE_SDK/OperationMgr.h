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
author      :    zWX229156
created     :    2015/01/12
description :    ���������ࣨ����Ⱥ�����ز�����
copyright   :    Copyright (c) 2015-2017 Huawei Tech.Co.,Ltd
history     :    2015/01/12 ��ʼ�汾
*********************************************************************/

#ifndef __OPERATION_MGR_H__
#define __OPERATION_MGR_H__

#include "eLTE_Types.h"

class CUserMgr;//lint !e763
class COperationMgr
{
public:
	COperationMgr();
	~COperationMgr();

public:
	//�����û�������
	ELTE_VOID SetUserMgr(CUserMgr* pUserMgr);

	//ȡ������Ⱥ��
	ELTE_INT32 UnSubscribeGroup(const ELTE_CHAR* pGroupID) const;

	//���Ĳ��Զ�����Ⱥ��
	ELTE_INT32 SubJoinGroup(const ELTE_CHAR* pGroupID) const;

	//������̬Ⱥ��
	ELTE_INT32 CreateDynamicGroup(const ELTE_CHAR* pDGNAParam) const;

	//ɾ����̬Ⱥ��
	ELTE_INT32 CancelDynamicGroup(const ELTE_CHAR* pResourceID) const;

	//��ȡ��ʱ���
	ELTE_INT32 GetTempGroupID(ELTE_INT32* iGroupID) const;

	//������ʱ��
	ELTE_INT32 CreateTempGroup(const ELTE_CHAR* pTEMPGParam) const;

	//�����������
	ELTE_INT32 P2PDial(const ELTE_CHAR* pResourceID) const;
	
	//�����˫�����
	ELTE_INT32 P2PHalfDpxDial(const ELTE_CHAR* pResourceID) const;

	//�ͷŰ�˫�����
	ELTE_INT32 P2PHalfDpxRelease(const ELTE_CHAR* pResourceID) const;

	//�����������
	ELTE_INT32 P2PRecv(const ELTE_CHAR* pResourceID) const;

	//�ܽ��������
	ELTE_INT32 P2PReject(const ELTE_CHAR* pResourceID) const;

	//�Ҷ��������
	ELTE_INT32 P2PHangup(const ELTE_CHAR* pResourceID) const;

	//����̨ǿ����
	ELTE_INT32 P2PBreakoff(const ELTE_CHAR* pResourceID) const;

	//������ǿ����+�����µ����
	ELTE_INT32 P2PBreakin(const ELTE_CHAR* pResourceID) const;

	//����������̶�����ʱ���������Ȩ
	ELTE_INT32 PTTDial(const ELTE_CHAR* pGroupID) const;

	//�ͷŻ�Ȩ
	ELTE_INT32 PTTRelease(const ELTE_CHAR* pGroupID) const;

	//�˳����
	ELTE_INT32 PTTHangup(const ELTE_CHAR* pGroupID) const;

	//����������
	ELTE_INT32 PTTEmergency(const ELTE_CHAR* pGroupID) const;

	//����̨ǿ�����
	ELTE_INT32 GroupBreakoff(const ELTE_CHAR* pGroupID) const;

	//����ִ̨�о���
	ELTE_INT32 VolMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pMuteParam) const;

	//����̨ȡ������
	ELTE_INT32 VolUnMute(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pUnMuteParam) const;

	//����/ȥ����GIS�ն�
	ELTE_INT32 GISSubscribe(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pGISParam) const;

	//���Ͷ���/����
	ELTE_INT32 SDSSendMessage(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pSDSParam) const;

	//�����˹�ת��
	ELTE_INT32 P2PTransfer(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pP2PTransferParam) const;

	//�����ɽ��顢�����ɽ����Ա��ɾ���ɽ����Ա
	ELTE_INT32 OperatePatchGroup(const ELTE_CHAR* pPatchGroupParam, const ELTE_USHORT& uOperateType) const;

	//ɾ���ɽ���
	ELTE_INT32 CancelPatchGroup(const ELTE_CHAR* pPatchGroupID) const;

	//�����ն˵�GIS������Ϣ
	ELTE_INT32 SetGisParam(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pUeGisCfg) const;

	//�޸Ķ�̬����
	ELTE_INT32 ModifyDynamicGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pDynamicGroupInfo) const;

	//����̨��������ʱ�û�
	ELTE_INT32 TempUserJoinGroup(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pPhonePatchParam) const;

private:
	CUserMgr*    m_pUserMgr;
};

#endif //__OPERATION_MGR_H__
