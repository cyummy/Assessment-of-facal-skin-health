// FaceDetect.cpp: implementation of the FaceDetect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "FaceDetect.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FaceDetect::FaceDetect()
{
	m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0	

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0
}

FaceDetect::FaceDetect(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					 unsigned char *pImgData1,unsigned char *pImgData2):
ImageDib(size, nBitCount, lpColorTable, pImgData1)
{

    m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0

	m_pImgDataIn1=pImgData1;//m_pImgData;
	m_pImgDataIn2=pImgData2;
	//肤色模型参数定义
	Cb_Mean=117.4361;                 //参数可调 根据不同的图像
	Cr_Mean=156.5599;
	Cov00=160.1301;
	Cov01=12.1430;
	Cov10=12.1430;
	Cov11=299.4574;
}

FaceDetect::~FaceDetect()
{

}

CSize FaceDetect::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

void FaceDetect::CalMedFl(double **s, int w, int h, int n)      //中值滤波平滑
{
	
	int i,j;
	double **temp;
	temp = new double*[h+2*(int)(n/2)];
	for(i=0;i<h+2*(int)(n/2);i++)
		temp[i] = new double[w+2*(int)(n/2)];
	for(i=0;i<w+2*(int)(n/2);i++)
		for(j=0;j<h+2*(int)(n/2);j++)
			temp[j][i] = 0.0;
		
		for(i=0;i<h;i++)
			for(j=0;j<w;j++)
				temp[i+(int)(n/2)][j+(int)(n/2)]=s[i][j];
			for(i=0;i<h;i++)
				for(j=0;j<w;j++)
				{
					s[i][j]=0.0;
					for(int r=0;r<n;r++)
						for(int c=0;c<n;c++)
							s[i][j]+=temp[i+r][j+c];
						s[i][j]/=n*n;
				}
				if(temp!=NULL)
				{
					for(i=0;i<h+2;i++)
						if(temp[i]!=NULL)
							delete temp[i];
						delete temp;
				}
				
}

/***********************************************************************
* 函数名称：CalSim()
* 函数参数：无
* 返回值：  无
* 说明： 相似度计算
***********************************************************************/
void FaceDetect::CalSim()         
{
	//若灰度图像,则返回
	if(m_nBitCount==8) return;

	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//灰值化后,每像素位数为8比特
	m_nBitCountOut=8;

	//颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//申请颜色表缓冲区,生成灰度图像的颜色表
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}

	//输入图像每像素字节数,彩色图像为3字节/像素
	int pixelByteIn=3;
	
	//输入图像每行像素所占字节数,必须是4的倍数
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占字节数,必须是4的倍数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//申请输出图像位图数据缓冲区
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//循环变量,图像的坐标
	int i,j,k;
	char c_t1[4];

	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByteOut+j)=255;  //全白
		}
	}

	m_pSimArray = new double*[m_imgHeight];
	for(i=0;i<m_imgHeight;i++)
		m_pSimArray[i] = new double[m_imgWidth];

	for(i=0;i<m_imgHeight;i++){
		for (j=0;j<m_imgWidth;j++){
			for(k=0;k<3;k++){
				c_t1[k]=*(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+k);
			}
			
			int C_b=(int)c_t1[0]&255;
			int C_g=(int)c_t1[1]&255;
			int C_r=(int)c_t1[2]&255;
            //从RGB色彩空间映射到YCbCr色彩空间
			double Cb=(128-37.797*C_r/255-74.203*C_g/255+112*C_b/255);
			double Cr=(128+112*C_r/255-93.786*C_g/255-18.214*C_b/255);
			//利用肤色模型，求相似度矩阵
			double tt =(Cb-Cb_Mean)*((Cb-Cb_Mean)*Cov11-(Cr-Cr_Mean)*Cov10)+(Cr-Cr_Mean)*(-(Cb-Cb_Mean)*Cov01+(Cr-Cr_Mean)*Cov00);
			tt =(-0.5*tt)/(Cov00*Cov11-Cov01*Cov10);
			m_pSimArray[i][j]=exp(tt);
		}
	}

	CalMedFl(m_pSimArray,m_imgWidth, m_imgHeight,9);  //中值滤波
	//用相似度矩阵中的最大值对相似度矩阵进行归一化
	double max=0.0;

	for(i=0;i<m_imgHeight;i++){
		for (j=0;j<m_imgWidth;j++){
			if(m_pSimArray[i][j]>max)
				max=m_pSimArray[i][j];
		}
	}
	for( i=0;i<m_imgHeight;i++){
		for (j=0;j<m_imgWidth;j++){
			m_pSimArray[i][j]= m_pSimArray[i][j]/max;
		}
    }
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByteOut+j)=(int)(m_pSimArray[i][j]*255);
		}
	}
}



