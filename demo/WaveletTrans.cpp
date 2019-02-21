// WaveletTrans.cpp: implementation of the WaveletTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "WaveletTrans.h"
#include "math.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WaveletTrans::WaveletTrans()
{
	m_nColorTableLengthOut=0;
	m_nScale = 0;
    m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_pColorTableOut=NULL;
	pCoeff = NULL;
}

WaveletTrans::WaveletTrans(CSize size, int nBitCount, LPRGBQUAD pColorTable, 
						   unsigned char *pImgData)
{
	m_nBitCountOut=m_nBitCount;
	m_pColorTableOut=m_lpColorTable;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];
	for(int i=0 ; i < m_imgWidth ; i++)
		for ( int j = 0 ; j < m_imgHeight ; j++)
			m_pImgDataOut[j * lineByte + i]=pImgData[j * lineByte + i];
	pCoeff = NULL;
}

WaveletTrans::~WaveletTrans()
{
	if (m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if (pCoeff != NULL)
	{
		delete[] pCoeff;
		pCoeff = NULL;
	}
}



/**********************************************************************
* �������ƣ� InputImageData()
* ������ CSize size 
*        int nBitCount 
*        LPRGBQUAD lpColorTable ��ɫ��ָ��
*        unsigned char *pImgData ͼ������
* ����ֵ�� void
* ˵���� ��view��ͼ�����ݸ���
**********************************************************************/
void WaveletTrans::InputImageData(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData)
{
	ReplaceDib(size, nBitCount,  lpColorTable, pImgData);
	
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	m_pColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
	memcpy(m_pColorTableOut,m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
}

CSize WaveletTrans::GetDimensions()
{
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

/**********************************************************************
* �������ƣ� SimpleWaveletTrans()
* ������     int scale �ֽ�߶ȣ�Ĭ��Ϊ1
* ����ֵ��   void
* ˵����     ��ͼ�����ݽ��и����߶ȵ�С���ֽ�
**********************************************************************/
void WaveletTrans::BasicWaveletTrans(int scale=1)
{
	if (scale<1 || scale>5)
	{
		return;
	}
	if(m_pImgDataOut!=NULL){
		delete[] m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	m_imgHeightOut=m_imgHeight;
	m_imgWidthOut=m_imgWidth;

	m_nScale=scale;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];
	WavlDecmposeTrans(m_pImgData, m_imgHeight, m_imgWidth, lineByte, scale, m_pImgDataOut);
}

/**********************************************************************
* �������ƣ�WavlDecmposeTrans()
* ����: unsigned char *inputData �����������ָ��
*       int inputHeight �������߶�
*       int inputWidth ���������
*       int lineByte �������ֽ���
*       int scale �ֽ�߶�
*       unsigned char *outputData �����������ָ��
* ����ֵ��void
* ˵��������һ��С���ֽ�
************************************************************************/
void WaveletTrans::WavlDecmposeTrans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData)
{   
	double r1=2.6;
	double r2=1.3;
	int j=4-scale;
	double K=1/8;
	double H11,H10,H01;

	unsigned char *buff = new unsigned char[lineByte*m_imgHeight];
	
	int imageHeight = inputHeight;
	int imageWidth = inputWidth;
	
	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];

	//��ͼ���������в���
	MatrixSampleRow(inputData, temp1, temp2, (imageHeight/2), imageWidth, lineByte);
	//�в�֣��õ���Ƶ����temp1
	MatrixSub(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ե�Ƶ����temp2�����в���
	MatrixSampleLine(temp2, temp01, temp00, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�ֵõ�LL 00��LH 01
	MatrixSub(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ը�Ƶ���ݽ����в���
	MatrixSampleLine(temp1, temp11, temp10, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�֣��õ�HL 10��HH 11
	MatrixSub(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);

	H11=sqrt(1+1)/K;
	H11=H11/pow(2,j);
	H11=pow(H11,4);
	H11=H11*2.415+1;
	H11=1/H11;
	H11=r1-r2*H11;
    
	H10=sqrt(1+0)/K;
	H10=H10/pow(2,j);
	H10=pow(H10,4);
	H10=H10*2.415+1;
	H10=1/H10;
	H10=r1-r2*H10;

	H01=H10;
    
	int i;
    for(i=0;i<(imageWidth/2) * (imageHeight/2);i++){
		temp11[i]=temp11[i]*H11;
		temp10[i]=temp10[i]*H10;
		temp01[i]=temp01[i]*H01;     
    }

	MatrixRegionCopy(temp01, (imageHeight/2), (imageWidth/2), buff+lineByte*(int)(imageHeight/2)+(int)(imageWidth/2), imageHeight, lineByte);
	MatrixRegionCopy(temp10, (imageHeight/2), (imageWidth/2), buff, imageHeight, lineByte);
	MatrixRegionCopy(temp11, (imageHeight/2), (imageWidth/2), buff+(int)(imageWidth/2), imageHeight, lineByte);

	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	
	scale--;
   
	//��LL2ϵ�������Ծ������
	if (scale==0){
        double m;
		double sum=0;
		double k=0.3;
		for(i=0;i<(imageWidth/2) * (imageHeight/2);i++){
			sum+=temp00[i];
		    m=sum/((imageWidth/2) * (imageHeight/2));
		    temp00[i]=temp00[i]-m;
		    temp00[i]=temp00[i]*k;
		    temp00[i]=temp00[i]+m;
            temp00[i]=(r1-r2)*temp00[i];	
		}
	}


	//������LL���еݹ�ֽ�
	if (scale > 0)
	{
		WavlDecmposeTrans(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	MatrixRegionCopy(temp00, (imageHeight/2), (imageWidth/2), buff+lineByte*(int)(imageHeight/2),
		imageHeight, lineByte);
	delete[] temp00;
	
	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}

/**********************************************************************
* �������ƣ� MatrixSampleRow()
* ������ unsigned char *matrixInput ��������������ָ��
*        unsigned char *matrixOutputOdd �����в������ɵľ���
*        unsigned char *matrixOutputEven ż���в������ɵľ���
*        int heightOutput �������߶�
*        int widthOutput ���������
*        int widthInput ���������
* ����ֵ�� void
* ˵���� �������������г�����
 **********************************************************************/
void WaveletTrans::MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd, unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput)
{
	int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * 2 * widthInput + w];//ż����
			matrixOutputOdd[h * widthOutput + w] = matrixInput[(h * 2 + 1) * widthInput + w];//������
		}
	}
}

/**********************************************************************
* �������ƣ� MatrixSub()
* ������ unsigned char *matrixA ��������A����ָ��
*        unsigned char *matrixB ��������B����ָ��
*        int height �߶�
*        int width ���
*        unsigned char *result ���������ָ��
* ����ֵ�� void
* ˵���� ���������������A��B���
**********************************************************************/
void WaveletTrans::MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result)
{
	unsigned char temp;
	int h, w;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = matrixA[h * width + w] - matrixB[h * width + w] + 128;
			result[h * width + w] = temp;
		}
	}
}

