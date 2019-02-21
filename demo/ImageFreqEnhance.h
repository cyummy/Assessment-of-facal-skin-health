// ImageFreqEnhance.h: interface for the ImageFreqEnhance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEFREQENHANCE_H__94FAB4B4_2B67_456D_8209_18D20A91F7C3__INCLUDED_)
#define AFX_IMAGEFREQENHANCE_H__94FAB4B4_2B67_456D_8209_18D20A91F7C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"
#include "FourierTrans.h"

class ImageFreqEnhance : public ImageDib  
{
public:
	int m_nBitCountOut;//输出图像每像素位数
	unsigned char * m_pImgDataOut;//输出图像位图数据指针
	LPRGBQUAD m_lpColorTableOut;//输出图像颜色表
	int m_imgWidthOut; //输出图像的宽
	int m_imgHeightOut;//输出图像的高
	int m_nColorTableLengthOut;//输出图像颜色表长度
public:
	FourierTrans FFtTrans;
public:
	ImageFreqEnhance();
	ImageFreqEnhance(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	virtual ~ImageFreqEnhance();

	CSize GetDimensions();//以像素为单位返回输出图像的宽和高
	void InputImageData(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);	//输入原图像数据

	void IdealLowPassFilter(int nWidth, int nHeight, int nRadius);  //理想低通滤波
	void ButterLowPassFilter(int nWidth, int nHeight, int nRadius);	//巴特沃斯低通滤波
	void IdealHighPassFilter(int nWidth, int nHeight, int nRadius);	//理想高通滤波
	void ButterHighPassFilter(int nWidth, int nHeight, int nRadius); //巴特沃斯高通滤波

};

#endif // !defined(AFX_IMAGEFREQENHANCE_H__94FAB4B4_2B67_456D_8209_18D20A91F7C3__INCLUDED_)
