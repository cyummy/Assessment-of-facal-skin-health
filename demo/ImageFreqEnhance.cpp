// ImageFreqEnhance.cpp: implementation of the ImageFreqEnhance class.
//
//////////////////////////////////////////////////////////////////////
#include "stdio.h"
#include "stdafx.h"
#include "demo1.h"
#include "ImageFreqEnhance.h"
#include "math.h"
#include <direct.h>
#include <complex>
#include <algorithm>

using namespace std;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageFreqEnhance::ImageFreqEnhance()
{
    m_pImgDataOut=NULL;//输出图像位图数据指针为空
	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0
    m_nBitCountOut=0;//输出图像每像素位数为0	
	m_imgWidthOut=0;//输出图像的宽为0
	m_imgHeightOut=0;//输出图像的高为0	
}

ImageFreqEnhance::ImageFreqEnhance(CSize size, int nBitCount, 
						   LPRGBQUAD lpColorTable,unsigned char *pImgData):
						   ImageDib(size, nBitCount, lpColorTable, pImgData)
{
	//输出图像每像素位数，与输入图像相同，都为灰度图像
	m_nBitCountOut=m_nBitCount;
	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	//为输出图像申请颜色表缓冲区，并将原图像输入图像和输出图像均为灰度格式，
	//故在构造函数中直接对颜色表进行拷贝处理
	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
	memcpy(m_lpColorTableOut,m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	
	m_pImgDataOut=NULL;//输出图像指针为空

}

ImageFreqEnhance::~ImageFreqEnhance()
{
    //释放输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	//释放输出图像颜色表缓冲区
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	
}


/***********************************************************************
* 函数名称：InputImgData()
*函数参数：
*  CSize size -输入图像大小（宽、高）
*  int nBitCount  -每像素位数
*  LPRGBQUAD lpColorTable  -颜色表指针
*  unsigned char *pImgData  -位图数据指针
*返回值：无
*说明：输入图像数据，与带参数构造函数类似，调用基类成员函数ReplaceDib创建
*     输入图像的DIB结构，并清理缓冲区，初始化输出图像的颜色表，为图像快
*     速傅立叶变换做准备
***********************************************************************/
void ImageFreqEnhance::InputImageData(CSize size, int nBitCount, 
						LPRGBQUAD lpColorTable, unsigned char *pImgData)
{
	//调用基类成员函数ReplaceDib为创建输入图像的DIB结构
	ReplaceDib(size, nBitCount,  lpColorTable, pImgData);

	//清理缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}

	//输出图像每像素位数与输入图像相同
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	 m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	 //申请输出图像颜色表缓冲区m_lpColorTableOut，并将输入图像颜色表数据拷贝
	 //至m_lpColorTableOut
	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
	memcpy(m_lpColorTableOut,m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
}

/***********************************************************************
* 函数名称：
* GetDimensions()
*函数参数：无
*返回值：图像的尺寸，用CSize类型表达
*说明：返回输出图像的宽和高
***********************************************************************/
CSize ImageFreqEnhance::GetDimensions()
{
	return CSize(m_imgWidthOut, m_imgHeightOut);
}



void ImageFreqEnhance::IdealLowPassFilter(int nWidth, int nHeight, int nRadius)
{
    // 循环控制变量
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth ;
	int nTransHeight;

	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;

/*	// 傅立叶变换的实部和虚部
	double dReal;
	double dImag;
	unsigned char unchValue;	// 图象象素值
*/
	// 低通滤波的半径不能超过频域的最大半径
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	//输出图像的大小与输入图像相同
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//申请输出图像缓冲区，并初始化为0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//申请傅立叶缓冲区，并初始化为0
	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	//输入图像数据进行二维傅立叶变换
	FFtTrans.ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);
   
// 下面开始实施低通滤波，把所有大于nRadius的高频分量设置为0
	//  注意这里高频分量采用的范数不是欧式距离，而是无穷大范数
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for(y=nRadius; y<nTransHeight; y++)
	{
		for(x=nRadius; x<nTransWidth; x++)
		{
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].real=0;
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag=0;
		}
	}
 
	// 经过低通滤波的图象进行反变换
	FFtTrans.ImageRevFFt2D(m_pImgDataOut, nWidth, nHeight);
/*	
	// 图像的亮度增强
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dReal = FFtTrans.m_pFFTBuf[y*nTransWidth + x].real;
			dImag = FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag;
			unchValue = (unsigned char)max(0,min(255,sqrt(dReal*dReal+dImag*dImag) ));
			m_pImgDataOut[y*nWidth + x] = unchValue ;
		}
	}*/

}