/***********************************************************************
* 函数名称：CalSBound
* 函数参数：int top,int bottom,int left,int right
* 返回值：  无
* 说明： 脸内轮廓提取
***********************************************************************/

void FaceDetect::CalSBound(int top,int bottom,int left,int right)
{

	//若灰度图像,则返回
	if(m_nBitCount==8) return;

	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//灰值化后,每像素位数为8比特
	m_nBitCountOut=8;

	//颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//申请颜色表缓冲区,生成灰度图像的颜色表
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}

	//输入图像每像素字节数,彩色图像为3字节/像素
	int pixelByteIn=3;
	
	//输入图像每行像素所占字节数,必须是4的倍数
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占字节数,必须是4的倍数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//申请输出图像位图数据缓冲区
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	//循环变量,图像的坐标
	int i,j,l;
		
	double **result;					 
	result = new double*[m_imgHeight];
	for(l=0 ; l<m_imgHeight; l++){
		result[l] = new double[m_imgWidth];
		for(j=0; j<m_imgWidth; j++)
			result[l][j] = *(m_pImgDataIn1+l*lineByteIn+j*pixelByteIn+2);
	}
	
	for(i=0; i<m_imgHeight; i++)
		for(j=0; j<m_imgWidth; j++){
			double r,g,temp;
			//归一化
			temp = *(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn)+
			       *(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+1)+
				   *(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+2);
			r = (double)*(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+2)/temp;
			g = (double)*(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+1)/temp;
			//通过肤色检测脸的位置
			if(g<0.398 && g > 0.246 && r<0.664 && r>0.233 && r>g && g>=0.5*(1-r)){
				*(m_pImgDataOut+i*lineByteOut+j)=255;              
			}
			else	*(m_pImgDataOut+i*lineByteOut+j)=0;
	}
	
	//高斯 拉普拉斯边缘算子
	for(i=top+2; i<bottom-2; i++)
		for(j=left+2; j<right-2; j++){
			result[i][j] =  
				(0-2.0/24.0)*((unsigned char)*(m_pImgDataOut+(i-2)*lineByteOut+j-2)) + 
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i-2)*lineByteOut+j-1)) + 
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i-2)*lineByteOut+j)) +
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i-2)*lineByteOut+j+1)) +
				(0-2.0/24.0)*((unsigned char)*(m_pImgDataOut+(i-2)*lineByteOut+j+2)) +
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i-1)*lineByteOut+j-2)) + 
				(8.0/24.0)  *((unsigned char)*(m_pImgDataOut+(i-1)*lineByteOut+j)) +
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i-1)*lineByteOut+j+2)) +
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+i*lineByteOut+j-2)) + 
				(8.0/24.0)  *((unsigned char)*(m_pImgDataOut+i*lineByteOut+j-1)) + 
				(1.0)       *((unsigned char)*(m_pImgDataOut+i*lineByteOut+j)) +
				(8.0/24.0)  *((unsigned char)*(m_pImgDataOut+i*lineByteOut+j+1)) +
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+i*lineByteOut+j+2)) +
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i+1)*lineByteOut+j-2)) + 
				(8.0/24.0)  *((unsigned char)*(m_pImgDataOut+(i+1)*lineByteOut+j)) +
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i+1)*lineByteOut+j+2)) +
				(0-2.0/24.0)*((unsigned char)*(m_pImgDataOut+(i+2)*lineByteOut+j-2)) + 
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i+2)*lineByteOut+j-1)) + 
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i+2)*lineByteOut+j)) +
				(0-4.0/24.0)*((unsigned char)*(m_pImgDataOut+(i+2)*lineByteOut+j+1)) +
				(0-2.0/24.0)*((unsigned char)*(m_pImgDataOut+(i+2)*lineByteOut+j+2));
	}
			
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByteOut+j)=255; //像素点设为全白
		}
	}

	for(i=top+1; i<bottom-1; i++)
		for(j=left+1; j<right-1; j++){
			int positive = 0;   
			int negtive  = 0;

			//3*3的子窗口 满足条件的像素点设为黑色 描出脸内轮廓
			for(int m=-1;m<=1;m++)                
				for(int n=-1;n<=1;n++)
					if(m!=0 || n!=0){
						if(result[i+m][j+n]<-5)negtive++;
						if(result[i+m][j+n]>=5)positive++;
					}
					if(positive>2 && negtive>2){
						*(m_pImgDataOut+i*lineByteOut+j) = 0;           
					}
	}
				
	if(result!=NULL){
		for (int i=0 ;i<m_imgHeight;i++)
			if(result[i]!=NULL) delete result[i];
			delete result;
	}
	

}


