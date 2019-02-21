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
    m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0	
}

ImageFreqEnhance::ImageFreqEnhance(CSize size, int nBitCount, 
						   LPRGBQUAD lpColorTable,unsigned char *pImgData):
						   ImageDib(size, nBitCount, lpColorTable, pImgData)
{
	//���ͼ��ÿ����λ����������ͼ����ͬ����Ϊ�Ҷ�ͼ��
	m_nBitCountOut=m_nBitCount;
	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	//Ϊ���ͼ��������ɫ������������ԭͼ������ͼ������ͼ���Ϊ�Ҷȸ�ʽ��
	//���ڹ��캯����ֱ�Ӷ���ɫ����п�������
	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
	memcpy(m_lpColorTableOut,m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	
	m_pImgDataOut=NULL;//���ͼ��ָ��Ϊ��

}

ImageFreqEnhance::~ImageFreqEnhance()
{
    //�ͷ����ͼ�񻺳���
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	//�ͷ����ͼ����ɫ������
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	
}


/***********************************************************************
* �������ƣ�InputImgData()
*����������
*  CSize size -����ͼ���С�����ߣ�
*  int nBitCount  -ÿ����λ��
*  LPRGBQUAD lpColorTable  -��ɫ��ָ��
*  unsigned char *pImgData  -λͼ����ָ��
*����ֵ����
*˵��������ͼ�����ݣ�����������캯�����ƣ����û����Ա����ReplaceDib����
*     ����ͼ���DIB�ṹ����������������ʼ�����ͼ�����ɫ��Ϊͼ���
*     �ٸ���Ҷ�任��׼��
***********************************************************************/
void ImageFreqEnhance::InputImageData(CSize size, int nBitCount, 
						LPRGBQUAD lpColorTable, unsigned char *pImgData)
{
	//���û����Ա����ReplaceDibΪ��������ͼ���DIB�ṹ
	ReplaceDib(size, nBitCount,  lpColorTable, pImgData);

	//��������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}

	//���ͼ��ÿ����λ��������ͼ����ͬ
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	 m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	 //�������ͼ����ɫ������m_lpColorTableOut����������ͼ����ɫ�����ݿ���
	 //��m_lpColorTableOut
	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
	memcpy(m_lpColorTableOut,m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
}

/***********************************************************************
* �������ƣ�
* GetDimensions()
*������������
*����ֵ��ͼ��ĳߴ磬��CSize���ͱ��
*˵�����������ͼ��Ŀ�͸�
***********************************************************************/
CSize ImageFreqEnhance::GetDimensions()
{
	return CSize(m_imgWidthOut, m_imgHeightOut);
}



void ImageFreqEnhance::IdealLowPassFilter(int nWidth, int nHeight, int nRadius)
{
    // ѭ�����Ʊ���
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// ����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth ;
	int nTransHeight;

	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;

/*	// ����Ҷ�任��ʵ�����鲿
	double dReal;
	double dImag;
	unsigned char unchValue;	// ͼ������ֵ
*/
	// ��ͨ�˲��İ뾶���ܳ���Ƶ������뾶
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	//���ͼ��Ĵ�С������ͼ����ͬ
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//�������ͼ�񻺳���������ʼ��Ϊ0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//���븵��Ҷ������������ʼ��Ϊ0
	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	//����ͼ�����ݽ��ж�ά����Ҷ�任
	FFtTrans.ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);
   
// ���濪ʼʵʩ��ͨ�˲��������д���nRadius�ĸ�Ƶ��������Ϊ0
	//  ע�������Ƶ�������õķ�������ŷʽ���룬�����������
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for(y=nRadius; y<nTransHeight; y++)
	{
		for(x=nRadius; x<nTransWidth; x++)
		{
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].real=0;
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag=0;
		}
	}
 
	// ������ͨ�˲���ͼ����з��任
	FFtTrans.ImageRevFFt2D(m_pImgDataOut, nWidth, nHeight);
/*	
	// ͼ���������ǿ
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
	// ѭ�����Ʊ���
	int y ;
	int x ;
	
	double dTmpOne ;
	double dTmpTwo ;
	double H;
	
/*	
	// ����Ҷ�任��ʵ�����鲿
	double dReal;
	double dImag;
	unsigned char unchValue;	// ͼ������ֵ
*/	
	// ����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth ;
	int nTransHeight;
	
	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// ��ͨ�˲��İ뾶���ܳ���Ƶ������뾶
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	//���ͼ��Ĵ�С������ͼ����ͬ
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	
	//�������ͼ�񻺳���������ʼ��Ϊ0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//���븵��Ҷ������������ʼ��Ϊ0
	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	//����ͼ�����ݽ��ж�ά����Ҷ�任
	FFtTrans.ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);
	
	
	// ���濪ʼʵʩButterWorth��ͨ�˲�
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

	// ������ͨ�˲���ͼ����з��任
	FFtTrans.ImageRevFFt2D(m_pImgDataOut, nWidth, nHeight);
	/*	
	// ͼ���������ǿ
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
	// ѭ�����Ʊ���
	int y ;
	int x ;
	
	double dTmpOne ;
	double dTmpTwo ;
	
/**/	
	// ����Ҷ�任��ʵ�����鲿
	double dReal;
	double dImag;
	unsigned char unchValue;	// ͼ������ֵ
	
	// ����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth ;
	int nTransHeight;
	
	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// ��ͨ�˲��İ뾶���ܳ���Ƶ������뾶
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	//���ͼ��Ĵ�С������ͼ����ͬ
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	
	//�������ͼ�񻺳���������ʼ��Ϊ0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//���븵��Ҷ������������ʼ��Ϊ0
	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	//����ͼ�����ݽ��ж�ά����Ҷ�任
	FFtTrans.ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);
	
	
	// ���濪ʼʵʩ�����ͨ�˲�
	for(y=0; y<nRadius ; y++)
	{
		for(x=0; x<nRadius; x++)
		{
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].real=0;
			FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag=0;
		}
	}

	// ������ͨ�˲���ͼ����з��任
	FFtTrans.ImageRevFFt2D(m_pImgDataOut, nWidth, nHeight);
	/*	*/
	// ͼ���������ǿ
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
	// ѭ�����Ʊ���
	int y ;
	int x ;
	
	double dTmpOne ;
	double dTmpTwo ;
	double H;
	
/*	*/
	// ����Ҷ�任��ʵ�����鲿
	double dReal;
	double dImag;
	unsigned char unchValue;	// ͼ������ֵ
	
	// ����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth ;
	int nTransHeight;
	
	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// ��ͨ�˲��İ뾶���ܳ���Ƶ������뾶
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	//���ͼ��Ĵ�С������ͼ����ͬ
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	
	//�������ͼ�񻺳���������ʼ��Ϊ0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//���븵��Ҷ������������ʼ��Ϊ0
	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	//����ͼ�����ݽ��ж�ά����Ҷ�任
	FFtTrans.ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);

	// ���濪ʼʵʩButterWorth��ͨ�˲�
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

	// ������ͨ�˲���ͼ����з��任
	FFtTrans.ImageRevFFt2D(m_pImgDataOut, nWidth, nHeight);
	/*	*/
	// ͼ���������ǿ
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




