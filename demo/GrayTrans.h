// GrayTrans.h: interface for the GrayTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAYTRANS_H__8F98F0EE_4F7D_4138_8573_4E4A065784C2__INCLUDED_)
#define AFX_GRAYTRANS_H__8F98F0EE_4F7D_4138_8573_4E4A065784C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class GrayTrans : public ImageDib  
{
public:
	//输出图像每像素位数
	int m_nBitCountOut;
	
	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;
	
	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;

	//输出图像的宽，像素为单位
	int m_imgWidthOut;
	
	//输出图像的高，像素为单位
	int m_imgHeightOut;
	
	//输出图像颜色表长度
	int m_nColorTableLengthOut;
	
public:
	void GaryImage();
	//不带参数的构造函数
	GrayTrans();
	
	//带参数的构造函数
	GrayTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	
	//析构函数
	~GrayTrans();
	
	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();
	
	//二值化
	void BinaryImage(int threshold=128);

	//反转
	void RevImage();

    //窗口变换
	void ThresholdWindow(int bTop, int bBottom);
	
	//线性拉伸
	void LinearStrech(CPoint point1,CPoint point2);
	
private:
	//单通道数据线性拉伸
	void LinearStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut,int imgWidth,int imgHeight,
		CPoint point1,CPoint point2);

};

#endif // !defined(AFX_GRAYTRANS_H__8F98F0EE_4F7D_4138_8573_4E4A065784C2__INCLUDED_)
