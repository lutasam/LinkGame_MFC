#pragma once
#include "CGameControl.h"

// CGameDlg 对话框

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameDlg();
	void InitBackground();
	void UpdateWindow();
	void UpdateMap();
	void DrawTipFrame(int nRow, int nCol);
	//void DrawTipLine(Vertex asvPath[4], int nVexnum);
	void DrawTipLine(Vertex asvPath[100], int nVexnum);//非线性连接线
	bool m_bFirstPoint;
	bool m_bPlaying;
	bool m_bPause;
	int m_GameMode = 0;
	int m_level = 1;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitElement();
	CDC m_dcMem;
	CDC m_dcBG;
	CDC m_dcElement;
	CDC m_dcMask;
	CPoint m_ptGameTop;
	CSize m_sizeElem;
	CRect m_rtGameRect;
	CGameControl m_GameC;
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClickedBtnStart();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnHint();
	afx_msg void OnBnClickedBtnReplay();
	CProgressCtrl m_GameProgress;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	CStatic m_LevelText;
public:
	afx_msg void OnBnClickedBtnTips();
};
