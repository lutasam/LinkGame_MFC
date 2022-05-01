#pragma once
#include "CGameLogic.h"
class CGameControl
{
public:
	static int s_nRows;
	static int s_nCols;
	static int s_nPicNum;
	void StartGame();
	int GetElement(int nRow, int nCol);
	void SetFirstPoint(int nRow, int nCol);
	void SetSecPoint(int nRow, int nCol);
	//bool Link(Vertex avPath[4], int& nVexnum);
	bool Link(Vertex avPath[100], int& nVexnum); //非线性
	void ClearMap();
	int IsWin(int nTime);
protected:
	CGraph m_graph; //非线性地图
	int** m_pGameMap;
	CGameLogic m_GameLogic;
	Vertex m_svSelFst;
	Vertex m_svSelSec;
public:
	bool Help(Vertex avPath[100], int& nVexnum); //非线性
	void Reset();
};