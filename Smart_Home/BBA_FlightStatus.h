#pragma once
#include "BBA_CWnd.h"
#include "resource.h"
#include <afxtempl.h>

class BBA_FlightStatus :public BBA_CWnd {
public:
	BBA_FlightStatus();
	~BBA_FlightStatus();
protected:
	DECLARE_MESSAGE_MAP();
public:
	/******************text ���****************/
#define STATIC_MAX_LINE_TEXT_NUM	10	//ÿһ�������
#define STATIC_MAX_LIST_TEXT_NUM	10	//ÿһ��

	CPoint textStartPoint;
	CPoint textPoint[STATIC_MAX_LINE_TEXT_NUM][STATIC_MAX_LIST_TEXT_NUM];	//TEXT�����λ��
	int textLineNum;	//ÿһ�е�text��Ŀ
	int textListNum;	//ÿһ�е�text��Ŀ
	int textLineInterval;	//ÿһ��Text�ļ��
	int textListInterval;	//ÿһ��Text�ļ��
	/******************************************/
	/******************Slider���****************/
#define STATIC_MAX_LINE_SLIDER_NUM	2	//ÿһ�������
#define STATIC_MAX_LIST_SLIDER_NUM	10	//ÿһ��
#define SLIDER_NUM	STATIC_MAX_LINE_SLIDER_NUM+STATIC_MAX_LIST_SLIDER_NUM
#define IDC_SLIDER_MSG_START	3000	//��ϢID��ʼֵ
	CPoint sliderStartPoint;	//�������
	CPoint sliderPoint[STATIC_MAX_LINE_SLIDER_NUM][STATIC_MAX_LIST_SLIDER_NUM];
	int sliderLineNum;	//ÿһ�е�slider��Ŀ
	int sliderListNum;	//ÿһ�е�slider��Ŀ
	int sliderLineInterval;	//ÿһ��Slider�ļ��
	int sliderListInterval;	//ÿһ��Slider�ļ��
	CSliderCtrl slider[STATIC_MAX_LINE_SLIDER_NUM][STATIC_MAX_LIST_SLIDER_NUM];
	/******************************************/
	CPaintDC *m_pPDC;	//
	CRect m_RectClientWindow;	//�ɻ��Ƶ�����,��OnPaint��Ϣʵʱ����
public:
	int CreatFlightStatusWindow(CString wndTitle, CRect *rect = NULL, CWnd* pParent = NULL, DWORD id = 0, DWORD style = WS_VISIBLE | WS_CHILD);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
