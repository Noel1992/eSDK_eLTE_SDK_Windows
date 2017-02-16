#pragma once

#include "resource.h"
#include "DataType.h"
#include "afxwin.h"

// CDConsoleDlg dialog
class CeLTE_SDK_Video_DemoDlg;

class CDConsoleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDConsoleDlg)

public:
	CDConsoleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDConsoleDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DCONSOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT InitDlg(WPARAM wParam, LPARAM lParam);

	// create hwnd and play
	afx_msg LRESULT StartRealPlay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIncommingVideo(WPARAM wParam, LPARAM lParam);

private:
	//get dc users
	void GetDcUsers();

	//add image res
	void AddImage(CImageList& imageList, UINT id);

	// get res image by user type
	int GetUserImageId(const CString& strCategory, bool bGray);

	// get res image by user status
	int GetUserImageId(const CString& strUserId, int iStatus);	
	
	//set video vall id
	void setVideoChannelList(const VideoChannelList& InStringList){m_strList = InStringList;}

	//get video vall id
	VideoChannelList* getVideoChannelListPointer(){return &m_strList;}

	std::string GetSnapshotPath(const std::string& strResId) const;

public:
	// set CELTE_SDK_DemoDlg wnd pointer
	void SetELTE_SDK_DemoDlg(CeLTE_SDK_Video_DemoDlg* pDlg);
	// get CELTE_SDK_DemoDlg wnd pointer
	CeLTE_SDK_Video_DemoDlg* GetELTE_SDK_DemoDlg(){return m_pELTE_SDK_DemoDlg;};
	// update user status
	void UpdateUserStatus(const CString& strUserId, int iStatus);
	// set current resid of video
	void SetPlayResID(std::string strResId);
	// set the resid of the commming video
	void SetCommingVideo(const CString& strResId) {m_strCommingVideo = strResId;}

public:
	CListCtrl m_DcUsers;
	CString m_ResStatus;	// String callback message ELTE_Event_NotifyResourceStatus
	CString m_ModuleStatus;	// String callback message ELTE_Event_NotifyModuleStatus
	CString m_GroupStatus;	// String callback message ELTE_Event_NotifyGroupStatus
	CString m_VideoStatus;	// String callback message ELTE_Event_NotifportyP2pvideocallStatus

	CeLTE_SDK_Video_DemoDlg* m_pELTE_SDK_DemoDlg;	// login wnd pointer
	virtual BOOL OnInitDialog();

private:
	CImageList m_ImageList;	    // image res list
	UserInfoList m_Users;       // DC Users  
	std::string m_strResId;     // current resid
	CString m_strCommingVideo;  // res id of the commming video
	VideoChannelList m_strList;// video vall list
	CString m_VideoChannel;    // video channel
	CString m_ResourceID;      // need vall res id

public:
	afx_msg void OnNMRClickListDcUsers(NMHDR *pNMHDR, LRESULT *pResult);
	// operate menu
	afx_msg void OnClickMenuItem(UINT uID);
	afx_msg void OnEnChangeEditSearch();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnRefreshlist();
	afx_msg void OnBnClickedBtnStartvwall();
	afx_msg void OnBnClickedBtnStopvwall();
	CComboBox m_ComboVWIDList;
	afx_msg void OnCbnSelchangeComboResource();
	afx_msg void OnBnClickedButtonDispatchvideo();
	afx_msg void OnBnClickedButtonUndispatch();
	afx_msg void OnIdok();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonResstatus();

	afx_msg void OnBnClickedButtonModulestatus();
	afx_msg void OnBnClickedButtonGroupstatus();
	afx_msg void OnBnClickedButtonVideostatus();
};
