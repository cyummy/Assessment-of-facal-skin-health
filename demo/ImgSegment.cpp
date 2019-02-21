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
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}

ImgSegment::ImgSegment(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImageDib(size, nBitCount, lpColorTable, pImgData)
{
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}

ImgSegment::~ImgSegment()
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




CSize ImgSegment::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

/***********************************************************************
* �������ƣ�
* Roberts()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Roberts��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::Roberts()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//�м����
	int x, y, t;

	//Roberts����
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x�����ݶ�
				x=*(m_pImgData+i*lineByte+j*pixelByte+k)
					-*(m_pImgData+(i+1)*lineByte+j*pixelByte+k);

				//y�����ݶ�
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
* �������ƣ�
* Sobel()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Sobel��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::Sobel()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//�м����
	int x, y, t;

	//Sobel����
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x�����ݶ�
				x= *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 + 2 * *(m_pImgData+i*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 - 2 * *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k);

				//y�����ݶ�
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
* �������ƣ�
* Prewitt()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Prewitt��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::Prewitt()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//�м����
	int x, y, t;

	//Prewitt����
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x�����ݶ�
				x= *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+i*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k);

				//y�����ݶ�
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
* �������ƣ�
* Laplacian()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Laplacian��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::Laplacian()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//�м����
	int t;

	//Laplacian����
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
* �������ƣ� EdgeTrace()
* ���������� ��
* ����ֵ��   ��
*˵��������һ����ֵͼ�񣬰Ѻ�ɫ������Ŀ�꣬��ɫ��������������Ŀ��ı߽�
***********************************************************************/
void ImgSegment::EdgeTrace()
{
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			           sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//�����ͼ���ʼ����255,��0�����������ر��
	memset(m_pImgDataOut,255,lineByte*m_imgHeight);

	//˳ʱ�붨���������ص��8�������꣬��һ��Ϊx�����ƫ��,�ڶ���Ϊy�����ƫ��
	int direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};

	//�߽���ʼ��,������ĵ�ǰ��,��ǰ��������
	CPoint startP, currentP, neighborP;

	//�Ƿ�ǰ������ʼ�߽���غϵı�־����
	int findStartPoint;

	//�����߽���ʼ��
	findStartPoint=0;

	//ѭ��������ͼ������
	int i,j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			//�ҵ���ʼ�߽��
			if(*(m_pImgData+i*lineByte+j)==0){
				startP.x=j;
				startP.y=i;
				*(m_pImgDataOut+i*lineByte+j)=0;
				findStartPoint=1;
				break;
			}
		}

		//�Ѿ��ҵ���ʼ�߽��
		if(findStartPoint)
			break;
	}

	//�߽����
	//�ӳ�ʼ�㿪ʼ����
	currentP.x=startP.x;
	currentP.y=startP.y;

	//������Ƿ�߽���־����
	int isContourP;

	//��ʼ����
	int startDirect=0;

	//0��ʾ��û�з�������ı߽���ʼ��
	findStartPoint=0;
	while(findStartPoint==0){
		isContourP=false;
		while(isContourP==false){
			neighborP.x=currentP.x+direction[startDirect][0];
			neighborP.y=currentP.y+direction[startDirect][1];

			//�����������
			if(*(m_pImgData+neighborP.y*lineByte+neighborP.x)==0){
				isContourP=true;
				currentP.x=neighborP.x;
				currentP.y=neighborP.y;

				if(currentP.x==startP.x&&currentP.y==startP.y)
					findStartPoint=true;//�ص��߽���ʼ����

				*(m_pImgDataOut+currentP.y*lineByte+currentP.x)=0;

				//ɨ�跽����ʱ����ת90��
				startDirect-=2;
				if(startDirect<0)
					startDirect+=8;
			}
			else{
				//ɨ�跽��˳ʱ����ת45��
				startDirect++;
				if(startDirect==8)
					startDirect=0;
			}

		}
	}
	
}


