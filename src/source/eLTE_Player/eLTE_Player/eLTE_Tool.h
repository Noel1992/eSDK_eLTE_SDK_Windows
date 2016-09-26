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
author      :    yubinbin wx200189
created     :    2014/07/01
description :    工具管理类。抽取内部使用的公用方法统一管理
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/01 初始版本
*********************************************************************/

#ifndef __ELTE_OCX_TOOL_H__
#define __ELTE_OCX_TOOL_H__

#include <sstream>


class eLTE_Tool
{
	// 文件路径
public:	
	static CString GetOcxPath();
	static std::string GetOcxPathA();
	static CString GetAppPath();
	static std::string GetAppPathA();
	static BOOL IsExistPath(const CString& path);
	static BOOL IsExistPath(const std::string& path);
	static BOOL CreateMultipleDirectory(const CString& path);
	static BOOL CreateMultipleDirectory(const std::string& path);
	static void RelativePathToAbsolute(CString& path);
	static void RelativePathToAbsolute(std::string& path);
	static void RelativePathToAbsoluteOcx(CString& path);
	static void RelativePathToAbsoluteOcx(std::string& path);

	// 编码转换
public:	
	static std::string UnicodeToANSI(const std::wstring& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static std::wstring ANSIToUnicode(const std::string& str);
	static std::wstring UTF8ToUnicode(const std::string& str);
	static std::string UTF8ToANSI(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str);

	// 运行参数设置
public:
	static BOOL ChangeEnvironmentVariable();

	// 获取OCX版本
public:
	static CString GetOcxVersion();

	// IP处理
public:
	static std::string GetIPByUIntValue(unsigned long ulIP);

	// 类型转换
public:
	// int 转换为 std::string
	static std::string Int2String(int iVar);
	// unsigned int 转换为 std::string
	static std::string UInt2String(unsigned int iVar);
	// double 装换为 std::string
	static std::string Double2String(double dVar);
	// std::string 转换为 int
	static int String2Int(const std::string& str);
	// std::string 转换为 unsigned int
	static unsigned int String2UInt(const std::string& str);
	// std::wstring 转换为 int
	static int WString2Int(const std::wstring& wstr);
	// std::wstring 转换为 unsigned int
	static unsigned int WString2UInt(const std::wstring& wstr);

	// 字符串替换
public:
	/**************************************************************************
	* name			: ReplaceString
	* description   : 字符串替换
	* input			: strSrc 源字符串
	* output		: strDst 目的字符串
					：strReplace 需要替换的字符串
					：strReplaceed 替换后的字符串
	* return		: 无
	**************************************************************************/
	static void ReplaceString(const std::string& strSrc, std::string& strDst, const std::string& strReplace, const std::string& strReplaceed);

private:
	eLTE_Tool(void);
	~eLTE_Tool(void);
};

#endif
