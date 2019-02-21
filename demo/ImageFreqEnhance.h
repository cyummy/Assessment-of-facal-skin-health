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
	int m_nBitCountOut;//���ͼ��ÿ����λ��
	unsigned char * m_pImgDataOut;//���ͼ��λͼ����ָ��
	LPRGBQUAD m_lpColorTableOut;//���ͼ����ɫ��
	int m_imgWidthOut; //���ͼ��Ŀ�
	int m_imgHeightOut;//���ͼ��ĸ�
	int m_nColorTableLengthOut;//���ͼ����ɫ����
public:
	FourierTrans FFtTrans;
public:
	ImageFreqEnhance();
	ImageFreqEnhance(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	virtual ~ImageFreqEnhance();

	CSize GetDimensions();//������Ϊ��λ�������ͼ��Ŀ�͸�
	void InputImageData(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);	//����ԭͼ������

	void IdealLowPassFilter(int nWidth, int nHeight, int nRadius);  //�����ͨ�˲�
	void ButterLowPassFilter(int nWidth, int nHeight, int nRadius);	//������˹��ͨ�˲�
	void IdealHighPassFilter(int nWidth, int nHeight, int nRadius);	//�����ͨ�˲�
	void ButterHighPassFilter(int nWidth, int nHeight, int nRadius); //������˹��ͨ�˲�

};

#endif // !defined(AFX_IMAGEFREQENHANCE_H__94FAB4B4_2B67_456D_8209_18D20A91F7C3__INCLUDED_)
