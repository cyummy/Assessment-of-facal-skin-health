// demo1View.cpp : implementation of the CDemo1View class
//

#include "math.h"
#include "stdafx.h"
#include "demo1.h"

#include "demo1Doc.h"
#include "demo1View.h"
#include "MainFrm.h"
#include "ImageDib.h"
#include "Detect.h"//检测
#include "GeometryTrans.h"//几何变换类
#include "GeometryMoveDlg.h"//平移参数设置
#include "ZoomDlg.h"   //缩放参数设置
#include "RotateDlg.h"//旋转角度设置
#include "GrayTrans.h" //灰度变换类
#include "HistogramDrawDlg.h"//直方图显示
#include "ThreshStrechDlg.h"//阈值设置
#include "StrechWindowDlg.h"//窗口变换参数设置
#include "LinearStepParamDlg.h"//分段线性拉伸参数设置
#include "ImageEnhance.h"//图像空域增强类
#include "NeiAverSmoothDlg.h"//图像邻域平均模版设置
#include "MedianSmoothDlg.h"//图像中值平均模板设置
#include "FourierTrans.h"//傅立叶变换类
#include "ImageFreqEnhance.h"//图像频域增强类
#include "WaveletTrans.h"//小波变换类
#include "Morphology.h"//图像形态学类
#include "ImgSegment.h"//图像分割类
#include "HistSegmentDlg.h"//图像直方图分割对话框
#include "FaceDetect.h"
#include "Register.h"

//全局变量
FourierTrans fftTrans;
WaveletTrans wavltTrans;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "ImageDib.h"
/////////////////////////////////////////////////////////////////////////////
// CDemo1View

IMPLEMENT_DYNCREATE(CDemo1View, CScrollView)

BEGIN_MESSAGE_MAP(CDemo1View, CScrollView)
	//{{AFX_MSG_MAP(CDemo1View)
    ON_COMMAND(id_Pot,OnPot)
	ON_COMMAND(id_Wrinkle,OnWrinkle)
	ON_COMMAND(id_Move, OnMove)
	ON_COMMAND(id_HorizontalMirror, OnHorizontalMirror)
	ON_COMMAND(id_VerticalMirror, OnVerticalMirror)
	ON_COMMAND(id_Zoom, OnZoom)
	ON_COMMAND(id_Clockwise90, OnClockwise90)
	ON_COMMAND(id_Anticlockwise90, OnAnticlockwise90)
	ON_COMMAND(id_Rotate180, OnRotate180)
	ON_COMMAND(id_FreeRotate, OnFreeRotate)
	ON_COMMAND(id_Binary, OnBinary)
	ON_COMMAND(id_HistogramDraw, OnHistogramDraw)
	ON_COMMAND(id_HistgramAver, OnHistgramAver)
	ON_COMMAND(id_ImageReverse, OnImageReverse)
	ON_COMMAND(id_ImgThresh, OnImgThresh)
	ON_COMMAND(id_ThresholdWindow, OnThresholdWindow)
	ON_COMMAND(id_LinearStrecth, OnLinearStrecth)
	ON_COMMAND(id_PepperSaltNoise, OnPepperSaltNoise)
	ON_COMMAND(id_GaussNoise, OnGaussNoise)
	ON_COMMAND(id_ChooseMaskSmooth, OnChooseMaskSmooth)
	ON_COMMAND(id_GradeSharp, OnGradeSharp)
	ON_COMMAND(id_NeiAveSmooth, OnNeiAveSmooth)
	ON_COMMAND(id_MedianSmooth, OnMedianSmooth)
	ON_COMMAND(id_LaplaceSharp, OnLaplaceSharp)
	ON_COMMAND(id_QuickFFt, OnQuickFFt)
	ON_COMMAND(id_QuickFFt_Reverse, OnQuickFFtReverse)
	ON_COMMAND(id_IdealLowPass, OnIdealLowPass)
	ON_COMMAND(id_ButterLowPass, OnButterLowPass)
	ON_COMMAND(id_IdealHighPass, OnIdealHighPass)
	ON_COMMAND(id_ButterHighPass, OnButterHighPass)
	ON_COMMAND(id_HarrWaveletTrans, OnHarrWaveletTrans)
	ON_COMMAND(id_HarrWavRevTrans, OnHarrWavRevTrans)
	ON_COMMAND(id_Erosion, OnErosion)
	ON_COMMAND(id_Dilation, OnDilation)
	ON_COMMAND(id_Open, OnOpen)
	ON_COMMAND(id_Close, OnClose)
	ON_COMMAND(id_Thinning, OnThinning)
	ON_COMMAND(id_Robert, OnRobert)
	ON_COMMAND(id_Sobel, OnSobel)
	ON_COMMAND(id_Prewitt, OnPrewitt)
	ON_COMMAND(id_Laplacian, OnLaplacian)
	ON_COMMAND(id_RegionGrow, OnRegionGrow)
	ON_COMMAND(id_EdgeTrace, OnEdgeTrace)
	ON_COMMAND(id_AdaptiveThreshold, OnAdaptThresholdSeg)
	ON_COMMAND(id_HistThreshSeg, OnHistThreshSeg)
	ON_COMMAND(id_similardegree, Onsimilardegree)
	ON_COMMAND(id_boundary, Onboundary)
	ON_COMMAND(id_facelocate, Onfacelocate)
	ON_COMMAND(id_innerboundary, Oninnerboundary)
	ON_COMMAND(id_eyelocate, Oneyelocate)
	ON_COMMAND(id_noselocate, Onnoselocate)
	ON_COMMAND(id_mouthlocate, Onmouthlocate)
	ON_COMMAND(id_leftfacelocate, Onleftfacelocate)
	ON_COMMAND(id_rightfacelocate, Onrightfacelocate)
    ON_COMMAND(id_Homomorphicfilter,OnHomomorphicfilter)
	ON_COMMAND(id_Adaptivethresegment, OnAdaptivethresegment)
	ON_COMMAND(id_DWThomomorphicfilter, OnDWThomomorphicfilter)
	ON_COMMAND(id_GaryImage, OnGaryImage)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo1View construction/destruction

void CDemo1View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize( 1600, 1600 ) );
}

/*   void CDemo1View::OnZw(){
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	Detect detect(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	double n=detect.ZwJc();
	CString str;
	str.Format("%s%.3lf","粗糙程度为：",n);
	MessageBox(str);
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(detect.GetDimensions(),detect.m_nBitCountOut,detect.m_lpColorTableOut, detect.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}*/


void CDemo1View::OnPot() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据

/*    CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义Detect类的对象detect
	Detect detect(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	detect.Pot();
	
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(detect.GetDimensions(),detect.m_nBitCountOut,detect.m_lpColorTableOut, detect.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);*/





	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	Detect detect(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	int n=detect.Pot();
	extern a,b,ccc,dd,e,ff,gg,h,zz;
	CString str;
	str.Format("%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d","总坑点数为：",n,"15~25之间的坑点个数为：",a,"25~35之间的坑点个数为：",b,"35~50之间的坑点个数为：",ccc,"50~60之间的坑点个数为：",dd,"60~70之间的坑点个数为：",e,"70~80之间的坑点个数为：",ff,"80~90之间的坑点个数为：",gg,"90~100之间的坑点个数为：",h);
	//str.Format("%s%d","斑点数为：",n);
	MessageBox(str);

	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(detect.GetDimensions(),detect.m_nBitCountOut,detect.m_lpColorTableOut, detect.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);


}

void CDemo1View::OnWrinkle() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义Detect类的对象detect
	Detect detect(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	Detect::zw s=detect.Wrinkle();
	CString str;
	//str.Format("%s%.3lf\n%s%.3lf\n%s%.3lf\n%s%.3lf","角二阶矩: ",s.asmm,"熵: ",s.ent,"对比度: ",s.con,"局部平稳性: ",s.idm);
	str.Format("%s%.3lf","R: ",s.ent/s.asmm);
	MessageBox(str);
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(detect.GetDimensions(),detect.m_nBitCountOut,detect.m_lpColorTableOut, detect.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}
//0.162 2.293 0.499 0.903
CDemo1View::CDemo1View()
{
	// TODO: add construction code here

}

CDemo1View::~CDemo1View()
{
}

BOOL CDemo1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDemo1View drawing
/**/
void CDemo1View::OnDraw(CDC* pDC)
{
	CDemo1Doc* pDoc = GetDocument();			//获取文档类指针
	ASSERT(pDoc != NULL);
	ImageDib* pDib = pDoc->m_dib;					//返回m_dib的指针
	
	pDib->Draw(pDC, CPoint(0, 0), pDib->GetDimensions()); 	//显示DIB
}


/////////////////////////////////////////////////////////////////////////////
// CDemo1View printing

BOOL CDemo1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDemo1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDemo1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDemo1View diagnostics

#ifdef _DEBUG
void CDemo1View::AssertValid() const
{
	CView::AssertValid();
}

void CDemo1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo1Doc* CDemo1View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo1Doc)));
	return (CDemo1Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemo1View message handlers

void CDemo1View::OnMove() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc = GetDocument();
	ImageDib *pDib = pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8 && pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//平移参数设置对话框
	GeometryMoveDlg dlgPara;
	if(dlgPara.DoModal()==IDOK)
	{
		//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
		GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//将dlgPara.m_Xmove, dlgPara.m_Ymove作为平移量，对图像平移
		geoTrans.Move(dlgPara.m_Xmove, dlgPara.m_Ymove);

		
/*	*/	//新建视图窗口，多文档显示变换结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();		
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);

//      单文档显示变换结果
/*
		CClientDC dc(this);
		ImageDib *dibNew = new ImageDib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),
			geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		dibNew->Draw(&dc,CPoint(0,0),dibNew->GetDimensions());
		pDib->Draw(&dc,CPoint(dibNew->GetDimensions().cx,0),pDib->GetDimensions());
 */
/*
		pDib->ReplaceDib(geoTrans.GetDimensions(),
			geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		Invalidate();
*/
	}	
}

void CDemo1View::OnHorizontalMirror() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//水平镜像
	geoTrans.MirrorHorTrans();
   //显示变换结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();		
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);

}

void CDemo1View::OnVerticalMirror() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//垂直镜像
	geoTrans.MirrorVerTrans();
   //显示变换结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();		
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
}

void CDemo1View::OnZoom() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//原图像尺寸
	CSize oldImgSize=pDib->GetDimensions();
	
	//缩放参数设置对话框
	ZoomDlg dlgPara;
	
	if(dlgPara.DoModal()==IDOK){
		//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
		GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//根据指定的缩放比例对对原图像进行缩放缩放
		geoTrans.Zoom(dlgPara.m_XstrechRatio,dlgPara.m_YstrechRatio);	
		
		//新建视图窗口，显示处理结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
			geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
		Invalidate();
	}
}

void CDemo1View::OnClockwise90() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//顺时针90度
	geoTrans.Clockwise90();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemo1View::OnAnticlockwise90() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//逆时针90度
	geoTrans.Anticlockwise90();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemo1View::OnRotate180() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//旋转180度
	geoTrans.Rotate180();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemo1View::OnFreeRotate() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	//旋转参数设置对话框
	RotateDlg dlgPara;
	if(dlgPara.DoModal()==IDOK){
		//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
		GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//调用Rotate函数进行图像旋转，将角度和插值方式参数传进函数
		geoTrans.Rotate(dlgPara.m_RotateAngle);
		
		//新建视图窗口，显示处理结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
			geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}


void CDemo1View::OnBinary() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.BinaryImage();
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemo1View::OnHistogramDraw() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//直方图绘制对话框
	HistogramDrawDlg dlgHist;
	dlgHist.m_himageDib.ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//显示直方图
   dlgHist.DoModal();
}


