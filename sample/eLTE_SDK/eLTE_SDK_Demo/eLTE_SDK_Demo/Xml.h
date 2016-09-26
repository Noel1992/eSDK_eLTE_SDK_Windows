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
filename    :    Xml.h
author      :    yubinbin wx200189
created     :    2015/01/16
description :    使用CString简单封装的XML字符串解析类。效率很低
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/16 初始版本
*********************************************************************/

#pragma once

class CXml
{
public:
	static CXml& Instance();

private:
	CXml(void);
	~CXml(void);

public:
	CString XmlParseElemValue(const CString& xmlStr, const CString& elem);
	void XmlParseDcGroups(const char* xmlStr, GroupInfoList& vecGroups);
	void XmlParseDcUsers(const ELTE_CHAR* xmlStr, UserInfoList& vecGroups);
	void XmlParseGroupUsers(const CString& xmlStr, GroupUserInfoList& vecGroupUsers);
	void XmlParseDcInfo(const CString& xmlStr, DcInfo& dcInfo);
	void XmlParseVWallIDList(const CString& xmlStr, VideoChannelList& vecUsers);
	void XmlParsePatchGroups(const char* xmlStr, GroupInfoList& vecGroups);
	void XmlParsePatchGroupUsers(const CString& xmlStr, GroupUserInfoList& vecGroupUsers);
};


#define GET_XML_RESULTE_CODE(xml) \
	StrToInt(CXml::Instance().XmlParseElemValue(xml, _T("ResultCode")))

#define GET_XML_ELEM_VALUE_INT(xml, elem) \
	StrToInt(CXml::Instance().XmlParseElemValue(xml, elem))

#define GET_XML_ELEM_VALUE_UINT(xml, elem) \
	(unsigned int)StrToInt(CXml::Instance().XmlParseElemValue(xml, elem))

#define GET_XML_ELEM_VALUE_STR(xml, elem) \
	CXml::Instance().XmlParseElemValue(xml, elem)