void ImgSegment::RegionGrow(CPoint SeedPos, int thresh)
{

	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			     sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//�����ͼ���ʼ����255
	memset(m_pImgDataOut,255,lineByte*m_imgHeight);
    
	//������������������
	//��ά����direction�����������ص�8����������õ���x��y�����ϵ�ƫ��,
	//���е�һ��Ϊx�����ƫ��,�ڶ���Ϊy�����ƫ��
	int direction[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

	//ջ���룬�˴��ٶ���ջ���������Ϊͼ����������
	CPoint *stack=new CPoint[m_imgWidth*m_imgHeight];

	//ջ��ָ��
	int top;

	//��ǰ������ĵ�͵����ĵ�
	CPoint currentPoint, popPoint;

	//ѭ������������array����ĵ�һά�±�
	int k;

	//��Ǳ���
	int label;

	//��ʱ����
	int temp1, temp2;

	//��¼�������صĻҶ�ֵ
	temp1=*(m_pImgData+SeedPos.y*lineByte+SeedPos.x);

	//���������ӵ��ñ��0,��ջ
	*(m_pImgDataOut+SeedPos.y*lineByte+SeedPos.x)=0;
	top=0;
	stack[top].x=SeedPos.x;
	stack[top].y=SeedPos.y;

	//��ջ
	while(top>-1){
		//����ջ��Ԫ��,��Ԫ���Ѿ�������
		popPoint.x=stack[top].x;
		popPoint.y=stack[top].y;
		top--;

		//���쵯��������Χ�Ƿ���û������������
		for(k=0;k<8;k++){

			//������������
			currentPoint.x=popPoint.x+direction[k][0];
			currentPoint.y=popPoint.y+direction[k][1];

			//���������ĵ㲻��ͼ���ڣ�������
			if(currentPoint.x<0||currentPoint.x>m_imgWidth-1||
				currentPoint.y<0||currentPoint.y>m_imgHeight-1)
				continue;
			
			//�õ���
			label=*(m_pImgDataOut+currentPoint.y*lineByte+currentPoint.x);

			//�����ĵ���Χ����û�����ĵ�
			if(label==255){
				temp2=*(m_pImgData+currentPoint.y*lineByte+currentPoint.x);
				//�����ǰ����������ػҶ�ֵ�����ӵ�Ҷ�ֵ֮��С�ڸ�������ֵ,
				//����Ϊ����,�����ջ����
				if(abs(temp1-temp2)<thresh){
					//���õ����������0
					*(m_pImgDataOut+currentPoint.y*lineByte+currentPoint.x)=0;
					top++;
					stack[top].x=currentPoint.x;
					stack[top].y=currentPoint.y;
				}
			}
		}
		
	}
	
	//���������
	delete []stack;

}


void ImgSegment::AdaptThreshSeg(unsigned char *pImgData)
{
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;
	
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	
	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;
	
	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	
	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	
	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	
	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	
	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];
	
	//����ͼ��ĺᡢ������
	int i,j;
	// �ֲ���ֵ
	int nLocalThresh[2][2] ;
	// ��ͼ���ƽ��ֵ
	int nLocAvg ;
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	for(j=0; j<m_imgHeight/2 ; j++ )
	{
		for(i=0; i<m_imgWidth/2 ; i++ )
		{
			nLocAvg += pImgData[j*lineByte + i];
		}
	}
	// �����ֵ
	nLocAvg /= ( (m_imgHeight/2) * (m_imgWidth/2) ) ;

	// ������ֵΪ��ͼ���ƽ��ֵ
	nLocalThresh[0][0] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
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
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	// y����
	for(j=m_imgHeight/2; j<m_imgHeight ; j++ )
	{
		// x����
		for(i=0; i<m_imgWidth/2 ; i++ )
		{
			nLocAvg += pImgData[j*lineByte + i];
		}
	}
	// �����ֵ
	nLocAvg /= ( (m_imgHeight - m_imgHeight/2) * (m_imgWidth/2) ) ;

	// ������ֵΪ��ͼ���ƽ��ֵ
	nLocalThresh[1][0] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
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
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	for(j=0; j<m_imgHeight/2 ; j++ )
	{
		for(i=m_imgWidth/2; i<m_imgWidth ; i++ )
		{
			nLocAvg += pImgData[j*lineByte + i];
		}
	}
	// �����ֵ
	nLocAvg /= ( (m_imgHeight/2) * (m_imgWidth - m_imgWidth/2) ) ;
	
	// ������ֵΪ��ͼ���ƽ��ֵ
	nLocalThresh[0][1] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
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
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	for(j=m_imgHeight/2; j<m_imgHeight ; j++ )
	{
		for(i=m_imgWidth/2; i<m_imgWidth ; i++ )
		{
			nLocAvg +=pImgData[j*lineByte + i];
		}
	}
	// �����ֵ
	nLocAvg /= ( (m_imgHeight - m_imgHeight/2) * (m_imgWidth - m_imgWidth/2) ) ;

	// ������ֵΪ��ͼ���ƽ��ֵ
	nLocalThresh[1][1] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
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
	// Ϊ����ʾ������ʾ���߼�1�ú�ɫ��ʾ���߼�0�ð�ɫ��ʾ
	for(j=0; j<m_imgHeight ; j++ )
	{
		for(i=0; i<m_imgWidth ; i++ )
		{
			m_pImgDataOut[j*lineByte + i] = 255 - m_pImgDataOut[j*lineByte + i] ;
		}
	}
}

