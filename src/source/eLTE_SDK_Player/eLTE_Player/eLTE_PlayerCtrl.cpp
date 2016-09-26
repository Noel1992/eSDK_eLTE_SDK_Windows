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

// eLTE_PlayerCtrl.cpp : Implementation of the CeLTE_PlayerCtrl ActiveX Control class.
#include "stdafx.h"
#include "eLTE_Player.h"
#include "eLTE_PlayerCtrl.h"
#include "eLTE_PlayerPropPage.h"
#include "afxdialogex.h"
#include "eLTE_Tool.h"
#include "eLTE_Log.h"
#include "XMLProcess.h"
#include "eLTE_Xml.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ELTE_SDK        "\\elte_sdk"

//lint -e1788 -e666
IMPLEMENT_DYNCREATE(CeLTE_PlayerCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CeLTE_PlayerCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_ELTE_EXIT_FULL_SCREEN, OnMsgExitFullScreen)
	ON_MESSAGE(WM_ELTE_POST_EVENT, OnMsgPostEvent)
	ON_MESSAGE(WM_ELTE_POST_PLAYER_EVENT, OnMsgPostPlayerEvent)
	ON_MESSAGE(WM_ELTE_VIDEO_EXIT, OnMsgStopVideoPlay)
END_MESSAGE_MAP()//lint -e786



// Dispatch map

BEGIN_DISPATCH_MAP(CeLTE_PlayerCtrl, COleControl)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_SetLogPath", dispidELTE_OCX_SetLogPath, ELTE_OCX_SetLogPath, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_SetLogLevel", dispidELTE_OCX_SetLogLevel, ELTE_OCX_SetLogLevel, VT_BSTR, VTS_UI4)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_UploadLogo", dispidELTE_OCX_UploadLogo, ELTE_OCX_UploadLogo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetVersion", dispidELTE_OCX_GetVersion, ELTE_OCX_GetVersion, VT_BSTR, VTS_UI4)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_SetLanguage", dispidELTE_OCX_SetLanguage, ELTE_OCX_SetLanguage, VT_BSTR, VTS_UI4)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_ShowToolbar", dispidELTE_OCX_ShowToolbar, ELTE_OCX_ShowToolbar, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_Load", dispidELTE_OCX_Load, ELTE_OCX_Load, VT_BSTR, VTS_UI4)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_UnLoad", dispidELTE_OCX_UnLoad, ELTE_OCX_UnLoad, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_Login", dispidELTE_OCX_Login, ELTE_OCX_Login, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_Logout", dispidELTE_OCX_Logout, ELTE_OCX_Logout, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_TriggerStatusReport", dispidELTE_OCX_TriggerStatusReport, ELTE_OCX_TriggerStatusReport, VT_BSTR, VTS_UI4)
	//DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_ProvisionManagerInit", dispidELTE_OCX_ProvisionManagerInit, ELTE_OCX_ProvisionManagerInit, VT_BSTR, VTS_BSTR VTS_BSTR)
	//DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_ProvisionManagerExit", dispidELTE_OCX_ProvisionManagerExit, ELTE_OCX_ProvisionManagerExit, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetDcGroups", dispidELTE_OCX_GetDcGroups, ELTE_OCX_GetDcGroups, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetDcUsers", dispidELTE_OCX_GetDcUsers, ELTE_OCX_GetDcUsers, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetGroupUsers", dispidELTE_OCX_GetGroupUsers, ELTE_OCX_GetGroupUsers, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetGroupInfo", dispidELTE_OCX_GetGroupInfo, ELTE_OCX_GetGroupInfo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetUserInfo", dispidELTE_OCX_GetUserInfo, ELTE_OCX_GetUserInfo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetDcInfo", dispidELTE_OCX_GetDcInfo, ELTE_OCX_GetDcInfo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_UnSubscribeGroup", dispidELTE_OCX_UnSubscribeGroup, ELTE_OCX_UnSubscribeGroup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetUserRECFileInfoList", dispidELTE_OCX_GetUserRECFileInfoList, ELTE_OCX_GetUserRECFileInfoList, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_StartRealPlay", dispidELTE_OCX_StartRealPlay, ELTE_OCX_StartRealPlay, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_ReverseRealPlay", dispidELTE_OCX_ReverseRealPlay, ELTE_OCX_ReverseRealPlay, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_StopRealPlay", dispidELTE_OCX_StopRealPlay, ELTE_OCX_StopRealPlay, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_ShowRealPlay", dispidELTE_OCX_ShowRealPlay, ELTE_OCX_ShowRealPlay, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_Snapshot", dispidELTE_OCX_Snapshot, ELTE_OCX_Snapshot, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_HideRealPlay", dispidELTE_OCX_HideRealPlay, ELTE_OCX_HideRealPlay, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_PlaySound", dispidELTE_OCX_PlaySound, ELTE_OCX_PlaySound, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_StopSound", dispidELTE_OCX_StopSound, ELTE_OCX_StopSound, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_SetVideoWindowPos", dispidELTE_OCX_SetVideoWindowPos, ELTE_OCX_SetVideoWindowPos, VT_BSTR, VTS_UI4 VTS_UI4 VTS_UI4 VTS_UI4)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_FullScreenDisplay", dispidELTE_OCX_FullScreenDisplay, ELTE_OCX_FullScreenDisplay, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_NormalScreenDisplay", dispidELTE_OCX_NormalScreenDisplay, ELTE_OCX_NormalScreenDisplay, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_SetTitleText", dispidELTE_OCX_SetTitleText, ELTE_OCX_SetTitleText, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_CreateDynamicGroup", dispidELTE_OCX_CreateDynamicGroup, ELTE_OCX_CreateDynamicGroup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_CancelDynamicGroup", dispidELTE_OCX_CancelDynamicGroup, ELTE_OCX_CancelDynamicGroup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_CreateTempGroup", dispidELTE_OCX_CreateTempGroup, ELTE_OCX_CreateTempGroup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetNextTempGroupID", dispidELTE_OCX_GetNextTempGroupID, ELTE_OCX_GetNextTempGroupID, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_P2PDial", dispidELTE_OCX_P2PDial, ELTE_OCX_P2PDial, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_P2PRecv", dispidELTE_OCX_P2PRecv, ELTE_OCX_P2PRecv, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_P2PReject", dispidELTE_OCX_P2PReject, ELTE_OCX_P2PReject, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_P2PHangup", dispidELTE_OCX_P2PHangup, ELTE_OCX_P2PHangup, VT_BSTR, VTS_BSTR)
	//DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_JoinGroup", dispidELTE_OCX_JoinGroup, ELTE_OCX_JoinGroup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_SubJoinGroup", dispidELTE_OCX_SubJoinGroup, ELTE_OCX_SubJoinGroup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_PTTDial", dispidELTE_OCX_PTTDial, ELTE_OCX_PTTDial, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_PTTRelease", dispidELTE_OCX_PTTRelease, ELTE_OCX_PTTRelease, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_PTTHangup", dispidELTE_OCX_PTTHangup, ELTE_OCX_PTTHangup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_PTTEmergency", dispidELTE_OCX_PTTEmergency, ELTE_OCX_PTTEmergency, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GroupBreakoff", dispidELTE_OCX_GroupBreakoff, ELTE_OCX_GroupBreakoff, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_P2PBreakin", dispidELTE_OCX_P2PBreakin, ELTE_OCX_P2PBreakin, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_P2PBreakoff", dispidELTE_OCX_P2PBreakoff, ELTE_OCX_P2PBreakoff, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_VolMute", dispidELTE_OCX_VolMute, ELTE_OCX_VolMute, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_VolUnMute", dispidELTE_OCX_VolUnMute, ELTE_OCX_VolUnMute, VT_BSTR, VTS_BSTR VTS_BSTR)
	//DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_MuteControl", dispidELTE_OCX_MuteControl, ELTE_OCX_MuteControl, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GISSubscribe", dispidELTE_OCX_GISSubscribe, ELTE_OCX_GISSubscribe, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_SetBypassBuildMedia", dispidELTE_OCX_SetBypassBuildMedia, ELTE_OCX_SetBypassBuildMedia, VT_BSTR, VTS_UI4)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_ProvisionManagerInitMRS", dispidELTE_OCX_ProvisionManagerInitMRS, ELTE_OCX_ProvisionManagerInitMRS, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_SDSSendMessage", dispidELTE_OCX_SDSSendMessage, ELTE_OCX_SDSSendMessage, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_StartVideoDispatch", dispidELTE_OCX_StartVideoDispatch, ELTE_OCX_StartVideoDispatch, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_StopVideoDispatch", dispidELTE_OCX_StopVideoDispatch, ELTE_OCX_StopVideoDispatch, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_RecvVideoPlay", dispidELTE_OCX_RecvVideoPlay, ELTE_OCX_RecvVideoPlay, VT_BSTR, VTS_BSTR VTS_BSTR)

	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetDcVWallIDList", dispidELTE_OCX_GetDcVWallIDList, ELTE_OCX_GetDcVWallIDList, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_VWallStart", dispidELTE_OCX_VWallStart, ELTE_OCX_VWallStart, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_VWallStop", dispidELTE_OCX_VWallStop, ELTE_OCX_VWallStop, VT_BSTR, VTS_BSTR VTS_BSTR)

	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_TelephoneDial", dispidELTE_OCX_TelephoneDial, ELTE_OCX_TelephoneDial, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_TelephoneHangup", dispidELTE_OCX_TelephoneHangup, ELTE_OCX_TelephoneHangup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_StartDiscreetListen", dispidELTE_OCX_StartDiscreetListen, ELTE_OCX_StartDiscreetListen, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_StopDiscreetListen", dispidELTE_OCX_StopDiscreetListen, ELTE_OCX_StopDiscreetListen, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_StartEnvironmentListen", dispidELTE_OCX_StartEnvironmentListen, ELTE_OCX_StartEnvironmentListen, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_P2PTransfer", dispidELTE_OCX_P2PTransfer, ELTE_OCX_P2PTransfer, VT_BSTR, VTS_BSTR VTS_BSTR)


	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_CreatePatchGroup", dispidELTE_OCX_CreatePatchGroup, ELTE_OCX_CreatePatchGroup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_CancelPatchGroup", dispidELTE_OCX_CancelPatchGroup, ELTE_OCX_CancelPatchGroup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_AddPatchGroupMember", dispidELTE_OCX_AddPatchGroupMember, ELTE_OCX_AddPatchGroupMember, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_DeletePatchGroupMember", dispidELTE_OCX_DeletePatchGroupMember, ELTE_OCX_DeletePatchGroupMember, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetPatchGroups", dispidELTE_OCX_GetPatchGroups, ELTE_OCX_GetPatchGroups, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetGroupMemberByPatchId", dispidELTE_OCX_GetGroupMemberByPatchId, ELTE_OCX_GetGroupMemberByPatchId, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetPatchGroupInfo", dispidELTE_OCX_GetPatchGroupInfo, ELTE_OCX_GetPatchGroupInfo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_PTZControl", dispidELTE_OCX_PTZControl, ELTE_OCX_PTZControl, VT_BSTR, VTS_BSTR VTS_UI4 VTS_UI4)
	//DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetUserSpecificGISCfg", dispidELTE_OCX_GetUserSpecificGISCfg, ELTE_OCX_GetUserSpecificGISCfg, VT_BSTR, VTS_BSTR)
	//DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_SetGisParam", dispidELTE_OCX_SetGisParam, ELTE_OCX_SetGisParam, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_GetGisSubscription", dispidELTE_OCX_GetGisSubscription, ELTE_OCX_GetGisSubscription, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_ModifyDynamicGroup", dispidELTE_OCX_ModifyDynamicGroup, ELTE_OCX_ModifyDynamicGroup, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_P2PHalfDpxDial", dispidELTE_OCX_P2PHalfDpxDial, ELTE_OCX_P2PHalfDpxDial, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_P2PHalfDpxRelease", dispidELTE_OCX_P2PHalfDpxRelease, ELTE_OCX_P2PHalfDpxRelease, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CeLTE_PlayerCtrl, "ELTE_OCX_TempUserJoinGroup", dispidELTE_OCX_TempUserJoinGroup, ELTE_OCX_TempUserJoinGroup, VT_BSTR, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()//lint -e786



// Event map

