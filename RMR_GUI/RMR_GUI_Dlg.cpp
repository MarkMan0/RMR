
// RMR_GUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RMR_GUI.h"
#include "RMR_GUI_Dlg.h"
#include "afxdialogex.h"
#include <string> 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRMR_GUIDlg dialog



CRMR_GUIDlg::CRMR_GUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RMR_GUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRMR_GUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRMR_GUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRMR_GUIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CRMR_GUIDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CRMR_GUIDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CRMR_GUIDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &CRMR_GUIDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CRMR_GUIDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CRMR_GUIDlg message handlers

BOOL CRMR_GUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRMR_GUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRMR_GUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rct;
		GetClientRect(&rct);
		CPaintDC painter(this);
		painter.BitBlt(10, 10, rct.Width()-100, rct.Height()-100, &painter, 0, 0, WHITENESS);
		CPen thick_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen *oldPen;
		oldPen = painter.SelectObject(&thick_pen);
		/*
		if (updateLaserPicture == 1)
		{
			updateLaserPicture = 0;

			
			//teraz tu kreslime random udaje... vykreslite to co treba... t.j. data z lidaru
			for (int k = 0; k<copyOfLaserData.numberOfScans; k++)
			{
				
				int dist = copyOfLaserData.Data[k].scanDistance/20;
				int xp = (rct.Width() / 2 + dist * 2 * sin((360.0 - copyOfLaserData.Data[k].scanAngle)*3.14159 / 180.0)) + rct.TopLeft().x;
				int yp = rct.Height()-(rct.Height() / 2 + dist * 2 * cos((360.0 - copyOfLaserData.Data[k].scanAngle)*3.14159 / 180.0)) + rct.TopLeft().y;
				painter.Ellipse(xp - 1, yp - 1, xp + 1, yp + 1);
				
			}
			
		}
		*/
		
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRMR_GUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRMR_GUIDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

//this->SetDlgItemTextW(IDC_EDIT1, CString(std::to_string(robotdata.EncoderRight).c_str()));
//this->SetDlgItemTextW(IDC_EDIT2, CString(std::to_string(robotdata.EncoderLeft).c_str()));
//this->SetDlgItemTextW(IDC_EDIT3, CString(std::to_string(robotdata.GyroAngle).c_str()));




void CRMR_GUIDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//pohyb dopredu
}


void CRMR_GUIDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

}


void CRMR_GUIDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here

}


void CRMR_GUIDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

}


void CRMR_GUIDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here

}
