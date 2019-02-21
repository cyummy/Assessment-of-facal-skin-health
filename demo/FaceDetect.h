// FaceDetect.h: interface for the FaceDetect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACEDETECT_H__2A711F63_7B57_4AA4_BFA9_FDB85C816B35__INCLUDED_)
#define AFX_FACEDETECT_H__2A711F63_7B57_4AA4_BFA9_FDB85C816B35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"
#include "FourierTrans.h"
#include "math.h"
#include "WaveletTrans.h"
#include "Morphology.h"


class FaceDetect : public ImageDib  
{
	public:
	//输出图像每像素位数
	int m_nBitCountOut;
	int m_nBitCountOut1;
 	int m_nBitCountOut2;



	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;
    unsigned char * m_pImgDataOut1;
    unsigned char * m_pImgDataOut2;

	
	unsigned char * m_pImgDataIn1;
	unsigned char * m_pImgDataIn2;

	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;
	LPRGBQUAD m_lpColorTableOut1;
	
	double** m_pSimArray;//相似度矩阵

	double Cb_Mean;
	double Cr_Mean;
	double Cov00;
	double Cov01;
	double Cov10;
	double Cov11;
	
	//double MAXNUM=1e+8			//最大整数

private:
	//输出图像的宽，像素为单位
	int m_imgWidthOut;

	//输出图像的高，像素为单位
	int m_imgHeightOut;

	//输出图像颜色表长度
	int m_nColorTableLengthOut;
    int m_nColorTableLengthOut1;

public:
	FourierTrans FFtTrans;
    WaveletTrans DWtTrans;
    Morphology filter;

public:
	void DWThomomorphicfilter();
	void Adaptivethresegment();
	void Homomorphicfilter();
    void ImageCut(int F_top,int F_bottom,int F_left,int F_right);
	void CalSBound(int top,int bottom,int left,int right);
	//不带参数的构造函数
	FaceDetect();

	//带参数的构造函数
	FaceDetect(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData1,unsigned char *pImgData2);

	//析构函数
	~FaceDetect();
	
	void CalMedFl(double **s,int w,int h,int n);
	void CalSim();
	/*double Cbmean;
	double Crmean;
	double Cov[2][2];

	int m_nDIBWidth;//图像宽度
	int m_nDIBHeight;//图像高度
	RGBQUAD** m_pSource;//原始数据
	double** m_pSimArray;//相似度矩阵
	BYTE** m_pBinArray;//二值矩阵
	bool m_bSimReady;//是否计算好相似度矩阵
	bool m_bBinReady;//是否计算二值化矩阵

	CSim();
	CSim(RGBQUAD **source,int w,int h);*/
	
	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();

};

#endif // !defined(AFX_FACEDETECT_H__2A711F63_7B57_4AA4_BFA9_FDB85C816B35__INCLUDED_)
