// CHelpDialog.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "CHelpDialog.h"
#include "afxdialogex.h"


// CHelpDialog 对话框

IMPLEMENT_DYNAMIC(CHelpDialog, CDialogEx)

CHelpDialog::CHelpDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HELP_DIALOG, pParent)
{

}

CHelpDialog::~CHelpDialog()
{
}

void CHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HELP_SCROLL, m_Scrollbar);
}


BEGIN_MESSAGE_MAP(CHelpDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CHelpDialog 消息处理程序


BOOL CHelpDialog::OnInitDialog()
{
	CRect temprect(0, 0, 410, 300);

	CWnd::SetWindowPos(NULL, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	CDialogEx::OnInitDialog();
	CBitmap bmpBasicHelp;
	CClientDC dc(this);

	bmpBasicHelp.LoadBitmapW(IDB_BASIC_HELP);
	m_dcMem.CreateCompatibleDC(&dc);
	m_dcMem.SelectObject(&bmpBasicHelp);

	CBitmap bitmap;   
	m_Scrollbar.SetScrollRange(0, 900);
	m_Scrollbar.SetScrollPos(0);
	UpdateHelp(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CHelpDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

					   // 不为绘图消息调用 CDialogEx::OnPaint()c
	int pos = m_Scrollbar.GetScrollPos();
	if (pos == 0)
	{
		CRect rc;
		GetClientRect(&rc);
		CClientDC dc(this);
		dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &m_dcMem, 0, 0, SRCCOPY);
	}
}

void CHelpDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos = m_Scrollbar.GetScrollPos();
	switch (nSBCode)
	{ 
	case SB_LINEUP:
		pos -= 1;
		break;
	case SB_LINEDOWN:
		pos += 1;
		break;
	case SB_PAGEUP:
		pos -= 10;
		break;
	case SB_PAGEDOWN:
		pos += 10;
		break;  
	case SB_TOP:
		pos = 1;
		break;
	case SB_BOTTOM:
		pos = 100;
		break;
	case SB_THUMBPOSITION:
		pos = nPos;
		break;  
	default:
		return;
	}

	m_Scrollbar.SetScrollPos(pos);
	UpdateHelp(pos);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CHelpDialog::UpdateHelp(int nPos)
{
	CClientDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), NULL, 0, 0, WHITENESS);
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &m_dcMem, 0, nPos, SRCCOPY);
	InvalidateRect(rc, FALSE);
	UpdateWindow();
}