// FourierTrans.cpp: implementation of the FourierTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "FourierTrans.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

 FourierTrans::FourierTrans()
{
    m_pImgDataOut=NULL;//输出图像位图数据指针为空
	
	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0
	
    m_nBitCountOut=0;//输出图像每像素位数为0	
	
	m_imgWidthOut=0;//输出图像的宽为0
	
	m_imgHeightOut=0;//输出图像的高为0	
	
	m_pFFTBuf=NULL;//傅立叶变换缓冲区为空
}

FourierTrans::FourierTrans(CSize size, int nBitCount, 
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
	
	m_pFFTBuf=NULL;//傅立叶变换缓冲区为空
}

FourierTrans::~FourierTrans()
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
	
	//释放傅立叶变换缓冲区
	if(m_pFFTBuf!=NULL){
		delete m_pFFTBuf;
		m_pFFTBuf=NULL;
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
void FourierTrans::InputImgData(CSize size, int nBitCount, 
						LPRGBQUAD lpColorTable, unsigned char *pImgData)
{
	//调用基类成员函数ReplaceDib为创建输入图像的DIB结构
	ReplaceDib(size, nBitCount,  lpColorTable, pImgData);

	//清理缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_pFFTBuf!=NULL){
		delete m_pFFTBuf;
		m_pFFTBuf=NULL;
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
CSize FourierTrans::GetDimensions()
{
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

/***********************************************************************
* 函数名称：Fourier()
*函数参数： 无
*返回值： 无
*说明：对输入图像进行快速傅立叶变换，要求输入图像的宽和高必须是2的幂次方
***********************************************************************/
void FourierTrans::Fourier()
{
	//输出图像的大小与输入图像相同
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//申请输出图像缓冲区，并初始化为0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//申请傅立叶缓冲区，并初始化为0
	m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);

	//输入图像数据进行二维傅立叶变换
	ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);
}

/***********************************************************************
* 函数名称：RevFourier()
* 函数参数：无
* 返回值：  无
* 说明：输入图像的快速傅立叶反变换，与ImageFFT()成对调用
******************************************************************/
void FourierTrans::RevFourier()
{
	//图像的二维傅立叶反变换,变换结果即是恢复的原图像，存入m_pImgDataOut
	ImageRevFFt2D(m_pImgDataOut, m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* 函数名称：ImageFFt1D()
* 函数参数：ComplexNumber *arrayBuf  -复数结构体类型数组
*           int n  
*           数组arrayBuf的长度，必须为2的幂次方
*返回值：   无
*说明：对复数结构体数组arrayBuf进行一维快速傅立叶变换，变换后的结果仍
*      存回arrayBuf中
***********************************************************************/
void FourierTrans::ImageFFt1D(ComplexNumber *arrayBuf, int n)
{
	//循环变量
	int i, k, r;
	
	//申请临时复数缓冲区buf1，长度为n
	ComplexNumber *buf1=new ComplexNumber[n];
	
	//将arrayBuf拷贝进buf1
	memcpy(buf1,arrayBuf,sizeof(ComplexNumber)*n);
	
	//申请临时复数缓冲区buf2，长度为n
	ComplexNumber *buf2=new ComplexNumber[n];
	
	//将arrayBuf数组元素基2抽取并重新排列
	//若0、1、2、3、4、5、6、7八点序列对调后变作0、4、2、6、1、5、3、7
	int t1,t2;
	for(r=1;pow(2,r)<n;r++){
		t1=pow(2,r);
		t2=pow(2,r-1);
		for(k=0;k<t1;k++){
			for(i=0;i<n/t1;i++){
				buf2[k*n/t1+i].real=buf1[k/2*n/t2+i*2+k%2].real;
				buf2[k*n/t1+i].imag=buf1[k/2*n/t2+i*2+k%2].imag;
			}
		}
		memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
	}
	
	
	//采用蝶型算法进行快速傅立叶变换
	//buf1是第r级的输入，buf2存放第r级的输出
	float c,s;
	for(r=1;pow(2,r)<=n;r++){
		t1=pow(2,r);
		for(k=0;k<n/t1;k++){
			for(i=t1/2;i<t1;i++){
				//加权因子
				c=cos(-2*PI*(i-t1/2)/t1);
				s=sin(-2*PI*(i-t1/2)/t1);
				buf1[k*t1+i].real=buf2[k*t1+i].real*c-buf2[k*t1+i].imag*s;
				buf1[k*t1+i].imag=buf2[k*t1+i].imag*c+buf2[k*t1+i].real*s;
			}
		}
		for(k=0; k<n/t1; k++){
			for(i=0;i<t1/2;i++){
				buf2[k*t1+i].real= buf1[k*t1+i].real+buf1[k*t1+i+t1/2].real;
				buf2[k*t1+i].imag= buf1[k*t1+i].imag+buf1[k*t1+i+t1/2].imag;
			}
			for(i=t1/2;i<t1;i++){
				buf2[k*t1+i].real= buf1[k*t1+i-t1/2].real-buf1[k*t1+i].real;
				buf2[k*t1+i].imag= buf1[k*t1+i-t1/2].imag-buf1[k*t1+i].imag;
			}
		}
		
		//第r级的输出存入buf1,作为下一级的输入数据
		memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
	}
	//傅立叶变换的结果存入arrayBuf
	memcpy(arrayBuf,buf2,sizeof(ComplexNumber)*n);
	
	//释放缓冲区
	delete[]buf2;
	delete[]buf1;
	
}

/***********************************************************************
* 函数名称：ImageFFt2D()
* 函数参数：unsigned char* imgBuf  -图像数据指针
*           int width  -图像宽
*           int height  -图像高
*           unsigned char *imgBufOut  -傅立叶变换后的输出图像
* 返回值：  无
* 说明：图像数据二维快速傅立叶变换将图像数据变成复数形式,进行两个一维快速
*      傅立叶变换,变换后的频谱以图像形式存入imgBufOut，此处要求图像宽和高
*      都为2的幂次方
***********************************************************************/
void FourierTrans::ImageFFt2D(unsigned char* imgBuf, int width, int height,
							 unsigned char *imgBufOut)
{
	//循环变量
	int i, j, u, v;
	
	//图像数据变成复数类型存入m_pFFTBuf
	for(i=0;i<width*height;i++){
		m_pFFTBuf[i].real=imgBuf[i];
		m_pFFTBuf[i].imag=0;
	}
	
	//申请ComplexNumber结构体数组,长度为height
	ComplexNumber *array=new ComplexNumber[height];
	
	//先纵向一维快速傅立叶变换
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v].real=m_pFFTBuf[v*width+u].real;
			array[v].imag=m_pFFTBuf[v*width+u].imag;
		}
		ImageFFt1D(array, height);
		for(v=0;v<height;v++){
			m_pFFTBuf[v*width+u].real=array[v].real;
			m_pFFTBuf[v*width+u].imag=array[v].imag;
		}
	}
	delete []array;
	
	//再横向一维快速傅立叶变换
	for(v=0;v<height;v++){
		ImageFFt1D(m_pFFTBuf+v*width, width);
	}
	
	//将频谱图以图像形式存入imgBufOut
	float t;
	int i0,j0;
	for(i=0;i<height;i++){
		//i0 = i;
		//j0 = j;
		for(j=0;j<width;j++){
			if(i<height/2)
				i0=i+height/2;
			else
				i0=i-height/2;
			if(j<width/2)
				j0=j+width/2;
			else
				j0=j-width/2;
			
			t=sqrt(m_pFFTBuf[i0*width+j0].real*m_pFFTBuf[i0*width+j0].real
				+m_pFFTBuf[i0*width+j0].imag*m_pFFTBuf[i0*width+j0].imag);
			t=t/500;
			if(t>255)
				imgBufOut[i*width+j]=255;
			else 
				imgBufOut[i*width+j]=t;
		}
	}
	
}



