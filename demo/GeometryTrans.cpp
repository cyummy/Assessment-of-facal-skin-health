// GeometryTrans.cpp: implementation of the GeometryTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "GeometryTrans.h"
#include "math.h"
//#define max(a,b) (a>b?a:b)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


GeometryTrans::GeometryTrans()
{

	m_pImgDataOut=NULL;//输出图像位图数据指针为空
	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0
    m_nBitCountOut=0;//输出图像每像素位数为0	
	m_imgWidthOut=0;//输出图像的宽为0
	m_imgHeightOut=0;//输出图像的高为0	
}

/***********************************************************************
* 函数名称： GeometryTrans()
* 函数参数： CSize size -图像大小（宽、高）
*            int nBitCount  -每像素所占位数
*            LPRGBQUAD lpColorTable  -颜色表指针
*            unsigned char *pImgData  -位图数据指针
* 返回值：   无
* 说明：本函数为带参数的构造函数，给定位图的大小、每像素位数、颜色表
*      及位图数据，调用ImgDib()对基类成员初始化，并初始化派生类的
*      数据成员
***********************************************************************/
GeometryTrans::GeometryTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
							 unsigned char *pImgData):
ImageDib(size, nBitCount, lpColorTable, pImgData)
{	
	//输出图像每像素位数与输入图像相同
    m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表与输入图像相同
	if(m_nColorTableLengthOut){
		//分配颜色表缓冲区，进行颜色表拷贝
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	else// 彩色图像没有颜色表
		 m_lpColorTableOut=NULL;

	//输出图像指针为空
	m_pImgDataOut=NULL;

	//输出图像宽和高置0
	m_imgWidthOut=0;
	m_imgHeightOut=0;
}

/***********************************************************************
* 函数名称：  ~GeometryTrans()
* 说明：析构函数，释放资源
***********************************************************************/
GeometryTrans::~GeometryTrans()
{
	//释放输出图像位图数据缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//释放输出图像颜色表
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}


/***********************************************************************
* 函数名称： GetDimensions()
* 函数参数： 无
* 返回值：   图像的尺寸，用CSize类型表达
* 说明：返回输出图像的宽和高
***********************************************************************/
CSize GeometryTrans::GetDimensions()
{
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

/***********************************************************************
* 函数名称：Move()
* 函数参数：int Xmove  -水平方向的平移量，以像素为单位
*           int Ymove  -垂直方向的平移量，以像素为单位
* 说明：当给定水平、垂直方向的位移量后，可实现对图像的平移操作，
        函数不改变图像的大小，超出图像的部分用黑色填充。
***********************************************************************/
void GeometryTrans::Move(int Xmove, int Ymove)
{
	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//输出图像的宽和高
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素字节数，输出图像与输入图像相等
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请缓冲区，存放输出结果
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//置黑色
	memset(m_pImgDataOut,0,lineByte*m_imgHeight);

	//循环变量，图像坐标
	int i,j;

	//循环变量,像素的每个通道
	int k;

	//每像素字节数，输出图像与输入图像相等
	int pixelByte=m_nBitCountOut/8;

	//平移运算
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			//输出的点在输入图像范围内
			if(i-Ymove>=0&&i-Ymove<m_imgHeight&&j-Xmove>=0&&j-Xmove<m_imgWidth)
			{
				for(k=0;k<pixelByte;k++)
					*(m_pImgDataOut+i*lineByte+j*pixelByte+k)
					=*(m_pImgData+(i-Ymove)*lineByte+(j-Xmove)*pixelByte+k);
			}
		}
	}
}

/***********************************************************************
* 函数名称： MirrorHorTrans()
* 说明：对图像水平镜像
***********************************************************************/
void GeometryTrans::MirrorHorTrans()
{
    //释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//输出图像的宽和高
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素字节数，输出图像与输入图像相等
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请缓冲区，存放输出结果
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//循环变量，图像坐标
	int i,j;

	//循环变量,像素的每个通道
	int k;

	//每像素字节数，输出图像与输入图像相等
	int pixelByte=m_nBitCountOut/8;

	//水平镜像
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++)
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)
				=*(m_pImgData+i*lineByte+(m_imgWidth-1-j)*pixelByte+k);
		}
	}
}

/***********************************************************************
* 函数名称： MirrorVerTrans()
* 说明：对图像垂直镜像
***********************************************************************/
void GeometryTrans::MirrorVerTrans()
{
	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//输出图像的宽和高
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素字节数，输出图像与输入图像相等
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请缓冲区，存放输出结果
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//循环变量，图像坐标
	int i,j;

	//循环变量,像素的每个通道
	int k;

	//每像素字节数，输出图像与输入图像相等
	int pixelByte=m_nBitCountOut/8;

	//垂直镜像
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++)
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)
				=*(m_pImgData+(m_imgHeight-1-i)*lineByte+j*pixelByte+k);
		}
	}
}

