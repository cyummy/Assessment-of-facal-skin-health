// Morphology.h: interface for the Morphology class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MORPHOLOGY_H__9556E8FD_5B31_4368_9C45_62BC56507403__INCLUDED_)
#define AFX_MORPHOLOGY_H__9556E8FD_5B31_4368_9C45_62BC56507403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

//�ṹԪ�ض�,�ýṹר��Ϊ���л����б任������
struct ElementPair
{
	int hitElement[9];
	int missElement[9];
};

class Morphology : public ImageDib  
{

public:
	int m_nBitCountOut;	//���ͼ��ÿ����λ��
	unsigned char * m_pImgDataOut;//���ͼ��λͼ����ָ��
	LPRGBQUAD m_lpColorTableOut;	//���ͼ����ɫ��
	int m_imgWidthOut;	//���ͼ��Ŀ�
	int m_imgHeightOut;	//���ͼ��ĸ�
	int m_InitTemp[25];
	int *m_TempBuf;	//�ṹԪ�أ�ģ�壩ָ��
	int TempW;     //�ṹԪ�ؿ�
	int TempH;     //�ṹԪ�ظ�
	ElementPair m_hitMissTemp[8];	//����8������Ļ��л����б任�ṹԪ�ض�
public:
	Morphology();	//���������Ĺ��캯��
    Morphology(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);	//�������Ĺ��캯��
	virtual ~Morphology();	//��������
	CSize GetDimensions();	//�������ͼ��ĳߴ�
public:
	void ImgErosion(unsigned char *imgBufIn,unsigned char *imgBufOut,	int imgWidth,
		int imgHeight,int *TempBuf, int TempW, int TempH);//��ʴ
	
	void ImgDilation(unsigned char *imgBufIn,unsigned char *imgBufOut,int imgWidth,
		int imgHeight,int *maskBuf, int maskW, int maskH);//����
	
	void Open();	//��ֵ��
	void Close();	//��ֵ��
	
	void ImgThinning();	//���л�����ϸ��
	
	void DefineElementPair();//������л����б任�ĽṹԪ�ض�

	void HitAndMiss(unsigned char *imgBufIn, unsigned char *imgBufOut,
		int imgWidth,int imgHeight,ElementPair hitMissMask);	//���л����б任

};

#endif // !defined(AFX_MORPHOLOGY_H__9556E8FD_5B31_4368_9C45_62BC56507403__INCLUDED_)
