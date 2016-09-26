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
filename    :    ProvisionMgr.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    ����̨���ù����ࡣ����Ⱥ����û��߼�������ӦeLTE SDK�ڵ�ProvisionManager��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_OCX_PROVISION_MGR_H__
#define __ELTE_OCX_PROVISION_MGR_H__


class ProvisionMgr
{
public:
	static ProvisionMgr& Instance();

private:
	ProvisionMgr(void);
	~ProvisionMgr(void);

public:
	// ���ù���ӿڳ�ʼ��
	int Init(const std::string& strSrvIp, int iUserId);
	// ý���������ʼ��
	const int InitMRS(const std::string& strSrvIp) const;
	// �˳����ù���ӿ�
	int Exit();
	// ��ȡ����̨��Ⱥ���б�
	int GetDcGroups(int iUserId, DcGroups* &pDcGroups) const;
	// ��ȡ����̨���û��б�
	int GetDcUsers(int iUserId, DcUsers* &pDcUsers) const;
	// ��ȡһ��Ⱥ��ĳ�Ա�б�
	int GetGroupUsers(int iGroupId, GrpUserList* &pGrpUserList) const;
	// ��ȡ����̨���ɽ����б�
	int GetPatchGroups(int iUserId, PatchGroupsList* &pPatchGroupsList) const;
	// ��ȡ�ɽ���ĳ�Ա�б�
	int GetGroupMemberByPatchId(int iGroupId, PatchGroupMembers* &pPatchGroupMembers) const;
	// ��ȡ�ɽ����������Ϣ
	int GetPatchGroupInfo(int iGroupId, PatchGroupInfo* &pPatchGroupInfo) const;
	// ��ȡһ��Ⱥ�����ϸ������Ϣ
	int GetGroupInfo(int iGroupId, GroupInfo* &pGroupInfo) const;
	// ��ȡһ���û�����ϸ��Ϣ
	int GetUserInfo(int iUserId, UserInfo* &pUserInfo) const;
	// ��ȡDC��ϸ��Ϣ
	int GetDcInfo(int iUserId, DcProperty* &pDcProperty) const;
	// ��ѯ¼���ļ���Ϣ
	int GetUserRECFileInfoList(RECQueryInfo* pCondition, UserRecFileInfoList* &pInfoList) const;
	// ��ȡ��Ƶ��ǽID�б�
	int GetDcVWallIDList(VWallInfoList* &m_WallInfoList) const;
	// ��ѯ�ն˵�GIS������Ϣ
	int GetUserSpecificGISCfg(int iResourceID, ProvGisCfg* &pUeGisCfg) const;
	// ��ȡ����̨���ĵ��ն��б�
	int GetGisSubscription(int iUserId, GisQuerySubList* &pGisQuerySubList) const;

private:
	bool m_bHaveInited;
};

#endif
