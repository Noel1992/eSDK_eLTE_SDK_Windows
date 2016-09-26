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
#include "Xml.h"
#include "eLTE_Tool.h"


CXml::CXml(void)
{
}


CXml::~CXml(void)
{
}


CXml& CXml::Instance()
{
	static CXml s_CXml;
	return s_CXml;
}

CString CXml::XmlParseElemValue(const CString& xmlStr, const CString& elem)
{
	CString value(xmlStr);
	CString elemBegin(elem);
	CString elemEnd(elem);
	elemBegin.Insert(0, _T("<"));
	elemBegin.Append(_T(">"));
	elemEnd.Insert(0, _T("</"));
	elemEnd.Append(_T(">"));

	value = value.Left(value.Find(elemEnd));
	value = value.Right(value.GetLength() - elemBegin.GetLength() - value.Find(elemBegin));

	return value;
}

void CXml::XmlParseDcGroups(const char* xmlStr, GroupInfoList& vecGroups)
{
	/************************************************************************
	<Content>
		<ResultCode></ResultCode>
		<GroupInfoList>
			<GroupInfo>
				<GroupID></GroupID>
				<GroupCategory></GroupCategory>
				<GroupPriority></GroupPriority>
				<GroupName></GroupName>
			</GroupInfo>
		</GroupInfoList>
	</Content>
	************************************************************************/
	std::string strXml = xmlStr;
	while (!strXml.empty())
	{
		GroupInfo* pInfo = new GroupInfo;
		pInfo->GroupID			= XmlParseElemValue(eLTE_Tool::ANSIToUnicode(strXml), _T("GroupID"));
		pInfo->GroupCategory	= XmlParseElemValue(eLTE_Tool::ANSIToUnicode(strXml), _T("GroupCategory"));
		pInfo->GroupPriority	= XmlParseElemValue(eLTE_Tool::ANSIToUnicode(strXml), _T("GroupPriority"));
		pInfo->GroupName		= XmlParseElemValue(eLTE_Tool::UTF8ToUnicode(strXml), _T("GroupName"));
		vecGroups.push_back(pInfo);

		strXml = strXml.substr(strXml.find("</GroupInfo>") + std::string("</GroupInfo>").length(), strXml.length() - strXml.find("</GroupInfo>") - std::string("</GroupInfo>").length());
		if (std::string::npos == strXml.find("<GroupInfo>"))
		{
			break;
		}
	}
}

void CXml::XmlParsePatchGroups(const char* xmlStr, GroupInfoList& vecGroups)
{
	/************************************************************************
	<Content> 
		<PatchGroupInfoList>
			<PatchGroupInfo>
				<GroupNumber></GroupNumber>
				<SetupDcId></SetupDcId>
				<PGPriority></PGPriority>
				<DcPatchIndex></DcPatchIndex>
				<PGName></PGName>
				<VPNID></VPNID>
			</PatchGroupInfo>
		</PatchGroupInfoList>
	</Content>
	************************************************************************/
	std::string strXml = xmlStr;
	if (std::string::npos == strXml.find("<PatchGroupInfo>"))
	{
		return;
	}
	while (!strXml.empty())
	{
		GroupInfo* pInfo = new GroupInfo;
		pInfo->GroupID			= XmlParseElemValue(eLTE_Tool::ANSIToUnicode(strXml), _T("GroupNumber"));
		pInfo->GroupCreator	    = XmlParseElemValue(eLTE_Tool::ANSIToUnicode(strXml), _T("SetupDcId"));
		pInfo->GroupPriority	= XmlParseElemValue(eLTE_Tool::ANSIToUnicode(strXml), _T("PGPriority"));
		pInfo->GroupName		= XmlParseElemValue(eLTE_Tool::UTF8ToUnicode(strXml), _T("PGName"));
		pInfo->GroupIndex		= XmlParseElemValue(eLTE_Tool::UTF8ToUnicode(strXml), _T("DcPatchIndex"));
		pInfo->GroupVPNID		= XmlParseElemValue(eLTE_Tool::UTF8ToUnicode(strXml), _T("VPNID"));
		vecGroups.push_back(pInfo);

		strXml = strXml.substr(strXml.find("</PatchGroupInfo>") + std::string("</PatchGroupInfo>").length(), strXml.length() - strXml.find("</PatchGroupInfo>") - std::string("</PatchGroupInfo>").length());
		if (std::string::npos == strXml.find("<PatchGroupInfo>"))
		{
			break;
		}
	}
}