void CDemo1View::OnHistgramAver() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	HistogramDrawDlg histodrawdlg1; 
	histodrawdlg1.m_himageDib.ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//直方图均衡
	histodrawdlg1.histogramAve();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(histodrawdlg1.GetDimensions(),histodrawdlg1.m_nBitCountOut,histodrawdlg1.m_lpColorTableOut, histodrawdlg1.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemo1View::OnImageReverse() 
{
	// TODO: Add your command handler code here
	//获取文档类指针
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//DIB类型判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	GrayTrans graytrans(pDib->GetDimensions(),
		pDib->m_nBitCount,pDib->m_lpColorTable,pDib->m_pImgData);
	
	//调用ReverseImg()对图像求反
	graytrans.RevImage();
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemo1View::OnImgThresh() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//阈值分割对话框类对象threshsetdlg
	ThreshStrechDlg threshsetdlg;
	if(threshsetdlg.DoModal()==IDOK)
	{
		//定义灰度变换类的对象graytrans，并将当前DIB数据作为输入数据传入graytrans中
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//根据用户选择的阈值，调用二值化函数分割图像
		if ((threshsetdlg.m_Thresh>=0)&&(threshsetdlg.m_Thresh<=255)) 	
			graytrans.BinaryImage(threshsetdlg.m_Thresh);
		
        else
		{
			AfxMessageBox("请输入一个0-255之间的阈值！");
		    return;
		}
			//新建视图，显示分割结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemo1View::OnThresholdWindow() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//阈值分割对话框类对象threshsetdlg
	StrechWindowDlg  strechwinsetdlg;
	if(strechwinsetdlg.DoModal()==IDOK)
	{
		//定义灰度变换类的对象graytrans，并将当前DIB数据作为输入数据传入graytrans中
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		graytrans.ThresholdWindow(strechwinsetdlg.m_nBottom,strechwinsetdlg.m_nTop);
	  
	   	//新建视图，显示分割结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemo1View::OnLinearStrecth() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//线性拉伸参数设置对话框
	LinearStepParamDlg linesdlgPara;
	if(linesdlgPara.DoModal()==IDOK){
		//获取用户设置的两个拉伸转折点
		CPoint point1,point2;
		point1.x=linesdlgPara.m_nX1;
		point1.y=linesdlgPara.m_nY1;
		point2.x =linesdlgPara.m_nX2;
		point2.y=linesdlgPara.m_nY2;
		
		//判断参数的合理性
		if(point1.x<0||point1.x>255||point2.x<0||point2.x>255
			||point1.y<0||point1.y>255||point2.y<0||point2.y>255
			||point1.x>point2.x){
			::MessageBox(0,"参数不合理",MB_OK,0);
			return;
		}
		
		//定义灰度变换的类对象
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//线性拉伸
		graytrans.LinearStrech(point1,point2);
		
		//新建窗口显示
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);		
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;	
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();	
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemo1View::OnPepperSaltNoise() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);
	imgnoise.AddPepperSaltNoise();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}


void CDemo1View::OnGaussNoise() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);
	imgnoise.AddGaussNoise();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnChooseMaskSmooth() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgSmooth(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	imgSmooth.ChooseMaskSmooth();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgSmooth.GetDimensions(),imgSmooth.m_nBitCountOut,imgSmooth.m_lpColorTable, imgSmooth.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);

}