/**********************************************************************
 * �������ƣ� MatrixSampleLine()
 * ������ unsigned char *matrixInput ��������������ָ��
 *        unsigned char *matrixOutputOdd �����в������ɵľ���
 *        unsigned char *matrixOutputEven ż���в������ɵľ���
 *        int heightOutput �������߶�
 *        int widthOutput ���������
 *        int widthInput ���������
 * ����ֵ��void
 * ˵���� �������������г�����
 **********************************************************************/
void WaveletTrans::MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
										  unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput)
{
	int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * widthInput + w * 2];
			matrixOutputOdd[h * widthOutput + w] = matrixInput[h * widthInput + w * 2 + 1];
		}
	}
}

/**********************************************************************
 * �������ƣ� MatrixPlus()
 * ������ unsigned char *matrixA ����;���A����ָ��
 *        unsigned char *matrixB ����;���B����ָ��
 *        int height �߶�
 *        int width ���
 *        unsigned char *result �;�������ָ��
 * ����ֵ��void
 * ˵���� ���������������A��B���
 **********************************************************************/
void WaveletTrans::MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result)
{
	unsigned char temp;
	int h, w;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = matrixA[h * width + w] + matrixB[h * width + w] - 128;
			result[h * width + w] = temp;
		}
	}
}

/**********************************************************************
* �������ƣ�MatrixRegionCopy()
* ������ unsigned char *matrixRegion Դ��������ָ��
*        int heightRegion ��������ĸ߶�
*        int widthRegion ��������Ŀ��
*        unsigned char *matrixDest Ŀ�ľ�������ָ��
*        int heightDest Ŀ�ľ�������ĸ߶�
*        int widthDest Ŀ�ľ�������Ŀ��
* ����ֵ��void
* ˵���� ��Դ�����ָ���������ݸ��Ƶ�Ŀ�ľ����ָ������
**********************************************************************/
void WaveletTrans::MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,
										  unsigned char *matrixDest, int heightDest, int widthDest)
{
	//��������߶ȺͿ��
	int heightMin = heightRegion>heightDest ? heightDest : heightRegion;
	int widthMin = widthRegion>widthDest ? widthDest : widthRegion;
	int h, w;
	for (h=0; h<heightMin; h++)
	{
		for (w=0; w<widthMin; w++)
		{
			matrixDest[h * widthDest + w] = matrixRegion[h * widthRegion + w];
		}
	}
}


