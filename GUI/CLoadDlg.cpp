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
END_MESSAGE_MAP()


// CLoadDlg message handlers


BOOL CLoadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetWindowPos(NULL, -1, -1, 300, 300, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	if (loading_img.Load(MAKEINTRESOURCE(IDR_GIF1), _T("GIF"))) 
	{
		loading_img.SetWindowPos(NULL, 0, 0, 300, 300, NULL);
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
