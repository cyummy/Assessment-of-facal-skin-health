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
	//ֱ��ͼ����
	int m_histArray[256];
	
	//ֱ��ͼƽ��ֵ
	float m_average;
	
	//ֱ��ͼ����
	float m_deviation;
	
	//���ͼ��ÿ����λ��
	int m_nBitCountOut;
	
	//���ͼ��λͼ����ָ��
	unsigned char * m_pImgDataOut;
	
	//���ͼ����ɫ��
	LPRGBQUAD m_lpColorTableOut;
	
private:
	//���ͼ��Ŀ�����Ϊ��λ
	int m_imgWidthOut;
	
	//���ͼ��ĸߣ�����Ϊ��λ
	int m_imgHeightOut;
	
	//���ͼ����ɫ����
	int m_nColorTableLengthOut;
	
public:
	void histogramAve();
	
	//������Ϊ��λ�������ͼ��Ŀ�͸�
	CSize GetDimensions();
	
	//ͳ��ֱ��ͼ
	void computeHistGray();
	
	//��ɫͼ������ֱ��ͼ
	void computeHistBrightness();
	
	//����ֱ��ͼ��ֵ
	float computeAverage();
	
	//����ֱ��ͼ����
	float computeDeviation();

	
	//��������
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
