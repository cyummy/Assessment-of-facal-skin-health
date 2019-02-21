#if !defined(AFX_MEDIANSMOOTHDLG_H__692F9888_3702_404C_8676_946B26F5E0F8__INCLUDED_)
#define AFX_MEDIANSMOOTHDLG_H__692F9888_3702_404C_8676_946B26F5E0F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MedianSmoothDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MedianSmoothDlg dialog

class MedianSmoothDlg : public CDialog
{
// Construction
public:
	MedianSmoothDlg(CWnd* pParent = NULL);   // standard constructor

public:
	int TempH,TempW,TempCX,TempCY;
// Dialog Data
	//{{AFX_DATA(MedianSmoothDlg)
	enum { IDD = IDD_DLG_MediaSmooth };
	int		m_radiovalue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MedianSmoothDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MedianSmoothDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRADIO1temp();
	afx_msg void OnRADIO2temp();
	afx_msg void OnRADIO3temp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDIANSMOOTHDLG_H__692F9888_3702_404C_8676_946B26F5E0F8__INCLUDED_)