/***********************************************************************
* 函数名称： Clockwise90()
* 说明：对图像顺时针旋转90度
***********************************************************************/
void GeometryTrans::Clockwise90()
{
	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//输入图像每行像素字节数
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//输出图像的宽和高
	m_imgWidthOut=m_imgHeight;
	m_imgHeightOut=m_imgWidth;

	//输出图像每行像素字节数
	int lineByteOut=(m_imgWidthOut*m_nBitCount/8+3)/4*4;

	//申请缓冲区，存放输出结果
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//循环变量，图像坐标
	int i,j;

	//循环变量,像素的每个通道
	int k;

	//每像素字节数，输出图像与输入图像相等
	int pixelByte=m_nBitCountOut/8;

	//顺时针90度
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			for(k=0;k<pixelByte;k++)
				*(m_pImgDataOut+i*lineByteOut+j*pixelByte+k)
				=*(m_pImgData+j*lineByte+(m_imgWidth-1-i)*pixelByte+k);
		}
	}
}


/***********************************************************************
* 函数名称： Anticlockwise90()
* 说明：对图像逆时针旋转90度
***********************************************************************/
void GeometryTrans::Anticlockwise90()
{
	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//输入图像每行像素字节数
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//输出图像的宽和高
	m_imgWidthOut=m_imgHeight;
	m_imgHeightOut=m_imgWidth;

	//输出图像每行像素字节数
	int lineByteOut=(m_imgWidthOut*m_nBitCount/8+3)/4*4;

	//申请缓冲区，存放输出结果
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//循环变量，图像坐标
	int i,j;

	//循环变量,像素的每个通道
	int k;

	//每像素字节数，输出图像与输入图像相等
	int pixelByte=m_nBitCountOut/8;

	//逆时针90度
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			for(k=0;k<pixelByte;k++)
				*(m_pImgDataOut+i*lineByteOut+j*pixelByte+k)
				=*(m_pImgData+(m_imgHeight-1-j)*lineByte+i*pixelByte+k);
		}
	}
}

/***********************************************************************
* 函数名称：Rotate180()
* 说明：对图像旋转180度
***********************************************************************/
void GeometryTrans::Rotate180()
{
	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//输出图像的宽和高
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素字节数，输出图像与输入图像相等
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请缓冲区，存放输出结果
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//循环变量，图像坐标
	int i,j;

	//循环变量,像素的每个通道
	int k;

	//每像素字节数，输出图像与输入图像相等
	int pixelByte=m_nBitCountOut/8;

	//旋转180度
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			for(k=0;k<pixelByte;k++)
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)
				=*(m_pImgData+(m_imgHeight-1-i)*lineByte+(m_imgWidth-1-j)*pixelByte+k);
		}
	}
}


/*************************************************************************
 * 函数名称： Zoom()
 * 参数:   float ratioX  -宽度缩放量
 *         float ratioY  -高度缩放量
  * 返回值: 无
 * 说明:   根据指定的大小和指定的插值方式，对原图像进行缩放，缩放后的结果存放
 *          在m_pImgDataOut缓冲区中，该函数同时适合于彩色和灰度图像
 ************************************************************************/
void GeometryTrans::Zoom(float ratioX, float ratioY)
{
	
	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
    
	//输出图像的宽和高
	m_imgWidthOut=int(m_imgWidth*ratioX+0.5) ;
	m_imgHeightOut=int(m_imgHeight*ratioY+0.5); 

	//输入图像每行像素字节数
	int lineByteIn=(m_imgWidth*m_nBitCount/8+3)/4*4;
	
	//输出图像每行像素字节数
    int	lineByteOut=(m_imgWidthOut*m_nBitCount/8+3)/4*4;

	//申请缓冲区，存放输出结果
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//每像素字节数，输入图像与输出图像相同
	int pixelByte=m_nBitCount/8;
	
	//输出图像在输入图像中待插值的位置坐标
	int coordinateX, coordinateY;
	
	//循环变量，输出图像的坐标
	int i, j;
	
	//循环变量,像素的每个通道
	int k;
	
	//近邻插值
	for(i=0; i< m_imgHeightOut; i++){
		for(j=0; j<m_imgWidthOut; j++){  
			//输出图像坐标为(j,i)的像素映射到原图中的坐标值，即插值位置
			coordinateX=j/ratioX+0.5;
			coordinateY=i/ratioY+0.5;
			
			//若插值位置在输入图像范围内，则近邻插值
			if(0<=coordinateX&&coordinateX<m_imgWidth
				&& coordinateY>=0&&coordinateY<m_imgHeight){
				for(k=0;k<pixelByte;k++)
					*(m_pImgDataOut + i * lineByteOut + j*pixelByte + k) 
					=*(m_pImgData+ coordinateY*lineByteIn+ coordinateX*pixelByte + k) ;
			}
			else //若不在输入图像范围内，则置255  
			{
				for(k=0;k<pixelByte;k++)
					*(m_pImgDataOut + i * lineByteOut + j*pixelByte+k) = 255;
			}
			
		}
	}


}


