// Histogram.h: interface for the Histogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTOGRAM_H__CA29573E_CFD7_43A7_8605_C55CE729399B__INCLUDED_)
#define AFX_HISTOGRAM_H__CA29573E_CFD7_43A7_8605_C55CE729399B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class Histogram : public ImageDib  
{
public:
	//ֱ��ͼ����
	int m_histArray[256];

	//ֱ��ͼƽ��ֵ
	float m_average;
	
	//ֱ��ͼ����
	float m_deviation;
	
	//���ͼ��ÿ����λ��
	int m_nBitCountOut;
	
	//���ͼ��λͼ����ָ��
	unsigned char * m_pImgDataOut;
	
	//���ͼ����ɫ��
	LPRGBQUAD m_lpColorTableOut;
	
private:
	//���ͼ��Ŀ�����Ϊ��λ
	int m_imgWidthOut;
	
	//���ͼ��ĸߣ�����Ϊ��λ
	int m_imgHeightOut;
	
	//���ͼ����ɫ����
	int m_nColorTableLengthOut;
	
public:
	//���������Ĺ��캯��
	Histogram();
	
	//�������Ĺ��캯��
	Histogram(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
	
	//��������
	~Histogram();
	
	//������Ϊ��λ�������ͼ��Ŀ�͸�
	CSize GetDimensions();
	
	//ͳ��ֱ��ͼ
	void computeHistGray();
	
	//��ɫͼ������ֱ��ͼ
	void computeHistBrightness();
	
	//����ֱ��ͼ��ֵ
	float computeAverage();
	
	//����ֱ��ͼ����
   float computeDeviation();
	
	//ֱ��ͼ����
	void histAve();
};

#endif // !defined(AFX_HISTOGRAM_H__CA29573E_CFD7_43A7_8605_C55CE729399B__INCLUDED_)
