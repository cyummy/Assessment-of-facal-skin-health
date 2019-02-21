/////////////////////////////////////////////////////////////////////////////
// GeometryMoveDlg dialog

class GeometryMoveDlg : public CDialog
{
// Construction
public:
	GeometryMoveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GeometryMoveDlg)
	enum { IDD = IDD_DLG_MOVE };
	int		m_Xmove;
	int		m_Ymove;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GeometryMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GeometryMoveDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
