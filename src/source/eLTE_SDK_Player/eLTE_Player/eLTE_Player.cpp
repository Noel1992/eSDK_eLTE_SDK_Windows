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

// eLTE_Player.cpp : Implementation of CeLTE_PlayerApp and DLL registration.

#include "stdafx.h"
#include "eLTE_Player.h"
#include <comcat.h>
#include <objsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CeLTE_PlayerApp theApp;

const GUID CDECL _tlid = { 0xEBB01215, 0xC409, 0x4D39, { 0xA1, 0x5D, 0x58, 0x16, 0xC3, 0x85, 0x2B, 0xB } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;
ULONG m_ulByMediaPass = 1;
const CATID CLSID_SafeItem ={0x6AB35B6F,0xD5D4,0x4DA3,{0x95,0x03,0x1A,0xD4,0xE5,0xE1,0xE5,0x89}};


// CeLTE_PlayerApp::InitInstance - DLL initialization

BOOL CeLTE_PlayerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CeLTE_PlayerApp::ExitInstance - DLL termination

int CeLTE_PlayerApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



//系统文件不修改
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)//lint !e715
{
	//USES_CONVERSION;

	ICatRegister* pcr = NULL;
	HRESULT hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		IID_ICatRegister, 
		(void**)&pcr);
	// Make sure the HKCR\Component Categories\{..catid...} key is registered.
	if (FAILED(hr))
	{
		return hr;
	}
	// Make sure the HKCR/Component Categories/{..catid}    
	// // key is registered.    
	CATEGORYINFO catinfo;
	catinfo.catid = catid;
	catinfo.lcid = 0x0409 ;     // english    

	// Make sure the provided description is not too long，
	// Only copy the first 127 characters if it is.
	int len = static_cast<int>(wcslen(catDescription)); 

	if (len > 127)
	{
		len = 127;
	}

	if(0 != wcsncpy_s(catinfo.szDescription, catDescription, (unsigned)len))
	{
		return S_FALSE;
	}
	catinfo.szDescription[len] = '\0';
	hr = pcr->RegisterCategories(1, &catinfo);
	pcr->Release();
	return hr;
}//lint !e1746 !e818

// HRESULT RegisterCLSIDInCategory -  Register your component categories information
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	// Register your component categories information.
	ICatRegister* pcr = NULL;
	HRESULT hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr); 
	if (SUCCEEDED(hr)) 
	{ 
		// Register this category as being "implemented" by the class.
		CATID rgcatid[1];
		rgcatid[0] = catid;
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
	{
		pcr->Release();
	}
	return hr;
} //lint !e1746  MFC自动生成的的函数

// HRESULT UnRegisterCLSIDInCategory -  Remove entries from the registry
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	ICatRegister *pcr = NULL ; 
	HRESULT hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,   
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);  

	if (SUCCEEDED(hr))
	{
		// Unregister this category as being "implemented" by the class.
		CATID rgcatid[1];
		rgcatid[0] = catid;
		hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
	{
		pcr->Release();
	}

	return hr;
} //lint !e1746  MFC自动生成的的函数
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	HRESULT hr;

	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
	{
		return ResultFromScode(SELFREG_E_TYPELIB);//lint !e835
	}

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
	{
		return ResultFromScode(SELFREG_E_CLASS);
	}
	hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data!");  //lint !e1776  MFC自动生成的的函数
	if (FAILED(hr))
	{
		return hr;
	}
	hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing); 
	if (FAILED(hr))      
	{
		return hr;   
	}
	hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls safely  scriptable!");   //lint !e1776  MFC自动生成的的函数
	if (FAILED(hr))
	{
		return hr;   
	}
	hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);  
	if (FAILED(hr))     
	{
		return hr;    
	}
	return NOERROR;
}//lint !e1788


// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	HRESULT hr;    
	AFX_MANAGE_STATE(_afxModuleAddrThis);   
	// Remove entries from the registry. 
	hr=UnRegisterCLSIDInCategory(CLSID_SafeItem,CATID_SafeForInitializing); 
	if (FAILED(hr)) 
	{
		return hr;
	}

	hr=UnRegisterCLSIDInCategory(CLSID_SafeItem,CATID_SafeForScripting);
	if (FAILED(hr))
	{      
		return hr; 
	}
	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
	{
		return ResultFromScode(SELFREG_E_TYPELIB);//lint !e835
	}
	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
	{ 
		return ResultFromScode(SELFREG_E_CLASS);
	}
	return NOERROR;
}//lint !e1788
