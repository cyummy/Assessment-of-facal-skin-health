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
* 函数名称： InputImageData()
* 参数： CSize size 
*        int nBitCount 
*        LPRGBQUAD lpColorTable 调色板指针
*        unsigned char *pImgData 图像数据
* 返回值： void
* 说明： 将view中图像数据更新
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
* 函数名称： SimpleWaveletTrans()
* 参数：     int scale 分解尺度，默认为1
* 返回值：   void
* 说明：     对图像数据进行给定尺度的小波分解
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
* 函数名称：WavlDecmposeTrans()
* 参数: unsigned char *inputData 输入矩阵数组指针
*       int inputHeight 输入矩阵高度
*       int inputWidth 输入矩阵宽度
*       int lineByte 矩阵行字节数
*       int scale 分解尺度
*       unsigned char *outputData 输出矩阵数组指针
* 返回值：void
* 说明：进行一遍小波分解
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

	//对图像矩阵进行行采样
	MatrixSampleRow(inputData, temp1, temp2, (imageHeight/2), imageWidth, lineByte);
	//行差分，得到高频数据temp1
	MatrixSub(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//对低频数据temp2进行列采样
	MatrixSampleLine(temp2, temp01, temp00, (imageHeight/2), (imageWidth/2), imageWidth);
	//列差分得到LL 00和LH 01
	MatrixSub(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//对高频数据进行列采样
	MatrixSampleLine(temp1, temp11, temp10, (imageHeight/2), (imageWidth/2), imageWidth);
	//列差分，得到HL 10和HH 11
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

	//释放空间
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	
	scale--;
   
	//对LL2系数作线性均衡调整
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


	//继续对LL进行递归分解
	if (scale > 0)
	{
		WavlDecmposeTrans(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	MatrixRegionCopy(temp00, (imageHeight/2), (imageWidth/2), buff+lineByte*(int)(imageHeight/2),
		imageHeight, lineByte);
	delete[] temp00;
	
	//把最后的结果拷贝到m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}

/**********************************************************************
* 函数名称： MatrixSampleRow()
* 参数： unsigned char *matrixInput 待采样矩阵数组指针
*        unsigned char *matrixOutputOdd 奇数行采样生成的矩阵
*        unsigned char *matrixOutputEven 偶数行采样生成的矩阵
*        int heightOutput 输出矩阵高度
*        int widthOutput 输出矩阵宽度
*        int widthInput 输入矩阵宽度
* 返回值： void
* 说明： 对输入矩阵进行行抽点采样
 **********************************************************************/
void WaveletTrans::MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd, unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput)
{
	int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * 2 * widthInput + w];//偶数行
			matrixOutputOdd[h * widthOutput + w] = matrixInput[(h * 2 + 1) * widthInput + w];//奇数行
		}
	}
}

/**********************************************************************
* 函数名称： MatrixSub()
* 参数： unsigned char *matrixA 待求差矩阵A数组指针
*        unsigned char *matrixB 待求差矩阵B数组指针
*        int height 高度
*        int width 宽度
*        unsigned char *result 差矩阵数组指针
* 返回值： void
* 说明： 将输入的两个矩阵A和B求差
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
 * 函数名称： MatrixSampleLine()
 * 参数： unsigned char *matrixInput 待采样矩阵数组指针
 *        unsigned char *matrixOutputOdd 奇数列采样生成的矩阵
 *        unsigned char *matrixOutputEven 偶数列采样生成的矩阵
 *        int heightOutput 输出矩阵高度
 *        int widthOutput 输出矩阵宽度
 *        int widthInput 输入矩阵宽度
 * 返回值：void
 * 说明： 对输入矩阵进行列抽点采样
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
 * 函数名称： MatrixPlus()
 * 参数： unsigned char *matrixA 待求和矩阵A数组指针
 *        unsigned char *matrixB 待求和矩阵B数组指针
 *        int height 高度
 *        int width 宽度
 *        unsigned char *result 和矩阵数组指针
 * 返回值：void
 * 说明： 将输入的两个矩阵A和B求和
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
* 函数名称：MatrixRegionCopy()
* 参数： unsigned char *matrixRegion 源矩阵数组指针
*        int heightRegion 复制区域的高度
*        int widthRegion 复制区域的宽度
*        unsigned char *matrixDest 目的矩阵数组指针
*        int heightDest 目的矩阵区域的高度
*        int widthDest 目的矩阵区域的宽度
* 返回值：void
* 说明： 将源矩阵的指定区域数据复制到目的矩阵的指定区域
**********************************************************************/
void WaveletTrans::MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,
										  unsigned char *matrixDest, int heightDest, int widthDest)
{
	//计算区域高度和宽度
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
* 函数名称： BasicWaveletReverse()
* 参数： void
* 返回值：void
* 说明： 对相应小波分解系数进行小波重构
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
* 函数名称：WavlRebuildTrans()
* 参数： unsigned char *inputData 输入矩阵数组指针
*        int inputHeight 输入矩阵高度
*        int inputWidth 输入矩阵宽度
*        int lineByte 矩阵行字节数
*        int scale 分解尺度
*        unsigned char *outputData 输出矩阵数组指针
* 返回值：void
* 说明： 进行一遍小波重构
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
	//递归对LL数据进行重构
	if (scale > 0)
	{
		this->WavlRebuildTrans(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	this->MatrixPlus(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	this->MatrixInsertLine(temp01, temp00, temp2, (imageHeight/2), (imageWidth/2), imageWidth);
	
	this->MatrixPlus(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	this->MatrixInsertRow(temp1, temp2, buff, (imageHeight/2), imageWidth, lineByte);
	
	//释放空间
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	delete[] temp00;
	
	//把最后的结果拷贝到m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}


/**********************************************************************
* 函数名称：MatrixInsertLine()
* 参数： unsigned char *matrixInputOdd 输入奇数列采样生成的矩阵
*        unsigned char *matrixInputEven 输入偶数列采样生成的矩阵
*        unsigned char *matrixOutput 输出待插点矩阵数组指针
*        int heightInput 输入矩阵高度
*        int widthInput 输入矩阵宽度
*        int widthOutput 输出矩阵宽度
* 返回值： void
* 说明： 对输入矩阵进行列插点采样
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
* 函数名称： MatrixInsertRow()
* 参数： unsigned char *matrixInputOdd 输入奇数行采样生成的矩阵
*        unsigned char *matrixInputEven 输入偶数行采样生成的矩阵
*        unsigned char *matrixOutput 输出待插点矩阵数组指针
*        int heightInput 输入矩阵高度
*        int widthInput 输入矩阵宽度
*        int widthOutput 输出矩阵宽度
* 返回值:void
* 说明： 对输入矩阵进行行插点采样
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