/**********************************************************************
* �������ƣ� BasicWaveletReverse()
* ������ void
* ����ֵ��void
* ˵���� ����ӦС���ֽ�ϵ������С���ع�
**********************************************************************/
void WaveletTrans::BasicWaveletReverse()
{
	if (m_nScale <= 0)
	{
		return;
	}
	if (m_pImgDataOut==NULL)
	{
		return;
	}
	m_imgHeightOut=m_imgHeight;
	m_imgWidthOut=m_imgWidth;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	this->WavlRebuildTrans(m_pImgDataOut, m_imgHeight, m_imgWidth, lineByte, m_nScale, m_pImgDataOut);
}


/**********************************************************************
* �������ƣ�WavlRebuildTrans()
* ������ unsigned char *inputData �����������ָ��
*        int inputHeight �������߶�
*        int inputWidth ���������
*        int lineByte �������ֽ���
*        int scale �ֽ�߶�
*        unsigned char *outputData �����������ָ��
* ����ֵ��void
* ˵���� ����һ��С���ع�
**********************************************************************/
void WaveletTrans::WavlRebuildTrans(unsigned char *inputData, int inputHeight, 
				int inputWidth, int lineByte, int scale, unsigned char *outputData)
{
	unsigned char *buff = new unsigned char[lineByte*m_imgHeight];
	
	int imageHeight = inputHeight;
	int imageWidth = inputWidth;
		
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	this->MatrixRegionCopy(inputData, imageHeight, lineByte, temp10, (imageHeight/2), (imageWidth/2));
	this->MatrixRegionCopy(inputData+(int)(imageWidth/2), imageHeight, lineByte, temp11, (imageHeight/2), (imageWidth/2));
	this->MatrixPlus(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);
	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	this->MatrixInsertLine(temp11, temp10, temp1, (imageHeight/2), (imageWidth/2), imageWidth);
	
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	this->MatrixRegionCopy(inputData+lineByte*(int)(imageHeight/2), imageHeight, lineByte, temp00, (imageHeight/2), (imageWidth/2));
	this->MatrixRegionCopy(inputData+lineByte*(int)(imageHeight/2)+(int)(imageWidth/2),imageHeight, lineByte, temp01, (imageHeight/2), (imageWidth/2));
		
	
	scale--;
	//�ݹ��LL���ݽ����ع�
	if (scale > 0)
	{
		this->WavlRebuildTrans(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	this->MatrixPlus(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	this->MatrixInsertLine(temp01, temp00, temp2, (imageHeight/2), (imageWidth/2), imageWidth);
	
	this->MatrixPlus(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	this->MatrixInsertRow(temp1, temp2, buff, (imageHeight/2), imageWidth, lineByte);
	
	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	delete[] temp00;
	
	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}


/**********************************************************************
* �������ƣ�MatrixInsertLine()
* ������ unsigned char *matrixInputOdd ���������в������ɵľ���
*        unsigned char *matrixInputEven ����ż���в������ɵľ���
*        unsigned char *matrixOutput ���������������ָ��
*        int heightInput �������߶�
*        int widthInput ���������
*        int widthOutput ���������
* ����ֵ�� void
* ˵���� �������������в�����
**********************************************************************/
void WaveletTrans::MatrixInsertLine(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
										  unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput)
{
	int h, w;
	for (h=0; h<heightInput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			if (w%2 == 0)
			{
				matrixOutput[h * widthOutput + w] = matrixInputEven[h * widthInput + w/2];
			} 
			else
			{
				matrixOutput[h * widthOutput + w] = matrixInputOdd[h * widthInput + w/2];
			}		
		}
	}
}

/**********************************************************************
* �������ƣ� MatrixInsertRow()
* ������ unsigned char *matrixInputOdd ���������в������ɵľ���
*        unsigned char *matrixInputEven ����ż���в������ɵľ���
*        unsigned char *matrixOutput ���������������ָ��
*        int heightInput �������߶�
*        int widthInput ���������
*        int widthOutput ���������
* ����ֵ:void
* ˵���� �������������в�����
**********************************************************************/
void WaveletTrans::MatrixInsertRow(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
										 unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput)
{
	int h, w;
	for (h=0; h<heightInput*2; h++)
	{
		for (w=0; w<widthInput; w++)
		{
			if (h%2 == 0)
			{
				matrixOutput[h * widthOutput + w] = matrixInputEven[(int)(h/2) * widthInput + w];
			} 
			else
			{
				matrixOutput[h * widthOutput + w] = matrixInputOdd[(int)(h/2) * widthInput + w];
			}		
		}
	}
}

