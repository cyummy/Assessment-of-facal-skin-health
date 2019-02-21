// NeiAverSmoothDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "NeiAverSmoothDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NeiAverSmoothDlg dialog


NeiAverSmoothDlg::NeiAverSmoothDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NeiAverSmoothDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(NeiAverSmoothDlg)
	m_radiovalue=0;
	TempH=3;
	TempW=3;
	TempCX=1;
	TempCY=1;
	fCoef=(float)(1.0/9.0);
	//}}AFX_DATA_INIT
}


void NeiAverSmoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NeiAverSmoothDlg)
	DDX_Radio(pDX, IDC_RADIO1_temp, m_radiovalue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NeiAverSmoothDlg, CDialog)
	//{{AFX_MSG_MAP(NeiAverSmoothDlg)
	ON_BN_CLICKED(IDC_RADIO1_temp, OnRADIO1temp)
	ON_BN_CLICKED(IDC_RADIO2_temp, OnRADIO2temp)
	ON_BN_CLICKED(IDC_RADIO3_temp, OnRADIO3temp)
	ON_BN_CLICKED(IDC_RADIO4_temp, OnRADIO4temp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NeiAverSmoothDlg message handlers


BOOL NeiAverSmoothDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	UpdateData(false);
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void NeiAverSmoothDlg::OnOK() 
{
	// TODO: Add extra validation here
    UpdateData(true);

	CDialog::OnOK();
}



void NeiAverSmoothDlg::OnRADIO1temp() 
{
	// TODO: Add your control notification handler code here
	m_radiovalue=0;
	TempH=3;
	TempW=3;
	TempCX=1;
	TempCY=1;
	fCoef=(float)(1.0/9.0);

	UpdateData(false);
}

void NeiAverSmoothDlg::OnRADIO2temp() 
{
	// TODO: Add your control notification handler code here
	m_radiovalue=1;
	TempH=5;
	TempW=5;
	TempCX=2;
	TempCY=2;
	fCoef=(float)(1.0/25.0);

	UpdateData(false);

}

void NeiAverSmoothDlg::OnRADIO3temp() 
{
	// TODO: Add your control notification handler code here
    m_radiovalue=2;
	TempH=7;
	TempW=7;
	TempCX=3;
	TempCY=3;
	fCoef=(float)(1.0/81.0);

	UpdateData(false);
}

void NeiAverSmoothDlg::OnRADIO4temp() 
{
	// TODO: Add your control notification handler code here
	m_radiovalue=3;
	TempH=5;
	TempW=5;
	TempCX=2;
	TempCY=2;
	fCoef=(float)(1.0/48.0);
	
	UpdateData(false);
}