void CDemo1View::OnGradeSharp() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgsharp(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	int nThresh=30;
	imgsharp.GradeSharp(nThresh);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	
	dibNew->ReplaceDib(imgsharp.GetDimensions(),imgsharp.m_nBitCountOut,imgsharp.m_lpColorTable, imgsharp.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
}

void CDemo1View::OnNeiAveSmooth() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	ImageEnhance imgsmooth(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	NeiAverSmoothDlg dlgparam;
	
	if (dlgparam.DoModal()==IDOK) {
        
		if (dlgparam.m_radiovalue==0||dlgparam.m_radiovalue==1||dlgparam.m_radiovalue==2) {
	
        float Temp1[81];
		int i;
		for (i = 0; i < 81; i++)
		{
			Temp1[i]=1.0;
		}

           imgsmooth.NeiAveTemplate(dlgparam.TempH,dlgparam.TempW,dlgparam.TempCX,
	       dlgparam.TempCY,Temp1,dlgparam.fCoef);
		}
		else if (dlgparam.m_radiovalue=3) {
		   	float Temp[25];
			//设模板为平均模板
			Temp[0]=0.0;
			Temp[1]=1.0;
			Temp[2]=2.0;
			Temp[3]=1.0;
			Temp[4]=0.0;
			
			Temp[5]=1.0;
			Temp[6]=2.0;
			Temp[7]=4.0;
			Temp[8]=2.0;
			Temp[9]=1.0;
			
			Temp[10]=2.0;
			Temp[11]=4.0;
			Temp[12]=8.0;
			Temp[13]=4.0;
			Temp[14]=2.0;
			
			Temp[15]=1.0;
			Temp[16]=2.0;
			Temp[17]=4.0;
			Temp[18]=2.0;
			Temp[19]=1.0;
			
			Temp[20]=0.0;
			Temp[21]=1.0;
			Temp[22]=2.0;
			Temp[23]=1.0;
	        Temp[24]=0.0;
			imgsmooth.NeiAveTemplate(dlgparam.TempH,dlgparam.TempW,dlgparam.TempCX,
	       dlgparam.TempCY,Temp,dlgparam.fCoef); 

		} 
		else
			return;
		
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgsmooth.GetDimensions(),imgsmooth.m_nBitCountOut,imgsmooth.m_lpColorTable, imgsmooth.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
	}
}


void CDemo1View::OnMedianSmooth() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	MedianSmoothDlg dlgparam;
    if (dlgparam.DoModal()==TRUE) {
	
	ImageEnhance imgsmooth(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	imgsmooth.MedianSmooth(dlgparam.TempH,dlgparam.TempW,dlgparam.TempCX,dlgparam.TempCY);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgsmooth.GetDimensions(),imgsmooth.m_nBitCountOut,imgsmooth.m_lpColorTable, imgsmooth.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	}
}

void CDemo1View::OnLaplaceSharp() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgsharp(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	int TempH, TempW, TempCX, TempCY;
	float fCoef;
	float Temp[9];
	//设模板为平均模板
	Temp[0]=-1.0;
	Temp[1]=-1.0;
	Temp[2]=-1.0;
	Temp[3]=-1.0;
	Temp[4]=9.0;
	Temp[5]=-1.0;
	Temp[6]=-1.0;
	Temp[7]=-1.0;
	Temp[8]=-1.0;
    //初始化对话框变量
	TempH=3;
	TempW=3;
	TempCX=1;
	TempCY=1;
	fCoef=1.0;
    imgsharp.LapTemplate(TempH,TempW,TempCX,TempCY,Temp,fCoef);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgsharp.GetDimensions(),imgsharp.m_nBitCountOut,imgsharp.m_lpColorTable, imgsharp.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnQuickFFt() 
{
	// TODO: Add your command handler code here
		// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给transform_FFT类对象FFTTrans
	fftTrans.InputImgData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//快速傅立叶变换
	fftTrans.Fourier();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(fftTrans.GetDimensions(),fftTrans.m_nBitCountOut,
		fftTrans.m_lpColorTable, fftTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}


void CDemo1View::OnQuickFFtReverse() 
{
	// TODO: Add your command handler code here
	fftTrans.RevFourier();
    //新建视图窗口，显示图像恢复的结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(fftTrans.GetDimensions(),fftTrans.m_nBitCountOut,
		fftTrans.m_lpColorTable, fftTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}



void CDemo1View::OnIdealLowPass() 
{
	// TODO: Add your command handler code here
    CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//图像理想低通滤波
	imgfreq.IdealLowPassFilter(pDib->m_imgWidth,pDib->m_imgHeight,pDib->m_imgHeight/16);
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgfreq.GetDimensions(),imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
	
}

void CDemo1View::OnButterLowPass() 
{
	// TODO: Add your command handler code here
    CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//图像理想低通滤波
	imgfreq.ButterLowPassFilter(pDib->m_imgWidth,pDib->m_imgHeight,pDib->m_imgHeight/16);
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgfreq.GetDimensions(),imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnIdealHighPass() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//图像理想低通滤波
	imgfreq.IdealHighPassFilter(pDib->m_imgWidth,pDib->m_imgHeight,pDib->m_imgHeight/16);
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgfreq.GetDimensions(),imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnButterHighPass() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理彩色图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//图像理想低通滤波
	imgfreq.ButterHighPassFilter(pDib->m_imgWidth,pDib->m_imgHeight,pDib->m_imgHeight/16);
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgfreq.GetDimensions(),imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnHarrWaveletTrans() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	wavltTrans.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    wavltTrans.BasicWaveletTrans(3);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(wavltTrans.GetDimensions(),wavltTrans.m_nBitCountOut,wavltTrans.m_pColorTableOut,wavltTrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnHarrWavRevTrans() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
    wavltTrans.BasicWaveletReverse();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(wavltTrans.GetDimensions(),wavltTrans.m_nBitCountOut,wavltTrans.m_pColorTableOut,wavltTrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnErosion() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;

	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
   
	//定义结构元素模板，默认为3*3模板
/*
    int i;
	for (i=0;i<TempH*TempW;i++) {
		m_InitTemp[i]=0;
	}	
*/
	//模板1
	morTrans.m_InitTemp[1]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[5]=1;
	morTrans.m_InitTemp[7]=1;
	
/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if(morTrans.m_TempBuf){
		delete []morTrans.m_TempBuf;
		morTrans.m_TempBuf=NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf=new int[morTrans.TempW*morTrans.TempH];
	int i0, j0;
	for(i0=0;i0<morTrans.TempH;i0++){
		for(j0=0;j0<morTrans.TempW;j0++){
			morTrans.m_TempBuf[i0*morTrans.TempW+j0]=morTrans.m_InitTemp[i0*morTrans.TempW+j0];
		}
	}
	
	morTrans.ImgErosion(morTrans.m_pImgData,morTrans.m_pImgDataOut,morTrans.m_imgWidth,morTrans.m_imgHeight,
		morTrans.m_TempBuf, morTrans.TempW, morTrans.TempH);
/*	*/
	//显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnDilation() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;

	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
   
	//定义结构元素模板，默认为3*3模板
/*
    int i;
	for (i=0;i<TempH*TempW;i++) {
		m_InitTemp[i]=0;
	}	
*/
	//模板1
	morTrans.m_InitTemp[1]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[5]=1;
	morTrans.m_InitTemp[7]=1;
	
/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if(morTrans.m_TempBuf){
		delete []morTrans.m_TempBuf;
		morTrans.m_TempBuf=NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf=new int[morTrans.TempW*morTrans.TempH];
	int i0, j0;
	for(i0=0;i0<morTrans.TempH;i0++){
		for(j0=0;j0<morTrans.TempW;j0++){
			morTrans.m_TempBuf[i0*morTrans.TempW+j0]=morTrans.m_InitTemp[i0*morTrans.TempW+j0];
		}
	}
	
	morTrans.ImgDilation(morTrans.m_pImgData,morTrans.m_pImgDataOut,morTrans.m_imgWidth,morTrans.m_imgHeight,
		morTrans.m_TempBuf, morTrans.TempW, morTrans.TempH);
/*	*/
	//显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnOpen() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;

	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
   
	//定义结构元素模板，默认为3*3模板
/*
    int i;
	for (i=0;i<TempH*TempW;i++) {
		m_InitTemp[i]=0;
	}	
*/
	//模板1
	morTrans.m_InitTemp[1]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[5]=1;
	morTrans.m_InitTemp[7]=1;
	
/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if(morTrans.m_TempBuf){
		delete []morTrans.m_TempBuf;
		morTrans.m_TempBuf=NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf=new int[morTrans.TempW*morTrans.TempH];
	int i0, j0;
	for(i0=0;i0<morTrans.TempH;i0++){
		for(j0=0;j0<morTrans.TempW;j0++){
			morTrans.m_TempBuf[i0*morTrans.TempW+j0]=morTrans.m_InitTemp[i0*morTrans.TempW+j0];
		}
	}
	
	morTrans.Open();
	
	/*	*/
	//显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnClose() 
{

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//获取新建视图指针
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	pFrame->MDINext();
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	

	//获取相关联的新的文档类指针
	CDemo1Doc* pDoc1=pView1->GetDocument();
	CDemo1Doc* pDoc2=pView2->GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1=pDoc1->GetPDib();
	ImageDib *pdib2=pDoc2->GetPDib();
	
	ImageDib *pdibtemp;
	
	//处理一幅彩色和一幅二值图像
	if(pdib1->m_nBitCount==24){
		if(pdib2->m_nBitCount==24){
			::MessageBox(0,"需要一幅二值图像",MB_OK,0);
			return ;
		}
	}
	else{
		if(pdib2->m_nBitCount==24){
			pdibtemp=pdib1;
			pdib1=pdib2;
			pdib2=pdibtemp;
		}
		else{
			::MessageBox(0,"需要一幅彩色图像",MB_OK,0);
			return ;
		}
	}

	CSize size=pdib1->GetDimensions();
	int lineByte=(size.cx+3)/4*4;  //灰度图像
	int lineByte1=(size.cx*3+3)/4*4;  //彩色图像


    int i,j;
	int right,left,top,bottom;
	for(i=0;i<size.cy;i++){
		for(j=0;j<size.cx;j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;			
		}
		if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		left=j+1;
		top=i+1;
		
    
	for(i=size.cy-1;i>=0;i--){
		for(j=size.cx-1;j>=0;j--){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;		
		}
        if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		right=j-1;
		bottom=i-1;


	
	//判断是否二值图像

	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pdib2->m_pImgData+i*lineByte+j)!=0
				&&*(pdib2->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pdib2->GetDimensions(),pdib2->m_nBitCount,
		pdib2->m_lpColorTable, pdib2->m_pImgData);
   
	//定义结构元素模板，默认为3*3模板

	//模板1
	morTrans.m_InitTemp[1]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[5]=1;
	morTrans.m_InitTemp[7]=1;
	


	if(morTrans.m_TempBuf){
		delete []morTrans.m_TempBuf;
		morTrans.m_TempBuf=NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf=new int[morTrans.TempW*morTrans.TempH];
	int i0, j0;
	for(i0=0;i0<morTrans.TempH;i0++){
		for(j0=0;j0<morTrans.TempW;j0++){
			morTrans.m_TempBuf[i0*morTrans.TempW+j0]=morTrans.m_InitTemp[i0*morTrans.TempW+j0];
		}
	}
	
	morTrans.Close();
	
	/*	*/
	//显示变换结果
//	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
	



	int NUM=(bottom-top-3)*(right-left-3);
	int B=0,n;
    for(i=top+2;i<bottom-1;i++){
		for(j=left+2;j<right-1;j++){
			if(*(dibNew->m_pImgData+i*lineByte+j)==0)      //?????
		    	B++;
			
		}
	}
    n=(float)B/NUM*100;
	CString str;
	str.Format("%s%d%s","坑点百分比为：",n,"%");
	MessageBox(str);

}




void CDemo1View::OnThinning() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}
	
	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    morTrans.ImgThinning();

	/*	*/
	//显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnRobert() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Roberts算子
	segment.Roberts();
	
	//新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();
}

void CDemo1View::OnSobel() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Sobel算子
	segment.Sobel();
	
	//新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();	
}

void CDemo1View::OnPrewitt() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Prewitt算子
	segment.Prewitt();
	
	//新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();	
}

void CDemo1View::OnLaplacian() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Laplacian算子
	segment.Laplacian();
	
	//新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();	
}

void CDemo1View::OnRegionGrow() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
    
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//设定种子点位置
	CPoint Pos(segment.m_imgWidth/2,segment.m_imgHeight/2);
	
	//阈值
	int nThresh=3;

	segment.RegionGrow(Pos,nThresh);
	
    //新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnEdgeTrace() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}
	//定义图像分割类对象segment，用当前DIB对其初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//边界跟踪
	segment.EdgeTrace();
	
	//新建视图，显示处理结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();
}

void CDemo1View::OnAdaptThresholdSeg() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度",MB_OK,0);
		return ;
	}
	//定义图像分割类对象segment，用当前DIB对其初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//自适应阈值分割
	segment.AdaptThreshSeg(pDib->m_pImgData);
	
	//新建视图，显示处理结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();

}

void CDemo1View::OnHistThreshSeg() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//直方图绘制对话框
	HistSegmentDlg dlgHist;
	dlgHist.m_himageDib.ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//显示直方图
    if (dlgHist.DoModal()==IDOK) {
	  GrayTrans gray(dlgHist.m_himageDib.GetDimensions(),dlgHist.m_himageDib.m_nBitCount,
		  dlgHist.m_himageDib.m_lpColorTable, dlgHist.m_himageDib.m_pImgData);
      gray.BinaryImage(dlgHist.m_threshchange);
	  //新建视图，显示处理结果
	  CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	  pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	  CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	  CDemo1Doc* pDocNew=pView->GetDocument();
	  ImageDib *dibNew=pDocNew->m_dib;
	  dibNew->ReplaceDib(gray.GetDimensions(),gray.m_nBitCountOut,gray.m_lpColorTableOut, gray.m_pImgDataOut);
	  pView->OnInitialUpdate();
	  pDocNew->SetModifiedFlag(TRUE);
	  pDocNew->UpdateAllViews(pView);	
      Invalidate();

  } 
  else
	  return;

}

void CDemo1View::Onsimilardegree() 
{
		//获取文档类指针
	CDemo1Doc *pDoc=GetDocument();
	//获取ImgCenterDib类对象m_dib的指针，访问当前DIB数据
	ImageDib *pDib=pDoc->GetPDib();
	
	//只处理彩色图像
	if(pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理彩色图像",MB_OK,0);
		return ;
	}
	
	
	FaceDetect simcalculation(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData,NULL);
	
	simcalculation.CalSim();
	
	//建立一个新视图，显示结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//发送新建文件的消息，创建一个新的文档-视图
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	//获取新建视图指针
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	
	//获取相关联的新的文档类指针
	CDemo1Doc* pDocNew=pView->GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *dibNew=pDocNew->GetPDib();

	//将变换后的输出图像作为新建文档的DIB进行显示
	dibNew->ReplaceDib(simcalculation.GetDimensions(),simcalculation.m_nBitCountOut,simcalculation.m_lpColorTableOut, simcalculation.m_pImgDataOut);
	
	//设置滚动窗口
	pView->OnInitialUpdate();


	//文档数据置脏，提示存盘信息
	pDocNew->SetModifiedFlag(TRUE);

	//各视图刷新显示
	pDocNew->UpdateAllViews(pView);
	
}

void CDemo1View::Onboundary() 
{
	CDemo1Doc *pDoc=GetDocument();
	ImageDib  *pDib=pDoc->GetPDib();
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}

	int i,j,thd;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	long pixel_scales=0;

	for(i=0;i<size.cx;i++){
		for(j=0;j<size.cy;j++){
			pixel_scales+=*(pDib->m_pImgData+j*lineByte+i);		
		}
	}
	
	thd=pixel_scales/(size.cx*size.cy);

	GrayTrans graytrans(size,pDib->m_nBitCount, pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.BinaryImage(thd);
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemo1View::Onfacelocate() 
{
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//获取新建视图指针
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	pFrame->MDINext();
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	

	//获取相关联的新的文档类指针
	CDemo1Doc* pDoc1=pView1->GetDocument();
	CDemo1Doc* pDoc2=pView2->GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1=pDoc1->GetPDib();
	ImageDib *pdib2=pDoc2->GetPDib();
	
	ImageDib *pdibtemp;
	
	//处理一幅彩色和一幅二值图像
	if(pdib1->m_nBitCount==24){
		if(pdib2->m_nBitCount==24){
			::MessageBox(0,"需要一幅二值图像",MB_OK,0);
			return ;
		}
	}
	else{
		if(pdib2->m_nBitCount==24){
			pdibtemp=pdib1;
			pdib1=pdib2;
			pdib2=pdibtemp;
		}
		else{
			::MessageBox(0,"需要一幅彩色图像",MB_OK,0);
			return ;
		}
	}

	CSize size=pdib1->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int lineByte1=(size.cx*3+3)/4*4;
    
	int i,j;

	double* temp=new double[pdib1->m_imgWidth];
	int count;
	for(i=0;i<pdib1->m_imgWidth;i++){
		count =0;
		for(j=0;j<pdib1->m_imgHeight;j++){
			if(*(pdib2->m_pImgData+j*lineByte+i) == 255)
				count++;
		}
		temp[i]=count;
	}

	int max = 0;
	int pos = -1;
    for(i=0;i<pdib1->m_imgWidth;i++){
		if(temp[i]>max){
			max= temp[i];
			pos = i;
		}
	}
	
	for(i=0;i<pdib1->m_imgWidth;i++){
		temp[i]= temp[i]/max;
	}
	
	int left,right,top,bottom;
	for(i=pos;i>=0;i--){
		if( temp[i]<0.2||i==0){
			left =i;
			break;
		}
	}
    for(i=pos;i<pdib1->m_imgWidth;i++){
		if(temp[i]<0.2||i==pdib1->m_imgWidth-1){
			right =i;
			break;
		}
	}
	
	double* temp1=new double[pdib1->m_imgHeight];
	for(j=0;j<pdib1->m_imgHeight;j++){
		count =0;
		for(i=0;i<pdib1->m_imgWidth;i++){
			if(*(pdib2->m_pImgData+j*lineByte+i) == 255)
				count++;
		}
		temp1[j]=count;
	}

	max = 0;
	pos = -1;
    for(i=0;i<pdib1->m_imgHeight;i++){
		if(temp1[i]>max){
			max= temp1[i];
			pos = i;
		}
	}
	
	for(i=0;i<pdib1->m_imgHeight;i++){
		temp1[i]= temp1[i]/max;
	}

	for(i=pos;i<pdib1->m_imgHeight;i++){
		if( temp1[i]<0.2||i==pdib1->m_imgHeight-1){
			top =i;
			break;
		}
	}
	bottom=top-(right-left)*1.2;           //经校正 左下角为坐标原点
	
    
	//人脸定位框 绿色
	for(i=bottom;i<=top;i++){
		*(pdib1->m_pImgData+i*lineByte1+left*3)=0;
		*(pdib1->m_pImgData+i*lineByte1+left*3+1)=255;
		*(pdib1->m_pImgData+i*lineByte1+left*3+2)=0;
		*(pdib1->m_pImgData+i*lineByte1+right*3)=0;
		*(pdib1->m_pImgData+i*lineByte1+right*3+1)=255;
		*(pdib1->m_pImgData+i*lineByte1+right*3+2)=0;
		*(pdib1->m_pImgData+i*lineByte1+(left+1)*3)=0;
		*(pdib1->m_pImgData+i*lineByte1+(left+1)*3+1)=255;
		*(pdib1->m_pImgData+i*lineByte1+(left+1)*3+2)=0;
		*(pdib1->m_pImgData+i*lineByte1+(right+1)*3)=0;
		*(pdib1->m_pImgData+i*lineByte1+(right+1)*3+1)=255;
		*(pdib1->m_pImgData+i*lineByte1+(right+1)*3+2)=0;
		*(pdib1->m_pImgData+i*lineByte1+(left-1)*3)=0;
		*(pdib1->m_pImgData+i*lineByte1+(left-1)*3+1)=255;
		*(pdib1->m_pImgData+i*lineByte1+(left-1)*3+2)=0;
		*(pdib1->m_pImgData+i*lineByte1+(right-1)*3)=0;
		*(pdib1->m_pImgData+i*lineByte1+(right-1)*3+1)=255;
		*(pdib1->m_pImgData+i*lineByte1+(right-1)*3+2)=0;
	}
	for(j=left;j<=right;j++){
		*(pdib1->m_pImgData+top*lineByte1+j*3)=0;
		*(pdib1->m_pImgData+top*lineByte1+j*3+1)=255;
		*(pdib1->m_pImgData+top*lineByte1+j*3+2)=0;
		*(pdib1->m_pImgData+bottom*lineByte1+j*3)=0;
		*(pdib1->m_pImgData+bottom*lineByte1+j*3+1)=255;
		*(pdib1->m_pImgData+bottom*lineByte1+j*3+2)=0;
		*(pdib1->m_pImgData+(top-1)*lineByte1+j*3)=0;
		*(pdib1->m_pImgData+(top-1)*lineByte1+j*3+1)=255;
		*(pdib1->m_pImgData+(top-1)*lineByte1+j*3+2)=0;
		*(pdib1->m_pImgData+(bottom-1)*lineByte1+j*3)=0;
		*(pdib1->m_pImgData+(bottom-1)*lineByte1+j*3+1)=255;
		*(pdib1->m_pImgData+(bottom-1)*lineByte1+j*3+2)=0;
		*(pdib1->m_pImgData+(top+1)*lineByte1+j*3)=0;
		*(pdib1->m_pImgData+(top+1)*lineByte1+j*3+1)=255;
		*(pdib1->m_pImgData+(top+1)*lineByte1+j*3+2)=0;
		*(pdib1->m_pImgData+(bottom+1)*lineByte1+j*3)=0;
		*(pdib1->m_pImgData+(bottom+1)*lineByte1+j*3+1)=255;
		*(pdib1->m_pImgData+(bottom+1)*lineByte1+j*3+2)=0;
	}


}



/***********************************************************************
* 函数名称：Oninnerboundary()
* 函数参数：无
* 返回值：  无
* 说明： 脸内轮廓提取
***********************************************************************/
void CDemo1View::Oninnerboundary() 
{
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->GetPDib();
	
	//只处理彩色图像
	if(pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理彩色图像",MB_OK,0);
		return ;
	}

	FaceDetect BoundaryDetect(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData,NULL);
    
	CSize size=pDib->GetDimensions();
	int lineByteIn=(size.cx*3+3)/4*4;
    
	//通过遍历整幅图像 获取人脸定位框的边界值
    int i,j,right,left,top,bottom;
	for(i=0;i<size.cy;i++){
		for(j=0;j<size.cx;j++){
			if(*(pDib->m_pImgData+i*lineByteIn+j*3+1)==255&&
			   *(pDib->m_pImgData+i*lineByteIn+j*3+0)==0&&
               *(pDib->m_pImgData+i*lineByteIn+j*3+2)==0)
				break;			
		}
		if(*(pDib->m_pImgData+i*lineByteIn+j*3+1)==255&&
			   *(pDib->m_pImgData+i*lineByteIn+j*3+0)==0&&
               *(pDib->m_pImgData+i*lineByteIn+j*3+2)==0)
		break;
	}
		left=j+1;
		top=i+1;
		
    
	for(i=size.cy-1;i>=0;i--){
		for(j=size.cx-1;j>=0;j--){
			if(*(pDib->m_pImgData+i*lineByteIn+j*3+1)==255&&
			   *(pDib->m_pImgData+i*lineByteIn+j*3+0)==0&&
               *(pDib->m_pImgData+i*lineByteIn+j*3+2)==0)
				break;		
		}
        if(*(pDib->m_pImgData+i*lineByteIn+j*3+1)==255&&
			   *(pDib->m_pImgData+i*lineByteIn+j*3+0)==0&&
               *(pDib->m_pImgData+i*lineByteIn+j*3+2)==0)
		break;
	}
		right=j-1;
		bottom=i-1;
		

  /*CString str;
	str.Format("%s%d\n%s%d\n%s%d\n%s%d\n","top=",top,"bottom=",bottom,"left=",left,"right=",right);//输出以检验结果是否正确
	MessageBox(str); */
	
	BoundaryDetect.CalSBound(top,bottom,left,right);
	
	//建立一个新视图，显示结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(BoundaryDetect.GetDimensions(),BoundaryDetect.m_nBitCountOut,BoundaryDetect.m_lpColorTableOut, BoundaryDetect.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

/***********************************************************************
* 函数名称：Oneyelocate() 
* 函数参数：无
* 返回值：  无
* 说明： 额头定位
***********************************************************************/
void CDemo1View::Oneyelocate() 
{
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//获取新建视图指针
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	pFrame->MDINext();
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();


	//获取相关联的新的文档类指针
	CDemo1Doc* pDoc1=pView1->GetDocument();
	CDemo1Doc* pDoc2=pView2->GetDocument();


	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1=pDoc1->GetPDib();
	ImageDib *pdib2=pDoc2->GetPDib();
	
	ImageDib *pdibtemp;
	
	//处理一幅彩色和一幅二值图像
	if(pdib1->m_nBitCount==24){
		if(pdib2->m_nBitCount==24){
			::MessageBox(0,"需要一幅二值图像",MB_OK,0);
			return ;
		}
	}
	else{
		if(pdib2->m_nBitCount==24){
			pdibtemp=pdib1;
			pdib1=pdib2;
			pdib2=pdibtemp;
		}
		else{
			::MessageBox(0,"需要一幅彩色图像",MB_OK,0);
			return ;
		}
	}
	
	CSize size=pdib1->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int lineByte1=(size.cx*3+3)/4*4;
    
	unsigned char *m_pImgDatabak;
	m_pImgDatabak =new unsigned char[pdib1->m_imgHeight*lineByte1];
	memcpy(m_pImgDatabak,pdib1->m_pImgData,pdib1->m_imgHeight*lineByte1);

	
	int i,j;
	int right,left,top,bottom;
	int F_right,F_left,F_top,F_bottom;
	for(i=0;i<size.cy;i++){
		for(j=0;j<size.cx;j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;			
		}
		if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		left=j+1;
		top=i+1;
		
    
	for(i=size.cy-1;i>=0;i--){
		for(j=size.cx-1;j>=0;j--){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;		
		}
        if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		right=j-1;
		bottom=i-1;

	//人脸中心
	int nCenterH = (right+left)/2;
	int nCenterV = (bottom+top)/2;
	//人脸面积
	int nArea = (right - left)*(bottom - top);
	//设定子窗口大小
	int nSubWinWidth  = (right - left)/12;
	int nSubWinHeight = (bottom - top)/30;
	int nSubWinArea =nSubWinHeight*nSubWinWidth;
	
	//水平投影
	int *temp = new int[pdib1->m_imgWidth]; 
	for(i = 0; i<pdib1->m_imgWidth; i++) 
		temp[i] = 0;
	
	for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
		for(j=left+nSubWinWidth; j<right-nSubWinWidth; j++){
			int count = 0;
			for(int p= -nSubWinHeight ;p<nSubWinHeight;p++)
				for(int q= -nSubWinWidth ;q<nSubWinWidth;q++){
					if(*(pdib2->m_pImgData+(i+p)*lineByte+j+q)==0)	count++;     //??????
			}
			if(count >= nSubWinArea/3){
				*(pdib1->m_pImgData+i*lineByte1+j*3) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+1) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+2) = 255;
				temp[j] ++;
			}
	}
		
	CList<CPoint,CPoint&> myList1(sizeof(CPoint));
	CList<CPoint,CPoint&> myList2(sizeof(CPoint));
		
	bool flag = 0;
	CPoint tPoint(-1,-1);
	for(i = 0; i<pdib1->m_imgWidth; i++){
		if(temp[i] == 0 && flag ==1){
			tPoint.y = i;
			myList1.AddTail(tPoint);
			flag = 0;
		}
		if(temp[i] > 0 && flag ==0){
			tPoint.x = i;
			flag = 1;
		}
	}
	delete temp;
	//链表修正，去除长度过小点
	for(i=0; i<myList1.GetCount();i++){
		CPoint temp(-1,-1);
		temp = myList1.GetAt(myList1.FindIndex(i));
		int minVal = (right - left)/20;
		if((temp.y-temp.x)>=minVal)
			myList2.AddTail(temp);
	}
	myList1.RemoveAll();
	//合并相邻很紧的区域
	bool quit = 1;
	while(quit){
		bool doJoin = false;
		for(int i=0; i<myList2.GetCount()-1;i++){
			CPoint temp1(-1,-1),temp2(-1,-1);
			temp1 = myList2.GetAt(myList2.FindIndex(i));
			temp2 = myList2.GetAt(myList2.FindIndex(i+1));
			if((temp2.x-temp1.y)<=(right - left)/40){
				temp1.y = temp2.y;
				myList2.RemoveAt(myList2.FindIndex(i));
				myList2.RemoveAt(myList2.FindIndex(i));
				if(i == 0)			myList2.AddHead(temp1);
				else			    myList2.InsertAfter(myList2.FindIndex(i-1),temp1);
				doJoin = true;
				break;
			}	
		}
		if(!doJoin)	quit = 0;
	}
	//左右眼的水平区域
	CPoint LEyeAreaH(-1,-1),REyeAreaH(-1,-1),LEyeAreaV(-1,-1),REyeAreaV(-1,-1);
	//没有找到眼睛区域
	if(myList2.GetCount()<2){
		CPoint t=myList2.GetHead();
		if((t.y-t.x)>(right - left)/2){
			LEyeAreaH.x = t.x; 
			LEyeAreaH.y = t.x+(t.y-t.x)/3; 
			REyeAreaH.x = t.y-(t.y-t.x)/3;
			REyeAreaH.y = t.y; 
		}
		else{
			AfxMessageBox("确认眼睛位置失败，请手动标定");
			return;
		}
	}
	//仅有两个区域
	else if(myList2.GetCount()==2){
		LEyeAreaH = myList2.GetHead();
		REyeAreaH = myList2.GetTail();
	}
	else{  //多于两个区域
		int ldis = -100000;
		int rdis = 100000;	
		for(i=0; i<myList2.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myList2.GetAt(myList2.FindIndex(i));
			//右眼
			if((temp.x+temp.y)/2 > nCenterH){
				if(((temp.x+temp.y)/2-nCenterH)<rdis){
					rdis = (temp.x+temp.y)/2-nCenterH;
					REyeAreaH = temp;
				}
			}
			//左眼
			else{
				if(((temp.x+temp.y)/2-nCenterH)>ldis){
					ldis = (temp.x+temp.y)/2-nCenterH;
					LEyeAreaH = temp;
				}
			}
		}
	}
	myList2.RemoveAll();
	///////////////////////////////////
	//确认两个眼睛的垂直区域
	//////////////////////////////////
	//左眼
	if(LEyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
			for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/100){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定左眼的位置");
		}
		else{
			LEyeAreaV = myListB.GetHead();
			
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_LeftEyeLeftCorner.x = 100000;
			m_LeftEyeRightCorner.x = -1;
			
			for(i=LEyeAreaV.x; i<= LEyeAreaV.y;i++)
				for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_LeftEyeLeftCorner.x){
							m_LeftEyeLeftCorner.x = j;
							m_LeftEyeLeftCorner.y = i;
						}
						if(j>m_LeftEyeRightCorner.x){
							m_LeftEyeRightCorner.x = j;
							m_LeftEyeRightCorner.y = i;
						}
					sumX += j;
					sumY += i;
					sum++;
					}
			m_LeftEye.x = (int)(sumX/sum);
			m_LeftEye.y = (int)(sumY/sum);	
		}
		myListB.RemoveAll();
	}
	//右眼
	if(REyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i <bottom-6*nSubWinHeight; i++)
			for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/50){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定右眼的位置");
		}
		else{
			if(myListB.GetCount()==1)
				REyeAreaV = myListB.GetHead();
			else{
				CPoint tt =  myListB.GetHead();
				int index = myListB.GetCount();
				while(tt.y > LEyeAreaV.x && index > 0){
					index --;
					tt = myListB.GetAt(myListB.FindIndex(myListB.GetCount()-index)); 
				}
				REyeAreaV = tt;		
			}
		
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_RightEyeLeftCorner.x = 100000;
			m_RightEyeRightCorner.x = -1;
			
			for(i=REyeAreaV.x; i<=REyeAreaV.y;i++)
				for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_RightEyeLeftCorner.x){
							m_RightEyeLeftCorner.x = j;
							m_RightEyeLeftCorner.y = i;
						}
						if(j>m_RightEyeRightCorner.x){
							m_RightEyeRightCorner.x = j;
							m_RightEyeRightCorner.y = i;
						}
					sumX += j;
					sumY += i;
					sum++;
					}
			m_RightEye.x = (int)(sumX/sum);
			m_RightEye.y = (int)(sumY/sum);
		}
		myListB.RemoveAll();
	}

    memcpy(pdib1->m_pImgData,m_pImgDatabak,pdib1->m_imgHeight*lineByte1);

	//双目斜角
	double tanThta;
	if(m_RightEye.y == m_LeftEye.y) tanThta = 0;
	else tanThta = (m_RightEye.y - m_LeftEye.y)/(m_RightEye.x - m_LeftEye.x);
	//双目距离
	int EyesDis = (m_RightEye.x-m_LeftEye.x)*(m_RightEye.x-m_LeftEye.x);
	EyesDis += (m_RightEye.y-m_LeftEye.y)*(m_RightEye.y-m_LeftEye.y);
	EyesDis = (int)sqrt(EyesDis);
	//双目平均高度
	int EyeV   = (m_RightEye.y + m_LeftEye.y)/2;
  
	//建立定位区域新视图
	F_top=EyeV+nSubWinHeight*4;
	F_bottom=bottom-nSubWinHeight*2;
    F_left=m_LeftEyeLeftCorner.x;
	F_right=m_RightEyeRightCorner.x;

    FaceDetect BoundaryDetect(pdib1->GetDimensions(),pdib1->m_nBitCount,
		                      pdib1->m_lpColorTable, pdib1->m_pImgData,NULL);
    BoundaryDetect.ImageCut(F_top,F_bottom,F_left,F_right);

	//建立一个新视图，显示结果
	pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(BoundaryDetect.GetDimensions(),BoundaryDetect.m_nBitCountOut,BoundaryDetect.m_lpColorTableOut, BoundaryDetect.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);

}