/***********************************************************************
* 函数名称：ImageRevFFt2D()
*函数参数： unsigned char* imgBufOut  -输出图像数据指针
*           int width  -图像宽
*           int height  -图像高
*返回值：   无
*说明：对傅立叶变换缓冲区m_pFFTBuf里的数据,进行两个一维快速傅立叶反变换,
*      变换后的频谱即是原图像的恢复数据,将恢复数据存入imgBufOut，此处要求
*      图像宽和高均为2的幂次方    
***********************************************************************/
void FourierTrans::ImageRevFFt2D(unsigned char* imgBufOut, int width, int height)
{
	//若傅立叶变换缓冲区为空,则失败返回
	if(!m_pFFTBuf)
		return;

	//变量
	int i, u, v;
	
	//先纵向傅立叶反变换
	ComplexNumber *array=new ComplexNumber[height];
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v].real=m_pFFTBuf[v*width+u].real;
			array[v].imag=m_pFFTBuf[v*width+u].imag;
		}
		ImageRevFFt1D(array, height);
		for(v=0;v<height;v++){
			m_pFFTBuf[v*width+u].real=array[v].real;
			m_pFFTBuf[v*width+u].imag=array[v].imag;
		}
	}
	delete []array;

	//再横向傅立叶反变换
	for(v=0;v<height;v++){
		ImageRevFFt1D(m_pFFTBuf+v*width, width);
	}

	//求m_pFFTBuf的幅度谱,恢复图像存入imgBufOut
	float t;
	for(i=0;i<width*height;i++){
		t=sqrt(m_pFFTBuf[i].real*m_pFFTBuf[i].real
			         +m_pFFTBuf[i].imag*m_pFFTBuf[i].imag);
		if(t>255)
			imgBufOut[i]=255;
		else imgBufOut[i]=t;
	}
}

/***********************************************************************
* 函数名称：ImageRevFFt1D()
* 函数参数：ComplexNumber *arrayBuf  -复数结构体类型数组
*           int n  -数组arrayBuf的长度，必须为2的幂次方
* 返回值：  无
*说明：对复数结构体数组arrayBuf进行一维快速傅立叶反变换，变换后的结果仍
*      存回arrayBuf中
***********************************************************************/
void FourierTrans::ImageRevFFt1D(ComplexNumber *arrayBuf, int n)
{
	//变量
	int i;

	//对数组arrayBuf求共轭
	for(i=0;i<n;i++)
		arrayBuf[i].imag=-arrayBuf[i].imag;

	//一维快速傅立叶变换
	ImageFFt1D(arrayBuf, n);

	//时域数据求共轭，并除以N
	for(i=0;i<n;i++){
		arrayBuf[i].real=arrayBuf[i].real/n;
		arrayBuf[i].imag=-arrayBuf[i].imag/n;
	}

}