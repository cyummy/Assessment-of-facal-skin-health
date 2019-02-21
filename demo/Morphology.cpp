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
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
	m_TempBuf=NULL;//�ṹԪ��ָ��Ϊ��
	TempH=3;   //�ṹԪ�ظ�
	TempW=3;   //�ṹԪ�ؿ�
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
	m_lpColorTableOut=NULL;	//���ͼ����ɫ����
	int colorTableLength=ComputeColorTabalLength(m_nBitCountOut);
	if(colorTableLength!=0){
      m_lpColorTableOut=new RGBQUAD[colorTableLength];
	  memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*colorTableLength);
	}
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];//�������ͼ������Ҫ�Ļ�����
	m_TempBuf=NULL;//�ṹԪ��ָ��Ϊ��
	TempH=3;
	TempW=3;
	int i;
	for (i=0;i<TempH*TempW;i++) {
	    m_InitTemp[i]=0;
	}
}

Morphology::~Morphology()
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
	
	//�ͷŽṹԪ�ػ�����
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
* �������ƣ� ImgErosion()
* ���������� unsigned char *imgBufIn   -����ʴ��ͼ��
*            unsigned char *imgBufOut  -��ʴ��Ľ��
*            int imgWidth   -ͼ���
*            int imgHeight   -ͼ���
*            int *TempBuf   -�ṹԪ�ػ�����ָ��
*            int TempW   -�ṹԪ�ؿ�
*            int TempH   -�ṹԪ�ظ�
*����ֵ��   ��
*˵������ֵ��ʴ��������,����Ϊ��ɫ,Ŀ��Ϊ��ɫ,��/��/��ֵ��̬ѧ�߽�Ȳ���
*      ��Ҫ�����������
***********************************************************************/
void Morphology::ImgErosion(unsigned char *imgBufIn,unsigned char *imgBufOut,int imgWidth,
							  int imgHeight,int *TempBuf, int TempW, int TempH) 
				