/***********************************************************************
* 函数名称：Onnoselocate()
* 函数参数：无
* 返回值：  无
* 说明： 鼻子定位
***********************************************************************/
void CDemo1View::Onnoselocate() 
{
    CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//获取新建视图指针
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	pFrame->MDINext();
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();


	//获取相关联的新的文档类指针
	CDemo1Doc* pDoc1=pView1->GetDocument();
	CDemo1Doc* pDoc2=pView2->GetDocument();


	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1=pDoc1->GetPDib();
	ImageDib *pdib2=pDoc2->GetPDib();
	
	ImageDib *pdibtemp;
	
	//处理一幅彩色和一幅二值图像
	if(pdib1->m_nBitCount==24){
		if(pdib2->m_nBitCount==24){
			::MessageBox(0,"需要一幅二值图像",MB_OK,0);
			return ;
		}
	}
	else{
		if(pdib2->m_nBitCount==24){
			pdibtemp=pdib1;
			pdib1=pdib2;
			pdib2=pdibtemp;
		}
		else{
			::MessageBox(0,"需要一幅彩色图像",MB_OK,0);
			return ;
		}
	}
	
	CSize size=pdib1->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int lineByte1=(size.cx*3+3)/4*4;
    
	unsigned char *m_pImgDatabak;
	m_pImgDatabak =new unsigned char[pdib1->m_imgHeight*lineByte1];
	memcpy(m_pImgDatabak,pdib1->m_pImgData,pdib1->m_imgHeight*lineByte1);

	
	int i,j;
	int right,left,top,bottom;
	int F_right,F_left,F_top,F_bottom;
	for(i=0;i<size.cy;i++){
		for(j=0;j<size.cx;j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;			
		}
		if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		left=j+1;
		top=i+1;
		
    
	for(i=size.cy-1;i>=0;i--){
		for(j=size.cx-1;j>=0;j--){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;		
		}
        if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		right=j-1;
		bottom=i-1;

	//人脸中心
	int nCenterH = (right+left)/2;
	int nCenterV = (bottom+top)/2;
	//人脸面积
	int nArea = (right - left)*(bottom - top);
	//设定子窗口大小
	int nSubWinWidth  = (right - left)/12;
	int nSubWinHeight = (bottom - top)/30;
	int nSubWinArea =nSubWinHeight*nSubWinWidth;
	
	//水平投影
	int *temp = new int[pdib1->m_imgWidth]; 
	for(i = 0; i<pdib1->m_imgWidth; i++) 
		temp[i] = 0;
	
	for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
		for(j=left+nSubWinWidth; j<right-nSubWinWidth; j++){
			int count = 0;
			for(int p= -nSubWinHeight ;p<nSubWinHeight;p++)
				for(int q= -nSubWinWidth ;q<nSubWinWidth;q++){
					if(*(pdib2->m_pImgData+(i+p)*lineByte+j+q)==0)	count++;     //??????
			}
			if(count >= nSubWinArea/3){
				*(pdib1->m_pImgData+i*lineByte1+j*3) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+1) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+2) = 255;
				temp[j] ++;
			}
	}
		
	CList<CPoint,CPoint&> myList1(sizeof(CPoint));
	CList<CPoint,CPoint&> myList2(sizeof(CPoint));
		
	bool flag = 0;
	CPoint tPoint(-1,-1);
	for(i = 0; i<pdib1->m_imgWidth; i++){
		if(temp[i] == 0 && flag ==1){
			tPoint.y = i;
			myList1.AddTail(tPoint);
			flag = 0;
		}
		if(temp[i] > 0 && flag ==0){
			tPoint.x = i;
			flag = 1;
		}
	}
	delete temp;
	//链表修正，去除长度过小点
	for(i=0; i<myList1.GetCount();i++){
		CPoint temp(-1,-1);
		temp = myList1.GetAt(myList1.FindIndex(i));
		int minVal = (right - left)/20;
		if((temp.y-temp.x)>=minVal)
			myList2.AddTail(temp);
	}
	myList1.RemoveAll();
	//合并相邻很紧的区域
	bool quit = 1;
	while(quit){
		bool doJoin = false;
		for(int i=0; i<myList2.GetCount()-1;i++){
			CPoint temp1(-1,-1),temp2(-1,-1);
			temp1 = myList2.GetAt(myList2.FindIndex(i));
			temp2 = myList2.GetAt(myList2.FindIndex(i+1));
			if((temp2.x-temp1.y)<=(right - left)/40){
				temp1.y = temp2.y;
				myList2.RemoveAt(myList2.FindIndex(i));
				myList2.RemoveAt(myList2.FindIndex(i));
				if(i == 0)			myList2.AddHead(temp1);
				else			    myList2.InsertAfter(myList2.FindIndex(i-1),temp1);
				doJoin = true;
				break;
			}	
		}
		if(!doJoin)	quit = 0;
	}
	//左右眼的水平区域
	CPoint LEyeAreaH(-1,-1),REyeAreaH(-1,-1),LEyeAreaV(-1,-1),REyeAreaV(-1,-1);
	//没有找到眼睛区域
	if(myList2.GetCount()<2){
		CPoint t=myList2.GetHead();
		if((t.y-t.x)>(right - left)/2){
			LEyeAreaH.x = t.x; 
			LEyeAreaH.y = t.x+(t.y-t.x)/3; 
			REyeAreaH.x = t.y-(t.y-t.x)/3;
			REyeAreaH.y = t.y; 
		}
		else{
			AfxMessageBox("确认眼睛位置失败，请手动标定");
			return;
		}
	}
	//仅有两个区域
	else if(myList2.GetCount()==2){
		LEyeAreaH = myList2.GetHead();
		REyeAreaH = myList2.GetTail();
	}
	else{  //多于两个区域
		int ldis = -100000;
		int rdis = 100000;	
		for(i=0; i<myList2.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myList2.GetAt(myList2.FindIndex(i));
			//右眼
			if((temp.x+temp.y)/2 > nCenterH){
				if(((temp.x+temp.y)/2-nCenterH)<rdis){
					rdis = (temp.x+temp.y)/2-nCenterH;
					REyeAreaH = temp;
				}
			}
			//左眼
			else{
				if(((temp.x+temp.y)/2-nCenterH)>ldis){
					ldis = (temp.x+temp.y)/2-nCenterH;
					LEyeAreaH = temp;
				}
			}
		}
	}
	myList2.RemoveAll();
	///////////////////////////////////
	//确认两个眼睛的垂直区域
	//////////////////////////////////
	//左眼
	if(LEyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
			for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/100){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定左眼的位置");
		}
		else{
			LEyeAreaV = myListB.GetHead();
			
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_LeftEyeLeftCorner.x = 100000;
			m_LeftEyeRightCorner.x = -1;
			
			for(i=LEyeAreaV.x; i<= LEyeAreaV.y;i++)
				for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_LeftEyeLeftCorner.x){
							m_LeftEyeLeftCorner.x = j;
							m_LeftEyeLeftCorner.y = i;
						}
						if(j>m_LeftEyeRightCorner.x){
							m_LeftEyeRightCorner.x = j;
							m_LeftEyeRightCorner.y = i;
					}
					sumX += j;
					sumY += i;
					sum++;
			}
			m_LeftEye.x = (int)(sumX/sum);
			m_LeftEye.y = (int)(sumY/sum);	
		}
		myListB.RemoveAll();
	}
	//右眼
	if(REyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i <bottom-6*nSubWinHeight; i++)
			for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/50){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定右眼的位置");
		}
		else{
			if(myListB.GetCount()==1)
				REyeAreaV = myListB.GetHead();
			else{
				CPoint tt =  myListB.GetHead();
				int index = myListB.GetCount();
				while(tt.y > LEyeAreaV.x && index > 0){
					index --;
					tt = myListB.GetAt(myListB.FindIndex(myListB.GetCount()-index)); 
				}
				REyeAreaV = tt;		
			}
		
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_RightEyeLeftCorner.x = 100000;
			m_RightEyeRightCorner.x = -1;
			
			for(i=REyeAreaV.x; i<=REyeAreaV.y;i++)
				for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_RightEyeLeftCorner.x){
							m_RightEyeLeftCorner.x = j;
							m_RightEyeLeftCorner.y = i;
						}
						if(j>m_RightEyeRightCorner.x){
							m_RightEyeRightCorner.x = j;
							m_RightEyeRightCorner.y = i;
					}
					sumX += j;
					sumY += i;
					sum++;
			}
			m_RightEye.x = (int)(sumX/sum);
			m_RightEye.y = (int)(sumY/sum);
		}
		myListB.RemoveAll();
	}


	for(i=top; i<=bottom; i++)
		for(j=left; j<=right; j++){
			double Y;
			Y = 0.30*(*(pdib1->m_pImgData+i*lineByte1+j*3+2))+0.59*(*(pdib1->m_pImgData+i*lineByte1+j*3+1))
				+0.11*(*(pdib1->m_pImgData+i*lineByte1+j*3));
			if(Y<100){
				*(pdib1->m_pImgData+i*lineByte1+j*3+2) = 255;  
			}
			else *(pdib1->m_pImgData+i*lineByte1+j*3+2) = 0;
	}
		
	AfxMessageBox("鼻子的肤色鉴定");
	//双目斜角
	double tanThta;
	if(m_RightEye.y == m_LeftEye.y) tanThta = 0;
	else tanThta = (m_RightEye.y - m_LeftEye.y)/(m_RightEye.x - m_LeftEye.x);
	//双目距离
	int EyesDis = (m_RightEye.x-m_LeftEye.x)*(m_RightEye.x-m_LeftEye.x);
	EyesDis += (m_RightEye.y-m_LeftEye.y)*(m_RightEye.y-m_LeftEye.y);
	EyesDis = (int)sqrt(EyesDis);
	//双目平均高度
	int EyeV    = (m_RightEye.y + m_LeftEye.y)/2;
	//可能的鼻子的区域
	int NoseUp   = (EyeV-0.5*EyesDis) < top ? top:(int)(EyeV-0.5*EyesDis);
	int NoseDown = (EyeV-0.8*EyesDis) < top ? top:(int)(EyeV-0.8*EyesDis);
		
	int* Y_Arry = new int[NoseUp-NoseDown];
	for(i =0 ;i < NoseUp-NoseDown ;i++) Y_Arry[i] = 0;
	int* X_Arry = new int[EyesDis];
	for(i =0 ;i < EyesDis ;i++) X_Arry[i] = 0;
	for(i = NoseDown ; i < NoseUp; i++)
		for(j = m_LeftEye.x+EyesDis/5; j< m_RightEye.x-EyesDis/5; j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255){
				Y_Arry[i-NoseDown] ++;
				X_Arry[j-m_LeftEye.x] ++;
			}
	}
			
	int maxY = 0;
	for(i =0 ;i < NoseUp-NoseDown ;i++){
		if(Y_Arry[i]>maxY){
			maxY = Y_Arry[i];
			m_MidNose.y =  i+NoseDown;
		}
	}
	m_LeftNostril.y  =(int)(m_MidNose.y - tanThta*EyesDis/2);
	m_RightNostril.y =(int)(m_MidNose.y + tanThta*EyesDis/2);
	for(i =0 ;i < EyesDis ;i++){
		if(X_Arry[i]>0){
			m_LeftNostril.x = i+m_LeftEye.x;
			break;
		}
	}
	for(i = EyesDis-1; i >=0 ;i--){
		if(X_Arry[i]>0){
			m_RightNostril.x = i+m_LeftEye.x;
			break;
		}
	}
	
	int min = 1000000;
	for(i = (int)(EyesDis/3+0.5) ; i <= (int)(2*EyesDis/3+0.5);i++){
		if(X_Arry[i]<min){
			min = X_Arry[i];
			m_MidNose.x = m_LeftEye.x+i;
		}
	}
	m_MidNose.x = (m_MidNose.x+(m_LeftEye.x+EyesDis/2))/2;
			
	memcpy(pdib1->m_pImgData,m_pImgDatabak,pdib1->m_imgHeight*lineByte1);

	//建立定位区域新视图
	F_top=NoseDown+0.15*EyesDis;
	F_bottom=EyeV-0.1*EyesDis;
    F_left=m_LeftEyeRightCorner.x+0.1*EyesDis;
	F_right=m_RightEyeLeftCorner.x-0.1*EyesDis;

    FaceDetect BoundaryDetect(pdib1->GetDimensions(),pdib1->m_nBitCount,
		                      pdib1->m_lpColorTable, pdib1->m_pImgData,NULL);
    BoundaryDetect.ImageCut(F_top,F_bottom,F_left,F_right);

	//建立一个新视图，显示结果
	pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(BoundaryDetect.GetDimensions(),BoundaryDetect.m_nBitCountOut,BoundaryDetect.m_lpColorTableOut, BoundaryDetect.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);

}

