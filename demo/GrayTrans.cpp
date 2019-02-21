// GrayTrans.cpp: implementation of the GrayTrans class.
//
//////////////////////////////////////////////////////////////////////
#include "stdio.h"
#include "stdafx.h"
#include "demo1.h"
#include "GrayTrans.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/***********************************************************************
* �������ƣ� GrayTrans()
* ˵�����޲����Ĺ��캯�����Գ�Ա�������г�ʼ��
***********************************************************************/
GrayTrans::GrayTrans()
{
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}

/***********************************************************************
* �������ƣ� GrayTrans()
* ���������� CSize size -ͼ���С�����ߣ�
*            int nBitCount  -ÿ���ر�����
*            LPRGBQUAD lpColorTable  -��ɫ��ָ��
*            unsigned char *pImgData  -λͼ����ָ��
* ����ֵ��   ��
* ˵����������Ϊ�������Ĺ��캯��������λͼ�Ĵ�С��ÿ����λ������ɫ��
*      ��λͼ���ݣ�����ImgDib()�Ի����Ա��ʼ������Ϊ����ͼ������
*      ��ص����ݳ�Ա,����ʼ�����ͼ����ص����ݳ�Ա
***********************************************************************/
GrayTrans::GrayTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					 unsigned char *pImgData):
ImageDib(size, nBitCount, lpColorTable, pImgData)
{

    m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}


/***********************************************************************
* �������ƣ�  ~GrayTrans()
* ˵���������������ͷ���Դ
***********************************************************************/
GrayTrans::~GrayTrans()
{
	//�ͷ����ͼ��λͼ���ݻ�����
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//�ͷ����ͼ����ɫ��
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}

/***********************************************************************
* �������ƣ� GetDimensions()
* ����������  ��
* ����ֵ��   ͼ��ĳߴ磬��CSize���ͱ��
* ˵�����������ͼ��Ŀ�͸�
***********************************************************************/
CSize GrayTrans::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* �������ƣ�  LinearStrech()
* ��������:   CPoint point1,CPoint point2  -�ֶ����ԵĶ�Ӧ��
* ����ֵ:     ��
* ˵�����ֶ��������캯��,���������ֶε㣬��m_pImgData��ָ�򻺳����еĻҶ�
*      ���ɫͼ������������죬Ϊm_pImgDataOut�����ڴ棬�����������
***********************************************************************/
void GrayTrans::LinearStrech(CPoint point1,CPoint point2)
{
	//�ͷžɵ����ͼ�񻺳���
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��ÿ����λ��������ͼ����ͬ
	m_nBitCountOut=m_nBitCount;

	//������ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//������ɫ������ͼ�����ɫ�������ͼ����ɫ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//���ͼ��ÿ��������ռ���ֽ���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	if(m_nBitCount==8)//����ǻҶ�ͼ������õ�ͨ�������������캯��
		LinearStrechForSnglChannel(m_pImgData, m_pImgDataOut,
		                             m_imgWidth, m_imgHeight,point1,point2);
	else{
		//����ɫͼ�����ÿ��ͨ�������ݽ��з��룬���������죬Ȼ���ٺϳ�Ϊ��ɫͼ��

		//��ͨ������ÿ��������ռ���ֽ���
		int lineByteGray=(m_imgWidth+3)/4*4;

		//��ʱ����������ŵ�ͨ������
		unsigned char *bufIn=new unsigned char[m_imgHeight*lineByteGray];

		//��ʱ����������ͨ����������Ľ��
		unsigned char *bufOut=new unsigned char[m_imgHeight*lineByteGray];
		//ѭ��������ͼ������
		int i, j;

		//ѭ��������������ɫͼ�������ͨ��
		int k;

		for(k=0;k<3;k++){
			//��ÿ��ͨ��������ȡ����������bufIn������
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					bufIn[i*lineByteGray+j]=*(m_pImgData+i*lineByteOut+j*3+k);
			}

			//��bufIn�������е����ݽ������죬����������bufOut��
			LinearStrechForSnglChannel(bufIn,bufOut,m_imgWidth,m_imgHeight,point1,point2);

			//����ͨ������Ľ���������ͼ��m_pImgDataOut��Ӧͨ����
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					*(m_pImgDataOut+i*lineByteOut+j*3+k)=bufOut[i*lineByteGray+j];
			}
		}

		//�ͷŻ�����
		delete []bufIn;
		delete []bufOut;
	}

}


