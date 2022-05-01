#include "pch.h"
#include "CGameLogic.h"
#include "CGameControl.h"
#include <iostream>
using namespace std;

int** CGameLogic::InitMap()
{
	int nRows = CGameControl::s_nRows;
	int nCols = CGameControl::s_nCols;
	int nPicNum = CGameControl::s_nPicNum;
	int** pGameMap = new int* [nRows];
	if (pGameMap == NULL) {
		cout << "内存操作异常！\n";
		//throw new CGameException(_T("内存操作异常！\n"));
	}
	else{
		for (int i = 0; i < nRows; i++) {
			pGameMap[i] = new int[nCols];
			if (pGameMap == NULL) {
				cout << "内存操作异常！\n";
				//throw new CGameException(_T("内存操作异常！\n"));
			}
			memset(pGameMap[i], NULL, sizeof(int) * nCols);
		}
	}
	if ((nRows * nCols) % (nPicNum * 2) != 0) {
		ReleaseMap(pGameMap);
		cout << "游戏花色与游戏地图大小不匹配！\n";
		//throw new CGameException(_T("游戏花色与游戏地图大小不匹配！\n"));
	}
	int nRepeatNum = nRows * nCols / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++) {
		for (int j = 0; j < nRepeatNum; j++) {
			pGameMap[nCount / nCols][nCount % nCols] = i;
			nCount++;
		}
	}
	srand((int)time(NULL));
	int nVertexNum = nRows * nCols;
	for (int i = 0; i < nVertexNum; i++) {
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;
		int nTmp = pGameMap[nIndex1 / nCols][nIndex1 % nCols];
		pGameMap[nIndex1 / nCols][nIndex1 % nCols] = pGameMap[nIndex2 / nCols][nIndex2 % nCols];
		pGameMap[nIndex2 / nCols][nIndex2 % nCols] = nTmp;
	}
	return pGameMap;
}

void CGameLogic::InitMap(CGraph& g) //非线性
{
	int nRows = CGameControl::s_nRows;
	int nCols = CGameControl::s_nCols;
	int nPicNum = CGameControl::s_nPicNum;
	int* pGameMap = new int [nRows * nCols];
	if ((nRows * nCols) % (nPicNum * 2) != 0) {
		delete[] pGameMap;
		cout << "游戏花色与游戏地图大小不匹配！\n";
		//throw new CGameException(_T("游戏花色与游戏地图大小不匹配！\n"));
	}
	int nRepeatNum = nRows * nCols / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++) {
		for (int j = 0; j < nRepeatNum; j++) {
			pGameMap[nCount++] = i;
		}
	}
	srand((int)time(NULL));
	int nVertexNum = nRows * nCols;
	for (int i = 0; i < nVertexNum; i++) {
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;
		int nTmp = pGameMap[nIndex1];
		pGameMap[nIndex1] = pGameMap[nIndex2];
		pGameMap[nIndex2] = nTmp;
	}

	for (int i = 0; i < nVertexNum; i++) {
		g.AddVertex(pGameMap[i]);
	}

	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			UpdateArc(g, i, j);
		}
	}
}

void CGameLogic::UpdateArc(CGraph& g, int nRow, int nCol)
{
	int nRows = CGameControl::s_nRows;
	int nCols = CGameControl::s_nCols;
	int nV1Index = nRow * nCols + nCol;
	if (nCol > 0) {
		int nV2Index = nV1Index - 1;
		if (g.GetVertex(nV1Index) == g.GetVertex(nV2Index) || g.GetVertex(nV2Index) == BLANK) {
			g.AddArc(nV1Index, nV2Index);
		}
	}
	if (nCol < nCols - 1) {
		int nV2Index = nV1Index + 1;
		if (g.GetVertex(nV1Index) == g.GetVertex(nV2Index) || g.GetVertex(nV2Index) == BLANK) {
			g.AddArc(nV1Index, nV2Index);
		}
	}
	if (nRow > 0) {
		int nV2Index = nV1Index - nCols;
		if (g.GetVertex(nV1Index) == g.GetVertex(nV2Index) || g.GetVertex(nV2Index) == BLANK) {
			g.AddArc(nV1Index, nV2Index);
		}
	}
	if(nRow < nRows - 1) {
		int nV2Index = nV1Index + nCols;
		if (g.GetVertex(nV1Index) == g.GetVertex(nV2Index) || g.GetVertex(nV2Index) == BLANK) {
			g.AddArc(nV1Index, nV2Index);
		}
	}
}

