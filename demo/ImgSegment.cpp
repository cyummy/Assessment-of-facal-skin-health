// ImgSegment.cpp: implementation of the ImgSegment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "ImgSegment.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImgSegment::ImgSegment()
{
	m_pImgDataOut=NULL;//输出图像位图数据指针为空
	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0
    m_nBitCountOut=0;//输出图像每像素位数为0	
	m_imgWidthOut=0;//输出图像的宽为0
	m_imgHeightOut=0;//输出图像的高为0
}

ImgSegment::ImgSegment(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImageDib(size, nBitCount, lpColorTable, pImgData)
{
	m_pImgDataOut=NULL;//输出图像位图数据指针为空
	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0
    m_nBitCountOut=0;//输出图像每像素位数为0
	m_imgWidthOut=0;//输出图像的宽为0
	m_imgHeightOut=0;//输出图像的高为0
}

ImgSegment::~ImgSegment()
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




CSize ImgSegment::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

/***********************************************************************
* 函数名称：
* Roberts()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Roberts边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
***********************************************************************/
void ImgSegment::Roberts()
{
	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//循环变量，图像的坐标
	int i,j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte=m_nBitCount/8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//中间变量
	int x, y, t;

	//Roberts算子
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x方向梯度
				x=*(m_pImgData+i*lineByte+j*pixelByte+k)
					-*(m_pImgData+(i+1)*lineByte+j*pixelByte+k);

				//y方向梯度
				y=*(m_pImgData+i*lineByte+j*pixelByte+k)
					-*(m_pImgData+i*lineByte+(j+1)*pixelByte+k);

				t=sqrt(x*x+y*y)+0.5;
				if(t>255)
					t=255;
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=t;
			}
		}
	}
}

/***********************************************************************
* 函数名称：
* Sobel()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Sobel边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
***********************************************************************/
void ImgSegment::Sobel()
{
	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//循环变量，图像的坐标
	int i,j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte=m_nBitCount/8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//中间变量
	int x, y, t;

	//Sobel算子
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x方向梯度
				x= *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 + 2 * *(m_pImgData+i*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 - 2 * *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k);

				//y方向梯度
				y= *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 + 2 * *(m_pImgData+(i-1)*lineByte+j*pixelByte+k)
				 + *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k)
				 - 2 * *(m_pImgData+(i+1)*lineByte+j*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k);

				t=sqrt(x*x+y*y)+0.5;
				if(t>255)
					t=255;
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=t;
			}
		}
	}

}

/***********************************************************************
* 函数名称：
* Prewitt()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Prewitt边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
***********************************************************************/
void ImgSegment::Prewitt()
{
	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//循环变量，图像的坐标
	int i,j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte=m_nBitCount/8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//中间变量
	int x, y, t;

	//Prewitt算子
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x方向梯度
				x= *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+i*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k);

				//y方向梯度
				y= *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 + *(m_pImgData+(i-1)*lineByte+j*pixelByte+k)
				 + *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+j*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k);

				t=sqrt(x*x+y*y)+0.5;
				if(t>255)
					t=255;
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=t;
			}
		}
	}

}


/***********************************************************************
* 函数名称：
* Laplacian()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Laplacian边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
***********************************************************************/
void ImgSegment::Laplacian()
{
	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//循环变量，图像的坐标
	int i,j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte=m_nBitCount/8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//中间变量
	int t;

	//Laplacian算子
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				t= 4 * *(m_pImgData+i*lineByte+j*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+j*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+j*pixelByte+k)
				 - *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+i*lineByte+(j+1)*pixelByte+k);

				t=abs(t)+0.5;
				if(t>255)
					t=255;
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=t;
			}
		}
	}
}

