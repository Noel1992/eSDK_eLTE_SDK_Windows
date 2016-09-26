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
filename    :    Language.h
author      :    yubinbin wx200189
created     :    2014/08/12
description :    国际化文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/08/12 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_LANGUAGE_H__
#define __ELTE_OCX_LANGUAGE_H__

#include <map>
#include <vector>


#define GET_STRING_PARAM(ID) ID, _T(#ID)
typedef std::map<CString, CString> VALUE_MAP;
typedef std::vector<CString> STRING_LIST;
typedef VALUE_MAP LANG_MAP;

class Language
{
public:
	Language(void);
	~Language(void);

public:
	void LoadLanguage(int iLangId);
	CString GetString(UINT uiID, const CString& strKey);

	// ini 文件操作
private:
	BOOL GetAllValueMap(VALUE_MAP &valueMap) const;
	BOOL GetSectionNames(STRING_LIST &nameList) const;
	BOOL GetSectionValueMap(LPCTSTR strSection, VALUE_MAP& valueMap) const;

private:
	CString m_strFileName;
	LANG_MAP m_langMap;
};

#endif
