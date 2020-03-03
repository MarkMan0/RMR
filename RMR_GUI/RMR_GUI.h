
// RMR_GUI.h : main header file for the PROJECT_NAME application
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	//#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRMR_GUIApp:
// See RMR_GUI.cpp for the implementation of this class
//

class CRMR_GUIApp : public CWinApp
{
public:
	CRMR_GUIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRMR_GUIApp theApp;