/***********************************************************************
* 函数名称： EdgeTrace()
* 函数参数： 无
* 返回值：   无
*说明：给定一个二值图像，把黑色区域看作目标，白色看作背景，跟踪目标的边界
***********************************************************************/
void ImgSegment::EdgeTrace()
{
	//只处理灰度图像
	if(m_nBitCount!=8)
		return;

	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			           sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//将输出图像初始化置255,用0代表轮廓像素标记
	memset(m_pImgDataOut,255,lineByte*m_imgHeight);

	//顺时针定义中心像素点的8邻域坐标，第一列为x方向的偏移,第二列为y方向的偏移
	int direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};

	//边界起始点,待处理的当前点,当前点的邻域点
	CPoint startP, currentP, neighborP;

	//是否当前点与起始边界点重合的标志变量
	int findStartPoint;

	//搜索边界起始点
	findStartPoint=0;

	//循环变量，图像坐标
	int i,j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			//找到起始边界点
			if(*(m_pImgData+i*lineByte+j)==0){
				startP.x=j;
				startP.y=i;
				*(m_pImgDataOut+i*lineByte+j)=0;
				findStartPoint=1;
				break;
			}
		}

		//已经找到起始边界点
		if(findStartPoint)
			break;
	}

	//边界跟踪
	//从初始点开始跟踪
	currentP.x=startP.x;
	currentP.y=startP.y;

	//邻域点是否边界点标志变量
	int isContourP;

	//开始方向
	int startDirect=0;

	//0表示还没有返回最初的边界起始点
	findStartPoint=0;
	while(findStartPoint==0){
		isContourP=false;
		while(isContourP==false){
			neighborP.x=currentP.x+direction[startDirect][0];
			neighborP.y=currentP.y+direction[startDirect][1];

			//搜索到邻域点
			if(*(m_pImgData+neighborP.y*lineByte+neighborP.x)==0){
				isContourP=true;
				currentP.x=neighborP.x;
				currentP.y=neighborP.y;

				if(currentP.x==startP.x&&currentP.y==startP.y)
					findStartPoint=true;//回到边界起始点了

				*(m_pImgDataOut+currentP.y*lineByte+currentP.x)=0;

				//扫描方向逆时针旋转90度
				startDirect-=2;
				if(startDirect<0)
					startDirect+=8;
			}
			else{
				//扫描方向顺时针旋转45度
				startDirect++;
				if(startDirect==8)
					startDirect=0;
			}

		}
	}
	
}


