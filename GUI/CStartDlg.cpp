// CStartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CStartDlg.h"
#include "afxdialogex.h"
#include "resource.h"

// CStartDlg dialog

IMPLEMENT_DYNAMIC(CStartDlg, CDialogEx)

CStartDlg::CStartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_START_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CStartDlg::~CStartDlg()
{
}

void CStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_BTN, m_btnStart);
	DDX_Control(pDX, IDC_START_TEXT, start_text);
}


BEGIN_MESSAGE_MAP(CStartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_START_BTN, &CStartDlg::OnBnClickedStartBtn)
END_MESSAGE_MAP()


// CStartDlg message handlers


void CStartDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnSysCommand(nID, lParam);
}


BOOL CStartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowPos(NULL, -1, -1, 900, 600, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	SetBackgroundImage(IDB_MAIN_BITMAP);

	// TODO:  Add extra initialization here

	//
	//	파일 로그를 초기화한다. 
	// 

	
	//
	//	사용자 임시 경로 받아오기
	//
	std::wstring user_temp_dir;
	get_temp_dirW(user_temp_dir);

	std::wstringstream strm;
	strm << user_temp_dir << L"Naga\\";
	CreateDirectory(strm.str().c_str(), NULL);
	strm << L"Naga.log";

	if (true != initialize_log(log_mask_all,
		log_level_debug,
		log_to_file | log_to_con | log_to_ods,
		strm.str().c_str()))
	{
		fwprintf(stderr, L"initialize_log() fail. give up! \n");
	}

	//
	//	로그의 출력 형식을 지정한다. 
	//
	set_log_format(false, false, false, true);

	m_btnStart.LoadStdImage(IDB_TRANS_PNG, _T("PNG"));	//	대기상태일 때 이미지
//	m_btnStart.LoadAltImage(IDC_START_PNG, _T("PNG"));	//	커서를 올려놓았을 때 이미지
	m_btnStart.EnableToggle(FALSE);
	m_btnStart.MoveWindow(0,0, 900,500);					// x, y, width, height

	GetDlgItem(IDC_VERSION)->SetWindowPos(NULL, 10, 520, 80, 20, NULL);
	GetDlgItem(IDC_TEAM)->SetWindowPos(NULL, 650, 520, 200, 20, NULL);
	start_text.SetWindowPos(NULL, 350, 520, 200, 20, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}




void CStartDlg::OnBnClickedStartBtn()
{
	// TODO: Add your control notification handler code here
	CGUIDlg dlg;
	dlg.DoModal();
	//::SendMessage(GetSafeHwnd(), WM_CLOSE, NULL, NULL);
	OnDestroy();
	OnOK();
}