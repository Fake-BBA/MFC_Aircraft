#pragma once
#include "BBA_CWnd.h"

class BBA_Coordinate :public BBA_CWnd {
public:
	BBA_Coordinate();
	~BBA_Coordinate();

protected:
	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnPaint();
public:
	int CreatCoordinate(CString wndTitle, CRect *rect=NULL, CWnd* pParent = NULL, DWORD id = 0, DWORD style = WS_VISIBLE | WS_CHILD);
};