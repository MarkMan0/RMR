
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
	ipaddress = "192.168.1.15";
	datacounter = 0;
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
	//CDialogEx::OnOK();
	laserthreadHandle = CreateThread(NULL, 0, laserUDPVlakno, (void *)this, 0, &laserthreadID);
	robotthreadHandle = CreateThread(NULL, 0, robotUDPVlakno, (void *)this, 0, &robotthreadID);
}

void CRMR_GUIDlg::processThisRobot()
{
	//tu mozete robit s datami z robota..napriklad pozerat ci nam niekam nenarazil
	// ale nic vypoctovo narocne - to iste vlakno ktore cita data z robota
	//teraz tu len vypisujeme data z robota. vyratajte si polohu. a vypiste spravnu
	if (datacounter % 5 == 0)
	{
		
		
		this->SetDlgItemTextW(IDC_EDIT1,CString(std::to_string(robotdata.EncoderRight).c_str()));
		this->SetDlgItemTextW(IDC_EDIT2,CString(std::to_string(robotdata.EncoderLeft).c_str()));
		this->SetDlgItemTextW(IDC_EDIT3,CString(std::to_string(robotdata.GyroAngle).c_str()));
	/*	ui->lineEdit_2->setText(QString::number(robotdata.EncoderRight));
		ui->lineEdit_3->setText(QString::number(robotdata.EncoderLeft));
		ui->lineEdit_4->setText(QString::number(robotdata.GyroAngle));
		*/
	}
	datacounter++;
}

void CRMR_GUIDlg::processThisLidar(LaserMeasurement &laserData)
{
	memcpy(&copyOfLaserData, &laserData, sizeof(LaserMeasurement));
	//tu mozete robit s datami z lidaru.. napriklad najst prekazky, zapisat do mapy. naplanovat ako sa prekazke vyhnut.
	// ale nic vypoctovo narocne - to iste vlakno ktore cita data z lidaru
	updateLaserPicture = 1;
	Invalidate();//tento prikaz prinuti prekreslit obrazovku.. zavola sa paintEvent funkcia



}


void CRMR_GUIDlg::laserprocess()
{
	WSADATA wsaData = { 0 };
	int iResult = 0;



	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	las_slen = sizeof(las_si_other);
	if ((las_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{

	}

	int las_broadcastene = 1;
	setsockopt(las_s, SOL_SOCKET, SO_BROADCAST, (char*)&las_broadcastene, sizeof(las_broadcastene));
	// zero out the structure
	memset((char *)&las_si_me, 0, sizeof(las_si_me));

	las_si_me.sin_family = AF_INET;
	las_si_me.sin_port = htons(52999);
	las_si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	las_si_posli.sin_family = AF_INET;
	las_si_posli.sin_port = htons(5299);
	las_si_posli.sin_addr.s_addr = inet_addr(ipaddress.data());//htonl(INADDR_BROADCAST);
	bind(las_s, (struct sockaddr*)&las_si_me, sizeof(las_si_me));
	char command = 0x00;
	if (sendto(las_s, &command, sizeof(command), 0, (struct sockaddr*) &las_si_posli, las_slen) == -1)
	{

	}
	LaserMeasurement measure;
	while (1)
	{
		if ((las_recv_len = recvfrom(las_s, (char*)&measure.Data, sizeof(LaserData) * 1000, 0, (struct sockaddr *) &las_si_other, (int*)&las_slen)) == -1)
		{

			continue;
		}
		measure.numberOfScans = las_recv_len / sizeof(LaserData);
		//tu mame data..zavolame si funkciu

		//     memcpy(&sens,buff,sizeof(sens));


		processThisLidar(measure);




	}
}


void CRMR_GUIDlg::robotprocess()
{
	WSADATA wsaData = { 0 };
	int iResult = 0;



	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if ((rob_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{

	}

	char rob_broadcastene = 1;
	setsockopt(rob_s, SOL_SOCKET, SO_BROADCAST, &rob_broadcastene, sizeof(rob_broadcastene));
	// zero out the structure
	memset((char *)&rob_si_me, 0, sizeof(rob_si_me));

	rob_si_me.sin_family = AF_INET;
	rob_si_me.sin_port = htons(53000);
	rob_si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	rob_si_posli.sin_family = AF_INET;
	rob_si_posli.sin_port = htons(5300);
	rob_si_posli.sin_addr.s_addr = inet_addr(ipaddress.data());//inet_addr("10.0.0.1");// htonl(INADDR_BROADCAST);
	rob_slen = sizeof(rob_si_me);
	bind(rob_s, (struct sockaddr*)&rob_si_me, sizeof(rob_si_me));

	std::vector<unsigned char> mess = robot.setDefaultPID();
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
	Sleep(100);
	mess = robot.setSound(440, 1000);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
	unsigned char buff[50000];
	while (1)
	{
		memset(buff, 0, 50000 * sizeof(char));
		if ((rob_recv_len = recvfrom(rob_s, (char*)&buff, sizeof(char) * 50000, 0, (struct sockaddr *) &rob_si_other, (int*)&rob_slen)) == -1)
		{

			continue;
		}
		//tu mame data..zavolame si funkciu

		//     memcpy(&sens,buff,sizeof(sens));
		struct timespec t;
		//      clock_gettime(CLOCK_REALTIME,&t);

		int returnval = robot.fillData(robotdata, (unsigned char*)buff);
		if (returnval == 0)
		{
			processThisRobot();
		}


	}
}


void CRMR_GUIDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//pohyb dopredu
	std::vector<unsigned char> mess = robot.setTranslationSpeed(250);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}


void CRMR_GUIDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	std::vector<unsigned char> mess = robot.setTranslationSpeed(0);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}


void CRMR_GUIDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	std::vector<unsigned char> mess = robot.setTranslationSpeed(-250);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}


void CRMR_GUIDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	std::vector<unsigned char> mess = robot.setRotationSpeed(3.14159 / 2);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}


void CRMR_GUIDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	std::vector<unsigned char> mess = robot.setRotationSpeed(-3.14159 / 2);
	if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
	{

	}
}