void ImageFreqEnhance::ButterLowPassFilter(int nWidth, int nHeight, int nRadius)
{
	// 循环控制变量
	int y ;
	int x ;
	
	double dTmpOne ;
	double dTmpTwo ;
	double H;
	
/*	
	// 傅立叶变换的实部和虚部
	double dReal;
	double dImag;
	unsigned char unchValue;	// 图象象素值
*/	
	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth ;
	int nTransHeight;
	
	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// 低通滤波的半径不能超过频域的最大半径
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	//输出图像的大小与输入图像相同
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	
	//申请输出图像缓冲区，并初始化为0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//申请傅立叶缓冲区，并初始化为0
	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	//输入图像数据进行二维傅立叶变换
	FFtTrans.ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);
	
	
	// 下面开始实施ButterWorth低通滤波
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			H = (double)(y*y+x*x) ; 
			H = H / (nRadius * nRadius); 
			H = 1/(1+H);
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].real=(FFtTrans.m_pFFTBuf[y*nTransWidth + x].real)*H;
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag=(FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag)*H;
		}
	}

	// 经过低通滤波的图象进行反变换
	FFtTrans.ImageRevFFt2D(m_pImgDataOut, nWidth, nHeight);
	/*	
	// 图像的亮度增强
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dReal = FFtTrans.m_pFFTBuf[y*nTransWidth + x].real;
			dImag = FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag;
			unchValue = (unsigned char)max(0,min(255,sqrt(dReal*dReal+dImag*dImag)));
			m_pImgDataOut[y*nWidth + x] = unchValue;
		}
	}*/
}


void ImageFreqEnhance::IdealHighPassFilter(int nWidth, int nHeight, int nRadius)
{
	// 循环控制变量
	int y ;
	int x ;
	
	double dTmpOne ;
	double dTmpTwo ;
	
/**/	
	// 傅立叶变换的实部和虚部
	double dReal;
	double dImag;
	unsigned char unchValue;	// 图象象素值
	
	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth ;
	int nTransHeight;
	
	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// 低通滤波的半径不能超过频域的最大半径
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	//输出图像的大小与输入图像相同
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	
	//申请输出图像缓冲区，并初始化为0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//申请傅立叶缓冲区，并初始化为0
	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	//输入图像数据进行二维傅立叶变换
	FFtTrans.ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);
	
	
	// 下面开始实施理想高通滤波
	for(y=0; y<nRadius ; y++)
	{
		for(x=0; x<nRadius; x++)
		{
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].real=0;
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag=0;
		}
	}

	// 经过高通滤波的图像进行反变换
	FFtTrans.ImageRevFFt2D(m_pImgDataOut, nWidth, nHeight);
	/*	*/
	// 图像的亮度增强
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dReal = FFtTrans.m_pFFTBuf[y*nTransWidth + x].real;
			dImag = FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag;
			unchValue = (unsigned char)max(0,min(255,sqrt(dReal*dReal+dImag*dImag)+100));
			m_pImgDataOut[y*nWidth + x] = unchValue ;
		}
	}
}


void ImageFreqEnhance::ButterHighPassFilter(int nWidth, int nHeight, int nRadius) 
{
	// 循环控制变量
	int y ;
	int x ;
	
	double dTmpOne ;
	double dTmpTwo ;
	double H;
	
/*	*/
	// 傅立叶变换的实部和虚部
	double dReal;
	double dImag;
	unsigned char unchValue;	// 图象象素值
	
	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth ;
	int nTransHeight;
	
	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// 低通滤波的半径不能超过频域的最大半径
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	//输出图像的大小与输入图像相同
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	
	//申请输出图像缓冲区，并初始化为0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//申请傅立叶缓冲区，并初始化为0
	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	//输入图像数据进行二维傅立叶变换
	FFtTrans.ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);

	// 下面开始实施ButterWorth低通滤波
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			H = (double)(y*y+x*x) ; 
		    H = (nRadius * nRadius) / H; 
			H = 1/(1+H);
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].real=(FFtTrans.m_pFFTBuf[y*nTransWidth + x].real)*H;
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag=(FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag)*H;
		}
	}

	// 经过低通滤波的图象进行反变换
	FFtTrans.ImageRevFFt2D(m_pImgDataOut, nWidth, nHeight);
	/*	*/
	// 图像的亮度增强
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dReal = FFtTrans.m_pFFTBuf[y*nTransWidth + x].real;
			dImag = FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag;
			unchValue = (unsigned char)max(0,min(255,sqrt(dReal*dReal+dImag*dImag)+100));
			m_pImgDataOut[y*nWidth + x] = unchValue;
		}
	}
}