{
	int lineByte=(imgWidth+3)/4*4;	//ÿ��ͼ�����ݵ��ֽ���Ϊ4�ı���
	memcpy(imgBufOut,imgBufIn,lineByte*imgHeight);	//������ͼ�񻺳����Ŀ��������ͼ�񻺳���
	int i,j,k,l;
	int flag;	//��־����
	//���ͼ����ÿһ������λ�ã��ж��Ƿ�ṹԪ��������Ŀ���ڲ�
	for(i=TempH/2;i<imgHeight-TempH/2;i++)
	{
		for(j=TempW/2;j<imgWidth-TempW/2;j++)
		{			
			//�жϽṹԪ���Ƿ�����ڵ�ǰ������Ŀ���ڲ�,1Ϊ����,0Ϊ������
			flag=1;
			for(k=-TempH/2;k<=TempH/2;k++)
			{
				for(l=-TempW/2;l<=TempW/2;l++)
				{
					//�����ǰ�ṹԪ��λ��Ϊ1,�ж����Ӧͼ���ϵ����ص��Ƿ�Ϊ��0
					if(TempBuf[(k+TempH/2)*TempW+l+TempW/2])
					{
						//���ͼ��ǰ����Ϊ0,��û�л��иõ�,���Ǹ�ʴ�����
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
* �������ƣ� ImgDilation()
* ���������� unsigned char *imgBufIn   -�����͵�ͼ��
*            unsigned char *imgBufOut  -���ͺ�Ľ��
*            int imgWidth   -ͼ���
*            int imgHeight   -ͼ���
*            int *maskBuf   -�ṹԪ�ػ�����ָ��
*            int maskW   -�ṹԪ�ؿ�
*            int maskH   -�ṹԪ�ظ�
*����ֵ��   ��
*˵������ֵ���ͻ�������,����Ϊ��ɫ,Ŀ��Ϊ��ɫ,��/��/��ֵ��̬ѧ�߽�Ȳ���
*      ��Ҫ�����������
***********************************************************************/
void Morphology::ImgDilation(unsigned char *imgBufIn,unsigned char *imgBufOut,int imgWidth,int imgHeight,
					 int *TempBuf, int TempW, int TempH)
{
	int lineByte=(imgWidth+3)/4*4;//ÿ��ͼ�����ݵ��ֽ���Ϊ4�ı���
	int i,j,k,l;
	//�����Ƕ�ͼ����Ŀ�겹���ĸ�ʴ�������������ͼ�����ݵĲ���
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			*(imgBufIn+i*lineByte+j)=255-*(imgBufIn+i*lineByte+j);
		}
	}
	//�����ǽṹԪ�صĶԳƼ��Բ����ĸ�ʴ,�˴�tempMask���m_mask�ĶԳƼ�
	int *tempMask=new int[TempW*TempH];
	for(k=0;k<TempH;k++){
		for(l=0;l<TempW;l++){
			tempMask[k*TempW+l]=TempBuf[(TempH-1-k)*TempW+TempW-1-l];
		}
	}
	int flag;	//��־����
	//���ͼ����ÿһ������λ�ã��ж��Ƿ�ṹԪ��������Ŀ���ڲ�
	for(i=TempH/2;i<imgHeight-TempH/2;i++)
	{
		for(j=TempW/2;j<imgWidth-TempW/2;j++)
		{			
			//�жϽṹԪ���Ƿ�����ڵ�ǰ������Ŀ���ڲ�,1Ϊ����,0Ϊ������
			flag=1;
			for(k=-TempH/2;k<=TempH/2;k++)
			{
				for(l=-TempW/2;l<=TempW/2;l++)
				{
					//�����ǰ�ṹԪ��λ��Ϊ1,�ж����Ӧͼ���ϵ����ص��Ƿ�Ϊ��0
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

	//�ýṹԪ�ضԳƼ���Ŀ�겹����ʴ��,��Ҫ�Խ������һ�β���,�������ͽ�����
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			*(imgBufOut+i*lineByte+j)=255-*(imgBufOut+i*lineByte+j);
		}
	}

	//�ָ�ԭʼ����
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			*(imgBufIn+i*lineByte+j)=255-*(imgBufIn+i*lineByte+j);
		}
	}
	
	delete []tempMask;
}



/***********************************************************************
* �������ƣ�Open()
* ������������
* ����ֵ��  ��
* ˵������ֵ�����㣬m_pImgDataΪ���룬m_pImgDataOutΪ���
***********************************************************************/
void Morphology::Open()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_TempBuf==NULL)
    	return;
	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth+3)/4*4;
	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];
	ImgErosion(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_TempBuf, TempW, TempH);	//�ȸ�ʴ
	ImgDilation(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_TempBuf, TempW, TempH);	//������
	delete []buf;	//�ͷŻ�����

}


/***********************************************************************
* �������ƣ�Close()
* ������������
* ����ֵ��  ��
* ˵������ֵ�����㣬m_pImgDataΪ���룬m_pImgDataOutΪ���
***********************************************************************/
void Morphology::Close()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_TempBuf==NULL)
		return;
	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth+3)/4*4;
	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];
	//������
	ImgDilation(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_TempBuf, TempW, TempH);

	//��ʴ
	ImgErosion(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_TempBuf, TempW, TempH);

	//�ͷŻ�����
	delete []buf;
}


