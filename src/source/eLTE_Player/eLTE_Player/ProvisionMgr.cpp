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

#include "StdAfx.h"
#include "ProvisionMgr.h"
#include "eLTE_Log.h"


ProvisionMgr::ProvisionMgr(void)
	: m_pProvMgr(NULL)
{
}


ProvisionMgr::~ProvisionMgr(void)
{
	m_pProvMgr = NULL;
}


ProvisionMgr& ProvisionMgr::Instance()
{
	static ProvisionMgr s_ProvisionMgr;
	return s_ProvisionMgr;
}

// 配置管理接口初始化
int ProvisionMgr::Init(const std::string& strSrvIp, int iUserId)
{
	LOG_TRACE();
	INFO_PARAM2(strSrvIp, iUserId);

	m_pProvMgr = ProvisionManager_T::instance();
	if (NULL == m_pProvMgr)
	{
		LOG_RUN_ERROR("ProvisionManager_T instance failed.");
		return eLTE_ERR_FAILED;
	}

	m_pProvMgr->init(strSrvIp.c_str(), iUserId);

	return eLTE_ERR_SUCCESS;
}

int ProvisionMgr::InitMRS(const std::string& strSrvIp)
{
	LOG_TRACE();
	INFO_PARAM1(strSrvIp);

	m_pProvMgr = ProvisionManager_T::instance();
	if (NULL == m_pProvMgr)
	{
		LOG_RUN_ERROR("ProvisionManager_T instance failed.");
		return eLTE_ERR_FAILED;
	}

	m_pProvMgr->initMRS(strSrvIp.c_str());

	return eLTE_ERR_SUCCESS;
}

// 退出配置管理接口
int ProvisionMgr::Exit()
{
	LOG_TRACE();

	m_pProvMgr = ProvisionManager_T::instance();
	if (NULL == m_pProvMgr)
	{
		LOG_RUN_ERROR("ProvisionManager_T instance failed.");
		return eLTE_ERR_FAILED;
	}

	m_pProvMgr->exit();
	m_pProvMgr = NULL;

	return eLTE_ERR_SUCCESS;
}

// 获取调度台的群组列表
int ProvisionMgr::GetDcGroups(int iUserId, DcGroups* &pDcGroups) const
{
	LOG_TRACE();
	INFO_PARAM1(iUserId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_ERR_FAILED;
	}

	pDcGroups = pProvMgr->getDcGroupsClone(iUserId);
	if (NULL == pDcGroups)
	{
		LOG_RUN_ERROR("getDcGroupsClone failed.");
		return eLTE_ERR_FAILED;
	}

	return eLTE_ERR_SUCCESS;
}

// 获取调度台的用户列表
int ProvisionMgr::GetDcUsers(int iUserId, DcUsers* &pDcUsers) const
{
	LOG_TRACE();
	INFO_PARAM1(iUserId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_ERR_FAILED;
	}

	pDcUsers = pProvMgr->getDcUsersClone(iUserId);
	if (NULL == pDcUsers)
	{
		LOG_RUN_ERROR("getDcUsersClone failed.");
		return eLTE_ERR_FAILED;
	}

	return eLTE_ERR_SUCCESS;
}

// 获取一个群组的成员列表
int ProvisionMgr::GetGroupUsers(int iGroupId, GrpUserList* &pGrpUserList) const
{
	LOG_TRACE();
	INFO_PARAM1(iGroupId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_ERR_FAILED;
	}

	pGrpUserList = pProvMgr->getGroupUsersClone(iGroupId);
	if (NULL == pGrpUserList)
	{
		LOG_RUN_ERROR("getGroupUsersClone failed.");
		return eLTE_ERR_FAILED;
	}

	return eLTE_ERR_SUCCESS;
}

// 获取一个群组的详细配置信息
int ProvisionMgr::GetGroupInfo(int iGroupId, GroupInfo* &pGroupInfo) const
{
	LOG_TRACE();
	INFO_PARAM1(iGroupId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_ERR_FAILED;
	}

	pGroupInfo = pProvMgr->getGroupInfoClone(iGroupId);
	if (NULL == pGroupInfo)
	{
		LOG_RUN_ERROR("getGroupInfoClone failed.");
		return eLTE_ERR_FAILED;
	}

	return eLTE_ERR_SUCCESS;
}

// 获取一个用户的详细信息
eLTE_RET ProvisionMgr::GetUserInfo(int iUserId, UserInfo* &pUserInfo) const
{
	LOG_TRACE();
	INFO_PARAM1(iUserId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_ERR_FAILED;
	}

	pUserInfo = pProvMgr->getUserInfoClone(iUserId);
	if (NULL == pUserInfo)
	{
		LOG_RUN_ERROR("getUserInfoClone failed.");
		return eLTE_ERR_FAILED;
	}

	return eLTE_ERR_SUCCESS;
}

// 获取DC详细信息
int ProvisionMgr::GetDcInfo(int iUserId, DcProperty* &pDcProperty) const
{
	LOG_TRACE();
	INFO_PARAM1(iUserId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_ERR_FAILED;
	}

	pDcProperty = pProvMgr->getDcProperty(iUserId);
	if (NULL == pDcProperty)
	{
		LOG_RUN_ERROR("getDcProperty failed.");
		return eLTE_ERR_FAILED;
	}

	return eLTE_ERR_SUCCESS;
}

// 查询录音文件信息
int ProvisionMgr::GetUserRECFileInfoList(RECQueryInfo* pCondition, UserRecFileInfoList* &pInfoList)
{
	LOG_TRACE();

	if (NULL == m_pProvMgr)
	{
		LOG_RUN_ERROR("Please initMRS first.");
		return eLTE_ERR_FAILED;
	}

	if (NULL == pCondition)
	{
		LOG_RUN_ERROR("pCondition is null.");
		return eLTE_ERR_INVALID_PARAM;
	}

	pInfoList = m_pProvMgr->getUserRecFileInfoList(pCondition);
	if (NULL == pInfoList)
	{
		LOG_RUN_ERROR("getUserRecFileInfoList failed.");
		return eLTE_ERR_FAILED;
	}

	return eLTE_ERR_SUCCESS;
}
