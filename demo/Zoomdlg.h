#if !defined(AFX_ZOOMDLG_H__F466623C_B72C_4E57_A4A3_0DBEE2E271D3__INCLUDED_)
#define AFX_ZOOMDLG_H__F466623C_B72C_4E57_A4A3_0DBEE2E271D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Zoomdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Zoomdlg dialog

class Zoomdlg : public CDialog
{
// Construction
public:
	Zoomdlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Zoomdlg)
	enum { IDD = IDD_DIALOG_ZOOM };
	float	m_strechHeight;
	float	m_strechWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Zoomdlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Zoomdlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// ZoomDlg dialog

class ZoomDlg : public CDialog
{
// Construction
public:
	ZoomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ZoomDlg)
	enum { IDD = IDD_DIALOG_ZOOM };
	float	m_YstrechRatio;
	float	m_XstrechRatio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ZoomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ZoomDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMDLG_H__F466623C_B72C_4E57_A4A3_0DBEE2E271D3__INCLUDED_)
