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
filename    :    DisplayMgr.h
author      :    yubinbin wx200189
created     :    2014/07/01
description :    eLTE SDKҵ����Ϣ�ص��ࡣͨ��ע����������յ���̨״ָ̬ʾ�¼�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_OCX_DISPLAY_MGR_H__
#define __ELTE_OCX_DISPLAY_MGR_H__


class DisplayMgr : public DisplayManager
{
public:
	DisplayMgr(void);
	virtual ~DisplayMgr(void); //lint !e1510

	//֪ͨ�����״̬�����Ϣ
	virtual void notifyGroupStatus(GrpDspInfo * grpInfo);

	//֪ͨ�û���Ⱥ����Դ��״̬�����Ϣ
	virtual void notifyResourceStatus(ResourceStatusIndicator * resourceStatus);

	//֪ͨ��Ե���е�״̬�����Ϣ
	virtual void notifyP2pcallStatus(P2pcallStatusIndicator * callStatus);

	//֪ͨ��Ƶ���е�״̬�����Ϣ��������Ƶ�ش�������ҪYCʵ��
	virtual void notifyP2pvideocallStatus(P2pvideocallStatusIndicator * callStatus);

	//֪ͨ�û���Ⱥ����Դ��״̬�����Ϣ,��ҪYCʵ��
	virtual void notifyUserStatus(ResourceStatusIndicator * resourceStatus);

	//֪ͨ�û�״̬����Ϣ�ķ������ʵ��
	virtual void notifyUserSdsStatus(SdsMessageIndicator * sdsmsg);

	//ģ�����״̬�¼�
	virtual void notifyModuleStatus(DCModuleStatusIndicator * moduleStatus);

	//��������Ϣ�ϱ���GUI
	virtual void notifySMS(SdsMessageIndicator * sdsmsg);

	//֪ͨ���ö���ı����Ϣ�� ����Ϊһ����ָ̬�룬 ʹ������Ҫdowncast Ϊ��Ӧ�ı������
	virtual void notifyProvisionChange(ProvChangeIndicator* pProvChange);

	//֪ͨ���ö����ȫͬ���� ����Ϊһ����ָ̬�룬 ʹ������Ҫdowncast Ϊ��Ӧ�ı������
	virtual void notifyProvisionAllResync(ProvChangeIndicator* pProvChangeIndicator);

	//֪ͨ״̬�����ȫͬ���� ����Ϊһ����ָ̬�룬 ʹ������Ҫdowncast Ϊ��Ӧ�ı������
	virtual void notifyStatusAllResync(ResourceStatusIndicator* pResourceStatusIndicator);
};

#endif