/***********************************************************************
* 函数名称：Onmouthlocate() 
* 函数参数：无
* 返回值：  无
* 说明： 下巴定位
***********************************************************************/
void CDemo1View::Onmouthlocate() 
{
CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//获取新建视图指针
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	pFrame->MDINext();
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();


	//获取相关联的新的文档类指针
	CDemo1Doc* pDoc1=pView1->GetDocument();
	CDemo1Doc* pDoc2=pView2->GetDocument();


	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1=pDoc1->GetPDib();
	ImageDib *pdib2=pDoc2->GetPDib();
	
	ImageDib *pdibtemp;
	
	//处理一幅彩色和一幅二值图像
	if(pdib1->m_nBitCount==24){
		if(pdib2->m_nBitCount==24){
			::MessageBox(0,"需要一幅二值图像",MB_OK,0);
			return ;
		}
	}
	else{
		if(pdib2->m_nBitCount==24){
			pdibtemp=pdib1;
			pdib1=pdib2;
			pdib2=pdibtemp;
		}
		else{
			::MessageBox(0,"需要一幅彩色图像",MB_OK,0);
			return ;
		}
	}
	
	CSize size=pdib1->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int lineByte1=(size.cx*3+3)/4*4;
    
	unsigned char *m_pImgDatabak;
	m_pImgDatabak =new unsigned char[pdib1->m_imgHeight*lineByte1];
	memcpy(m_pImgDatabak,pdib1->m_pImgData,pdib1->m_imgHeight*lineByte1);

	
	int i,j;
	int right,left,top,bottom;
	int F_right,F_left,F_top,F_bottom;
	for(i=0;i<size.cy;i++){
		for(j=0;j<size.cx;j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;			
		}
		if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		left=j+1;
		top=i+1;
		
    
	for(i=size.cy-1;i>=0;i--){
		for(j=size.cx-1;j>=0;j--){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;		
		}
        if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		right=j-1;
		bottom=i-1;

	//人脸中心
	int nCenterH = (right+left)/2;
	int nCenterV = (bottom+top)/2;
	//人脸面积
	int nArea = (right - left)*(bottom - top);
	//设定子窗口大小
	int nSubWinWidth  = (right - left)/12;
	int nSubWinHeight = (bottom - top)/30;
	int nSubWinArea =nSubWinHeight*nSubWinWidth;
	
	//水平投影
	int *temp = new int[pdib1->m_imgWidth]; 
	for(i = 0; i<pdib1->m_imgWidth; i++) 
		temp[i] = 0;
	
	for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
		for(j=left+nSubWinWidth; j<right-nSubWinWidth; j++){
			int count = 0;
			for(int p= -nSubWinHeight ;p<nSubWinHeight;p++)
				for(int q= -nSubWinWidth ;q<nSubWinWidth;q++){
					if(*(pdib2->m_pImgData+(i+p)*lineByte+j+q)==0)	count++;     //??????
			}
			if(count >= nSubWinArea/3){
				*(pdib1->m_pImgData+i*lineByte1+j*3) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+1) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+2) = 255;
				temp[j] ++;
			}
	}
		
	CList<CPoint,CPoint&> myList1(sizeof(CPoint));
	CList<CPoint,CPoint&> myList2(sizeof(CPoint));
		
	bool flag = 0;
	CPoint tPoint(-1,-1);
	for(i = 0; i<pdib1->m_imgWidth; i++){
		if(temp[i] == 0 && flag ==1){
			tPoint.y = i;
			myList1.AddTail(tPoint);
			flag = 0;
		}
		if(temp[i] > 0 && flag ==0){
			tPoint.x = i;
			flag = 1;
		}
	}
	delete temp;
	//链表修正，去除长度过小点
	for(i=0; i<myList1.GetCount();i++){
		CPoint temp(-1,-1);
		temp = myList1.GetAt(myList1.FindIndex(i));
		int minVal = (right - left)/20;
		if((temp.y-temp.x)>=minVal)
			myList2.AddTail(temp);
	}
	myList1.RemoveAll();
	//合并相邻很紧的区域
	bool quit = 1;
	while(quit){
		bool doJoin = false;
		for(int i=0; i<myList2.GetCount()-1;i++){
			CPoint temp1(-1,-1),temp2(-1,-1);
			temp1 = myList2.GetAt(myList2.FindIndex(i));
			temp2 = myList2.GetAt(myList2.FindIndex(i+1));
			if((temp2.x-temp1.y)<=(right - left)/40){
				temp1.y = temp2.y;
				myList2.RemoveAt(myList2.FindIndex(i));
				myList2.RemoveAt(myList2.FindIndex(i));
				if(i == 0)			myList2.AddHead(temp1);
				else			    myList2.InsertAfter(myList2.FindIndex(i-1),temp1);
				doJoin = true;
				break;
			}	
		}
		if(!doJoin)	quit = 0;
	}
	//左右眼的水平区域
	CPoint LEyeAreaH(-1,-1),REyeAreaH(-1,-1),LEyeAreaV(-1,-1),REyeAreaV(-1,-1);
	//没有找到眼睛区域
	if(myList2.GetCount()<2){
		CPoint t=myList2.GetHead();
		if((t.y-t.x)>(right - left)/2){
			LEyeAreaH.x = t.x; 
			LEyeAreaH.y = t.x+(t.y-t.x)/3; 
			REyeAreaH.x = t.y-(t.y-t.x)/3;
			REyeAreaH.y = t.y; 
		}
		else{
			AfxMessageBox("确认眼睛位置失败，请手动标定");
			return;
		}
	}
	//仅有两个区域
	else if(myList2.GetCount()==2){
		LEyeAreaH = myList2.GetHead();
		REyeAreaH = myList2.GetTail();
	}
	else{  //多于两个区域
		int ldis = -100000;
		int rdis = 100000;	
		for(i=0; i<myList2.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myList2.GetAt(myList2.FindIndex(i));
			//右眼
			if((temp.x+temp.y)/2 > nCenterH){
				if(((temp.x+temp.y)/2-nCenterH)<rdis){
					rdis = (temp.x+temp.y)/2-nCenterH;
					REyeAreaH = temp;
				}
			}
			//左眼
			else{
				if(((temp.x+temp.y)/2-nCenterH)>ldis){
					ldis = (temp.x+temp.y)/2-nCenterH;
					LEyeAreaH = temp;
				}
			}
		}
	}
	myList2.RemoveAll();
	///////////////////////////////////
	//确认两个眼睛的垂直区域
	//////////////////////////////////
	//左眼
	if(LEyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
			for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/100){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定左眼的位置");
		}
		else{
			LEyeAreaV = myListB.GetHead();
			
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_LeftEyeLeftCorner.x = 100000;
			m_LeftEyeRightCorner.x = -1;
			
			for(i=LEyeAreaV.x; i<= LEyeAreaV.y;i++)
				for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_LeftEyeLeftCorner.x){
							m_LeftEyeLeftCorner.x = j;
							m_LeftEyeLeftCorner.y = i;
						}
						if(j>m_LeftEyeRightCorner.x){
							m_LeftEyeRightCorner.x = j;
							m_LeftEyeRightCorner.y = i;
						}
					sumX += j;
					sumY += i;
					sum++;
					}
			m_LeftEye.x = (int)(sumX/sum);
			m_LeftEye.y = (int)(sumY/sum);	
		}
		myListB.RemoveAll();
	}
	//右眼
	if(REyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i <bottom-6*nSubWinHeight; i++)
			for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/50){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定右眼的位置");
		}
		else{
			if(myListB.GetCount()==1)
				REyeAreaV = myListB.GetHead();
			else{
				CPoint tt =  myListB.GetHead();
				int index = myListB.GetCount();
				while(tt.y > LEyeAreaV.x && index > 0){
					index --;
					tt = myListB.GetAt(myListB.FindIndex(myListB.GetCount()-index)); 
				}
				REyeAreaV = tt;		
			}
		
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_RightEyeLeftCorner.x = 100000;
			m_RightEyeRightCorner.x = -1;
			
			for(i=REyeAreaV.x; i<=REyeAreaV.y;i++)
				for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_RightEyeLeftCorner.x){
							m_RightEyeLeftCorner.x = j;
							m_RightEyeLeftCorner.y = i;
						}
						if(j>m_RightEyeRightCorner.x){
							m_RightEyeRightCorner.x = j;
							m_RightEyeRightCorner.y = i;
						}
					sumX += j;
					sumY += i;
					sum++;
					}
			m_RightEye.x = (int)(sumX/sum);
			m_RightEye.y = (int)(sumY/sum);
		}
		myListB.RemoveAll();
	}
	
	for(i=top; i<=bottom; i++)
		for(j=left; j<=right; j++){
			BYTE R,G,B;
			double temp,dlta;
			R = *(pdib1->m_pImgData+i*lineByte1+j*3+2);
			G = *(pdib1->m_pImgData+i*lineByte1+j*3+1);
			B = *(pdib1->m_pImgData+i*lineByte1+j*3);
			if((R==G) && (G==B)) temp = 0;
			else temp = 0.5*(2*R-G-B)/sqrt((R-G)*(R-G)+(R-B)*(G-B));
			dlta = acos(temp);
			if(dlta < 0.2){
				*(pdib1->m_pImgData+i*lineByte1+j*3+2) = 255;  
			}
			else *(pdib1->m_pImgData+i*lineByte1+j*3+2) = 0;
	
	}
		
	AfxMessageBox("下巴的肤色鉴定");

	//双目斜角
	double tanThta;
	if(m_RightEye.y == m_LeftEye.y) tanThta = 0;
	else tanThta = (m_RightEye.y - m_LeftEye.y)/(m_RightEye.x - m_LeftEye.x);
	//双目距离
	int EyesDis = (m_RightEye.x-m_LeftEye.x)*(m_RightEye.x-m_LeftEye.x);
	EyesDis += (m_RightEye.y-m_LeftEye.y)*(m_RightEye.y-m_LeftEye.y);
	EyesDis = (int)sqrt(EyesDis);
	//双目平均高度
	int EyeV    = (m_RightEye.y + m_LeftEye.y)/2;
	//可能的嘴的区域
	int MouthUp   = (EyeV-1.0*EyesDis) < top ? top:(int)(EyeV-1.0*EyesDis);
	int MouthDown = (EyeV-1.5*EyesDis) < top ? top:(int)(EyeV-1.5*EyesDis);

	
	int* Y_Arry = new int[MouthUp-MouthDown];
	for(i =0 ;i < MouthUp-MouthDown ;i++) Y_Arry[i] = 0;
	int* X_Arry = new int[EyesDis];
	for(i =0 ;i < EyesDis ;i++) X_Arry[i] = 0;
	for(i = MouthDown ; i < MouthUp; i++)
		for(j = m_LeftEye.x; j< m_RightEye.x; j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255){
					Y_Arry[i-MouthDown] ++;
					X_Arry[j-m_LeftEye.x] ++;
			}
	}
			
	int maxY = 0;
	for(i =0 ;i < MouthUp-MouthDown ;i++){
		if(Y_Arry[i]>maxY){
			maxY = Y_Arry[i];
			m_MidMouth.y =  i+MouthDown - (MouthUp-MouthDown)/10;
		}
	}
	m_LeftMouthCorner.y  =(int)(m_MidMouth.y - tanThta*EyesDis/2);
	m_RightMouthCorner.y =(int)(m_MidMouth.y + tanThta*EyesDis/2);
	for(i =0 ;i < EyesDis ;i++){
		if(X_Arry[i]>0){
			m_LeftMouthCorner.x = i+m_LeftEye.x;
			break;
		}
	}
	for(i = EyesDis -1; i >=0 ;i--){
		if(X_Arry[i]>0){
			m_RightMouthCorner.x = m_LeftEye.x+i;
			break;
		}
	}
			
	//唇中点较薄
	int min = 1000000;
	for(i = (int)(3*EyesDis/7+0.5) ; i <= (int)(4*EyesDis/7+0.5);i++){
		if(X_Arry[i]<min){
			min = X_Arry[i];
			m_MidMouth.x = m_LeftEye.x+i;
		}
	}
	m_MidMouth.x = (int)(m_MidMouth.x+(m_LeftEye.x+EyesDis/2))/2;

	memcpy(pdib1->m_pImgData,m_pImgDatabak,pdib1->m_imgHeight*lineByte1);

	//建立定位区域新视图
	F_top=top+0.1*EyesDis;
	F_bottom=EyeV-1.35*EyesDis;
    F_left=m_LeftMouthCorner.x;
	F_right=m_RightMouthCorner.x;

    FaceDetect BoundaryDetect(pdib1->GetDimensions(),pdib1->m_nBitCount,
		                      pdib1->m_lpColorTable, pdib1->m_pImgData,NULL);
    BoundaryDetect.ImageCut(F_top,F_bottom,F_left,F_right);

	//建立一个新视图，显示结果
	pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(BoundaryDetect.GetDimensions(),BoundaryDetect.m_nBitCountOut,BoundaryDetect.m_lpColorTableOut, BoundaryDetect.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}

