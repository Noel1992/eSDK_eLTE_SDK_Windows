#pragma once

#include "resource.h"
#include "DataType.h"

// CDConsoleDlg dialog
class CeLTE_SDK_GIS_DemoDlg;

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

private:
	//get dc users
	void GetDcUsers();

	//add image res
	void AddImage(CImageList& imageList, UINT id);

	// get res image by user type
	int GetUserImageId(const CString& strCategory, bool bGray);

	// get res image by user status
	int GetUserImageId(const CString& strUserId, int iStatus);	

public:
	// set CELTE_SDK_DemoDlg wnd pointer
	void SetELTE_SDK_DemoDlg(CeLTE_SDK_GIS_DemoDlg* pDlg);
	// update user status
	void UpdateUserStatus(const CString& strUserId, int iStatus);
	void SetGISStatus(CString& strGISStatus);
	void SetGISReport(CString& strGISReport);

public:
	CListCtrl m_DcUsers;

	CeLTE_SDK_GIS_DemoDlg* m_pELTE_SDK_DemoDlg;	// login wnd pointer
	virtual BOOL OnInitDialog();

private:
	CImageList m_ImageList;	    // image res list
	UserInfoList m_Users;       // DC Users  
	CString m_strGISStatus;	    // all call back string ELTE_Event_NotifyGISStatus
	CString m_strGISReport;	    // all call back string ELTE_Event_NotifyGISReport
public:
	afx_msg void OnNMRClickListDcUsers(NMHDR *pNMHDR, LRESULT *pResult);
	// operate menu
	afx_msg void OnClickMenuItem(UINT uID);
	afx_msg void OnEnChangeEditSearch();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonGisstatus();
	afx_msg void OnBnClickedButtonGisreport();
	afx_msg void OnClose();
};
