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
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}

FaceDetect::FaceDetect(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					 unsigned char *pImgData1,unsigned char *pImgData2):
ImageDib(size, nBitCount, lpColorTable, pImgData1)
{

    m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0

	m_pImgDataIn1=pImgData1;//m_pImgData;
	m_pImgDataIn2=pImgData2;
	//��ɫģ�Ͳ�������
	Cb_Mean=117.4361;                 //�����ɵ� ���ݲ�ͬ��ͼ��
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

void FaceDetect::CalMedFl(double **s, int w, int h, int n)      //��ֵ�˲�ƽ��
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
* �������ƣ�CalSim()
* ������������
* ����ֵ��  ��
* ˵���� ���ƶȼ���
***********************************************************************/
void FaceDetect::CalSim()         
{
	//���Ҷ�ͼ��,�򷵻�
	if(m_nBitCount==8) return;

	//�ͷžɵ����ͼ�����ݼ���ɫ������
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
	int i,j,k;
	char c_t1[4];

	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByteOut+j)=255;  //ȫ��
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
            //��RGBɫ�ʿռ�ӳ�䵽YCbCrɫ�ʿռ�
			double Cb=(128-37.797*C_r/255-74.203*C_g/255+112*C_b/255);
			double Cr=(128+112*C_r/255-93.786*C_g/255-18.214*C_b/255);
			//���÷�ɫģ�ͣ������ƶȾ���
			double tt =(Cb-Cb_Mean)*((Cb-Cb_Mean)*Cov11-(Cr-Cr_Mean)*Cov10)+(Cr-Cr_Mean)*(-(Cb-Cb_Mean)*Cov01+(Cr-Cr_Mean)*Cov00);
			tt =(-0.5*tt)/(Cov00*Cov11-Cov01*Cov10);
			m_pSimArray[i][j]=exp(tt);
		}
	}

	CalMedFl(m_pSimArray,m_imgWidth, m_imgHeight,9);  //��ֵ�˲�
	//�����ƶȾ����е����ֵ�����ƶȾ�����й�һ��
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
* �������ƣ�CalSBound
* ����������int top,int bottom,int left,int right
* ����ֵ��  ��
* ˵���� ����������ȡ
***********************************************************************/

void FaceDetect::CalSBound(int top,int bottom,int left,int right)
{

	//���Ҷ�ͼ��,�򷵻�
	if(m_nBitCount==8) return;

	//�ͷžɵ����ͼ�����ݼ���ɫ������
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
			//��һ��
			temp = *(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn)+
			       *(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+1)+
				   *(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+2);
			r = (double)*(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+2)/temp;
			g = (double)*(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+1)/temp;
			//ͨ����ɫ�������λ��
			if(g<0.398 && g > 0.246 && r<0.664 && r>0.233 && r>g && g>=0.5*(1-r)){
				*(m_pImgDataOut+i*lineByteOut+j)=255;              
			}
			else	*(m_pImgDataOut+i*lineByteOut+j)=0;
	}
	
	//��˹ ������˹��Ե����
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
			*(m_pImgDataOut+i*lineByteOut+j)=255; //���ص���Ϊȫ��
		}
	}

	for(i=top+1; i<bottom-1; i++)
		for(j=left+1; j<right-1; j++){
			int positive = 0;   
			int negtive  = 0;

			//3*3���Ӵ��� �������������ص���Ϊ��ɫ �����������
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
	//���Ҷ�ͼ��,�򷵻�
	if(m_nBitCount==8) return;

	//�ͷžɵ����ͼ�����ݼ���ɫ������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//ÿ����λ��Ϊ24����
	m_nBitCountOut=24;

	//����ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
	int pixelByteIn=3;
    //���ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
    int pixelByteOut=3;
	
	//����ͼ��ÿ��������ռ�ֽ���,������4�ı���
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;
	//���ͼ��ÿ��������ռ�ֽ���,������4�ı���
    int lineByteOut=(m_imgWidth*pixelByteOut+3)/4*4;

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;


	//�������ͼ��λͼ���ݻ�����
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	//ѭ������,ͼ�������
	int i,j;
    for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByteOut+j*3+0)=255;
			*(m_pImgDataOut+i*lineByteOut+j*3+1)=255;
			*(m_pImgDataOut+i*lineByteOut+j*3+2)=255;//���ص���Ϊȫ��
		}
	}
    
	//��λ��
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
         *v = max;               // v��0��1֮��
         delta = max - min;
         if( max != 0 )
            *s = delta / max;       // s��0��1֮��
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
            *h += 360;                  //h��0��360֮��
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





