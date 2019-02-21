// ImageEnhance.cpp: implementation of the ImageEnhance class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "ImageEnhance.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageEnhance::ImageEnhance()
{
	
	m_nColorTableLengthOut=0;
    m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_lpColorTableOut=NULL;
	
}

ImageEnhance::ImageEnhance(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImageDib(size, nBitCount, lpColorTable, pImgData)
{
	
    m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_lpColorTableOut=NULL;
	m_nColorTableLengthOut=0;
}

ImageEnhance::~ImageEnhance()
{
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut==NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}

/**********************************************************************
* �������ƣ�
* AddPepperSaltNoise()
* ������ void
* ����ֵ��void
* ˵���� ��view��ͼ����ӽ�������
**********************************************************************/
void ImageEnhance::AddPepperSaltNoise()
{
	unsigned char* pDIB;
	int bytecount,i;
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	pDIB=m_pImgData;
	if(m_nBitCount==8||m_nBitCount==24)
	{
		bytecount = m_imgWidth * m_imgHeight *m_nBitCount/8; //��ȡλͼ���������ֽ���
	}
	else
	{
		AfxMessageBox("ֻ�ܴ������ɫ��8λ�Ҷ�ͼ��!");
		return ;
	}
	if (m_nBitCount==8)      //����Ҷ�ͼ��
	{  
        for (i=0;i<bytecount;i++)
        {
			if(rand()>32000)  pDIB[i]=0;
			if(rand()<200)    pDIB[i]=255;
        }
	}
	else	                 //24λ���ɫͼ��
	{
        for (i=0;i<bytecount;i=i+3)
        {
			int num=rand();
            if (num>32000) 
			{
				pDIB[i]=(rand())%255;        //����ÿһ�����ص�RGBֵ
				pDIB[i+1]=(rand())%255;
				pDIB[i+2]=(rand())%255;
			}
			if (num<200) 
			{
				pDIB[i]=(rand())%255;
				pDIB[i+1]=(rand())%255;
				pDIB[i+2]=(rand())%255;
			}
        }
	}
	//�����µ�DIBλͼ
	m_nBitCountOut=m_nBitCount;
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	}
	int j, pixelByte=m_nBitCountOut/8;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth*pixelByte;j++)
			*(m_pImgDataOut+i*lineByteOut+j)= *(pDIB+i*lineByteOut+j);
	}
}
/**********************************************************************
* �������ƣ� AddGaussNoise()
* ������ void
* ����ֵ��void
* ˵���� ��view��ͼ������������
**********************************************************************/
void ImageEnhance::AddGaussNoise()
{
	unsigned char* pDIB;
	int bytecount,i;
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	pDIB=m_pImgData;
	if(m_nBitCount==8)
	{
		bytecount = m_imgWidth * m_imgHeight *m_nBitCount/8; //��ȡλͼ���������ֽ���
	}
	else
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		return ;
	}
    //DIB������������	
    for( i=0;i<bytecount;i++)
	{
		LONG Temp=rand();
		Temp = pDIB[i]*224/256+Temp/512;
		pDIB[i] = Temp >= 255 ? 255 : Temp;
    }
	
	m_nBitCountOut=m_nBitCount;
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	}
	
	int j, pixelByte=m_nBitCountOut/8;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth*pixelByte;j++)
			*(m_pImgDataOut+i*lineByteOut+j)= *(pDIB+i*lineByteOut+j);
	}
}

