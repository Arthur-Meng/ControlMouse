
// ControlMouseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlMouse.h"
#include "ControlMouseDlg.h"
#include "afxdialogex.h"
#include <string.h>
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include<conio.h>  
#include<windows.h>  

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include "eyex/EyeX.h"
#include"Resource.h"
#pragma comment (lib, "Tobii.EyeX.Client.lib")

// ID of the global interactor that provides our data stream; must be unique within the application.
static const TX_STRING InteractorId = "Twilight Sparkle";

// global variables
static TX_HANDLE g_hGlobalInteractorSnapshot = TX_EMPTY_HANDLE;
static int number = 0,direction=0,ifdouble=0,stop=0;
static float change = 0;


BOOL InitializeGlobalInteractorSnapshot(TX_CONTEXTHANDLE hContext)
{
	TX_HANDLE hInteractor = TX_EMPTY_HANDLE;
	TX_GAZEPOINTDATAPARAMS params = { TX_GAZEPOINTDATAMODE_LIGHTLYFILTERED };
	BOOL success;

	success = txCreateGlobalInteractorSnapshot(
		hContext,
		InteractorId,
		&g_hGlobalInteractorSnapshot,
		&hInteractor) == TX_RESULT_OK;
	success &= txCreateGazePointDataBehavior(hInteractor, &params) == TX_RESULT_OK;

	txReleaseObject(&hInteractor);

	return success;
}


void TX_CALLCONVENTION OnSnapshotCommitted(TX_CONSTHANDLE hAsyncData, TX_USERPARAM param)
{
	// check the result code using an assertion.
	// this will catch validation errors and runtime errors in debug builds. in release builds it won't do anything.

	TX_RESULT result = TX_RESULT_UNKNOWN;
	txGetAsyncDataResultCode(hAsyncData, &result);
	assert(result == TX_RESULT_OK || result == TX_RESULT_CANCELLED);
}


void TX_CALLCONVENTION OnEngineConnectionStateChanged(TX_CONNECTIONSTATE connectionState, TX_USERPARAM userParam)
{
	switch (connectionState) {
	case TX_CONNECTIONSTATE_CONNECTED: {
		BOOL success;
		success = txCommitSnapshotAsync(g_hGlobalInteractorSnapshot, OnSnapshotCommitted, NULL) == TX_RESULT_OK;
	}
									   break;

	case TX_CONNECTIONSTATE_DISCONNECTED:
		break;

	case TX_CONNECTIONSTATE_TRYINGTOCONNECT:
		break;

	case TX_CONNECTIONSTATE_SERVERVERSIONTOOLOW:
		break;

	case TX_CONNECTIONSTATE_SERVERVERSIONTOOHIGH:
		break;
	}
}


void OnGazeDataEvent(TX_HANDLE hGazeDataBehavior)
{
	TX_GAZEPOINTDATAEVENTPARAMS eventParams;
	if (txGetGazePointDataEventParams(hGazeDataBehavior, &eventParams) == TX_RESULT_OK) {
		//printf("%f, %f", eventParams.X, eventParams.Y);
		float x = eventParams.X, y = eventParams.Y;
		CRect re;
		SetCursorPos(x , y ); 
		if (stop = 0) {
			if (number == 0) {
				re.left = x - 20;
				re.right = x + 20;
				re.top = y - 20;
				re.bottom = y + 20;
				number++;
			}
			else {
				if (x >= re.left&&x <= re.right&&y >= re.top&&y <= re.bottom) {
					number++;
					if (number >= 10) {
						if (direction == 0) {
							if (ifdouble == 0) {
								mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
								mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
							}
							if (ifdouble == 1) {
								mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
								mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
								mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
								mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
							}

						}
						if (direction == 1) {
							mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
							mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
						}
					}
				}
				else {
					number = 0;
				}
			}
		}
		
	}
}


void TX_CALLCONVENTION HandleEvent(TX_CONSTHANDLE hAsyncData, TX_USERPARAM userParam)
{
	TX_HANDLE hEvent = TX_EMPTY_HANDLE;
	TX_HANDLE hBehavior = TX_EMPTY_HANDLE;

	txGetAsyncDataContent(hAsyncData, &hEvent);

	// NOTE. Uncomment the following line of code to view the event object. The same function can be used with any interaction object.
	//OutputDebugStringA(txDebugObject(hEvent));

	if (txGetEventBehavior(hEvent, &hBehavior, TX_BEHAVIORTYPE_GAZEPOINTDATA) == TX_RESULT_OK) {
		OnGazeDataEvent(hBehavior);
		txReleaseObject(&hBehavior);
	}

	txReleaseObject(&hEvent);
}


void mymain()
{
	TX_CONTEXTHANDLE hContext = TX_EMPTY_HANDLE;
	TX_TICKET hConnectionStateChangedTicket = TX_INVALID_TICKET;
	TX_TICKET hEventHandlerTicket = TX_INVALID_TICKET;
	BOOL success;

	// initialize and enable the context that is our link to the EyeX Engine.
	success = txInitializeEyeX(TX_EYEXCOMPONENTOVERRIDEFLAG_NONE, NULL, NULL, NULL, NULL) == TX_RESULT_OK;
	success &= txCreateContext(&hContext, TX_FALSE) == TX_RESULT_OK;
	success &= InitializeGlobalInteractorSnapshot(hContext);
	success &= txRegisterConnectionStateChangedHandler(hContext, &hConnectionStateChangedTicket, OnEngineConnectionStateChanged, NULL) == TX_RESULT_OK;
	success &= txRegisterEventHandler(hContext, &hEventHandlerTicket, HandleEvent, NULL) == TX_RESULT_OK;
	success &= txEnableConnection(hContext) == TX_RESULT_OK;

	// let the events flow until a key is pressed.

}



// CControlMouseDlg 对话框



CControlMouseDlg::CControlMouseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONTROLMOUSE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CControlMouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CControlMouseDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CControlMouseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CControlMouseDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDCANCEL, &CControlMouseDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CControlMouseDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CControlMouseDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CControlMouseDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CControlMouseDlg 消息处理程序

BOOL CControlMouseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	AfxGetMainWnd()->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	mymain();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CControlMouseDlg::OnPaint()
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
HCURSOR CControlMouseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CControlMouseDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	direction = 0;
	stop = 0;
}


void CControlMouseDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	ifdouble = 0;
}


void CControlMouseDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CControlMouseDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	direction = 1;
	stop = 0;
}


void CControlMouseDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	ifdouble = 1;
}


void CControlMouseDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	stop = 1;
}