void FaceDetect::ImageCut(int F_top,int F_bottom,int F_left,int F_right)
{
	//若灰度图像,则返回
	if(m_nBitCount==8) return;

	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//每像素位数为24比特
	m_nBitCountOut=24;

	//输入图像每像素字节数,彩色图像为3字节/像素
	int pixelByteIn=3;
    //输出图像每像素字节数,彩色图像为3字节/像素
    int pixelByteOut=3;
	
	//输入图像每行像素所占字节数,必须是4的倍数
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;
	//输出图像每行像素所占字节数,必须是4的倍数
    int lineByteOut=(m_imgWidth*pixelByteOut+3)/4*4;

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;


	//申请输出图像位图数据缓冲区
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	//循环变量,图像的坐标
	int i,j;
    for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByteOut+j*3+0)=255;
			*(m_pImgDataOut+i*lineByteOut+j*3+1)=255;
			*(m_pImgDataOut+i*lineByteOut+j*3+2)=255;//像素点设为全白
		}
	}
    
	//定位框
    for(i=F_top;i<=F_bottom;i++){
		*(m_pImgDataOut+i*lineByteOut+F_left*3)=0;
		*(m_pImgDataOut+i*lineByteOut+F_left*3+1)=255;
		*(m_pImgDataOut+i*lineByteOut+F_left*3+2)=0;
		*(m_pImgDataOut+i*lineByteOut+F_right*3)=0;
		*(m_pImgDataOut+i*lineByteOut+F_right*3+1)=255;
		*(m_pImgDataOut+i*lineByteOut+F_right*3+2)=0;
		*(m_pImgDataOut+i*lineByteOut+(F_left+1)*3)=0;
		*(m_pImgDataOut+i*lineByteOut+(F_left+1)*3+1)=255;
		*(m_pImgDataOut+i*lineByteOut+(F_left+1)*3+2)=0;
		*(m_pImgDataOut+i*lineByteOut+(F_right+1)*3)=0;
		*(m_pImgDataOut+i*lineByteOut+(F_right+1)*3+1)=255;
		*(m_pImgDataOut+i*lineByteOut+(F_right+1)*3+2)=0;
		*(m_pImgDataOut+i*lineByteOut+(F_left-1)*3)=0;
		*(m_pImgDataOut+i*lineByteOut+(F_left-1)*3+1)=255;
		*(m_pImgDataOut+i*lineByteOut+(F_left-1)*3+2)=0;
		*(m_pImgDataOut+i*lineByteOut+(F_right-1)*3)=0;
		*(m_pImgDataOut+i*lineByteOut+(F_right-1)*3+1)=255;
		*(m_pImgDataOut+i*lineByteOut+(F_right-1)*3+2)=0;
	}
	for(j=F_left;j<=F_right;j++){
		*(m_pImgDataOut+F_bottom*lineByteOut+j*3)=0;
		*(m_pImgDataOut+F_bottom*lineByteOut+j*3+1)=255;
		*(m_pImgDataOut+F_bottom*lineByteOut+j*3+2)=0;
		*(m_pImgDataOut+F_top*lineByteOut+j*3)=0;
		*(m_pImgDataOut+F_top*lineByteOut+j*3+1)=255;
		*(m_pImgDataOut+F_top*lineByteOut+j*3+2)=0;
		*(m_pImgDataOut+(F_bottom-1)*lineByteOut+j*3)=0;
		*(m_pImgDataOut+(F_bottom-1)*lineByteOut+j*3+1)=255;
		*(m_pImgDataOut+(F_bottom-1)*lineByteOut+j*3+2)=0;
		*(m_pImgDataOut+(F_top-1)*lineByteOut+j*3)=0;
		*(m_pImgDataOut+(F_top-1)*lineByteOut+j*3+1)=255;
		*(m_pImgDataOut+(F_top-1)*lineByteOut+j*3+2)=0;
		*(m_pImgDataOut+(F_bottom+1)*lineByteOut+j*3)=0;
		*(m_pImgDataOut+(F_bottom+1)*lineByteOut+j*3+1)=255;
		*(m_pImgDataOut+(F_bottom+1)*lineByteOut+j*3+2)=0;
		*(m_pImgDataOut+(F_top+1)*lineByteOut+j*3)=0;
		*(m_pImgDataOut+(F_top+1)*lineByteOut+j*3+1)=255;
		*(m_pImgDataOut+(F_top+1)*lineByteOut+j*3+2)=0;
	}
	
	for(i=F_top+2;i<F_bottom-1;i++){
		for(j=F_left+2;j<F_right-1;j++){
			*(m_pImgDataOut+i*lineByteOut+j*3+0)=*(m_pImgDataIn1+i*lineByteIn+j*3+0);
			*(m_pImgDataOut+i*lineByteOut+j*3+1)=*(m_pImgDataIn1+i*lineByteIn+j*3+1);
			*(m_pImgDataOut+i*lineByteOut+j*3+2)=*(m_pImgDataIn1+i*lineByteIn+j*3+2);
		}
	}

}



     float MIN(float a,float b){
           float min;
           min=a;
           if(b<a)
	          min=b;
           return min;  
	 }

     float MAX(float a,float b){
           float max;
           max=a;
	       if(b>a)
		     max=b;
	       return max;
	 }




    void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v){

         float min, max, delta;
         min = MIN( r, MIN( g, b ));
         max = MAX( r, MAX( g, b ));	 
         *v = max;               // v在0到1之间
         delta = max - min;
         if( max != 0 )
            *s = delta / max;       // s在0到1之间
         else {
               // r = g = b = 0        // s = 0, v is undefined
            *s = 0;
            *h = -1;
            return;
		 }
         if( r == max )
            *h = ( g - b ) *60/ delta;     // between yellow & magenta
         else if( g == max )
            *h =( 2 + ( b - r ) )*60/ delta; // between cyan & yellow
         else
            *h = (4 + ( r - g)) *60/ delta; // between magenta & cyan
                                  // degrees
         if( *h < 0 )
            *h += 360;                  //h在0到360之间
	}

    void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v ){

         int i;
         float f, p, q, t;
         if( s == 0 ) {
             // achromatic (grey)
            *r = *g = *b = v;
         return;
		 }
         h /= 60;            // sector 0 to 5
         i = floor( h );
         f = h - i;          // factorial part of h
         p = v * ( 1 - s );
         q = v * ( 1 - s * f );
         t = v * ( 1 - s * ( 1 - f ) );
         switch( i ) {
         case 0:
              *r = v;
              *g = t;
              *b = p;
              break;
         case 1:
              *r = q;
              *g = v;
              *b = p;
              break;
         case 2:
              *r = p;
              *g = v;
              *b = t;
              break;
         case 3:
              *r = p;
              *g = q;
              *b = v;
              break;
         case 4:
              *r = t;
              *g = p;
              *b = v;
              break;
         default:        // case 5:
              *r = v;
              *g = p;
              *b = q;
              break;
		 }
	}  