BEGIN_EVENT_MAP(CeLTE_PlayerCtrl, COleControl)
	EVENT_CUSTOM_ID("ELTE_OCX_Event", eventidELTE_OCX_Event, ELTE_OCX_Event, VTS_UI4 VTS_BSTR)
	EVENT_CUSTOM_ID("ELTE_OCX_PlayerEvent", eventidELTE_OCX_PlayerEvent, ELTE_OCX_PlayerEvent, VTS_UI4 VTS_BSTR)
END_EVENT_MAP()//lint -e786



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CeLTE_PlayerCtrl, 1)
	PROPPAGEID(CeLTE_PlayerPropPage::guid)
END_PROPPAGEIDS(CeLTE_PlayerCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CeLTE_PlayerCtrl, "ELTE_PLAYER.eLTE_PlayerCtrl.1",
	0x6ab35b6f, 0xd5d4, 0x4da3, 0x95, 0x3, 0x1a, 0xd4, 0xe5, 0xe1, 0xe5, 0x89)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CeLTE_PlayerCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID IID_DeLTE_Player = { 0x76935AE5, 0xA248, 0x4C3E, { 0x87, 0x59, 0x0, 0xB6, 0x16, 0xF9, 0x7F, 0x41 } };
const IID IID_DeLTE_PlayerEvents = { 0x241C1E26, 0xBFDE, 0x49BC, { 0xA4, 0x40, 0xE6, 0xC2, 0x8D, 0xF3, 0xA7, 0xEE } };


// Control type information

static const DWORD _dweLTE_PlayerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CeLTE_PlayerCtrl, IDS_ELTE_PLAYER, _dweLTE_PlayerOleMisc)



// CeLTE_PlayerCtrl::CeLTE_PlayerCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CeLTE_PlayerCtrl

BOOL CeLTE_PlayerCtrl::CeLTE_PlayerCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ELTE_PLAYER,
			IDB_ELTE_PLAYER,
			afxRegApartmentThreading,
			_dweLTE_PlayerOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


HHOOK CeLTE_PlayerCtrl::s_KeyboardHook = NULL;
HHOOK CeLTE_PlayerCtrl::s_MouseHook = NULL;
std::set<CeLTE_PlayerCtrl*> CeLTE_PlayerCtrl::m_eLTE_PlayerList;
std::map<int, CeLTE_PlayerCtrl*> CeLTE_PlayerCtrl::m_WaitPlayVideoList;
BOOL CeLTE_PlayerCtrl::m_bLogInited = FALSE;
// CeLTE_PlayerCtrl::CeLTE_PlayerCtrl - Constructor

CeLTE_PlayerCtrl::CeLTE_PlayerCtrl()
	: m_GdiplusToken(0)
	, m_ulType(0)
	, m_bHaveLoaded(FALSE)
{
	InitializeIIDs(&IID_DeLTE_Player, &IID_DeLTE_PlayerEvents);
	// TODO: Initialize your control's instance data here.
}



// CeLTE_PlayerCtrl::~CeLTE_PlayerCtrl - Destructor

CeLTE_PlayerCtrl::~CeLTE_PlayerCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CeLTE_PlayerCtrl::OnDraw - Drawing function

void CeLTE_PlayerCtrl::OnDraw(
			CDC* pdc, const CRect& /*rcBounds*/, const CRect& /*rcInvalid*/)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);
}//lint -e715



// CeLTE_PlayerCtrl::DoPropExchange - Persistence support

void CeLTE_PlayerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CeLTE_PlayerCtrl::OnResetState - Reset control to default state

void CeLTE_PlayerCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CeLTE_PlayerCtrl::AboutBox - Display an "About" box to the user

void CeLTE_PlayerCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_ELTE_PLAYER);
	dlgAbout.DoModal();
}//lint !e1762



// CeLTE_PlayerCtrl message handlers

LRESULT CALLBACK CeLTE_PlayerCtrl::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == nCode)
	{
		KBDLLHOOKSTRUCT *pkbh = (KBDLLHOOKSTRUCT *)lParam;
		if (NULL != pkbh)
		{
			if (VK_ESCAPE == pkbh->vkCode)
			{
				std::set<CeLTE_PlayerCtrl*>::iterator itor = m_eLTE_PlayerList.begin();
				for (; m_eLTE_PlayerList.end() != itor; itor++)
				{
					CeLTE_PlayerCtrl* pOcx = *itor;
					if (NULL != pOcx)
					{
						pOcx->PostMessage(WM_ELTE_EXIT_FULL_SCREEN);
					}
				}
				return 1L;
			}
		}
	}

	return CallNextHookEx(s_KeyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK CeLTE_PlayerCtrl::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == nCode)
	{
		// 全屏时忽略鼠标事件
		return 1L;
	}

	return CallNextHookEx(s_KeyboardHook, nCode, wParam, lParam);
}

int CeLTE_PlayerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	(void)m_ConfigMgr.Init();

	return 0;
}

void CeLTE_PlayerCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (NULL != m_VideoPane.GetSafeHwnd())
	{
		m_VideoPane.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE);
	}
}

void CeLTE_PlayerCtrl::OnMove(int x, int y)
{
	COleControl::OnMove(x, y);

	// TODO: Add your message handler code here
	if (NULL != m_VideoPane.GetSafeHwnd())
	{
		m_VideoPane.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE);
	}
}

void CeLTE_PlayerCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (NULL != m_VideoPane.GetSafeHwnd() && !m_VideoPane.IsFullScreen())
	{
		int iRet = m_VideoPane.SetFullScreen();
		if (eLTE_ERR_SUCCESS == iRet)
		{
			// 播放器全屏事件
			PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_FULLSCREEN, NULL);
		}
	}

	COleControl::OnLButtonDblClk(nFlags, point);
}

LRESULT CeLTE_PlayerCtrl::OnMsgExitFullScreen(WPARAM wparam, LPARAM lparam)
{
	if (NULL != m_VideoPane.GetSafeHwnd() && m_VideoPane.IsFullScreen())
	{
		int iRet = m_VideoPane.ResetFullScreen();
		if (eLTE_ERR_SUCCESS == iRet)
		{
			// 播放器退出全屏事件
			PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_NORMALSCREEN, NULL);
		}
	}

	return 0L;
}

ELTE_VOID __SDK_CALL CeLTE_PlayerCtrl::ELTE_EventCallBack(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData)
{
	LOG_TRACE();

	if (NULL == pEventBuf && EVENT_NOTIFY_PROVISION_ALLRESYNC != iEventType)
	{
		LOG_RUN_DEBUG("pEventBuf is null.");
		return;
	}

	if(NULL != pUserData)
	{	
		CeLTE_PlayerCtrl* pCtrl = (CeLTE_PlayerCtrl*)pUserData;
		char* pBuf = NULL;
		if(EVENT_NOTIFY_PROVISION_ALLRESYNC != iEventType)
		{
			pBuf = new char[uiBufSize + 1];
			if(NULL == pBuf)
			{
				return;
			}
			memcpy(pBuf, (char*)pEventBuf, uiBufSize);
			pBuf[uiBufSize] = '\0';
		}
		pCtrl->PostMessage(WM_ELTE_POST_EVENT,(WPARAM)iEventType, (LPARAM)pBuf);
	}
}//lint !e818

LRESULT CeLTE_PlayerCtrl::OnMsgPostEvent(WPARAM wparam, LPARAM lparam)
{
	LOG_TRACE();
	char* pBuf = (char*)lparam;
	int iEventId = (int)wparam;
	INFO_PARAM1(iEventId);
	if (NULL == pBuf && EVENT_NOTIFY_PROVISION_ALLRESYNC != iEventId)
	{
		return -1L;
	}
	
	//目前只有EVENT_NOTIFY_PROVISION_ALLRESYNC == iEventId，字符串才会为空。
	std::string strXml = "";
	if(pBuf)
	{
		strXml = pBuf;
		delete [] pBuf;
		pBuf = NULL;
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	CString xmlStr;

	switch (iEventId)
	{
	case EVENT_NOTIFY_USER_STATUS:				// 设备状态变化事件通知
		{
			eLTE_Tool::SplitString(strXml, "<UserName>", "</UserName>", strStart, strCentre, strEnd, iSplitFlag);
		}
		break;
	case EVENT_NOTIFY_P2P_VIDEO_CALL_STATUS:	// 开始实况事件通知
		{
			NotifyP2pvideocallStatusInfo pInfo;
			eLTE_RET iRet = XMLProcess::ParseXmlStringEventNotifyP2pvideocallStatus(pInfo, strXml);
			if (eLTE_ERR_SUCCESS != iRet)
			{
				LOG_RUN_ERROR("ParseXmlStringEventNotifyP2pvideocallStatus failed.");
				return -1L;
			}

			// 设置视频参数，用于工具栏操作
			if (P2Pvideocall_IND_STATUS_ANSWERED == pInfo.iCallStatus
				|| P2Pvideocall_IND_STATUS_ACKED == pInfo.iCallStatus
				|| P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED == pInfo.iCallStatus)
			{
				if (eLTE_ERR_SUCCESS != iRet)
				{
					LOG_RUN_ERROR("Stop audio failed.");
					return -1L;
				}
				// 正在等待播放视频，用于工具栏播放视频
				std::map<int, CeLTE_PlayerCtrl*>::iterator itor = m_WaitPlayVideoList.find(pInfo.iCallee);
				if (m_WaitPlayVideoList.end() == itor)
				{
					itor = m_WaitPlayVideoList.find(pInfo.iCaller);
				}
				if (m_WaitPlayVideoList.end() != itor)
				{
					CeLTE_PlayerCtrl* &pOCX = itor->second;
					if (NULL != pOCX)
					{
						MediaPlayer& mediaPlayer = pOCX->m_MediaPlayer;
						CVideoPane& videoPane = pOCX->m_VideoPane;

						if (P2Pvideocall_IND_STATUS_ANSWERED == pInfo.iCallStatus
							|| P2Pvideocall_IND_STATUS_ACKED == pInfo.iCallStatus)
						{
							// 设置播放参数
							mediaPlayer.SetPlayerParam((unsigned int)pInfo.iLocalVideoPort, (unsigned int)pInfo.iLocalAudioPort, (unsigned int)pInfo.iRemoteVideoPort, (unsigned int)pInfo.iRemoteAudioPort);

							// 播放视频
							(void)mediaPlayer.StartPlayer(videoPane.GetVideoStaticSafeHwnd());
						}

						// 激活工具栏按钮
						videoPane.EnableImageButton(TRUE);
					}
					m_WaitPlayVideoList.erase(itor);
				}
			}
			// 挂断等待
			else if (P2Pvideocall_IND_STATUS_HANGUPED_ACTIVE == pInfo.iCallStatus
				|| P2Pvideocall_IND_STATUS_HANGUPED == pInfo.iCallStatus)
			{
				std::map<int, CeLTE_PlayerCtrl*>::iterator itor = m_WaitPlayVideoList.find(pInfo.iCallee);
				if (m_WaitPlayVideoList.end() == itor)
				{
					itor = m_WaitPlayVideoList.find(pInfo.iCaller);
				}
				if (m_WaitPlayVideoList.end() != itor)
				{
					// 修改界面
					itor->second->m_VideoPane.PlayAudio(FALSE);
					itor->second->m_VideoPane.EnableImageButton(FALSE);
					// 重置界面
					itor->second->m_VideoPane.Invalidate(TRUE);
					SetEvent(OperationMgr::m_hHangupEvent);
				}				
			}
		}
		break;
	case EVENT_NOTIFY_RESOURCE_STATUS:			// 群组关系状态变化事件通知
		{
			eLTE_Tool::SplitString(strXml, "<ResourceName>", "</ResourceName>", strStart, strCentre, strEnd, iSplitFlag);
		}
		break;
	case EVENT_NOTIFY_PROVISION_CHANGE:			// 设备属性配置变更通知事件
		{
			//节点不会同时存在
			eLTE_Tool::SplitString(strXml, "<UserName>", "</UserName>", strStart, strCentre, strEnd, iSplitFlag);
			eLTE_Tool::SplitString(strXml, "<GroupName>", "</GroupName>", strStart, strCentre, strEnd, iSplitFlag);
			eLTE_Tool::SplitString(strXml, "<Alias>", "</Alias>", strStart, strCentre, strEnd, iSplitFlag);
			eLTE_Tool::SplitString(strXml, "<StMsg>", "</StMsg>", strStart, strCentre, strEnd, iSplitFlag);
			eLTE_Tool::SplitString(strXml, "<PGName>", "</PGName>", strStart, strCentre, strEnd, iSplitFlag);
			eLTE_Tool::SplitString(strXml, "<Address>", "</Address>", strStart, strCentre, strEnd, iSplitFlag);
		}
		break;
	case EVENT_NOTIFY_PROVISION_ALLRESYNC:		// 自动下载配置数据通知事件
		{
			// 不用做处理
		}
		break;
	case EVENT_NOTIFY_P2P_CALL_STATUS:			// 点呼状态变化事件
		{
		}
		break;
	case EVENT_NOTIFY_GROUP_STATUS:				// 组呼状态变化事件
		{
			eLTE_Tool::SplitString(strXml, "<SpeakerName>", "</SpeakerName>", strStart, strCentre, strEnd, iSplitFlag);
		}
		break;
	case EVENT_NOTIFY_MODULE_STATUS:			// 模块组件状态事件
		{
		}
		break;
	case EVENT_NOTIFY_GIS_STATUS:               // 终端订阅状态上报事件
		{
		}
		break;
	case EVENT_NOTIFY_GIS_REPORT:               // 终端GIS信息上报事件
		{
		}
		break;
	case EVENT_NOTIFY_SDS_REPORT:  //短信信息上报事件
		{
			//Currently only sdscontent transcoding, need to confirm it
			if(eLTE_ERR_SUCCESS != XMLProcess::SetXmlStringEventNotifySDSReport(strXml, xmlStr))
			{
				LOG_RUN_ERROR("SetXmlStringEventNotifySDSReport failed.");
				return -1L;
			}
		}
		break;
	case EVENT_NOTIFY_SDS_RETCODE:  //短信发送状态上报事件	
		{
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid eventId.");
		}
	}

	if(0 == xmlStr.GetLength())
	{
		if(0 == iSplitFlag) //ansi
		{
			xmlStr = eLTE_Tool::ANSIToUnicode(strXml).c_str();
		}
		else if(1 == iSplitFlag) //ansi and utf8
		{
			xmlStr.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
			if (!strCentre.empty())
			{
				xmlStr.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
			}
			xmlStr.Append(eLTE_Tool::ANSIToUnicode(strEnd).c_str());
		}
	}
	
	// fireEvent
	ELTE_OCX_Event((ULONG)iEventId, xmlStr);
	return 0L;
}

LRESULT CeLTE_PlayerCtrl::OnMsgPostPlayerEvent(WPARAM wparam, LPARAM lparam)
{
	LOG_TRACE();
	int iEventId = (int)wparam;
	INFO_PARAM1(iEventId);

	CString xmlStr(_T(""));
	switch (iEventId)
	{
	case eLTE_PLAYER_HIDE:
	case eLTE_PLAYER_SHOW:
	case eLTE_PLAYER_FULLSCREEN:
	case eLTE_PLAYER_NORMALSCREEN:
	case eLTE_PLAYER_MUTE:
	case eLTE_PLAYER_UNMUTE:
		break;
	case eLTE_PLAYER_REVERSE:
	case eLTE_PLAYER_SNAPSHOT:
	case eLTE_PLAYER_POSITION:
	case eLTE_PLAYER_TITLE:
		{
			PlayerEventParamInfo* pInfo = (PlayerEventParamInfo*)lparam;
			if (NULL == pInfo)
			{
				LOG_RUN_ERROR("pInfo is null.");
				return -1L;
			}

			eLTE_RET iRet = XMLProcess::SetXmlStringPlayerEvent(iEventId, pInfo, xmlStr);
			if (eLTE_ERR_SUCCESS != iRet)
			{
				delete pInfo;
				LOG_RUN_ERROR("SetXmlStringPlayerEvent failed.");
				return -1L;
			}

			delete pInfo;
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid eventId.");
			return -1L;
		}
	}

	// fireEvent
	ELTE_OCX_PlayerEvent((ULONG)iEventId, xmlStr);
	return 0L;
}

LRESULT CeLTE_PlayerCtrl::OnMsgStopVideoPlay(WPARAM /*wparam*/, LPARAM /*lparam*/)
{
	LOG_TRACE();

	// 获取资源ID
	int iResId = m_MediaPlayer.GetMediaPlayerResID();

	// 关闭视频回传
	int iRet = OperationMgr::Instance().DcVideoHangup(iResId);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("VideoHangup failed.");
	}

	// 停止播放视频
	iRet = m_MediaPlayer.StopAudio();
	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("StopAudio failed.");
	}

	iRet = m_MediaPlayer.StopPlayer();
	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("StopPlayer failed.");
	}

	// 重置界面
	m_VideoPane.Invalidate(TRUE);
	
	if (eLTE_ERR_SUCCESS == iRet)
	{
		// 禁用工具栏按钮
		m_VideoPane.EnableAudio(FALSE);
		m_VideoPane.EnableImageButton(FALSE);

		// 隐藏播放窗口
		m_VideoPane.ShowWindow(SW_HIDE);
	}
	m_MediaPlayer.SetMediaPlayerResID("0");
	return 0L;
}