void CGameLogic::Clear(CGraph& g, Vertex V1, Vertex V2) //非线性
{
	int nRows = CGameControl::s_nRows, nCols = CGameControl::s_nCols;
	int nV1Index = V1.row * nCols + V1.col;
	int nV2Index = V2.row * nCols + V2.col;
	g.UpdateVertex(nV1Index, BLANK);
	g.UpdateVertex(nV2Index, BLANK);
	UpdateArc(g, V1.row, V1.col);
	UpdateArc(g, V2.row, V2.col);
}

bool CGameLogic::IsLink(CGraph& g, Vertex V1, Vertex V2)
{
	int nRows = CGameControl::s_nRows; int nCols = CGameControl::s_nCols;
	int nV1Index = V1.row * nCols + V1.col;
	int nV2Index = V2.row * nCols + V2.col;
	ClearStack();
	PushVertex(nV1Index);
	if (Searchpath(g, nV1Index, nV2Index) == true) {
		return true;
	}
	PopVertex();
	return false;
}

void CGameLogic::ReleaseMap(int**& pGameMap)
{
	for (int i = 0; i < CGameControl::s_nRows; i++) {
		delete []pGameMap[i];
	}
	delete[]pGameMap;
}

bool CGameLogic::IsLink(int** pGameMap, Vertex v1, Vertex v2)
{
	ClearStack();
	PushVertex(v1);
	bool flag1 = false, flag2 = false;
	if (v1.row == v2.row) {
		if (LinkInRow(pGameMap, v1, v2)) {
			PushVertex(v2);
			flag1 = true;
		}
		else {
			flag1 = false;
		}
	}
	if (v1.col == v2.col) {
		if (LinkInCol(pGameMap, v1, v2)) {
			PushVertex(v2);
			flag1 = true;
		}
		else {
			flag1 = false;
		}
	}
	if ((flag1 == false) && (OneCornerLink(pGameMap, v1, v2) || TwoCornerLink(pGameMap, v1, v2))) {
		PushVertex(v2);
		flag2 = true;
	}
	else {
		flag2 = false;
	}
	return flag1 || flag2;
}

void CGameLogic::Clear(int** pGameMap, Vertex v1, Vertex v2)
{
	pGameMap[v1.row][v1.col] = BLANK;
	pGameMap[v2.row][v2.col] = BLANK;
}

//int CGameLogic::GetVexPath(Vertex avPath[4]) 
//{
//	for (int i = 0; i < m_nVexNum; i++) {
//		avPath[i] = m_avPath[i];
//	}
//	return m_nVexNum;
//}

int CGameLogic::GetVexPath(Vertex avPath[100])
{
	for (int i = 0; i < m_nVexNum; i++) {
		int col = m_anPath[i] % CGameControl::s_nCols;
		int row = m_anPath[i] / CGameControl::s_nCols;
		avPath[i] = { row, col, BLANK };
	}
	return m_nVexNum;
}

bool CGameLogic::LinkInRow(int** pGameMap, Vertex v1, Vertex v2)
{
	int nCol1 = v1.col;
	int nCol2 = v2.col;
	int nRow = v1.row;
	if (nCol1 > nCol2) {
		int nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;
	}

	for (int i = nCol1 + 1; i <= nCol2; i++) {
		if (i == nCol2) {
			return true;
		}
		if (pGameMap[nRow][i] != BLANK) break;
	}
	return false;
}

bool CGameLogic::LinkInCol(int** pGameMap, Vertex v1, Vertex v2)
{
	int nRow1 = v1.row;
	int nRow2 = v2.row;
	int nCol = v2.col;
	if (nRow1 > nRow2) {
		int nTemp = nRow1;
		nRow1 = nRow2;
		nRow2 = nTemp;
	}

	for (int i = nRow1 + 1; i <= nRow2; i++) {
		if (i == nRow2) {
			return true;
		}
		if (pGameMap[i][nCol] != BLANK) break;
	}
	return false;
}

