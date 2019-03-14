#pragma once
#include "BBA_CWnd.h"

class BBA_CoordinateWaveform{
public:
	BBA_CoordinateWaveform();
	~BBA_CoordinateWaveform();
public:
	CPen pen;
#define MAX_POINT	300
	POINT point[MAX_POINT];
	POINT startPoint;	//起始点
	POINT endPoint;		//最新的点
	int CreatWaveform(int x,int y);
	int DrawWaveform(CDC *pDC,COLORREF colour, int nWidth);
	
};

class BBA_Coordinate :public BBA_CWnd {
public:
	BBA_Coordinate();
	~BBA_Coordinate();

#define IDC_TIMER_WAVEFORM	1329
protected:
	DECLARE_MESSAGE_MAP();
private:
	CPaintDC *m_pPDC;
	BBA_CoordinateWaveform waveform;

private:
	int DrawCoordinate(CDC *pDC);
public:
	int m_CoordinateCenter_X;	//X轴与中心偏移值，0为不偏移
	int m_CoordinateCenter_Y;	//Y轴与中心偏移值，0为不偏移
	POINT m_StartX;	//X轴起点坐标
	POINT m_EndX;	//X轴终点坐标
	POINT m_StartY;	//Y轴起点坐标
	POINT m_EndY;	//Y轴终点坐标
	int m_unitOneX;	//多少个像素代表X轴的一个刻度小格
	int m_unitOneY;	//多少个像素代表Y轴的一个刻度小格
	int m_uintLen;	//刻度的长度
	CRect m_RectClientWindow;	//可绘制的区域,由OnPaint消息实时更新

public:
	int CreatCoordinateWindow(CString wndTitle, CRect *rect = NULL, CWnd* pParent = NULL, DWORD id = 0, DWORD style = WS_VISIBLE | WS_CHILD);
	void OnTimer(UINT_PTR nIDEvent);
public:

public:
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
};