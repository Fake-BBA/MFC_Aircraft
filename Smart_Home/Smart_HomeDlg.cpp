
// Smart_HomeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Smart_Home.h"
#include "Smart_HomeDlg.h"
#include "afxdialogex.h"
#include "smartHomeTypedef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmartHomeDlg 对话框



CSmartHomeDlg::CSmartHomeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMART_HOME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmartHomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_CListBox_Ctrl);
}

BEGIN_MESSAGE_MAP(CSmartHomeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_BBA_UDP_RECV, &CSmartHomeDlg::OnAP_ServerMsg)
	ON_MESSAGE(WM_BBA_UDP_RECV+1, &CSmartHomeDlg::OnStationServerMsg)
	ON_BN_CLICKED(IDC_BUTTON_SET_WIFI, &CSmartHomeDlg::OnBnClickedButtonSetWifi)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_LOG, &CSmartHomeDlg::OnBnClickedButtonCleanLog)
	ON_BN_CLICKED(IDC_BUTTON_GET_WIFI_IP, &CSmartHomeDlg::OnBnClickedButtonGetWifiIp)
	ON_BN_CLICKED(IDC_BUTTON_SEND_STATION, &CSmartHomeDlg::OnBnClickedButtonSendStation)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_COORDINATE, &CSmartHomeDlg::OnBnClickedButtonOpenCoordinate)
	ON_BN_CLICKED(IDC_BUTTON_FLIGHT_STATUS, &CSmartHomeDlg::OnBnClickedButtonFlightStatus)
END_MESSAGE_MAP()




BOOL CSmartHomeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	bbaLog.BBA_LOG_Init(&m_CListBox_Ctrl);	//初始化日志类
	m_p_BBA_Coordinate = NULL;	//初始化波形图窗口指针
	m_p_BBA_FlightStatus = NULL;	//初始化飞行器数据窗口指针
	//OnBnClickedButtonFlightStatus();	//测试用
	bba_AP_udp.Init("192.168.4.2", 1025, "192.168.4.1",1026);	//初始化连接到ESP8266 AP的IP地址和端口
	bba_Station_udp.Init(bba_Station_udp.localIP, 1026, "192.168.31.80", 1025);	//初始化连接到ESP8266station的IP地址和端口

	//SOCKET 异步消息
	WSAAsyncSelect(bba_AP_udp.m_socket, m_hWnd, WM_BBA_UDP_RECV, FD_READ);	//监听套接字UDP接收到的消息
	WSAAsyncSelect(bba_Station_udp.m_socket, m_hWnd, WM_BBA_UDP_RECV+1, FD_READ);	//监听套接字UDP接收到的消息
	//m_pCWnd->ShowWindow(2);
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSmartHomeDlg::OnPaint()
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSmartHomeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSmartHomeDlg::OnBnClickedButtonCleanLog()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CListBox_Ctrl.ResetContent();
}

LRESULT CSmartHomeDlg::OnAP_ServerMsg(WPARAM wParam, LPARAM lParam)
{
	bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, "Message Recv");
	
	
	SOCKET m_socket = (SOCKET)wParam;
	char p_buffer[200];
	char logStr[200];
	memset(p_buffer, 0x00, 200);
	int len;
	
	switch (WSAGETSELECTEVENT(lParam))
	{

	case FD_READ:
		len = bba_AP_udp.Recvfrom(p_buffer);
		enum MessageCtrFunction messageCtrFunction;
		static union MessagePacketUnion messagePacketUnion;
		memcpy(messagePacketUnion.p_buff, p_buffer, MESSAGE_PACKET_SIZE);

		messageCtrFunction = (MessageCtrFunction)messagePacketUnion.messagePacket.function_word;
		sprintf(logStr, "功能字：%d\r\n", messageCtrFunction);
		bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);

		sprintf(logStr, "发送者：%d\r\n", messagePacketUnion.messagePacket.sender);
		bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
		
		sprintf(logStr,"接收者：%d\r\n", messagePacketUnion.messagePacket.receiver);
		bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);

		switch (messageCtrFunction)
		{
		case GET_WIFI_IP:
			sprintf(logStr, "设备：%d 已连接上的IP：%d.%d.%d.%d\r\n", messagePacketUnion.messagePacket.sender,\
				messagePacketUnion.messagePacket.data[0], messagePacketUnion.messagePacket.data[1],\
				messagePacketUnion.messagePacket.data[2], messagePacketUnion.messagePacket.data[3]);
			bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
			break;
		default:
			sprintf(logStr, "没有对应功能字:%d\r\n", messagePacketUnion.messagePacket.function_word);
			bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
			break;
		}
		
	case FD_CLOSE:
		break;
	default:
		break;
	}
	return 1;
}

