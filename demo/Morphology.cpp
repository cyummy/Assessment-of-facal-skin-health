// Morphology.cpp: implementation of the Morphology class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "Morphology.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Morphology::Morphology()
{
	m_pImgDataOut=NULL;//输出图像位图数据指针为空
	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
    m_nBitCountOut=0;//输出图像每像素位数为0	
	m_imgWidthOut=0;//输出图像的宽为0
	m_imgHeightOut=0;//输出图像的高为0
	m_TempBuf=NULL;//结构元素指针为空
	TempH=3;   //结构元素高
	TempW=3;   //结构元素宽
    int i;
	for (i=0;i<TempH*TempW;i++) {
		m_InitTemp[i]=0;
	}
}

Morphology::Morphology(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					   unsigned char *pImgData):
            ImageDib(size, nBitCount, lpColorTable, pImgData)
{
    m_nBitCountOut=m_nBitCount;
	m_lpColorTableOut=NULL;	//输出图像颜色表处理
	int colorTableLength=ComputeColorTabalLength(m_nBitCountOut);
	if(colorTableLength!=0){
      m_lpColorTableOut=new RGBQUAD[colorTableLength];
	  memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*colorTableLength);
	}
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];//申请输出图像所需要的缓冲区
	m_TempBuf=NULL;//结构元素指针为空
	TempH=3;
	TempW=3;
	int i;
	for (i=0;i<TempH*TempW;i++) {
	    m_InitTemp[i]=0;
	}
}

Morphology::~Morphology()
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
	
	//释放结构元素缓冲区
	if(m_TempBuf){
		delete []m_TempBuf;
		m_TempBuf=NULL;
	}
}

CSize Morphology::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* 函数名称： ImgErosion()
* 函数参数： unsigned char *imgBufIn   -待腐蚀的图像
*            unsigned char *imgBufOut  -腐蚀后的结果
*            int imgWidth   -图像宽
*            int imgHeight   -图像高
*            int *TempBuf   -结构元素缓冲区指针
*            int TempW   -结构元素宽
*            int TempH   -结构元素高
*返回值：   无
*说明：二值腐蚀基本运算,背景为黑色,目标为白色,开/闭/二值形态学边界等操作
*      都要调用这个函数
***********************************************************************/
void Morphology::ImgErosion(unsigned char *imgBufIn,unsigned char *imgBufOut,int imgWidth,
							  int imgHeight,int *TempBuf, int TempW, int TempH) 
				
{
	int lineByte=(imgWidth+3)/4*4;	//每行图像数据的字节数为4的倍数
	memcpy(imgBufOut,imgBufIn,lineByte*imgHeight);	//把输入图像缓冲区的拷贝到输出图像缓冲区
	int i,j,k,l;
	int flag;	//标志变量
	//针对图像中每一个像素位置，判断是否结构元素能填入目标内部
	for(i=TempH/2;i<imgHeight-TempH/2;i++)
	{
		for(j=TempW/2;j<imgWidth-TempW/2;j++)
		{			
			//判断结构元素是否可以在当前点填入目标内部,1为可以,0为不可以
			flag=1;
			for(k=-TempH/2;k<=TempH/2;k++)
			{
				for(l=-TempW/2;l<=TempW/2;l++)
				{
					//如果当前结构元素位置为1,判断与对应图像上的像素点是否为非0
					if(TempBuf[(k+TempH/2)*TempW+l+TempW/2])
					{
						//如果图像当前象素为0,则没有击中该点,不是腐蚀的输出
						if(!*(imgBufIn+(i+k)*lineByte+j+l))
							flag=0;	
					}
				}
			}


			if(flag)
				*(imgBufOut+i*lineByte+j)=255;
			else
				*(imgBufOut+i*lineByte+j)=0;
		}
	}
}