void CDemo1View::Onleftfacelocate() 
{
    CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//获取新建视图指针
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	pFrame->MDINext();
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();


	//获取相关联的新的文档类指针
	CDemo1Doc* pDoc1=pView1->GetDocument();
	CDemo1Doc* pDoc2=pView2->GetDocument();


	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1=pDoc1->GetPDib();
	ImageDib *pdib2=pDoc2->GetPDib();
	
	ImageDib *pdibtemp;
	
	//处理一幅彩色和一幅二值图像
	if(pdib1->m_nBitCount==24){
		if(pdib2->m_nBitCount==24){
			::MessageBox(0,"需要一幅二值图像",MB_OK,0);
			return ;
		}
	}
	else{
		if(pdib2->m_nBitCount==24){
			pdibtemp=pdib1;
			pdib1=pdib2;
			pdib2=pdibtemp;
		}
		else{
			::MessageBox(0,"需要一幅彩色图像",MB_OK,0);
			return ;
		}
	}
	
	CSize size=pdib1->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int lineByte1=(size.cx*3+3)/4*4;
    
	unsigned char *m_pImgDatabak;
	m_pImgDatabak =new unsigned char[pdib1->m_imgHeight*lineByte1];
	memcpy(m_pImgDatabak,pdib1->m_pImgData,pdib1->m_imgHeight*lineByte1);

	
	int i,j;
	int right,left,top,bottom;
	int F_right,F_left,F_top,F_bottom;
	for(i=0;i<size.cy;i++){
		for(j=0;j<size.cx;j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;			
		}
		if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		left=j+1;
		top=i+1;
		
    
	for(i=size.cy-1;i>=0;i--){
		for(j=size.cx-1;j>=0;j--){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;		
		}
        if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		right=j-1;
		bottom=i-1;

	//人脸中心
	int nCenterH = (right+left)/2;
	int nCenterV = (bottom+top)/2;
	//人脸面积
	int nArea = (right - left)*(bottom - top);
	//设定子窗口大小
	int nSubWinWidth  = (right - left)/12;
	int nSubWinHeight = (bottom - top)/30;
	int nSubWinArea =nSubWinHeight*nSubWinWidth;
	
	//水平投影
	int *temp = new int[pdib1->m_imgWidth]; 
	for(i = 0; i<pdib1->m_imgWidth; i++) 
		temp[i] = 0;
	
	for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
		for(j=left+nSubWinWidth; j<right-nSubWinWidth; j++){
			int count = 0;
			for(int p= -nSubWinHeight ;p<nSubWinHeight;p++)
				for(int q= -nSubWinWidth ;q<nSubWinWidth;q++){
					if(*(pdib2->m_pImgData+(i+p)*lineByte+j+q)==0)	count++;     //??????
			}
			if(count >= nSubWinArea/3){
				*(pdib1->m_pImgData+i*lineByte1+j*3) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+1) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+2) = 255;
				temp[j] ++;
			}
	}
		
	CList<CPoint,CPoint&> myList1(sizeof(CPoint));
	CList<CPoint,CPoint&> myList2(sizeof(CPoint));
		
	bool flag = 0;
	CPoint tPoint(-1,-1);
	for(i = 0; i<pdib1->m_imgWidth; i++){
		if(temp[i] == 0 && flag ==1){
			tPoint.y = i;
			myList1.AddTail(tPoint);
			flag = 0;
		}
		if(temp[i] > 0 && flag ==0){
			tPoint.x = i;
			flag = 1;
		}
	}
	delete temp;
	//链表修正，去除长度过小点
	for(i=0; i<myList1.GetCount();i++){
		CPoint temp(-1,-1);
		temp = myList1.GetAt(myList1.FindIndex(i));
		int minVal = (right - left)/20;
		if((temp.y-temp.x)>=minVal)
			myList2.AddTail(temp);
	}
	myList1.RemoveAll();
	//合并相邻很紧的区域
	bool quit = 1;
	while(quit){
		bool doJoin = false;
		for(int i=0; i<myList2.GetCount()-1;i++){
			CPoint temp1(-1,-1),temp2(-1,-1);
			temp1 = myList2.GetAt(myList2.FindIndex(i));
			temp2 = myList2.GetAt(myList2.FindIndex(i+1));
			if((temp2.x-temp1.y)<=(right - left)/40){
				temp1.y = temp2.y;
				myList2.RemoveAt(myList2.FindIndex(i));
				myList2.RemoveAt(myList2.FindIndex(i));
				if(i == 0)			myList2.AddHead(temp1);
				else			    myList2.InsertAfter(myList2.FindIndex(i-1),temp1);
				doJoin = true;
				break;
			}	
		}
		if(!doJoin)	quit = 0;
	}
	//左右眼的水平区域
	CPoint LEyeAreaH(-1,-1),REyeAreaH(-1,-1),LEyeAreaV(-1,-1),REyeAreaV(-1,-1);
	//没有找到眼睛区域
	if(myList2.GetCount()<2){
		CPoint t=myList2.GetHead();
		if((t.y-t.x)>(right - left)/2){
			LEyeAreaH.x = t.x; 
			LEyeAreaH.y = t.x+(t.y-t.x)/3; 
			REyeAreaH.x = t.y-(t.y-t.x)/3;
			REyeAreaH.y = t.y; 
		}
		else{
			AfxMessageBox("确认眼睛位置失败，请手动标定");
			return;
		}
	}
	//仅有两个区域
	else if(myList2.GetCount()==2){
		LEyeAreaH = myList2.GetHead();
		REyeAreaH = myList2.GetTail();
	}
	else{  //多于两个区域
		int ldis = -100000;
		int rdis = 100000;	
		for(i=0; i<myList2.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myList2.GetAt(myList2.FindIndex(i));
			//右眼
			if((temp.x+temp.y)/2 > nCenterH){
				if(((temp.x+temp.y)/2-nCenterH)<rdis){
					rdis = (temp.x+temp.y)/2-nCenterH;
					REyeAreaH = temp;
				}
			}
			//左眼
			else{
				if(((temp.x+temp.y)/2-nCenterH)>ldis){
					ldis = (temp.x+temp.y)/2-nCenterH;
					LEyeAreaH = temp;
				}
			}
		}
	}
	myList2.RemoveAll();
	///////////////////////////////////
	//确认两个眼睛的垂直区域
	//////////////////////////////////
	//左眼
	if(LEyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
			for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/100){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定左眼的位置");
		}
		else{
			LEyeAreaV = myListB.GetHead();
			
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_LeftEyeLeftCorner.x = 100000;
			m_LeftEyeRightCorner.x = -1;
			
			for(i=LEyeAreaV.x; i<= LEyeAreaV.y;i++)
				for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_LeftEyeLeftCorner.x){
							m_LeftEyeLeftCorner.x = j;
							m_LeftEyeLeftCorner.y = i;
						}
						if(j>m_LeftEyeRightCorner.x){
							m_LeftEyeRightCorner.x = j;
							m_LeftEyeRightCorner.y = i;
						}
					sumX += j;
					sumY += i;
					sum++;
					}
			m_LeftEye.x = (int)(sumX/sum);
			m_LeftEye.y = (int)(sumY/sum);	
		}
		myListB.RemoveAll();
	}
	//右眼
	if(REyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i <bottom-6*nSubWinHeight; i++)
			for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/50){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定右眼的位置");
		}
		else{
			if(myListB.GetCount()==1)
				REyeAreaV = myListB.GetHead();
			else{
				CPoint tt =  myListB.GetHead();
				int index = myListB.GetCount();
				while(tt.y > LEyeAreaV.x && index > 0){
					index --;
					tt = myListB.GetAt(myListB.FindIndex(myListB.GetCount()-index)); 
				}
				REyeAreaV = tt;		
			}
		
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_RightEyeLeftCorner.x = 100000;
			m_RightEyeRightCorner.x = -1;
			
			for(i=REyeAreaV.x; i<=REyeAreaV.y;i++)
				for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_RightEyeLeftCorner.x){
							m_RightEyeLeftCorner.x = j;
							m_RightEyeLeftCorner.y = i;
						}
						if(j>m_RightEyeRightCorner.x){
							m_RightEyeRightCorner.x = j;
							m_RightEyeRightCorner.y = i;
						}
					sumX += j;
					sumY += i;
					sum++;
					}
			m_RightEye.x = (int)(sumX/sum);
			m_RightEye.y = (int)(sumY/sum);
		}
		myListB.RemoveAll();
	}
	
	for(i=top; i<=bottom; i++)
		for(j=left; j<=right; j++){
			BYTE R,G,B;
			double temp,dlta;
			R = *(pdib1->m_pImgData+i*lineByte1+j*3+2);
			G = *(pdib1->m_pImgData+i*lineByte1+j*3+1);
			B = *(pdib1->m_pImgData+i*lineByte1+j*3);
			if((R==G) && (G==B)) temp = 0;
			else temp = 0.5*(2*R-G-B)/sqrt((R-G)*(R-G)+(R-B)*(G-B));
			dlta = acos(temp);
			if(dlta < 0.2){
				*(pdib1->m_pImgData+i*lineByte1+j*3+2) = 255;  
			}
			else *(pdib1->m_pImgData+i*lineByte1+j*3+2) = 0;
	
	}
		
	AfxMessageBox("左脸的肤色鉴定");

	//双目斜角
	double tanThta;
	if(m_RightEye.y == m_LeftEye.y) tanThta = 0;
	else tanThta = (m_RightEye.y - m_LeftEye.y)/(m_RightEye.x - m_LeftEye.x);
	//双目距离
	int EyesDis = (m_RightEye.x-m_LeftEye.x)*(m_RightEye.x-m_LeftEye.x);
	EyesDis += (m_RightEye.y-m_LeftEye.y)*(m_RightEye.y-m_LeftEye.y);
	EyesDis = (int)sqrt(EyesDis);
	//双目平均高度
	int EyeV    = (m_RightEye.y + m_LeftEye.y)/2;
	//可能的嘴的区域
	int MouthUp   = (EyeV-1.0*EyesDis) < top ? top:(int)(EyeV-1.0*EyesDis);
	int MouthDown = (EyeV-1.5*EyesDis) < top ? top:(int)(EyeV-1.5*EyesDis);

	
	int* Y_Arry = new int[MouthUp-MouthDown];
	for(i =0 ;i < MouthUp-MouthDown ;i++) Y_Arry[i] = 0;
	int* X_Arry = new int[EyesDis];
	for(i =0 ;i < EyesDis ;i++) X_Arry[i] = 0;
	for(i = MouthDown ; i < MouthUp; i++)
		for(j = m_LeftEye.x; j< m_RightEye.x; j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255){
					Y_Arry[i-MouthDown] ++;
					X_Arry[j-m_LeftEye.x] ++;
			}
	}
			
	int maxY = 0;
	for(i =0 ;i < MouthUp-MouthDown ;i++){
		if(Y_Arry[i]>maxY){
			maxY = Y_Arry[i];
			m_MidMouth.y =  i+MouthDown - (MouthUp-MouthDown)/10;
		}
	}
	m_LeftMouthCorner.y  =(int)(m_MidMouth.y - tanThta*EyesDis/2);
	m_RightMouthCorner.y =(int)(m_MidMouth.y + tanThta*EyesDis/2);
	for(i =0 ;i < EyesDis ;i++){
		if(X_Arry[i]>0){
			m_LeftMouthCorner.x = i+m_LeftEye.x;
			break;
		}
	}
	for(i = EyesDis -1; i >=0 ;i--){
		if(X_Arry[i]>0){
			m_RightMouthCorner.x = m_LeftEye.x+i;
			break;
		}
	}
			
	//唇中点较薄
	int min = 1000000;
	for(i = (int)(3*EyesDis/7+0.5) ; i <= (int)(4*EyesDis/7+0.5);i++){
		if(X_Arry[i]<min){
			min = X_Arry[i];
			m_MidMouth.x = m_LeftEye.x+i;
		}
	}
	m_MidMouth.x = (int)(m_MidMouth.x+(m_LeftEye.x+EyesDis/2))/2;

	memcpy(pdib1->m_pImgData,m_pImgDatabak,pdib1->m_imgHeight*lineByte1);

	//建立定位区域新视图
	F_top=m_MidMouth.y;
	F_bottom=EyeV-0.15*EyesDis;
    F_left=m_LeftEyeLeftCorner.x-0.3*EyesDis;;
	F_right=m_LeftMouthCorner.x;

    FaceDetect BoundaryDetect(pdib1->GetDimensions(),pdib1->m_nBitCount,
		                      pdib1->m_lpColorTable, pdib1->m_pImgData,NULL);
    BoundaryDetect.ImageCut(F_top,F_bottom,F_left,F_right);

	//建立一个新视图，显示结果
	pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(BoundaryDetect.GetDimensions(),BoundaryDetect.m_nBitCountOut,BoundaryDetect.m_lpColorTableOut, BoundaryDetect.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
}

