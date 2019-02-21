// Histogram.h: interface for the Histogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTOGRAM_H__CA29573E_CFD7_43A7_8605_C55CE729399B__INCLUDED_)
#define AFX_HISTOGRAM_H__CA29573E_CFD7_43A7_8605_C55CE729399B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class Histogram : public ImageDib  
{
public:
	//直方图数组
	int m_histArray[256];

	//直方图平均值
	float m_average;
	
	//直方图方差
	float m_deviation;
	
	//输出图像每像素位数
	int m_nBitCountOut;
	
	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;
	
	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;
	
private:
	//输出图像的宽，像素为单位
	int m_imgWidthOut;
	
	//输出图像的高，像素为单位
	int m_imgHeightOut;
	
	//输出图像颜色表长度
	int m_nColorTableLengthOut;
	
public:
	//不带参数的构造函数
	Histogram();
	
	//带参数的构造函数
	Histogram(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
	
	//析构函数
	~Histogram();
	
	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();
	
	//统计直方图
	void computeHistGray();
	
	//彩色图像亮度直方图
	void computeHistBrightness();
	
	//计算直方图均值
	float computeAverage();
	
	//计算直方图方差
   float computeDeviation();
	
	//直方图均衡
	void histAve();
};

#endif // !defined(AFX_HISTOGRAM_H__CA29573E_CFD7_43A7_8605_C55CE729399B__INCLUDED_)
