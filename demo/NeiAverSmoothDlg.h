#if !defined(AFX_NEIAVERSMOOTHDLG_H__651358E0_6885_4781_BF7F_756C9C4610B5__INCLUDED_)
#define AFX_NEIAVERSMOOTHDLG_H__651358E0_6885_4781_BF7F_756C9C4610B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NeiAverSmoothDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NeiAverSmoothDlg dialog

class NeiAverSmoothDlg : public CDialog
{
// Construction
public:
    int TempH, TempW, TempCX, TempCY;
    float fCoef;

public:
	NeiAverSmoothDlg(CWnd* pParent = NULL);   // standard constructor
    
// Dialog Data
	//{{AFX_DATA(NeiAverSmoothDlg)
	enum { IDD = IDD_DLG_NeiborSmooth };
	int		m_radiovalue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NeiAverSmoothDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NeiAverSmoothDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRADIO1temp();
	afx_msg void OnRADIO2temp();
	afx_msg void OnRADIO3temp();
	afx_msg void OnRADIO4temp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEIAVERSMOOTHDLG_H__651358E0_6885_4781_BF7F_756C9C4610B5__INCLUDED_)