void ImageEnhance::ChooseMaskSmooth()
{
	if(m_nBitCount!=8)
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		return ;
	}
    int n,ji[9],nmin;
	float mean[9],bunsan[9],bmin;
	
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
   	//����Ҫ���Ƶ�ͼ������
	m_nBitCountOut=m_nBitCount;
    int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	}
	
	memset(m_pImgDataOut,255,lineByteOut * m_imgHeight);
	for(int j=2;j<=m_imgHeight-3;j++)
		for(int i=2;i<=m_imgWidth-3;i++)
		{
			//��9�ֽ�������ľ�ֵ���䷽��
			//��1��������
			ji[0]=m_pImgData[(j-1)*lineByteOut+(i-1)];
			ji[1]=m_pImgData[(j-1)*lineByteOut+i];
			ji[2]=m_pImgData[(j-1)*lineByteOut+(i+1)];
			ji[3]=m_pImgData[j*lineByteOut+(i-1)];
			ji[4]=m_pImgData[j*lineByteOut+i];
			ji[5]=m_pImgData[j*lineByteOut+(i+1)];
			ji[6]=m_pImgData[(j+1)*lineByteOut+(i-1)];
			ji[7]=m_pImgData[(j+1)*lineByteOut+i];
			ji[8]=m_pImgData[(j+1)*lineByteOut+(i+1)];
			mean[0]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6]+ji[7]+ji[8])/9;
			bunsan[0]=0;
			for(n=0;n<=8;n++)
				bunsan[0]+=ji[n]*ji[n]-mean[0]*mean[0];
			//��2��������
			ji[0]=m_pImgData[(j-2)*lineByteOut+(i-1)];
			ji[1]=m_pImgData[(j-2)*lineByteOut+i];
			ji[2]=m_pImgData[(j-2)*lineByteOut+(i+1)];
			ji[3]=m_pImgData[(j-1)*lineByteOut+(i-1)];
			ji[4]=m_pImgData[(j-1)*lineByteOut+i];
			ji[5]=m_pImgData[(j-1)*lineByteOut+(i+1)];
			ji[6]=m_pImgData[j*lineByteOut+i];
			mean[1]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[1]=0;
			for(n=0;n<=6;n++)
				bunsan[1]+=ji[n]*ji[n]-mean[1]*mean[1];
			//��3��������
			ji[0]=m_pImgData[(j-1)*lineByteOut+(i-2)];
			ji[1]=m_pImgData[(j-1)*lineByteOut+(i-1)];
			ji[2]=m_pImgData[j*lineByteOut+(i-2)];
			ji[3]=m_pImgData[j*lineByteOut+(i-1)];
			ji[4]=m_pImgData[j*lineByteOut+i];
			ji[5]=m_pImgData[(j+1)*lineByteOut+(i-2)];
			ji[6]=m_pImgData[(j+1)*lineByteOut+(i-1)];
			mean[2]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[2]=0;
			for(n=0;n<=6;n++)
				bunsan[2]+=ji[n]*ji[n]-mean[2]*mean[2];
			//��4��������
			ji[0]=m_pImgData[j*lineByteOut+i];
			ji[1]=m_pImgData[(j+1)*lineByteOut+(i-1)];
			ji[2]=m_pImgData[(j+1)*lineByteOut+i];
			ji[3]=m_pImgData[(j+1)*lineByteOut+(i+1)];
			ji[4]=m_pImgData[(j+2)*lineByteOut+(i-1)];
			ji[5]=m_pImgData[(j+2)*lineByteOut+i];
			ji[6]=m_pImgData[(j+2)*lineByteOut+(i+1)];
			mean[3]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[3]=0;
			for(n=0;n<=6;n++)
				bunsan[3]+=ji[n]*ji[n]-mean[3]*mean[3];
			//��5��������
			ji[0]=m_pImgData[(j-1)*lineByteOut+(i+1)];
			ji[1]=m_pImgData[(j-1)*lineByteOut+(i+2)];
			ji[2]=m_pImgData[j*lineByteOut+i];
			ji[3]=m_pImgData[j*lineByteOut+(i+1)];
			ji[4]=m_pImgData[j*lineByteOut+(i+2)];
			ji[5]=m_pImgData[(j+1)*lineByteOut+(i+1)];
			ji[6]=m_pImgData[(j+1)*lineByteOut+(i+2)]; 
			mean[4]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[4]=0;
			for(n=0;n<=6;n++)
				bunsan[4]+=ji[n]*ji[n]-mean[4]*mean[4];	
			//��6��������
			ji[0]=m_pImgData[(j-2)*lineByteOut+(i+1)];
			ji[1]=m_pImgData[(j-2)*lineByteOut+(i+2)];
			ji[2]=m_pImgData[(j-1)*lineByteOut+i];
			ji[3]=m_pImgData[(j-1)*lineByteOut+(i+1)];
			ji[4]=m_pImgData[(j-1)*lineByteOut+(i+2)];
			ji[5]=m_pImgData[j*lineByteOut+i];
			ji[6]=m_pImgData[j*lineByteOut+(i+1)]; 
			mean[5]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[5]=0;
			for(n=0;n<=6;n++)
				bunsan[5]+=ji[n]*ji[n]-mean[5]*mean[5];
			//��7��������
			ji[0]=m_pImgData[(j-2)*lineByteOut+(i-2)];
			ji[1]=m_pImgData[(j-2)*lineByteOut+(i-1)];
			ji[2]=m_pImgData[(j-1)*lineByteOut+(i-2)];
			ji[3]=m_pImgData[(j-1)*lineByteOut+(i-1)];
			ji[4]=m_pImgData[(j-1)*lineByteOut+i];
			ji[5]=m_pImgData[j*lineByteOut+(i-1)];
			ji[6]=m_pImgData[j*lineByteOut+i];
			mean[6]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[6]=0;
			for(n=0;n<=6;n++)
				bunsan[6]+=ji[n]*ji[n]-mean[6]*mean[6];
			//��8��������
			ji[0]=m_pImgData[j*lineByteOut+(i-1)];
			ji[1]=m_pImgData[j*lineByteOut+i];
			ji[2]=m_pImgData[(j+1)*lineByteOut+(i-2)];
			ji[3]=m_pImgData[(j+1)*lineByteOut+(i-1)];
			ji[4]=m_pImgData[(j+1)*lineByteOut+i];
			ji[5]=m_pImgData[(j+2)*lineByteOut+(i-2)];
			ji[6]=m_pImgData[(j+2)*lineByteOut+(i-1)];
			mean[7]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[7]=0;
			for(n=0;n<=6;n++)
				bunsan[7]+=ji[n]*ji[n]-mean[7]*mean[7];
			//��9��������
			ji[0]=m_pImgData[j*lineByteOut+i];
			ji[1]=m_pImgData[j*lineByteOut+(i+1)];
			ji[2]=m_pImgData[(j+1)*lineByteOut+i];
			ji[3]=m_pImgData[(j+1)*lineByteOut+(i+1)];
			ji[4]=m_pImgData[(j+1)*lineByteOut+(i+2)];
			ji[5]=m_pImgData[(j+2)*lineByteOut+(i+1)];
			ji[6]=m_pImgData[(j+2)*lineByteOut+(i+2)];
			mean[8]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[8]=0;
			for(n=0;n<=6;n++)
				bunsan[8]+=ji[n]*ji[n]-mean[8]*mean[8];
			//�󷽲���С�Ľ�������nmin
			bmin=bunsan[0];
			nmin=0;
			for(n=0;n<=8;n++)
			{
				if(bmin>bunsan[n])
				{
					bmin=bunsan[n];
					nmin=n;
				}
				//��nmin��ֵ�����������Ϊ��ʾͼ���ֵ
				m_pImgDataOut[j*lineByteOut+i]=(int)(mean[nmin]+0.5);
			}				 
		}
}





