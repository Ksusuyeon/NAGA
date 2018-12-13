
// GUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GUI.h"
#include "GUIDlg.h"
#include "program.h"
#include "afxdialogex.h"
#include "prefetch.h"
#include "update.h"
#include "phverify/verify.h"
#include "Win32Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::list<pprogram> my_list;
std::list<pprogram> delete_list;
std::list<pblackp> black_list;
std::list<punknownp> unknown_list;
std::list<punknownp> del_unknown_list;


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, CGUIDlg::OnNMCustomdrawList1)
END_MESSAGE_MAP()


// CGUIDlg dialog



CGUIDlg::CGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LIST1, m_listView);
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SIDE_PIC, sideImg);
}

BEGIN_MESSAGE_MAP(CGUIDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CGUIDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_DELETE_BTN, &CGUIDlg::OnBnClickedDeleteBtn)
	ON_BN_CLICKED(IDC_SELECT_BTN, &CGUIDlg::OnBnClickedSelectBtn)
	ON_BN_CLICKED(IDC_RESET_BTN, &CGUIDlg::OnBnClickedResetBtn)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CGUIDlg::OnLvnColumnclickList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CGUIDlg::OnNMCustomdrawList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CGUIDlg::OnNMDblclkList1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CGUIDlg message handlers

BOOL CGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//
	//	list control �ʱ�ȭ
	//

	CRect rect;

	GetClientRect(&rect);
	CPoint pos;
	pos.x = GetSystemMetrics(SM_CXSCREEN) / 2.0f - rect.Width() / 2.0f;
	pos.y = GetSystemMetrics(SM_CYSCREEN) / 2.0f - rect.Height() / 2.0f;;

	SetWindowPos(NULL, pos.x, pos.y, 1200, 570, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	LPWSTR szText[COLNUM] = { L" " ,L"�̸�", L"������ ���ð�",  L"������", L"����" };
	int nWidth[COLNUM] = { 25,300,200,200,100 };

	LV_COLUMN iCol;
	iCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	iCol.fmt = LVCFMT_LEFT;

	//
	//	���̵� �̹��� ����
	//
	sideImg.SetWindowPos(NULL, 10, 10, 300, 450, NULL);
//	sideImg.SetWindowPos(NULL, 870, 10, 300, 450, NULL);
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
	sideImg.SetBitmap(hBmp);

	//
	//	LVS_EX_CHECKBOXES �� ���� üũ�ڽ��� �̿��� �� �ִ�.
	//
	m_listView.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_listView.SetWindowPos(NULL, 320, 10, 850, 450, NULL);
//	m_listView.SetWindowPos(NULL, 10, 10, 850, 450, NULL);
	
	//
	//	������ Column�� �����ϰ� ũ�⸦ ����
	//
	for (int i = 0; i < COLNUM; i++) {
		iCol.pszText = szText[i];
		iCol.iSubItem = i;
		iCol.cx = nWidth[i];
		iCol.fmt = LVCFMT_CENTER;
		m_listView.InsertColumn(i, &iCol);
	}

	//
	//	��ư���� ��ġ ���� (���� �ʿ�)
	//
	GetDlgItem(IDC_RESET_BTN)->SetWindowPos(NULL, 1110 - 65 - 65, 465, 60, 30, NULL);
	GetDlgItem(IDC_SELECT_BTN)->SetWindowPos(NULL, 1110 -65, 465, 60, 30, NULL);
	GetDlgItem(IDC_DELETE_BTN)->SetWindowPos(NULL, 1110, 465, 60, 30, NULL);

	//
	//	�α׸� ����ϱ� ���� static text �߰�
	//
//	GetDlgItem(IDC_LOG_STATIC)->SetWindowPos(NULL, 0, 500 , 1200, 20, NULL);
//	SetDlgItemTextW(IDC_LOG_STATIC, L"���α׷��� ����Ǿ����ϴ�. ");
	
	//
	//	�ϴ� â�� ����Ǹ� list control�� ������ش�
	//
	ShowWindow(SW_SHOW);
	RedrawWindow();

	//
	//	loadingâ�� ���� ������ ����
	//
	run_load_dlg(NULL);

	//
	//	data �Է�
	//
	insert_naga_data();

	return TRUE;  // return TRUE  unless you set the focus to a control
}



