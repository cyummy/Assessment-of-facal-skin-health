#ifndef _INSIDE_VISUAL_CPP_Register
#define _INSIDE_VISUAL_CPP_Register
#include "ImageDib.h"

#include <afxtempl.h>
#include <vector>
using namespace std;

typedef vector<float> fvect;

//ͼ����׼��
typedef CArray<CPoint,CPoint> ptArray;

class Register:public ImageDib
{
public:
	int m_nBitCount1;//����ͼ��1��ÿ����λ��
	unsigned char * m_pImgData1;//����ͼ��1��λͼ����ָ��
	LPRGBQUAD m_lpColorTable1;//����ͼ��1����ɫ��ָ��
	int m_nColorTableLength1;//����ͼ��1����ɫ����

	int m_nBitCount2;//����ͼ��2��ÿ����λ��
	unsigned char * m_pImgData2;//����ͼ��2��λͼ����ָ��
	LPRGBQUAD m_lpColorTable2;//����ͼ��2����ɫ��ָ��
	int m_nColorTableLength2;//����ͼ��2����ɫ����
	
	int m_nBitCountOut;//���ͼ��ÿ����λ��
	unsigned char * m_pImgDataOut;//���ͼ��λͼ����ָ��
	LPRGBQUAD m_lpColorTableOut;//���ͼ����ɫ��ָ��
	int m_nColorTableLengthOut;//���ͼ����ɫ����

public:
	//���������Ĺ��캯��
	Register();
	//����ͼ��1������
	void InputImg1(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
		           unsigned char *pImgData);
	//����ͼ��2������
	void InputImg2(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
		           unsigned char *pImgData);

	//��������
	~Register();

	//�������ͼ��ĳߴ�
	CSize GetDimensions();	
    
	//ģ��ƥ��Ķ���ӿں���
	BOOL relativityRegistering();

	//�ǵ��⺯��,ֻ����ͼ��1�Ľǵ�
	void HarisConner();

	//���ڽǵ�������׼�Ķ���ӿں���
	BOOL ConnerRegistering();

private:
	//����ͼ��1�Ŀ���
	int m_imgWidth1;
	int m_imgHeight1;

	//����ͼ��2�Ŀ���
	int m_imgWidth2;
	int m_imgHeight2;

	//���ͼ��Ŀ��
	int m_imgWidthOut;
	int m_imgHeightOut;

	//ͼ��1��2ƥ���Ľǵ�����
	ptArray m_Matchpt1;
	ptArray m_Matchpt2;

	//ͼ��1�Ľǵ�����
	ptArray m_cornerpos1;

	//ͼ��2�Ľǵ�����
	ptArray m_cornerpos2;

	//�ǵ������ֵ
	int m_space;

	//ģ��ƥ���׼����
	CPoint RegisterBasedSpacial(unsigned char *imgBuf, int width, int height, 
		unsigned char * blockBuf, int blockW, int blockH);

	//Haris�ǵ��⺯��,���Ľǵ���pCornerpos����
	void HarisConnerDetect(unsigned char *imgBufIn, int width, int height, 
		                   ptArray * pCornerpos);

	//����harris�ǵ���ǰ�ľ������
	void conv2(float *Src, int width, int height, float *mark, int markWidth,
		       int markHeight );

	//�����˺���
	void ArraydotMultiArray( float * dest, float * src1, float * src2, 
		                     int width , int height);

    //����harris�ǵ���ȡ����ȡ�ֲ�����ֵ����
    bool LocalMaximum(float * src, int width, int height, int posx, 
		              int posy , int neiboursize);

	///���ݾ����һ������
	void SpaceSuppress(ptArray * pCornerpos);

	//��ʮ�ֺ���
	void DrawCross(unsigned char * imgBufIn, int imWidth, int imHeight,
		           ptArray * pCornerpos);


	//���ڽǵ�������׼�ı�׼����
	void RegisterBasedConner(unsigned char *imgBuf1, unsigned char *imgBuf2, 
		int width, int height,ptArray *pCornerpos1, ptArray *pCornerpos2,
		 ptArray *pCornerMatch1 , ptArray *pCornerMatch2);

	//���������ǵ����飬����ƥ�������
	void DrawLine(unsigned char *imgBufOut, int widthOut, int heightOut,
		ptArray *pcornerpos1, ptArray *pcornerpos2);

	//ʵ�����������ֵ�ֽ�
	int dluav(double a[],int m,int n,double u[],double v[],double eps,int ka);

	//�������㺯������dluav��������
	void sss(double fg[2],double cs[2]);

	//�������㺯������dluav��������
	void ppp(double a[],double e[],double s[],double v[],int m,int n);

	//ʵ���������
	void damul(double a[],double b[],int m,int n,int k,double c[]);
};
#endif // _INSIDE_VISUAL_CPP_Match