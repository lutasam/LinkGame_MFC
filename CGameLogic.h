#pragma once
#include "globol.h"
#include "CGraph.h"
class CGameLogic
{
private:
	Vertex m_avPath[4];
	int m_anPath[100]; //非线性
	int m_nCorner; //非线性保存路径的拐点数
	int m_nVexNum;
public:
	int** InitMap();
	//void ResetMap(int** pGameMap);
	void ReleaseMap(int** &pGameMap);
	bool IsLink(int** pGameMap, Vertex v1, Vertex v2);
	void Clear(int** pGameMap, Vertex v1, Vertex v2);
	//int GetVexPath(Vertex avPath[4]);
	int GetVexPath(Vertex avPath[100]);
	//bool IsBlank(int** pGameMap);
protected:
	bool LinkInRow(int** pGameMap, Vertex v1, Vertex v2);
	bool LinkInCol(int** pGameMap, Vertex v1, Vertex v2);
	bool OneCornerLink(int** pGameMap, Vertex v1, Vertex v2);
	bool LineY(int** pGameMap, int nRow1, int nRow2, int nCol);
	bool LineX(int** pGameMap, int nRow, int nCol1, int nCol2);
	void PushVertex(Vertex v);
	//Vertex PopVertex();
	void PopVertex();
	void ClearStack();
	bool TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2);
public:
	void InitMap(CGraph& g); //非线性生成地图
	void UpdateArc(CGraph& g, int nRow, int nCol); //非线性判断有边
	void Clear(CGraph& g, Vertex V1, Vertex V2); //非线性清除顶点
	bool IsLink(CGraph& g, Vertex V1, Vertex V2); //非线性判断连通
	void PushVertex(int nV); //非线性
	bool Searchpath(CGraph& g, int nV0, int nV1); //非线性搜寻路径
	bool isExist(int nV); //非线性
	bool isCornor();
	bool IsBlank(CGraph &g); 
	void ResetMap(CGraph& g);
};

