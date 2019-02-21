#if !defined(AFX_STRECHWINDOWDLG_H__E181372E_6695_466E_AD93_4C2AFA6071F9__INCLUDED_)
#define AFX_STRECHWINDOWDLG_H__E181372E_6695_466E_AD93_4C2AFA6071F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StrechWindowDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// StrechWindowDlg dialog

class StrechWindowDlg : public CDialog
{
// Construction
public:
	StrechWindowDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(StrechWindowDlg)
	enum { IDD = IDD_Dlg_THRESHWD };
	int		m_nBottom;
	int		m_nTop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StrechWindowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(StrechWindowDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRECHWINDOWDLG_H__E181372E_6695_466E_AD93_4C2AFA6071F9__INCLUDED_)