LRESULT CSmartHomeDlg::OnStationServerMsg(WPARAM wParam, LPARAM lParam)
{
	//bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, "Message Recv");


	SOCKET m_socket = (SOCKET)wParam;
	char p_buffer[200];
	char logStr[200];
	memset(p_buffer, 0x00, 200);
	int len;

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		len = bba_Station_udp.Recvfrom(p_buffer);
		m_p_BBA_FlightStatus->UpdataFlightStatusWindows(p_buffer, len);
		//bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, p_buffer);
		break;
	case FD_CLOSE:
		break;
	default:
		break;
	}
	return 1;
}

void CSmartHomeDlg::OnBnClickedButtonSetWifi()
{
	// TODO: 在此添加控件通知处理程序代码
	char wifiSSid[50];
	char wifiPass[50];
	char device[10];
	MessagePacketUnion messPack;
	uint8 ssidLen, passLen,deviceNum;
	CString cstringWifiSSid;
	CString cstringWifiPass;
	CString cstringDevice;

	GetDlgItem(IDC_EDIT_DEVICE_NUM)->GetWindowText(cstringDevice);
	strcpy(device, cstringDevice);

	messPack.messagePacket.sender = 1;
	messPack.messagePacket.receiver = atoi(device);
	messPack.messagePacket.function_word = WIFI_CONFIG;
	
	GetDlgItem(IDC_EDIT_WIFI_SSID)->GetWindowText(cstringWifiSSid);
	GetDlgItem(IDC_EDIT_WIFI_PASSWORD)->GetWindowText(cstringWifiPass);

	strcpy(wifiSSid, cstringWifiSSid);
	strcpy(wifiPass, cstringWifiPass);
	
	ssidLen = strlen(wifiSSid);
	passLen = strlen(wifiPass);

	messPack.messagePacket.data[0] = ssidLen;
	memcpy(messPack.messagePacket.data +1, wifiSSid, ssidLen);
	messPack.messagePacket.data[ssidLen+1] = passLen;
	memcpy(messPack.messagePacket.data +ssidLen+2, wifiPass, passLen);

	bba_AP_udp.Send((char *)messPack.p_buff, 5+ssidLen+passLen+2);
	bbaLog.CoutLog(BBA_LOG_TYPE_PRINT,"设置新设备Wifi");
}


void CSmartHomeDlg::OnBnClickedButtonGetWifiIp()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstringDevice;
	char device[10];
	char logStr[100];

	GetDlgItem(IDC_EDIT_DEVICE_NUM)->GetWindowText(cstringDevice);
	strcpy(device, cstringDevice);

	MessagePacketUnion messPack;
	messPack.messagePacket.sender = 1;
	messPack.messagePacket.receiver = atoi(device);
	messPack.messagePacket.function_word = GET_WIFI_IP;
	bba_AP_udp.Send((char *)messPack.p_buff, 5);
	sprintf(logStr, "请求设备：%d IP\r\n", messPack.messagePacket.receiver);
	bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
}


void CSmartHomeDlg::OnBnClickedButtonSendStation()
{
	// TODO: 在此添加控件通知处理程序代码
	bba_Station_udp.Send((char *)"Hello World!", 12);
}


void CSmartHomeDlg::OnBnClickedButtonOpenCoordinate()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_p_BBA_Coordinate == NULL)
	{
		m_p_BBA_Coordinate = new BBA_Coordinate();
		m_p_BBA_Coordinate->CreatCoordinateWindow("Coordinate", NULL, this, NULL, NULL);
	}
}


void CSmartHomeDlg::OnBnClickedButtonFlightStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_p_BBA_FlightStatus == NULL)
	{
		//this->GetClientRect(&rect);
		CRect rect(300, 300, 1500, 1000);
		m_p_BBA_FlightStatus = new BBA_FlightStatus();
		m_p_BBA_FlightStatus->CreatFlightStatusWindow("Flight Status", &rect, this, NULL, NULL);
		m_p_BBA_FlightStatus->bba_Station_udp = &bba_Station_udp;
	}
}
