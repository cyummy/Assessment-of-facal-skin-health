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
	int m_imgWidthOut; //���ͼ��Ŀ�
	int m_imgHeightOut;//���ͼ��ĸ�
	int m_nScale;//�ֽ�߶�	

public:
	//���캯��
	WaveletTrans();
    //�������Ĺ��캯��
	WaveletTrans(CSize size, int nBitCount, LPRGBQUAD pColorTable, unsigned char *pImgData);
	//��������
	virtual ~WaveletTrans();

public:
	void InputImageData(CSize size, int nBitCount,LPRGBQUAD lpColorTable, unsigned char *pImgData);
	CSize GetDimensions();//������Ϊ��λ�������ͼ��Ŀ�͸�
protected:
	//����������
	void MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,
		unsigned char *matrixDest, int heightDest, int widthDest);
	//�������
	void MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);	
	//�в���
	void MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);
	//�в���
	void MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);
public:	
	//һ��С���ֽ�
	void WavlDecmposeTrans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	//С���任
	void BasicWaveletTrans(int scale);
	
protected:
	//�������
	void MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);
	//�в��
	void MatrixInsertRow(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);
	//�в��
	void MatrixInsertLine(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);
public:
	//һ��С���ع�
	void WavlRebuildTrans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	//С���ع�
	void BasicWaveletReverse();

};


#endif // !defined(AFX_WAVELETTRANS_H__CF82C3DD_BEBF_42EF_A229_9B73FADFD68F__INCLUDED_)