/*************************************************************************
 * 函数名称：  Rotate()
 * 参数:   int angle  -旋转角度
 * 说明:   根据指定旋转角度，调用近邻插值旋转函数对原图像进行旋转，
 *         旋转结果输出至m_pImgDataOut缓冲区中，该函数同时适合于彩色和灰度图像
 ************************************************************************/
void GeometryTrans::Rotate(int angle)
{
	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//每像素所占字节数，输入图像与输出图像相同
	int pixelByte=m_nBitCount/8;

	// 输入图像每行像素所占字节数
	int lineByte=(m_imgWidth*pixelByte+3)/4*4;

	// 旋转角度（弧度）, 将旋转角度从度转换到弧度
	float	fRotateAngle= 2*3.1415926*angle/360;
	
	// 输入图像四个角的坐标，以图像中心为坐标系原点
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	
	// 旋转后四个角的坐标，以图像中心为坐标系原点
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// 计算旋转角度的正弦
	float fSina = (float) sin((double)fRotateAngle);
	
	// 计算旋转角度的余弦
	float fCosa = (float) cos((double)fRotateAngle);
	
	// 计算原图的四个角的坐标，以图像中心为坐标系原点
	fSrcX1 = (float) (- (m_imgWidth  - 1) / 2);
	fSrcY1 = (float) (  (m_imgHeight - 1) / 2);
	fSrcX2 = (float) (  (m_imgWidth  - 1) / 2);
	fSrcY2 = (float) (  (m_imgHeight - 1) / 2);
	fSrcX3 = (float) (- (m_imgWidth  - 1) / 2);
	fSrcY3 = (float) (- (m_imgHeight - 1) / 2);
	fSrcX4 = (float) (  (m_imgWidth  - 1) / 2);
	fSrcY4 = (float) (- (m_imgHeight - 1) / 2);
	
	// 计算新图四个角的坐标，以图像中心为坐标系原点
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	// 旋转后输出图像宽度
	m_imgWidthOut  = (LONG) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	// 旋转后输出图像高度
	m_imgHeightOut = (LONG) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) ) + 0.5);

	// 旋转后输出图像每行的字节数
	int lineByteOut=(m_imgWidthOut*pixelByte+3)/4*4;
	
	//分配缓冲区，存放旋转结果
	m_pImgDataOut = new unsigned char[lineByteOut*m_imgHeightOut];

	// 两个常数，这样不用以后每次都计算了
	float f1 = (float) (-0.5 * (m_imgWidthOut - 1) * fCosa 
		                + 0.5 * (m_imgHeightOut - 1) * fSina + 0.5 * (m_imgWidth  - 1));
	float f2 = (float) (-0.5 * (m_imgWidthOut - 1) * fSina 
		                - 0.5 * (m_imgHeightOut - 1) * fCosa + 0.5 * (m_imgHeight - 1));

	// 循环变量，输出图像坐标
	int	i, j;

	//循环变量，像素的每个通道
	int k;
	
	//输出图像在输入图像中待插值的位置坐标，必须浮点型
	int	coordinateX, coordinateY;

	// 最近邻插值旋转
	for(i = 0; i < m_imgHeightOut; i++)
	{
		for(j = 0; j < m_imgWidthOut; j++)
		{
			
			// 输出图像像素(j,i)映射到输入图像的坐标，近邻插值取整数
			coordinateX = (int)(j * fCosa - i * fSina + f1 + 0.5);
			coordinateY = (int)(j * fSina + i * fCosa + f2 + 0.5);
			
			// 判断是否在输入图像范围内
			if( (coordinateX >= 0) && (coordinateX < m_imgWidth) && (coordinateY >= 0) 
				&& (coordinateY < m_imgHeight))
			{
				//将图像每个通道的数据进行分别插值，彩色图像pixelByte为3，
				//灰度图像pixelByte为1
				for(k=0; k<pixelByte; k++)
					*(m_pImgDataOut+i*lineByteOut+j*pixelByte+k)
					=*(m_pImgData+coordinateY*lineByte+coordinateX*pixelByte+k);
			}
			else
			{
				// 对于不在原图中的像素，赋值为255
				for(k=0; k<pixelByte; k++)
		    		*(m_pImgDataOut+i*lineByteOut+j*pixelByte+k) = 255;
			}
			
		}
		
	}
	
}







