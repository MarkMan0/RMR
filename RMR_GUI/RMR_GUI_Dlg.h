
// RMR_GUIDlg.h : header file
//

#pragma once

#include<windows.h>
#include<iostream>
//#include<arpa/inet.h>
//#include<unistd.h>
//#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include "ckobuki.h"

#include "rplidar.h"

// CRMR_GUIDlg dialog
class CRMR_GUIDlg : public CDialogEx
{
// Construction
public:
	CRMR_GUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RMR_GUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();


	void robotprocess();
	void laserprocess();
	void processThisLidar(LaserMeasurement &laserData);

	void processThisRobot();
	HANDLE robotthreadHandle; // handle na vlakno
	DWORD robotthreadID;  // id vlakna
	static DWORD WINAPI robotUDPVlakno(void *param)
	{
		((CRMR_GUIDlg*)param)->robotprocess();
		return 0;
	}
	HANDLE laserthreadHandle; // handle na vlakno
	DWORD laserthreadID;  // id vlakna
	static DWORD WINAPI laserUDPVlakno(void *param)
	{
		((CRMR_GUIDlg*)param)->laserprocess();

		return 0;
	}
	//veci na broadcast laser
	struct sockaddr_in las_si_me, las_si_other, las_si_posli;

	int las_s, las_recv_len;
	unsigned int las_slen;
	//veci na broadcast robot
	struct sockaddr_in rob_si_me, rob_si_other, rob_si_posli;

	int rob_s, rob_recv_len;
	unsigned int rob_slen;

	int updateLaserPicture;
	LaserMeasurement copyOfLaserData;
	std::string ipaddress;
	CKobuki robot;
	TKobukiData robotdata;
	int datacounter;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
};
