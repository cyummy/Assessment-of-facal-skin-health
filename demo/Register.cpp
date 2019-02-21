#include "stdafx.h"
#include "Register.h"
#include "math.h"

/***********************************************************************
* 函数名称：
* Register()
*
*说明：无参数的构造函数，对成员变量进行初始化
***********************************************************************/
Register::Register()
{
	//输入图像1相关的成员变量赋初值
	m_nBitCount1=0;
	m_pImgData1=NULL;
	m_lpColorTable1=NULL;
	m_nColorTableLength1=0;
	m_imgWidth1=0;
	m_imgHeight1=0;
	
	//输入图像2相关的成员变量赋初值
	m_nBitCount2=0;
	m_pImgData2=NULL;
	m_lpColorTable2=NULL;
	m_nColorTableLength2=0;
	m_imgWidth2=0;
	m_imgHeight2=0;
	
	//输出图像相关的成员变量赋初值
	m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_lpColorTableOut=NULL;
	m_nColorTableLengthOut=0;
	m_imgWidthOut=0;
	m_imgHeightOut=0;
	m_space=10;
}

/***********************************************************************
* 函数名称：
*   ~Register()
*
*说明：析构函数，释放资源
***********************************************************************/
Register::~Register()
{
	//释放图像1的缓冲区
	if(m_pImgData1!=NULL){
		delete []m_pImgData1;
		m_pImgData1=NULL;
	}
	if(m_lpColorTable1==NULL){
		delete []m_lpColorTable1;
		m_lpColorTable1=NULL;
	}
	
	//释放图像2的缓冲区
	if(m_pImgData2!=NULL){
		delete []m_pImgData2;
		m_pImgData2=NULL;
	}
	if(m_lpColorTable2==NULL){
		delete []m_lpColorTable2;
		m_lpColorTable2=NULL;
	}
	
	//释放输出图像的缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut==NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}

/***********************************************************************
* 函数名称：
* GetDimensions()
*
*函数参数：
*  无
*
*返回值：
*   图像的尺寸，用CSize类型表达
*
*说明：返回输出图像的宽和高
***********************************************************************/
CSize Register::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* 函数名称：
* InputImg1()
*
*函数参数：
*  CSize size -输入图像大小（宽、高）
*  int nBitCount  -每像素位数
*  LPRGBQUAD lpColorTable  -颜色表指针
*  unsigned char *pImgData  -位图数据指针
*
*返回值：
*   无
*
*说明：输入图像数据，作为参加配准的第一个图像，由数据成员m_pImgData1指向。
***********************************************************************/
void Register::InputImg1(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
						 unsigned char *pImgData)
{
	//清除缓冲区
	if(m_pImgData1!=NULL){
		delete []m_pImgData1;
		m_pImgData1=NULL;
	}
	if(m_lpColorTable1==NULL){
		delete []m_lpColorTable1;
		m_lpColorTable1=NULL;
	}
	
	//为图像1相关的成员变量赋值
	m_nBitCount1=nBitCount;
	m_nColorTableLength1=ComputeColorTabalLength(nBitCount);
	
	//计算输入图像的大小，并为图像1分配合适大小的空间存储位图数据
	m_imgWidth1=size.cx;
	m_imgHeight1=size.cy;
	int lineByte=(m_imgWidth1*nBitCount/8+3)/4*4;
	int imgBufSize=m_imgHeight1*lineByte;
	m_pImgData1=new BYTE[imgBufSize];
	memcpy(m_pImgData1, pImgData, imgBufSize);
	
	//如果有颜色表，分配颜色表所需要的空间，并拷贝颜色表
	if(m_nColorTableLength1!=0){
		m_lpColorTable1= new RGBQUAD[m_nColorTableLength1];
		memcpy(m_lpColorTable1,lpColorTable,
			   sizeof(RGBQUAD) * m_nColorTableLength1);
	}
}


/***********************************************************************
* 函数名称：
* InputImg2()
*
*函数参数：
*  CSize size -输入图像大小（宽、高）
*  int nBitCount  -每像素位数
*  LPRGBQUAD lpColorTable  -颜色表指针
*  unsigned char *pImgData  -位图数据指针
*
*返回值：
*   无
*
*说明：输入图像数据，作为参加配准的第二个图像，由数据成员m_pImgData2指向。
***********************************************************************/
void Register::InputImg2(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData)
{
	//清除缓冲区
	if(m_pImgData2!=NULL){
		delete []m_pImgData2;
		m_pImgData2=NULL;
	}
	if(m_lpColorTable2==NULL){
		delete []m_lpColorTable2;
		m_lpColorTable2=NULL;
	}
	
	//为图像2相关的成员变量赋值
	m_nBitCount2=nBitCount;
	m_nColorTableLength2=ComputeColorTabalLength(nBitCount);
	
	//计算输入图像的大小，并为图像2分配合适大小的空间存储位图数据
	m_imgWidth2=size.cx;
	m_imgHeight2=size.cy;
	int lineByte=(m_imgWidth2*nBitCount/8+3)/4*4;
	int imgBufSize=m_imgHeight2*lineByte;
	m_pImgData2=new BYTE[imgBufSize];
	memcpy(m_pImgData2, pImgData, imgBufSize);
	
	//如果有颜色表，分配颜色表所需要的空间，并拷贝颜色表
	if(m_nColorTableLength2!=0){
		m_lpColorTable2= new RGBQUAD[m_nColorTableLength2];
		memcpy(m_lpColorTable2,lpColorTable,sizeof(RGBQUAD) * m_nColorTableLength2);
	}
}


/***********************************************************************
* 函数名称：
* RegisterBasedSpacial()
*
*函数参数：
*  unsigned char *imgBuf  -参考图(搜索图)位图数据
*  int width  -参考图宽,像素为单位
, int height  -参考图高,像素为单位
*  unsigned char * blockBuf  -基准图(模板)位图数据
*  int blockW  -基准图宽,像素为单位
*  int blockH  -基准图高,像素为单位
*
*返回值：
*   基准图左下角在参考图中的位置坐标,用CPoint类型表达
*
*说明：给定参考图和基准图数据，进行模板匹配，并返回基准图左下角在参考图中
*      的位置坐标,该函数只对灰度图像有效
***********************************************************************/
CPoint Register::RegisterBasedSpacial(unsigned char *imgBuf, int width, int height, 
								unsigned char * blockBuf, int blockW, int blockH)
{
	//图像每行像素的字节数
	int lineByte=(width+3)/4*4;
	
	//标准图像每行字节数
	int blockLineByte=(blockW+3)/4*4;
	
	//循环变量
	int i, j, i0, j0;
	
	//中间变量，每搜索一个位置记录该位置上的相关性测度，本函数用对应像素差的
	//绝对值并求和作为相关性测度，所以sum值最小的位置为最佳匹配位置
	int sum;
	
	//用min记录具有最小sum的值, 及具有最小sum的位置
	int min, minX, minY;
	
	//中间变量
	int t1, t2;
	
    //为了节省时间，此处行方向和纵方向搜索步长为3，读者可根据需要自己调整步长
	for(i=0;i<height-blockH;i+=3){
		for(j=0;j<width-blockW; j+=3){
			sum=0;
			
			//搜索匹配窗口内的像素，(i0,j0)的步长也可以视情况和需要来定
			for(i0=0;i0<blockH;i0++){
				for(j0=0;j0<blockW;j0++){
					//对应像素相减，并计算差的绝对值之和
					t1=*(imgBuf+(i+i0)*lineByte+j+j0);
					t2=*(blockBuf+i0*blockLineByte+j0);
					sum += abs(t1-t2);				
				}
			}
			
			//第一个搜索区域，此时也是最小sum值的位置
			if(i==0&&j==0){
				min=sum;
				minX=0;
				minY=0;
			}
			else{//已经不是第一个搜索位置了
				if(min>sum){//出现一个新的最小位置，记录此时的最小值及其位置坐标
					min=sum;
					minX=j;
					minY=i;
				}
			}
			
		}
	}
	
	//返回匹配位置
	CPoint pos(minX,minY);
	return pos;
}


/***********************************************************************
* 函数名称：
* relativityRegistering()
*
*函数参数：
*  无
*
*返回值：
*   1为成功,0为失败
*
*说明：把m_pImgData1所指向数据作为参考图，m_pImgData2所指向数据作为基准图，
*      进行模板匹配，为m_pImgDataOut分配空间，将结果输出至m_pImgDataOut中。
***********************************************************************/
BOOL Register::relativityRegistering()
{
	//基准图与参考图格式不统一
	if(m_nBitCount1!=m_nBitCount2){	
		return FALSE;//返回
	}

	//匹配位置变量,调用模板匹配函数,获得基准图左下角在参考图中的位置
	CPoint macthPos(0,0);
	macthPos=RegisterBasedSpacial(m_pImgData1, m_imgWidth1, m_imgHeight1, 
		m_pImgData2, m_imgWidth2, m_imgHeight2);
	
	//将匹配结果在输出图像中显示
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	
	//输出图像与输入图像为同一类型,此处为灰度类型
	m_nBitCountOut=m_nBitCount1;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	if(m_nColorTableLengthOut!=0){
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable1,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	
	//输出图像的宽、高及每行像素字节数，与参考图大小相同
	m_imgWidthOut=m_imgWidth1;
	m_imgHeightOut=m_imgHeight1;
	int lineByte=(m_imgWidth1*m_nBitCount1/8+3)/4*4;

	//为输出图像分配空间，并将参考图拷贝至m_pImgDataOut
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeightOut];
	memcpy( m_pImgDataOut,m_pImgData1, lineByte*m_imgHeightOut);
	
	//将基准图的位置用黑色框标记出来，以显示匹配结果的正确性
	for(int i=0;i<m_imgWidth2;i++){
		*(m_pImgDataOut+(macthPos.y+0)*lineByte+macthPos.x+i)=0;
		*(m_pImgDataOut+(macthPos.y+m_imgHeight2-1)*lineByte+macthPos.x+i)=0;
	}
	for(i=0;i<m_imgHeight2;i++){
		*(m_pImgDataOut+(macthPos.y+i)*lineByte+macthPos.x+0)=0;
		*(m_pImgDataOut+(macthPos.y+i)*lineByte+macthPos.x+m_imgWidth2)=0;
	}
	return TRUE;
}


/***********************************************************************
* 函数名称：
* conv2()
*
*函数参数：
*  float *Src  -要处理的数据
*  int width  -参考图宽,像素为单位
, int height  -参考图高,像素为单位
*  float *mark  -(模板)数据
*  int markWidth  -模板宽,像素为单位
*  int markHeight  -模板高,像素为单位
*返回值：
*   无
*
*说明：用mark模板对数组Src进行卷积操作，结果存在Src中
***********************************************************************/
void Register::conv2(float *Src, int width, int height, float *mark,
					 int markWidth , int markHeight )
{
	int i,j,m,n;
	//temp为临时数组，避免在原图上直接操作
	float *temp = new float [width*height];
	memset( temp, 0 , sizeof(float)*width*height);
	float sum;
	int lineByte=(width+3)/4*4;
	
	/////卷积，边缘处不操作
	for (i = (int) markWidth/2 ; i < width - markWidth/2 ;i++)
		for (j = (int) markHeight/2 ; j < height - markHeight/2 ; j++)
		{
			sum=0;
			for (m = -markWidth/2 ; m <= markWidth/2 ; m++)
				for (n = -markHeight/2 ; n <= markHeight/2 ; n++)
				{
					sum += Src[(j+n) * lineByte+ i+m] 
						 * mark[(n+markHeight/2) * markWidth + m+markWidth/2];
				}
				temp[j * lineByte + i]=sum;
		}
		memcpy( Src, temp , sizeof(float)*lineByte*height);
		delete[] temp;
}
/***********************************************************************
* 函数名称：
* ArraydotMultiArray()
*
*函数参数：
*  float * src1 ， 源矩阵1
*  float * src2    源矩阵2
, 
* float * dest  结果矩阵
* int width   矩阵的宽
* int height  矩阵的高
*返回值：
*   无
*
*说明：矩阵点乘函数即，dest[i]=src1[i]*src2[i]
***********************************************************************/
void Register::ArraydotMultiArray( float * dest , float * src1 , 
								  float * src2 , int width , int height)
{
	int lineByte=(width+3)/4*4;
	
	for (int i = 0 ; i < width ; i++ )
		for ( int j = 0 ; j < height ; j++) 
			dest[j * lineByte + i] = src1[j * lineByte + i] * src2[j* lineByte + i];
}

/***********************************************************************
* 函数名称：
* LocalMaximum()
*
*函数参数：
*  float * src  ，存放特征值的矩阵
* int width   矩阵的宽
* int height  矩阵的高
* int posx , 要判断的点的列坐标
* int posy , 要判断的点的行坐标
* int neiboursize , 邻域的大小
*返回值：
*  bool型，若是局部最大，返回真，否则返回假
*
*说明：判断点(posx,posy)的特征值，是否为局部极大值
***********************************************************************/
bool Register::LocalMaximum(float * src , int width , int height ,
							int posx , int posy , int neiboursize)
{
	int i,j;
	int lineByte=(width+3)/4*4;
	//判断是否为局部最大
	for ( i = -neiboursize/2 ; i <= neiboursize/2 ; i++)
		for ( j = -neiboursize/2 ; j <= neiboursize/2 ; j++)
		{
			if ( src[(posy + j) * lineByte + posx+i] > src[posy * lineByte + posx])
				return 0;
		}
		return 1;
}

/***********************************************************************
* 函数名称：
* SpaceSuppress()
*
*函数参数：
* ptArray * pCornerpos ,存放特征点的矩阵
*返回值：
*  无
*
*说明：将pCornerpos中相距小于距离阈值的点对剔除
***********************************************************************/
void Register::SpaceSuppress(ptArray * pCornerpos)
{
	int i,j;
	int length;
	//length特征点的个数
	length = pCornerpos->GetSize();
	CPoint pt1,pt2;
	//distancetable 存放两两特征点距离的数组
	double * distacetable = new double [length * length];
	//表识点对时小于距离阈值
	bool * marktable = new bool [length * length];
	memset( distacetable , 0 , sizeof(double) * length * length);
	memset( marktable , 0 , sizeof(bool) * length * length);
	
	for ( i = 0 ; i < length-1 ; i++) 
	{
		pt1 = pCornerpos->GetAt(i);
		for ( j = i+1 ; j < length ; j++)
		{
			pt2 = pCornerpos->GetAt(j);
			//求取距离
			distacetable[j * length + i] = sqrt(pow(pt1.x - pt2.x , 2)
				+ pow(pt1.y - pt2.y , 2));
			if ( distacetable[j* length + i] < m_space)
			{
				//表识距离小于阈值
				marktable[j * length + i] = 1;
			}
		}
	}
	//remove_num表示去除的点数
	int temp, remove_num=0;
	for ( i= 0 ; i < length-1 ; i++)
	{
		temp = 0;
		//判断是否由与其距离小于阈值的点
		for ( j = 0 ; j < length-1 ; j++)
		{
			temp += marktable[i * length + j];
		}
		if ( temp != 0)
		{ 
			//从特征点数组中剔除该点
			pCornerpos->RemoveAt( i - remove_num );
			remove_num++;
		}			
	}
	
	delete []distacetable;
	delete []marktable;
}
/***********************************************************************
* 函数名称：
* HarisConnerDetect()
*
*函数参数：
* unsigned char *imgBufIn, 待提取角点的图像数组
* int width, 图像的宽
* int height, 图像的高
* ptArray * pCornerpos ,存放特征点的矩阵
*返回值：
*  无
*
*说明：提取图像imgBfuIn的角点，存放于矩阵pCornerpos中
***********************************************************************/
void Register::HarisConnerDetect(unsigned char *imgBufIn, int width, 
								 int height, ptArray * pCornerpos)
{
	int imWidth = width;
	int imHeight = height;
	//图像模糊卷积模版
	float blur[7] = {1, 6 , 15 , 20 , 15 , 6 , 1}; 
	//滤波模版
	float prefilt[3] = { 0.223755 , 0.552490 , 0.223755 };
	//对图像求导模版
	float derivfilt[3] = { -0.453014 , 0 , 0.453014 };
	int lineByte=(imWidth*m_nBitCount1/8+3)/4*4;
	
	int i,j;
	////模板归一化（取均值）
	float sum=0;
	for ( i = 0 ; i < 7 ; i++ )
		sum += blur[i];
	for ( i = 0 ; i < 7 ; i++ )
		blur[i] /= sum;
    
	//fx 图像的一阶x方向导数 ， fy 图像的一阶y方向导数
	float * fx = new float [lineByte * imHeight];
	float * fy = new float [lineByte * imHeight];
	//fx2每个像素值为fx对应像素值的平方
	float * fx2 = new float [lineByte * imHeight];
	//fy2每个像素值为fy对应像素值的平方
	float * fy2 = new float [lineByte * imHeight];
	//fxy=fx*fy
	float * fxy = new float [lineByte * imHeight];
	
    ///initiale
	for ( i =0 ; i < imWidth ; i++){
		for ( j = 0 ; j < imHeight ; j++){	
			fx[j * lineByte + i] = (float) imgBufIn[j * lineByte + i];
			fy[j * lineByte + i] = (float) imgBufIn[j * lineByte + i];
		}
	}
	//////计算fx，fy，fx2，fy2及fxy
	conv2( fx , imWidth , imHeight , prefilt , 1 , 3 );
	conv2( fx , imWidth , imHeight , derivfilt , 3 , 1 );
	conv2( fy , imWidth , imHeight , prefilt , 3 , 1 );
	conv2( fy , imWidth , imHeight , prefilt , 1 , 3 );
	ArraydotMultiArray( fx2 , fx , fx , imWidth , imHeight);
	conv2( fx2 , imWidth , imHeight , blur , 1 , 3 );
	conv2( fx2 , imWidth , imHeight , blur , 3 , 1 );
	ArraydotMultiArray( fy2 , fy , fy , imWidth , imHeight);
	conv2( fy2 , imWidth , imHeight , blur , 1 , 3 );
	conv2( fy2 , imWidth , imHeight , blur , 3 , 1 );
	ArraydotMultiArray( fxy , fx , fy , imWidth , imHeight);
	conv2( fxy , imWidth , imHeight , blur , 1 , 3 );
	conv2( fxy , imWidth , imHeight , blur , 3 , 1 );
	
	delete[] fx;
	delete[] fy;
		
	float * mean_array = new float [lineByte * imHeight];
	//detOfHessian为每个像素点处的特征矩阵
	float * detOfHessian = new float [lineByte * imHeight];
	float * norm_array = new float [lineByte * imHeight];
	//qualityOfcorner为每个像素的特征的品质因素，值越大越代表是角点
	float * qualityOfcorner = new float [lineByte * imHeight];
	
	float maxquality = 0;
	//thresh为阈值系数，它乘以最大品质因素即为阈值
	float thresh = 0.05;  // 0.005;  
	//距离边界小于border的像素不考虑
	int border = 20;    // do not consider the boundary pixels 
	float t1, t2;
	for ( i = 0; i < imWidth ; i++){
		for (j = 0 ; j < imHeight ; j++){
			mean_array[j * lineByte + i] 
				=( fx2[j * lineByte + i] + fy2[j * lineByte + i])/2;
			
			detOfHessian[j * lineByte + i] 
				= fx2[j * lineByte + i] * fy2[j * lineByte + i] 
				- pow(fxy[j * lineByte + i] , 2);
			t1=pow(mean_array[j*lineByte+i] , 2);
			t2=detOfHessian[j * lineByte + i];
			norm_array[j*lineByte+i] = sqrt(t1-t2);
			
			//边界点不予考虑
			if (i < border || i > imWidth - border || j < border 
				|| j > imHeight - border)
			{
				qualityOfcorner[j * lineByte + i] = 0;
			}
			else
			{
				t1=abs(mean_array[j*lineByte+i] - norm_array[j*lineByte+i]);
				t2=abs(mean_array[j*lineByte+i] + norm_array[j*lineByte+i]);
				if(t1<t2)
			    	qualityOfcorner[j * lineByte + i]= t1;
				else
					qualityOfcorner[j * lineByte + i]= t2;
			}
			///求最大的特征值
			if (qualityOfcorner[j * lineByte + i] > maxquality)
				maxquality = qualityOfcorner[j * lineByte + i];
		}
	}
	delete[] fx2;
	delete[] fy2;
	delete[] fxy;
	delete[] mean_array;
	delete[] detOfHessian;
	delete[] norm_array;
	
	//thq为阈值
	float thq = thresh * maxquality ;
	//supneibor为取最大的邻域范围
	int supneibor = 20;
	//求局部最大
	for ( i = supneibor/2 ; i < imWidth-supneibor/2 ; i++)
	{
		for ( j = supneibor/2 ; j < imHeight-supneibor/2 ; j++)
		{
			if ( qualityOfcorner[j * lineByte + i] > thq && 
				LocalMaximum(qualityOfcorner, imWidth, imHeight,i, j,supneibor)) 
			{
				//满足不在边界范围和局部最大两个条件就认为是角点
				pCornerpos->Add(CPoint(i,j));
			}
		}
	}
	delete[] qualityOfcorner;
	//进一步根据距离进行抑制，两角点相距不能太近
	SpaceSuppress(pCornerpos);
}

/***********************************************************************
* 函数名称：
* DrawCross()
*
*函数参数：
* unsigned char *imgBufIn, 待提取角点的图像数组
* int width, 图像的宽
* int height, 图像的高
* ptArray * pCornerpos ,存放特征点的矩阵
*返回值：
*  无
*
*说明：在图像的特征点的位置画十字，十字的宽为5，高为5
***********************************************************************/
void Register::DrawCross(unsigned char * imgBufIn , int imWidth , 
						 int imHeight , ptArray * pCornerpos )
{
	int NumOfCorner = pCornerpos->GetSize();
	CPoint pt;
	for ( int m = 0 ; m< NumOfCorner ; m++)
	{
		//获得特征点
		pt=pCornerpos->GetAt(m);
		
		int x,y;
		int i,j;
		x = pt.x;
		y = pt.y;
		int lineByte=(imWidth+3)/4*4;
		//画十字，即像素值设为255
		for ( j = -2 ; j <= 2 ; j++ ) 
		{
			imgBufIn[(y+j) * lineByte + x] = 255;
		}
		for ( i = -2 ; i <= 2 ; i++ ) 
		{
			imgBufIn[y * lineByte + x+i] = 255;
		}
	}
}
/***********************************************************************
* 函数名称：
* HarisConner()
*
*函数参数：
* 无
*返回值：
*  无
*
*说明：调用HarisConnerDetect()函数检测图像1的角点，并DrawCross画出十字，
*      结果输出到m_pImgDataOut中，对外的接口
***********************************************************************/
void Register::HarisConner()
{
	if (m_pImgData1==NULL)
	{
		return;
	}
	HarisConnerDetect(m_pImgData1,m_imgWidth1,m_imgHeight1,&m_cornerpos1);
	
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	//输出图像的位数与输入图像相同
	m_nBitCountOut=m_nBitCount1;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCount1);
	
    //输出图像的宽和高，与输入图像相同
	m_imgWidthOut=m_imgWidth1;
	m_imgHeightOut=m_imgHeight1;
	
	int lineByte=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	int imgBufSize=m_imgHeightOut*lineByte;
	m_pImgDataOut=new BYTE[imgBufSize];
	memcpy(m_pImgDataOut, m_pImgData1, imgBufSize);
	
	//如果有颜色表，分配颜色表所需要的空间，并拷贝颜色表
	if(m_nColorTableLengthOut!=0){
		m_lpColorTableOut= new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable1, 
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	//在输出图像上画十字
	DrawCross(m_pImgDataOut,m_imgWidthOut,m_imgHeightOut,&m_cornerpos1);	
}

/***********************************************************************
* 函数名称：
* RegisterBasedConner()
*
*函数参数：
* unsigned char *imgBuf1, 图像1
* unsigned char *imgBuf2, 图像2
* int width, 图像的宽
* int height, 图像的高
* ptArray *pCornerpos1, 图像1单独提取的角点 
* ptArray *pCornerpos2 , 图像2单独提取的角点
* ptArray *pCornerMatch1 , 配准后图像1中与图像2中匹配上的角点
* ptArray *pCornerMatch2 ,配准后图像2中与图像1中匹配上的角点
*返回值：
*  无
*
*说明：根据两幅图像各自提取的角点进行配准
***********************************************************************/
void Register::RegisterBasedConner(unsigned char *imgBuf1,unsigned char *imgBuf2,
		int width, int height,ptArray *pCornerpos1,ptArray *pCornerpos2 ,
		  ptArray *pCornerMatch1 , ptArray *pCornerMatch2)
{
	int cnt1 , cnt2;
 	cnt1 = pCornerpos1->GetSize();
	cnt2 = pCornerpos2->GetSize();
	
	int i , j , m ,n ;
	int W = 15 ; 
	double aver1 , aver2;
	double sigama1 , sigama2;
	double temp;
	int x1, y1, x2 , y2;

	int lineByte = (width+3)/4*4;
	int sigama = (int) (1.0/8*height);
	double Cij;
	//G为相关矩阵
	double * G = new double[cnt1*cnt2];
	double r;
	//建立相关矩阵
	for ( i = 0 ; i < cnt1 ; i++)
	{
		for ( j = 0 ; j < cnt2 ; j++)
		{
			//获取角点
			x1 = pCornerpos1->GetAt(i).x;
			y1 = pCornerpos1->GetAt(i).y;
			x2 = pCornerpos2->GetAt(j).x;
			y2 = pCornerpos2->GetAt(j).y;
			//aver1、aver2为角点邻域像素的均值
			aver1 = 0;
			aver2 = 0;
			//sigama1、sigama2为角点邻域像素的方差
			sigama1 = 0;
			sigama2 = 0;
			temp=0;
			//求取均值
			for ( m = - (int) (W/2) ; m <= (int) (W/2) ; m++ )
			{
				for ( n = -(int) (W/2) ; n <= (int) (W/2) ; n++ )
				{
					aver1 += *(imgBuf1+lineByte*(y1+m)+x1+n);
					aver2 += *(imgBuf2+lineByte*(y2+m)+x2+n);
				}
			}
			aver1 /= (W*W);
			aver2 /= (W*W);
			//求取方差
			for ( m = - (int) (W/2) ; m <= (int) (W/2) ; m++ )
			{
				for ( n = -(int) (W/2) ; n <= (int) (W/2) ; n++ )
				{
					sigama1 += pow(*(imgBuf1+lineByte*(y1+m)+x1+n)-aver1,2);
					sigama2 += pow(*(imgBuf2+lineByte*(y2+m)+x2+n)-aver2,2);
					temp += (*(imgBuf1+lineByte*(y1+m)+x1+n)-aver1)
						*(*(imgBuf2+lineByte*(y2+m)+x2+n)-aver2);
				}
			}
			sigama1=sqrt(double(sigama1));
            sigama2=sqrt(double(sigama2));
            Cij=temp/(W*W*sigama1*sigama2);
            r=pow(x1-x2,2)+pow(y1-y2,2);
			G[i*cnt2+j]=(Cij+1)/2*exp(-r/(2*sigama*sigama));
		}
 	}
	 double * U = new double [cnt1*cnt1];
	 double * D = new double [cnt1*cnt2];
	 double * V = new double [cnt2*cnt2];
	 double eps = 1e-5;
	 int ka = max(cnt1,cnt2)+1;
	 //进行奇异值分解
	 dluav( G , cnt1 , cnt2 , U , V , eps , ka);
	 int length = min(cnt1,cnt2);
	 //将对角线上的值设为1 
	 for ( i = 0 ; i < length ; i++ )
	 {
	   G[i*cnt2+i] = 1;
     }
	 //P新的相关矩阵
	 double * P = new double [cnt1*cnt2];
	 damul( U , G , cnt1 , cnt1, cnt2 , D );
	 damul( D , V , cnt1 , cnt2 ,cnt2 , P );

	 ptArray pt1,pt2;
	 double maxrow = -1999;
	 double maxcol = -1999;
	 int imax,jmax;
	 for ( i = 0 ; i < cnt1 ; i++)
	 {
		 maxrow = -1999;
		 maxcol = -1999;
		 //求取该行最大的点所在的列
		 for ( j = 0 ; j < cnt2 ; j++)
		 {
			 if (P[i*cnt2+j] > maxrow)
			 {
				 maxrow = P[i*cnt2+j];
				 jmax = j ;
			 }
		 }
		 //求取jmax列最大的点所在的行
		 for ( m = 0 ; m < cnt1 ; m++)
		 {
			 if (P[m*cnt2+jmax] > maxcol)
			 {
				 maxcol = P[m*cnt2+jmax];
				 imax = m;
			 }
		 }
		 //判断是否既为行最大和列最大
		 if ( imax == i )
		 {
			 pCornerMatch1->Add(pCornerpos1->GetAt(i));
			 pCornerMatch2->Add(pCornerpos2->GetAt(jmax));
		 }
	 }
		delete []U;
		delete []V;
		delete []G;
		delete []P;
}
/***********************************************************************
* 函数名称：
* ConnerRegistering()
*
*函数参数：
*     1为成功,0为失败
*
*返回值：
*  无
*
*说明：调用matchBasedConner()函数对图像1和图像2进行角点匹配 ，并将配准后的
*      两幅图合并到一幅图中，并画出角点对的对应关系，对外的接口
***********************************************************************/
BOOL Register::ConnerRegistering()
{
	if (m_pImgData1==NULL || m_pImgData2==NULL)
	{
		return false;
	}
	if (m_imgHeight1!=m_imgHeight2 || m_imgWidth1!=m_imgWidth2)
	{
		return false;
	}

	//图像1提取角点
	HarisConnerDetect(m_pImgData1,m_imgWidth1,m_imgHeight1,&m_cornerpos1);

	//图2提取角点
	HarisConnerDetect(m_pImgData2,m_imgWidth2,m_imgHeight2,&m_cornerpos2);

	//角点匹配
	RegisterBasedConner(m_pImgData1,m_pImgData2,m_imgWidth2,m_imgHeight2,
		    &m_cornerpos1, &m_cornerpos2,&m_Matchpt1,&m_Matchpt2);

	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	
	//输出图像的位数与输入图像相同
	m_nBitCountOut=m_nBitCount1;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCount1);
	
    
	int lineByte=(m_imgWidth1*m_nBitCount1/8+3)/4*4;
	int imgBufSize=m_imgHeight1*lineByte;
	
	//输出图像的宽为原图的两倍，高与原图的相同
	m_imgWidthOut=2*lineByte;
	m_imgHeightOut=m_imgHeight1;

	m_pImgDataOut=new BYTE[imgBufSize*2];
    
	unsigned char *tempbuf1 = new unsigned char[lineByte*m_imgHeight1];
	unsigned char *tempbuf2 = new unsigned char[lineByte*m_imgHeight2];
	
	memcpy(tempbuf1,m_pImgData1,lineByte*m_imgHeight1);
	memcpy(tempbuf2,m_pImgData2,lineByte*m_imgHeight1);

	//画十字
	DrawCross(tempbuf1,m_imgWidth1,m_imgHeight1,&m_Matchpt1);
	DrawCross(tempbuf2,m_imgWidth2,m_imgHeight2,&m_Matchpt2);

	//将匹配的两幅图合并为一幅图，第一幅图在左边，第二幅图在右边
	for (int i = 0 ; i<lineByte;i++)
	{
		for (int j = 0 ; j<m_imgHeightOut;j++)
		{
			m_pImgDataOut[j*2*lineByte+i]=tempbuf1[j*lineByte+i];
			m_pImgDataOut[j*2*lineByte+i+lineByte]=tempbuf2[j*lineByte+i];
		}
	}
	CPoint pt1;
	pt1=m_Matchpt1.GetAt(0);

	//匹配的点对之间画线
	DrawLine(m_pImgDataOut,m_imgWidthOut,m_imgHeightOut,&m_Matchpt1,&m_Matchpt2);
    
	//如果有颜色表，分配颜色表所需要的空间，并拷贝颜色表
	if(m_nColorTableLengthOut!=0){
		m_lpColorTableOut= new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable1, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	delete[] tempbuf1;
	delete[] tempbuf2;
	return true;
}

/***********************************************************************
* 函数名称：
* DrawLine()
*
*函数参数：
* unsigned char *imgBufOut, 图像数组
* int widthOut, 图像的宽
* int heightOut, 图像的高
* ptArray *pcornerpos1, 存放合并前的第一幅图像的匹配上的角点, 即在原图中的坐标
* ptArray *pcornerpos2, 存放合并前的第二幅图像的匹配上的角点，即在原图中的坐标
*返回值：
*  无
*
*说明：匹配的点对之间画线，结果在imgBufOut中输出
***********************************************************************/
void Register::DrawLine(unsigned char *imgBufOut, int widthOut, int heightOut,
		ptArray *pcornerpos1, ptArray *pcornerpos2)
{
	double k;
	int num = pcornerpos1->GetSize();
	CPoint pt1;
	CPoint pt2;
	for ( int i=0; i<num ; i++)
	{
		pt1=pcornerpos1->GetAt(i);
		pt2=pcornerpos2->GetAt(i);
		pt2.x += widthOut/2;
		//k为直线斜率
		k = 1.0*(pt2.y-pt1.y)/(1.0*(pt2.x-pt1.x));
		//画线，即将线上的像素值设为255
		for (int x = pt1.x ; x<=pt2.x ; x++)
		{
			int y = (int)(pt1.y+k*(x-pt1.x));
			imgBufOut[y*widthOut+x] = 255;
		}
	}
}

//实数矩阵的奇异值分解
//利用Householder变换及变形QR算法
//a-长度为m*n的数组，返回时其对角线依次给出奇异值，其余元素为0
//m-矩阵的行数
//n-矩阵的列数
//u-长度为m*m的数组，返回时存放奇异值分解的左奇异量U
//v-长度为n*n的数组，返回时存放奇异值分解的左奇异量V
//eps-精度要求
//ka-整型变量，其值为max(n,m)+1
//调用函数：dluav(),ppp(),sss()
int Register::dluav(double a[],int m,int n,double u[],double v[],double eps,int ka)
{ 
	int i,j,k,l,it,ll,kk,ix,iy,mm,nn,iz,m1,ks;
	double d,dd,t,sm,sm1,em1,sk,ek,b,c,shh,fg[2],cs[2];
	double *s,*e,*w;
	s=(double *)malloc(ka*sizeof(double));
	e=(double *)malloc(ka*sizeof(double));
	w=(double *)malloc(ka*sizeof(double));
	it=60; 
	k=n;
	if (m-1<n) 
	{
		k=m-1;
	}
	l=m;
	if (n-2<m) 
	{
		l=n-2;
	}
	if (l<0) 
	{
		l=0;
	}
	ll=k;
	if (l>k) 
	{
		ll=l;
	}
	if (ll>=1)
	{ 
		for (kk=1; kk<=ll; kk++)
		{ 
			if (kk<=k)
			{
				d=0.0;
				for (i=kk; i<=m; i++)
				{
					ix=(i-1)*n+kk-1; 
					d=d+a[ix]*a[ix];
				}
				s[kk-1]=sqrt(d);
				if (s[kk-1]!=0.0)
				{ 
					ix=(kk-1)*n+kk-1;
					if (a[ix]!=0.0)
					{ 
						s[kk-1]=fabs(s[kk-1]);
						if (a[ix]<0.0)
						{
							s[kk-1]=-s[kk-1];
						}
					}
					for (i=kk; i<=m; i++)
					{ 
						iy=(i-1)*n+kk-1;
						a[iy]=a[iy]/s[kk-1];
					}
					a[ix]=1.0+a[ix];
				}
				s[kk-1]=-s[kk-1];
			}
			if (n>=kk+1)
			{ 
				for (j=kk+1; j<=n; j++)
				{ 
					if ((kk<=k)&&(s[kk-1]!=0.0))
					{ 
						d=0.0;
						for (i=kk; i<=m; i++)
						{ 
							ix=(i-1)*n+kk-1;
							iy=(i-1)*n+j-1;
							d=d+a[ix]*a[iy];
						}
						d=-d/a[(kk-1)*n+kk-1];
						for (i=kk; i<=m; i++)
						{ 
							ix=(i-1)*n+j-1;
							iy=(i-1)*n+kk-1;
							a[ix]=a[ix]+d*a[iy];
						}
					}
					e[j-1]=a[(kk-1)*n+j-1];
				}
			}
			if (kk<=k)
			{
				for (i=kk; i<=m; i++)
				{ 
					ix=(i-1)*m+kk-1; 
					iy=(i-1)*n+kk-1;
					u[ix]=a[iy];
				}
			}
			if (kk<=l)
			{ 
				d=0.0;
				for (i=kk+1; i<=n; i++)
				{
					d=d+e[i-1]*e[i-1];
				}
				e[kk-1]=sqrt(d);
				if (e[kk-1]!=0.0)
				{
					if (e[kk]!=0.0)
					{
						e[kk-1]=fabs(e[kk-1]);
						if (e[kk]<0.0)
						{
							e[kk-1]=-e[kk-1];
						}
					}
					for (i=kk+1; i<=n; i++)
					{
						e[i-1]=e[i-1]/e[kk-1];
					}
					e[kk]=1.0+e[kk];
				}
				e[kk-1]=-e[kk-1];
				if ((kk+1<=m)&&(e[kk-1]!=0.0))
				{ 
					for (i=kk+1; i<=m; i++)
					{
						w[i-1]=0.0;
					}
					for (j=kk+1; j<=n; j++)
					{
						for (i=kk+1; i<=m; i++)
						{
							w[i-1]=w[i-1]+e[j-1]*a[(i-1)*n+j-1];
						}
					}
					for (j=kk+1; j<=n; j++)
					{
						for (i=kk+1; i<=m; i++)
						{ 
							ix=(i-1)*n+j-1;
							a[ix]=a[ix]-w[i-1]*e[j-1]/e[kk];
						}
					}
				}
				for (i=kk+1; i<=n; i++)
				{
					v[(i-1)*n+kk-1]=e[i-1];
				}
			}
  }
}
	mm=n;
	if (m+1<n) 
	{
		mm=m+1;
	}
	if (k<n) 
	{
		s[k]=a[k*n+k];
	}
	if (m<mm) 
	{
		s[mm-1]=0.0;
	}
	if (l+1<mm) 
	{
		e[l]=a[l*n+mm-1];
	}
	e[mm-1]=0.0;
	nn=m;
	if (m>n) 
	{
		nn=n;
	}
	if (nn>=k+1)
	{ 
		for (j=k+1; j<=nn; j++)
		{ 
			for (i=1; i<=m; i++)
			{
				u[(i-1)*m+j-1]=0.0;
			}
			u[(j-1)*m+j-1]=1.0;
		}
	}
	if (k>=1)
	{ 
		for (ll=1; ll<=k; ll++)
		{ 
			kk=k-ll+1; iz=(kk-1)*m+kk-1;
			if (s[kk-1]!=0.0)
			{ 
				if (nn>=kk+1)
				{
					for (j=kk+1; j<=nn; j++)
					{
						d=0.0;
						for (i=kk; i<=m; i++)
						{
							ix=(i-1)*m+kk-1;
							iy=(i-1)*m+j-1;
							d=d+u[ix]*u[iy]/u[iz];
						}
						d=-d;
						for (i=kk; i<=m; i++)
						{ 
							ix=(i-1)*m+j-1;
							iy=(i-1)*m+kk-1;
							u[ix]=u[ix]+d*u[iy];
						}
					}
				}
				for (i=kk; i<=m; i++)
				{
					ix=(i-1)*m+kk-1;
					u[ix]=-u[ix];
				}
				u[iz]=1.0+u[iz];
				if (kk-1>=1)
				{
					for (i=1; i<=kk-1; i++)
					{
						u[(i-1)*m+kk-1]=0.0;
					}
				}
			}
			else
			{ 
				for (i=1; i<=m; i++)
				{
					u[(i-1)*m+kk-1]=0.0;
				}
				u[(kk-1)*m+kk-1]=1.0;
			}
		}
	}
	for (ll=1; ll<=n; ll++)
	{ 
		kk=n-ll+1;
		iz=kk*n+kk-1;
		if ((kk<=l)&&(e[kk-1]!=0.0))
		{
			for (j=kk+1; j<=n; j++)
			{ 
				d=0.0;
				for (i=kk+1; i<=n; i++)
				{
					ix=(i-1)*n+kk-1; 
					iy=(i-1)*n+j-1;
					d=d+v[ix]*v[iy]/v[iz];
				}
				d=-d;
				for (i=kk+1; i<=n; i++)
				{
					ix=(i-1)*n+j-1; 
					iy=(i-1)*n+kk-1;
					v[ix]=v[ix]+d*v[iy];
				}
			}
		}
		for (i=1; i<=n; i++)
		{
			v[(i-1)*n+kk-1]=0.0;
		}
		v[iz-n]=1.0;
	}
	for (i=1; i<=m; i++)
	{
		for (j=1; j<=n; j++)
		{
			a[(i-1)*n+j-1]=0.0;
		}
	}
	m1=mm; 
	it=60;
while (1==1)
{ 
	if (mm==0)
	{ 
		ppp(a,e,s,v,m,n);
		free(s); 
		free(e); 
		free(w); 
		return(1);
	}
	if (it==0)
	{ 
		ppp(a,e,s,v,m,n);
		free(s); 
		free(e); 
		free(w); 
		return(-1);
	}
	kk=mm-1;
	while ((kk!=0)&&(fabs(e[kk-1])!=0.0))
	{ 
		d=fabs(s[kk-1])+fabs(s[kk]);
		dd=fabs(e[kk-1]);
		if (dd>eps*d)
		{
			kk=kk-1;
		}
		else 
		{
			e[kk-1]=0.0;
		}
	}
	if (kk==mm-1)
	{
		kk=kk+1;
		if (s[kk-1]<0.0)
		{ 
			s[kk-1]=-s[kk-1];
			for (i=1; i<=n; i++)
			{ 
				ix=(i-1)*n+kk-1; 
				v[ix]=-v[ix];
			}
		}
		while ((kk!=m1)&&(s[kk-1]<s[kk]))
		{ 
			d=s[kk-1]; 
			s[kk-1]=s[kk];
			s[kk]=d;
			if (kk<n)
			{
				for (i=1; i<=n; i++)
				{
					ix=(i-1)*n+kk-1; 
					iy=(i-1)*n+kk;
					d=v[ix]; 
					v[ix]=v[iy];
					v[iy]=d;
				}
			}
			if (kk<m)
			{
				for (i=1; i<=m; i++)
				{
					ix=(i-1)*m+kk-1; iy=(i-1)*m+kk;
					d=u[ix]; u[ix]=u[iy]; u[iy]=d;
				}
			}
			kk=kk+1;
		}
		it=60;
		mm=mm-1;
	}
	else
	{ 
		ks=mm;
		while ((ks>kk)&&(fabs(s[ks-1])!=0.0))
		{
			d=0.0;
			if (ks!=mm)
			{
				d=d+fabs(e[ks-1]);
			}
			if (ks!=kk+1)
			{
				d=d+fabs(e[ks-2]);
			}
			dd=fabs(s[ks-1]);
			if (dd>eps*d) 
			{
				ks=ks-1;
			}
			else 
			{
				s[ks-1]=0.0;
			}
		}
		if (ks==kk)
		{ 
			kk=kk+1;
			d=fabs(s[mm-1]);
			t=fabs(s[mm-2]);
			if (t>d) 
			{
				d=t;
			}
			t=fabs(e[mm-2]);
			if (t>d) 
			{
				d=t;
			}
			t=fabs(s[kk-1]);
			if (t>d) 
			{
				d=t;
			}
			t=fabs(e[kk-1]);
			if (t>d) 
			{
				d=t;
			}
			sm=s[mm-1]/d; 
			sm1=s[mm-2]/d;
			em1=e[mm-2]/d;
			sk=s[kk-1]/d;
			ek=e[kk-1]/d;
			b=((sm1+sm)*(sm1-sm)+em1*em1)/2.0;
			c=sm*em1; 
			c=c*c; 
			shh=0.0;
			if ((b!=0.0)||(c!=0.0))
			{ 
				shh=sqrt(b*b+c);
				if (b<0.0) 
				{
					shh=-shh;
				}
				shh=c/(b+shh);
			}
			fg[0]=(sk+sm)*(sk-sm)-shh;
			fg[1]=sk*ek;
			for (i=kk; i<=mm-1; i++)
			{ 
				sss(fg,cs);
				if (i!=kk)
				{ 
					e[i-2]=fg[0];
				}
				fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];
				e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];
				fg[1]=cs[1]*s[i];
				s[i]=cs[0]*s[i];
				if ((cs[0]!=1.0)||(cs[1]!=0.0))
				{
					for (j=1; j<=n; j++)
					{
						ix=(j-1)*n+i-1;
						iy=(j-1)*n+i;
						d=cs[0]*v[ix]+cs[1]*v[iy];
						v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
						v[ix]=d;
					}
				}
				sss(fg,cs);
				s[i-1]=fg[0];
				fg[0]=cs[0]*e[i-1]+cs[1]*s[i];
				s[i]=-cs[1]*e[i-1]+cs[0]*s[i];
				fg[1]=cs[1]*e[i];
				e[i]=cs[0]*e[i];
				if (i<m)
				{
					if ((cs[0]!=1.0)||(cs[1]!=0.0))
					{
						for (j=1; j<=m; j++)
						{
							ix=(j-1)*m+i-1;
							iy=(j-1)*m+i;
							d=cs[0]*u[ix]+cs[1]*u[iy];
							u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
							u[ix]=d;
						}
					}
				}
			}
			e[mm-2]=fg[0];
			it=it-1;
		}
		else
		{
			if (ks==mm)
			{ 
				kk=kk+1;
				fg[1]=e[mm-2];
				e[mm-2]=0.0;
				for (ll=kk; ll<=mm-1; ll++)
				{ 
					i=mm+kk-ll-1;
					fg[0]=s[i-1];
					sss(fg,cs);
					s[i-1]=fg[0];
					if (i!=kk)
					{
						fg[1]=-cs[1]*e[i-2];
						e[i-2]=cs[0]*e[i-2];
					}
					if ((cs[0]!=1.0)||(cs[1]!=0.0))
					{
						for (j=1; j<=n; j++)
						{
							ix=(j-1)*n+i-1;
							iy=(j-1)*n+mm-1;
							d=cs[0]*v[ix]+cs[1]*v[iy];
							v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
							v[ix]=d;
						}
					}
				}
			}
			else
			{ 
				kk=ks+1;
				fg[1]=e[kk-2];
				e[kk-2]=0.0;
				for (i=kk; i<=mm; i++)
				{
					fg[0]=s[i-1];
					sss(fg,cs);
					s[i-1]=fg[0];
					fg[1]=-cs[1]*e[i-1];
					e[i-1]=cs[0]*e[i-1];
					if ((cs[0]!=1.0)||(cs[1]!=0.0))
					{
						for (j=1; j<=m; j++)
						{
							ix=(j-1)*m+i-1;
							iy=(j-1)*m+kk-2;
							d=cs[0]*u[ix]+cs[1]*u[iy];
							u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
							u[ix]=d;
						}
					}
				}
			}
		}
  }
}
	return(1);
}

