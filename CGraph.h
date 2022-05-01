#pragma once
#include "globol.h"

class CGraph
{
protected:
	Vertices m_Vertices;
	AdjMatrix m_AdjMartix;
	int m_nVexnum;
	int m_nArenum;
public:
	void UpdateVertex(int nIndex, int info);
	bool GetArc(int nV1Index, int nV2Index);
	int GetVexnum();
	void InitGraph();
	int AddVertex(int info);
	int GetVertex(int nIndex);
	void AddArc(int nV1Index, int nV2Index);
	void ClearGraph();
};