BSTR CeLTE_PlayerCtrl::ELTE_OCX_SetLogPath(LPCTSTR pLogFilePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	CXml xml;
	
	// 日志模块还没有启动，不能输出日志
	// 入参检测
	if (NULL == pLogFilePath)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		return strResult.AllocSysString();
	}

	// Load前调用
	if (m_bHaveLoaded)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		return strResult.AllocSysString();
	}

	// 设置日志路径
	eLTE_RET iRet = ELTE_SDK_SetLogPath(eLTE_Tool::UnicodeToANSI(pLogFilePath).c_str());
	if(eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		return strResult.AllocSysString();
	}

	{
		iRet = eLTE_Log::Instance().SetLogPath(eLTE_Tool::UnicodeToANSI(pLogFilePath));
		GET_RETURN_CODE_XML(xml, iRet, strResult);
	}

	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_SetLogLevel(ULONG ulLogLevel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	CXml xml;

	// Load前调用
	if (m_bHaveLoaded)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		return strResult.AllocSysString();
	}

	// 设置日志级别
	eLTE_RET iRet = ELTE_SDK_SetLogLevel(ulLogLevel);
	if(eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		return strResult.AllocSysString();
	}

	{
		iRet = eLTE_Log::Instance().SetLogLevel(ulLogLevel);
		GET_RETURN_CODE_XML(xml, iRet, strResult);
	}

	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_UploadLogo(LPCTSTR pLogoFilePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	CXml xml;

	// 入参检测
	if (NULL == pLogoFilePath)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		return strResult.AllocSysString();
	}

	// 判断文件是否存在
	if (!eLTE_Tool::IsExistPath(pLogoFilePath))
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		return strResult.AllocSysString();
	}

	// 图片格式是否支持, 这里GDI+没有启动，使用CImage
	ATL::CImage Image;
	HRESULT hResult = Image.Load(pLogoFilePath);
	if(FAILED(hResult))
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		return strResult.AllocSysString();
	}
	if (!Image.IsNull())
	{
		Image.Destroy();
	}	

	// 拷贝文件到Skin目录, 覆盖拷贝
	CString szImageName(pLogoFilePath);
	int iIndex = (szImageName.GetLength() - szImageName.ReverseFind(_T('\\'))) - 1;
	szImageName = szImageName.Right(iIndex);
	CString szPath = eLTE_Tool::GetOcxPath();
	szPath.Append(_T("Skin\\"));
	szPath.Append(szImageName);
	if (!CopyFile(pLogoFilePath, szPath, FALSE))
	{
		// 拷贝失败. 日志模块还没有启动，无法打印
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		return strResult.AllocSysString();
	}

	// 修改配置文件eLTE_Player.ini
	szPath = eLTE_Tool::GetOcxPath();
	szPath.Append(CONFIG_FILE_NAME);
	CString strValue = _T(".\\Skin\\");
	strValue.Append(szImageName);
	if (!WritePrivateProfileString(SECTION_SKIN_CONFIG, _T("logoPath"), strValue, szPath))
	{
		// 修改配置文件失败
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		return strResult.AllocSysString();
	}

	//重新读取配置文件eLTE_Player.ini
	(void)m_ConfigMgr.Init();

	// 修改内存变量
	m_VideoPane.SetParam(pLogoFilePath);

	GET_RETURN_CODE_XML(xml, eLTE_ERR_SUCCESS, strResult);
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetVersion(ULONG ulVersion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	CXml xml;

	// 获取eLTE_Player.ocx 版本
	if (eLTE_VERSION_OCX == ulVersion)
	{
		CString strVersion = eLTE_Tool::GetOcxVersion();
		GET_VERSION_XML(xml, eLTE_Tool::UnicodeToANSI(strVersion.GetBuffer()).c_str(), strResult);
	}
	// 获取eLTE SDK 版本
	else if (eLTE_VERSION_SDK == ulVersion)
	{
		GET_VERSION_XML(xml, "", strResult);
	}
	// 非法版本
	else
	{
		GET_VERSION_XML(xml, "", strResult);
	}

	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_SetLanguage(ULONG ulLanguage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	CXml xml;

	m_VideoPane.SetLanguage((int)ulLanguage);
	GET_RETURN_CODE_XML(xml, eLTE_ERR_SUCCESS, strResult);

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_ShowToolbar(LONG ulToolbar)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	CXml xml;

	if (0 == ulToolbar)
	{
		m_VideoPane.ShowToolBar(FALSE);
	}
	else
	{
		m_VideoPane.ShowToolBar(TRUE);
	}
	eLTE_RET iRet = eLTE_ERR_SUCCESS;
	// 更新播放窗口大小
	if(!m_ulByMediaPass)
	{
		RECT wnd;
		memset(&wnd, 0x0, sizeof(wnd));
		m_VideoPane.GetVideoStatic().GetWindowRect(&wnd);
		ELTE_INT32 iResId = m_MediaPlayer.GetMediaPlayerResID();
		iRet = ELTE_SDK_SetPlayWindowSize(eLTE_Tool::Int2String(iResId).c_str(), (ELTE_ULONG)(wnd.right - wnd.left), (ELTE_ULONG)(wnd.bottom - wnd.top));
		if(eLTE_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("When show tool bar, call ELTE_SDK_SetPlayWindowSize failed, the res id is %d.", iResId);
		}
	}
	GET_RETURN_CODE_XML(xml, iRet, strResult);
	return strResult.AllocSysString();
}

BSTR CeLTE_PlayerCtrl::ELTE_OCX_Load(ULONG ulType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	CXml xml;

	// 日志初始化
	if (!m_bLogInited)
	{
		std::string strIniPath = eLTE_Tool::GetOcxPathA();
		strIniPath.append(LOG_INI_FILE_NAME);
		unsigned int logLevel[LOG_CATEGORY] = {eLTE_Log::Instance().m_logLevel,eLTE_Log::Instance().m_logLevel,eLTE_Log::Instance().m_logLevel};
		int iRet = LOG_INIT(strIniPath.c_str(), logLevel, eLTE_Log::Instance().m_logPath.c_str());
		if (eLTE_ERR_SUCCESS != iRet)
		{
			GET_RETURN_CODE_XML(xml, eLTE_ERR_LOG_INIT, strResult);
			return strResult.AllocSysString();
		}
		m_bLogInited = TRUE;
	}
	
	// 入参打印
	LOG_INTERFACE_TRACE();
	INFO_PARAM1(ulType);

	//入参校验
	if (eLTE_LOAD_OPERATE_MGR != ulType
		&& eLTE_LOAD_MEDIA_PLAYER != ulType)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("Invalid param.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "Type:%d", ulType);
		return strResult.AllocSysString();
	}

	// 不能重复加载
	if (m_bHaveLoaded)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Can't load eLTE_Player.ocx multiple.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "Type:%d", ulType);
		return strResult.AllocSysString();
	}

	// 需要加载插件执行管理操作
	if (eLTE_LOAD_OPERATE_MGR == ulType)
	{
		m_ulType = ulType;
		// 初始化eLTE SDK

		// 设置工作路径
		TCHAR srcWorkpath[MAX_PATH] = {0};
		GetCurrentDirectory(MAX_PATH, srcWorkpath);
		CString workpath = eLTE_Tool::GetOcxPath();
		workpath += ELTE_SDK;
		SetCurrentDirectory(workpath);

		ELTE_INT32 iRet = ELTE_SDK_Init((ELTE_INT32)m_ulByMediaPass);

		// 还原工作路径
		SetCurrentDirectory(srcWorkpath);

		if (eLTE_ERR_SUCCESS != iRet)
		{
			GET_RETURN_CODE_XML(xml, iRet, strResult);
			LOG_RUN_ERROR("ELTE_SDK_Init failed.");
			LOG_INTERFACE_INFO(iRet, "Type:%d", ulType);
			return strResult.AllocSysString();
		}
		OperationMgr::Instance().m_pCeLTE_PlayerCtrl =  this;

		// 设置回调函数
		iRet = ELTE_SDK_SetEventCallBack(ELTE_EventCallBack, this);
		if (eLTE_ERR_SUCCESS != iRet)
		{
			GET_RETURN_CODE_XML(xml, iRet, strResult);
			LOG_RUN_ERROR("ELTE_SDK_SetEventCallBack failed.");
			LOG_INTERFACE_INFO(iRet, "Type:%d", ulType);
			return strResult.AllocSysString();
		}
	}

	// GDI+
	int iRet = (int)Gdiplus::GdiplusStartup(&m_GdiplusToken, &m_GdiplusStartupInput, NULL);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GdiplusStartup failed.");
		LOG_INTERFACE_INFO(iRet, "Type:%d", ulType);
		return strResult.AllocSysString();
	}

	// 修改eLTE SDK 环境变量EASY_INSTALL
	(void)eLTE_Tool::ChangeEnvironmentVariable();

	// 初始化配置参数
	(void)m_ConfigMgr.Start(m_MediaPlayer, m_VideoPane, OperationMgr::Instance());
	/*if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ConfigMgr start failed.");
		LOG_INTERFACE_INFO(iRet, "Type:%d", ulType);
		return strResult.AllocSysString();
	}*/

	// 创建视频窗口
	if (NULL != m_VideoPane.GetSafeHwnd())
	{
		m_VideoPane.DestroyWindow();
	}
	CRect rect;
	GetClientRect(rect);
	m_VideoPane.SetCeLTE_PlayerCtrl(this);

	iRet = m_VideoPane.Create(CVideoPane::IDD, CWnd::GetDesktopWindow());
	if(!iRet)
	{
		LOG_RUN_ERROR("Create VideoPane failed %d.", GetLastError());
	}
	iRet = 0;
	m_VideoPane.ShowWindow(SW_HIDE);

	// eLTE_Player 列表
	m_eLTE_PlayerList.insert(this);
	m_bHaveLoaded = TRUE;

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "Type:%d", ulType);
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_UnLoad(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here	
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 需要加载插件执行管理操作
	if (eLTE_LOAD_OPERATE_MGR == m_ulType)
	{
		// 退出eLTE SDK
		ELTE_INT32 iRet = ELTE_SDK_Cleanup();
		if (eLTE_ERR_SUCCESS != iRet)
		{
			GET_RETURN_CODE_XML(xml, iRet, strResult);
			LOG_RUN_ERROR("AppContextMgr Exit failed.");
			LOG_INTERFACE_INFO(iRet, "");
			return strResult.AllocSysString();
		}
		m_ulType = 0;
	}

	// 销毁播放窗口
	if (NULL != m_VideoPane.GetSafeHwnd())
	{
		m_VideoPane.DestroyWindow();
	}
	CWnd* pParentWnd = GetParentOwner();
	if (NULL != pParentWnd)
	{
		pParentWnd->Invalidate(TRUE);
	}

	// 去初始化配置参数
	eLTE_RET iRet = m_ConfigMgr.Exit();
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ConfigMgr Exit failed.");
		LOG_INTERFACE_INFO(iRet, "");
		return strResult.AllocSysString();
	}

	// 键盘钩子
	if (NULL != CeLTE_PlayerCtrl::s_KeyboardHook)
	{
		::UnhookWindowsHookEx(CeLTE_PlayerCtrl::s_KeyboardHook);
		CeLTE_PlayerCtrl::s_KeyboardHook = NULL;
	}
	// 鼠标钩子
	if (NULL != CeLTE_PlayerCtrl::s_MouseHook)
	{
		::UnhookWindowsHookEx(CeLTE_PlayerCtrl::s_MouseHook);
		CeLTE_PlayerCtrl::s_MouseHook = NULL;
	}

	// GDI+
	Gdiplus::GdiplusShutdown(m_GdiplusToken);

	// 移除eLTE_Player 列表
	std::set<CeLTE_PlayerCtrl*>::iterator itor = m_eLTE_PlayerList.find(this);
	if (m_eLTE_PlayerList.end() != itor)
	{
		m_eLTE_PlayerList.erase(itor);
	}

	if (m_eLTE_PlayerList.empty())
	{
		// 媒体去初始化
		(void)m_MediaPlayer.UninitPlayer();

		// 日志结束
		iRet = LOG_EXIT();
		LOG_INTERFACE_INFO(iRet, "");
		m_bLogInited = FALSE;
		m_bHaveLoaded = FALSE;
	}
	else
	{
		LOG_INTERFACE_INFO(iRet, "");
	}
	// 设置返回值
	GET_RETURN_CODE_XML(xml, iRet, strResult);
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_Login(LPCTSTR pUserID, LPCTSTR pPWD, LPCTSTR pServerIP, LPCTSTR pLocalIP, LPCTSTR pServerSIPPort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pUserID || NULL == pPWD || NULL == pServerIP || NULL == pLocalIP || NULL == pServerSIPPort)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	ELTE_INT32 iRet = ELTE_SDK_Login(eLTE_Tool::UnicodeToANSI(pUserID).c_str(), eLTE_Tool::UnicodeToANSI(pPWD).c_str(), 
		                             eLTE_Tool::UnicodeToANSI(pServerIP).c_str(), eLTE_Tool::UnicodeToANSI(pLocalIP).c_str(), 
									 eLTE_Tool::String2UInt(eLTE_Tool::UnicodeToANSI(pServerSIPPort)));

	GET_RETURN_CODE_XML(xml, iRet, strResult);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("DcLogin failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s, ServerIP:%s, LocalIP:%s, SipPort:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str(), eLTE_Tool::UnicodeToANSI(pServerIP).c_str(), eLTE_Tool::UnicodeToANSI(pLocalIP).c_str(), eLTE_Tool::UnicodeToANSI(pServerSIPPort).c_str());
		return strResult.AllocSysString();
	}

	LOG_INTERFACE_INFO(iRet, "UserID:%s, ServerIP:%s, LocalIP:%s, SipPort:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str(), eLTE_Tool::UnicodeToANSI(pServerIP).c_str(), eLTE_Tool::UnicodeToANSI(pLocalIP).c_str(), eLTE_Tool::UnicodeToANSI(pServerSIPPort).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_Logout(LPCTSTR pUserID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pUserID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	int iRet = ELTE_SDK_Logout(eLTE_Tool::UnicodeToANSI(pUserID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		LOG_RUN_ERROR("DcLogout failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_TriggerStatusReport(ULONG ulEnableStatusReport)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 触发状态上报
	ELTE_INT32 iRet = ELTE_SDK_TriggerStatusReport((ELTE_INT32)ulEnableStatusReport);

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "EnableStatusReport:%d", ulEnableStatusReport);
	return strResult.AllocSysString();
}//lint !e1762


