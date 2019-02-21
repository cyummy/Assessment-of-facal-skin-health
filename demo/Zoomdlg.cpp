// Zoomdlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "Zoomdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Zoomdlg dialog


Zoomdlg::Zoomdlg(CWnd* pParent /*=NULL*/)
	: CDialog(Zoomdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Zoomdlg)
	m_strechHeight = 0.0f;
	m_strechWidth = 0.0f;
	//}}AFX_DATA_INIT
}


void Zoomdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Zoomdlg)
	DDX_Text(pDX, IDC_EDIT_strechH, m_strechHeight);
	DDX_Text(pDX, IDC_EDIT_strechW, m_strechWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Zoomdlg, CDialog)
	//{{AFX_MSG_MAP(Zoomdlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Zoomdlg message handlers
/////////////////////////////////////////////////////////////////////////////
// ZoomDlg dialog


ZoomDlg::ZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ZoomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ZoomDlg)
	m_YstrechRatio = 0.0f;
	m_XstrechRatio = 0.0f;
	//}}AFX_DATA_INIT
}


void ZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ZoomDlg)
	DDX_Text(pDX, IDC_EDIT_strechH, m_YstrechRatio);
	DDX_Text(pDX, IDC_EDIT_strechW, m_XstrechRatio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ZoomDlg, CDialog)
	//{{AFX_MSG_MAP(ZoomDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZoomDlg message handlers
