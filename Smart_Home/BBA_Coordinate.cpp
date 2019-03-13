#include "stdafx.h"
#include "BBA_Coordinate.h"

BBA_Coordinate::BBA_Coordinate() {};
BBA_Coordinate::~BBA_Coordinate() {};

int BBA_Coordinate::CreatCoordinate(CString wndTitle, CRect *rect, CWnd* pParent, DWORD id, DWORD style)
{
	int ret = CreatWindow(wndTitle, rect, pParent, id, style);
	return ret;
}

BEGIN_MESSAGE_MAP(BBA_Coordinate, BBA_CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void BBA_Coordinate::OnPaint()
{
	CPaintDC dc(this); //设备环境
	dc.Ellipse(100, 100, 200, 300);
}



