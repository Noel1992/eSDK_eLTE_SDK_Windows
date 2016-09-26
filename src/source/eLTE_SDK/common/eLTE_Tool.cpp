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
#include "eLTE_Error.h"
#include "eLTE_Message.h"
#include "eLTE_Log.h"
#include "crypto/crypto_api.h"
#include "eSDK_Securec.h"

#pragma comment(lib, "Shlwapi.lib")


#define STR_LENGTH_SIZE     20
#define INT_FORMAT_D	    10
#define DOUBLE_STR_SIZE	    64
#define DOUBLE_FORMAT	    "%g"
#define UINT_FORMAT		    "%u"

#define SECTION_SERVER_INFO			"ServerInfo"
#define KEY_NEED_ENCRYPT            "NeedEncrypt"
#define KEY_PASSWORD                "Password"
#define SPECIAL_STRING "�١٢�"

// �����ļ���ȡ����ֵ���
#define CHECK_READ_CONFIG_FILE(x)			\
	if(0 == x)								\
	{										\
	return "";	\
	}

eLTE_Tool::eLTE_Tool()
{

}

eLTE_Tool::~eLTE_Tool()
{

}

std::string eLTE_Tool::GetServicePath()
{
	char path[MAX_PATH] = {0};
	GetModuleFileName(NULL, path, MAX_PATH);
	std::string strPath(path);
	strPath = strPath.substr(0, strPath.rfind("\\")+1);
	return strPath;
}

std::string eLTE_Tool::GetServicePathEx()
{
	char path[MAX_PATH] = {0};
	GetModuleFileName(NULL, path, MAX_PATH);
	std::string strPath(path);
	return strPath;
}

std::string eLTE_Tool::GetDllPath(const std::string& strDllName)
{
	HMODULE hModule = GetModuleHandle(strDllName.c_str());
	if(NULL == hModule)
	{
		return "";
	}
	char dllpath[MAX_PATH];
	eSDK_MEMSET(dllpath, 0, sizeof(dllpath));
	::GetModuleFileName(hModule, dllpath, MAX_PATH);
	std::string strPath(dllpath);
	strPath = strPath.substr(0, strPath.rfind("\\")+1);
	return strPath;
}

void eLTE_Tool::RelativePathToAbsolute(std::string& strPath)
{
	if (PathIsRelative(strPath.c_str()))
	{
		std::string strSvc = GetServicePath();
		strSvc.append(strPath);
		strPath = strSvc;
	}
}

BOOL eLTE_Tool::CreateMultipleDirectory(const std::string& path)
{
	std::string strPath(path);

	// �жϿ�
	if (strPath.empty())
	{
		return FALSE;
	}
	// ����Ѿ����ڷ��سɹ�
	if (PathFileExists(strPath.c_str()))
	{
		return TRUE;
	}
	else
	{
		// ��ȥ��β��"\\"
		while ('\\' == strPath.back())
		{
			strPath = strPath.substr(0, strPath.rfind("\\"));
		}

		// ��ȡ�ϼ�Ŀ¼
		std::string prePath = strPath.substr(0, strPath.rfind("\\"));

		// �����ڵ�Ŀ¼
		if (prePath == strPath)
		{
			return FALSE;
		}

		if (PathFileExists(prePath.c_str()))
		{
			return CreateDirectoryA(strPath.c_str(), NULL);
		}
		else
		{
			// ���ǰ���Ŀ¼�����ڣ����ȴ���ǰ���Ŀ¼
			if (CreateMultipleDirectory(prePath))
			{
				// ǰ��Ŀ¼���������Ժ����ٴ��������Ŀ¼
				return CreateMultipleDirectory(strPath);
			}
			return FALSE;
		}
	}
}

// int ת��Ϊ std::string
std::string eLTE_Tool::Int2String(int iVar)
{
	char buf[STR_LENGTH_SIZE] = {0};
	(void)_itoa_s(iVar, buf, INT_FORMAT_D);
	return std::string(buf);
}

// unsigned int ת��Ϊ std::string
std::string eLTE_Tool::UInt2String(unsigned int iVar)
{
	char _Buf[STR_LENGTH_SIZE];
	(void)sprintf_s(_Buf, sizeof(_Buf), UINT_FORMAT, iVar);
	return std::string(_Buf);
}

// double װ��Ϊ std::string
std::string eLTE_Tool::Double2String(double dVar)
{
	char _Buf[DOUBLE_STR_SIZE];
	(void)sprintf_s(_Buf, sizeof(_Buf), DOUBLE_FORMAT, dVar);
	return std::string(_Buf);
}

// std::string ת��Ϊ int
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

// std::string ת��Ϊ unsigned int
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

// std::wstring ת��Ϊ int
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

// std::wstring ת��Ϊ unsigned int
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

std::string eLTE_Tool::UnicodeToANSI(const std::wstring& str)
{
	std::string strResult("");

	int textlen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	if (0 >= textlen)
	{
		// WideCharToMultiByte failed.
		return strResult;
	}

	size_t bufsize = (size_t)(textlen+1);
	char* pBuf = new char[bufsize];
	eSDK_MEMSET(pBuf, 0, sizeof(char)*bufsize);
	WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pBuf, textlen, NULL, NULL);//lint !e713

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
	eSDK_MEMSET(pBuf, 0, sizeof(char)*bufsize);
	WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pBuf, textlen, NULL, NULL);//lint !e713

	strResult = pBuf;
	delete[] pBuf;

	return strResult;
}

