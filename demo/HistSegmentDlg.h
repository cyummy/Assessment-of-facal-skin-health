#if !defined(AFX_HISTSEGMENTDLG_H__82F48CD1_E148_4DFC_AA91_498AC9678CF1__INCLUDED_)
#define AFX_HISTSEGMENTDLG_H__82F48CD1_E148_4DFC_AA91_498AC9678CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistSegmentDlg.h : header file
//

#include "ImageDib.h"
/////////////////////////////////////////////////////////////////////////////
// HistSegmentDlg dialog

class HistSegmentDlg : public CDialog
{
// Construction
public:
	HistSegmentDlg(CWnd* pParent = NULL);   // standard constructor

    ImageDib m_himageDib;
	
public:
	//ֱ��ͼ����
	int m_histArray[256];
	
	//���ͼ��ÿ����λ��
	int m_nBitCountOut;
	
	//���ͼ��λͼ����ָ��
	unsigned char * m_pImgDataOut;
	
	//���ͼ����ɫ��
	LPRGBQUAD m_lpColorTableOut;

	//���ͼ��Ŀ�����Ϊ��λ
	int m_imgWidthOut;
	
	//���ͼ��ĸߣ�����Ϊ��λ
	int m_imgHeightOut;
	
	//���ͼ����ɫ����
	int m_nColorTableLengthOut;
	
public:
	//������Ϊ��λ�������ͼ��Ŀ�͸�
	CSize GetDimensions();
	
	//ͳ��ֱ��ͼ
	void computeHistGray();
	
	//��ɫͼ������ֱ��ͼ
	void computeHistBrightness();

	//��������
	~HistSegmentDlg();
// Dialog Data
	//{{AFX_DATA(HistSegmentDlg)
	enum { IDD = IDD_DIALOG_HistSegment };
	int		m_threshchange;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HistSegmentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HistSegmentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTSEGMENTDLG_H__82F48CD1_E148_4DFC_AA91_498AC9678CF1__INCLUDED_)
