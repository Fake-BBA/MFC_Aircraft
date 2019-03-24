#include "stdafx.h"
#include "BBA_FlightStatus.h"

BBA_FlightStatus::BBA_FlightStatus()
{

}
BBA_FlightStatus::~BBA_FlightStatus()
{

}

int BBA_FlightStatus::CreatFlightStatusWindow(CString wndTitle, CRect *rect, CWnd* pParent, DWORD id, DWORD style)
{
	unsigned long int myStyle = WS_SYSMENU;
	int ret = CreatWindow(wndTitle, rect, pParent, id, myStyle);
	return ret;
}

BEGIN_MESSAGE_MAP(BBA_FlightStatus, BBA_CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

void BBA_FlightStatus::OnSize(UINT nType, int cx, int cy)
{
	BBA_CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}
void BBA_FlightStatus::OnPaint()
{
	//获得可绘制屏幕区域大小
	this->GetClientRect(&m_RectClientWindow);

	m_pPDC = new CPaintDC(this);
	for (int i = 0; i < textLineNum; i++)
	{
		for (int j = 0; j < textListNum; j++)
		{
			m_pPDC->TextOut(textPoint[i][j].x, textPoint[i][j].y, "Hello");
		}
	}
}

int BBA_FlightStatus::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BBA_CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetClientRect(&m_RectClientWindow);

	textLineNum=3;	//每一行有3个text
	textListNum=3;	//每一列有3个text
	textLineInterval = 30;	//每一行间隔为10个像素
	textListInterval = 80;	//每一列的间隔为20个像素

	textStartPoint.x = 20;	//text矩阵输出的起点x
	textStartPoint.y = 50;	//text矩阵输出的起点y
	for (int i = 0; i < textLineNum; i++)
	{
		for (int j = 0; j < textListNum; j++)
		{
			textPoint[i][j].x = textStartPoint.x+ i * textListInterval;
			textPoint[i][j].y = textStartPoint.y+j * textLineInterval;
		}
	}

	CString temp;
	CRect rect;
	sliderLineNum = 2;	//每一行有2个slider
	sliderListNum = 10;	//每一列有10个slider
	sliderLineInterval = 50;	//每一行slider的间隔
	sliderListInterval = 300;	//每一列slider的间隔
	sliderStartPoint.x = m_RectClientWindow.right/2;	//slider矩阵的起点x
	sliderStartPoint.y = 100;	//slider矩阵的起点y

	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			sliderPoint[i][j].x = sliderStartPoint.x + i * sliderListInterval;
			sliderPoint[i][j].y = sliderStartPoint.y + j * sliderLineInterval;

			rect.left = sliderPoint[i][j].x;
			rect.top = sliderPoint[i][j].y;
			rect.bottom = rect.top + 30;	//滑条的高
			rect.right = rect.left + 200;	//滑条的长
			slider[i][j].Create(WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, rect, this, IDC_SLIDER_MSG_START + i);
		}
	}
}