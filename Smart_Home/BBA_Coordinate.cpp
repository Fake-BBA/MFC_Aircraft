#include "stdafx.h"
#include "BBA_Coordinate.h"

BBA_CoordinateLine::BBA_CoordinateLine() 
{
	pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
}

BBA_CoordinateLine::~BBA_CoordinateLine() {};

BBA_Coordinate::BBA_Coordinate() {};
BBA_Coordinate::~BBA_Coordinate() {};

int BBA_Coordinate::CreatCoordinate(CString wndTitle, CRect *rect, CWnd* pParent, DWORD id, DWORD style)
{
	unsigned long int myStyle = WS_OVERLAPPED | WS_SYSMENU | WS_HSCROLL;
	int ret = CreatWindow(wndTitle, rect, pParent, id, myStyle);

	return ret;
}

BEGIN_MESSAGE_MAP(BBA_Coordinate, BBA_CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void BBA_Coordinate::OnPaint()
{
	CPaintDC dc(this);

	POINT point[5] = { {20,20},{30,30},{40,40},{80,70},{200,200} };
	dc.PolyBezier(point, 4);

	
}



