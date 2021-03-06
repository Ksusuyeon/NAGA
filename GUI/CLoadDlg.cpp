// CLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CLoadDlg.h"
#include "afxdialogex.h"
#include "resource.h"

// CLoadDlg dialog

IMPLEMENT_DYNAMIC(CLoadDlg, CDialogEx)

CLoadDlg::CLoadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOADING_DIALOG, pParent)
{

}

CLoadDlg::~CLoadDlg()
{
}

void CLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOADING, loading_img);
}


BEGIN_MESSAGE_MAP(CLoadDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_CLOSE, plzClose)
END_MESSAGE_MAP()


// CLoadDlg message handlers


BOOL CLoadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetWindowPos(NULL, -1, -1, 250, 250, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	if (loading_img.Load(MAKEINTRESOURCE(IDR_GIF1), _T("GIF"))) 
	{
		loading_img.SetWindowPos(NULL, 0, 0, 250, 250, NULL);
		loading_img.Draw();
	}
	if (!loading_img.IsGIF()) {
		log_info "not gif image" log_end;
	}
	if (!loading_img.IsAnimatedGIF()) {
		log_info "not playing now" log_end;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CLoadDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	loading_img.UnLoad();
	// TODO: Add your message handler code here
}


void CLoadDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnClose();
	OnOK();
}

void CLoadDlg::terminateDialog() {
	OnDestroy();
	OnOK();
}


void CLoadDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	// CDialogEx::OnOK();
}


void CLoadDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	// CDialogEx::OnCancel();
}


void CLoadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_CLOSE)
	{
		EndDialog(IDCANCEL);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

LRESULT CLoadDlg::plzClose(WPARAM wParam, LPARAM lParam)
{
//	OnCancel();
	return TRUE;
}