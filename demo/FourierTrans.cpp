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
    m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0
	
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	
	
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0	
	
	m_pFFTBuf=NULL;//����Ҷ�任������Ϊ��
}

FourierTrans::FourierTrans(CSize size, int nBitCount, 
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
	
	m_pFFTBuf=NULL;//����Ҷ�任������Ϊ��
}

FourierTrans::~FourierTrans()
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
	
	//�ͷŸ���Ҷ�任������
	if(m_pFFTBuf!=NULL){
		delete m_pFFTBuf;
		m_pFFTBuf=NULL;
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
void FourierTrans::InputImgData(CSize size, int nBitCount, 
						LPRGBQUAD lpColorTable, unsigned char *pImgData)
{
	//���û����Ա����ReplaceDibΪ��������ͼ���DIB�ṹ
	ReplaceDib(size, nBitCount,  lpColorTable, pImgData);

	//��������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_pFFTBuf!=NULL){
		delete m_pFFTBuf;
		m_pFFTBuf=NULL;
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
CSize FourierTrans::GetDimensions()
{
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

/***********************************************************************
* �������ƣ�Fourier()
*���������� ��
*����ֵ�� ��
*˵����������ͼ����п��ٸ���Ҷ�任��Ҫ������ͼ��Ŀ�͸߱�����2���ݴη�
***********************************************************************/
void FourierTrans::Fourier()
{
	//���ͼ��Ĵ�С������ͼ����ͬ
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//�������ͼ�񻺳���������ʼ��Ϊ0
	m_pImgDataOut=new unsigned char[m_imgWidthOut*m_imgHeightOut];
	memset(m_pImgDataOut, 0, m_imgWidthOut*m_imgHeightOut);
	
	//���븵��Ҷ������������ʼ��Ϊ0
	m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
	memset(m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);

	//����ͼ�����ݽ��ж�ά����Ҷ�任
	ImageFFt2D(m_pImgData, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);
}

/***********************************************************************
* �������ƣ�RevFourier()
* ������������
* ����ֵ��  ��
* ˵��������ͼ��Ŀ��ٸ���Ҷ���任����ImageFFT()�ɶԵ���
******************************************************************/
void FourierTrans::RevFourier()
{
	//ͼ��Ķ�ά����Ҷ���任,�任������ǻָ���ԭͼ�񣬴���m_pImgDataOut
	ImageRevFFt2D(m_pImgDataOut, m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* �������ƣ�ImageFFt1D()
* ����������ComplexNumber *arrayBuf  -�����ṹ����������
*           int n  
*           ����arrayBuf�ĳ��ȣ�����Ϊ2���ݴη�
*����ֵ��   ��
*˵�����Ը����ṹ������arrayBuf����һά���ٸ���Ҷ�任���任��Ľ����
*      ���arrayBuf��
***********************************************************************/
void FourierTrans::ImageFFt1D(ComplexNumber *arrayBuf, int n)
{
	//ѭ������
	int i, k, r;
	
	//������ʱ����������buf1������Ϊn
	ComplexNumber *buf1=new ComplexNumber[n];
	
	//��arrayBuf������buf1
	memcpy(buf1,arrayBuf,sizeof(ComplexNumber)*n);
	
	//������ʱ����������buf2������Ϊn
	ComplexNumber *buf2=new ComplexNumber[n];
	
	//��arrayBuf����Ԫ�ػ�2��ȡ����������
	//��0��1��2��3��4��5��6��7�˵����жԵ������0��4��2��6��1��5��3��7
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
	
	
	//���õ����㷨���п��ٸ���Ҷ�任
	//buf1�ǵ�r�������룬buf2��ŵ�r�������
	float c,s;
	for(r=1;pow(2,r)<=n;r++){
		t1=pow(2,r);
		for(k=0;k<n/t1;k++){
			for(i=t1/2;i<t1;i++){
				//��Ȩ����
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
		
		//��r�����������buf1,��Ϊ��һ������������
		memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
	}
	//����Ҷ�任�Ľ������arrayBuf
	memcpy(arrayBuf,buf2,sizeof(ComplexNumber)*n);
	
	//�ͷŻ�����
	delete[]buf2;
	delete[]buf1;
	
}

/***********************************************************************
* �������ƣ�ImageFFt2D()
* ����������unsigned char* imgBuf  -ͼ������ָ��
*           int width  -ͼ���
*           int height  -ͼ���
*           unsigned char *imgBufOut  -����Ҷ�任������ͼ��
* ����ֵ��  ��
* ˵����ͼ�����ݶ�ά���ٸ���Ҷ�任��ͼ�����ݱ�ɸ�����ʽ,��������һά����
*      ����Ҷ�任,�任���Ƶ����ͼ����ʽ����imgBufOut���˴�Ҫ��ͼ���͸�
*      ��Ϊ2���ݴη�
***********************************************************************/
void FourierTrans::ImageFFt2D(unsigned char* imgBuf, int width, int height,
							 unsigned char *imgBufOut)
{
	//ѭ������
	int i, j, u, v;
	
	//ͼ�����ݱ�ɸ������ʹ���m_pFFTBuf
	for(i=0;i<width*height;i++){
		m_pFFTBuf[i].real=imgBuf[i];
		m_pFFTBuf[i].imag=0;
	}
	
	//����ComplexNumber�ṹ������,����Ϊheight
	ComplexNumber *array=new ComplexNumber[height];
	
	//������һά���ٸ���Ҷ�任
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
	
	//�ٺ���һά���ٸ���Ҷ�任
	for(v=0;v<height;v++){
		ImageFFt1D(m_pFFTBuf+v*width, width);
	}
	
	//��Ƶ��ͼ��ͼ����ʽ����imgBufOut
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
* �������ƣ�ImageRevFFt2D()
*���������� unsigned char* imgBufOut  -���ͼ������ָ��
*           int width  -ͼ���
*           int height  -ͼ���
*����ֵ��   ��
*˵�����Ը���Ҷ�任������m_pFFTBuf�������,��������һά���ٸ���Ҷ���任,
*      �任���Ƶ�׼���ԭͼ��Ļָ�����,���ָ����ݴ���imgBufOut���˴�Ҫ��
*      ͼ���͸߾�Ϊ2���ݴη�    
***********************************************************************/
void FourierTrans::ImageRevFFt2D(unsigned char* imgBufOut, int width, int height)
{
	//������Ҷ�任������Ϊ��,��ʧ�ܷ���
	if(!m_pFFTBuf)
		return;

	//����
	int i, u, v;
	
	//��������Ҷ���任
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

	//�ٺ�����Ҷ���任
	for(v=0;v<height;v++){
		ImageRevFFt1D(m_pFFTBuf+v*width, width);
	}

	//��m_pFFTBuf�ķ�����,�ָ�ͼ�����imgBufOut
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
* �������ƣ�ImageRevFFt1D()
* ����������ComplexNumber *arrayBuf  -�����ṹ����������
*           int n  -����arrayBuf�ĳ��ȣ�����Ϊ2���ݴη�
* ����ֵ��  ��
*˵�����Ը����ṹ������arrayBuf����һά���ٸ���Ҷ���任���任��Ľ����
*      ���arrayBuf��
***********************************************************************/
void FourierTrans::ImageRevFFt1D(ComplexNumber *arrayBuf, int n)
{
	//����
	int i;

	//������arrayBuf����
	for(i=0;i<n;i++)
		arrayBuf[i].imag=-arrayBuf[i].imag;

	//һά���ٸ���Ҷ�任
	ImageFFt1D(arrayBuf, n);

	//ʱ���������������N
	for(i=0;i<n;i++){
		arrayBuf[i].real=arrayBuf[i].real/n;
		arrayBuf[i].imag=-arrayBuf[i].imag/n;
	}

}