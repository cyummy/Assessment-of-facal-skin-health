// StrechWindowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "StrechWindowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StrechWindowDlg dialog


StrechWindowDlg::StrechWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(StrechWindowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(StrechWindowDlg)
	m_nBottom = 0;
	m_nTop = 0;
	//}}AFX_DATA_INIT
}


void StrechWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StrechWindowDlg)
	DDX_Text(pDX, IDC_EDIT_StrechBottom, m_nBottom);
	DDX_Text(pDX, IDC_EDIT_StrechTop, m_nTop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StrechWindowDlg, CDialog)
	//{{AFX_MSG_MAP(StrechWindowDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StrechWindowDlg message handlers
