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

	// TODO: �ڴ˴������Ϣ����������
}
void BBA_FlightStatus::OnPaint()
{
	//��ÿɻ�����Ļ�����С
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

	textLineNum=3;	//ÿһ����3��text
	textListNum=3;	//ÿһ����3��text
	textLineInterval = 30;	//ÿһ�м��Ϊ10������
	textListInterval = 80;	//ÿһ�еļ��Ϊ20������

	textStartPoint.x = 20;	//text������������x
	textStartPoint.y = 50;	//text������������y
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
	sliderLineNum = 2;	//ÿһ����2��slider
	sliderListNum = 10;	//ÿһ����10��slider
	sliderLineInterval = 50;	//ÿһ��slider�ļ��
	sliderListInterval = 300;	//ÿһ��slider�ļ��
	sliderStartPoint.x = m_RectClientWindow.right/2;	//slider��������x
	sliderStartPoint.y = 100;	//slider��������y

	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			sliderPoint[i][j].x = sliderStartPoint.x + i * sliderListInterval;
			sliderPoint[i][j].y = sliderStartPoint.y + j * sliderLineInterval;

			rect.left = sliderPoint[i][j].x;
			rect.top = sliderPoint[i][j].y;
			rect.bottom = rect.top + 30;	//�����ĸ�
			rect.right = rect.left + 200;	//�����ĳ�
			slider[i][j].Create(WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, rect, this, IDC_SLIDER_MSG_START + i);
		}
	}
}