/***********************************************************************
* �������ƣ�  LinearStrechForSnglChannel()
* ��������:   unsigned char *pImgDataIn   -�������λͼ����ָ��
*             unsigned char *pImgDataOut  -�����λͼ����ָ��
*             int imgWidth   -λͼ��,����Ϊ��λ
*             int imgHeight  -λͼ��,����Ϊ��λ
*             CPoint point1,point2 -�ֶ����Ե�����ת�۵�����
* ����ֵ:     ��
* ˵��������һ��ͨ�������ݣ��Լ�����ת�۵����꣬���ֶ���������, �����
*     ����pImgDataOut��ָ��Ļ�������
***********************************************************************/
void GrayTrans::LinearStrechForSnglChannel(unsigned char *pImgDataIn, 
		unsigned char *pImgDataOut, int imgWidth,int imgHeight,CPoint point1,CPoint point2)
{
	//ѭ��������ͼ������
	int i,j;

	//ÿ�����ص��ֽ�������ͨ��ͼ��
	int lineByte=(imgWidth+3)/4*4;

	//(x1,y1)��(x2,y2)Ϊ�����ֶε������
	double x1=point1.x,y1=point1.y,x2=point2.x, y2=point2.y;

	//�����ֶε�ֱ��б��
	double slop1=y1/x1,slop2=(y2-y1)/(x2-x1),slop3=(255-y2)/(255-x2);

	//����ֱ�������б��
	double dis1=0,dis2=y1-slop2*x1,dis3=y2-slop3*x2;

	//ӳ���
	int Map[256];

	//���������ֶΣ�������ͼ��ÿ���Ҷȼ�����ӳ���
	for(i=0;i<256;i++)	
	{
		if(i<x1)
			Map[i]=(int)(slop1*i+dis1+0.5);
		else if(i<x2)
			Map[i]=(int)(slop2*i+dis2+0.5);
		else
			Map[i]=(int)(slop3*i+dis3+0.5);
	}

	//��ʱ����
	int temp;

	//��ÿ�����أ������������ػҶȼ�����ӳ�����Ϊ������ظ�ֵ
	for(i=0;i<imgHeight;i++)
	{
		for(j=0;j<lineByte;j++)
		{
			//����ͼ��(i,j)���صĻҶȼ�
			temp=*(pImgDataIn+i*lineByte+j);

			//����ӳ���Ϊ���ͼ��ֵ
			*(pImgDataOut+i*lineByte+j)=Map[temp];
		}
	}
}




void GrayTrans::RevImage()
{
	//�ͷžɵ����ͼ�����ݼ���ɫ������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	
	//���ͼ��ÿ����λ����ԭͼ��ͬ
	m_nBitCountOut=m_nBitCount;
	
	//��ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	
	//�������ɫ����Ϊ��ɫ������ڴ�
	if(m_nColorTableLengthOut!=0){
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	
	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	
	//ÿ������ռ�ֽ���������Ϊ4�ı���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	
	//����ռ䣬��ű任����
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	
	//ѭ��������ͼ�������
	int i,j;
	
	//ÿ����ռ�ֽ���
	int pixelByte=m_nBitCountOut/8;
	
	//ѭ������,����ÿ�����ص�ÿ������,�����ɫͼ����������
	int k;
	
	//����
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++)
				*(m_pImgDataOut+i*lineByteOut+j*pixelByte+k)
				=255-*(m_pImgData+i*lineByteOut+j*pixelByte+k);
		}
	}

}