void Register::ppp(double a[],double e[],double s[],double v[],int m,int n)
{ 
	int i,j,p,q;
	double d;
	if (m>=n) 
	{
		i=n;
	}
	else 
	{
		i=m;
	}
	for (j=1; j<=i-1; j++)
	{
		a[(j-1)*n+j-1]=s[j-1];
		a[(j-1)*n+j]=e[j-1];
	}
	a[(i-1)*n+i-1]=s[i-1];
	if (m<n) 
	{
		a[(i-1)*n+i]=e[i-1];
	}
	for (i=1; i<=n-1; i++)
	{
		for (j=i+1; j<=n; j++)
		{
			p=(i-1)*n+j-1;
			q=(j-1)*n+i-1;
			d=v[p]; 
			v[p]=v[q];
			v[q]=d;
		}
	}
	return;
}

void Register::sss(double fg[2],double cs[2])
{ 
	double r,d;
	if ((fabs(fg[0])+fabs(fg[1]))==0.0)
	{ 
		cs[0]=1.0; 
		cs[1]=0.0; 
		d=0.0;
	}
	else 
	{ 
		d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
		if (fabs(fg[0])>fabs(fg[1]))
		{ 
			d=fabs(d);
			if (fg[0]<0.0)
			{
				d=-d;
			}
		}
		if (fabs(fg[1])>=fabs(fg[0]))
		{ 
			d=fabs(d);
			if (fg[1]<0.0) 
			{
				d=-d;
			}
		}
		cs[0]=fg[0]/d; 
		cs[1]=fg[1]/d;
	}
	r=1.0;
	if (fabs(fg[0])>fabs(fg[1])) 
	{
		r=cs[1];
	}
	else
	{
		if(cs[0]!=0.0)
		{
			r=1.0/cs[0];
		}
		fg[0]=d; 
		fg[1]=r;
		return;
	}
} 

void Register::damul(double a[],double b[],int m,int n,int k,double c[])
{ 
	int i,j,l,u;
    for (i=0; i<m; i++)
	{
		for (j=0; j<k; j++)
		{ 
			u=i*k+j; 
			c[u]=0.0;
			for (l=0; l<n; l++)
			{
				c[u]+=a[i*n+l]*b[l*k+j];
			}
		}
	}
    return;
}