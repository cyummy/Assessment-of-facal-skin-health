#if !defined(AFX_HISTOGRAMDRAWDLG_H__2CABD0F5_E9B6_4128_9979_64C4DAE89356__INCLUDED_)
#define AFX_HISTOGRAMDRAWDLG_H__2CABD0F5_E9B6_4128_9979_64C4DAE89356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"
// HistogramDrawDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// HistogramDrawDlg dialog

class HistogramDrawDlg : public CDialog
{
// Construction
public:
	HistogramDrawDlg(CWnd* pParent = NULL);   // standard constructor
    ImageDib m_himageDib;

public:
	//直方图数组
	int m_histArray[256];
	
	//直方图平均值
	float m_average;
	
	//直方图方差
	float m_deviation;
	
	//输出图像每像素位数
	int m_nBitCountOut;
	
	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;
	
	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;
	
private:
	//输出图像的宽，像素为单位
	int m_imgWidthOut;
	
	//输出图像的高，像素为单位
	int m_imgHeightOut;
	
	//输出图像颜色表长度
	int m_nColorTableLengthOut;
	
public:
	void histogramAve();
	
	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();
	
	//统计直方图
	void computeHistGray();
	
	//彩色图像亮度直方图
	void computeHistBrightness();
	
	//计算直方图均值
	float computeAverage();
	
	//计算直方图方差
	float computeDeviation();

	
	//析构函数
	~HistogramDrawDlg();
// Dialog Data
	//{{AFX_DATA(HistogramDrawDlg)
	enum { IDD = IDD_DIALOG_Histogram };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HistogramDrawDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HistogramDrawDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOGRAMDRAWDLG_H__2CABD0F5_E9B6_4128_9979_64C4DAE89356__INCLUDED_)
