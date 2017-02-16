/*
Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
	   eSDK is licensed under the Apache License, Version 2.0 (the "License");
	   you may not use this file except in compliance with the License.
	   You may obtain a copy of the License at
	
	       http://www.apache.org/licenses/LICENSE-2.0

	
	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.

*/
/********************************************************************
filename    :    eLTE_Tool.h
author      :    yubinbin wx200189
created     :    2015/01/12
description :    公共方法接口类，主要对一些公共方法进行统一封装(服务端和客户端公共文件)
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 初始版本
*********************************************************************/

#ifndef __ELTE_TOOL_H__
#define __ELTE_TOOL_H__

#include <string>


class eLTE_Tool
{
private:
	eLTE_Tool();
	~eLTE_Tool();

	// 路径处理
public:
	static std::string GetServicePath();
	static std::string GetServicePathEx();
	static std::string GetDllPath(const std::string& strDllName);
	static void RelativePathToAbsolute(std::string& strPath);
	static BOOL CreateMultipleDirectory(const std::string& path);

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

	// 编码转换
public:	
	static std::string UnicodeToANSI(const std::wstring& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static std::wstring ANSIToUnicode(const std::string& str);
	static std::wstring UTF8ToUnicode(const std::string& str);
	static std::string UTF8ToANSI(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str);

	// IP处理
public:
	static std::string GetIPByUIntValue(unsigned long ulIP);
	static int subStrIPort(const std::string& str,std::string& _ip,std::string& _port);

	// 密码处理
public:
	static std::string GetEncryptPassword(const std::string& strPath);

	// 密码加密处理
private:
	static int Sha256AndBase64Encrypt(const char* pInBuf, unsigned char* pOutBuf, int& iOutLen);

public:
	//特殊字符替换
	static void ReplaceString(const std::string& strSrc, std::string& strDst, const std::string& strReplace, const std::string& strReplaceed);
};

#endif
