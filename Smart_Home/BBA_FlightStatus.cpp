#include "stdafx.h"
#include "BBA_FlightStatus.h"
#include "Resource.h"
#include "smartHomeTypedef.h"

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
	ON_WM_HSCROLL()	//滑条消息
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
	//输出飞行器姿态信息
	for (int i = 0; i < textLineNum; i++)
	{
		for (int j = 0; j < textListNum; j++)
		{
			m_pPDC->TextOut(textPoint[i][j].x, textPoint[i][j].y, textStr[i][j]);
		}
	}
	//绘制滑动条名称
	CPoint point;
	CString temp;
	
	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			temp.LoadStringA(IDS_STRING_STATIC_PITCH+i*sliderListNum+j);
			point = sliderPoint[i][j];
			point.y = point.y + sliderWidth / 2 - 8;	//输出在中间
			point.x = point.x - 50;	//输出起点在滑条前50个像素点
			m_pPDC->TextOut(point.x, point.y, temp);
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
	textListInterval = 150;	//每一列的间隔为20个像素

	textStartPoint.x = 20;	//text矩阵输出的起点x
	textStartPoint.y = 50;	//text矩阵输出的起点y
	for (int i = 0; i < textLineNum; i++)
	{
		for (int j = 0; j < textListNum; j++)
		{
			textPoint[i][j].x = textStartPoint.x+ i * textListInterval;
			textPoint[i][j].y = textStartPoint.y+j * textLineInterval;
			memcpy(textStr[i][j], "Hello", 6);
		}
	}

	CString temp;
	CRect rect;
	sliderLineNum = 1;	//每一行有1个slider
	sliderListNum = 12;	//每一列有10个slider
	sliderLineInterval = 50;	//每一行slider的间隔
	sliderListInterval = 300;	//每一列slider的间隔
	sliderStartPoint.x = m_RectClientWindow.right-300;	//slider矩阵的起点x
	sliderStartPoint.y = 20;	//slider矩阵的起点y
	sliderLength = 250;		//滑条长
	sliderWidth = 30;		//滑条高
	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			sliderPoint[i][j].x = sliderStartPoint.x + i * sliderListInterval;
			sliderPoint[i][j].y = sliderStartPoint.y + j * sliderLineInterval;

			rect.left = sliderPoint[i][j].x;
			rect.top = sliderPoint[i][j].y;
			
			rect.bottom = rect.top + sliderWidth;	//滑条的高
			rect.right = rect.left + sliderLength;	//滑条的长
			slider[i][j].Create(WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, rect, this, IDC_SLIDER_MSG_START + i);
		}
	}
	slider[0][0].SetRange(-180, 180, TRUE);
	slider[0][1].SetRange(-180, 180, TRUE);
	slider[0][2].SetRange(-180, 180, TRUE);
	
	return 0;
}

void BBA_FlightStatus::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	BBA_CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	pScrollBar->GetDlgCtrlID();

	char outPutPos[10];	//存储数字串
	char SendBuff[50];	
	int sendPos=0;	//sendBuff的索引
	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			if(&slider[i][j]==(CSliderCtrl*)pScrollBar)
			{ 
				int pos = slider[i][j].GetPos();
				CPoint point = sliderPoint[i][j];
				point.y = point.y + sliderWidth / 2-8;	//输出在中间
				point.x = point.x + sliderLength;	//字长
				itoa(pos, outPutPos, 10);	//十进制
				memset(outPutPos + strlen(outPutPos), ' ', 10 - strlen(outPutPos));	//将0后面的填空格
				outPutPos[9] = 0;	//主动加结束符
				m_pPDC->TextOut(point.x,point.y, TEXT(outPutPos));
			}
		}
	}
	bba_Station_udp->Send(SendBuff,sendPos);
}

int BBA_FlightStatus::UpdataFlightStatusWindows(char *buff, int len)
{
	float pitch =(int16(buff[4] << 8)  + uint8(buff[5])) / 100;
	float roll= (int16(buff[6] << 8)  + uint8(buff[7])) / 100;
	float yaw= (int16(buff[8] << 8) + uint8(buff[9]) ) / 100;

	int acc_x = (int16(buff[4] << 8) + uint8(buff[5]));
	int acc_y= (int16(buff[4] << 8) + uint8(buff[5]));
	int acc_z= (int16(buff[4] << 8) + uint8(buff[5]));

	int gyro_x= (int16(buff[4] << 8) + uint8(buff[5]));
	int gyro_y= (int16(buff[4] << 8) + uint8(buff[5]));
	int gyro_z= (int16(buff[4] << 8) + uint8(buff[5]));
	
	sprintf(textStr[0][0], "Pitch:%f        ", pitch);
	sprintf(textStr[0][1], "Roll:%f        ", roll);
	sprintf(textStr[0][2], "Yaw:%f        ", yaw);

	sprintf(textStr[1][0], "ACC_X:%d        ", acc_x);
	sprintf(textStr[1][1], "ACC_Y:%d        ", acc_y);
	sprintf(textStr[1][2], "ACC_Z:%d        ", acc_z);

	sprintf(textStr[2][0], "GYRO_X:%d        ", gyro_x);
	sprintf(textStr[2][1], "GYRO_Y:%d        ", gyro_y);
	sprintf(textStr[2][2], "GYRO_Z:%d        ", gyro_z);

	//return 1;
	//Invalidate();	//发出重绘消息OnPaint
	for (int i = 0; i < textLineNum; i++)
	{
		for (int j = 0; j < textListNum; j++)
		{
			m_pPDC->TextOut(textPoint[i][j].x, textPoint[i][j].y, textStr[i][j]);
		}
	}
	return 0;
}
