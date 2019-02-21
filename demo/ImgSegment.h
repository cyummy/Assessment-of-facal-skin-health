// ImgSegment.h: interface for the ImgSegment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGSEGMENT_H__8745C184_7B7A_4116_9351_36AEA5427282__INCLUDED_)
#define AFX_IMGSEGMENT_H__8745C184_7B7A_4116_9351_36AEA5427282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class ImgSegment : public ImageDib  
{
public:
	//输出图像每像素位数
	int m_nBitCountOut;
	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;	
	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;	
    //输出图像的宽
	int m_imgWidthOut;	
	//输出图像的高
	int m_imgHeightOut;
	//输出图像颜色表长度
	int m_nColorTableLengthOut;	
public:
	//不带参数的构造函数
	ImgSegment();	
	//带参数的构造函数
	ImgSegment(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	//析构函数
	virtual ~ImgSegment();
public:
	//以像素为单位返回输出图像的尺寸
	CSize GetDimensions();
    //自适应阈值分割
	void AdaptThreshSeg(unsigned char *pImgData);
	//Roberts算子
	void Roberts();	
    //Sobel算子
	void Sobel();	
	//Prewitt算子
	void Prewitt();	
	//Laplacian算子
	void Laplacian();	
public:
	//区域生长
	void RegionGrow(CPoint SeedPos, int thresh);
	//曲线跟踪
	void EdgeTrace();
};

#endif // !defined(AFX_IMGSEGMENT_H__8745C184_7B7A_4116_9351_36AEA5427282__INCLUDED_)
