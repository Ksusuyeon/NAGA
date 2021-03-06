// CBankDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBankDlg.h"
#include "afxdialogex.h"


// CBankDlg dialog

IMPLEMENT_DYNAMIC(CBankDlg, CDialogEx)

CBankDlg::CBankDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BANK_DIALOG, pParent)
{

}

CBankDlg::~CBankDlg()
{
}

void CBankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
	DDX_Control(pDX, IDC_CHECK4, m_check4);
	DDX_Control(pDX, IDC_CHECK5, m_check5);
	DDX_Control(pDX, IDC_CHECK6, m_check6);
	DDX_Control(pDX, IDC_CHECK7, m_check7);
	DDX_Control(pDX, IDC_CHECK8, m_check8);
	DDX_Control(pDX, IDC_CHECK9, m_check9);
	DDX_Control(pDX, IDC_CHECK10, m_check10);
	DDX_Control(pDX, IDC_CHECK11, m_check11);
	DDX_Control(pDX, IDC_CHECK12, m_check12);
	DDX_Control(pDX, IDC_CHECK13, m_check13);
}


BEGIN_MESSAGE_MAP(CBankDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBankDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK10, &CBankDlg::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK1, &CBankDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CBankDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CBankDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CBankDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CBankDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CBankDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &CBankDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &CBankDlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, &CBankDlg::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK11, &CBankDlg::OnBnClickedCheck11)
	ON_BN_CLICKED(IDC_CHECK12, &CBankDlg::OnBnClickedCheck12)
	ON_BN_CLICKED(IDC_CHECK13, &CBankDlg::OnBnClickedCheck13)
END_MESSAGE_MAP()


// CBankDlg message handlers


void CBankDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	OnDestroy();
	OnOK();
}




void CBankDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://www.kbstar.com/ "), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here 
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://www.ibk.co.kr/ "), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://pib.kjbank.com/ib20/mnu/BPB0000000001"), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck4()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT("https://banking.nonghyup.com/nhbank.html"), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck5()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://www.dgb.co.kr/dgb_ebz_main.jsp "), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck6()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://www.busanbank.co.kr/ib20/mnu/BHP00001"), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck7()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://www.citibank.co.kr/ComMainCnts0100.act?ref=https://www.google.co.kr/ "), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck8()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://www.kdb.co.kr/ih/simpleJsp.do "), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck9()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT("https://www.shinhan.com/index.jsp "), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck10()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://www.wooribank.com/ "), NULL, SW_SHOW);

}

void CBankDlg::OnBnClickedCheck11()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT("https://www.standardchartered.co.kr/np/kr/Intro.jsp"), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck12()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://www.jbbank.co.kr/ "), NULL, SW_SHOW);

}


void CBankDlg::OnBnClickedCheck13()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" https://www.kebhana.com/ "), NULL, SW_SHOW);

}


BOOL CBankDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int height = 30;
	int width = 80;
	int margin = 20;

	int first_row = 60;
	int second_row = first_row + width + margin;
	int third_row = second_row + width + margin;
	int fourth_row = third_row + width + margin;

	// TODO:  Add extra initialization here
	m_check1.SetWindowPos(NULL, first_row, 60, width, height, NULL);
	m_check2.SetWindowPos(NULL, first_row, 110, width, height, NULL);
	m_check3.SetWindowPos(NULL, first_row, 160, width, height, NULL);
	m_check4.SetWindowPos(NULL, first_row, 210, width, height, NULL);

	m_check5.SetWindowPos(NULL, second_row, 60, width, height, NULL);
	m_check6.SetWindowPos(NULL, second_row, 110, width, height, NULL);
	m_check7.SetWindowPos(NULL, second_row, 160, width, height, NULL);
	m_check8.SetWindowPos(NULL, second_row, 210, width, height, NULL);
	
	m_check9.SetWindowPos(NULL, third_row, 60, width, height, NULL);
	m_check10.SetWindowPos(NULL, third_row, 110, width, height, NULL);
	m_check11.SetWindowPos(NULL, third_row, 160, width, height, NULL);
	m_check12.SetWindowPos(NULL, third_row, 210, width, height, NULL);
	
	m_check13.SetWindowPos(NULL, fourth_row, 60, width, height, NULL);

	list_t update_list = get_update_list();
	for (auto update : update_list) {
		if (update.first.find("국민은행") != string::npos) {
			m_check1.EnableWindow(FALSE);
		}

		else if (update.first.find("기업은행") != string::npos) {
			m_check2.EnableWindow(FALSE);

		}

		else if (update.first.find("광주은행") != string::npos) {
			m_check3.EnableWindow(FALSE);

		}

		else if (update.first.find("농협") != string::npos) {
			m_check4.EnableWindow(FALSE);

		}

		else if (update.first.find("대구은행") != string::npos) {
			m_check5.EnableWindow(FALSE);

		}

		else if (update.first.find("부산은행") != string::npos) {
			m_check6.EnableWindow(FALSE);
		}

		else if (update.first.find("시티은행") != string::npos) {
			m_check7.EnableWindow(FALSE);

		}

		else if (update.first.find("산업은행") != string::npos) {
			m_check8.EnableWindow(FALSE);

		}

		else if (update.first.find("신한은행") != string::npos) {
			m_check9.EnableWindow(FALSE);

		}

		else if (update.first.find("우리은행") != string::npos) {
			m_check10.EnableWindow(FALSE);

		}

		else if (update.first.find("제일은행") != string::npos) {
			m_check11.EnableWindow(FALSE);

		}

		else if (update.first.find("전북은행") != string::npos) {
			m_check12.EnableWindow(FALSE);

		}

		else if (update.first.find("하나은행") != string::npos) {
			m_check13.EnableWindow(FALSE);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
