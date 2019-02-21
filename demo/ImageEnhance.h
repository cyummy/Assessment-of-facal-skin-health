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
	//���캯��
	ImageEnhance();
	ImageEnhance(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
	virtual ~ImageEnhance();
	
	//��ӽ�������
	void AddPepperSaltNoise();
	
	//��Ӹ�˹����
	void AddGaussNoise();
    
	//���þ�ֵģ�����ͼ��ƽ��
	void NeiAveTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float fCoef);
	
	//��ֵ�˲�
	BYTE FindMedianValue(unsigned char* lpbArray,int iArrayLen);
	void MedianSmooth(int iFilterH, int iFilterW, int iFilterCX, int iFilterCY);
	
	//������˹�� ת��Ϊģ������
	void LapTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float fCoef);
	
	//�ݶ���
	void GradeSharp(int Thresh);
	
	//ѡ����ģƽ��
	void ChooseMaskSmooth();


};

#endif // !defined(AFX_IMAGEENHANCE_H__42487467_7166_46F6_B236_826A4969E420__INCLUDED_)
