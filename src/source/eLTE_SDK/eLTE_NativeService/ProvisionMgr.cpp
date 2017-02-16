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
#include "ProvisionMgr.h"
#include "eLTE_Log.h"
#include "eLTE_Error.h"


ProvisionMgr::ProvisionMgr(void)
	: m_bHaveInited(false)
{
}


ProvisionMgr::~ProvisionMgr(void)
{
	m_bHaveInited = false;
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

	// 状态校验
	if (m_bHaveInited)
	{
		LOG_RUN_WARN("Already Init ProvisionMgr.");
		return eLTE_SVC_ERR_SUCCESS;
	}

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("ProvisionManager_T instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pProvMgr->init(strSrvIp.c_str(), iUserId);
	m_bHaveInited = true;

	return eLTE_SVC_ERR_SUCCESS;
}

// 媒体服务器初始化
const int ProvisionMgr::InitMRS(const std::string& strSrvIp) const
{
	LOG_TRACE();
	INFO_PARAM1(strSrvIp);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("ProvisionManager_T instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	int iRet = pProvMgr->initMRS4DC(strSrvIp.c_str());

	return iRet;
}

// 退出配置管理接口
int ProvisionMgr::Exit()
{
	LOG_TRACE();

	if (!m_bHaveInited)
	{
		return eLTE_SVC_ERR_SUCCESS;
	}

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("ProvisionManager_T instance failed.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pProvMgr->exit();
	m_bHaveInited = false;

	return eLTE_SVC_ERR_SUCCESS;
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
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pDcGroups = pProvMgr->getDcGroupsClone(iUserId);
	if (NULL == pDcGroups)
	{
		LOG_RUN_ERROR("getDcGroupsClone failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

int ProvisionMgr::GetGisSubscription(int iUserId, GisQuerySubList* &pGisQuerySubList) const
{
	LOG_TRACE();
	INFO_PARAM1(iUserId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pGisQuerySubList = pProvMgr->queryGISSubscription(iUserId);
	if (NULL == pGisQuerySubList)
	{
		LOG_RUN_ERROR("queryGISSubscription failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

// 查询终端的GIS配置信息
int ProvisionMgr::GetUserSpecificGISCfg(int iResourceID, ProvGisCfg* &pUeGisCfg) const
{
	LOG_TRACE();
	INFO_PARAM1(iResourceID);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pUeGisCfg = pProvMgr->queryUserSpecificGISCfg(iResourceID);
	if (NULL == pUeGisCfg)
	{
		LOG_RUN_INFO("UeGisCfg is null.");
	}

	return eLTE_SVC_ERR_SUCCESS;
}

// 获取调度台的派接组信息
int ProvisionMgr::GetPatchGroups(int iUserId, PatchGroupsList* &pPatchGroupsList) const
{
	LOG_TRACE();
	INFO_PARAM1(iUserId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pPatchGroupsList = pProvMgr->getPatchGroupsClone(iUserId);
	if (NULL == pPatchGroupsList)
	{
		LOG_RUN_ERROR("getPatchGroupsClone failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

// 获取派接组的属性信息
int ProvisionMgr::GetPatchGroupInfo(int iGroupId, PatchGroupInfo* &pPatchGroupInfo) const
{
	LOG_TRACE();
	INFO_PARAM1(iGroupId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pPatchGroupInfo = pProvMgr->getPatchGroupById(iGroupId);
	if (NULL == pPatchGroupInfo)
	{
		LOG_RUN_ERROR("getPatchGroupById failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

// 获取派接组的成员信息
int ProvisionMgr::GetGroupMemberByPatchId(int iGroupId, PatchGroupMembers* &pPatchGroupMembers) const
{
	LOG_TRACE();
	INFO_PARAM1(iGroupId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pPatchGroupMembers = pProvMgr->getGroupMemberByPatchId(iGroupId);
	if (NULL == pPatchGroupMembers)
	{
		LOG_RUN_ERROR("getGroupMemberByPatchId failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
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
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pDcUsers = pProvMgr->getDcUsersClone(iUserId);
	if (NULL == pDcUsers)
	{
		LOG_RUN_ERROR("getDcUsersClone failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
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
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pGrpUserList = pProvMgr->getGroupUsersClone(iGroupId);
	if (NULL == pGrpUserList)
	{
		LOG_RUN_ERROR("getGroupUsersClone failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
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
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pGroupInfo = pProvMgr->getGroupInfoClone(iGroupId);
	if (NULL == pGroupInfo)
	{
		LOG_RUN_ERROR("getGroupInfoClone failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

// 获取一个用户的详细信息
int ProvisionMgr::GetUserInfo(int iUserId, UserInfo* &pUserInfo) const
{
	LOG_TRACE();
	INFO_PARAM1(iUserId);

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pUserInfo = pProvMgr->getUserInfoClone(iUserId);
	if (NULL == pUserInfo)
	{
		LOG_RUN_ERROR("getUserInfoClone failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
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
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pDcProperty = pProvMgr->getDcProperty(iUserId);
	if (NULL == pDcProperty)
	{
		LOG_RUN_ERROR("getDcProperty failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

// 查询录音文件信息
int ProvisionMgr::GetUserRECFileInfoList(RECQueryInfo* pCondition, UserRecFileInfoList* &pInfoList) const
{
	LOG_TRACE();

	if (NULL == pCondition)
	{
		LOG_RUN_ERROR("pCondition is null.");
		return eLTE_SVC_ERR_INVALID_PARAM;
	}

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	pInfoList = pProvMgr->getUserRecFileInfoList(pCondition);
	if (NULL == pInfoList)
	{
		LOG_RUN_ERROR("getUserRecFileInfoList failed.");
		return eLTE_SVC_ERR_FAILED;
	}

	return eLTE_SVC_ERR_SUCCESS;
}

// 获取视频上墙通道列表
int ProvisionMgr::GetDcVWallIDList(VWallInfoList* &m_WallInfoList) const
{
	LOG_TRACE();

	ProvisionManager* pProvMgr = ProvisionManager_T::instance();	
	if (NULL == pProvMgr)
	{
		LOG_RUN_ERROR("pProvMgr is null.");
		return eLTE_SVC_ERR_NULL_POINTER;
	}

	m_WallInfoList = pProvMgr->getVWallsClone();


	if (NULL == m_WallInfoList)
	{
		LOG_RUN_ERROR("getVWallsClone failed.");
		return eLTE_SVC_ERR_FAILED;
	}
	return eLTE_SVC_ERR_SUCCESS;
}