/***********************************************************************
* 函数名称： ImgDilation()
* 函数参数： unsigned char *imgBufIn   -待膨胀的图像
*            unsigned char *imgBufOut  -膨胀后的结果
*            int imgWidth   -图像宽
*            int imgHeight   -图像高
*            int *maskBuf   -结构元素缓冲区指针
*            int maskW   -结构元素宽
*            int maskH   -结构元素高
*返回值：   无
*说明：二值膨胀基本运算,背景为黑色,目标为白色,开/闭/二值形态学边界等操作
*      都要调用这个函数
***********************************************************************/
void Morphology::ImgDilation(unsigned char *imgBufIn,unsigned char *imgBufOut,int imgWidth,int imgHeight,
					 int *TempBuf, int TempW, int TempH)
{
	int lineByte=(imgWidth+3)/4*4;//每行图像数据的字节数为4的倍数
	int i,j,k,l;
	//膨胀是对图像中目标补集的腐蚀，因此先求输入图像数据的补集
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			*(imgBufIn+i*lineByte+j)=255-*(imgBufIn+i*lineByte+j);
		}
	}
	//膨胀是结构元素的对称集对补集的腐蚀,此处tempMask存放m_mask的对称集
	int *tempMask=new int[TempW*TempH];
	for(k=0;k<TempH;k++){
		for(l=0;l<TempW;l++){
			tempMask[k*TempW+l]=TempBuf[(TempH-1-k)*TempW+TempW-1-l];
		}
	}
	int flag;	//标志变量
	//针对图像中每一个像素位置，判断是否结构元素能填入目标内部
	for(i=TempH/2;i<imgHeight-TempH/2;i++)
	{
		for(j=TempW/2;j<imgWidth-TempW/2;j++)
		{			
			//判断结构元素是否可以在当前点填入目标内部,1为可以,0为不可以
			flag=1;
			for(k=-TempH/2;k<=TempH/2;k++)
			{
				for(l=-TempW/2;l<=TempW/2;l++)
				{
					//如果当前结构元素位置为1,判断与对应图像上的像素点是否为非0
					if(tempMask[(k+TempH/2)*TempW+l+TempW/2])
					{
						if(!*(imgBufIn+(i+k)*lineByte+j+l))
							flag=0;	
					}
				}
			}


			if(flag)
				*(imgBufOut+i*lineByte+j)=255;
			else
				*(imgBufOut+i*lineByte+j)=0;
		}
	}

	//用结构元素对称集对目标补集腐蚀后,还要对结果再求一次补集,才是膨胀结果输出
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			*(imgBufOut+i*lineByte+j)=255-*(imgBufOut+i*lineByte+j);
		}
	}

	//恢复原始数据
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			*(imgBufIn+i*lineByte+j)=255-*(imgBufIn+i*lineByte+j);
		}
	}
	
	delete []tempMask;
}



/***********************************************************************
* 函数名称：Open()
* 函数参数：无
* 返回值：  无
* 说明：二值开运算，m_pImgData为输入，m_pImgDataOut为输出
***********************************************************************/
void Morphology::Open()
{
	//如果没有结构元素输入，则返回
	if(m_TempBuf==NULL)
    	return;
	//灰度图像每行像素所占字节数
	int lineByte=(m_imgWidth+3)/4*4;
	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];
	ImgErosion(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_TempBuf, TempW, TempH);	//先腐蚀
	ImgDilation(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_TempBuf, TempW, TempH);	//后膨胀
	delete []buf;	//释放缓冲区

}


/***********************************************************************
* 函数名称：Close()
* 函数参数：无
* 返回值：  无
* 说明：二值闭运算，m_pImgData为输入，m_pImgDataOut为输出
***********************************************************************/
void Morphology::Close()
{
	//如果没有结构元素输入，则返回
	if(m_TempBuf==NULL)
		return;
	//灰度图像每行像素所占字节数
	int lineByte=(m_imgWidth+3)/4*4;
	//中间结果缓冲区申请,用来存放腐蚀后的中间结果
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];
	//先膨胀
	ImgDilation(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_TempBuf, TempW, TempH);

	//后腐蚀
	ImgErosion(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_TempBuf, TempW, TempH);

	//释放缓冲区
	delete []buf;
}


