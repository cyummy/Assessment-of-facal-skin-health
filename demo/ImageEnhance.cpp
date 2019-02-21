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
* 函数名称：
* AddPepperSaltNoise()
* 参数： void
* 返回值：void
* 说明： 在view中图像添加椒盐噪声
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
		bytecount = m_imgWidth * m_imgHeight *m_nBitCount/8; //获取位图数据区的字节数
	}
	else
	{
		AfxMessageBox("只能处理真彩色和8位灰度图像!");
		return ;
	}
	if (m_nBitCount==8)      //处理灰度图像
	{  
        for (i=0;i<bytecount;i++)
        {
			if(rand()>32000)  pDIB[i]=0;
			if(rand()<200)    pDIB[i]=255;
        }
	}
	else	                 //24位真彩色图像
	{
        for (i=0;i<bytecount;i=i+3)
        {
			int num=rand();
            if (num>32000) 
			{
				pDIB[i]=(rand())%255;        //处理每一个像素的RGB值
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
	//生成新的DIB位图
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
* 函数名称： AddGaussNoise()
* 参数： void
* 返回值：void
* 说明： 在view中图像添加随机噪声
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
		bytecount = m_imgWidth * m_imgHeight *m_nBitCount/8; //获取位图数据区的字节数
	}
	else
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
    //DIB中添加随机噪声	
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
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
    int n,ji[9],nmin;
	float mean[9],bunsan[9],bmin;
	
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
   	//创建要复制的图像区域
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
			//求9种近邻区域的均值及其方差
			//第1近邻区域
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
			//第2近邻区域
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
			//第3近邻区域
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
			//第4近邻区域
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
			//第5近邻区域
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
			//第6近邻区域
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
			//第7近邻区域
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
			//第8近邻区域
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
			//第9近邻区域
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
			//求方差最小的近邻区域nmin
			bmin=bunsan[0];
			nmin=0;
			for(n=0;n<=8;n++)
			{
				if(bmin>bunsan[n])
				{
					bmin=bunsan[n];
					nmin=n;
				}
				//把nmin的值四舍五入后作为显示图像的值
				m_pImgDataOut[j*lineByteOut+i]=(int)(mean[nmin]+0.5);
			}				 
		}
}





void ImageEnhance::GradeSharp(int Thresh)
{
   	unsigned char*	pSrc;       // 指向源图像的指针
	unsigned char*	pDst; 	
	unsigned char*	pSrc1;
	unsigned char*	pSrc2;	
	LONG	i,j;				// 循环变量
	int	bTemp;
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
	//创建要复制的图像区域
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
	
	for(i = 0; i < m_imgHeight; i++)		// 每行
	{		
		for(j = 0; j < m_imgWidth; j++)		// 每列
		{
			//指向新DIB第i行第j列的像素的指针
			pDst = m_pImgDataOut + lineByte * (m_imgHeight -1 - i) + j;
			
			// 进行梯度运算
			// 指向DIB第i行，第j个象素的指针
			pSrc  = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 1 - i) + j;			
			// 指向DIB第i+1行，第j个象素的指针
			pSrc1 = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 2 - i) + j;			
			// 指向DIB第i行，第j+1个象素的指针
			pSrc2 = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 1 - i) 
				+ j + 1;
			
			bTemp = abs((*pSrc)-(*pSrc1)) + abs((*pSrc)-(*pSrc2));
			
			// 判断是否小于阈值
			if ((bTemp+120) < 255)
			{
				// 判断是否大于阈值，对于小于情况，灰度值不变。
				if (bTemp >= Thresh)
				{
					*pSrc = bTemp + 120;
				}
			}
			else
			{
				*pSrc = 255;
			}
			//生成新的DIB像素值
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
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
    //创建要复制的图像区域
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
	
	//行处理(去掉边缘几行)
	for (i = TempCY; i < m_imgHeight - TempH + TempCY + 1; i++)
	{
		//列处理(去掉边缘几列)
		for (j = TempCX; j < m_imgWidth - TempW + TempCX + 1; j++)
		{
			//指向新DIB第i行第j列的像素的指针
			pDst = m_pImgDataOut + lineByte * (m_imgHeight - 1 - i) + j;
			
			value=0;
			//计算
			for (k = 0; k < TempH; k++)
			{
				for (l = 0; l < TempW; l++)
				{
					pSrc = m_pImgData + lineByte * (m_imgHeight - 1 - i 
						+ TempCY - k) + j - TempCX + l;
					//计算加权平均,保存像素值
					value += (*pSrc) * fpTempArray[k * TempW + l];
				}
			}
			//乘以系数
			value*=fCoef;
			//取结果的绝对值
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
	unsigned char*	value;         //指向滤波器数组的指针
	
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//计算图像每行的字节数
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
	//分配内存，以保存新图像
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		//为处理后的图像分配内存空间
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}
	int pixelByte = m_nBitCountOut / 8;
	for(i =  0; i < m_imgHeight; i++){
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut + i * lineByteOut +j) = *(m_pImgData + i * lineByteOut + j);
	}
	//暂时分配内存，以保存滤波器数组
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
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
    //创建要复制的图像区域
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
	
	//行处理
	for (i = TempCY; i < m_imgHeight - TempH + TempCY + 1; i++)
	{
		//列处理
		for (j = TempCX; j <m_imgWidth - TempW + TempCX + 1; j++)
		{
			//指向新DIB第i行第j列的像素的指针
			pDst = m_pImgDataOut + lineByte * (m_imgHeight -1 - i) + j;
			
			value=0;
			//计算
			for (k = 0; k < TempH; k++)
			{
				for (l = 0; l < TempW; l++)
				{
					pSrc = m_pImgData + lineByte * (m_imgHeight - 1 - i + TempCY - k)
						+ j - TempCX + l;
					//计算加权平均
					value += (*pSrc) * fpTempArray[k * TempW + l];
				}
			}
			//乘以系数
			value *= f;
			//取结果的绝对值
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