void ImageEnhance::GradeSharp(int Thresh)
{
   	unsigned char*	pSrc;       // ָ��Դͼ���ָ��
	unsigned char*	pDst; 	
	unsigned char*	pSrc1;
	unsigned char*	pSrc2;	
	LONG	i,j;				// ѭ������
	int	bTemp;
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		return ;
	}
	//����Ҫ���Ƶ�ͼ������
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}
	
	int pixelByte = m_nBitCountOut / 8;
	for(i = 0; i < m_imgHeight; i++){
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut + i * lineByteOut + j) = *(m_pImgData + i * lineByteOut + j);
	}
	
	for(i = 0; i < m_imgHeight; i++)		// ÿ��
	{		
		for(j = 0; j < m_imgWidth; j++)		// ÿ��
		{
			//ָ����DIB��i�е�j�е����ص�ָ��
			pDst = m_pImgDataOut + lineByte * (m_imgHeight -1 - i) + j;
			
			// �����ݶ�����
			// ָ��DIB��i�У���j�����ص�ָ��
			pSrc  = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 1 - i) + j;			
			// ָ��DIB��i+1�У���j�����ص�ָ��
			pSrc1 = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 2 - i) + j;			
			// ָ��DIB��i�У���j+1�����ص�ָ��
			pSrc2 = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 1 - i) 
				+ j + 1;
			
			bTemp = abs((*pSrc)-(*pSrc1)) + abs((*pSrc)-(*pSrc2));
			
			// �ж��Ƿ�С����ֵ
			if ((bTemp+120) < 255)
			{
				// �ж��Ƿ������ֵ������С��������Ҷ�ֵ���䡣
				if (bTemp >= Thresh)
				{
					*pSrc = bTemp + 120;
				}
			}
			else
			{
				*pSrc = 255;
			}
			//�����µ�DIB����ֵ
			*pDst = *pSrc;
		}
	}
	
}


void ImageEnhance::NeiAveTemplate(int TempH, int TempW, int TempCX, 
								  int TempCY, float *fpTempArray, float fCoef)
{
	unsigned char*	pSrc;
	unsigned char*	pDst;   
	int	i,j,k,l;
	float	value;
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		return ;
	}
    //����Ҫ���Ƶ�ͼ������
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth*m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut = new unsigned char[lineByteOut*m_imgHeight];
	}
	
	int pixelByte = m_nBitCountOut / 8;
	for(i = 0; i < m_imgHeight; i++){
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut+i*lineByteOut+j)= *(m_pImgData+i*lineByteOut+j);
	}
	
	//�д���(ȥ����Ե����)
	for (i = TempCY; i < m_imgHeight - TempH + TempCY + 1; i++)
	{
		//�д���(ȥ����Ե����)
		for (j = TempCX; j < m_imgWidth - TempW + TempCX + 1; j++)
		{
			//ָ����DIB��i�е�j�е����ص�ָ��
			pDst = m_pImgDataOut + lineByte * (m_imgHeight - 1 - i) + j;
			
			value=0;
			//����
			for (k = 0; k < TempH; k++)
			{
				for (l = 0; l < TempW; l++)
				{
					pSrc = m_pImgData + lineByte * (m_imgHeight - 1 - i 
						+ TempCY - k) + j - TempCX + l;
					//�����Ȩƽ��,��������ֵ
					value += (*pSrc) * fpTempArray[k * TempW + l];
				}
			}
			//����ϵ��
			value*=fCoef;
			//ȡ����ľ���ֵ
			value=(float)fabs(value);
			if (value > 255)
			{
				*pDst = 255;	
			}
			else
			{
				*pDst = (unsigned char)(value + 0.5);
			}
		}
	}
}