//��ͳ̬ͬ�˲�
void FaceDetect::Homomorphicfilter()
{
	    //���Ҷ�ͼ��,�򷵻�
	    if(m_nBitCount==8) 
			return;
   
        //�ͷžɵ����ͼ�񻺳���
     	if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;

		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
	

		//�Ҷ�ͼ�����
/*		m_nBitCountOut=8;

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
		
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

        //���ͼ��Ŀ��,������ͼ�����
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;

        m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
        
		m_nBitCountOut1=8;
        int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;


        //����ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
		int m_nBitCount=24;  
		//����ͼ��ÿ��������ռ�ֽ���,������4�ı���
		int lineByteIn=(m_imgWidth*m_nBitCount/8+3)/4*4;*/
        
        
	
        //��ɫͼ�����
		m_nBitCountOut=24;
		int pixelByteOut=3;
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

        //���ͼ��Ŀ��,������ͼ�����
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;

        m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

		m_nBitCountOut1=8;
        m_nBitCountOut2=8;
		
		//���ͼ��ÿ��������ռ�ֽ���,������4�ı���
        int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;
		int lineByteOut2=(m_imgWidth*m_nBitCountOut2/8+3)/4*4;

		//����ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
		int m_nBitCount=24;  
		//����ͼ��ÿ��������ռ�ֽ���,������4�ı���
		int lineByteIn=(m_imgWidth*m_nBitCount/8+3)/4*4;
		
		//�������ͼ��λͼ���ݻ�����	
        m_pImgDataOut1=new unsigned char[lineByteOut1*m_imgHeight];


	    
		
		//ѭ������,ͼ�������
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
				B=b/255.0;  //����ת������
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
	
	
      	// ����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2���������ݣ�
	    int nTransWidth ;
	    int nTransHeight;
	
	    // ������и���Ҷ�任�ĵ���	��2���������ݣ�
	    dTmpOne = log(m_imgWidth)/log(2);
    	dTmpTwo = ceil(dTmpOne)		   ;
    	dTmpTwo = pow(2,dTmpTwo)	   ;
    	nTransWidth = (int) dTmpTwo	   ;
	
    	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
    	dTmpOne = log(m_imgHeight)/log(2);
    	dTmpTwo = ceil(dTmpOne)		   ;
    	dTmpTwo = pow(2,dTmpTwo)	   ;
    	nTransHeight = (int) dTmpTwo   ;
	
    	//�˲��İ뾶���ܳ���Ƶ������뾶
    	double D0=10;
    	if(D0 > nTransWidth-1 || D0 > nTransHeight-1)
		{
	    	return ;
		}
	    
		//�������ͼ�񻺳���������ʼ��Ϊ0
	    m_pImgDataOut2=new unsigned char[m_imgWidthOut*m_imgHeightOut];
        memset(m_pImgDataOut2, 0, m_imgWidthOut*m_imgHeightOut);
	    //���븵��Ҷ������������ʼ��Ϊ0
     	FFtTrans.m_pFFTBuf=new ComplexNumber[m_imgWidthOut*m_imgHeightOut];
    	memset(FFtTrans.m_pFFTBuf, 0, sizeof(ComplexNumber)*m_imgWidthOut*m_imgHeightOut);	
	
	    //����ͼ�����ݽ��ж�ά����Ҷ�任
        FFtTrans.ImageFFt2D(m_pImgDataOut1, m_imgWidthOut, m_imgHeightOut,m_pImgDataOut2);//m_pImgDataOut2�����Ƶ��ͼ��
       

        double Rh=2;
        double Rl=0.5;
        double c=1.4;
		double tha,spec;

	    // ���濪ʼʵʩButterWorth��ͨ�˲�
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

	    // ������ͨ�˲���ͼ����з��任
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
    //���Ҷ�ͼ��,�򷵻�
	if(m_nBitCount==8) return;

	//�ͷžɵ����ͼ�����ݼ���ɫ������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//�ͷžɵ����ͼ�����ݼ���ɫ������
	if(m_pImgDataOut1!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut1!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}


	//��ֵ����,ÿ����λ��Ϊ8����
	m_nBitCountOut=8;
    m_nBitCountOut1=8;

	//��ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
    m_nColorTableLengthOut1=ComputeColorTabalLength(m_nBitCountOut1);


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
    
	//������ɫ������,���ɻҶ�ͼ�����ɫ��
	if(m_nColorTableLengthOut1!=0){
    	m_lpColorTableOut1=new RGBQUAD[m_nColorTableLengthOut1];
		for(int i=0; i<m_nColorTableLengthOut1;i++){
			m_lpColorTableOut1[i].rgbBlue=i;
			m_lpColorTableOut1[i].rgbGreen=i;
			m_lpColorTableOut1[i].rgbRed=i;
			m_lpColorTableOut1[i].rgbReserved=0;
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
	int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;


	//�������ͼ��λͼ���ݻ�����
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	m_pImgDataOut1=new unsigned char[lineByteOut1*m_imgHeight];

    memset(m_pImgDataOut, 255, m_imgWidthOut*m_imgHeightOut);

	
    //ͨ����������ͼ�� ��ȡ����ͼ��������λ��ı߽�ֵ
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
 	//���ݻ�ֵ����ʽΪ���ͼ��ֵ
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
    
    int C=7,N=9;//��Ҫ����һϵ�в��� ʹ��ʶ��Ч���ﵽ���
	int nLocAvg ;	// ��ͼ���ƽ��ֵ
	int max,min;
	double r,g,temp;
	// ��ͼ�����ɨ�裺	
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
			
			//��һ��
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
	    //���Ҷ�ͼ��,�򷵻�
	    if(m_nBitCount==8) 
			return;
   
        //�ͷžɵ����ͼ�񻺳���
     	if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;

		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}

		
	

		//�Ҷ�ͼ�����
/*		m_nBitCountOut=8;

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
		
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

        //���ͼ��Ŀ��,������ͼ�����
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;

        m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
        
		m_nBitCountOut1=8;
        int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;


        //����ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
		int m_nBitCount=24;  
		//����ͼ��ÿ��������ռ�ֽ���,������4�ı���
		int lineByteIn=(m_imgWidth*m_nBitCount/8+3)/4*4;*/
        
        
	
        //��ɫͼ�����
		m_nBitCountOut=24;
		int pixelByteOut=3;
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

        //���ͼ��Ŀ��,������ͼ�����
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;

        m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

		m_nBitCountOut1=8;
		//��ɫ����
	    m_nColorTableLengthOut1=ComputeColorTabalLength(m_nBitCountOut1);
	    //������ɫ������,���ɻҶ�ͼ�����ɫ��
	    if(m_nColorTableLengthOut1!=0){
    	   m_lpColorTableOut1=new RGBQUAD[m_nColorTableLengthOut1];
		   for(int i=0; i<m_nColorTableLengthOut1;i++){
			m_lpColorTableOut1[i].rgbBlue=i;
			m_lpColorTableOut1[i].rgbGreen=i;
			m_lpColorTableOut1[i].rgbRed=i;
			m_lpColorTableOut1[i].rgbReserved=0;
		   }
		}		
		//���ͼ��ÿ��������ռ�ֽ���,������4�ı���
        int lineByteOut1=(m_imgWidth*m_nBitCountOut1/8+3)/4*4;
		
    	//�������ͼ��λͼ���ݻ�����	
        m_pImgDataOut1=new unsigned char[lineByteOut1*m_imgHeight];

		//����ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
		int m_nBitCount=24;  
		//����ͼ��ÿ��������ռ�ֽ���,������4�ı���
		int lineByteIn=(m_imgWidth*m_nBitCount/8+3)/4*4;
		
	


	    
		
		//ѭ������,ͼ�������
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
				B=b/255.0;  //����ת������
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
	    //����ͼ�����ݽ���С���任  

	    //������ͨ�˲���ͼ����з��任

	   
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

