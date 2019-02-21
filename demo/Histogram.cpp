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
* �������ƣ�
* Histogram()
*
*˵�����޲����Ĺ��캯������ʼ����Ա����
***********************************************************************/
Histogram::Histogram()
{
	//ֱ��ͼ������0
	for(int i=0;i<256;i++)
    	m_histArray[i]=0;

	m_average=0;//��ֵΪ0

	m_deviation=0;//����Ϊ0

    m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0

}

/***********************************************************************
* �������ƣ�
* Histogram()
*
*����������
*  CSize size -ͼ���С�����ߣ�
*  int nBitCount  -ÿ������ռλ��
*  LPRGBQUAD lpColorTable  -��ɫ��ָ��
*  unsigned char *pImgData  -λͼ����ָ��
*
*����ֵ��
*   ��
*
*˵����������Ϊ�������Ĺ��캯��������λͼ�Ĵ�С��ÿ����λ������ɫ��
*      ��λͼ���ݣ�����ImgDib()�Ի����Ա��ʼ��������ʼ���������
*      ���ݳ�Ա
***********************************************************************/
Histogram::Histogram(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImageDib(size, nBitCount, lpColorTable, pImgData)
{
	//ֱ��ͼ������0
	for(int i=0;i<256;i++)
    	m_histArray[i]=0;

	m_average=0;//��ֵΪ0

	m_deviation=0;//����Ϊ0

    m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}

/***********************************************************************
* �������ƣ�
*   ~Histogram()

*
*˵���������������ͷ���Դ
***********************************************************************/

Histogram::~Histogram()
{
	//�ͷ����ͼ��λͼ���ݻ�����
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//�ͷ����ͼ����ɫ��
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}

/***********************************************************************
* �������ƣ�
* GetDimensions()
*
*����������
*  ��
*
*����ֵ��
*   ͼ��ĳߴ磬��CSize���ͱ��
*
*˵�����������ͼ��Ŀ�͸�
***********************************************************************/
CSize Histogram::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* �������ƣ�
* computeHistGray()
*
*˵�����Ҷ�ͼ��ͳ��ֱ��ͼ��m_histArray�д���˵�ǰͼ���ͳ������
***********************************************************************/
void Histogram::computeHistGray()
{
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//ѭ������
	int i,j;

	//ֱ��ͼ������0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;

	//ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�м����
	int temp;

	//ͳ�ƻҶ�ֱ��ͼ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			temp=*(m_pImgData+i*lineByte+j);
		    m_histArray[temp]++;
		}
	}
}


/***********************************************************************
* �������ƣ�
* computeHistBrightness()
*
*˵������ɫͼ������ֱ��ͼ��m_histArray�д���˵�ǰͼ�������ͳ������
***********************************************************************/
 
void Histogram::computeHistBrightness()
{
	//��ɫͼ����Ч
	if(m_nBitCount!=24)
		return;

	//ѭ������
	int i,j;

	//ֱ��ͼ������0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;

	//ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�м����
	int temp;

	//ͳ������ֱ��ͼ
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
* �������ƣ�
* computeAverage()
*
*˵��������ֱ��ͼ��ֵ��ֱ��ͼ��ͳ������
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
* �������ƣ�
* computeDeviation()
*
*˵��������ֱ��ͼ���ֱ��ͼ��ͳ������
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
* �������ƣ�
* histAve()
*
*˵����ֱ��ͼ���⣬�ú���ֻ�ԻҶ�ͼ����Ч
***********************************************************************/
void Histogram::histAve()
{
	// ֻ����Ҷ�
	if(m_nBitCount!=8)
		return;

	//�ͷžɵ����ͼ�񻺳���
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��ÿ����λ��������ͼ����ͬ
	m_nBitCountOut=m_nBitCount;

	//������ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//������ɫ������ͼ�����ɫ�������ͼ����ɫ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//���ͼ��ÿ��������ռ���ֽ���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//ѭ������
	int i,j;

	//ӳ���
	double map[256];

	//�м����
	int sum,tmp;

	//ͳ�ƻҶ�ֱ��ͼ
	computeHistGray();

	//����ӳ���
	sum=0;
	for(i=0;i<256;i++){
		sum+=m_histArray[i];
		map[i]=(double)sum*255/(m_imgWidth*m_imgHeight)+0.5;
	}
	
	//������ݸ�ֵ
	for(i=0;i<m_imgHeight;i++)
	{
		for(j=0;j<m_imgWidth;j++)
		{		
			tmp=*(m_pImgData+i*lineByteOut+j);
			*(m_pImgDataOut+i*lineByteOut+j)=(int)map[tmp];
		}
	}

}