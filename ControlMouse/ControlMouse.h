
// ControlMouse.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CControlMouseApp: 
// �йش����ʵ�֣������ ControlMouse.cpp
//

class CControlMouseApp : public CWinApp
{
public:
	CControlMouseApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CControlMouseApp theApp;