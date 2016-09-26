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
		vecUsers.insert(UserInfoList::value_type(StrToInt(pInfo->UserID), pInfo));
		
		strXml = strXml.substr(strXml.find("</UserInfo>") + std::string("</UserInfo>").length(), strXml.length() - strXml.find("</UserInfo>") - std::string("</UserInfo>").length());
		if (std::string::npos == strXml.find("<UserInfo>"))
		{
			break;
		}
	}
}
