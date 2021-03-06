#pragma once
#include "PictureEx.h"


// CLoadDlg dialog

class CLoadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoadDlg)

public:
	CLoadDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLoadDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void terminateDialog();
	DECLARE_MESSAGE_MAP()
public:
	CPictureEx loading_img;
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT plzClose(WPARAM wParam, LPARAM lParam);
};
