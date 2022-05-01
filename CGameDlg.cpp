// CGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "CGameDlg.h"
#include "afxdialogex.h"
#include "globol.h"
#include "CHelpDialog.h"


// CGameDlg 对话框

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	EnableAutomation();

	m_bPlaying = false;
	m_bFirstPoint = true;
	m_ptGameTop.Offset(20, 50);
	m_sizeElem.SetSize(40, 40);
	m_rtGameRect.left = m_ptGameTop.x;
	m_rtGameRect.top = m_ptGameTop.y;
	m_rtGameRect.bottom = m_ptGameTop.y + m_sizeElem.cy * CGameControl::s_nRows;
	m_rtGameRect.right = m_ptGameTop.x + m_sizeElem.cx * CGameControl::s_nCols;
	m_level = 1;
}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::InitBackground()
{
	UpdateWindow();
	CString bgpath("theme\\picture\\fruit_bg.bmp"); 
	CClientDC dc(this);
	HANDLE hBackG = ::LoadImageW(NULL, bgpath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  
	m_dcBG.CreateCompatibleDC(&dc);  
	m_dcBG.SelectObject(hBackG);

	m_dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, GAMEWND_WIDTH, GAMEWND_HEIGHT);
	m_dcMem.SelectObject(&bmpMem);

	m_dcMem.BitBlt(0, 0, GAMEWND_WIDTH, GAMEWND_HEIGHT, &m_dcBG, 0, 0, SRCCOPY);
}

void CGameDlg::UpdateWindow()
{
	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);
	this->GetClientRect(rtClient);
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();
	MoveWindow(0, 0, GAMEWND_WIDTH + nSpanWidth, GAMEWND_HEIGHT + nSpanHeight);
	CenterWindow();
}

void CGameDlg::UpdateMap()
{
	int nLeft = m_ptGameTop.x, nTop = m_ptGameTop.y; 
	int nElemW = m_sizeElem.cx, nElemH = m_sizeElem.cy;
	int nRows = CGameControl::s_nRows;
	int nCols = CGameControl::s_nCols;
	m_dcMem.BitBlt(m_rtGameRect.left, m_rtGameRect.top,
		m_rtGameRect.Width(), m_rtGameRect.Height(), &m_dcBG, m_rtGameRect.left, m_rtGameRect.top, SRCCOPY);
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			int nElemVal = m_GameC.GetElement(i, j);
			if (nElemVal == BLANK) continue;
			m_dcMem.StretchBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcMask, 0, nElemVal * nElemH, nElemW, nElemH, SRCPAINT);
			m_dcMem.StretchBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcElement, 0, nElemVal * nElemH, nElemW, nElemH, SRCAND);
		}
	}
	InvalidateRect(m_rtGameRect, FALSE);
}

void CGameDlg::DrawTipFrame(int nRow, int nCol)
{
	CClientDC dc(this);
	CBrush brush(RGB(233, 43, 43));
	CRect rtTipFrame;
	rtTipFrame.left = m_ptGameTop.x + nCol * m_sizeElem.cx;
	rtTipFrame.top = m_ptGameTop.y + nRow * m_sizeElem.cy;
	rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
	rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;
	dc.FrameRect(rtTipFrame, &brush);
}

//void CGameDlg::DrawTipLine(Vertex asvPath[4], int nVexnum)
//{
//	CClientDC dc(this);
//	CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));
//	CPen* pOldPen = dc.SelectObject(&penLine);
//
//	dc.MoveTo(m_ptGameTop.x + asvPath[0].col * m_sizeElem.cx + m_sizeElem.cx / 2, m_ptGameTop.y + asvPath[0].row * m_sizeElem.cy + m_sizeElem.cy / 2);
//
//	for (int i = 1; i < nVexnum; i++)
//	{
//		dc.LineTo(m_ptGameTop.x + asvPath[i].col * m_sizeElem.cx + m_sizeElem.cx / 2, m_ptGameTop.y + asvPath[i].row * m_sizeElem.cy + m_sizeElem.cy / 2);
//	}
//	dc.SelectObject(pOldPen);
//}