void ImgSegment::RegionGrow(CPoint SeedPos, int thresh)
{

	//只处理灰度图像
	if(m_nBitCount!=8)
		return;

	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			     sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//将输出图像初始化置255
	memset(m_pImgDataOut,255,lineByte*m_imgHeight);
    
	//以下是区域生长代码
	//二维数组direction代表中心像素点8邻域坐标与该点在x和y方向上的偏移,
	//其中第一列为x方向的偏移,第二列为y方向的偏移
	int direction[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

	//栈申请，此处假定进栈的像素最多为图像总像素数
	CPoint *stack=new CPoint[m_imgWidth*m_imgHeight];

	//栈顶指针
	int top;

	//当前正处理的点和弹出的点
	CPoint currentPoint, popPoint;

	//循环变量，遍历array数组的第一维下标
	int k;

	//标记变量
	int label;

	//临时变量
	int temp1, temp2;

	//记录种子像素的灰度值
	temp1=*(m_pImgData+SeedPos.y*lineByte+SeedPos.x);

	//将给定种子点置标记0,入栈
	*(m_pImgDataOut+SeedPos.y*lineByte+SeedPos.x)=0;
	top=0;
	stack[top].x=SeedPos.x;
	stack[top].y=SeedPos.y;

	//堆栈
	while(top>-1){
		//弹出栈顶元素,该元素已经生长过
		popPoint.x=stack[top].x;
		popPoint.y=stack[top].y;
		top--;

		//考察弹出像素周围是否有没有生长的像素
		for(k=0;k<8;k++){

			//待考察的邻域点
			currentPoint.x=popPoint.x+direction[k][0];
			currentPoint.y=popPoint.y+direction[k][1];

			//如果待考察的点不在图像内，则跳过
			if(currentPoint.x<0||currentPoint.x>m_imgWidth-1||
				currentPoint.y<0||currentPoint.y>m_imgHeight-1)
				continue;
			
			//该点标号
			label=*(m_pImgDataOut+currentPoint.y*lineByte+currentPoint.x);

			//弹出的点周围有尚没生长的点
			if(label==255){
				temp2=*(m_pImgData+currentPoint.y*lineByte+currentPoint.x);
				//如果当前被考察的像素灰度值与种子点灰度值之差小于给定的阈值,
				//则认为相似,将其进栈处理
				if(abs(temp1-temp2)<thresh){
					//给该点置生长标记0
					*(m_pImgDataOut+currentPoint.y*lineByte+currentPoint.x)=0;
					top++;
					stack[top].x=currentPoint.x;
					stack[top].y=currentPoint.y;
				}
			}
		}
		
	}
	
	//清除缓冲区
	delete []stack;

}


void ImgSegment::AdaptThreshSeg(unsigned char *pImgData)
{
	//只处理灰度图像
	if(m_nBitCount!=8)
		return;
	
	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	
	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;
	
	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	
	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	
	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	
	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	
	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];
	
	//遍历图象的横、纵坐标
	int i,j;
	// 局部阈值
	int nLocalThresh[2][2] ;
	// 子图象的平均值
	int nLocAvg ;
	// 对左上图像逐点扫描：
	nLocAvg = 0 ;
	for(j=0; j<m_imgHeight/2 ; j++ )
	{
		for(i=0; i<m_imgWidth/2 ; i++ )
		{
			nLocAvg += pImgData[j*lineByte + i];
		}
	}
	// 计算均值
	nLocAvg /= ( (m_imgHeight/2) * (m_imgWidth/2) ) ;

	// 设置阈值为子图象的平均值
	nLocalThresh[0][0] = nLocAvg ;

	// 对左上图像逐点扫描进行分割：
	for(j=0; j<m_imgHeight/2 ; j++ )
	{
		for(i=0; i<m_imgWidth/2 ; i++ )
		{
			if(pImgData[j*lineByte + i]<nLocalThresh[0][0])
				m_pImgDataOut[j*lineByte + i] = 255 ;
			else
			{
				m_pImgDataOut[j*lineByte + i] = 0 ;
			}
			
		}
	}

	// =============================================
	// 对左下图像逐点扫描：
	nLocAvg = 0 ;
	// y方向
	for(j=m_imgHeight/2; j<m_imgHeight ; j++ )
	{
		// x方向
		for(i=0; i<m_imgWidth/2 ; i++ )
		{
			nLocAvg += pImgData[j*lineByte + i];
		}
	}
	// 计算均值
	nLocAvg /= ( (m_imgHeight - m_imgHeight/2) * (m_imgWidth/2) ) ;

	// 设置阈值为子图象的平均值
	nLocalThresh[1][0] = nLocAvg ;

	// 对左下图像逐点扫描进行分割：
	for(j=m_imgHeight/2; j<m_imgHeight ; j++ )
	{
		for(i=0; i<m_imgWidth/2 ; i++ )
		{
			if(pImgData[j*lineByte + i]<nLocalThresh[1][0])
			    m_pImgDataOut[j*lineByte + i] = 255 ;
			else
			{
				m_pImgDataOut[j*lineByte + i] = 0 ;
			}
			
		}
	}

	// =============================================
	// 对右上图像逐点扫描：
	nLocAvg = 0 ;
	for(j=0; j<m_imgHeight/2 ; j++ )
	{
		for(i=m_imgWidth/2; i<m_imgWidth ; i++ )
		{
			nLocAvg += pImgData[j*lineByte + i];
		}
	}
	// 计算均值
	nLocAvg /= ( (m_imgHeight/2) * (m_imgWidth - m_imgWidth/2) ) ;
	
	// 设置阈值为子图象的平均值
	nLocalThresh[0][1] = nLocAvg ;

	// 对右上图像逐点扫描进行分割：
	for(j=0; j<m_imgHeight/2 ; j++ )
	{
		for(i=m_imgWidth/2; i<m_imgWidth ; i++ )
		{
			if(pImgData[j*lineByte + i]<nLocalThresh[0][1])
				m_pImgDataOut[j*lineByte + i] = 255 ;
			else
			{
				m_pImgDataOut[j*lineByte + i] = 0 ;
			}
			
		}
	}

	// =============================================
	// 对右下图像逐点扫描：
	nLocAvg = 0 ;
	for(j=m_imgHeight/2; j<m_imgHeight ; j++ )
	{
		for(i=m_imgWidth/2; i<m_imgWidth ; i++ )
		{
			nLocAvg +=pImgData[j*lineByte + i];
		}
	}
	// 计算均值
	nLocAvg /= ( (m_imgHeight - m_imgHeight/2) * (m_imgWidth - m_imgWidth/2) ) ;

	// 设置阈值为子图象的平均值
	nLocalThresh[1][1] = nLocAvg ;

	// 对右下图像逐点扫描进行分割：
	for(j=m_imgHeight/2; j<m_imgHeight ; j++ )
	{
		for(i=m_imgWidth/2; i<m_imgWidth ; i++ )
		{
			if(pImgData[j*lineByte + i]<nLocalThresh[1][1])
				m_pImgDataOut[j*lineByte + i] = 255 ;
			else
			{
				m_pImgDataOut[j*lineByte + i] = 0 ;
			}
		}
	}
/*	*/
	// 为了显示方便显示，逻辑1用黑色显示，逻辑0用白色显示
	for(j=0; j<m_imgHeight ; j++ )
	{
		for(i=0; i<m_imgWidth ; i++ )
		{
			m_pImgDataOut[j*lineByte + i] = 255 - m_pImgDataOut[j*lineByte + i] ;
		}
	}
}

