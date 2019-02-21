// RotateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "RotateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RotateDlg dialog


RotateDlg::RotateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RotateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RotateDlg)
	m_RotateAngle = 0;
	//}}AFX_DATA_INIT
}


void RotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RotateDlg)
	DDX_Text(pDX, IDC_EDIT_Rotate, m_RotateAngle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RotateDlg, CDialog)
	//{{AFX_MSG_MAP(RotateDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RotateDlg message handlers
