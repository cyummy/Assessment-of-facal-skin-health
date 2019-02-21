// demo1View.h : interface of the CDemo1View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMO1VIEW_H__22279905_D13D_47FC_A210_36D8D93C09EF__INCLUDED_)
#define AFX_DEMO1VIEW_H__22279905_D13D_47FC_A210_36D8D93C09EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDemo1View : public CScrollView
{
protected: // create from serialization only
	CDemo1View();
	DECLARE_DYNCREATE(CDemo1View)

// Attributes
public:
	CDemo1Doc* GetDocument();
    //int image_initial[2000][2000][3];
	//int image_green[2000][2000][3];
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo1View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemo1View();
	void CDemo1View::OnInitialUpdate();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemo1View)
	afx_msg void OnPot();
	afx_msg void OnWrinkle();
	afx_msg void OnMove();
	afx_msg void OnHorizontalMirror();
	afx_msg void OnVerticalMirror();
	afx_msg void OnZoom();
	afx_msg void OnClockwise90();
	afx_msg void OnAnticlockwise90();
	afx_msg void OnRotate180();
	afx_msg void OnFreeRotate();
	afx_msg void OnBinary();
	afx_msg void OnHistogramDraw();
	afx_msg void OnHistgramAver();
	afx_msg void OnImageReverse();
	afx_msg void OnImgThresh();
	afx_msg void OnThresholdWindow();
	afx_msg void OnLinearStrecth();
	afx_msg void OnPepperSaltNoise();
	afx_msg void OnGaussNoise();
	afx_msg void OnChooseMaskSmooth();
	afx_msg void OnGradeSharp();
	afx_msg void OnNeiAveSmooth();
	afx_msg void OnMedianSmooth();
	afx_msg void OnLaplaceSharp();
	afx_msg void OnQuickFFt();
	afx_msg void OnQuickFFtReverse();
	afx_msg void OnIdealLowPass();
	afx_msg void OnButterLowPass();
	afx_msg void OnIdealHighPass();
	afx_msg void OnButterHighPass();
	afx_msg void OnHarrWaveletTrans();
	afx_msg void OnHarrWavRevTrans();
	afx_msg void OnErosion();
	afx_msg void OnDilation();
	afx_msg void OnOpen();
	afx_msg void OnClose();
	afx_msg void OnThinning();
	afx_msg void OnRobert();
	afx_msg void OnSobel();
	afx_msg void OnPrewitt();
	afx_msg void OnLaplacian();
	afx_msg void OnRegionGrow();
	afx_msg void OnEdgeTrace();
	afx_msg void OnAdaptThresholdSeg();
	afx_msg void OnHistThreshSeg();
	afx_msg void Onsimilardegree();
	afx_msg void Onboundary();
	afx_msg void Onfacelocate();
	afx_msg void Oninnerboundary();
	afx_msg void Oneyelocate();
	afx_msg void Onnoselocate();
	afx_msg void Onmouthlocate();
	afx_msg void Onleftfacelocate();
	afx_msg void Onrightfacelocate();
    afx_msg void OnHomomorphicfilter();
	afx_msg void OnAdaptivethresegment();
	afx_msg void OnDWThomomorphicfilter();
	afx_msg void OnGaryImage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//判断当前输入二值形态学模板是否有效
	bool ValidBinaryMorphMask(int *maskBuf, int maskW, int maskH);
	CPoint m_LeftEye;
	CPoint m_LeftEyeLeftCorner;
	CPoint m_LeftEyeRightCorner;
	CPoint m_RightEye;
	CPoint m_RightEyeLeftCorner;
	CPoint m_RightEyeRightCorner;
	CPoint m_LeftNostril;
	CPoint m_RightNostril;
	CPoint m_LeftMouthCorner;
	CPoint m_RightMouthCorner;
	CPoint m_MidMouth;
	CPoint m_MidNose;
};

#ifndef _DEBUG  // debug version in demo1View.cpp
inline CDemo1Doc* CDemo1View::GetDocument()
   { return (CDemo1Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO1VIEW_H__22279905_D13D_47FC_A210_36D8D93C09EF__INCLUDED_)