//传统同态滤波
void FaceDetect::Homomorphicfilter()
{
	    //若灰度图像,则返回
	    if(m_nBitCount==8) 
			return;
   
        //释放旧的输出图像缓冲区
     	if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;

		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
	

		//灰度图像输出
/*		m_nBitCountOut=8;

		//颜色表长度
	    m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	    //申请颜色表缓冲区,生成灰度图像的颜色表
	    if(m_nColorTableLengthOut!=0){
    	   m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		   for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		   }
		}
		
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

        //输出图像的宽高,与输入图像相等
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;

        m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
        
		m_nBitCountOut1=8;
        int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;


        //输入图像每像素字节数,彩色图像为3字节/像素
		int m_nBitCount=24;  
		//输入图像每行像素所占字节数,必须是4的倍数
		int lineByteIn=(m_imgWidth*m_nBitCount/8+3)/4*4;*/
        
        
	
        //彩色图像输出
		m_nBitCountOut=24;
		int pixelByteOut=3;
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

        //输出图像的宽高,与输入图像相等
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;

        m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

		m_nBitCountOut1=8;
        m_nBitCountOut2=8;
		
		//输出图像每行像素所占字节数,必须是4的倍数
        int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;
		int lineByteOut2=(m_imgWidth*m_nBitCountOut2/8+3)/4*4;

		//输入图像每像素字节数,彩色图像为3字节/像素
		int m_nBitCount=24;  
		//输入图像每行像素所占字节数,必须是4的倍数
		int lineByteIn=(m_imgWidth*m_nBitCount/8+3)/4*4;
		
		//申请输出图像位图数据缓冲区	
        m_pImgDataOut1=new unsigned char[lineByteOut1*m_imgHeight];


	    
		
		//循环变量,图像的坐标
		int m,n;
		int r,g,b;
		float R,G,B,H,S,V;
		float *pa,*pb,*pc;
		pa=new float[1000000];//V
		pb=new float[1000000];//S
		pc=new float[1000000];//H
		
	
		for(m=0;m<m_imgHeight;m++){
			for(n=0;n<m_imgWidth;n++){
                b=*(m_pImgDataIn1+m*lineByteIn+n*3+0);
				g=*(m_pImgDataIn1+m*lineByteIn+n*3+1);
				r=*(m_pImgDataIn1+m*lineByteIn+n*3+2); 
				B=b/255.0;  //整型转浮点型
				G=g/255.0;
				R=r/255.0; 
	          	RGBtoHSV(R, G, B, pc+m*lineByteOut1+n, pb+m*lineByteOut1+n, pa+m*lineByteOut1+n);
				V=*(pa+m*lineByteOut1+n);
                float K=-log(V)*100;
				if(K>255.0f)	K=255.0f;
	            *(m_pImgDataOut1+m*lineByteOut1+n)=(unsigned char)(K);                    
			}
		}

        int y ;
     	int x ;
	
	    double dTmpOne ;
	    double dTmpTwo ;
	    double D;
	
	
      	// 傅立叶变换的宽度和高度（2的整数次幂）
	    int nTransWidth ;
	    int nTransHeight;
	
	    // 计算进行傅立叶变换的点数	（2的整数次幂）
	    dTmpOne = log(m_imgWidth)/log(2);
    	dTmpTwo = ceil(dTmpOne)		   ;
    	dTmpTwo = pow(2,dTmpTwo)	   ;
    	nTransWidth = (int) dTmpTwo	   ;
	
    	// 计算进行傅立叶变换的点数 （2的整数次幂）
    	dTmpOne = log(m_imgHeight)/log(2);
    	dTmpTwo = ceil(dTmpOne)		   ;
    	dTmpTwo = pow(2,dTmpTwo)	   ;
    	nTransHeight = (int) dTmpTwo   ;
	
    	//滤波的半径不能超过频域的最大半径
    	double D0=10;
    	if(D0 > nTransWidth-1 || D0 > nTransHeight-1)
		{
	    	return ;
		}
	    
		//申请输出图像缓冲区，并初始化为0
	    m_pImgDataOut2=new unsigned char[m_imgWidthOut*m_imgHeightOut];
        memset(m_pImgDataOut2, 0, m_imgWidthOut*m_imgHeightOut);
	    //申请傅立叶缓冲区，并初始化为0
     	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
    	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	    //输入图像数据进行二维傅立叶变换
        FFtTrans.ImageFFt2D(m_pImgDataOut1, m_imgWidthOut, m_imgHeightOut,m_pImgDataOut2);//m_pImgDataOut2存的是频谱图像
       

        double Rh=2;
        double Rl=0.5;
        double c=1.4;
		double tha,spec;

	    // 下面开始实施ButterWorth高通滤波
     	for(y=0; y<nTransHeight; y++)
		{
		   for(x=0; x<nTransWidth; x++)
		   {
		       D = (double)((y-(nTransHeight-1)/2)*(y-(nTransHeight-1)/2)+(x-(nTransWidth-1)/2)*(x-(nTransWidth-1)/2));  
		       D=D*D*c*c*c*c;
		       D = (D0 * D0 * D0 * D0) / D; 
		 	   D = 1/(1+D);
	           D = D*(Rh-Rl)+Rl;
		       //FFtTrans.m_pFFTBuf[y*nTransWidth + x].real=(FFtTrans.m_pFFTBuf[y*nTransWidth + x].real)*D;
           	   //FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag=(FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag)*D;
  
			   tha=atan2(FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag,FFtTrans.m_pFFTBuf[y*nTransWidth + x].real);
			   spec=sqrt(pow(FFtTrans.m_pFFTBuf[y*nTransWidth + x].real,2) + pow(FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag,2))*D;//*(m_pImgDataOut2+y*lineByteOut2+x)*1;
		       FFtTrans.m_pFFTBuf[y*nTransWidth + x].real=spec*cos(tha);
           	   FFtTrans.m_pFFTBuf[y*nTransWidth + x].imag=spec*sin(tha);
		   }	
		}

	    // 经过高通滤波的图象进行反变换
    	FFtTrans.ImageRevFFt2D(m_pImgDataOut1,m_imgWidthOut, m_imgHeightOut);
	   
       for(m=0;m<m_imgHeight;m++){
			for(n=0;n<m_imgWidth;n++){
			   V=*(m_pImgDataOut1+m*lineByteOut1+n)/100.0f;
               pa[m*lineByteOut1+n]=1/exp(V);
               V=pa[m*lineByteOut1+n];
			   S=pb[m*lineByteOut1+n];
		   	   H=pc[m*lineByteOut1+n]; 
			   HSVtoRGB( pc+m*lineByteOut1+n, pb+m*lineByteOut1+n, pa+m*lineByteOut1+n, H, S, V );
			   *(m_pImgDataOut+m*lineByteOut+n*pixelByteOut+0)=*(pa+m*lineByteOut1+n)*255;
     	       *(m_pImgDataOut+m*lineByteOut+n*pixelByteOut+1)=*(pb+m*lineByteOut1+n)*255;
			   *(m_pImgDataOut+m*lineByteOut+n*pixelByteOut+2)=*(pc+m*lineByteOut1+n)*255;	   
			}
		}
}




