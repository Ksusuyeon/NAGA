#pragma once


// CStartDlg dialog
#include "GdipButton.h"
#include "GUIDlg.h"

class CStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartDlg)

public:
	HICON m_hIcon;
	CStartDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CStartDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_START_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStartBtn();
	CGdipButton m_btnStart;
	CStatic start_text;
};
