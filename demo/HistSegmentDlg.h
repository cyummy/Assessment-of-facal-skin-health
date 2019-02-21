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
	//直方图数组
	int m_histArray[256];
	
	//输出图像每像素位数
	int m_nBitCountOut;
	
	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;
	
	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;

	//输出图像的宽，像素为单位
	int m_imgWidthOut;
	
	//输出图像的高，像素为单位
	int m_imgHeightOut;
	
	//输出图像颜色表长度
	int m_nColorTableLengthOut;
	
public:
	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();
	
	//统计直方图
	void computeHistGray();
	
	//彩色图像亮度直方图
	void computeHistBrightness();

	//析构函数
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
