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
	ON_WM_HSCROLL()	//������Ϣ
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
	//�����������̬��Ϣ
	for (int i = 0; i < textLineNum; i++)
	{
		for (int j = 0; j < textListNum; j++)
		{
			m_pPDC->TextOut(textPoint[i][j].x, textPoint[i][j].y, textStr[i][j]);
		}
	}
	//���ƻ���������
	CPoint point;
	CString temp;
	
	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			temp.LoadStringA(IDS_STRING_STATIC_PITCH+i*sliderListNum+j);
			point = sliderPoint[i][j];
			point.y = point.y + sliderWidth / 2 - 8;	//������м�
			point.x = point.x - 50;	//�������ڻ���ǰ50�����ص�
			m_pPDC->TextOut(point.x, point.y, temp);
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
	textListInterval = 150;	//ÿһ�еļ��Ϊ20������

	textStartPoint.x = 20;	//text������������x
	textStartPoint.y = 50;	//text������������y
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
	sliderLineNum = 1;	//ÿһ����1��slider
	sliderListNum = 12;	//ÿһ����10��slider
	sliderLineInterval = 50;	//ÿһ��slider�ļ��
	sliderListInterval = 300;	//ÿһ��slider�ļ��
	sliderStartPoint.x = m_RectClientWindow.right-300;	//slider��������x
	sliderStartPoint.y = 20;	//slider��������y
	sliderLength = 250;		//������
	sliderWidth = 30;		//������
	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			sliderPoint[i][j].x = sliderStartPoint.x + i * sliderListInterval;
			sliderPoint[i][j].y = sliderStartPoint.y + j * sliderLineInterval;

			rect.left = sliderPoint[i][j].x;
			rect.top = sliderPoint[i][j].y;
			
			rect.bottom = rect.top + sliderWidth;	//�����ĸ�
			rect.right = rect.left + sliderLength;	//�����ĳ�
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	BBA_CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	pScrollBar->GetDlgCtrlID();

	char outPutPos[10];	//�洢���ִ�
	char SendBuff[50];	
	int sendPos=0;	//sendBuff������
	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			if(&slider[i][j]==(CSliderCtrl*)pScrollBar)
			{ 
				int pos = slider[i][j].GetPos();
				CPoint point = sliderPoint[i][j];
				point.y = point.y + sliderWidth / 2-8;	//������м�
				point.x = point.x + sliderLength;	//�ֳ�
				itoa(pos, outPutPos, 10);	//ʮ����
				memset(outPutPos + strlen(outPutPos), ' ', 10 - strlen(outPutPos));	//��0�������ո�
				outPutPos[9] = 0;	//�����ӽ�����
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
	//Invalidate();	//�����ػ���ϢOnPaint
	for (int i = 0; i < textLineNum; i++)
	{
		for (int j = 0; j < textListNum; j++)
		{
			m_pPDC->TextOut(textPoint[i][j].x, textPoint[i][j].y, textStr[i][j]);
		}
	}
	return 0;
}