void CDemo1View::Onrightfacelocate() 
{
    CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//获取新建视图指针
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	pFrame->MDINext();
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();


	//获取相关联的新的文档类指针
	CDemo1Doc* pDoc1=pView1->GetDocument();
	CDemo1Doc* pDoc2=pView2->GetDocument();


	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1=pDoc1->GetPDib();
	ImageDib *pdib2=pDoc2->GetPDib();
	
	ImageDib *pdibtemp;
	
	//处理一幅彩色和一幅二值图像
	if(pdib1->m_nBitCount==24){
		if(pdib2->m_nBitCount==24){
			::MessageBox(0,"需要一幅二值图像",MB_OK,0);
			return ;
		}
	}
	else{
		if(pdib2->m_nBitCount==24){
			pdibtemp=pdib1;
			pdib1=pdib2;
			pdib2=pdibtemp;
		}
		else{
			::MessageBox(0,"需要一幅彩色图像",MB_OK,0);
			return ;
		}
	}
	
	CSize size=pdib1->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int lineByte1=(size.cx*3+3)/4*4;
    
	unsigned char *m_pImgDatabak;
	m_pImgDatabak =new unsigned char[pdib1->m_imgHeight*lineByte1];
	memcpy(m_pImgDatabak,pdib1->m_pImgData,pdib1->m_imgHeight*lineByte1);

	
	int i,j;
	int right,left,top,bottom;
	int F_right,F_left,F_top,F_bottom;
	for(i=0;i<size.cy;i++){
		for(j=0;j<size.cx;j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;			
		}
		if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		left=j+1;
		top=i+1;
		
    
	for(i=size.cy-1;i>=0;i--){
		for(j=size.cx-1;j>=0;j--){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
			   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
               *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
				break;		
		}
        if(*(pdib1->m_pImgData+i*lineByte1+j*3+1)==255&&
		   *(pdib1->m_pImgData+i*lineByte1+j*3+0)==0&&
           *(pdib1->m_pImgData+i*lineByte1+j*3+2)==0)
		   break;
	}
		right=j-1;
		bottom=i-1;

	//人脸中心
	int nCenterH = (right+left)/2;
	int nCenterV = (bottom+top)/2;
	//人脸面积
	int nArea = (right - left)*(bottom - top);
	//设定子窗口大小
	int nSubWinWidth  = (right - left)/12;
	int nSubWinHeight = (bottom - top)/30;
	int nSubWinArea =nSubWinHeight*nSubWinWidth;
	
	//水平投影
	int *temp = new int[pdib1->m_imgWidth]; 
	for(i = 0; i<pdib1->m_imgWidth; i++) 
		temp[i] = 0;
	
	for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
		for(j=left+nSubWinWidth; j<right-nSubWinWidth; j++){
			int count = 0;
			for(int p= -nSubWinHeight ;p<nSubWinHeight;p++)
				for(int q= -nSubWinWidth ;q<nSubWinWidth;q++){
					if(*(pdib2->m_pImgData+(i+p)*lineByte+j+q)==0)	count++;     //??????
			}
			if(count >= nSubWinArea/3){
				*(pdib1->m_pImgData+i*lineByte1+j*3) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+1) = 0;
				*(pdib1->m_pImgData+i*lineByte1+j*3+2) = 255;
				temp[j] ++;
			}
	}
		
	CList<CPoint,CPoint&> myList1(sizeof(CPoint));
	CList<CPoint,CPoint&> myList2(sizeof(CPoint));
		
	bool flag = 0;
	CPoint tPoint(-1,-1);
	for(i = 0; i<pdib1->m_imgWidth; i++){
		if(temp[i] == 0 && flag ==1){
			tPoint.y = i;
			myList1.AddTail(tPoint);
			flag = 0;
		}
		if(temp[i] > 0 && flag ==0){
			tPoint.x = i;
			flag = 1;
		}
	}
	delete temp;
	//链表修正，去除长度过小点
	for(i=0; i<myList1.GetCount();i++){
		CPoint temp(-1,-1);
		temp = myList1.GetAt(myList1.FindIndex(i));
		int minVal = (right - left)/20;
		if((temp.y-temp.x)>=minVal)
			myList2.AddTail(temp);
	}
	myList1.RemoveAll();
	//合并相邻很紧的区域
	bool quit = 1;
	while(quit){
		bool doJoin = false;
		for(int i=0; i<myList2.GetCount()-1;i++){
			CPoint temp1(-1,-1),temp2(-1,-1);
			temp1 = myList2.GetAt(myList2.FindIndex(i));
			temp2 = myList2.GetAt(myList2.FindIndex(i+1));
			if((temp2.x-temp1.y)<=(right - left)/40){
				temp1.y = temp2.y;
				myList2.RemoveAt(myList2.FindIndex(i));
				myList2.RemoveAt(myList2.FindIndex(i));
				if(i == 0)			myList2.AddHead(temp1);
				else			    myList2.InsertAfter(myList2.FindIndex(i-1),temp1);
				doJoin = true;
				break;
			}	
		}
		if(!doJoin)	quit = 0;
	}
	//左右眼的水平区域
	CPoint LEyeAreaH(-1,-1),REyeAreaH(-1,-1),LEyeAreaV(-1,-1),REyeAreaV(-1,-1);
	//没有找到眼睛区域
	if(myList2.GetCount()<2){
		CPoint t=myList2.GetHead();
		if((t.y-t.x)>(right - left)/2){
			LEyeAreaH.x = t.x; 
			LEyeAreaH.y = t.x+(t.y-t.x)/3; 
			REyeAreaH.x = t.y-(t.y-t.x)/3;
			REyeAreaH.y = t.y; 
		}
		else{
			AfxMessageBox("确认眼睛位置失败，请手动标定");
			return;
		}
	}
	//仅有两个区域
	else if(myList2.GetCount()==2){
		LEyeAreaH = myList2.GetHead();
		REyeAreaH = myList2.GetTail();
	}
	else{  //多于两个区域
		int ldis = -100000;
		int rdis = 100000;	
		for(i=0; i<myList2.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myList2.GetAt(myList2.FindIndex(i));
			//右眼
			if((temp.x+temp.y)/2 > nCenterH){
				if(((temp.x+temp.y)/2-nCenterH)<rdis){
					rdis = (temp.x+temp.y)/2-nCenterH;
					REyeAreaH = temp;
				}
			}
			//左眼
			else{
				if(((temp.x+temp.y)/2-nCenterH)>ldis){
					ldis = (temp.x+temp.y)/2-nCenterH;
					LEyeAreaH = temp;
				}
			}
		}
	}
	myList2.RemoveAll();
	///////////////////////////////////
	//确认两个眼睛的垂直区域
	//////////////////////////////////
	//左眼
	if(LEyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++)
			for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/100){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定左眼的位置");
		}
		else{
			LEyeAreaV = myListB.GetHead();
			
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_LeftEyeLeftCorner.x = 100000;
			m_LeftEyeRightCorner.x = -1;
			
			for(i=LEyeAreaV.x; i<= LEyeAreaV.y;i++)
				for(j=LEyeAreaH.x; j<=LEyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_LeftEyeLeftCorner.x){
							m_LeftEyeLeftCorner.x = j;
							m_LeftEyeLeftCorner.y = i;
						}
						if(j>m_LeftEyeRightCorner.x){
							m_LeftEyeRightCorner.x = j;
							m_LeftEyeRightCorner.y = i;
						}
					sumX += j;
					sumY += i;
					sum++;
					}
			m_LeftEye.x = (int)(sumX/sum);
			m_LeftEye.y = (int)(sumY/sum);	
		}
		myListB.RemoveAll();
	}
	//右眼
	if(REyeAreaH != CPoint(-1,-1)){
		int *tArray = new int[pdib1->m_imgHeight]; 
		int i,j;
		for(i = 0; i<pdib1->m_imgHeight; i++) tArray[i] = 0;
		
		for(i=nCenterV+nSubWinHeight; i <bottom-6*nSubWinHeight; i++)
			for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
				if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0)
					tArray[i] ++;
				
		CList<CPoint,CPoint&> myListA(sizeof(CPoint));
		CList<CPoint,CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1,-1);
		for(i = nCenterV+nSubWinHeight; i < bottom-6*nSubWinHeight; i++){
			if(tArray[i] > 0 && flag ==0){
				tPoint.x = i;
				flag = 1;
			}
			if(tArray[i] == 0 && flag ==1){
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for(i=0; i<myListA.GetCount();i++){
			CPoint temp(-1,-1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top)/100;
			if((temp.y-temp.x)>=minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while(quit){
			bool doJoin = false;
			for(int i=0; i<myListB.GetCount()-1;i++){
				CPoint temp1(-1,-1),temp2(-1,-1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i+1));
				if((temp2.x-temp1.y)<=(bottom - top)/50){
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if(i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i-1),temp1);
					doJoin = true;
					break;
				}	
			}
			if(!doJoin)	quit = 0;
		}
		if(myListB.GetCount()==0){
			AfxMessageBox("无法确定右眼的位置");
		}
		else{
			if(myListB.GetCount()==1)
				REyeAreaV = myListB.GetHead();
			else{
				CPoint tt =  myListB.GetHead();
				int index = myListB.GetCount();
				while(tt.y > LEyeAreaV.x && index > 0){
					index --;
					tt = myListB.GetAt(myListB.FindIndex(myListB.GetCount()-index)); 
				}
				REyeAreaV = tt;		
			}
		
			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_RightEyeLeftCorner.x = 100000;
			m_RightEyeRightCorner.x = -1;
			
			for(i=REyeAreaV.x; i<=REyeAreaV.y;i++)
				for(j=REyeAreaH.x; j<=REyeAreaH.y;j++)
					if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255 && *(pdib1->m_pImgData+i*lineByte1+j*3+1) == 0){
						if(j<m_RightEyeLeftCorner.x){
							m_RightEyeLeftCorner.x = j;
							m_RightEyeLeftCorner.y = i;
						}
						if(j>m_RightEyeRightCorner.x){
							m_RightEyeRightCorner.x = j;
							m_RightEyeRightCorner.y = i;
						}
					sumX += j;
					sumY += i;
					sum++;
					}
			m_RightEye.x = (int)(sumX/sum);
			m_RightEye.y = (int)(sumY/sum);
		}
		myListB.RemoveAll();
	}
	
	for(i=top; i<=bottom; i++)
		for(j=left; j<=right; j++){
			BYTE R,G,B;
			double temp,dlta;
			R = *(pdib1->m_pImgData+i*lineByte1+j*3+2);
			G = *(pdib1->m_pImgData+i*lineByte1+j*3+1);
			B = *(pdib1->m_pImgData+i*lineByte1+j*3);
			if((R==G) && (G==B)) temp = 0;
			else temp = 0.5*(2*R-G-B)/sqrt((R-G)*(R-G)+(R-B)*(G-B));
			dlta = acos(temp);
			if(dlta < 0.2){
				*(pdib1->m_pImgData+i*lineByte1+j*3+2) = 255;  
			}
			else *(pdib1->m_pImgData+i*lineByte1+j*3+2) = 0;
	
	}
		
	AfxMessageBox("右脸的肤色鉴定");

	//双目斜角
	double tanThta;
	if(m_RightEye.y == m_LeftEye.y) tanThta = 0;
	else tanThta = (m_RightEye.y - m_LeftEye.y)/(m_RightEye.x - m_LeftEye.x);
	//双目距离
	int EyesDis = (m_RightEye.x-m_LeftEye.x)*(m_RightEye.x-m_LeftEye.x);
	EyesDis += (m_RightEye.y-m_LeftEye.y)*(m_RightEye.y-m_LeftEye.y);
	EyesDis = (int)sqrt(EyesDis);
	//双目平均高度
	int EyeV    = (m_RightEye.y + m_LeftEye.y)/2;
	//可能的嘴的区域
	int MouthUp   = (EyeV-1.0*EyesDis) < top ? top:(int)(EyeV-1.0*EyesDis);
	int MouthDown = (EyeV-1.5*EyesDis) < top ? top:(int)(EyeV-1.5*EyesDis);

	
	int* Y_Arry = new int[MouthUp-MouthDown];
	for(i =0 ;i < MouthUp-MouthDown ;i++) Y_Arry[i] = 0;
	int* X_Arry = new int[EyesDis];
	for(i =0 ;i < EyesDis ;i++) X_Arry[i] = 0;
	for(i = MouthDown ; i < MouthUp; i++)
		for(j = m_LeftEye.x; j< m_RightEye.x; j++){
			if(*(pdib1->m_pImgData+i*lineByte1+j*3+2) == 255){
					Y_Arry[i-MouthDown] ++;
					X_Arry[j-m_LeftEye.x] ++;
			}
	}
			
	int maxY = 0;
	for(i =0 ;i < MouthUp-MouthDown ;i++){
		if(Y_Arry[i]>maxY){
			maxY = Y_Arry[i];
			m_MidMouth.y =  i+MouthDown - (MouthUp-MouthDown)/10;
		}
	}
	m_LeftMouthCorner.y  =(int)(m_MidMouth.y - tanThta*EyesDis/2);
	m_RightMouthCorner.y =(int)(m_MidMouth.y + tanThta*EyesDis/2);
	for(i =0 ;i < EyesDis ;i++){
		if(X_Arry[i]>0){
			m_LeftMouthCorner.x = i+m_LeftEye.x;
			break;
		}
	}
	for(i = EyesDis -1; i >=0 ;i--){
		if(X_Arry[i]>0){
			m_RightMouthCorner.x = m_LeftEye.x+i;
			break;
		}
	}
			
	//唇中点较薄
	int min = 1000000;
	for(i = (int)(3*EyesDis/7+0.5) ; i <= (int)(4*EyesDis/7+0.5);i++){
		if(X_Arry[i]<min){
			min = X_Arry[i];
			m_MidMouth.x = m_LeftEye.x+i;
		}
	}
	m_MidMouth.x = (int)(m_MidMouth.x+(m_LeftEye.x+EyesDis/2))/2;

	memcpy(pdib1->m_pImgData,m_pImgDatabak,pdib1->m_imgHeight*lineByte1);

	//建立定位区域新视图
	F_top=m_MidMouth.y;
	F_bottom=EyeV-0.15*EyesDis;
    F_left=m_RightMouthCorner.x;
	F_right=m_RightEyeRightCorner.x+0.3*EyesDis;

    FaceDetect BoundaryDetect(pdib1->GetDimensions(),pdib1->m_nBitCount,
		                      pdib1->m_lpColorTable, pdib1->m_pImgData,NULL);
    BoundaryDetect.ImageCut(F_top,F_bottom,F_left,F_right);

	//建立一个新视图，显示结果
	pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(BoundaryDetect.GetDimensions(),BoundaryDetect.m_nBitCountOut,BoundaryDetect.m_lpColorTableOut, BoundaryDetect.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);		
}

