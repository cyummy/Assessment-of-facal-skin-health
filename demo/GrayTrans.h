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
	//���ͼ��ÿ����λ��
	int m_nBitCountOut;
	
	//���ͼ��λͼ����ָ��
	unsigned char * m_pImgDataOut;
	
	//���ͼ����ɫ��
	LPRGBQUAD m_lpColorTableOut;

	//���ͼ��Ŀ�����Ϊ��λ
	int m_imgWidthOut;
	
	//���ͼ��ĸߣ�����Ϊ��λ
	int m_imgHeightOut;
	
	//���ͼ����ɫ����
	int m_nColorTableLengthOut;
	
public:
	void GaryImage();
	//���������Ĺ��캯��
	GrayTrans();
	
	//�������Ĺ��캯��
	GrayTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	
	//��������
	~GrayTrans();
	
	//������Ϊ��λ�������ͼ��Ŀ�͸�
	CSize GetDimensions();
	
	//��ֵ��
	void BinaryImage(int threshold=128);

	//��ת
	void RevImage();

    //���ڱ任
	void ThresholdWindow(int bTop, int bBottom);
	
	//��������
	void LinearStrech(CPoint point1,CPoint point2);
	
private:
	//��ͨ��������������
	void LinearStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut,int imgWidth,int imgHeight,
		CPoint point1,CPoint point2);

};

#endif // !defined(AFX_GRAYTRANS_H__8F98F0EE_4F7D_4138_8573_4E4A065784C2__INCLUDED_)