void CGameDlg::DrawTipLine(Vertex asvPath[100], int nVexnum)
{
	CClientDC dc(this);
	CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));
	CPen* pOldPen = dc.SelectObject(&penLine);

	dc.MoveTo(m_ptGameTop.x + asvPath[0].col * m_sizeElem.cx + m_sizeElem.cx / 2, m_ptGameTop.y + asvPath[0].row * m_sizeElem.cy + m_sizeElem.cy / 2);

	for (int i = 1; i < nVexnum; i++)
	{
		dc.LineTo(m_ptGameTop.x + asvPath[i].col * m_sizeElem.cx + m_sizeElem.cx / 2, m_ptGameTop.y + asvPath[i].row * m_sizeElem.cy + m_sizeElem.cy / 2);
	}
	dc.SelectObject(pOldPen);
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAME_TIME, m_GameProgress);
	DDX_Control(pDX, IDC_GAME_LEVEL, m_LevelText);
}

void CGameDlg::InitElement()
{
	CClientDC dc(this);
	HANDLE hBmp = ::LoadImageW(NULL, _T("theme\\picture\\fruit_element.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_dcElement.CreateCompatibleDC(&dc);
	m_dcElement.SelectObject(hBmp);

	HANDLE hMask = ::LoadImageW(NULL, _T("theme\\picture\\fruit_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_dcMask.CreateCompatibleDC(&dc);
	m_dcMask.SelectObject(hMask);
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_START, &CGameDlg::OnClickedBtnStart)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CGameDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_HINT, &CGameDlg::OnBnClickedBtnHint)
	ON_BN_CLICKED(IDC_BTN_REPLAY, &CGameDlg::OnBnClickedBtnReplay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TIPS, &CGameDlg::OnBnClickedBtnTips)
END_MESSAGE_MAP()


// CGameDlg 消息处理程序


BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化
	InitBackground();
	InitElement();
	CWnd* le = GetDlgItem(IDC_GAME_LEVEL);
	le->ShowWindow(SW_HIDE);
	CString title;
	if (m_GameMode == 1) {
		CWnd* tp = GetDlgItem(IDC_GAME_TIME);
		tp->ShowWindow(SW_HIDE);
		title = "卡通连连看-休闲模式";
		SetWindowText(title);
	}

	if (m_GameMode == 2) {
		le = GetDlgItem(IDC_GAME_LEVEL);
		le->ShowWindow(SW_SHOW);
		title = "卡通连连看-关卡模式";
		SetWindowText(title);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CPaintDC dc(this);
		dc.BitBlt(0, 0, GAMEWND_WIDTH, GAMEWND_HEIGHT, &m_dcMem, 0, 0, SRCCOPY);
		if (m_bPlaying) UpdateMap();
		CDialogEx::OnPaint();
	}
}

void CGameDlg::OnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_GameC.StartGame();
	UpdateMap();
	m_bPlaying = true;
	CWnd* bt = GetDlgItem(IDC_BTN_START);
	bt->EnableWindow(false);

	m_GameProgress.SetRange(0, 60 * 5);
	m_GameProgress.SetStep(-1);
	m_GameProgress.SetPos(60 * 5);
	this->SetTimer(PLAY_TIMER_ID, 1000, NULL);
	if (m_GameMode == 1) KillTimer(PLAY_TIMER_ID);
}

//void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	if (!m_bPlaying) return;
//	if (point.y < m_rtGameRect.top|| point.y > m_rtGameRect.bottom||
//		point.x < m_rtGameRect.left|| point.x > m_rtGameRect.right) {
//		return CDialogEx::OnLButtonUp(nFlags, point);
//	}
//	else {
//		int row = (point.y - m_ptGameTop.y) / m_sizeElem.cx;
//		int col = (point.x - m_ptGameTop.x) / m_sizeElem.cy;
//		if (row > CGameControl::s_nRows || col > CGameControl::s_nCols) {
//			return CDialogEx::OnLButtonUp(nFlags, point);
//		}
//		if (m_GameC.GetElement(row, col) != BLANK) {
//			DrawTipFrame(row, col);
//			if (m_bFirstPoint) {
//				m_GameC.SetFirstPoint(row, col);
//			}
//			else {
//				m_GameC.SetSecPoint(row, col);
//				Vertex avPath[4];
//				int nVexnum;
//				bool bSuc = m_GameC.Link(avPath, nVexnum);
//				if (bSuc == true) {
//					DrawTipLine(avPath, nVexnum);
//					Sleep(300);
//				}
//				int GameStatus = m_GameC.IsWin(m_GameProgress.GetPos());
//				if (GameStatus == GAME_WIN) {
//					KillTimer(PLAY_TIMER_ID);
//					if (m_GameMode == 2 && m_level <= 3) {
//						m_level++;
//						CString s; s.Format(_T("第%d关"), m_level);
//						MessageBox(s);
//						s.Format(_T("关卡：%d"), m_level);
//						m_LevelText.SetWindowTextW(s);
//						m_GameC.StartGame();
//						m_GameProgress.SetRange(0, 60 * (5-m_level));
//						m_GameProgress.SetStep(-1);
//						m_GameProgress.SetPos(60 * 60 * (5 - m_level));
//					}
//					else {
//						m_level = 1;
//						CWnd* bt = GetDlgItem(IDC_BTN_START);
//						bt->EnableWindow(true);
//						m_bPlaying = false;
//						MessageBox(_T("恭喜您取得胜利！！！"));
//					}
//				}
//				UpdateMap();
//			}
//			m_bFirstPoint = !m_bFirstPoint;
//		}
//	}
//}

void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point) //非线性
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bPlaying) return;
	if (point.y < m_rtGameRect.top || point.y > m_rtGameRect.bottom ||
		point.x < m_rtGameRect.left || point.x > m_rtGameRect.right) {
		return CDialogEx::OnLButtonUp(nFlags, point);
	}
	else {
		int row = (point.y - m_ptGameTop.y) / m_sizeElem.cx;
		int col = (point.x - m_ptGameTop.x) / m_sizeElem.cy;
		if (row > CGameControl::s_nRows || col > CGameControl::s_nCols) {
			return CDialogEx::OnLButtonUp(nFlags, point);
		}
		if (m_GameC.GetElement(row, col) != BLANK) {
			DrawTipFrame(row, col);
			if (m_bFirstPoint) {
				m_GameC.SetFirstPoint(row, col);
			}
			else {
				m_GameC.SetSecPoint(row, col);
				Vertex avPath[100];
				int nVexnum;
				bool bSuc = m_GameC.Link(avPath, nVexnum);
				if (bSuc == true) {
					DrawTipLine(avPath, nVexnum);
					Sleep(300);
				}
				int GameStatus = m_GameC.IsWin(m_GameProgress.GetPos());
				if (GameStatus == GAME_WIN) {
					KillTimer(PLAY_TIMER_ID);
					if (m_GameMode == 2 && m_level <= 3) {
						m_level++;
						CString s; s.Format(_T("第%d关"), m_level);
						MessageBox(s);
						s.Format(_T("关卡：%d"), m_level);
						m_LevelText.SetWindowTextW(s);
						m_GameC.StartGame();
						m_GameProgress.SetRange(0, 60 * (5 - m_level));
						m_GameProgress.SetStep(-1);
						m_GameProgress.SetPos(60 * 60 * (5 - m_level));
					}
					else {
						m_level = 1;
						CWnd* bt = GetDlgItem(IDC_BTN_START);
						bt->EnableWindow(true);
						m_bPlaying = false;
						MessageBox(_T("恭喜您取得胜利！！！"));
					}
				}
				UpdateMap();
			}
			m_bFirstPoint = !m_bFirstPoint;
		}
	}
}

