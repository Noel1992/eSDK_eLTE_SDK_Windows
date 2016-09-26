#include "stdafx.h"
#include <Shlwapi.h>
#include <algorithm>
//public function interface class 
#include "eLTE_Tool.h"
//Error Code define class
#include "eLTE_Error.h"
//#include "eLTE_Message.h"
//log manage class 
#include "eLTE_Log.h"

#pragma comment(lib, "Shlwapi.lib")


#define STR_LENGTH_SIZE     20
#define INT_FORMAT_D	    10
#define DOUBLE_STR_SIZE	    64
#define DOUBLE_FORMAT	    "%g"
#define UINT_FORMAT		    "%u"

#define SECTION_SERVER_INFO			"ServerInfo"
#define KEY_NEED_ENCRYPT            "NeedEncrypt"
#define KEY_PASSWORD                "Password"
#define SPECIAL_STRING "①≠②"

// 配置文件读取返回值检测
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
	memset(dllpath, 0, sizeof(dllpath));
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

	// 判断空
	if (strPath.empty())
	{
		return FALSE;
	}
	// 如果已经存在返回成功
	if (PathFileExists(strPath.c_str()))
	{
		return TRUE;
	}
	else
	{
		// 除去结尾的"\\"
		while ('\\' == strPath.back())
		{
			strPath = strPath.substr(0, strPath.rfind("\\"));
		}

		// 获取上级目录
		std::string prePath = strPath.substr(0, strPath.rfind("\\"));

		// 不存在的目录
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
			// 如果前面的目录不存在，则先创建前面的目录
			if (CreateMultipleDirectory(prePath))
			{
				// 前面目录创建好了以后，则再创建后面的目录
				return CreateMultipleDirectory(strPath);
			}
			return FALSE;
		}
	}
}

// int 转换为 std::string
std::string eLTE_Tool::Int2String(int iVar)
{
	char buf[STR_LENGTH_SIZE] = {0};
	(void)_itoa_s(iVar, buf, INT_FORMAT_D);
	return std::string(buf);
}

// unsigned int 转换为 std::string
std::string eLTE_Tool::UInt2String(unsigned int iVar)
{
	char _Buf[STR_LENGTH_SIZE];
	(void)sprintf_s(_Buf, sizeof(_Buf), UINT_FORMAT, iVar);
	return std::string(_Buf);
}

// double 装换为 std::string
std::string eLTE_Tool::Double2String(double dVar)
{
	char _Buf[DOUBLE_STR_SIZE];
	(void)sprintf_s(_Buf, sizeof(_Buf), DOUBLE_FORMAT, dVar);
	return std::string(_Buf);
}

// std::string 转换为 int
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

// std::string 转换为 unsigned int
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

int eLTE_Tool::subStrIPort(const std::string& str,std::string& _ip,std::string& _port)
{
	int iRet=0;
	try
	{
		int startIndex=str.find(',');
		_ip=str.substr(0,startIndex);
		_port=str.substr(startIndex+1);
	}
	catch (...)
	{
		iRet=-1;
	}
	return iRet;
}

// std::wstring 转换为 int
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

// std::wstring 转换为 unsigned int
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
	memset(pBuf, 0, sizeof(char)*bufsize);
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
	memset(pBuf, 0, sizeof(char)*bufsize);
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
	memset(pBuf, 0, sizeof(wchar_t)*bufsize);
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
	memset(pBuf, 0, sizeof(wchar_t)*bufsize);
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