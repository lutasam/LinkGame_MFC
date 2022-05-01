#include "pch.h"
#include "CGraph.h"
#include "CGameControl.h"

void CGraph::UpdateVertex(int nIndex, int info)
{
    m_Vertices[nIndex] = info;
}

bool CGraph::GetArc(int nV1Index, int nV2Index)
{
    return m_AdjMartix[nV1Index][nV2Index];
}

int CGraph::GetVexnum()
{
    return m_nVexnum;
}

void CGraph::InitGraph()
{
    int nRows = CGameControl::s_nRows;
    int nCols = CGameControl::s_nCols;
    m_Vertices = new int[nRows * nCols];
    m_AdjMartix = new int* [nRows * nCols];
    for (int i = 0; i < nRows * nCols; i++) {
        m_AdjMartix[i] = new int[nRows * nCols];
        memset(m_AdjMartix[i], 0, sizeof(m_AdjMartix[i]));
    }
    memset(m_Vertices, 0, sizeof(m_Vertices));
    m_nArenum = 0;
    m_nVexnum = 0;
}

int CGraph::AddVertex(int info)
{
    this->m_Vertices[m_nVexnum++] = info;
    return m_nVexnum;
}

int CGraph::GetVertex(int nIndex)
{
    return m_Vertices[nIndex];
}

void CGraph::AddArc(int nV1Index, int nV2Index)
{
    m_AdjMartix[nV1Index][nV2Index] = true;
    m_AdjMartix[nV2Index][nV1Index] = true;
    m_nArenum++;
}

void CGraph::ClearGraph()
{
    delete[] m_Vertices;
    for (int i = 0; i < CGameControl::s_nRows; i++) {
        delete[] m_AdjMartix[i];
    }
    delete[] m_AdjMartix;
}