void CXml::XmlParseDcUsers(const ELTE_CHAR* xmlStr, UserInfoList& vecUsers)
{
	/************************************************************************
	<Content>
		<ResultCode></ResultCode>
		<UserInfoList>
			<UserInfo>
				<UserID></UserID>
				<UserCategory></UserCategory>
				<UserPriority></UserPriority>
				<UserName></UserName>
			</UserInfo>
		</UserInfoList>
	</Content>
	************************************************************************/
	std::string strXml = xmlStr;
	while (!strXml.empty())
	{
		UserInfo* pInfo = new UserInfo;
		pInfo->UserID			= XmlParseElemValue(eLTE_Tool::ANSIToUnicode(strXml), _T("UserID"));
		pInfo->UserCategory		= XmlParseElemValue(eLTE_Tool::ANSIToUnicode(strXml), _T("UserCategory"));
		pInfo->UserPriority		= XmlParseElemValue(eLTE_Tool::ANSIToUnicode(strXml), _T("UserPriority"));
		pInfo->UserName			= XmlParseElemValue(eLTE_Tool::UTF8ToUnicode(strXml), _T("UserName"));
		vecUsers.push_back(pInfo);
		
		strXml = strXml.substr(strXml.find("</UserInfo>") + std::string("</UserInfo>").length(), strXml.length() - strXml.find("</UserInfo>") - std::string("</UserInfo>").length());
		if (std::string::npos == strXml.find("<UserInfo>"))
		{
			break;
		}
	}
}

void CXml::XmlParseGroupUsers(const CString& xmlStr, GroupUserInfoList& vecGroupUsers)
{
	/************************************************************************
	<Content>
		<ResultCode></ResultCode>
		<GroupUserInfoList>
			<GroupUserInfo>
				<UserID></UserID>
				<GroupID></GroupID>
				<UserPriorityInGroup></UserPriorityInGroup>
				<MemberType></MemberType>
			</GroupUserInfo>
		</GroupUserInfoList>
	</Content>
	************************************************************************/
	CString str(xmlStr);	
	while (!str.IsEmpty())
	{
		GroupUserInfo* pInfo = new GroupUserInfo;
		pInfo->UserID				= XmlParseElemValue(str, _T("UserID"));
		pInfo->GroupID				= XmlParseElemValue(str, _T("GroupID"));
		pInfo->UserPriorityInGroup	= XmlParseElemValue(str, _T("UserPriorityInGroup"));
		pInfo->MemberType			= XmlParseElemValue(str, _T("MemberType"));
		vecGroupUsers.push_back(pInfo);

		str = str.Right(str.GetLength() - str.Find(_T("</GroupUserInfo>")) - CString(_T("</GroupUserInfo>")).GetLength());
		if (-1 == str.Find(_T("<GroupUserInfo>")))
		{
			break;
		}
	}
}

void CXml::XmlParsePatchGroupUsers(const CString& xmlStr, GroupUserInfoList& vecGroupUsers)
{
	/************************************************************************
	<Content> 
		<PatchGroupMemberList>
			<PatchGroupMember>
				<GroupNumber></GroupNumber>
				<MemberGroupId></MemberGroupId>
			</PatchGroupMember>
		</PatchGroupMemberList>
	</Content>
	************************************************************************/
	CString str(xmlStr);
	if(-1 == str.Find(_T("<PatchGroupMember>")))
	{
		return;
	}
	while (!str.IsEmpty())
	{
		GroupUserInfo* pInfo = new GroupUserInfo;
		pInfo->GroupID				= XmlParseElemValue(str, _T("GroupNumber"));
		pInfo->MemberGroupId	    = XmlParseElemValue(str, _T("MemberGroupId"));
		vecGroupUsers.push_back(pInfo);

		str = str.Right(str.GetLength() - str.Find(_T("</PatchGroupMember>")) - CString(_T("</PatchGroupMember>")).GetLength());
		if (-1 == str.Find(_T("<PatchGroupMember>")))
		{
			break;
		}
	}
}

void CXml::XmlParseDcInfo(const CString& xmlStr, DcInfo& dcInfo)
{
	/************************************************************************
	<Content>
		<ResultCode></ResultCode>
		<DcInfo>
			<DcID></DcID>
			<Priviledge></Priviledge>
			<Role></Role>
			<Alias></Alias>
		</DcInfo>
	</Content>
	************************************************************************/

	dcInfo.DcID			= XmlParseElemValue(xmlStr, _T("DcID"));
	dcInfo.Priviledge	= XmlParseElemValue(xmlStr, _T("Priviledge"));
	dcInfo.Role			= XmlParseElemValue(xmlStr, _T("Role"));
	dcInfo.Alias		= XmlParseElemValue(xmlStr, _T("Alias"));
}

void CXml::XmlParseVWallIDList(const CString& xmlStr, VideoChannelList& vecUsers)
{
	/************************************************************************
	--- XML ∏Ò Ω ---
	<Content> 
		<VWallIDList>
			<VWallID>
				<DstObjId></DstObjId>
				<IDState></IDState>
			</VWallID>
		</VWallIDList>
	</Content>
	************************************************************************/
	CString str(xmlStr);	
	while (!str.IsEmpty())
	{
		//UserInfo* pInfo = new UserInfo;
		CString VWallID	= XmlParseElemValue(str, _T("VWallID"));

		CString DstObjId = XmlParseElemValue(VWallID, _T("DstObjId"));
		CString IDState = XmlParseElemValue(VWallID, _T("IDState"));

		if ("" != IDState)
		{
			vecUsers[DstObjId] = IDState;
		}		

		str = str.Right(str.GetLength() - str.Find(_T("</VWallID>")) - CString(_T("</VWallID>")).GetLength());
		if (-1 == str.Find(_T("<VWallID>")))
		{
			break;
		}		
	}
}