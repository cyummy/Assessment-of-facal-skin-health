#if !defined(AFX_THRESHSTRECHDLG_H__F5A88015_EC16_4053_AA58_DA0978131BCE__INCLUDED_)
#define AFX_THRESHSTRECHDLG_H__F5A88015_EC16_4053_AA58_DA0978131BCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThreshStrechDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ThreshStrechDlg dialog

class ThreshStrechDlg : public CDialog
{
// Construction
public:
	ThreshStrechDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ThreshStrechDlg)
	enum { IDD = IDD_DLG_THRESH };
	int		m_Thresh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ThreshStrechDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ThreshStrechDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THRESHSTRECHDLG_H__F5A88015_EC16_4053_AA58_DA0978131BCE__INCLUDED_)
