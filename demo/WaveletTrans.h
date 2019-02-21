// WaveletTrans.h: interface for the WaveletTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVELETTRANS_H__CF82C3DD_BEBF_42EF_A229_9B73FADFD68F__INCLUDED_)
#define AFX_WAVELETTRANS_H__CF82C3DD_BEBF_42EF_A229_9B73FADFD68F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class WaveletTrans : public ImageDib  
{
public:
	int m_nBitCountOut;
	unsigned char * m_pImgDataOut;
	LPRGBQUAD m_pColorTableOut;
	int m_nColorTableLengthOut;
	double* pCoeff;
	int m_imgWidthOut; //输出图像的宽
	int m_imgHeightOut;//输出图像的高
	int m_nScale;//分解尺度	

public:
	//构造函数
	WaveletTrans();
    //带参数的构造函数
	WaveletTrans(CSize size, int nBitCount, LPRGBQUAD pColorTable, unsigned char *pImgData);
	//析构函数
	virtual ~WaveletTrans();

public:
	void InputImageData(CSize size, int nBitCount,LPRGBQUAD lpColorTable, unsigned char *pImgData);
	CSize GetDimensions();//以像素为单位返回输出图像的宽和高
protected:
	//矩阵区域复制
	void MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,
		unsigned char *matrixDest, int heightDest, int widthDest);
	//矩阵相减
	void MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);	
	//行采样
	void MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);
	//列采样
	void MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);
public:	
	//一次小波分解
	void WavlDecmposeTrans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	//小波变换
	void BasicWaveletTrans(int scale);
	
protected:
	//矩阵相加
	void MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);
	//行插点
	void MatrixInsertRow(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);
	//列插点
	void MatrixInsertLine(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);
public:
	//一次小波重构
	void WavlRebuildTrans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	//小波重构
	void BasicWaveletReverse();

};


#endif // !defined(AFX_WAVELETTRANS_H__CF82C3DD_BEBF_42EF_A229_9B73FADFD68F__INCLUDED_)