void CGameDlg::OnBnClickedBtnPause()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bPlaying = !m_bPlaying;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_PAUSE);
	if (m_bPlaying)
		pBtn->SetWindowTextW(_T("暂停游戏"));
	else {
		MessageBox(_T("游戏暂停中。。。"));
		pBtn->SetWindowTextW(_T("继续游戏"));
	}
}

void CGameDlg::OnBnClickedBtnHint()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bPlaying)
		return;
	Vertex avPath[100];
	int nVexnum = 0;
	bool bSuc = m_GameC.Help(avPath, nVexnum);
	if (bSuc == true)
	{
		DrawTipLine(avPath, nVexnum);
		Sleep(1000);
		UpdateMap();
	}
	else
	{
		MessageBox(_T("已无法再连！"));
	}
}

void CGameDlg::OnBnClickedBtnReplay()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bPlaying)
	{
		m_GameC.Reset();
		UpdateMap();
	}
}

void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == PLAY_TIMER_ID && m_bPlaying) {
		m_GameProgress.StepIt();
	}
	if (m_GameProgress.GetPos() == 0) {
		KillTimer(PLAY_TIMER_ID);
		m_GameC.ClearMap();
		CWnd* bt = GetDlgItem(IDC_BTN_START);
		bt->EnableWindow(true);
		m_bPlaying = false;
		UpdateMap();
		m_bFirstPoint = false;
		m_level = 1;
		MessageBox(_T("再接再厉！！！"));
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CGameDlg::OnBnClickedBtnTips()
{
	// TODO: 在此添加控件通知处理程序代码
	CHelpDialog dlg;
	dlg.DoModal();
}