//BSTR CeLTE_PlayerCtrl::ELTE_OCX_ProvisionManagerInit(LPCTSTR pServerIP, LPCTSTR pUserID)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	CString strResult;
//
//	// TODO: Add your dispatch handler code here
//	LOG_INTERFACE_TRACE();
//	CXml xml;
//
//	// load类型是否支持
//	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
//
//	// 入参检测
//	if (NULL == pServerIP || NULL == pUserID)
//	{
//		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
//		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
//		return strResult.AllocSysString();
//	}
//
//	//int iRet = ProvisionMgr::Instance().Init(eLTE_Tool::UnicodeToANSI(pServerIP), eLTE_Tool::WString2Int(pUserID));
//
//	//GET_RETURN_CODE_XML(xml, iRet, strResult);
//	//LOG_INTERFACE_INFO(iRet, "ServerIP:%s, UserID:%s", eLTE_Tool::UnicodeToANSI(pServerIP).c_str(), eLTE_Tool::UnicodeToANSI(pUserID).c_str());
//	return strResult.AllocSysString();
//}//lint !e1762


//BSTR CeLTE_PlayerCtrl::ELTE_OCX_ProvisionManagerExit(void)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	CString strResult;
//
//	// TODO: Add your dispatch handler code here
//	LOG_INTERFACE_TRACE();
//	CXml xml;
//
//	// load类型是否支持
//	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
//
//	//int iRet = ProvisionMgr::Instance().Exit();
//	//GET_RETURN_CODE_XML(xml, iRet, strResult);
//
//	//LOG_INTERFACE_INFO(iRet, "");
//	return strResult.AllocSysString();
//}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetDcGroups(LPCTSTR pUserID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pUserID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取群组列表
	ELTE_CHAR* pDcGroups = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcGroups(eLTE_Tool::UnicodeToANSI(pUserID).c_str(), &pDcGroups);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetDcGroups failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	std::string strSrc = pDcGroups;
	int iExistElement = 0;
	//字符转码
	for (;;)
	{
		iSplitFlag = 0;
		eLTE_Tool::SplitString(strSrc, "<GroupName>", "</GroupName>", strStart, strCentre, strEnd, iSplitFlag);
		if(1 == iSplitFlag)
		{
			strResult.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
			strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
			//strResult.Append(eLTE_Tool::ANSIToUnicode("</GroupName><GroupName>").c_str());
			iExistElement = 1;
		}
		else
		{
			break;
		}
		strSrc = strEnd;
	}

	if(1 == iExistElement)
	{
		strResult.Append(eLTE_Tool::ANSIToUnicode("</GroupName></GroupInfo></GroupInfoList></Content>").c_str());
	}
	else
	{
		strResult = eLTE_Tool::ANSIToUnicode(pDcGroups).c_str();
	}

	iRet = ELTE_SDK_ReleaseBuffer(pDcGroups);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<GroupInfoList>"));

	LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetDcUsers(LPCTSTR pUserID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pUserID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取用户列表
	ELTE_CHAR* pDcUsers = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcUsers(eLTE_Tool::UnicodeToANSI(pUserID).c_str(), &pDcUsers);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetDcUsers failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	std::string strSrc = pDcUsers;
	int iExistElement = 0;
	//字符转码
	for (;;)
	{
		iSplitFlag = 0;
		eLTE_Tool::SplitString(strSrc, "<UserName>", "</UserName>", strStart, strCentre, strEnd, iSplitFlag);
		if(1 == iSplitFlag)
		{
			strResult.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
			strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
			//strResult.Append(eLTE_Tool::ANSIToUnicode("</UserName><UserName>").c_str());
			iExistElement = 1;
		}
		else
		{
			break;
		}
		strSrc = strEnd;
	}

	if(1 == iExistElement)
	{
		strResult.Append(eLTE_Tool::ANSIToUnicode("</UserName></UserInfo></UserInfoList></Content>").c_str());
	}
	else
	{
		strResult = eLTE_Tool::ANSIToUnicode(pDcUsers).c_str();
	}

	iRet = ELTE_SDK_ReleaseBuffer(pDcUsers);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<UserInfoList>"));

	LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetGroupUsers(LPCTSTR pGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取一个群组的成员列表
	ELTE_CHAR* pGrpUserList = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetGroupUsers(eLTE_Tool::UnicodeToANSI(pGroupID).c_str(), &pGrpUserList);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetGroupUsers failed.");
		LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
		return strResult.AllocSysString();
	}

	strResult = eLTE_Tool::ANSIToUnicode(pGrpUserList).c_str();
	iRet = ELTE_SDK_ReleaseBuffer(pGrpUserList);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
		return strResult.AllocSysString();
	}
	
	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<GroupUserInfoList>"));

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetGroupInfo(LPCTSTR pGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取群组详细配置信息
	ELTE_CHAR* pGroupInfo = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetGroupInfo(eLTE_Tool::UnicodeToANSI(pGroupID).c_str(), &pGroupInfo);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetGroupInfo failed.");
		LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
		return strResult.AllocSysString();
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	eLTE_Tool::SplitString(pGroupInfo, "<GroupName>", "</GroupName>", strStart, strCentre, strEnd, iSplitFlag);

	if(0 == iSplitFlag)
	{
		strResult = eLTE_Tool::ANSIToUnicode(pGroupInfo).c_str();
	}
	else
	{
		strResult.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
		strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
		strResult.Append(eLTE_Tool::ANSIToUnicode(strEnd).c_str());
	}

	iRet = ELTE_SDK_ReleaseBuffer(pGroupInfo);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<GroupInfo>"));

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetUserInfo(LPCTSTR pUserID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pUserID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取用户详细信息
	ELTE_CHAR* pUserInfo = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetUserInfo(eLTE_Tool::UnicodeToANSI(pUserID).c_str(), &pUserInfo);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetUserInfo failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	eLTE_Tool::SplitString(pUserInfo, "<UserName>", "</UserName>", strStart, strCentre, strEnd, iSplitFlag);

	if(0 == iSplitFlag)
	{
		strResult = eLTE_Tool::ANSIToUnicode(pUserInfo).c_str();
	}
	else
	{
		strResult.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
		strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
		strResult.Append(eLTE_Tool::ANSIToUnicode(strEnd).c_str());
	}

	iRet = ELTE_SDK_ReleaseBuffer(pUserInfo);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<UserInfo>"));

	LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetDcInfo(LPCTSTR pUserID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pUserID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取DC详细信息
	ELTE_CHAR* pDcProperty = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcInfo(eLTE_Tool::UnicodeToANSI(pUserID).c_str(), &pDcProperty);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetDcInfo failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	eLTE_Tool::SplitString(pDcProperty, "<Alias>", "</Alias>", strStart, strCentre, strEnd, iSplitFlag);

	if(0 == iSplitFlag)
	{
		strResult = eLTE_Tool::ANSIToUnicode(pDcProperty).c_str();
	}
	else
	{
		strResult.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
		strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
		strResult.Append(eLTE_Tool::ANSIToUnicode(strEnd).c_str());
	}

	iRet = ELTE_SDK_ReleaseBuffer(pDcProperty);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<DcInfo>"));

	LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_UnSubscribeGroup(LPCTSTR pGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 去订阅群组
	ELTE_INT32 iRet = ELTE_SDK_UnSubscribeGroup(eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetUserRECFileInfoList(LPCTSTR pQueryXml)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pQueryXml)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 查询录音文件信息
	ELTE_CHAR* pInfoList = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetUserRECFileInfoList(eLTE_Tool::UnicodeToANSI(pQueryXml).c_str(), &pInfoList);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetUserRECFileInfoList failed.");
		LOG_INTERFACE_INFO(iRet, "QueryXml:%s", eLTE_Tool::UnicodeToANSI(pQueryXml).c_str());
		if(pInfoList)
		{
			(void)ELTE_SDK_ReleaseBuffer(pInfoList);
		}
		return strResult.AllocSysString();
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	std::string strSrc = pInfoList;
	int iExistElement = 0;
	/************************************************************************
	--- XML 格式 ---
	<Content>
		<ResultCode></ResultCode>
		<RECFileInfoList>
			<RECFileInfo>
				<CallType></CallType>
				<Caller></Caller>
				<Callee></Callee>
				<ResourceID></ResourceID>
				<StartSec></StartSec>
				<EndSec></EndSec>
				<UrlFTP></UrlFTP>
				<UrlRTSP></UrlRTSP>
			</RECFileInfo>
		<RECFileInfoList>
	</Content>
	************************************************************************/
	//字符转码
	for (;;)
	{
		iSplitFlag = 0;
		eLTE_Tool::SplitString(strSrc, "<UrlFTP>", "</UrlFTP>", strStart, strCentre, strEnd, iSplitFlag);
		if(1 == iSplitFlag)
		{
			strResult.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
			strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
			strResult.Append(eLTE_Tool::ANSIToUnicode("</UrlFTP><UrlRTSP>").c_str());
			iExistElement = 1;
		}
		else
		{
			break;
		}
		//strSrc = strEnd;
		iSplitFlag = 0;
		eLTE_Tool::SplitString(strSrc, "<UrlRTSP>", "</UrlRTSP>", strStart, strCentre, strEnd, iSplitFlag);
		if(1 == iSplitFlag)
		{
			strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
			//strResult.Append(eLTE_Tool::ANSIToUnicode("</UrlRTSP></RECFileInfo>").c_str());
		}
		else
		{
			break;
		}
		strSrc = strEnd;
	}

	if(1 == iExistElement)
	{
		strResult.Append(eLTE_Tool::ANSIToUnicode("</UrlRTSP></RECFileInfo></RECFileInfoList></Content>").c_str());
	}
	else
	{
		strResult = eLTE_Tool::ANSIToUnicode(pInfoList).c_str();
	}

	iRet = ELTE_SDK_ReleaseBuffer(pInfoList);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "QueryXml:%s", eLTE_Tool::UnicodeToANSI(pQueryXml).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<RECFileInfoList>"));

	LOG_INTERFACE_INFO(iRet, "QueryXml:%s", eLTE_Tool::UnicodeToANSI(pQueryXml).c_str());
	return strResult.AllocSysString();
}//lint !e1762

BSTR CeLTE_PlayerCtrl::ELTE_OCX_StartRealPlay(LPCTSTR pResourceID, LPCTSTR pVideoParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID || NULL == pVideoParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 解析视频参数
	std::string strVideoFormat("");
	std::string strCameraType("");
	std::string strUserConfirmType("");
	std::string strMuteType("");
	int iRet = XMLProcess::XmlParseVideoULParameter(eLTE_Tool::UnicodeToANSI(pVideoParam),
		strVideoFormat, strCameraType, strUserConfirmType, strMuteType);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("XmlParseVideoULParameter failed.");
		LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoParam).c_str());
		return strResult.AllocSysString();
	}

	// 构建视频参数
	STVideoParam param;
	param.strFmtValue = strVideoFormat;
	param.strCameraType = strCameraType;
	param.strUserCfmType = strUserConfirmType;
	param.strMuteType = strMuteType;
	param.strVideoType = "0";//0为调度台发起视频回传
	// 开启视频回传
	iRet = ELTE_SDK_StartRealPlay(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoParam).c_str());
	if (eLTE_ERR_SUCCESS == iRet)
	{
		iRet = OperationMgr::Instance().DcVideoMonitor(eLTE_Tool::WString2Int(pResourceID), param);
	}

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoParam).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_ReverseRealPlay(LPCTSTR pResourceID, LPCTSTR pVideoParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID || NULL == pVideoParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 关闭视频回传
	int iRet = OperationMgr::Instance().DcVideoHangup(eLTE_Tool::WString2Int(pResourceID), false);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("DcVideoHangup failed.");
		LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoParam).c_str());
		return strResult.AllocSysString();
	}

	// 查找播放器
	std::set<CeLTE_PlayerCtrl*>::iterator itor_b = m_eLTE_PlayerList.begin();
	std::set<CeLTE_PlayerCtrl*>::iterator itor_e = m_eLTE_PlayerList.end();
	CeLTE_PlayerCtrl* pOCX = NULL;
	for (; itor_e != itor_b; itor_b++)
	{
		pOCX = *itor_b;
		if (NULL != pOCX)
		{
			int iResId = pOCX->m_MediaPlayer.GetMediaPlayerResID();
			if (eLTE_Tool::WString2Int(pResourceID) == iResId)
			{
				// 停止视频播放
				(void)pOCX->m_MediaPlayer.StopPlayer();
				pOCX->m_VideoPane.PlayAudio(FALSE);
				pOCX->m_VideoPane.EnableImageButton(FALSE);
				pOCX->m_VideoPane.Invalidate(TRUE);
				break;
			}
		}
	}

	// 开启视频回传
	iRet = ELTE_SDK_StartRealPlay(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoParam).c_str());

	/*if(eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		return strResult.AllocSysString();
	}*/

	std::map<int, CeLTE_PlayerCtrl*>::iterator itor = m_WaitPlayVideoList.find(eLTE_Tool::String2Int(eLTE_Tool::UnicodeToANSI(pResourceID).c_str()));
	if (m_WaitPlayVideoList.end() == itor && pOCX)
	{
		(void)m_WaitPlayVideoList.insert(std::make_pair(eLTE_Tool::String2Int(eLTE_Tool::UnicodeToANSI(pResourceID).c_str()), this));
	}

	//if(!m_ulByMediaPass)
	//{
	//	iRet = ELTE_SDK_SetPlayWindow(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), (ELTE_VOID*)m_VideoPane.GetVideoStaticSafeHwnd());
	//	if(eLTE_ERR_SUCCESS != iRet)
	//	{
	//		GET_RETURN_CODE_XML(xml, iRet, strResult);
	//		LOG_RUN_ERROR("ELTE_SDK_SetPlayWindow failed. (%d)", iRet);
	//		return strResult.AllocSysString();
	//	}
	//	// 激活工具栏按钮
	//	//m_VideoPane.EnableImageButton(TRUE);
	//}

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoParam).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_StopRealPlay(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 关闭视频回传
	int iRet = OperationMgr::Instance().DcVideoHangup(eLTE_Tool::WString2Int(pResourceID));
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("DcVideoHangup failed.");
		LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
		return strResult.AllocSysString();
	}	

	// 查找播放器
	std::set<CeLTE_PlayerCtrl*>::iterator itor_b = m_eLTE_PlayerList.begin();
	std::set<CeLTE_PlayerCtrl*>::iterator itor_e = m_eLTE_PlayerList.end();
	for (; itor_e != itor_b; itor_b++)
	{
		CeLTE_PlayerCtrl* pOCX = *itor_b;
		if (NULL != pOCX)
		{
			int iResId = pOCX->m_MediaPlayer.GetMediaPlayerResID();
			if (eLTE_Tool::WString2Int(pResourceID) == iResId)
			{
				// 停止播放视频
				(void)pOCX->m_MediaPlayer.StopAudio();
				iRet = pOCX->m_MediaPlayer.StopPlayer();
				//GET_RETURN_CODE_XML(xml, iRet, strResult);
				// 关闭播放器窗口
				pOCX->m_VideoPane.PostMessage(WM_CLOSE);
				break;
			}
		}
	}

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_ShowRealPlay(LPCTSTR pResourceID, LPCTSTR pLocalMediaAddr, LPCTSTR pRemoteMediaAddr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 入参检测
	if (NULL == pResourceID || NULL == pLocalMediaAddr || NULL == pRemoteMediaAddr)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 状态校验
	if (NULL == m_VideoPane.GetSafeHwnd())
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Please load ocx first.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "ResourceID:%s, LocalMediaAddr:%s, RemoteMediaAddr:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pLocalMediaAddr).c_str(), eLTE_Tool::UnicodeToANSI(pRemoteMediaAddr).c_str());
		return strResult.AllocSysString();
	}

	// 解析XML入参
	std::string strLocalIP("");
	unsigned int uiLocalVideoPort = 0;
	unsigned int uiLocalAudioPort = 0;
	std::string strRemoteIP("");
	unsigned int uiRemoteVideoPort = 0;
	unsigned int uiRemoteAudioPort = 0;
	int iRet = XMLProcess::XmlParsePlayVideoParam(
		eLTE_Tool::UnicodeToANSI(pLocalMediaAddr),
		eLTE_Tool::UnicodeToANSI(pRemoteMediaAddr),
		strLocalIP, uiLocalVideoPort, uiLocalAudioPort,
		strRemoteIP, uiRemoteVideoPort, uiRemoteAudioPort);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("XmlParsePlayVideoParam failed.");
		LOG_INTERFACE_INFO(iRet, "ResourceID:%s, LocalMediaAddr:%s, RemoteMediaAddr:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pLocalMediaAddr).c_str(), eLTE_Tool::UnicodeToANSI(pRemoteMediaAddr).c_str());
		return strResult.AllocSysString();
	}

	// 设置资源ID
	m_MediaPlayer.SetMediaPlayerResID(eLTE_Tool::UnicodeToANSI(pResourceID));

	// 初始化播放器

	if(m_ulByMediaPass)
	{
		iRet = m_MediaPlayer.InitPlayerParam((IVS_CHAR*)strLocalIP.c_str(),//lint !e1773
			uiLocalVideoPort, uiLocalAudioPort);
		if (eLTE_ERR_SUCCESS != iRet)
		{
			GET_RETURN_CODE_XML(xml, iRet, strResult);
			LOG_RUN_ERROR("MediaPlayer InitPlayerParam failed.");
			LOG_INTERFACE_INFO(iRet, "ResourceID:%s, LocalMediaAddr:%s, RemoteMediaAddr:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pLocalMediaAddr).c_str(), eLTE_Tool::UnicodeToANSI(pRemoteMediaAddr).c_str());
			return strResult.AllocSysString();
		}
	}

	// 播放视频
	iRet = m_MediaPlayer.StartPlayer((IVS_CHAR*)strRemoteIP.c_str(),//lint !e1773
		uiRemoteVideoPort, uiRemoteAudioPort, m_VideoPane.GetVideoStaticSafeHwnd());
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("MediaPlayer StartPlayer failed.");
		LOG_INTERFACE_INFO(iRet, "ResourceID:%s, LocalMediaAddr:%s, RemoteMediaAddr:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pLocalMediaAddr).c_str(), eLTE_Tool::UnicodeToANSI(pRemoteMediaAddr).c_str());
		return strResult.AllocSysString();
	}

	// 伴音判断
	std::string strMuteType = OperationMgr::Instance().GetMuteType(eLTE_Tool::WString2Int(pResourceID));
	if ("0" == strMuteType)
	{
		// 有伴音
		m_VideoPane.EnableAudio(TRUE);
	}
	else if ("1" == strMuteType)
	{
		// 无伴音
		m_VideoPane.EnableAudio(FALSE);
	}

	// 是否切换摄像头的判断。m_VideoType=0:调度台请求视频，m_VideoType=1:终端上传视频或视频分发
	std::string strVideoType = OperationMgr::Instance().GetVideoType(eLTE_Tool::WString2Int(pResourceID));
	if ("0" == strVideoType)
	{
		// 调度台发起视频
		m_VideoPane.EnableReversePlay(TRUE);
	}
	else if ("1" == strVideoType)
	{
		// 终端上传视频或者视频分发
		m_VideoPane.EnableReversePlay(FALSE);
	}

	// 显示播放窗口
	m_VideoPane.ShowWindow(SW_SHOW);
	m_VideoPane.Invalidate(TRUE);

	// 激活工具栏按钮
	//if(m_ulByMediaPass)
	//{
		m_VideoPane.EnableImageButton(TRUE);
	//}

	// 播放器窗体显示事件
	PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_SHOW, NULL);

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, LocalMediaAddr:%s, RemoteMediaAddr:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pLocalMediaAddr).c_str(), eLTE_Tool::UnicodeToANSI(pRemoteMediaAddr).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_Snapshot(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 状态校验
	if (NULL == m_VideoPane.GetSafeHwnd())
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Please load ocx first.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
		return strResult.AllocSysString();
	}

	// 本地抓图
	std::string strSnapshotPath = "";
	int iRet = m_MediaPlayer.LocalSnapshot(eLTE_Tool::UnicodeToANSI(pResourceID), strSnapshotPath);
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	if (eLTE_ERR_SUCCESS == iRet)
	{
		PlayerEventParamInfo* pInfo = new PlayerEventParamInfo;
		if (NULL != pInfo)
		{
			pInfo->strSnapshotPath = strSnapshotPath;
			// 播放器抓图事件
			PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_SNAPSHOT, (LPARAM)pInfo);
		}
	}

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_HideRealPlay(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 状态校验
	if (NULL == m_VideoPane.GetSafeHwnd())
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Please load ocx first.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "");
		return strResult.AllocSysString();
	}
	// 停止播放视频
	int iRet = m_MediaPlayer.StopPlayer();
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	m_VideoPane.PlayAudio(FALSE);

	// 隐藏播放窗口
	m_VideoPane.ShowWindow(SW_HIDE);

	// 播放器窗体隐藏事件
	//PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_HIDE, NULL);

	
	LOG_INTERFACE_INFO(iRet, "");
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_PlaySound(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 状态校验
	if (NULL == m_VideoPane.GetSafeHwnd())
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Please load ocx first.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "");
		return strResult.AllocSysString();
	}

	// 开始播放音频
	int iRet = m_MediaPlayer.PlayAudio();
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	// 设置静音按钮
	if (eLTE_ERR_SUCCESS == iRet)
	{
		m_VideoPane.PlayAudio(TRUE);

		// 播放器取消静音事件
		PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_UNMUTE, NULL);
	}

	LOG_INTERFACE_INFO(iRet, "");
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_StopSound(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 状态校验
	if (NULL == m_VideoPane.GetSafeHwnd())
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Please load ocx first.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "");
		return strResult.AllocSysString();
	}

	// 停止播放音频
	int iRet = m_MediaPlayer.StopAudio();

	// 设置静音按钮
	if (eLTE_ERR_SUCCESS == iRet)
	{
		m_VideoPane.PlayAudio(FALSE);

		// 播放器静音事件
		PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_MUTE, NULL);
	}

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "");
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_SetVideoWindowPos(ULONG ulLeft, ULONG ulTop, ULONG ulWidth, ULONG ulHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 状态校验
	if (NULL == m_VideoPane.GetSafeHwnd())
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Please load ocx first.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "Left:%d, Top:%d, Width:%d, Height:%d", ulLeft, ulTop, ulWidth, ulHeight);
		return strResult.AllocSysString();
	}

	m_VideoPane.SetWindowPos(NULL, (int)ulLeft, (int)ulTop, (int)ulWidth, (int)ulHeight, SWP_DRAWFRAME);
	m_VideoPane.Invalidate(TRUE);
	int iRet = eLTE_ERR_SUCCESS;
	// 更新播放窗口大小
	if(!m_ulByMediaPass)
	{
		RECT wnd;
		memset(&wnd, 0x0, sizeof(wnd));
		m_VideoPane.GetVideoStatic().GetWindowRect(&wnd);
		ELTE_INT32 iResId = m_MediaPlayer.GetMediaPlayerResID();
		iRet = ELTE_SDK_SetPlayWindowSize(eLTE_Tool::Int2String(iResId).c_str(), (ELTE_ULONG)(wnd.right - wnd.left), (ELTE_ULONG)(wnd.bottom - wnd.top));
		if(eLTE_ERR_SUCCESS != iRet)
		{
			LOG_RUN_ERROR("When set video window pos, call ELTE_SDK_SetPlayWindowSize failed, the res id is %d.", iResId);
		}
	}
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "Left:%d, Top:%d, Width:%d, Height:%d", ulLeft, ulTop, ulWidth, ulHeight);
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_FullScreenDisplay(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 状态校验
	if (NULL == m_VideoPane.GetSafeHwnd())
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Please load ocx first.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "");
		return strResult.AllocSysString();
	}

	eLTE_RET iRet = m_VideoPane.SetFullScreen();
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	if (eLTE_ERR_SUCCESS == iRet)
	{
		// 播放器全屏事件
		PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_FULLSCREEN, NULL);
	}

	LOG_INTERFACE_INFO(iRet, "");
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_NormalScreenDisplay(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 状态校验
	if (NULL == m_VideoPane.GetSafeHwnd())
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Please load ocx first.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "");
		return strResult.AllocSysString();
	}

	eLTE_RET iRet = m_VideoPane.ResetFullScreen();
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	if (eLTE_ERR_SUCCESS == iRet)
	{
		// 播放器退出全屏事件
		PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_NORMALSCREEN, NULL);
	}

	LOG_INTERFACE_INFO(iRet, "");
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_SetTitleText(LPCTSTR pTitleText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 入参校验
	if (NULL == pTitleText)
	{
		LOG_RUN_ERROR("pTitleText is null.");
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 状态校验
	if (NULL == m_VideoPane.GetSafeHwnd())
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_FAILED, strResult);
		LOG_RUN_ERROR("Please load ocx first.");
		LOG_INTERFACE_INFO(eLTE_ERR_FAILED, "TitleText:%s", eLTE_Tool::UnicodeToANSI(pTitleText).c_str());
		return strResult.AllocSysString();
	}

	m_VideoPane.SetCaptionText(pTitleText);
	GET_RETURN_CODE_XML(xml, eLTE_ERR_SUCCESS, strResult);

	// 播放器取消静音事件
	PlayerEventParamInfo* pInfo = new PlayerEventParamInfo;
	if (NULL != pInfo)
	{
		pInfo->strTitle = eLTE_Tool::UnicodeToANSI(pTitleText);
		PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_TITLE, (LPARAM)pInfo);
	}

	LOG_INTERFACE_INFO(eLTE_ERR_SUCCESS, "TitleText:%s", eLTE_Tool::UnicodeToANSI(pTitleText).c_str());
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_CreateDynamicGroup(LPCTSTR pDGNAParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pDGNAParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pDGNAParam is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 创建动态重组
	ELTE_INT32 iRet = ELTE_SDK_CreateDynamicGroup(eLTE_Tool::UnicodeToANSI(pDGNAParam).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "DGNAParam:%s", eLTE_Tool::UnicodeToANSI(pDGNAParam).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_CancelDynamicGroup(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 删除动态重组
	ELTE_INT32 iRet = ELTE_SDK_CancelDynamicGroup(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_CreateTempGroup(LPCTSTR pTEMPGParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pTEMPGParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pTEMPGParam is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 创建临时组
	ELTE_INT32 iRet = ELTE_SDK_CreateTempGroup(eLTE_Tool::UnicodeToANSI(pTEMPGParam).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "TEMPGParam:%s", eLTE_Tool::UnicodeToANSI(pTEMPGParam).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetNextTempGroupID(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 申请一个临时组号
	ELTE_INT32 iGroupID = 0; 
	ELTE_INT32 iRet	= ELTE_SDK_GetTempGroupID(&iGroupID);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetTempGroupID failed.");
		return strResult.AllocSysString();
	}

	// 获取返回XML
	GET_NEXT_TEMP_GROUPID_XML(xml, iGroupID, strResult);

	LOG_INTERFACE_INFO(eLTE_ERR_SUCCESS, "");
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_P2PDial(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code hereLOG_INTERFACE_TRACE();
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 发起语音点呼
	int iRet = ELTE_SDK_P2PDial(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_P2PRecv(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 接收语音点呼
	int iRet = ELTE_SDK_P2PRecv(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_P2PReject(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 拒接语音点呼
	int iRet = ELTE_SDK_P2PReject(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_P2PHangup(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 挂断语音点呼
	int iRet = ELTE_SDK_P2PHangup(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


//BSTR CeLTE_PlayerCtrl::ELTE_OCX_JoinGroup(LPCTSTR pGroupID)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	CString strResult;
//
//	// TODO: Add your dispatch handler code here
//	LOG_INTERFACE_TRACE();
//	CXml xml;
//
//	// load类型是否支持
//	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
//
//	// 入参检测
//	if (NULL == pGroupID)
//	{
//		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
//		LOG_RUN_ERROR("pGroupID is null.");
//		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
//		return strResult.AllocSysString();
//	}
//
//	// 加入群组
//	int iRet = ELTE_SDK_SubJoinGroup(eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
//	GET_RETURN_CODE_XML(xml, iRet, strResult);
//
//	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
//	return strResult.AllocSysString();
//}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_SubJoinGroup(LPCTSTR pGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pGroupID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 订阅并自动加入群组
	int iRet = ELTE_SDK_SubJoinGroup(eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_PTTDial(LPCTSTR pGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pGroupID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 发起组呼（固定和临时组呼）或抢权
	int iRet = ELTE_SDK_PTTDial(eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_PTTRelease(LPCTSTR pGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pGroupID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 释放话权
	int iRet = ELTE_SDK_PTTRelease(eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_PTTHangup(LPCTSTR pGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pGroupID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 退出组呼
	int iRet = ELTE_SDK_PTTHangup(eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_PTTEmergency(LPCTSTR pGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pGroupID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 发起紧急组呼
	int iRet = ELTE_SDK_PTTEmergency(eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GroupBreakoff(LPCTSTR pGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pGroupID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 强拆组呼
	int iRet = ELTE_SDK_GroupBreakoff(eLTE_Tool::UnicodeToANSI(pGroupID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());

	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_P2PBreakin(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 抢话（强拆点呼+发起新点呼）
	int iRet = ELTE_SDK_P2PBreakin(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_P2PBreakoff(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 强拆点呼
	int iRet = ELTE_SDK_P2PBreakoff(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_VolMute(LPCTSTR pResourceID, LPCTSTR pMuteParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 入参检测
	if (NULL == pResourceID || NULL == pMuteParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	//执行静音
	ELTE_INT32 iRet = ELTE_SDK_VolMute(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pMuteParam).c_str());


	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, MuteParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pMuteParam).c_str());
	m_MediaPlayer.SetAllowPlaySound(FALSE);
	// 设置静音按钮
	if (eLTE_ERR_SUCCESS == iRet)
	{
		if(m_VideoPane.GetAudioBtnState())
		{
			m_VideoPane.PlayAudio(FALSE);
		}
		// 播放器静音事件
		PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_MUTE, NULL);
	}
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_VolUnMute(LPCTSTR pResourceID, LPCTSTR pMuteParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// 入参检测
	if (NULL == pResourceID || NULL == pMuteParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 取消静音
	ELTE_INT32 iRet = ELTE_SDK_VolUnMute(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pMuteParam).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, MuteParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pMuteParam).c_str());
	m_MediaPlayer.SetAllowPlaySound(TRUE);
	// 设置静音按钮
	if (eLTE_ERR_SUCCESS == iRet)
	{
		m_VideoPane.PlayAudio(TRUE);

		// 播放器取消静音事件
		PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_UNMUTE, NULL);
	}
	return strResult.AllocSysString();
}


//BSTR CeLTE_PlayerCtrl::ELTE_OCX_MuteControl(LPCTSTR pResourceID, LPCTSTR pMuteType)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	CString strResult;
//
//	// TODO: Add your dispatch handler code here
//	LOG_INTERFACE_TRACE();
//	CXml xml;
//
//	// load类型是否支持
//	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
//
//	// 入参检测
//	if (NULL == pResourceID || NULL == pMuteType)
//	{
//		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
//		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
//		return strResult.AllocSysString();
//	}
//
//	// 执行静音控制
//	//int iRet = OperationMgr::Instance().MuteControl(eLTE_Tool::WString2Int(pResourceID), eLTE_Tool::WString2Int(pMuteType));
//	int iRet = 0;
//
//	GET_RETURN_CODE_XML(xml, iRet, strResult);
//	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, MuteParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pMuteType).c_str());
//	return strResult.AllocSysString();
//}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GISSubscribe(LPCTSTR pResourceID, LPCTSTR pGISParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID || NULL == pGISParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// GIS订阅/去订阅
	ELTE_INT32 iRet = ELTE_SDK_GISSubscribe(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pGISParam).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, GISParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pGISParam).c_str());


	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_SDSSendMessage(LPCTSTR pResourceID, LPCTSTR pSDSParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID || NULL == pSDSParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 发送短彩信
	ELTE_INT32 iRet = ELTE_SDK_SDSSendMessage(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pSDSParam).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());

	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_SetBypassBuildMedia(ULONG ulBypass)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	//LOG_INTERFACE_TRACE();
	
	CXml xml;

	m_ulByMediaPass = ulBypass;

	GET_RETURN_CODE_XML(xml, eLTE_ERR_SUCCESS, strResult);
	//LOG_INTERFACE_INFO(eLTE_ERR_SUCCESS, "Bypass:%d", ulBypass);
	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_ProvisionManagerInitMRS(LPCTSTR pServerIP)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pServerIP)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	ELTE_INT32 iRet = ELTE_SDK_ProvisionManagerInitMRS(eLTE_Tool::UnicodeToANSI(pServerIP).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ServerIP:%s", eLTE_Tool::UnicodeToANSI(pServerIP).c_str());
	return strResult.AllocSysString();
}//lint !e1762


// 抓图
int CeLTE_PlayerCtrl::ToolBarSnapshot()
{
	LOG_TRACE();
	std::string strSnapshotPath = "";
	int iRet = m_MediaPlayer.LocalSnapshot(strSnapshotPath);

	if (eLTE_ERR_SUCCESS == iRet)
	{
		PlayerEventParamInfo* pInfo = new PlayerEventParamInfo;
		if (NULL != pInfo)
		{
			pInfo->strSnapshotPath = strSnapshotPath;
			// 播放器抓图事件
			PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_SNAPSHOT, (LPARAM)pInfo);
		}
	}
	return iRet;
}


// 切换前后置摄像头
int CeLTE_PlayerCtrl::ToolBarReversePlayer()
{
	LOG_TRACE();

	// 关闭视频播放
	(void)m_MediaPlayer.StopAudio();
	(void)m_MediaPlayer.StopPlayer();

	//// 重置界面
	//m_VideoPane.Invalidate(TRUE);

	//// 修改界面
	//m_VideoPane.PlayAudio(FALSE);
	m_VideoPane.EnableImageButton(FALSE);

	// 切换镜头
	int iResId = m_MediaPlayer.GetMediaPlayerResID();
	int iRet = OperationMgr::Instance().DcVideoReverse(iResId);

	//----------------异步播放
	std::map<int, CeLTE_PlayerCtrl*>::iterator itor = m_WaitPlayVideoList.find(iResId);
	if (m_WaitPlayVideoList.end() == itor)
	{
		(void)m_WaitPlayVideoList.insert(std::make_pair(iResId, this));
	}
	return iRet;
}


// 开启关闭伴音
int CeLTE_PlayerCtrl::ToolBarPlayAudio(BOOL bEnable)
{
	LOG_TRACE();
	int iRet;
	if (bEnable)
	{
		iRet = m_MediaPlayer.PlayAudio();
		if (eLTE_ERR_SUCCESS == iRet)
		{
			// 播放器取消静音事件
			PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_UNMUTE, NULL);
		}
	}
	else
	{
		
		iRet = m_MediaPlayer.StopAudio();
		if (eLTE_ERR_SUCCESS == iRet)
		{
			// 播放器取消静音事件
			PostMessage(WM_ELTE_POST_PLAYER_EVENT, (WPARAM)eLTE_PLAYER_MUTE, NULL);
		}
	}
	return iRet;
}



BSTR CeLTE_PlayerCtrl::ELTE_OCX_StartVideoDispatch(LPCTSTR pResourceID, LPCTSTR pVideoDispatchParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID || NULL == pVideoDispatchParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 开始视频分发
	ELTE_INT32 iRet = ELTE_SDK_StartVideoDispatch(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoDispatchParam).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoDispatchParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoDispatchParam).c_str());


	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_StopVideoDispatch(LPCTSTR pResourceID, LPCTSTR pVideoDispatchParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID || NULL == pVideoDispatchParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 开始视频分发
	ELTE_INT32 iRet = ELTE_SDK_StopVideoDispatch(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoDispatchParam).c_str());
		
	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoDispatchParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pVideoDispatchParam).c_str());

	return strResult.AllocSysString();
}//lint !e1762


BSTR CeLTE_PlayerCtrl::ELTE_OCX_RecvVideoPlay(LPCTSTR pResourceID, LPCTSTR pMuteType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID || NULL == pMuteType)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 解析视频参数
	std::string strVideoFormat("-1");
	std::string strCameraType("-1");
	std::string strUserConfirmType("-1");
	std::string strMuteType(eLTE_Tool::UnicodeToANSI(pMuteType));
	std::string strVideoType("1");//0:调度台请求视频；1：终端上传视频、调度台视频分发

	// 构建视频参数
	STVideoParam param;
	param.strFmtValue = strVideoFormat;
	param.strCameraType = strCameraType;
	param.strUserCfmType = strUserConfirmType;
	param.strMuteType = strMuteType;
	param.strVideoType = strVideoType;
	// 接收终端视频上传请求
	ELTE_INT32 iRet = ELTE_SDK_RecvVideoPlay(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	if (eLTE_ERR_SUCCESS == iRet)
	{
		iRet = OperationMgr::Instance().DcVideoMonitor(eLTE_Tool::WString2Int(pResourceID), param);
	}

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoParam:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pMuteType).c_str());

	return strResult.AllocSysString();
}//lint !e1762

BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetDcVWallIDList(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here

	LOG_INTERFACE_TRACE();
	CXml xml;
	
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	
	//获取ID列表
	ELTE_CHAR* pIDList = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetDcVWallIDList(&pIDList);
	
	if(NULL != pIDList)
	{
		strResult = pIDList;
		LOG_RUN_DEBUG("ELTE_SDK_GetDcVWallIDList return value:%s", pIDList);

		ELTE_INT32 iResult = ELTE_SDK_ReleaseBuffer(pIDList);
		if (eLTE_ERR_SUCCESS != iResult)
		{
			GET_RETURN_CODE_XML(xml, iResult, strResult);
			LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
			LOG_INTERFACE_INFO(iResult, "");
			return strResult.AllocSysString();
		}
	}

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetDcVWallIDList failed.");
		LOG_INTERFACE_INFO(iRet, "");
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<VWallIDList>"));
	LOG_INTERFACE_INFO(iRet, "");
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_VWallStart(LPCTSTR pResVWallID, LPCTSTR pVWallStartParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here

	CXml xml;
	LOG_INTERFACE_TRACE();
	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	// 入参检测
	if (NULL == pResVWallID || NULL == pVWallStartParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}
	
	ELTE_INT32 iRet = ELTE_SDK_VWallStart(eLTE_Tool::UnicodeToANSI(pResVWallID).c_str(), eLTE_Tool::UnicodeToANSI(pVWallStartParam).c_str());
	
	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VWallParam:%s", eLTE_Tool::UnicodeToANSI(pResVWallID).c_str(), eLTE_Tool::UnicodeToANSI(pVWallStartParam).c_str());

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_VWallStop(LPCTSTR pResVWallID, LPCTSTR pVWallStopParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here

	LOG_INTERFACE_TRACE();
	CXml xml;
	
	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	
	// 入参检测
	if (NULL == pResVWallID || NULL == pVWallStopParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}
	
	// 接收终端视频上传请求
	ELTE_INT32 iRet = ELTE_SDK_VWallStop(eLTE_Tool::UnicodeToANSI(pResVWallID).c_str(), eLTE_Tool::UnicodeToANSI(pVWallStopParam).c_str());
	
	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "ResourceID:%s, VideoParam:%s", eLTE_Tool::UnicodeToANSI(pResVWallID).c_str(), eLTE_Tool::UnicodeToANSI(pVWallStopParam).c_str());

	return strResult.AllocSysString();
}



BSTR CeLTE_PlayerCtrl::ELTE_OCX_TelephoneDial(LPCTSTR pTelNumber)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LOG_INTERFACE_TRACE();
	CString strResult;
	CXml xml;
	//load 类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	//入参检测
	if (NULL == pTelNumber)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}
	//发起PSTN/PLMN电话呼叫
	ELTE_INT32 iRet = ELTE_SDK_TelephoneDial(eLTE_Tool::UnicodeToANSI(pTelNumber).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "TelNumber:%s",eLTE_Tool::UnicodeToANSI(pTelNumber).c_str());
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_TelephoneHangup(LPCTSTR pTelNumber)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LOG_INTERFACE_TRACE();
	CString strResult;
	CXml xml;
	//load 类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	//入参检测
	if (NULL == pTelNumber)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}
	//挂断PSTN/PLMN电话呼叫
	ELTE_INT32 iRet = ELTE_SDK_TelephoneHangup(eLTE_Tool::UnicodeToANSI(pTelNumber).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "TelNumber:%s",eLTE_Tool::UnicodeToANSI(pTelNumber).c_str());
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_StartDiscreetListen(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LOG_INTERFACE_TRACE();
	CString strResult;
	CXml xml;
	//load 类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	//入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}
	//发起缜密侦听
	ELTE_INT32 iRet = ELTE_SDK_StartDiscreetListen(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "TelNumber:%s",eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_StopDiscreetListen(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LOG_INTERFACE_TRACE();
	CString strResult;
	CXml xml;
	//load 类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	//入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}
	//停止缜密侦听
	ELTE_INT32 iRet = ELTE_SDK_StopDiscreetListen(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "TelNumber:%s",eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_StartEnvironmentListen(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LOG_INTERFACE_TRACE();
	
	CString strResult;
	CXml xml;
	//load 类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	//入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}
	//发起环境侦听
	ELTE_INT32 iRet = ELTE_SDK_StartEnvironmentListen(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "TelNumber:%s",eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_P2PTransfer(LPCTSTR pResourceID, LPCTSTR pP2PTransferParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LOG_INTERFACE_TRACE();
	CString strResult;

	CXml xml;
	//load 类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	//入参检测
	if (NULL == pResourceID || NULL == pP2PTransferParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}
	//转接
	ELTE_INT32 iRet = ELTE_SDK_P2PTransfer(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pP2PTransferParam).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "pResourceID:%s  pP2PTransferParam:%s",\
		eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), \
		eLTE_Tool::UnicodeToANSI(pP2PTransferParam).c_str() \
		);

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_CreatePatchGroup(LPCTSTR pPatchGroupParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
	
	// 入参检测
	if (NULL == pPatchGroupParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pPatchGroupParam is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 创建派接组
	ELTE_INT32 iRet = ELTE_SDK_CreatePatchGroup(eLTE_Tool::UnicodeToANSI(pPatchGroupParam).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "PatchGroupParam:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupParam).c_str());
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_CancelPatchGroup(BSTR pPatchGroupID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pPatchGroupID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pPatchGroupID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 删除动态重组
	ELTE_INT32 iRet = ELTE_SDK_CancelPatchGroup(eLTE_Tool::UnicodeToANSI(pPatchGroupID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "PatchGroupID:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupID).c_str());
	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_AddPatchGroupMember(LPCTSTR pPatchGroupParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pPatchGroupParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pPatchGroupParam is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 增加派接组成员
	ELTE_INT32 iRet = ELTE_SDK_AddPatchGroupMember(eLTE_Tool::UnicodeToANSI(pPatchGroupParam).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "PatchGroupParam:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupParam).c_str());

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_DeletePatchGroupMember(LPCTSTR pPatchGroupParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pPatchGroupParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pPatchGroupParam is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 删除派接组成员
	ELTE_INT32 iRet = ELTE_SDK_DeletePatchGroupMember(eLTE_Tool::UnicodeToANSI(pPatchGroupParam).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "PatchGroupParam:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupParam).c_str());

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetPatchGroups(LPCTSTR pDcUser)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pDcUser)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取派接组列表
	ELTE_CHAR* pPatchGroups = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetPatchGroups(eLTE_Tool::UnicodeToANSI(pDcUser).c_str(), &pPatchGroups);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetPatchGroups failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pDcUser).c_str());
		(void)ELTE_SDK_ReleaseBuffer(pPatchGroups);
		return strResult.AllocSysString();
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	std::string strSrc = pPatchGroups;
	int iExistElement = 0;
	//字符转码
	for (;;)
	{
		iSplitFlag = 0;
		eLTE_Tool::SplitString(strSrc, "<PGName>", "</PGName>", strStart, strCentre, strEnd, iSplitFlag);
		if(1 == iSplitFlag)
		{
			strResult.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
			strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
			//strResult.Append(eLTE_Tool::ANSIToUnicode("</GroupName><GroupName>").c_str());
			iExistElement = 1;
		}
		else
		{
			break;
		}
		strSrc = strEnd;
	}

	if(1 == iExistElement)
	{
		strResult.Append(eLTE_Tool::ANSIToUnicode("</PGName></PatchGroupInfo></PatchGroupInfoList></Content>").c_str());
	}
	else
	{
		strResult = eLTE_Tool::ANSIToUnicode(pPatchGroups).c_str();
	}

	iRet = ELTE_SDK_ReleaseBuffer(pPatchGroups);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pDcUser).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<PatchGroupInfoList>"));

	LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pDcUser).c_str());


	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetGroupMemberByPatchId(LPCTSTR pPatchGroupId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pPatchGroupId)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取派接组列表
	ELTE_CHAR* pPatchGroupMembers = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetGroupMemberByPatchId(eLTE_Tool::UnicodeToANSI(pPatchGroupId).c_str(), &pPatchGroupMembers);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetGroupMemberByPatchId failed.");
		LOG_INTERFACE_INFO(iRet, "PatchGroupId:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupId).c_str());
		(void)ELTE_SDK_ReleaseBuffer(pPatchGroupMembers);
		return strResult.AllocSysString();
	}
	
	strResult = eLTE_Tool::ANSIToUnicode(pPatchGroupMembers).c_str();

	iRet = ELTE_SDK_ReleaseBuffer(pPatchGroupMembers);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "PatchGroupId:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupId).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCodep
	eLTE_Tool::AddResultCodeNode(strResult, _T("<PatchGroupMemberList>"));

	LOG_INTERFACE_INFO(iRet, "PatchGroupId:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupId).c_str());

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetPatchGroupInfo(LPCTSTR pPatchGroupId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pPatchGroupId)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取群组详细配置信息
	ELTE_CHAR* pGroupInfo = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetPatchGroupInfo(eLTE_Tool::UnicodeToANSI(pPatchGroupId).c_str(), &pGroupInfo);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetPatchGroupInfo failed.");
		LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupId).c_str());
		(void)ELTE_SDK_ReleaseBuffer(pGroupInfo);
		return strResult.AllocSysString();
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	eLTE_Tool::SplitString(pGroupInfo, "<PGName>", "</PGName>", strStart, strCentre, strEnd, iSplitFlag);

	if(0 == iSplitFlag)
	{
		strResult = eLTE_Tool::ANSIToUnicode(pGroupInfo).c_str();
	}
	else
	{
		strResult.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
		strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
		strResult.Append(eLTE_Tool::ANSIToUnicode(strEnd).c_str());
	}

	iRet = ELTE_SDK_ReleaseBuffer(pGroupInfo);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupId).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<PatchGroupInfo>"));

	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pPatchGroupId).c_str());

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_PTZControl(LPCTSTR pResourceID, ULONG ulPTZControlCode, ULONG ulPTZControlValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	ELTE_INT32 iRet = ELTE_SDK_PTZControl(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), ulPTZControlCode, ulPTZControlValue);

	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	return strResult.AllocSysString();
}


//BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetUserSpecificGISCfg(LPCTSTR pResourceID)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	CString strResult;
//
//	// TODO: Add your dispatch handler code here
//	LOG_INTERFACE_TRACE();
//	CXml xml;
//
//	// load类型是否支持
//	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
//
//	// 入参检测
//	if (NULL == pResourceID)
//	{
//		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
//		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
//		return strResult.AllocSysString();
//	}
//
//	// 获取GIS配置信息
//	ELTE_CHAR* pGisCfgInfo = NULL;
//	ELTE_INT32 iRet = ELTE_SDK_GetUserSpecificGISCfg(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), &pGisCfgInfo);
//
//	if (eLTE_ERR_SUCCESS != iRet)
//	{
//		GET_RETURN_CODE_XML(xml, iRet, strResult);
//		LOG_RUN_ERROR("GetUserSpecificGISCfg failed.");
//		LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
//		return strResult.AllocSysString();
//	}
//
//	strResult = pGisCfgInfo;
//
//	iRet = ELTE_SDK_ReleaseBuffer(pGisCfgInfo);
//	if (eLTE_ERR_SUCCESS != iRet)
//	{
//		GET_RETURN_CODE_XML(xml, iRet, strResult);
//		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
//		LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
//		return strResult.AllocSysString();
//	}
//
//	//add ResultCode
//	eLTE_Tool::AddResultCodeNode(strResult, _T("<UeGisCfgInfo>"));
//
//	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
//
//	return strResult.AllocSysString();
//}
//
//
//BSTR CeLTE_PlayerCtrl::ELTE_OCX_SetGisParam(LPCTSTR pResourceID, LPCTSTR pUeGisCfg)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	CString strResult;
//
//	// TODO: Add your dispatch handler code here
//	LOG_INTERFACE_TRACE();
//	CXml xml;
//
//	// load类型是否支持
//	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);
//
//	// 入参检测
//	if (NULL == pResourceID || NULL == pUeGisCfg)
//	{
//		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
//		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
//		return strResult.AllocSysString();
//	}
//
//	// set gis cfg
//	ELTE_INT32 iRet = ELTE_SDK_SetGisParam(eLTE_Tool::UnicodeToANSI(pResourceID).c_str(), eLTE_Tool::UnicodeToANSI(pUeGisCfg).c_str());
//
//	GET_RETURN_CODE_XML(xml, iRet, strResult);
//	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
//
//	return strResult.AllocSysString();
//}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_GetGisSubscription(LPCTSTR pUserID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pUserID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 获取调度台订阅GIS上报的终端列表
	ELTE_CHAR* pGisQuerySubList = NULL;
	ELTE_INT32 iRet = ELTE_SDK_GetGisSubscription(eLTE_Tool::UnicodeToANSI(pUserID).c_str(), &pGisQuerySubList);

	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("GetGisSubscription failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	std::string strStart;
	std::string strCentre;
	std::string strEnd;
	int iSplitFlag = 0;
	std::string strSrc = pGisQuerySubList;
	int iExistElement = 0;
	//字符转码
	for (;;)
	{
		iSplitFlag = 0;
		eLTE_Tool::SplitString(strSrc, "<UserName>", "</UserName>", strStart, strCentre, strEnd, iSplitFlag);
		if(1 == iSplitFlag)
		{
			strResult.Append(eLTE_Tool::ANSIToUnicode(strStart).c_str());
			strResult.Append(eLTE_Tool::UTF8ToUnicode(strCentre).c_str());
			//strResult.Append(eLTE_Tool::ANSIToUnicode("</UserName><UserName>").c_str());
			iExistElement = 1;
		}
		else
		{
			break;
		}
		strSrc = strEnd;
	}

	if(1 == iExistElement)
	{
		strResult.Append(eLTE_Tool::ANSIToUnicode("</UserName></GisQuerySubscription></GisQuerySubList></Content>").c_str());
	}
	else
	{
		strResult = eLTE_Tool::ANSIToUnicode(pGisQuerySubList).c_str();
	}

	iRet = ELTE_SDK_ReleaseBuffer(pGisQuerySubList);
	if (eLTE_ERR_SUCCESS != iRet)
	{
		GET_RETURN_CODE_XML(xml, iRet, strResult);
		LOG_RUN_ERROR("ELTE_SDK_ReleaseBuffer failed.");
		LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());
		return strResult.AllocSysString();
	}

	//add ResultCode
	eLTE_Tool::AddResultCodeNode(strResult, _T("<GisQuerySubList>"));

	LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_ModifyDynamicGroup(LPCTSTR pUserID, LPCTSTR pDynamicGroupInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pDynamicGroupInfo || NULL == pUserID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// modify dynamic group
	ELTE_INT32 iRet = ELTE_SDK_ModifyDynamicGroup(eLTE_Tool::UnicodeToANSI(pUserID).c_str(), eLTE_Tool::UnicodeToANSI(pDynamicGroupInfo).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "UserID:%s", eLTE_Tool::UnicodeToANSI(pUserID).c_str());

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_P2PHalfDpxDial(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 发起半双工点呼
	int iRet = ELTE_SDK_P2PHalfDpxDial(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_P2PHalfDpxRelease(LPCTSTR pResourceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pResourceID)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_RUN_ERROR("pResourceID is null.");
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// 释放半双工点呼
	int iRet = ELTE_SDK_P2PHalfDpxRelease(eLTE_Tool::UnicodeToANSI(pResourceID).c_str());
	GET_RETURN_CODE_XML(xml, iRet, strResult);

	LOG_INTERFACE_INFO(iRet, "ResourceID:%s", eLTE_Tool::UnicodeToANSI(pResourceID).c_str());

	return strResult.AllocSysString();
}


BSTR CeLTE_PlayerCtrl::ELTE_OCX_TempUserJoinGroup(LPCTSTR pGroupID, LPCTSTR pPhonePatchParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	LOG_INTERFACE_TRACE();
	CXml xml;

	// load类型是否支持
	CHECK_ELTE_OCX_LOAD_TYPE(m_ulType, xml, strResult);

	// 入参检测
	if (NULL == pGroupID || NULL == pPhonePatchParam)
	{
		GET_RETURN_CODE_XML(xml, eLTE_ERR_INVALID_PARAM, strResult);
		LOG_INTERFACE_INFO(eLTE_ERR_INVALID_PARAM, "");
		return strResult.AllocSysString();
	}

	// join call 
	ELTE_INT32 iRet = ELTE_SDK_TempUserJoinGroup(eLTE_Tool::UnicodeToANSI(pGroupID).c_str(), eLTE_Tool::UnicodeToANSI(pPhonePatchParam).c_str());

	GET_RETURN_CODE_XML(xml, iRet, strResult);
	LOG_INTERFACE_INFO(iRet, "GroupID:%s", eLTE_Tool::UnicodeToANSI(pGroupID).c_str());

	return strResult.AllocSysString();
}
