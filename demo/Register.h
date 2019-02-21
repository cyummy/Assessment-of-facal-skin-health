#ifndef _INSIDE_VISUAL_CPP_Register
#define _INSIDE_VISUAL_CPP_Register
#include "ImageDib.h"

#include <afxtempl.h>
#include <vector>
using namespace std;

typedef vector<float> fvect;

//图像配准类
typedef CArray<CPoint,CPoint> ptArray;

class Register:public ImageDib
{
public:
	int m_nBitCount1;//输入图像1的每像素位数
	unsigned char * m_pImgData1;//输入图像1的位图数据指针
	LPRGBQUAD m_lpColorTable1;//输入图像1的颜色表指针
	int m_nColorTableLength1;//输入图像1的颜色表长度

	int m_nBitCount2;//输入图像2的每像素位数
	unsigned char * m_pImgData2;//输入图像2的位图数据指针
	LPRGBQUAD m_lpColorTable2;//输入图像2的颜色表指针
	int m_nColorTableLength2;//输入图像2的颜色表长度
	
	int m_nBitCountOut;//输出图像每像素位数
	unsigned char * m_pImgDataOut;//输出图像位图数据指针
	LPRGBQUAD m_lpColorTableOut;//输出图像颜色表指针
	int m_nColorTableLengthOut;//输出图像颜色表长度

public:
	//不带参数的构造函数
	Register();
	//输入图像1的数据
	void InputImg1(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
		           unsigned char *pImgData);
	//输入图像2的数据
	void InputImg2(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
		           unsigned char *pImgData);

	//析构函数
	~Register();

	//返回输出图像的尺寸
	CSize GetDimensions();	
    
	//模板匹配的对外接口函数
	BOOL relativityRegistering();

	//角点检测函数,只计算图像1的角点
	void HarisConner();

	//基于角点特征配准的对外接口函数
	BOOL ConnerRegistering();

private:
	//输入图像1的宽、高
	int m_imgWidth1;
	int m_imgHeight1;

	//输入图像2的宽、高
	int m_imgWidth2;
	int m_imgHeight2;

	//输出图像的宽高
	int m_imgWidthOut;
	int m_imgHeightOut;

	//图像1、2匹配后的角点数组
	ptArray m_Matchpt1;
	ptArray m_Matchpt2;

	//图像1的角点数组
	ptArray m_cornerpos1;

	//图像2的角点数组
	ptArray m_cornerpos2;

	//角点距离阈值
	int m_space;

	//模板匹配标准函数
	CPoint RegisterBasedSpacial(unsigned char *imgBuf, int width, int height, 
		unsigned char * blockBuf, int blockW, int blockH);

	//Haris角点检测函数,检测的角点由pCornerpos返回
	void HarisConnerDetect(unsigned char *imgBufIn, int width, int height, 
		                   ptArray * pCornerpos);

	//用于harris角点检测前的卷积函数
	void conv2(float *Src, int width, int height, float *mark, int markWidth,
		       int markHeight );

	//矩阵点乘函数
	void ArraydotMultiArray( float * dest, float * src1, float * src2, 
		                     int width , int height);

    //用于harris角点提取的提取局部极大值函数
    bool LocalMaximum(float * src, int width, int height, int posx, 
		              int posy , int neiboursize);

	///根据距离进一步抑制
	void SpaceSuppress(ptArray * pCornerpos);

	//画十字函数
	void DrawCross(unsigned char * imgBufIn, int imWidth, int imHeight,
		           ptArray * pCornerpos);


	//基于角点特征配准的标准函数
	void RegisterBasedConner(unsigned char *imgBuf1, unsigned char *imgBuf2, 
		int width, int height,ptArray *pCornerpos1, ptArray *pCornerpos2,
		 ptArray *pCornerMatch1 , ptArray *pCornerMatch2);

	//根据两个角点数组，画出匹配的连线
	void DrawLine(unsigned char *imgBufOut, int widthOut, int heightOut,
		ptArray *pcornerpos1, ptArray *pcornerpos2);

	//实数矩阵的奇异值分解
	int dluav(double a[],int m,int n,double u[],double v[],double eps,int ka);

	//矩阵运算函数，被dluav函数调用
	void sss(double fg[2],double cs[2]);

	//矩阵运算函数，被dluav函数调用
	void ppp(double a[],double e[],double s[],double v[],int m,int n);

	//实数矩阵相乘
	void damul(double a[],double b[],int m,int n,int k,double c[]);
};
#endif // _INSIDE_VISUAL_CPP_Match