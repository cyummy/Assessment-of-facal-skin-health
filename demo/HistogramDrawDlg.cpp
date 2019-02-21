// HistogramDrawDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "HistogramDrawDlg.h"
#include "ImageDib.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HistogramDrawDlg dialog


HistogramDrawDlg::HistogramDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(HistogramDrawDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(HistogramDrawDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	for(int i=0;i<256;i++)
		m_histArray[i]=0;
	
	m_average=0;//均值为0
	m_deviation=0;//方差为0
	m_pImgDataOut=NULL;//输出图像位图数据指针为空
	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0
    m_nBitCountOut=0;//输出图像每像素位数为0
	m_imgWidthOut=0;//输出图像的宽为0
	m_imgHeightOut=0;//输出图像的高为0

}


void HistogramDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HistogramDrawDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HistogramDrawDlg, CDialog)
	//{{AFX_MSG_MAP(HistogramDrawDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HistogramDrawDlg message handlers
BOOL HistogramDrawDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		// TODO: Add extra initialization here

	m_nBitCountOut=m_himageDib.m_nBitCount;
    m_imgWidthOut=m_himageDib.m_imgWidth;
    m_imgHeightOut=m_himageDib.m_imgHeight;
	//将静态框1移至rect指定位置，用来画直方图
	CRect rect;
	rect=CRect(50,60,300,170);
	GetDlgItem(IDC_STATIC_Hist)->MoveWindow(&rect);
	
	//将颜色提示条移至指定位置
	rect=CRect(50,195,300,228);
	GetDlgItem(IDC_STATIC_GrayColor)->MoveWindow(&rect);

 
	// 如果是灰度图像，则调用灰度直方图统计函数

	if(m_nBitCountOut==8)
		computeHistGray();
	else//彩色图像则统计亮度直方图
		computeHistBrightness();
	
	//计算直方图的均值和方差
    float m_ave=computeAverage();
    float m_dev=computeDeviation();
	
	//将均值信息显示在均值静态框中
	char szStr[10];    
 	sprintf(szStr, "%.2f",m_ave); 
    SetDlgItemText(IDC_STATIC_AveShow, szStr);         
	
	//将方差信息显示在方差静态框中
    sprintf(szStr, "%.2f", m_dev);            
    SetDlgItemText(IDC_STATIC_DevShow, szStr); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void HistogramDrawDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//彩色和灰度图像有效
	if(m_nBitCountOut!= 8 && m_nBitCountOut!= 24) 
		return ;
	
	//获取直方图绘制静态框的矩形区域
	CRect clientRect;  
	GetDlgItem(IDC_STATIC_Hist)->GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);
	
	BeginWaitCursor();	    // Draw Back Ground
	//画背景
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(clientRect.left, clientRect.top-5,
		clientRect.right, clientRect.bottom+1);
	
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
	str.Format("50");
	dc.TextOut(clientRect.left+50,clientRect.bottom+1,str);
	str.Format("100");
	dc.TextOut(clientRect.left+100,clientRect.bottom+1,str);
	str.Format("150");
	dc.TextOut(clientRect.left+150,clientRect.bottom+1,str);
	str.Format("200");
	dc.TextOut(clientRect.left+200,clientRect.bottom+1,str);
	str.Format("255");
	dc.TextOut(clientRect.left+255,clientRect.bottom+1,str);
	
 	EndWaitCursor();
	// Do not call CDialog::OnPaint() for painting messages
}


HistogramDrawDlg::~HistogramDrawDlg()
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
CSize HistogramDrawDlg::GetDimensions()
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
void HistogramDrawDlg::computeHistGray()
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
 
void HistogramDrawDlg::computeHistBrightness()
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

                                                                  


/***********************************************************************
* 函数名称：
* computeAverage()
*
*说明：计算直方图均值，直方图的统计特征
***********************************************************************/
float HistogramDrawDlg::computeAverage()
{
	int sum=0;
	for(int i=0;i<256;i++)
		sum+= i*m_histArray[i];
	m_average=(float)sum/(m_himageDib.m_imgWidth*m_himageDib.m_imgHeight);
    float m_aver=m_average;
    return m_aver;
}

/***********************************************************************
* 函数名称：
* computeDeviation()
*
*说明：计算直方图方差，直方图的统计特征
***********************************************************************/
float HistogramDrawDlg::computeDeviation()
{

	double deviation=0;
	for(int i=0;i<256;i++)
		deviation += (i-m_average)*(i-m_average)*m_histArray[i];

	deviation/=(m_himageDib.m_imgWidth*m_himageDib.m_imgHeight);
	m_deviation=sqrt(deviation);
    float m_devi=m_deviation;
	return m_devi;
}


void HistogramDrawDlg::histogramAve()
{
  	// 只处理灰度
	if(m_himageDib.m_nBitCount!=8)
		return;

	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像每像素位数与输入图像相同
	m_nBitCountOut=m_himageDib.m_nBitCount;

	//计算颜色表长度
	m_nColorTableLengthOut=m_himageDib.ComputeColorTabalLength(m_nBitCountOut);

	
	//若有颜色表，输入图像的颜色表与输出图像颜色表相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_himageDib.m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_himageDib.m_imgWidth;
	m_imgHeightOut=m_himageDib.m_imgHeight;

	//输出图像每行像素所占的字节数
	int lineByteOut=(m_himageDib.m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_himageDib.m_imgHeight];

	//循环变量
	int i,j;

	//映射表
	double map[256];

	//中间变量
	int sum,tmp;

	//统计灰度直方图
	computeHistGray();

	//计算映射表
	sum=0;
	for(i=0;i<256;i++){
		sum+=m_histArray[i];
		map[i]=(double)sum*255/(m_himageDib.m_imgWidth*m_himageDib.m_imgHeight)+0.5;
	}
	
	//输出数据赋值
	for(i=0;i<m_himageDib.m_imgHeight;i++)
	{
		for(j=0;j<m_himageDib.m_imgWidth;j++)
		{		
			tmp=*(m_himageDib.m_pImgData+i*lineByteOut+j);
			*(m_pImgDataOut+i*lineByteOut+j)=(int)map[tmp];
		}
	}
}
