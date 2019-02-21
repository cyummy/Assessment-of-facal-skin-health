// ThreshStrechDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "ThreshStrechDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ThreshStrechDlg dialog


ThreshStrechDlg::ThreshStrechDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ThreshStrechDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ThreshStrechDlg)
	m_Thresh = 0;
	//}}AFX_DATA_INIT
}


void ThreshStrechDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ThreshStrechDlg)
	DDX_Text(pDX, IDC_EDIT_Thresh, m_Thresh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ThreshStrechDlg, CDialog)
	//{{AFX_MSG_MAP(ThreshStrechDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ThreshStrechDlg message handlers