/**************************************************************************
*������
*    BinaryImage()
*
*������
*    int threshold   -��ֵ
*
*����ֵ
*    ��
*
*˵�����ú�����m_pImgData��Ϊ����ͼ�񣬸��ݸ�������ֵthreshold��ͼ����ж�ֵ����
*      �������m_pImgDataOut�С����ǲ�ɫͼ�����Ƚ�ͼ����л�ֵͼ����ٶ�ֵ����
***************************************************************************/
void GrayTrans::BinaryImage(int threshold)
{
	//���ڻҶ�ͼ��
	if(m_nBitCount==8){
		//�ͷžɵ����ͼ�����ݼ���ɫ������
		if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;
		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
		//���ͼ���ÿ����λ������ɫ����
		m_nBitCountOut=m_nBitCount;
		m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
		
		//�������ͼ����ɫ����������������ͼ����ɫ���������ͼ����ɫ����
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
		
		//���ͼ��Ŀ��,������ͼ�����
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;
		
		
		//ͼ��ÿ��������ռ�ֽ���������ͼ�������ͼ��ÿ��������ռ�ֽ������
		int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
		
		//�������ͼ��λͼ���ݻ�����
		m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];
		
		//ѭ��������ͼ�������
		int i,j;
		
		//��ֵ��
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++){
				if(*(m_pImgData+i*lineByte+j)<threshold)
					*(m_pImgDataOut+i*lineByte+j)=0;
				else
					*(m_pImgDataOut+i*lineByte+j)=255;
			}
		}
	}
	else
	{
		if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;
		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
		//��ֵ����,ÿ����λ��Ϊ8����
		m_nBitCountOut=8;
		//��ɫ����
		m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
		//������ɫ������,���ɻҶ�ͼ�����ɫ��
		if(m_nColorTableLengthOut!=0){
			m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
			for(int i=0; i<m_nColorTableLengthOut;i++){
				m_lpColorTableOut[i].rgbBlue=i;
				m_lpColorTableOut[i].rgbGreen=i;
				m_lpColorTableOut[i].rgbRed=i;
				m_lpColorTableOut[i].rgbReserved=0;
			}
		}
		
		//����ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
		int pixelByteIn=3;
		//����ͼ��ÿ��������ռ�ֽ���,������4�ı���
		int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;

		//���ͼ��Ŀ��,������ͼ�����
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;
		
		//���ͼ��ÿ��������ռ�ֽ���,������4�ı���
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
		
		//�������ͼ��λͼ���ݻ�����
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
		
		//ѭ������,ͼ�������
		int m,n;
		
		//���ݻ�ֵ����ʽΪ���ͼ��ֵ
		for(m=0;m<m_imgHeight;m++){
			for(n=0;n<m_imgWidth;n++)
				*(m_pImgDataOut+m*lineByteOut+n)=0.11**(m_pImgData+m*lineByteIn+n*pixelByteIn+0)
				+0.59**(m_pImgData+m*lineByteIn+n*pixelByteIn+1)
				+0.30**(m_pImgData+m*lineByteIn+n*pixelByteIn+2)+0.5;
	}
		//���ͼ��ÿ��������ռ�ֽ���������ͼ�������ͼ��ÿ��������ռ�ֽ�������
		int i,j;
		//��ֵ��
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++)
				if(*(m_pImgDataOut+i*lineByteOut+j)<threshold)
					*(m_pImgDataOut+i*lineByteOut+j)=0;
				else
					*(m_pImgDataOut+i*lineByteOut+j)=255;
		}
	}
}



void GrayTrans::ThresholdWindow(int bBottom, int bTop )
{
    
	//�ͷžɵ����ͼ�����ݼ���ɫ������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ���ÿ����λ������ɫ����
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	
	//�������ͼ����ɫ����������������ͼ����ɫ���������ͼ����ɫ����
	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
	memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	
	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ͼ��ÿ��������ռ�ֽ���������ͼ�������ͼ��ÿ��������ռ�ֽ������
	int lineByte=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	
	//�������ͼ��λͼ���ݻ�����
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeightOut];
	
	int pixelByte=m_nBitCountOut/8;
	//ѭ��������ͼ�������
	int i,j,k;

	//���ڱ任
	for(i=0;i<m_imgHeightOut ;i++){
		
		for(j=0;j<m_imgWidthOut;j++){
			for(k=0;k<pixelByte;k++){
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=*(m_pImgData+i*lineByte+j*pixelByte+k);
		
			if(*(m_pImgData+i*lineByte+j*pixelByte+k)<bBottom)
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=0;
			else if (*(m_pImgData+i*lineByte+j*pixelByte+k)>bTop) 
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=255;
			}
		
		}
	}

}


//�ҶȻ�
void GrayTrans::GaryImage()
{

	//���ڻҶ�ͼ��
	if(m_nBitCount==8)
		return;
	

		if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;
		}

		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
		//��ֵ����,ÿ����λ��Ϊ8����
		m_nBitCountOut=8;
		//��ɫ����
		m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
		//������ɫ������,���ɻҶ�ͼ�����ɫ��
		if(m_nColorTableLengthOut!=0){
			m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
			for(int i=0; i<m_nColorTableLengthOut;i++){
				m_lpColorTableOut[i].rgbBlue=i;
				m_lpColorTableOut[i].rgbGreen=i;
				m_lpColorTableOut[i].rgbRed=i;
				m_lpColorTableOut[i].rgbReserved=0;
			}
		}
		
		//����ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
		int pixelByteIn=3;
		//����ͼ��ÿ��������ռ�ֽ���,������4�ı���
		int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;

		//���ͼ��Ŀ��,������ͼ�����
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;
		
		//���ͼ��ÿ��������ռ�ֽ���,������4�ı���
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
		
		//�������ͼ��λͼ���ݻ�����
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
		
		//ѭ������,ͼ�������
		int m,n;
		
		//���ݻ�ֵ����ʽΪ���ͼ��ֵ
		for(m=0;m<m_imgHeight;m++){
			for(n=0;n<m_imgWidth;n++)
				*(m_pImgDataOut+m*lineByteOut+n)=0.11**(m_pImgData+m*lineByteIn+n*pixelByteIn+0)
				+0.59**(m_pImgData+m*lineByteIn+n*pixelByteIn+1)
				+0.30**(m_pImgData+m*lineByteIn+n*pixelByteIn+2)+0.5;
	}


}
