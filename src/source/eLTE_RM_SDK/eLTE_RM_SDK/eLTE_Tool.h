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
description :    ���������ӿ��࣬��Ҫ��һЩ������������ͳһ��װ(����˺Ϳͻ��˹����ļ�)
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 ��ʼ�汾
*********************************************************************/

#ifndef __ELTE_TOOL_H__
#define __ELTE_TOOL_H__

#include <string>


class eLTE_Tool
{
private:
	eLTE_Tool();
	~eLTE_Tool();

	// ·������
public:
	static std::string GetServicePath();
	static std::string GetServicePathEx();
	static std::string GetDllPath(const std::string& strDllName);
	static void RelativePathToAbsolute(std::string& strPath);
	static BOOL CreateMultipleDirectory(const std::string& path);

	// ����ת��
public:
	// int ת��Ϊ std::string
	static std::string Int2String(int iVar);
	// unsigned int ת��Ϊ std::string
	static std::string UInt2String(unsigned int iVar);
	// double װ��Ϊ std::string
	static std::string Double2String(double dVar);
	// std::string ת��Ϊ int
	static int String2Int(const std::string& str);
	// std::string ת��Ϊ unsigned int
	static unsigned int String2UInt(const std::string& str);
	// std::wstring ת��Ϊ int
	static int WString2Int(const std::wstring& wstr);
	// std::wstring ת��Ϊ unsigned int
	static unsigned int WString2UInt(const std::wstring& wstr);

	// ����ת��
public:	
	static std::string UnicodeToANSI(const std::wstring& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static std::wstring ANSIToUnicode(const std::string& str);
	static std::wstring UTF8ToUnicode(const std::string& str);
	static std::string UTF8ToANSI(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str);

	// IP����
public:
	static std::string GetIPByUIntValue(unsigned long ulIP);
	static int subStrIPort(const std::string& str,std::string& _ip,std::string& _port);

	// ���봦��
public:
	static std::string GetEncryptPassword(const std::string& strPath);

	// ������ܴ���
private:
	static int Sha256AndBase64Encrypt(const char* pInBuf, unsigned char* pOutBuf, int& iOutLen);

public:
	//�����ַ��滻
	static void ReplaceString(const std::string& strSrc, std::string& strDst, const std::string& strReplace, const std::string& strReplaceed);
};

#endif
