
// Smart_HomeDlg.h: 头文件
//
#include "BBA_SOCKET.h"
#include "bba_log.h"
#include "smartHomeTypedef.h"
#include "BBA_Coordinate.h"
#pragma once

// CSmartHomeDlg 对话框
class CSmartHomeDlg : public CDialogEx
{
// 构造
public:
	CSmartHomeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMART_HOME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnAP_ServerMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStationServerMsg(WPARAM wParam, LPARAM lParam);

	BBA_Log bbaLog;
	BBA_UDP bba_AP_udp;
	BBA_UDP bba_Station_udp;
	BBA_Coordinate *m_p_BBA_Coordinate;
	CListBox m_CListBox_Ctrl;

	afx_msg void OnBnClickedButtonSetWifi();
	afx_msg void OnBnClickedButtonCleanLog();
	afx_msg void OnBnClickedButtonGetWifiIp();
	afx_msg void OnBnClickedButtonSendStation();
	afx_msg void OnBnClickedButtonOpenCoordinate();
};