void CDemo1View:: OnHomomorphicfilter()
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib  *pDib=pDoc->m_dib;
	
	//只处理彩色图像
	if(pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理彩色图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	

	FaceDetect imagefreq(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData,NULL);
	
	imagefreq.Homomorphicfilter();
	
	//建立一个新视图，显示结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//发送新建文件的消息，创建一个新的文档-视图
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	//获取新建视图指针
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	
	//获取相关联的新的文档类指针
	CDemo1Doc* pDocNew=pView->GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *dibNew=pDocNew->GetPDib();

	//将变换后的输出图像作为新建文档的DIB进行显示
	dibNew->ReplaceDib(imagefreq.GetDimensions(),imagefreq.m_nBitCountOut,imagefreq.m_lpColorTableOut, imagefreq.m_pImgDataOut);
	
	//设置滚动窗口
	pView->OnInitialUpdate();


	//文档数据置脏，提示存盘信息
	pDocNew->SetModifiedFlag(TRUE);

	//各视图刷新显示
	pDocNew->UpdateAllViews(pView);

}


void CDemo1View::OnAdaptivethresegment() 
{
	// TODO: Add your command handler code here
    CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义FaceDetect类的对象facedetect
	FaceDetect facedetect(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData,NULL);
	facedetect.Adaptivethresegment();
	
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(facedetect.GetDimensions(),facedetect.m_nBitCountOut,facedetect.m_lpColorTableOut, facedetect.m_pImgDataOut);

	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);


	
	
}

void CDemo1View::OnDWThomomorphicfilter() 
{
	// TODO: Add your command handler code here
    	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib  *pDib=pDoc->m_dib;
	
	//只处理彩色图像
	if(pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理彩色图像",MB_OK,0);
		return ;
	}
	
	
	FaceDetect imagefreq(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData,NULL);
	
	imagefreq.DWThomomorphicfilter();
	
	//建立一个新视图，显示结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//发送新建文件的消息，创建一个新的文档-视图
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	//获取新建视图指针
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	
	//获取相关联的新的文档类指针
	CDemo1Doc* pDocNew=pView->GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *dibNew=pDocNew->GetPDib();

	//将变换后的输出图像作为新建文档的DIB进行显示
	dibNew->ReplaceDib(imagefreq.GetDimensions(),imagefreq.m_nBitCountOut,imagefreq.m_lpColorTableOut, imagefreq.m_pImgDataOut);
	
	//设置滚动窗口
	pView->OnInitialUpdate();


	//文档数据置脏，提示存盘信息
	pDocNew->SetModifiedFlag(TRUE);

	//各视图刷新显示
	pDocNew->UpdateAllViews(pView);	
}

void CDemo1View::OnGaryImage() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.GaryImage();
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}