bool CGameLogic::OneCornerLink(int** pGameMap, Vertex v1, Vertex v2)
{
	if (pGameMap[v1.row][v2.col] == BLANK) {
		if (LineY(pGameMap, v1.row, v2.row, v2.col) && LineX(pGameMap, v1.row, v1.col, v2.col)) {
			Vertex v = { v1.row, v2.col, BLANK };
			PushVertex(v);
			return true;
		}
	}

	if (pGameMap[v2.row][v1.col] == BLANK) {
		if (LineY(pGameMap, v1.row, v2.row, v1.col) && LineX(pGameMap, v2.row, v1.col, v2.col)) {
			Vertex v = { v2.row, v1.col, BLANK };
			PushVertex(v);
			return true;
		}
	}
	return false;
}

bool CGameLogic::LineY(int** pGameMap, int nRow1, int nRow2, int nCol)
{
	Vertex v1 = { nRow1, nCol, pGameMap[nRow1][nCol] };
	Vertex v2 = { nRow2, nCol, pGameMap[nRow2][nCol] };
	return LinkInCol(pGameMap, v1, v2);
}

bool CGameLogic::LineX(int** pGameMap, int nRow, int nCol1, int nCol2)
{
	Vertex v1 = { nRow, nCol1, pGameMap[nRow][nCol1] };
	Vertex v2 = { nRow, nCol2, pGameMap[nRow][nCol2] };
	return LinkInRow(pGameMap, v1, v2);
}

void CGameLogic::PushVertex(Vertex v)
{
	if (m_nVexNum < 4) m_avPath[m_nVexNum++] = v;
}

void CGameLogic::PushVertex(int nV) //非线性
{
	m_anPath[m_nVexNum++] = nV;
	if (isCornor()) m_nCorner++;
}

bool CGameLogic::Searchpath(CGraph& g, int nV0, int nV1)
{
	//int nVexnum = g.GetVexnum();

	int nCol = nV0 % CGameControl::s_nCols;
	int nRow = nV0 / CGameControl::s_nCols;
	if (nCol > 0) {
		int nV = nV0 - 1;
		if (g.GetArc(nV0, nV1) && !isExist(nV)) {
			PushVertex(nV);
			if (m_nCorner <= 2) {
				if (nV != nV1) {
					if (g.GetVertex(nV) == BLANK) {
						if (Searchpath(g, nV, nV1)) return true;
					}
				}
				else {
					return true;
				}
			}
			PopVertex();
		}
	}
	if (nCol < CGameControl::s_nCols - 1) {
		int nV = nV0 + 1;
		if (g.GetArc(nV0, nV1) && !isExist(nV)) {
			PushVertex(nV);
			if (m_nCorner <= 2) {
				if (nV != nV1) {
					if (g.GetVertex(nV) == BLANK) {
						if (Searchpath(g, nV, nV1)) return true;
					}
				}
				else {
					return true;
				}
			}
			PopVertex();
		}
	}
	if (nRow > 0) {
		int nV = nV0 - CGameControl::s_nCols;
		if (g.GetArc(nV0, nV1) && !isExist(nV)) {
			PushVertex(nV);
			if (m_nCorner <= 2) {
				if (nV != nV1) {
					if (g.GetVertex(nV) == BLANK) {
						if (Searchpath(g, nV, nV1)) return true;
					}
				}
				else {
					return true;
				}
			}
			PopVertex();
		}
	}
	if (nRow < CGameControl::s_nRows - 1) {
		int nV = nV0 + CGameControl::s_nCols;
		if (g.GetArc(nV0, nV1) && !isExist(nV)) {
			PushVertex(nV);
			if (m_nCorner <= 2) {
				if (nV != nV1) {
					if (g.GetVertex(nV) == BLANK) {
						if (Searchpath(g, nV, nV1)) return true;
					}
				}
				else {
					return true;
				}
			}
			PopVertex();
		}
	}
	return false;
}

bool CGameLogic::isExist(int nV)
{
	for (int i = 0; i < m_nVexNum; i++) {
		if (m_anPath[i] == nV) return true;
	}
	return false;
}

bool CGameLogic::isCornor()
{
	if (m_nVexNum > 2) {
		int col1 = m_anPath[m_nVexNum - 3] % CGameControl::s_nCols;
		int row1 = m_anPath[m_nVexNum - 3] / CGameControl::s_nCols;
		int col2 = m_anPath[m_nVexNum - 2] % CGameControl::s_nCols;
		int row2 = m_anPath[m_nVexNum - 2] / CGameControl::s_nCols;
		int col3 = m_anPath[m_nVexNum - 1] % CGameControl::s_nCols;
		int row3 = m_anPath[m_nVexNum - 1] / CGameControl::s_nCols;
		int dx = row1 - row2;
		int dy = col1 - col2;
		if (dx == 0) {
			if (row3 != row1) return true;
			else return false;
		}
		else{
			if (col3 != col1) return true;
			else return false;
		}
	}
	return false;
}