std::wstring eLTE_Tool::ANSIToUnicode(const std::string& str)
{
	std::wstring strResult(L"");

	int textlen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (0 >= textlen)
	{
		// MultiByteToWideChar failed.
		return strResult;
	}

	size_t bufsize = (size_t)(textlen+1);
	wchar_t* pBuf = new wchar_t[bufsize];
	eSDK_MEMSET(pBuf, 0, sizeof(wchar_t)*bufsize);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pBuf, textlen);//lint !e713

	strResult = pBuf;
	delete[] pBuf;

	return strResult;
}

std::wstring eLTE_Tool::UTF8ToUnicode(const std::string& str)
{
	std::wstring strResult(L"");
	int textlen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	if (0 >= textlen)
	{
		// MultiByteToWideChar failed.
		return strResult;
	}

	size_t bufsize = (size_t)(textlen+1);
	wchar_t* pBuf = new wchar_t[bufsize];
	eSDK_MEMSET(pBuf, 0, sizeof(wchar_t)*bufsize);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pBuf, textlen);//lint !e713

	strResult = pBuf;
	delete[] pBuf;

	return strResult;
}

std::string eLTE_Tool::UTF8ToANSI(const std::string& str)
{
	std::wstring strUnicode = UTF8ToUnicode(str);
	std::string strAnsi = UnicodeToANSI(strUnicode);
	return strAnsi;
}

std::string eLTE_Tool::ANSIToUTF8(const std::string& str)
{
	std::wstring strUnicode = ANSIToUnicode(str);
	std::string strUtf8 = UnicodeToUTF8(strUnicode);
	return strUtf8;
}

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

std::string eLTE_Tool::GetEncryptPassword(const std::string& strPath)
{
	const int LENGTH = 256;
	char val[LENGTH] = {0};
	std::string strPassword = "";

	eSDK_MEMSET(val, 0, sizeof(char)*LENGTH);
	unsigned long dwRet = GetPrivateProfileString(SECTION_SERVER_INFO, KEY_NEED_ENCRYPT, "", val, LENGTH, strPath.c_str());
	CHECK_READ_CONFIG_FILE(dwRet);
	std::string m_strNeedEncrypt = val;

	eSDK_MEMSET(val, 0, sizeof(char)*LENGTH);
	dwRet = GetPrivateProfileString(SECTION_SERVER_INFO, KEY_PASSWORD, "", val, LENGTH, strPath.c_str());
	CHECK_READ_CONFIG_FILE(dwRet);
	strPassword = val;

	transform(m_strNeedEncrypt.begin(),m_strNeedEncrypt.end(),m_strNeedEncrypt.begin(),tolower);
	if (0 == m_strNeedEncrypt.compare("true"))
	{
		//��������
		unsigned char encryptPwd[BUFFER_SIZE] = {0};
		int iEncryptLen = BUFFER_SIZE;
		int iRet = Sha256AndBase64Encrypt(val, encryptPwd, iEncryptLen);
		if(0 != iRet)
		{
			LOG_RUN_ERROR("Encrypt password failed, error code[%d]", iRet);
			return "";
		}
		std::string strNewPassword;
		if(std::string::npos == strPassword.find(SPECIAL_STRING))
		{
			//��һ�μ���
			strNewPassword = SPECIAL_STRING;
			strNewPassword.append((char*)encryptPwd);
			dwRet = WritePrivateProfileString(SECTION_SERVER_INFO, KEY_PASSWORD, strNewPassword.c_str(), strPath.c_str());
			CHECK_READ_CONFIG_FILE(dwRet);
			//�ټ�һ����
			eSDK_MEMSET(encryptPwd, 0x00, BUFFER_SIZE);
			iRet = Sha256AndBase64Encrypt(strNewPassword.c_str(), encryptPwd, iEncryptLen);
			if(0 != iRet)
			{
				LOG_RUN_ERROR("Encrypt password failed, error code[%d]", iRet);
				return "";
			}
		}
		strPassword = (char*)encryptPwd;
	}
	return strPassword;
}

int eLTE_Tool::Sha256AndBase64Encrypt(const char* pInBuf, unsigned char* pOutBuf, int& iOutLen)
{
	unsigned int iEncryptLen = 0;
	if(0 != SHA256_ENCRYPT((unsigned char*)pInBuf, strlen(pInBuf), pOutBuf, iEncryptLen))//lint !e1773
	{
		LOG_RUN_ERROR("SHA256_ENCRYPT failed");
		return -1;
	}
	unsigned char strBuf[BUFFER_SIZE]={0};
	eSDK_MEMCPY(strBuf, sizeof(strBuf), pOutBuf, iEncryptLen);
	eSDK_MEMSET(pOutBuf, 0x00, iEncryptLen);
	iOutLen = BUFFER_SIZE;
	if(TRUE != BASE64_ENCRYPT(BINARY_NO_FORMAT, strBuf, iEncryptLen, pOutBuf, iOutLen))//lint !e1773 !e713
	{
		LOG_RUN_ERROR("BASE64_ENCRYPT failed");
		return -1;
	}

	return 0;
}//lint !e1762

void eLTE_Tool::ReplaceString(const std::string& strSrc, std::string& strDst, const std::string& strReplace, const std::string& strReplaceed)
{
	strDst = strSrc;
	std::string::size_type pos = 0;
	std::string::size_type replaceSize = strReplace.size();
	std::string::size_type replaceedSize = strReplaceed.size();

	while((pos = strDst.find(strReplace, pos)) != std::string::npos)
	{
		strDst.replace(pos, replaceSize, strReplaceed);
		pos += replaceedSize;
	}
}

int eLTE_Tool::IsDigit(const std::string& strRec)
{
	for(unsigned i = 0; i < strRec.length(); ++i)
	{
		if(!isdigit(strRec[i]))
		{
			return eLTE_SVC_ERR_FAILED;
		}
	}
	return eLTE_SVC_ERR_SUCCESS;
}