/***********************************************************************
* �������ƣ�DefineElementPair()
* ������������
* ����ֵ��  ��
* ˵����Ϊ���л����б任�ĽṹԪ�ض�m_hitMissMask��ֵ����8��
***********************************************************************/
void Morphology::DefineElementPair()
{
	//����8������Ļ��л����б任�ṹԪ�ض�
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
* �������ƣ�
* HitAndMiss()
*
*����������
*  unsigned char *imgBufIn   -���任������ͼ��
*  unsigned char *imgBufOut   -���ͼ��
*  int imgWidth   -ͼ���
*  int imgHeight   -ͼ���
*  struct ElementPair hitMissMask  -���л����нṹԪ�ض�
*
*����ֵ��
*   ��
*
*˵�������л����б任,0������,255����Ŀ��
***********************************************************************/
void Morphology::HitAndMiss(unsigned char *imgBufIn, unsigned char *imgBufOut,
							int imgWidth,int imgHeight, ElementPair hitMissMask)						 
{
	//ѭ������
	int i, j;
	
	//��־������1��ʾ�ṹԪ�ض���Ч��0��ʾ��Ч
	int validateFlag=1;
	
	//���ṹԪ�ض��Ƿ񽻼�Ϊ��,��������Ϊ����Ϊ��Ч�ṹԪ�ض�,�㷨���˳�
	for(i=0; i<3;i++){
		for(j=0;j<3;j++){
			if(hitMissMask.hitElement[i*3+j]&&hitMissMask.missElement[i*3+j]){
				validateFlag=0;
				break;
			}
		}
	}
	
	//�Ƿ��ṹԪ�ض�,����
	if(validateFlag==0)
		return;
	
	//ͼ��ÿ��������ռ�ֽ���
	int lineByte=(imgWidth+3)/4*4;
	
	//���ͼ����0
	memset(imgBufOut, 0, lineByte*imgHeight);
	
	//ѭ������
	int k,l;
	
	//���б�־�����ͻ����б�־����
	int hitFlag, missFlag;
	
	for(i=1; i<imgHeight-1; i++){
		for(j=1;j<imgWidth-1; j++){
			hitFlag=1;
			missFlag=1;
			for(k=-1;k<=1;k++){
				for(l=-1;l<=1;l++){
					//������нṹԪ�ص�ǰλ��Ϊ1
					if(hitMissMask.hitElement[(k+1)*3+l+1]==1){
						//�ж�ͼ���Ӧ���Ƿ�Ϊ0,�����,��û�л���ͼ��ǰ��
						if(!*(imgBufIn+(i+k)*lineByte+j+l))
							hitFlag=0;
					}
					
					//��������нṹԪ�ص�ǰλ��Ϊ1
					if(hitMissMask.missElement[(k+1)*3+l+1]==1){
						//�ж�ͼ���Ӧ���Ƿ�Ϊ0,�����,��û�л���ͼ��ǰ��
						if(*(imgBufIn+(i+k)*lineByte+j+l))
							missFlag=0;
					}
				}
			}
			
			//�������255
			if(hitFlag&&missFlag)
				*(imgBufOut+i*lineByte+j)=255;
		}
	}
}

/***********************************************************************
* �������ƣ� ImgThinning()
* ���������� ��
* ����ֵ��   ��
*˵�����û��л����б任������ͼ�����ϸ��
***********************************************************************/
void Morphology::ImgThinning()
{
	//����8������ĽṹԪ�ض�
	DefineElementPair();
	
	
	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth+3)/4*4;
	
	//���뻺����������м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];
	memcpy(buf, m_pImgData, lineByte*m_imgHeight);
	
	//ѭ������
	int i, k, l;
	
	//thinFlag������¼��ǰһ��ѭ���������Ƿ���ϸ�����ĵ�,���Ϊ0����û
	//�п���ϸ���ĵ����,ϸ����ֹ	
	int thinFlag=1;
	while(thinFlag){
		thinFlag=0;
		//��8������ĽṹԪ�ضԶ���ͼ����һ�λ��л����б任,����������
		//��ԭͼ��ȥ��
		for(i=0;i<8;i++){
			HitAndMiss(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,m_hitMissTemp[i]);
			//�����л����б任����������ԭ������ȥ��
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
	
	//�����ϸ���Ľ��������m_pImgDataOut
	memcpy(m_pImgDataOut, buf,  lineByte*m_imgHeight);
	
	//�ͷŻ�����
	delete []buf;
}


