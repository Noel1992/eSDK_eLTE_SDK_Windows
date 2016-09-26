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


// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars



#include "eLTE_SDK.h"
#include "eLTE_Types.h"





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <GdiPlus.h>
#pragma comment(lib, "GdiPlus.lib")

#include "DataType.h"

static CString IntToStr(int value)
{
	CString str;
	str.Format(_T("%d"), value);
	return str;
}

static CString UIntToStr(unsigned int value)
{
	CString str;
	str.Format(_T("%d"), value);
	return str;
}

struct GroupInfo
{
	CString GroupID;
	CString GroupCategory;
	CString GroupPriority;
	CString GroupName;
	CString GroupCreator;
	CString GroupIndex;
	CString GroupVPNID;
};

struct UserInfo
{
	CString UserID;
	CString UserCategory;
	CString UserPriority;
	CString UserName;
};

struct GroupUserInfo
{
	CString UserID;
	CString GroupID;
	CString UserPriorityInGroup;
	CString MemberType;
	CString MemberGroupId;
};

typedef std::vector<GroupInfo*> GroupInfoList;
typedef std::vector<UserInfo*> UserInfoList;
typedef std::vector<GroupUserInfo*> GroupUserInfoList;
typedef std::list<CString> AttachFileList;	/** Attach file list */
//typedef std::vector<CString> VideoChannelList;
typedef std::map<CString, CString> VideoChannelList;

struct DcInfo
{
	CString DcID;
	CString Priviledge;
	CString Role;
	CString Alias;
};

struct VideoParamInfo
{
	CString strFormat;
	CString strCamera;
	CString strConfirm;
	CString strMute;

	VideoParamInfo()
	{
		strFormat = _T("");
		strCamera = _T("");
		strConfirm = _T("");
		strMute = _T("");
	}

	VideoParamInfo& operator = (const VideoParamInfo& v)
	{
		strFormat = v.strFormat;
		strCamera = v.strCamera;
		strConfirm = v.strConfirm;
		strMute = v.strMute;
		return *this;
	}

	void Reset()
	{
		strFormat = _T("");
		strCamera = _T("");
		strConfirm = _T("");
		strMute = _T("");
	}
};

struct DispatchParamInfo
{
	CString strFormat;
	std::list<CString> strDstViewerList;

	DispatchParamInfo()
	{
		strFormat = _T("");
		strDstViewerList.clear();
	}

	DispatchParamInfo& operator = (const DispatchParamInfo& v)
	{
		strFormat = v.strFormat;
		strDstViewerList = v.strDstViewerList;
		return *this;
	}

	void Reset()
	{
		strFormat = _T("");
		strDstViewerList.clear();
	}
};

struct UnDispatchParamInfo
{
	CString strUsrId;

	UnDispatchParamInfo()
	{
		strUsrId = _T("");
	}

	UnDispatchParamInfo& operator = (const UnDispatchParamInfo& v)
	{
		strUsrId = v.strUsrId;
		return *this;
	}

	void Reset()
	{
		strUsrId = _T("");
	}
};

struct DGNAParamInfo
{
	CString GroupID;
	CString DcID;
	CString Alias;
	CString Priority;
	CString MaxPeriod;
	CString GroupList[8];
	CString UserList[200];
};

struct SDSParamInfo
{
	CString sdstype;
	CString msgBody;
	CString receiver;
	AttachFileList files;
	CString msgId;
};

struct PCHGRPParamInfo
{
	CString DcID;
	CString GroupID;
	CString Alias;
	CString GroupList[20];
};
