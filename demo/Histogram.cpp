// Histogram.cpp: implementation of the Histogram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "Histogram.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/***********************************************************************
* 函数名称：
* Histogram()
*
*说明：无参数的构造函数，初始化成员变量
***********************************************************************/
Histogram::Histogram()
{
	//直方图数组清0
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

/***********************************************************************
* 函数名称：
* Histogram()
*
*函数参数：
*  CSize size -图像大小（宽、高）
*  int nBitCount  -每像素所占位数
*  LPRGBQUAD lpColorTable  -颜色表指针
*  unsigned char *pImgData  -位图数据指针
*
*返回值：
*   无
*
*说明：本函数为带参数的构造函数，给定位图的大小、每像素位数、颜色表
*      及位图数据，调用ImgDib()对基类成员初始化，并初始化派生类的
*      数据成员
***********************************************************************/
Histogram::Histogram(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImageDib(size, nBitCount, lpColorTable, pImgData)
{
	//直方图数组清0
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

/***********************************************************************
* 函数名称：
*   ~Histogram()

*
*说明：析构函数，释放资源
***********************************************************************/

Histogram::~Histogram()
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
CSize Histogram::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* 函数名称：
* computeHistGray()
*
*说明：灰度图像统计直方图，m_histArray中存放了当前图像的统计数据
***********************************************************************/
void Histogram::computeHistGray()
{
	//只处理灰度图像
	if(m_nBitCount!=8)
		return;

	//循环变量
	int i,j;

	//直方图数组清0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;

	//每行像素所占字节数
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//中间变量
	int temp;

	//统计灰度直方图
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			temp=*(m_pImgData+i*lineByte+j);
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
 
void Histogram::computeHistBrightness()
{
	//彩色图像有效
	if(m_nBitCount!=24)
		return;

	//循环变量
	int i,j;

	//直方图数组清0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;

	//每行像素所占字节数
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//中间变量
	int temp;

	//统计亮度直方图
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			temp=0.11**(m_pImgData+i*lineByte+j*3+0)
				+0.59**(m_pImgData+i*lineByte+j*3+1)
				+0.30**(m_pImgData+i*lineByte+j*3+2)+0.5;
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
float Histogram::computeAverage()
{
	int sum=0;
	for(int i=0;i<256;i++)
		sum+= i*m_histArray[i];
	m_average=(float)sum/(m_imgWidth*m_imgHeight);
    float m_aver=m_average;
    return m_aver;
}

/***********************************************************************
* 函数名称：
* computeDeviation()
*
*说明：计算直方图方差，直方图的统计特征
***********************************************************************/
float Histogram::computeDeviation()
{

	double deviation=0;
	for(int i=0;i<256;i++)
		deviation += (i-m_average)*(i-m_average)*m_histArray[i];

	deviation/=(m_imgWidth*m_imgHeight);
	m_deviation=sqrt(deviation);
    float m_devi=m_deviation;
	return m_devi;
}

/***********************************************************************
* 函数名称：
* histAve()
*
*说明：直方图均衡，该函数只对灰度图像有效
***********************************************************************/
void Histogram::histAve()
{
	// 只处理灰度
	if(m_nBitCount!=8)
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
	m_nBitCountOut=m_nBitCount;

	//计算颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//若有颜色表，输入图像的颜色表与输出图像颜色表相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占的字节数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

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
		map[i]=(double)sum*255/(m_imgWidth*m_imgHeight)+0.5;
	}
	
	//输出数据赋值
	for(i=0;i<m_imgHeight;i++)
	{
		for(j=0;j<m_imgWidth;j++)
		{		
			tmp=*(m_pImgData+i*lineByteOut+j);
			*(m_pImgDataOut+i*lineByteOut+j)=(int)map[tmp];
		}
	}

}