///	@brief  data�� �о���� ���� �ε� ���̾�α׸� ����ϱ� ���� �Լ�
///
///
UINT CGUIDlg::run_load_dlg(LPVOID _mothod) {
	// 
	// Dialog �� ������ �����忡�� ����ݴϴ�.
	//
	CLoadDlg load_dlg;
	boost::thread* dlg_thread = new boost::thread([&load_dlg]() {
		load_dlg.DoModal();
	});

	//
	// �����͸� ��������...
	//
	get_naga_data(NULL);

	//
	// �ε� ���̾�α׸� �����մϴ�. 
	//
	load_dlg.EndDialog(0);

	// 
	// ���̾�α׸� ����־��� ������ ��ü�� �������ݴϴ�. 
	// 
	dlg_thread->join();
	delete dlg_thread;

	return 1;
}

///	@brief  ���������� data�� �о���� �Լ�
///
///
UINT CGUIDlg::get_naga_data(LPVOID _mothod) {
	CGUIDlg *pDlg = (CGUIDlg*)AfxGetApp()->m_pMainWnd;

	my_list.clear();
	black_list.clear();
	unknown_list.clear();

	//
	//	program �κ� (black list �޾ƿ�)
	//
	compare_lists(&my_list);

	wstring null_string = L"";
	for (auto mine : my_list) {
		pblackp temp = new blackp(mine->id(), mine->name(), mine->vendor(), mine->version(), mine->uninstaller(), null_string.c_str(), mine->vendor());
		black_list.push_back(temp);
	}

	//
	//	update �κ� �߰�
	//
	if (!get_update_info(&black_list)) {
		log_err "get_update_info err" log_end;
	}

	//
	//	prefetch �Ľ� �κ� �߰�	(unknown list �޾ƿ�)
	//
	if (!get_prefetch_info(&unknown_list)) {
		log_err "get_prefetch_info() err" log_end;
	}

	return 1;
}

///	@brief �о�� �����͸� ����Ʈ ��Ʈ�ѿ� ����ϴ� �Լ�
///
///
void CGUIDlg::insert_naga_data(void) {

	//
	//	black list �� ȭ�鿡 ����ϴ� �κ�
	//
	for (auto black : black_list)
	{
		wstring name = black->name();
		if (name.find(L"Veraport") != std::string::npos) {
			insertData(
				L"Update", const_cast<LPWSTR>(black->name()), const_cast<LPWSTR>(black->version()), L"", const_cast<LPWSTR>(black->bank()));
		}
		else {
			insertData(
				LPWSTR(L"Security"), const_cast<LPWSTR>(black->name()), const_cast<LPWSTR>(black->version()), L"", const_cast<LPWSTR>(black->bank()));
		}
	}


	//
	//	unknown list �� ȭ�鿡 ����ϴ� �κ�
	//
	for (auto line : unknown_list) {

		log_err "--- %ws",line->id() log_end;
		//
		//	valid ���� ������� ������� ����
		//
		if (!line->isValid()) {
			continue;
		}
		wstring	lastuse = line->lastuse();

		wstringstream last_stm;
		int loc = lastuse.find(L" ");
		last_stm << lastuse.substr(0, loc);

		wstring file_name = (file_name_from_file_pathw(line->id())).c_str();
		to_lower_string(file_name);

		insertData(
			LPWSTR(L"Unknown"),
			LPWSTR(file_name.c_str()),
			(LPWSTR)(line->version()),
			(LPWSTR)(last_stm.str().c_str()),
			(LPWSTR)line->cert());
	}
}

/// @brief	����Ʈ ��Ʈ�ѿ� �����͸� �߰��ϱ� ���� �Լ�
///	�̸�, ���������ð�, ����, �������� ������ �����Ѵ�.
void CGUIDlg::insertData(LPWSTR type, LPWSTR name, LPWSTR version, LPWSTR lastuse, LPWSTR cert) {
	LV_ITEM lvitem;
	int count = m_listView.GetItemCount();

	// üũ�ڽ��� ���� ����
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = count;
	lvitem.iSubItem = 0;
	lvitem.pszText = type;
	m_listView.InsertItem(&lvitem);

	// �̸� �Է�
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = count;
	lvitem.iSubItem = 1;
	lvitem.pszText = name;
	m_listView.SetItem(&lvitem);

	//	������ ���ð� �Է�
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = count;
	lvitem.iSubItem = 2;
	lvitem.pszText = lastuse;
	m_listView.SetItem(&lvitem);

	// ���� �Է�
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = count;
	lvitem.iSubItem = 4;
	lvitem.pszText = version;
	m_listView.SetItem(&lvitem);

	//	������ ���� ����
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = count;
	lvitem.iSubItem = 3;
	lvitem.pszText = cert;
	m_listView.SetItem(&lvitem);

}


void CGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{


	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);



	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGUIDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

}

///	@brief  Delete ��ư�� ���� �� ����Ǵ� �Լ�
///
void CGUIDlg::OnBnClickedDeleteBtn()
{
	delete_list.clear();
	del_unknown_list.clear();

	bool blackFlag = false, unknownFlag = false;

	log_warn "[ ����ڰ� ������ ���α׷� ]" log_end;
	// TODO: Add your control notification handler code here

	for (int i = 0; i < m_listView.GetItemCount(); i++) {
		if (m_listView.GetCheck(i) == true) {
			CString p_kind = m_listView.GetItemText(i, 0);

			//
			//	black list���� unknown list���� �Ǻ�
			//
			if ((p_kind.Find(_T("Security")) != -1) || (p_kind.Find(_T("Update")) != -1)) {
				blackFlag = true; unknownFlag = false;
			}
			else if (p_kind.Find(_T("Unknown")) != -1) {
				blackFlag = false; unknownFlag = true;
			}

			CString delete_name = m_listView.GetItemText(i, 1);

			//
			//	black list �� ���
			//
			if (blackFlag) {
				pprogram temp = find_program(delete_name, my_list);
				log_info "black list : %ws", temp->name() log_end;
				if (temp != NULL) {
					delete_list.push_back(temp);
				}
			}

			//
			//	unknown list �� ���
			//
			else if (unknownFlag) {
				punknownp temp = find_unknown(delete_name, unknown_list);
				log_info "unknown list : %ws", temp->id() log_end;
				if (temp != NULL) {
					del_unknown_list.push_back(temp);
				}
			}

		}
	}

	// delete_list ���� uninstaller handle ����! 
	for (auto mouse : delete_list) {
		STARTUPINFO startupInfo = { 0 };
		PROCESS_INFORMATION processInfo;
		startupInfo.cb = sizeof(STARTUPINFO);
		log_info "uninstaller : %ws", mouse->uninstaller() log_end;
		::CreateProcess(NULL, (LPWSTR)(mouse->uninstaller()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
	}

	// unknown_list ���� uninstaller handle ����! 
	for (auto mouse : del_unknown_list) {
		wstring full_path = mouse->id();
		wstring dir_path = directory_from_file_pathw(full_path.c_str());
		to_lower_string(dir_path);

		list<pprogram> temp_list;
		get_all_program(&temp_list);

		for (auto installed : temp_list) {
			wstring temp_full_path = installed->uninstaller();
			wstring installed_path = directory_from_file_pathw(temp_full_path.c_str());
			to_lower_string(installed_path);
			
			//
			//	exe ������ �����ϴ� ��ο� ���α׷� �߰�/���ſ� �ִ� ���α׷��� ��θ� ���Ѵ�.
			//
			if (installed_path.find(dir_path) != wstring::npos) {
				mouse->setUninstaller(temp_full_path);
			}
		}

		STARTUPINFO startupInfo = { 0 };
		PROCESS_INFORMATION processInfo;
		startupInfo.cb = sizeof(STARTUPINFO);

		//
		//	uninstaller�� ������ ��� uninstaller�� ����
		//
		if (!::CreateProcess(NULL, (LPWSTR)(mouse->uninstaller()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo)) {
			wstring temp_path = mouse->id();
			wstring temp_dir = directory_from_file_pathw(temp_path.c_str());
			
			//
			//	uninstaller�� �������� ���� ��� �ش� ��θ� �������ش�.
			//
			ShellExecute(NULL, L"open", L"explorer.exe", temp_dir.c_str(), NULL, SW_SHOW);
		}
	}

}

///	@brief �̸����� unknown �� ã�Ƽ� ��ȯ�ϴ� �Լ�
///
punknownp CGUIDlg::find_unknown(CString program_name, std::list<punknownp> temp_list) {
	log_info "%ws", program_name log_end;
	int result;
	for (auto mine : temp_list) {
		wstring temp_id = mine->id();
		to_lower_string(temp_id);
		if (temp_id.find(program_name) != wstring::npos) {
			return mine;
		}
	}
	return NULL;
}

///	@brief �̸����� black �� ã�Ƽ� ��ȯ�ϴ� �Լ�
///
pprogram CGUIDlg::find_program(CString program_name, list<pprogram> temp_list) {
	log_info "%ws", program_name log_end;
	int result;
	for (auto mine : temp_list) {
		result = wcscmp(program_name, mine->name());
		if (result == 0) return mine;
	}
	return NULL;
}


///	@brief check box�� üũ�ϸ� ����Ǵ� �Լ�
///
void CGUIDlg::OnBnClickedSelectBtn()
{
	for (int i = 0; i < m_listView.GetItemCount(); i++)
	{
		m_listView.SetCheck(i, TRUE);
	}
}


///	@brief reset ��ư�� Ŭ���ϸ� ����Ǵ� �Լ�
///
void CGUIDlg::OnBnClickedResetBtn()
{
	// TODO: Add your control notification handler code here
	m_listView.DeleteAllItems();
	my_list.clear();
	black_list.clear();
	unknown_list.clear();

	run_load_dlg(NULL);
	insert_naga_data();

}


///	@brief ����� �� ����Ǵ� �Լ�
///			�޸� ��ȯ�� ���⼭ ����!
void CGUIDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	std::wstring user_temp_dir;
	get_temp_dirW(user_temp_dir);

	std::wstringstream create_strm;
	create_strm << user_temp_dir << L"Naga\\result";
	// TODO: Add your message handler code here
	delete_all_csv(create_strm.str().c_str(), 1);
	finalize_log();
	for (auto p : my_list)
	{
		delete p;
	}
	my_list.clear();
}

///	@brief  list control�� �����ϱ� ���� �Լ�
///
void CGUIDlg::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int count = m_listView.GetItemCount();
	int unknown_loc = -1;

	for (int i = 0; i < count; i++)
	{
		//
		//	������ ���� �о�� Unknown�� �߰ߵǸ� Stop
		//
		CString name = m_listView.GetItemText(i, 0);
		if (name.Find(_T("Unknown")) != -1) {
			log_info "loc : %d", i log_end;
			unknown_loc = i;
			break;
		}
	}

	if (unknown_loc == -1) return;

	for (int j = unknown_loc; j < count; j++) {
		m_listView.DeleteItem(unknown_loc);
	}

	
	unknown_list.sort(unknown_list_sort());
	
	//
	//	unknown list �� ȭ�鿡 ����ϴ� �κ�
	//
	for (auto line : unknown_list) {
		//
		//	valid ���� ������� ������� ����
		//
		if (!line->isValid()) {
			continue;
		}
		wstring	lastuse = line->lastuse();

		wstringstream last_stm;
		int loc = lastuse.find(L" ");
		last_stm << lastuse.substr(0, loc);

		wstring file_name = (file_name_from_file_pathw(line->id())).c_str();
		to_lower_string(file_name);

		insertData(
			LPWSTR(L"Unknown"),
			LPWSTR(file_name.c_str()),
			(LPWSTR)(line->version()),
			(LPWSTR)(last_stm.str().c_str()),
			(LPWSTR)line->cert());
	}
	
}

///	@brief  list control�� �� ���� ������ ������ ���� �Լ�
///
void CGUIDlg::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	BOOL bUpdateFlag = FALSE;
	BOOL bSecurityFlag = FALSE;
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	CString p_type = m_listView.GetItemText(pLVCD->nmcd.dwItemSpec, 0);

	*pResult = 0;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;

	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		//
		//	black list�� ���
		//	(���� ����� ������ ���� ���α׷�)
		if (p_type.Find(_T("Security")) != -1)		
		{
			pLVCD->clrText = RGB(33, 151, 216);
			pLVCD->clrTextBk = RGB(237, 255, 255);
		}

		//
		//	update list�� ���
		//	(������Ʈ�� �ʿ��� ������ ���� ���α׷�) like veraport
		else if (p_type.Find(_T("Update")) != -1)	
		{
			pLVCD->clrText = RGB(255, 107, 107);
			pLVCD->clrTextBk = RGB(253, 212, 212);

		}
		else
		{
			//
			//	unknown program�� ���
			//	��ĥ���� ����
		}

		*pResult = CDRF_DODEFAULT;
	}

}


///	@brief	list control�� �׸��� ����Ŭ���� ����Ǵ� �Լ�
///
void CGUIDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	*pResult = 0;
	int row = pNMItemActivate->iItem;
	int col = pNMItemActivate->iSubItem;
	log_info "clicked!! %d %d", row, col log_end;

	CString name = m_listView.GetItemText(row, 0);
	if (name.Find(_T("Update")) != -1) {
		CBankDlg dlg;
		dlg.DoModal();
	}
}



HBRUSH CGUIDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	if (pWnd->GetDlgCtrlID() == IDC_LOG_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
	}

	return hbr;
}
