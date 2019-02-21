// HistSegmentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "HistSegmentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HistSegmentDlg dialog


HistSegmentDlg::HistSegmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(HistSegmentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(HistSegmentDlg)
	m_threshchange = 128;
	//}}AFX_DATA_INIT
	for(int i=0;i<256;i++)
		m_histArray[i]=0;
	
	m_pImgDataOut=NULL;//输出图像位图数据指针为空
	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0
    m_nBitCountOut=0;//输出图像每像素位数为0
	m_imgWidthOut=0;//输出图像的宽为0
	m_imgHeightOut=0;//输出图像的高为0

}


void HistSegmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HistSegmentDlg)
	DDX_Slider(pDX, IDC_SLIDER_GrayValue, m_threshchange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HistSegmentDlg, CDialog)
	//{{AFX_MSG_MAP(HistSegmentDlg)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HistSegmentDlg message handlers

HistSegmentDlg::~HistSegmentDlg()
{

	//释放输出图像位图数据缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	
	//释放输出图像颜色表
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

}

BOOL HistSegmentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	
//	// TODO: Add extra initialization here
	m_nBitCountOut=m_himageDib.m_nBitCount;
    m_imgWidthOut=m_himageDib.m_imgWidth;
    m_imgHeightOut=m_himageDib.m_imgHeight;
	//将静态框1移至rect指定位置，用来画直方图
	CRect rect;
	rect=CRect(100,60,350,170);
	GetDlgItem(IDC_STATIC_Hist)->MoveWindow(&rect);
	
	//将进度条移至制定位置
	rect=CRect(100,195,350,228);
	GetDlgItem(IDC_SLIDER_GrayValue)->MoveWindow(&rect);

	//将颜色提示条移至指定位置
	rect=CRect(100,235,350,268);
	GetDlgItem(IDC_STATIC_GrayColor)->MoveWindow(&rect);
	
	//将文本框移至指定位置
	rect=CRect(10,195,95,228);
	GetDlgItem(IDC_STATIC_text)->MoveWindow(&rect) ;
	rect=CRect(350,195,420,228);
	GetDlgItem(IDC_STATIC_Value)->MoveWindow(&rect) ;

	
//	// 如果是灰度图像，则调用灰度直方图统计函数
	
	if(m_nBitCountOut==8)
		computeHistGray();
	else//彩色图像则统计亮度直方图
		computeHistBrightness();
    
	char szStr[10];
	CSliderCtrl *pSlider=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_GrayValue);
	pSlider->SetRange(0,255);
	pSlider->SetPos(m_threshchange);
	sprintf(szStr, "%d",m_threshchange);            
    SetDlgItemText(IDC_STATIC_Value, szStr); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void HistSegmentDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_nBitCountOut!= 8 && m_nBitCountOut!= 24) 
		return ;
	
	//获取直方图绘制静态框的矩形区域
	CRect clientRect;  
	GetDlgItem(IDC_STATIC_Hist)->GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);
	
	BeginWaitCursor();	    // Draw Back Ground
	//画背景
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(clientRect.left-1, clientRect.top-5,
		clientRect.right+5, clientRect.bottom+1);
	
	//画直方图灰度频率与直方图绘制矩形区域比例关系，画直方图
	unsigned int max=0;;
	for(int i=0;i<256;i++){
		if(m_histArray[i]>max)
			max=m_histArray[i];
	}
	DWORD bufTemp[256];
	double x=clientRect.Height();
	for(i=0;i<256;i++){
		bufTemp[i]=(unsigned long)((float)m_histArray[i]*x/max);
	}
	for(i=clientRect.left;i<=clientRect.right;i++){
		dc.MoveTo(i,clientRect.bottom);
		dc.LineTo(i,clientRect.bottom-bufTemp[i-clientRect.left]);
	}   
	
	CString str;
	SetBkMode(dc,1);      //字体背景设为透明
    str.Format("0");
	dc.TextOut(clientRect.left,clientRect.bottom+1,str);
	str.Format("255");
	dc.TextOut(clientRect.left+255,clientRect.bottom+1,str);
	


	//创建蓝色画笔
	CPen *pBluePen=new CPen;
	pBluePen->CreatePen(PS_SOLID,1,RGB(0,0,255));
	
	//选中当前蓝色画笔，并保存旧的画笔
	CGdiObject *pOldPen=dc.SelectObject(pBluePen);
	
	//在直方图中画出阈值线位置
	dc.MoveTo(m_threshchange+100,clientRect.bottom);
	dc.LineTo(m_threshchange+100,clientRect.top);
	
	//恢复旧画笔
	dc.SelectObject(pOldPen);
	// Do not call CDialog::OnPaint() for painting messages
}



/***********************************************************************
* 函数名称：
* GetDimensions()
*
*函数参数：
*  无
*
*返回值：
*   图像的尺寸，用CSize类型表达
*
*说明：返回输出图像的宽和高
***********************************************************************/
CSize HistSegmentDlg::GetDimensions()
{	
	m_imgWidthOut=m_himageDib.m_imgWidth;
	m_imgHeightOut=m_himageDib.m_imgHeight;
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* 函数名称：
* computeHistGray()
*
*说明：灰度图像统计直方图，m_histArray中存放了当前图像的统计数据
***********************************************************************/
void HistSegmentDlg::computeHistGray()
{
	
	//只处理灰度图像
	if(m_nBitCountOut!=8)
		return;

	//循环变量
	int i,j;

	//直方图数组清0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;
    
	//每行像素所占字节数
	int lineByte=(m_himageDib.m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//中间变量
	int temp;

	//统计灰度直方图
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			temp=*(m_himageDib.m_pImgData+i*lineByte+j);
		    m_histArray[temp]++;
		}
	}
}


/***********************************************************************
* 函数名称：
* computeHistBrightness()
*
*说明：彩色图像亮度直方图，m_histArray中存放了当前图像的亮度统计数据
***********************************************************************/
 
void HistSegmentDlg::computeHistBrightness()
{
	//彩色图像有效
	if(m_nBitCountOut!=24)
		return;

	//循环变量
	int i,j;

	//直方图数组清0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;
	//每行像素所占字节数
	int lineByte=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;

	//中间变量
	int temp;

	//统计亮度直方图
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			temp=0.11**(m_himageDib.m_pImgData+i*lineByte+j*3+0)
				+0.59**(m_himageDib.m_pImgData+i*lineByte+j*3+1)
				+0.30**(m_himageDib.m_pImgData+i*lineByte+j*3+2)+0.5;
		    m_histArray[temp]++;
		}
	}
}

                                                          


void HistSegmentDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	// 根据滑竿条位置更新阈值
	CSliderCtrl *pSlider=(CSliderCtrl *)pScrollBar;
	m_threshchange=pSlider->GetPos();
	
	//在静态框中显示调整的阈值
	char szStr[20];
	sprintf(szStr, "%d",m_threshchange);            
    SetDlgItemText(IDC_STATIC_Value, szStr);  
	//刷新显示
	Invalidate();
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

