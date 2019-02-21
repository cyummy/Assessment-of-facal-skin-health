// MedianSmoothDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "MedianSmoothDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MedianSmoothDlg dialog


MedianSmoothDlg::MedianSmoothDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MedianSmoothDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(MedianSmoothDlg)
	m_radiovalue = 0;
	TempH=3;
	TempW=3;
	TempCX=1;
	TempCY=1;
	//}}AFX_DATA_INIT
}


void MedianSmoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MedianSmoothDlg)
	DDX_Radio(pDX, IDC_RADIO1_temp, m_radiovalue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MedianSmoothDlg, CDialog)
	//{{AFX_MSG_MAP(MedianSmoothDlg)
	ON_BN_CLICKED(IDC_RADIO1_temp, OnRADIO1temp)
	ON_BN_CLICKED(IDC_RADIO2_temp, OnRADIO2temp)
	ON_BN_CLICKED(IDC_RADIO3_temp, OnRADIO3temp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MedianSmoothDlg message handlers

BOOL MedianSmoothDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MedianSmoothDlg::OnRADIO1temp() 
{
	// TODO: Add your control notification handler code here
	m_radiovalue = 0;
	TempH=3;
	TempW=3;
	TempCX=1;
	TempCY=1;
	UpdateData(false);
}

void MedianSmoothDlg::OnRADIO2temp() 
{
	// TODO: Add your control notification handler code here
	m_radiovalue=1;
	TempH=5;
	TempW=5;
	TempCX=2;
	TempCY=2;
	UpdateData(false);
}

void MedianSmoothDlg::OnRADIO3temp() 
{
	// TODO: Add your control notification handler code here
	m_radiovalue=2;
	TempH=7;
	TempW=7;
	TempCX=3;
	TempCY=3;
    UpdateData(false);
}

void MedianSmoothDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	CDialog::OnOK();
}


