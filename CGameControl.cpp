#include "pch.h"
#include "CGameControl.h"

int CGameControl::s_nRows = 10;
int CGameControl::s_nCols = 16;
int CGameControl::s_nPicNum = 10;

//void CGameControl::StartGame()
//{
//	m_pGameMap = m_GameLogic.InitMap();
//}

void CGameControl::StartGame() //非线性
{
	m_graph.InitGraph();
	m_GameLogic.InitMap(m_graph);
}

//int CGameControl::GetElement(int nRow, int nCol)
//{
//	return m_pGameMap[nRow][nCol];
//}

int CGameControl::GetElement(int nRow, int nCol) //非线性
{
	return m_graph.GetVertex(nRow * CGameControl::s_nCols + nCol);
}

void CGameControl::SetFirstPoint(int nRow, int nCol)
{
	m_svSelFst.col = nCol;
	m_svSelFst.row = nRow;
}

void CGameControl::SetSecPoint(int nRow, int nCol)
{
	m_svSelSec.col = nCol;
	m_svSelSec.row = nRow;
}

//bool CGameControl::Link(Vertex avPath[4], int& nVexnum)
//{
//	if (m_svSelFst.row == m_svSelSec.row && m_svSelFst.col == m_svSelSec.col) {
//		return false;
//	}
//	if (m_pGameMap[m_svSelFst.row][m_svSelFst.col] != m_pGameMap[m_svSelSec.row][m_svSelSec.col]) {
//		return false;
//	}
//	if (m_GameLogic.IsLink(m_pGameMap, m_svSelFst, m_svSelSec)) {
//		m_GameLogic.Clear(m_pGameMap, m_svSelFst, m_svSelSec);
//		nVexnum = m_GameLogic.GetVexPath(avPath);
//		return true;
//	}
//	return false;
//}

bool CGameControl::Link(Vertex avPath[100], int& nVexnum)
{
	int nV1Index = m_svSelFst.row * CGameControl::s_nCols + m_svSelFst.col;
	int nV2Index = m_svSelSec.row * CGameControl::s_nCols + m_svSelSec.col;
	if (m_svSelFst.row == m_svSelSec.row && m_svSelFst.col == m_svSelSec.col) {
		return false;
	}
	if (m_graph.GetVertex(nV1Index) != m_graph.GetVertex(nV2Index)) {
		return false;
	}
	if (m_GameLogic.IsLink(m_graph, m_svSelFst, m_svSelSec)) {
		m_GameLogic.Clear(m_graph, m_svSelFst, m_svSelSec);
		nVexnum = m_GameLogic.GetVexPath(avPath);
		return true;
	}
	return false;
}

void CGameControl::ClearMap()
{
	for (int i = 0; i < s_nRows; i++) {
		for (int j = 0; j < s_nCols; j++) {
			m_pGameMap[i][j] = BLANK;
		}
	}
}

//int CGameControl::IsWin(int nTime)
//{
//	if (nTime <= 0) return GAME_LOSE;
//	if (m_GameLogic.IsBlank(m_pGameMap)) {
//		return GAME_WIN;
//	}
//	return GAME_PLAYING;
//}

int CGameControl::IsWin(int nTime) //非线性
{
	if (nTime <= 0) return GAME_LOSE;
	if (m_GameLogic.IsBlank(m_graph)) {
		return GAME_WIN;
	}
	return GAME_PLAYING;
}

//bool CGameControl::Help(Vertex avPath[4], int& nVexnum)
//{
//	// TODO: 在此处添加实现代码.
//	for (int x1 = 0; x1 < s_nRows; x1++) {
//		for (int y1 = 0; y1 < s_nCols; y1++)
//		{
//			if (m_pGameMap[x1][y1] != BLANK)
//			{
//				SetFirstPoint(x1, y1);
//				for (int x2 = x1; x2 < s_nRows; x2++) {
//					for (int y2 = y1 + 1; y2 < s_nCols; y2++)
//					{
//						if (m_pGameMap[x2][y2] == m_pGameMap[x1][y1])
//						{
//							SetSecPoint(x2, y2);
//							if (m_GameLogic.IsLink(m_pGameMap, m_svSelFst, m_svSelSec))
//							{
//								nVexnum = m_GameLogic.GetVexPath(avPath);
//								return true;
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//	return false;
//}

bool CGameControl::Help(Vertex avPath[100], int& nVexnum)
{
	// TODO: 在此处添加实现代码.
	for (int x1 = 0; x1 < s_nRows; x1++) {
		for (int y1 = 0; y1 < s_nCols; y1++)
		{
			int nV1Index = s_nCols * x1 + y1;
			if (m_graph.GetVertex(nV1Index) != BLANK)
			{
				SetFirstPoint(x1, y1);
				for (int x2 = x1; x2 < s_nRows; x2++) {
					for (int y2 = y1 + 1; y2 < s_nCols; y2++)
					{
						int nV2Index = s_nCols * x2 + y2;
						if (m_graph.GetVertex(nV1Index) == m_graph.GetVertex(nV2Index))
						{
							SetSecPoint(x2, y2);
							if (m_GameLogic.IsLink(m_graph, m_svSelFst, m_svSelSec))
							{
								nVexnum = m_GameLogic.GetVexPath(avPath);
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

//void CGameControl::Reset()
//{
//	// TODO: 在此处添加实现代码.
//	m_GameLogic.ResetMap(m_pGameMap);
//}

void CGameControl::Reset() //非线性
{
	// TODO: 在此处添加实现代码.
	m_GameLogic.ResetMap(m_graph);
}