void FaceDetect::Adaptivethresegment()
{
    //若灰度图像,则返回
	if(m_nBitCount==8) return;

	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut1!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut1!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}


	//灰值化后,每像素位数为8比特
	m_nBitCountOut=8;
    m_nBitCountOut1=8;

	//颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
    m_nColorTableLengthOut1=ComputeColorTabalLength(m_nBitCountOut1);


	//申请颜色表缓冲区,生成灰度图像的颜色表
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}
    
	//申请颜色表缓冲区,生成灰度图像的颜色表
	if(m_nColorTableLengthOut1!=0){
    	m_lpColorTableOut1=new RGBQUAD[m_nColorTableLengthOut1];
		for(int i=0; i<m_nColorTableLengthOut1;i++){
			m_lpColorTableOut1[i].rgbBlue=i;
			m_lpColorTableOut1[i].rgbGreen=i;
			m_lpColorTableOut1[i].rgbRed=i;
			m_lpColorTableOut1[i].rgbReserved=0;
		}
	}

	//输入图像每像素字节数,彩色图像为3字节/像素
	int pixelByteIn=3;
	
	//输入图像每行像素所占字节数,必须是4的倍数
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占字节数,必须是4的倍数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;


	//申请输出图像位图数据缓冲区
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	m_pImgDataOut1=new unsigned char[lineByteOut1*m_imgHeight];

    memset(m_pImgDataOut, 255, m_imgWidthOut*m_imgHeightOut);

	
    //通过遍历整幅图像 获取输入图像人脸定位框的边界值
    int i,j,right,left,top,bottom;
	for(i=0;i<m_imgHeight;i++){
	    for(j=0;j<m_imgWidth;j++){
			if(*(m_pImgData+i*lineByteIn+j*pixelByteIn+1)==255&&
			   *(m_pImgData+i*lineByteIn+j*pixelByteIn+0)==0&&
               *(m_pImgData+i*lineByteIn+j*pixelByteIn+2)==0)
			   break;			
		}
	    	if(*(m_pImgData+i*lineByteIn+j*pixelByteIn+1)==255&&
			   *(m_pImgData+i*lineByteIn+j*pixelByteIn+0)==0&&
               *(m_pImgData+i*lineByteIn+j*pixelByteIn+2)==0)
	    	break;
	}
	left=j+1;
	top=i+1;
		
    
    for(i=m_imgHeight-1;i>=0;i--){
		 for(j=m_imgWidth-1;j>=0;j--){
		     if(*(m_pImgData+i*lineByteIn+j*pixelByteIn+1)==255&&
			    *(m_pImgData+i*lineByteIn+j*pixelByteIn+0)==0&&
                *(m_pImgData+i*lineByteIn+j*pixelByteIn+2)==0)
			    break;		
		}
            if(*(m_pImgData+i*lineByteIn+j*pixelByteIn+1)==255&&
		       *(m_pImgData+i*lineByteIn+j*pixelByteIn+0)==0&&
               *(m_pImgData+i*lineByteIn+j*pixelByteIn+2)==0)
	    	break;
	}
	right=j-1;
	bottom=i-1;


    int m,n;
    //int MAX;
 	//根据灰值化公式为输出图像赋值
    for(m=top+2;m<bottom-1;m++){
	   for(n=left+2;n<right-1;n++){
		   *(m_pImgDataOut1+m*lineByteOut1+n)=0.11**(m_pImgData+m*lineByteIn+n*pixelByteIn+0)
				                             +0.59**(m_pImgData+m*lineByteIn+n*pixelByteIn+1)
			                                 +0.30**(m_pImgData+m*lineByteIn+n*pixelByteIn+2)+0.5;			
		   
	   }   
	}
    