void ImageEnhance::MedianSmooth(int FilterH, int FilterW, int FilterCX, int FilterCY)
{
	unsigned char*	pSrc;
    unsigned char*	pDst;
    int		i,j,k,l;
	unsigned char*	value;         //ָ���˲��������ָ��
	
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//����ͼ��ÿ�е��ֽ���
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		return ;
	}
	//�����ڴ棬�Ա�����ͼ��
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		//Ϊ������ͼ������ڴ�ռ�
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}
	int pixelByte = m_nBitCountOut / 8;
	for(i =  0; i < m_imgHeight; i++){
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut + i * lineByteOut +j) = *(m_pImgData + i * lineByteOut + j);
	}
	//��ʱ�����ڴ棬�Ա����˲�������
	value = new unsigned char[FilterH * FilterW];
	for (i = FilterCY; i < m_imgHeight - FilterH ; i++)//+ FilterCY + 1
	{
		for (j = FilterCX; j < m_imgWidth - FilterW ; j++)//+ FilterCX + 1
		{
			
			pDst = m_pImgDataOut + lineByte * (m_imgHeight - 1 - i) + j;
			for (k = 0; k < FilterH; k++)
			{
				for (l = 0; l < FilterW; l++)
				{
					pSrc = m_pImgData + lineByte * (m_imgHeight - 1 - i 
						+ FilterCY - k) + j - FilterCX + l; //...............................
					value[k * FilterW + l] = *pSrc;
				}
			}
			*pDst = FindMedianValue(value,FilterW * FilterH);
		}
	}
}


unsigned char ImageEnhance::FindMedianValue(unsigned char* pbArray,int ArrayLen)
{
	int i,j;
	unsigned char Temp;
	
	for(j =0; j<ArrayLen; j++)
	{
		for(i =0; i <ArrayLen-j-1; i++)
		{
			if(pbArray[i]>pbArray[i+1])
			{
				Temp=pbArray[i];
				pbArray[i]=pbArray[i+1];
				pbArray[i+1]=Temp;
			}
			
		}
	}
	if ((ArrayLen&1)>0)
	{
		Temp=pbArray[(ArrayLen+1)/2];
	}
	else
	{
		Temp=(pbArray[ArrayLen/2]+pbArray[ArrayLen/2+1])/2;
	}
	
	return Temp;
}


void ImageEnhance::LapTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float f)
{
	unsigned char*	pSrc;
	unsigned char*	pDst;   
	int		i,j,k,l;
	float	value;
	
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		return ;
	}
    //����Ҫ���Ƶ�ͼ������
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}
	
	int pixelByte = m_nBitCountOut / 8;
	for(i = 0; i < m_imgHeight; i++){
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut + i * lineByteOut + j) = *(m_pImgData + i * lineByteOut + j);
	}
	
	//�д���
	for (i = TempCY; i < m_imgHeight - TempH + TempCY + 1; i++)
	{
		//�д���
		for (j = TempCX; j <m_imgWidth - TempW + TempCX + 1; j++)
		{
			//ָ����DIB��i�е�j�е����ص�ָ��
			pDst = m_pImgDataOut + lineByte * (m_imgHeight -1 - i) + j;
			
			value=0;
			//����
			for (k = 0; k < TempH; k++)
			{
				for (l = 0; l < TempW; l++)
				{
					pSrc = m_pImgData + lineByte * (m_imgHeight - 1 - i + TempCY - k)
						+ j - TempCX + l;
					//�����Ȩƽ��
					value += (*pSrc) * fpTempArray[k * TempW + l];
				}
			}
			//����ϵ��
			value *= f;
			//ȡ����ľ���ֵ
			value = (float)fabs(value);
			if (value > 255)
			{
				*pDst = 255;	
			}
			else
			{
				*pDst = (unsigned char)(value+0.5);
			}
		}
	}
}




