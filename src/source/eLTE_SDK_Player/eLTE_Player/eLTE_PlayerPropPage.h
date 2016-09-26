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

#pragma once

// eLTE_PlayerPropPage.h : Declaration of the CeLTE_PlayerPropPage property page class.


// CeLTE_PlayerPropPage : See eLTE_PlayerPropPage.cpp for implementation.

class CeLTE_PlayerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CeLTE_PlayerPropPage)
	DECLARE_OLECREATE_EX(CeLTE_PlayerPropPage)//lint !e1768

// Constructor
public:
	CeLTE_PlayerPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_ELTE_PLAYER };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};//lint !e1712

