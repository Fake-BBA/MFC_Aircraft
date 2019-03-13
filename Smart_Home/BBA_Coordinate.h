#pragma once
#include "BBA_CWnd.h"

class BBA_CoordinateLine{
public:
	BBA_CoordinateLine();
	~BBA_CoordinateLine();
public:
	CPen pen;
	POINT point[10000];
};

class BBA_Coordinate :public BBA_CWnd {
public:
	BBA_Coordinate();
	~BBA_Coordinate();

protected:
	DECLARE_MESSAGE_MAP();
private:
	BBA_CoordinateLine line;
public:
	afx_msg void OnPaint();
public:
	int CreatCoordinate(CString wndTitle, CRect *rect=NULL, CWnd* pParent = NULL, DWORD id = 0, DWORD style = WS_VISIBLE | WS_CHILD);
};