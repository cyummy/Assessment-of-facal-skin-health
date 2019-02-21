// FourierTrans.h: interface for the FourierTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOURIERTRANS_H__7C7EA6C8_0851_4F24_89CE_482E569B25C9__INCLUDED_)
#define AFX_FOURIERTRANS_H__7C7EA6C8_0851_4F24_89CE_482E569B25C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

//�궨�壬PI
#define PI 3.1415926

//�������ͽṹ��
struct ComplexNumber
{
	float imag;// imaginary�鲿
	float real;//ʵ��
};


class FourierTrans : public ImageDib  
{
public:
	//���ͼ��ÿ����λ��
	int m_nBitCountOut;
	
	//���ͼ��λͼ����ָ��
	unsigned char * m_pImgDataOut;
	
	//���ͼ����ɫ��
	LPRGBQUAD m_lpColorTableOut;
	
	//���ͼ��Ŀ�
	int m_imgWidthOut;
	
	//���ͼ��ĸ�
	int m_imgHeightOut;
	
	//���ͼ����ɫ����
	int m_nColorTableLengthOut;
	
	//����ͼ��Ϊ��2���ݴεķ���ʱ����չͼ������Ҫ�Ļ�����ָ��
	unsigned char *m_pImgExpandBuf;
	
	//����Ҷ�任������
	ComplexNumber *m_pFFTBuf;
	
	//ͼ���Ƿ���չ�ı�־����
	bool m_isExpanded;
	
public:
	//�����������캯��
	FourierTrans();

	//�������Ĺ��캯��
	FourierTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	
	//��������
	virtual ~FourierTrans();
	
	//������Ϊ��λ�������ͼ��Ŀ�͸�
	CSize GetDimensions();
	
	//����ԭͼ������
	void InputImgData(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	
	//ͼ����Ҷ�任
	void Fourier();
	
	//����Ҷ���任
	void RevFourier();
	
public:
	//һά���ٸ���Ҷ���任
	void ImageFFt1D(ComplexNumber *arrayBuf, int n);
	
	//һά���ٸ���Ҷ���任
	void ImageRevFFt1D(ComplexNumber *arrayBuf, int n);
	
	//��ά���ٸ���Ҷ�任
	void ImageFFt2D(unsigned char* imgBuf, int width, int height, unsigned char *imgBufOut);
	
	//��ά���ٸ���Ҷ�任
	void ImageRevFFt2D(unsigned char* imgBufOut, int width, int height);
};

#endif // !defined(AFX_FOURIERTRANS_H__7C7EA6C8_0851_4F24_89CE_482E569B25C9__INCLUDED_)
