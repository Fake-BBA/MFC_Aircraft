#pragma once
#include "BBA_CWnd.h"
#include "resource.h"
#include <afxtempl.h>
class BBA_CoordinateWaveform{
public:
	BBA_CoordinateWaveform();
	~BBA_CoordinateWaveform();
	BBA_CoordinateWaveform(BBA_CoordinateWaveform &p);
	BBA_CoordinateWaveform operator=(const BBA_CoordinateWaveform &p);
public:
	CPen pen;
#define MAX_POINT	300
	POINT point[MAX_POINT];
	POINT startPoint;	//��ʼ��
	POINT endPoint;		//���µĵ�
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
	CList<BBA_CoordinateWaveform,BBA_CoordinateWaveform&> waveformList;
	

private:
	int DrawCoordinate(CDC *pDC);
public:
	int m_CoordinateCenter_X;	//X��������ƫ��ֵ��0Ϊ��ƫ��
	int m_CoordinateCenter_Y;	//Y��������ƫ��ֵ��0Ϊ��ƫ��
	POINT m_StartX;	//X���������
	POINT m_EndX;	//X���յ�����
	POINT m_StartY;	//Y���������
	POINT m_EndY;	//Y���յ�����
	int m_unitOneX;	//���ٸ����ش���X���һ���̶�С��
	int m_unitOneY;	//���ٸ����ش���Y���һ���̶�С��
	int m_uintLen;	//�̶ȵĳ���
	CRect m_RectClientWindow;	//�ɻ��Ƶ�����,��OnPaint��Ϣʵʱ����

public:
	int CreatCoordinateWindow(CString wndTitle, CRect *rect = NULL, CWnd* pParent = NULL, DWORD id = 0, DWORD style = WS_VISIBLE | WS_CHILD);
	void OnTimer(UINT_PTR nIDEvent);
public:
#define IDC_BTN_REFLASH	2048
	CButton m_BtReflash;	//ˢ�¿��ư�ť
public:
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnReflashClick();
};