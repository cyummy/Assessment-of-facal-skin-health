// Morphology.h: interface for the Morphology class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MORPHOLOGY_H__9556E8FD_5B31_4368_9C45_62BC56507403__INCLUDED_)
#define AFX_MORPHOLOGY_H__9556E8FD_5B31_4368_9C45_62BC56507403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

//结构元素对,该结构专门为击中击不中变换而定义
struct ElementPair
{
	int hitElement[9];
	int missElement[9];
};

class Morphology : public ImageDib  
{

public:
	int m_nBitCountOut;	//输出图像每像素位数
	unsigned char * m_pImgDataOut;//输出图像位图数据指针
	LPRGBQUAD m_lpColorTableOut;	//输出图像颜色表
	int m_imgWidthOut;	//输出图像的宽
	int m_imgHeightOut;	//输出图像的高
	int m_InitTemp[25];
	int *m_TempBuf;	//结构元素（模板）指针
	int TempW;     //结构元素宽
	int TempH;     //结构元素高
	ElementPair m_hitMissTemp[8];	//定义8个方向的击中击不中变换结构元素对
public:
	Morphology();	//不带参数的构造函数
    Morphology(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);	//带参数的构造函数
	virtual ~Morphology();	//析构函数
	CSize GetDimensions();	//返回输出图像的尺寸
public:
	void ImgErosion(unsigned char *imgBufIn,unsigned char *imgBufOut,	int imgWidth,
		int imgHeight,int *TempBuf, int TempW, int TempH);//腐蚀
	
	void ImgDilation(unsigned char *imgBufIn,unsigned char *imgBufOut,int imgWidth,
		int imgHeight,int *maskBuf, int maskW, int maskH);//膨胀
	
	void Open();	//二值开
	void Close();	//二值闭
	
	void ImgThinning();	//击中击不中细化
	
	void DefineElementPair();//定义击中击不中变换的结构元素对

	void HitAndMiss(unsigned char *imgBufIn, unsigned char *imgBufOut,
		int imgWidth,int imgHeight,ElementPair hitMissMask);	//击中击不中变换

};

#endif // !defined(AFX_MORPHOLOGY_H__9556E8FD_5B31_4368_9C45_62BC56507403__INCLUDED_)