//bool CGameLogic::IsBlank(int** pGameMap)
//{
//	for (int i = 0; i < CGameControl::s_nRows; i++) {
//		for (int j = 0; j < CGameControl::s_nCols; j++) {
//			if (pGameMap[i][j] != BLANK) {
//				return false;
//			}
//		}
//	}
//	return false;
//}

bool CGameLogic::IsBlank(CGraph& g)
{
	for (int i = 0; i < CGameControl::s_nCols * CGameControl::s_nRows; i++) {
		if (g.GetVertex(i) != BLANK) return false;
	}
	return true;
}

//Vertex CGameLogic::PopVertex()
//{
//	return m_avPath[m_nVexNum--];
//}

void CGameLogic::PopVertex() //非线性
{
	if (isCornor()) m_nCorner--;
	m_anPath[m_nVexNum--];
}

//void CGameLogic::ClearStack()
//{
//	m_nVexNum = 0;
//}

void CGameLogic::ClearStack() //非线性
{
	m_nVexNum = 0;
	m_nCorner = 0;
}

bool CGameLogic::TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2)
{
	for (int nCol = 0; nCol < CGameControl::s_nCols; nCol++) {
		if (pGameMap[v1.row][nCol] == BLANK && pGameMap[v2.row][nCol] == BLANK) {
			if (LineY(pGameMap, v1.row, v2.row, nCol)) {
				if (LineX(pGameMap, v1.row, v1.col, nCol) && LineX(pGameMap, v2.row, v2.col, nCol)) {
					Vertex vx1 = { v1.row, nCol, BLANK };
					Vertex vx2 = { v2.row, nCol, BLANK };
					PushVertex(vx1);
					PushVertex(vx2);
					return true;
				}
			}
		}
	}

	for (int nRow = 0; nRow < CGameControl::s_nRows; nRow++) {
		if (pGameMap[nRow][v1.col] == BLANK && pGameMap[nRow][v2.col] == BLANK) {
			if (LineX(pGameMap, nRow, v1.col, v2.col)) {
				if (LineY(pGameMap, nRow, v1.row, v1.col) && LineY(pGameMap, nRow, v2.row, v2.col)) {
					Vertex vx1 = { nRow, v1.col, BLANK };
					Vertex vx2 = { nRow, v2.col, BLANK };
					PushVertex(vx1);
					PushVertex(vx2);
					return true;
				}
			}
		}
	}
	return false;
}

//void CGameLogic::ResetMap(int** pGameMap)
//{
//	// TODO: 在此处添加实现代码.
//	int nRows = CGameControl::s_nRows;
//	int nCols = CGameControl::s_nCols;
//	srand((int)time(NULL));
//	int nVertexNum = nRows * nCols;
//	for (int i = 0; i < 100; i++)
//	{
//		int nIndex1 = rand() % nVertexNum;
//		int nIndex2 = rand() % nVertexNum;
//		int nTmp = pGameMap[nIndex1 / nCols][nIndex1 % nCols];
//		pGameMap[nIndex1 / nCols][nIndex1 % nCols] = pGameMap[nIndex2 / nCols][nIndex2 % nCols];
//		pGameMap[nIndex2 / nCols][nIndex2 % nCols] = nTmp;
//	}
//}

void CGameLogic::ResetMap(CGraph &g)
{
	// TODO: 在此处添加实现代码.
	int nRows = CGameControl::s_nRows;
	int nCols = CGameControl::s_nCols;
	srand((int)time(NULL));
	int nVertexNum = g.GetVexnum();
	for (int i = 0; i < nVertexNum; i++)
	{
		if (g.GetVertex(i) == BLANK) continue;
		int nIndex1; while (g.GetVertex((nIndex1 = rand() % nVertexNum)) == BLANK);
		int nIndex2; while (g.GetVertex((nIndex2 = rand() % nVertexNum)) == BLANK);
		int nTmp = g.GetVertex(nIndex1);
		g.UpdateVertex(nIndex1, g.GetVertex(nIndex2));
		g.UpdateVertex(nIndex2, nTmp);
	}
}
