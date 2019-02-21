// ImageEnhance.h: interface for the ImageEnhance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEENHANCE_H__42487467_7166_46F6_B236_826A4969E420__INCLUDED_)
#define AFX_IMAGEENHANCE_H__42487467_7166_46F6_B236_826A4969E420__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class ImageEnhance : public ImageDib  
{

public:
    int m_nBitCountOut;
	unsigned char * m_pImgDataOut;
	LPRGBQUAD m_lpColorTableOut;
	int m_nColorTableLengthOut;

public:
	//构造函数
	ImageEnhance();
	ImageEnhance(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
	virtual ~ImageEnhance();
	
	//添加椒盐噪声
	void AddPepperSaltNoise();
	
	//添加高斯噪声
	void AddGaussNoise();
    
	//采用均值模板进行图像平滑
	void NeiAveTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float fCoef);
	
	//中值滤波
	BYTE FindMedianValue(unsigned char* lpbArray,int iArrayLen);
	void MedianSmooth(int iFilterH, int iFilterW, int iFilterCX, int iFilterCY);
	
	//拉普拉斯锐化 转化为模板运算
	void LapTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float fCoef);
	
	//梯度锐化
	void GradeSharp(int Thresh);
	
	//选择掩模平滑
	void ChooseMaskSmooth();


};

#endif // !defined(AFX_IMAGEENHANCE_H__42487467_7166_46F6_B236_826A4969E420__INCLUDED_)
