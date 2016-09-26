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
filename    :    eLTE_Tool.h
author      :    zhangziqian zwx229156
created     :    2016/05/30
description :    公共方法接口类，主要对一些公共方法进行统一封装
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2016/05/30 初始版本
*********************************************************************/

#ifndef __ELTE_TOOL_H__
#define __ELTE_TOOL_H__

#include <string>


class eLTE_Tool
{
private:
	eLTE_Tool();
	~eLTE_Tool();

	// type converter
public:
	// int converter to std::string
	static std::string Int2String(int iVar);
	// unsigned int converter to std::string
	static std::string UInt2String(unsigned int iVar);
	// double converter to std::string
	static std::string Double2String(double dVar);
	// std::string converter to int
	static int String2Int(const std::string& str);
	// std::string converter to unsigned int
	static unsigned int String2UInt(const std::string& str);
	// std::wstring converter to int
	static int WString2Int(const std::wstring& wstr);
	// std::wstring converter to unsigned int
	static unsigned int WString2UInt(const std::wstring& wstr);

	// encode convertion
public:	
	static std::string UnicodeToANSI(const CString& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static CString ANSIToUnicode(const std::string& str);
	static CString UTF8ToUnicode(const std::string& str);
	static std::string UTF8ToANSI(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str);

	// IP process
public:
	static std::string GetIPByUIntValue(unsigned long ulIP);
};

#endif
