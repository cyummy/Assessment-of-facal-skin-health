#if !defined(AFX_LINEARSTEPPARAMDLG_H__D72D321B_363E_4DA8_82C9_7385E03BAF15__INCLUDED_)
#define AFX_LINEARSTEPPARAMDLG_H__D72D321B_363E_4DA8_82C9_7385E03BAF15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinearStepParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LinearStepParamDlg dialog

class LinearStepParamDlg : public CDialog
{
// Construction
public:
	// ��ǰ����϶�״̬��0��ʾδ�϶���1��ʾ�����϶���һ�㣬2��ʾ�����϶��ڶ��㡣
	int		m_nIsDraging;
	
	// ��Ӧ����¼��ľ�������
	CRect	m_rectMouse;
	
	// ��ʶ�Ƿ��Ѿ�������Ƥ����
	BOOL	m_bDrawed;
	
	// ��������������ʱ��λ��
	CPoint	m_pointMsClick;
	
	// ��������϶�ʱ��λ��
	CPoint	m_pointMsMove;
	
	LinearStepParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LinearStepParamDlg)
	enum { IDD = IDD_DLG_LinearStretch };
	int		m_nX1;
	int		m_nX2;
	int		m_nY1;
	int		m_nY2;
	CString	m_X1;
	CString	m_Y1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LinearStepParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LinearStepParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnOK();
	afx_msg void OnKillfocusEDITLinetransX1();
	afx_msg void OnKillfocusEDITLinetransX2();
	afx_msg void OnKillfocusEDITLinetransY1();
	afx_msg void OnKillfocusEDITLinetransY2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEARSTEPPARAMDLG_H__D72D321B_363E_4DA8_82C9_7385E03BAF15__INCLUDED_)
