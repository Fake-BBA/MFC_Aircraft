#include "stdafx.h"
#include "BBA_Coordinate.h"

BBA_CoordinateWaveform::BBA_CoordinateWaveform() 
{
	endPoint.x = 900;
	endPoint.y = 222;

	startPoint.x = 0;
	startPoint.y = 222;

	for (int i = 0; i < MAX_POINT; i++)
	{
		point[i].x = 700;
		point[i].y = 222;
	}

}

BBA_CoordinateWaveform::~BBA_CoordinateWaveform() {};

int BBA_CoordinateWaveform::CreatWaveform(int x, int y)
{
	int i;
	x = 5;
	
	//将所有坐标的索引向前移动一个索引单位
	for (i = 0; i<MAX_POINT-2; i++)
	{
		point[i].x = point[i + 1].x - point[i].x;
		point[i].y = point[i + 1].y;
	}

	point[MAX_POINT-1].x = endPoint.x;	//刷新点位置固定
	point[MAX_POINT - 1].y = endPoint.y+ (int)y %100-50;

	

	return 0;
}

int BBA_CoordinateWaveform::DrawWaveform(CDC *pDC,COLORREF colour, int nWidth)
{
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pDC->SelectObject(pen);
	pDC->Polyline(point, MAX_POINT);	//画出波形
	pen.DeleteObject();
	return 0;
}

BBA_Coordinate::BBA_Coordinate() 
{

	m_CoordinateCenter_X = 0;
	m_CoordinateCenter_Y = 0;
	m_unitOneX = 40;	//40个像素代表X轴的一个小格
	m_unitOneY = 40;	//40个像素代表Y轴的一个小格
	m_uintLen=5;	//刻度的长度
}

BBA_Coordinate::~BBA_Coordinate() {};

int BBA_Coordinate::CreatCoordinateWindow(CString wndTitle, CRect *rect, CWnd* pParent, DWORD id, DWORD style)
{
	unsigned long int myStyle = WS_SYSMENU | WS_HSCROLL;
	int ret = CreatWindow(wndTitle, rect, pParent, id, myStyle);
	SetTimer(IDC_TIMER_WAVEFORM, 20, NULL);	//设置定时器
	return ret;
}

BEGIN_MESSAGE_MAP(BBA_Coordinate, BBA_CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER(IDC_TIMER_WAVEFORM, &BBA_Coordinate::OnTimer(UINT_PTR nIDEvent))

END_MESSAGE_MAP()

void BBA_Coordinate::OnTimer(UINT_PTR nIDEvent)
{
	//int i;
	//for ( i= MAX_POINT - 2; i >0 ; i--)
	//{
	//	;// waveform.point[i] = waveform.point[i - 1];
	//}
	waveform.CreatWaveform(40,rand());
	//KillTimer(IDC_TIMER_WAVEFORM);
	Invalidate();
}

int BBA_Coordinate::DrawCoordinate(CDC *pDC)
{
	m_StartX.x = 0;
	m_StartX.y= m_RectClientWindow.Height() / 2 + m_CoordinateCenter_X;
	m_EndX.x = m_RectClientWindow.Width();
	m_EndX.y = m_StartX.y;

	m_StartY.x = m_RectClientWindow.Width()/2+ m_CoordinateCenter_Y;
	m_StartY.y = 0;
	m_EndY.x = m_StartY.x;
	m_EndY.y = m_RectClientWindow.Height();
	//画X轴
	pDC->MoveTo(m_StartX);
	pDC->LineTo(m_EndX);

	//画Y轴
	pDC->MoveTo(m_StartY);
	pDC->LineTo(m_EndY);

	//画X轴刻度
	for (int i = 0; i < m_RectClientWindow.Width() / m_unitOneX; i++)
	{
		pDC->MoveTo(m_StartX.x+ i*m_unitOneX,m_StartX.y);
		pDC->LineTo(m_StartX.x + i * m_unitOneX, m_StartX.y- m_uintLen);
	}

	//画Y轴刻度
	for (int i = 0; i < m_RectClientWindow.Height() / m_unitOneY; i++)
	{
		pDC->MoveTo(m_StartY.x , m_StartY.y + i * m_unitOneY);
		pDC->LineTo(m_StartY.x + m_uintLen, m_StartY.y + i * m_unitOneY);
	}
	return 0;
}
void BBA_Coordinate::OnPaint()
{
	//获得可绘制屏幕区域大小
	this->GetClientRect(&m_RectClientWindow);

	m_pPDC = new CPaintDC(this);
	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象

	//随后建立与屏幕显示兼容的内存显示设备
	MemDC.CreateCompatibleDC(NULL);
	//这时还不能绘图，因为没有地方画 ^_^
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
	MemBitmap.CreateCompatibleBitmap(m_pPDC, m_RectClientWindow.Width(), m_RectClientWindow.Height());
	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	//先用背景色将位图清除干净，这里我用的是白色作为背景
	//你也可以用自己应该用的颜色
	MemDC.FillSolidRect(0, 0, m_RectClientWindow.Width(), m_RectClientWindow.Height(), RGB(255, 255, 255));
	//绘图
	DrawCoordinate(&MemDC);		//画出坐标轴
	waveform.DrawWaveform(&MemDC,RGB(255,0,0), 1);	//画出波形
	//将内存中的图拷贝到屏幕上进行显示
	m_pPDC->BitBlt(0, 0, m_RectClientWindow.Width(), m_RectClientWindow.Height(), &MemDC, 0, 0, SRCCOPY);
	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();

	//delete m_pPDC;
}


void BBA_Coordinate::OnSize(UINT nType, int cx, int cy)
{
	BBA_CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}