/*	MAX=*(m_pImgDataOut1+(top+2)*lineByteOut1+left+2);
	for(m=top+2;m<bottom-1;m++){
	   for(n=left+2;n<right-1;n++){	   		
		   if(*(m_pImgDataOut1+m*lineByteOut1+n)>MAX)
			  MAX=*(m_pImgDataOut1+m*lineByteOut1+n);
	   }   
	}

    for(m=top+2;m<bottom-1;m++){
	   for(n=left+2;n<right-1;n++){
           *(m_pImgDataOut1+m*lineByteOut1+n)=*(m_pImgDataOut1+m*lineByteOut1+n)/MAX;
		   *(m_pImgDataOut1+m*lineByteOut1+n)=*(m_pImgDataOut1+m*lineByteOut1+n)*255;
	   }
	}*/
    
    int C=7,N=9;//需要调整一系列参数 使得识别效果达到最佳
	int nLocAvg ;	// 子图象的平均值
	int max,min;
	double r,g,temp;
	// 对图像逐点扫描：	
	for(m=top+2+(N-1)/2;m<bottom-1-(N-1)/2;m++){
		for(n=left+2+(N-1)/2;n<right-1-(N-1)/2;n++){
		    max=min=*(m_pImgDataOut1+m*lineByteOut1+n);
			nLocAvg = 0 ;
            for(i=0;i<N;i++){
				for(j=0;j<N;j++){	
					if(*(m_pImgDataOut1+(m-(N-1)/2+i)*lineByteOut1+(n-(N-1)/2+j))>max)
					    max=*(m_pImgDataOut1+(m-(N-1)/2+i)*lineByteOut1+(n-(N-1)/2+j));
					if(*(m_pImgDataOut1+(m-(N-1)/2+i)*lineByteOut1+(n-(N-1)/2+j))<min)
					    min=*(m_pImgDataOut1+(m-(N-1)/2+i)*lineByteOut1+(n-(N-1)/2+j));
				}
			}
            nLocAvg=(max+min)/2;
			
			//归一化
			temp = *(m_pImgData+m*lineByteIn+n*pixelByteIn)+
			       *(m_pImgData+m*lineByteIn+n*pixelByteIn+1)+
				   *(m_pImgData+m*lineByteIn+n*pixelByteIn+2);
			r = (double)*(m_pImgData+m*lineByteIn+n*pixelByteIn+2)/temp;
			g = (double)*(m_pImgData+m*lineByteIn+n*pixelByteIn+1)/temp;
			
			if(abs(*(m_pImgDataOut1+m*lineByteOut1+n)-nLocAvg)>C && g<0.398 && g > 0.246 && r<0.664 && r>0.233 && r>g && g>=0.5*(1-r))
			   *(m_pImgDataOut+m*lineByteOut+n)=0;
			else
			   *(m_pImgDataOut+m*lineByteOut+n)=255;		
		}
	}