/***********************************************************************
* 函数名称：DefineElementPair()
* 函数参数：无
* 返回值：  无
* 说明：为击中击不中变换的结构元素对m_hitMissMask赋值，共8对
***********************************************************************/
void Morphology::DefineElementPair()
{
	//定义8个方向的击中击不中变换结构元素对
	int i, k, l;
	for(i=0;i<8;i++){
		for(k=0;k<3;k++){
			for(l=0;l<3;l++){
				m_hitMissTemp[i].hitElement[k*3+l]=0;
				m_hitMissTemp[i].missElement[k*3+l]=0;
			}
		}
	}
	
	m_hitMissTemp[0].hitElement[0]=1;
	m_hitMissTemp[0].hitElement[1]=1;
	m_hitMissTemp[0].hitElement[2]=1;
	m_hitMissTemp[0].hitElement[4]=1;
	m_hitMissTemp[0].missElement[6]=1;
	m_hitMissTemp[0].missElement[7]=1;
	m_hitMissTemp[0].missElement[8]=1;
	
	m_hitMissTemp[1].hitElement[6]=1;
	m_hitMissTemp[1].hitElement[7]=1;
	m_hitMissTemp[1].hitElement[8]=1;
	m_hitMissTemp[1].hitElement[4]=1;
	m_hitMissTemp[1].missElement[0]=1;
	m_hitMissTemp[1].missElement[1]=1;
	m_hitMissTemp[1].missElement[2]=1;
	
	m_hitMissTemp[2].hitElement[2]=1;
	m_hitMissTemp[2].hitElement[5]=1;
	m_hitMissTemp[2].hitElement[8]=1;
	m_hitMissTemp[2].hitElement[4]=1;
	m_hitMissTemp[2].missElement[0]=1;
	m_hitMissTemp[2].missElement[3]=1;
	m_hitMissTemp[2].missElement[6]=1;
	
	m_hitMissTemp[3].hitElement[0]=1;
	m_hitMissTemp[3].hitElement[3]=1;
	m_hitMissTemp[3].hitElement[6]=1;
	m_hitMissTemp[3].hitElement[4]=1;
	m_hitMissTemp[3].missElement[2]=1;
	m_hitMissTemp[3].missElement[5]=1;
	m_hitMissTemp[3].missElement[8]=1;
	
	
	m_hitMissTemp[4].hitElement[0]=1;
	m_hitMissTemp[4].hitElement[1]=1;
	m_hitMissTemp[4].hitElement[3]=1;
	m_hitMissTemp[4].hitElement[4]=1;
	m_hitMissTemp[4].missElement[5]=1;
	m_hitMissTemp[4].missElement[7]=1;
	m_hitMissTemp[4].missElement[8]=1;
	
	m_hitMissTemp[5].hitElement[5]=1;
	m_hitMissTemp[5].hitElement[7]=1;
	m_hitMissTemp[5].hitElement[8]=1;
	m_hitMissTemp[5].hitElement[4]=1;
	m_hitMissTemp[5].missElement[0]=1;
	m_hitMissTemp[5].missElement[1]=1;
	m_hitMissTemp[5].missElement[3]=1;
	
	
	m_hitMissTemp[6].hitElement[1]=1;
	m_hitMissTemp[6].hitElement[2]=1;
	m_hitMissTemp[6].hitElement[5]=1;
	m_hitMissTemp[6].hitElement[4]=1;
	m_hitMissTemp[6].missElement[3]=1;
	m_hitMissTemp[6].missElement[6]=1;
	m_hitMissTemp[6].missElement[7]=1;
	
	m_hitMissTemp[7].hitElement[3]=1;
	m_hitMissTemp[7].hitElement[6]=1;
	m_hitMissTemp[7].hitElement[7]=1;
	m_hitMissTemp[7].hitElement[4]=1;
	m_hitMissTemp[7].missElement[1]=1;
	m_hitMissTemp[7].missElement[2]=1;
	m_hitMissTemp[7].missElement[5]=1;
}


