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
#include <Shlwapi.h>
#include <algorithm>
#include "eLTE_Tool.h"


#pragma comment(lib, "Shlwapi.lib")


#define STR_LENGTH_SIZE     20
#define INT_FORMAT_D	    10
#define DOUBLE_STR_SIZE	    64
#define DOUBLE_FORMAT	    "%g"
#define UINT_FORMAT		    "%u"



eLTE_Tool::eLTE_Tool()
{

}

eLTE_Tool::~eLTE_Tool()
{

}

// int convert std::string
std::string eLTE_Tool::Int2String(int iVar)
{
	char buf[STR_LENGTH_SIZE] = {0};
	(void)_itoa_s(iVar, buf, INT_FORMAT_D);
	return std::string(buf);
}

// unsigned int convert std::string
std::string eLTE_Tool::UInt2String(unsigned int iVar)
{
	char _Buf[STR_LENGTH_SIZE];
	(void)sprintf_s(_Buf, sizeof(_Buf), UINT_FORMAT, iVar);
	return std::string(_Buf);
}

// double convert std::string
std::string eLTE_Tool::Double2String(double dVar)
{
	char _Buf[DOUBLE_STR_SIZE];
	(void)sprintf_s(_Buf, sizeof(_Buf), DOUBLE_FORMAT, dVar);
	return std::string(_Buf);
}

// std::string convert int
int eLTE_Tool::String2Int(const std::string& str)
{
	int iRet = 0;

	try
	{
		iRet = std::stoi(str);
	}
	catch (...)
	{
		iRet = 0;
	}

	return iRet;
}

// std::string convert unsigned int
unsigned int eLTE_Tool::String2UInt(const std::string& str)
{
	unsigned int uiRet = 0;

	try
	{
		uiRet = std::stoul(str);
	}
	catch (...)
	{
		uiRet = 0;
	}

	return uiRet;
}

// std::wstring convert int
int eLTE_Tool::WString2Int(const std::wstring& wstr)
{
	int iRet = 0;

	try
	{
		iRet = std::stoi(wstr);
	}
	catch (...)
	{
		iRet = 0;
	}

	return iRet;
}

// std::wstring convert unsigned int
unsigned int eLTE_Tool::WString2UInt(const std::wstring& wstr)
{
	unsigned int uiRet = 0;

	try
	{
		uiRet = std::stoul(wstr);
	}
	catch (...)
	{
		uiRet = 0;
	}

	return uiRet;
}

std::string eLTE_Tool::UnicodeToANSI(const CString& str)
{
	std::string strResult("");

	int textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	if (0 >= textlen)
	{
		// WideCharToMultiByte failed.
		return strResult;
	}

	size_t bufsize = (size_t)(textlen+1);
	char* pBuf = new char[bufsize];
	memset(pBuf, 0, sizeof(char)*bufsize);
	WideCharToMultiByte(CP_ACP, 0, str, -1, pBuf, textlen, NULL, NULL);//lint !e713

	strResult = pBuf;
	delete[] pBuf;

	return strResult;
}

std::string eLTE_Tool::UnicodeToUTF8(const std::wstring& str)
{
	std::string strResult("");

	int textlen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	if (0 >= textlen)
	{
		// WideCharToMultiByte failed.
		return strResult;
	}

	size_t bufsize = (size_t)(textlen+1);
	char* pBuf = new char[bufsize];
	memset(pBuf, 0, sizeof(char)*bufsize);
	WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pBuf, textlen, NULL, NULL);//lint !e713

	strResult = pBuf;
	delete[] pBuf;

	return strResult;
}

CString eLTE_Tool::ANSIToUnicode(const std::string& str)
{
	CString strResult(L"");

	int textlen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (0 >= textlen)
	{
		// MultiByteToWideChar failed.
		return strResult;
	}

	size_t bufsize = (size_t)(textlen+1);
	wchar_t* pBuf = new wchar_t[bufsize];
	memset(pBuf, 0, sizeof(wchar_t)*bufsize);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pBuf, textlen);//lint !e713

	strResult = pBuf;
	delete[] pBuf;

	return strResult;
}

CString eLTE_Tool::UTF8ToUnicode(const std::string& str)
{
	CString strResult(L"");
	int textlen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	if (0 >= textlen)
	{
		// MultiByteToWideChar failed.
		return strResult;
	}

	size_t bufsize = (size_t)(textlen+1);
	wchar_t* pBuf = new wchar_t[bufsize];
	memset(pBuf, 0, sizeof(wchar_t)*bufsize);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pBuf, textlen);//lint !e713

	strResult = pBuf;
	delete[] pBuf;

	return strResult;
}

//std::string eLTE_Tool::UTF8ToANSI(const std::string& str)
//{
//	std::wstring strUnicode = UTF8ToUnicode(str);
//	std::string strAnsi = UnicodeToANSI(strUnicode);
//	return strAnsi;
//}

//std::string eLTE_Tool::ANSIToUTF8(const std::string& str)
//{
//	std::wstring strUnicode = ANSIToUnicode(str);
//	std::string strUtf8 = UnicodeToUTF8(strUnicode);
//	return strUtf8;
//}

std::string eLTE_Tool::GetIPByUIntValue(unsigned long ulIP)
{
	int val1 = (ulIP & 0xFF000000) >> 24;
	int val2 = (ulIP & 0x00FF0000) >> 16;
	int val3 = (ulIP & 0x0000FF00) >> 8;
	int val4 = (ulIP & 0x000000FF);

	std::string str = ".";
	std::string strIP
		= Int2String(val1)
		+ str
		+ Int2String(val2)
		+ str
		+ Int2String(val3)
		+ str
		+ Int2String(val4);

	return strIP;
}