//	filter = Morphology(CSize(m_imgWidthOut,m_imgHeightOut),m_nBitCountOut,m_lpColorTableOut,m_pImgDataOut);
	//memcpy(filter.m_pImgData, m_pImgDataOut, m_imgWidthOut*m_imgHeightOut);
  //  filter.Close();



}

void FaceDetect::DWThomomorphicfilter()
{
	    //若灰度图像,则返回
	    if(m_nBitCount==8) 
			return;
   
        //释放旧的输出图像缓冲区
     	if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;

		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}

		
	

		//灰度图像输出
/*		m_nBitCountOut=8;

		//颜色表长度
	    m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	    //申请颜色表缓冲区,生成灰度图像的颜色表
	    if(m_nColorTableLengthOut!=0){
    	   m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		   for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		   }
		}
		
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

        //输出图像的宽高,与输入图像相等
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;

        m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
        
		m_nBitCountOut1=8;
        int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;


        //输入图像每像素字节数,彩色图像为3字节/像素
		int m_nBitCount=24;  
		//输入图像每行像素所占字节数,必须是4的倍数
		int lineByteIn=(m_imgWidth*m_nBitCount/8+3)/4*4;*/
        
        
	
        //彩色图像输出
		m_nBitCountOut=24;
		int pixelByteOut=3;
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

        //输出图像的宽高,与输入图像相等
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;

        m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

		m_nBitCountOut1=8;
		//颜色表长度
	    m_nColorTableLengthOut1=ComputeColorTabalLength(m_nBitCountOut1);
	    //申请颜色表缓冲区,生成灰度图像的颜色表
	    if(m_nColorTableLengthOut1!=0){
    	   m_lpColorTableOut1=new RGBQUAD[m_nColorTableLengthOut1];
		   for(int i=0; i<m_nColorTableLengthOut1;i++){
			m_lpColorTableOut1[i].rgbBlue=i;
			m_lpColorTableOut1[i].rgbGreen=i;
			m_lpColorTableOut1[i].rgbRed=i;
			m_lpColorTableOut1[i].rgbReserved=0;
		   }
		}		
		//输出图像每行像素所占字节数,必须是4的倍数
        int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;
		
    	//申请输出图像位图数据缓冲区	
        m_pImgDataOut1=new unsigned char[lineByteOut1*m_imgHeight];

		//输入图像每像素字节数,彩色图像为3字节/像素
		int m_nBitCount=24;  
		//输入图像每行像素所占字节数,必须是4的倍数
		int lineByteIn=(m_imgWidth*m_nBitCount/8+3)/4*4;
		
	


	    
		
		//循环变量,图像的坐标
		int m,n;
		int r,g,b;
		float R,G,B,H,S,V;
		float *pa,*pb,*pc;
		pa=new float[1000000];//V
		pb=new float[1000000];//S
		pc=new float[1000000];//H
		
	
		for(m=0;m<m_imgHeight;m++){
			for(n=0;n<m_imgWidth;n++){
                b=*(m_pImgDataIn1+m*lineByteIn+n*3+0);
				g=*(m_pImgDataIn1+m*lineByteIn+n*3+1);
				r=*(m_pImgDataIn1+m*lineByteIn+n*3+2); 
				B=b/255.0;  //整型转浮点型
				G=g/255.0;
				R=r/255.0; 
	          	RGBtoHSV(R, G, B, pc+m*lineByteOut1+n, pb+m*lineByteOut1+n, pa+m*lineByteOut1+n);
				V=*(pa+m*lineByteOut1+n);
                float K=-log(V)*100;
				if(K>255.0f)	K=255.0f;
	             *(m_pImgDataOut1+m*lineByteOut1+n)=(unsigned char)(K);    
               //*(m_pImgDataOut+m*lineByteOut+n)=(unsigned char)(K); 
			}
		}

	
	    	
		DWtTrans.InputImageData(CSize(m_imgWidth,m_imgHeight),m_nBitCountOut1,
		m_lpColorTableOut1, m_pImgDataOut1);
		DWtTrans.BasicWaveletTrans(3);

		//DWtTrans.BasicWaveletReverse();
		//m_pImgDataOut = DWtTrans.m_pImgDataOut;

        //DWtTrans.InputImageData(CSize(m_imgWidth,m_imgHeight),m_nBitCountOut,	m_lpColorTableOut, m_pImgDataOut);
	
		DWtTrans.BasicWaveletReverse();
        m_pImgDataOut1 = DWtTrans.m_pImgDataOut;
	    //输入图像数据进行小波变换  

	    //经过高通滤波的图象进行反变换

	   
      for(m=0;m<m_imgHeight;m++){
			for(n=0;n<m_imgWidth;n++){
			   V=*(m_pImgDataOut1+m*lineByteOut1+n)/100.0f;
               pa[m*lineByteOut1+n]=1/exp(V);
               V=pa[m*lineByteOut1+n];
			   S=pb[m*lineByteOut1+n];
		   	   H=pc[m*lineByteOut1+n]; 
			   HSVtoRGB( pc+m*lineByteOut1+n, pb+m*lineByteOut1+n, pa+m*lineByteOut1+n, H, S, V );
			   *(m_pImgDataOut+m*lineByteOut+n*pixelByteOut+0)=*(pa+m*lineByteOut1+n)*255;
     	       *(m_pImgDataOut+m*lineByteOut+n*pixelByteOut+1)=*(pb+m*lineByteOut1+n)*255;
			   *(m_pImgDataOut+m*lineByteOut+n*pixelByteOut+2)=*(pc+m*lineByteOut1+n)*255;	   
			}
		}
}