/***********************************************************************
* 函数名称：
* HitAndMiss()
*
*函数参数：
*  unsigned char *imgBufIn   -待变换的输入图像
*  unsigned char *imgBufOut   -输出图像
*  int imgWidth   -图像宽
*  int imgHeight   -图像高
*  struct ElementPair hitMissMask  -击中击不中结构元素对
*
*返回值：
*   无
*
*说明：击中击不中变换,0代表背景,255代表目标
***********************************************************************/
void Morphology::HitAndMiss(unsigned char *imgBufIn, unsigned char *imgBufOut,
							int imgWidth,int imgHeight, ElementPair hitMissMask)						 
{
	//循环变量
	int i, j;
	
	//标志变量，1表示结构元素对有效，0表示无效
	int validateFlag=1;
	
	//检查结构元素对是否交集为空,交集若不为空则为无效结构元素对,算法将退出
	for(i=0; i<3;i++){
		for(j=0;j<3;j++){
			if(hitMissMask.hitElement[i*3+j]&&hitMissMask.missElement[i*3+j]){
				validateFlag=0;
				break;
			}
		}
	}
	
	//非法结构元素对,返回
	if(validateFlag==0)
		return;
	
	//图像每行像素所占字节数
	int lineByte=(imgWidth+3)/4*4;
	
	//输出图像置0
	memset(imgBufOut, 0, lineByte*imgHeight);
	
	//循环变量
	int k,l;
	
	//击中标志变量和击不中标志变量
	int hitFlag, missFlag;
	
	for(i=1; i<imgHeight-1; i++){
		for(j=1;j<imgWidth-1; j++){
			hitFlag=1;
			missFlag=1;
			for(k=-1;k<=1;k++){
				for(l=-1;l<=1;l++){
					//如果击中结构元素当前位置为1
					if(hitMissMask.hitElement[(k+1)*3+l+1]==1){
						//判断图像对应点是否为0,如果是,则没有击中图像当前点
						if(!*(imgBufIn+(i+k)*lineByte+j+l))
							hitFlag=0;
					}
					
					//如果击不中结构元素当前位置为1
					if(hitMissMask.missElement[(k+1)*3+l+1]==1){
						//判断图像对应点是否为0,如果是,则没有击中图像当前点
						if(*(imgBufIn+(i+k)*lineByte+j+l))
							missFlag=0;
					}
				}
			}
			
			//输出点置255
			if(hitFlag&&missFlag)
				*(imgBufOut+i*lineByte+j)=255;
		}
	}
}

/***********************************************************************
* 函数名称： ImgThinning()
* 函数参数： 无
* 返回值：   无
*说明：用击中击不中变换对输入图像进行细化
***********************************************************************/
void Morphology::ImgThinning()
{
	//定义8个方向的结构元素对
	DefineElementPair();
	
	
	//灰度图像每行像素所占字节数
	int lineByte=(m_imgWidth+3)/4*4;
	
	//申请缓冲区，存放中间结果
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];
	memcpy(buf, m_pImgData, lineByte*m_imgHeight);
	
	//循环变量
	int i, k, l;
	
	//thinFlag用来记录当前一个循环周期内是否有细化掉的点,如果为0表明没
	//有可以细化的点存在,细化终止	
	int thinFlag=1;
	while(thinFlag){
		thinFlag=0;
		//用8个方向的结构元素对都对图像作一次击中击不中变换,并将输出结果
		//从原图中去除
		for(i=0;i<8;i++){
			HitAndMiss(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,m_hitMissTemp[i]);
			//将击中击不中变换的输出结果从原数据中去除
			for(k=0;k<m_imgHeight;k++){
				for(l=0;l<lineByte;l++){
					if(*(m_pImgDataOut+k*lineByte+l)==255){
						*(buf+k*lineByte+l)=0;
						thinFlag=1;
					}
				}
			}
		}
	}
	
	//把最后细化的结果拷贝到m_pImgDataOut
	memcpy(m_pImgDataOut, buf,  lineByte*m_imgHeight);
	
	//释放缓冲区
	delete []buf;
}


