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
author      :    zhangziqian zwx229156
created     :    2016/05/30
description :    使用CString简单封装的XML字符串解析类。
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2016/05/30 初始版本
*********************************************************************/
#include "DataType.h"

#pragma once

class CXml
{
public:
	static CXml& Instance();

private:
	CXml(void);
	~CXml(void);

public:
	//parse elem value
	CString XmlParseElemValue(const CString& xmlStr, const CString& elem);

	//parse dc users
	void XmlParseDcUsers(const ELTE_CHAR* xmlStr, UserInfoList& vecGroups);
};


#define GET_XML_RESULTE_CODE(xml) \
	StrToInt(CXml::Instance().XmlParseElemValue(xml, _T("ResultCode")))

#define GET_XML_ELEM_VALUE_INT(xml, elem) \
	StrToInt(CXml::Instance().XmlParseElemValue(xml, elem))

#define GET_XML_ELEM_VALUE_UINT(xml, elem) \
	(unsigned int)StrToInt(CXml::Instance().XmlParseElemValue(xml, elem))

#define GET_XML_ELEM_VALUE_STR(xml, elem) \
	CXml::Instance().XmlParseElemValue(xml, elem)

