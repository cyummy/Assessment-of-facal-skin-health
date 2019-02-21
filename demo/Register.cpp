#include "stdafx.h"
#include "Register.h"
#include "math.h"

/***********************************************************************
* �������ƣ�
* Register()
*
*˵�����޲����Ĺ��캯�����Գ�Ա�������г�ʼ��
***********************************************************************/
Register::Register()
{
	//����ͼ��1��صĳ�Ա��������ֵ
	m_nBitCount1=0;
	m_pImgData1=NULL;
	m_lpColorTable1=NULL;
	m_nColorTableLength1=0;
	m_imgWidth1=0;
	m_imgHeight1=0;
	
	//����ͼ��2��صĳ�Ա��������ֵ
	m_nBitCount2=0;
	m_pImgData2=NULL;
	m_lpColorTable2=NULL;
	m_nColorTableLength2=0;
	m_imgWidth2=0;
	m_imgHeight2=0;
	
	//���ͼ����صĳ�Ա��������ֵ
	m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_lpColorTableOut=NULL;
	m_nColorTableLengthOut=0;
	m_imgWidthOut=0;
	m_imgHeightOut=0;
	m_space=10;
}

/***********************************************************************
* �������ƣ�
*   ~Register()
*
*˵���������������ͷ���Դ
***********************************************************************/
Register::~Register()
{
	//�ͷ�ͼ��1�Ļ�����
	if(m_pImgData1!=NULL){
		delete []m_pImgData1;
		m_pImgData1=NULL;
	}
	if(m_lpColorTable1==NULL){
		delete []m_lpColorTable1;
		m_lpColorTable1=NULL;
	}
	
	//�ͷ�ͼ��2�Ļ�����
	if(m_pImgData2!=NULL){
		delete []m_pImgData2;
		m_pImgData2=NULL;
	}
	if(m_lpColorTable2==NULL){
		delete []m_lpColorTable2;
		m_lpColorTable2=NULL;
	}
	
	//�ͷ����ͼ��Ļ�����
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
* �������ƣ�
* GetDimensions()
*
*����������
*  ��
*
*����ֵ��
*   ͼ��ĳߴ磬��CSize���ͱ��
*
*˵�����������ͼ��Ŀ�͸�
***********************************************************************/
CSize Register::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* �������ƣ�
* InputImg1()
*
*����������
*  CSize size -����ͼ���С�����ߣ�
*  int nBitCount  -ÿ����λ��
*  LPRGBQUAD lpColorTable  -��ɫ��ָ��
*  unsigned char *pImgData  -λͼ����ָ��
*
*����ֵ��
*   ��
*
*˵��������ͼ�����ݣ���Ϊ�μ���׼�ĵ�һ��ͼ�������ݳ�Աm_pImgData1ָ��
***********************************************************************/
void Register::InputImg1(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
						 unsigned char *pImgData)
{
	//���������
	if(m_pImgData1!=NULL){
		delete []m_pImgData1;
		m_pImgData1=NULL;
	}
	if(m_lpColorTable1==NULL){
		delete []m_lpColorTable1;
		m_lpColorTable1=NULL;
	}
	
	//Ϊͼ��1��صĳ�Ա������ֵ
	m_nBitCount1=nBitCount;
	m_nColorTableLength1=ComputeColorTabalLength(nBitCount);
	
	//��������ͼ��Ĵ�С����Ϊͼ��1������ʴ�С�Ŀռ�洢λͼ����
	m_imgWidth1=size.cx;
	m_imgHeight1=size.cy;
	int lineByte=(m_imgWidth1*nBitCount/8+3)/4*4;
	int imgBufSize=m_imgHeight1*lineByte;
	m_pImgData1=new BYTE[imgBufSize];
	memcpy(m_pImgData1, pImgData, imgBufSize);
	
	//�������ɫ��������ɫ������Ҫ�Ŀռ䣬��������ɫ��
	if(m_nColorTableLength1!=0){
		m_lpColorTable1= new RGBQUAD[m_nColorTableLength1];
		memcpy(m_lpColorTable1,lpColorTable,
			   sizeof(RGBQUAD) * m_nColorTableLength1);
	}
}


/***********************************************************************
* �������ƣ�
* InputImg2()
*
*����������
*  CSize size -����ͼ���С�����ߣ�
*  int nBitCount  -ÿ����λ��
*  LPRGBQUAD lpColorTable  -��ɫ��ָ��
*  unsigned char *pImgData  -λͼ����ָ��
*
*����ֵ��
*   ��
*
*˵��������ͼ�����ݣ���Ϊ�μ���׼�ĵڶ���ͼ�������ݳ�Աm_pImgData2ָ��
***********************************************************************/
void Register::InputImg2(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData)
{
	//���������
	if(m_pImgData2!=NULL){
		delete []m_pImgData2;
		m_pImgData2=NULL;
	}
	if(m_lpColorTable2==NULL){
		delete []m_lpColorTable2;
		m_lpColorTable2=NULL;
	}
	
	//Ϊͼ��2��صĳ�Ա������ֵ
	m_nBitCount2=nBitCount;
	m_nColorTableLength2=ComputeColorTabalLength(nBitCount);
	
	//��������ͼ��Ĵ�С����Ϊͼ��2������ʴ�С�Ŀռ�洢λͼ����
	m_imgWidth2=size.cx;
	m_imgHeight2=size.cy;
	int lineByte=(m_imgWidth2*nBitCount/8+3)/4*4;
	int imgBufSize=m_imgHeight2*lineByte;
	m_pImgData2=new BYTE[imgBufSize];
	memcpy(m_pImgData2, pImgData, imgBufSize);
	
	//�������ɫ��������ɫ������Ҫ�Ŀռ䣬��������ɫ��
	if(m_nColorTableLength2!=0){
		m_lpColorTable2= new RGBQUAD[m_nColorTableLength2];
		memcpy(m_lpColorTable2,lpColorTable,sizeof(RGBQUAD) * m_nColorTableLength2);
	}
}


/***********************************************************************
* �������ƣ�
* RegisterBasedSpacial()
*
*����������
*  unsigned char *imgBuf  -�ο�ͼ(����ͼ)λͼ����
*  int width  -�ο�ͼ��,����Ϊ��λ
, int height  -�ο�ͼ��,����Ϊ��λ
*  unsigned char * blockBuf  -��׼ͼ(ģ��)λͼ����
*  int blockW  -��׼ͼ��,����Ϊ��λ
*  int blockH  -��׼ͼ��,����Ϊ��λ
*
*����ֵ��
*   ��׼ͼ���½��ڲο�ͼ�е�λ������,��CPoint���ͱ��
*
*˵���������ο�ͼ�ͻ�׼ͼ���ݣ�����ģ��ƥ�䣬�����ػ�׼ͼ���½��ڲο�ͼ��
*      ��λ������,�ú���ֻ�ԻҶ�ͼ����Ч
***********************************************************************/
CPoint Register::RegisterBasedSpacial(unsigned char *imgBuf, int width, int height, 
								unsigned char * blockBuf, int blockW, int blockH)
{
	//ͼ��ÿ�����ص��ֽ���
	int lineByte=(width+3)/4*4;
	
	//��׼ͼ��ÿ���ֽ���
	int blockLineByte=(blockW+3)/4*4;
	
	//ѭ������
	int i, j, i0, j0;
	
	//�м������ÿ����һ��λ�ü�¼��λ���ϵ�����Բ�ȣ��������ö�Ӧ���ز��
	//����ֵ�������Ϊ����Բ�ȣ�����sumֵ��С��λ��Ϊ���ƥ��λ��
	int sum;
	
	//��min��¼������Сsum��ֵ, ��������Сsum��λ��
	int min, minX, minY;
	
	//�м����
	int t1, t2;
	
    //Ϊ�˽�ʡʱ�䣬�˴��з�����ݷ�����������Ϊ3�����߿ɸ�����Ҫ�Լ���������
	for(i=0;i<height-blockH;i+=3){
		for(j=0;j<width-blockW; j+=3){
			sum=0;
			
			//����ƥ�䴰���ڵ����أ�(i0,j0)�Ĳ���Ҳ�������������Ҫ����
			for(i0=0;i0<blockH;i0++){
				for(j0=0;j0<blockW;j0++){
					//��Ӧ����������������ľ���ֵ֮��
					t1=*(imgBuf+(i+i0)*lineByte+j+j0);
					t2=*(blockBuf+i0*blockLineByte+j0);
					sum += abs(t1-t2);				
				}
			}
			
			//��һ���������򣬴�ʱҲ����Сsumֵ��λ��
			if(i==0&&j==0){
				min=sum;
				minX=0;
				minY=0;
			}
			else{//�Ѿ����ǵ�һ������λ����
				if(min>sum){//����һ���µ���Сλ�ã���¼��ʱ����Сֵ����λ������
					min=sum;
					minX=j;
					minY=i;
				}
			}
			
		}
	}
	
	//����ƥ��λ��
	CPoint pos(minX,minY);
	return pos;
}


/***********************************************************************
* �������ƣ�
* relativityRegistering()
*
*����������
*  ��
*
*����ֵ��
*   1Ϊ�ɹ�,0Ϊʧ��
*
*˵������m_pImgData1��ָ��������Ϊ�ο�ͼ��m_pImgData2��ָ��������Ϊ��׼ͼ��
*      ����ģ��ƥ�䣬Ϊm_pImgDataOut����ռ䣬����������m_pImgDataOut�С�
***********************************************************************/
BOOL Register::relativityRegistering()
{
	//��׼ͼ��ο�ͼ��ʽ��ͳһ
	if(m_nBitCount1!=m_nBitCount2){	
		return FALSE;//����
	}

	//ƥ��λ�ñ���,����ģ��ƥ�亯��,��û�׼ͼ���½��ڲο�ͼ�е�λ��
	CPoint macthPos(0,0);
	macthPos=RegisterBasedSpacial(m_pImgData1, m_imgWidth1, m_imgHeight1, 
		m_pImgData2, m_imgWidth2, m_imgHeight2);
	
	//��ƥ���������ͼ������ʾ
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	
	//���ͼ��������ͼ��Ϊͬһ����,�˴�Ϊ�Ҷ�����
	m_nBitCountOut=m_nBitCount1;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	if(m_nColorTableLengthOut!=0){
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable1,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	
	//���ͼ��Ŀ��߼�ÿ�������ֽ�������ο�ͼ��С��ͬ
	m_imgWidthOut=m_imgWidth1;
	m_imgHeightOut=m_imgHeight1;
	int lineByte=(m_imgWidth1*m_nBitCount1/8+3)/4*4;

	//Ϊ���ͼ�����ռ䣬�����ο�ͼ������m_pImgDataOut
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeightOut];
	memcpy( m_pImgDataOut,m_pImgData1, lineByte*m_imgHeightOut);
	
	//����׼ͼ��λ���ú�ɫ���ǳ���������ʾƥ��������ȷ��
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
* �������ƣ�
* conv2()
*
*����������
*  float *Src  -Ҫ���������
*  int width  -�ο�ͼ��,����Ϊ��λ
, int height  -�ο�ͼ��,����Ϊ��λ
*  float *mark  -(ģ��)����
*  int markWidth  -ģ���,����Ϊ��λ
*  int markHeight  -ģ���,����Ϊ��λ
*����ֵ��
*   ��
*
*˵������markģ�������Src���о���������������Src��
***********************************************************************/
void Register::conv2(float *Src, int width, int height, float *mark,
					 int markWidth , int markHeight )
{
	int i,j,m,n;
	//tempΪ��ʱ���飬������ԭͼ��ֱ�Ӳ���
	float *temp = new float [width*height];
	memset( temp, 0 , sizeof(float)*width*height);
	float sum;
	int lineByte=(width+3)/4*4;
	
	/////�������Ե��������
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
* �������ƣ�
* ArraydotMultiArray()
*
*����������
*  float * src1 �� Դ����1
*  float * src2    Դ����2
, 
* float * dest  �������
* int width   ����Ŀ�
* int height  ����ĸ�
*����ֵ��
*   ��
*
*˵���������˺�������dest[i]=src1[i]*src2[i]
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
* �������ƣ�
* LocalMaximum()
*
*����������
*  float * src  ���������ֵ�ľ���
* int width   ����Ŀ�
* int height  ����ĸ�
* int posx , Ҫ�жϵĵ��������
* int posy , Ҫ�жϵĵ��������
* int neiboursize , ����Ĵ�С
*����ֵ��
*  bool�ͣ����Ǿֲ���󣬷����棬���򷵻ؼ�
*
*˵�����жϵ�(posx,posy)������ֵ���Ƿ�Ϊ�ֲ�����ֵ
***********************************************************************/
bool Register::LocalMaximum(float * src , int width , int height ,
							int posx , int posy , int neiboursize)
{
	int i,j;
	int lineByte=(width+3)/4*4;
	//�ж��Ƿ�Ϊ�ֲ����
	for ( i = -neiboursize/2 ; i <= neiboursize/2 ; i++)
		for ( j = -neiboursize/2 ; j <= neiboursize/2 ; j++)
		{
			if ( src[(posy + j) * lineByte + posx+i] > src[posy * lineByte + posx])
				return 0;
		}
		return 1;
}

/***********************************************************************
* �������ƣ�
* SpaceSuppress()
*
*����������
* ptArray * pCornerpos ,���������ľ���
*����ֵ��
*  ��
*
*˵������pCornerpos�����С�ھ�����ֵ�ĵ���޳�
***********************************************************************/
void Register::SpaceSuppress(ptArray * pCornerpos)
{
	int i,j;
	int length;
	//length������ĸ���
	length = pCornerpos->GetSize();
	CPoint pt1,pt2;
	//distancetable ���������������������
	double * distacetable = new double [length * length];
	//��ʶ���ʱС�ھ�����ֵ
	bool * marktable = new bool [length * length];
	memset( distacetable , 0 , sizeof(double) * length * length);
	memset( marktable , 0 , sizeof(bool) * length * length);
	
	for ( i = 0 ; i < length-1 ; i++) 
	{
		pt1 = pCornerpos->GetAt(i);
		for ( j = i+1 ; j < length ; j++)
		{
			pt2 = pCornerpos->GetAt(j);
			//��ȡ����
			distacetable[j * length + i] = sqrt(pow(pt1.x - pt2.x , 2)
				+ pow(pt1.y - pt2.y , 2));
			if ( distacetable[j* length + i] < m_space)
			{
				//��ʶ����С����ֵ
				marktable[j * length + i] = 1;
			}
		}
	}
	//remove_num��ʾȥ���ĵ���
	int temp, remove_num=0;
	for ( i= 0 ; i < length-1 ; i++)
	{
		temp = 0;
		//�ж��Ƿ����������С����ֵ�ĵ�
		for ( j = 0 ; j < length-1 ; j++)
		{
			temp += marktable[i * length + j];
		}
		if ( temp != 0)
		{ 
			//���������������޳��õ�
			pCornerpos->RemoveAt( i - remove_num );
			remove_num++;
		}			
	}
	
	delete []distacetable;
	delete []marktable;
}
/***********************************************************************
* �������ƣ�
* HarisConnerDetect()
*
*����������
* unsigned char *imgBufIn, ����ȡ�ǵ��ͼ������
* int width, ͼ��Ŀ�
* int height, ͼ��ĸ�
* ptArray * pCornerpos ,���������ľ���
*����ֵ��
*  ��
*
*˵������ȡͼ��imgBfuIn�Ľǵ㣬����ھ���pCornerpos��
***********************************************************************/
void Register::HarisConnerDetect(unsigned char *imgBufIn, int width, 
								 int height, ptArray * pCornerpos)
{
	int imWidth = width;
	int imHeight = height;
	//ͼ��ģ�����ģ��
	float blur[7] = {1, 6 , 15 , 20 , 15 , 6 , 1}; 
	//�˲�ģ��
	float prefilt[3] = { 0.223755 , 0.552490 , 0.223755 };
	//��ͼ����ģ��
	float derivfilt[3] = { -0.453014 , 0 , 0.453014 };
	int lineByte=(imWidth*m_nBitCount1/8+3)/4*4;
	
	int i,j;
	////ģ���һ����ȡ��ֵ��
	float sum=0;
	for ( i = 0 ; i < 7 ; i++ )
		sum += blur[i];
	for ( i = 0 ; i < 7 ; i++ )
		blur[i] /= sum;
    
	//fx ͼ���һ��x������ �� fy ͼ���һ��y������
	float * fx = new float [lineByte * imHeight];
	float * fy = new float [lineByte * imHeight];
	//fx2ÿ������ֵΪfx��Ӧ����ֵ��ƽ��
	float * fx2 = new float [lineByte * imHeight];
	//fy2ÿ������ֵΪfy��Ӧ����ֵ��ƽ��
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
	//////����fx��fy��fx2��fy2��fxy
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
	//detOfHessianΪÿ�����ص㴦����������
	float * detOfHessian = new float [lineByte * imHeight];
	float * norm_array = new float [lineByte * imHeight];
	//qualityOfcornerΪÿ�����ص�������Ʒ�����أ�ֵԽ��Խ�����ǽǵ�
	float * qualityOfcorner = new float [lineByte * imHeight];
	
	float maxquality = 0;
	//threshΪ��ֵϵ�������������Ʒ�����ؼ�Ϊ��ֵ
	float thresh = 0.05;  // 0.005;  
	//����߽�С��border�����ز�����
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
			
			//�߽�㲻�迼��
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
			///����������ֵ
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
	
	//thqΪ��ֵ
	float thq = thresh * maxquality ;
	//supneiborΪȡ��������Χ
	int supneibor = 20;
	//��ֲ����
	for ( i = supneibor/2 ; i < imWidth-supneibor/2 ; i++)
	{
		for ( j = supneibor/2 ; j < imHeight-supneibor/2 ; j++)
		{
			if ( qualityOfcorner[j * lineByte + i] > thq && 
				LocalMaximum(qualityOfcorner, imWidth, imHeight,i, j,supneibor)) 
			{
				//���㲻�ڱ߽緶Χ�;ֲ����������������Ϊ�ǽǵ�
				pCornerpos->Add(CPoint(i,j));
			}
		}
	}
	delete[] qualityOfcorner;
	//��һ�����ݾ���������ƣ����ǵ���಻��̫��
	SpaceSuppress(pCornerpos);
}

/***********************************************************************
* �������ƣ�
* DrawCross()
*
*����������
* unsigned char *imgBufIn, ����ȡ�ǵ��ͼ������
* int width, ͼ��Ŀ�
* int height, ͼ��ĸ�
* ptArray * pCornerpos ,���������ľ���
*����ֵ��
*  ��
*
*˵������ͼ����������λ�û�ʮ�֣�ʮ�ֵĿ�Ϊ5����Ϊ5
***********************************************************************/
void Register::DrawCross(unsigned char * imgBufIn , int imWidth , 
						 int imHeight , ptArray * pCornerpos )
{
	int NumOfCorner = pCornerpos->GetSize();
	CPoint pt;
	for ( int m = 0 ; m< NumOfCorner ; m++)
	{
		//���������
		pt=pCornerpos->GetAt(m);
		
		int x,y;
		int i,j;
		x = pt.x;
		y = pt.y;
		int lineByte=(imWidth+3)/4*4;
		//��ʮ�֣�������ֵ��Ϊ255
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
* �������ƣ�
* HarisConner()
*
*����������
* ��
*����ֵ��
*  ��
*
*˵��������HarisConnerDetect()�������ͼ��1�Ľǵ㣬��DrawCross����ʮ�֣�
*      ��������m_pImgDataOut�У�����Ľӿ�
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
	//���ͼ���λ��������ͼ����ͬ
	m_nBitCountOut=m_nBitCount1;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCount1);
	
    //���ͼ��Ŀ�͸ߣ�������ͼ����ͬ
	m_imgWidthOut=m_imgWidth1;
	m_imgHeightOut=m_imgHeight1;
	
	int lineByte=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	int imgBufSize=m_imgHeightOut*lineByte;
	m_pImgDataOut=new BYTE[imgBufSize];
	memcpy(m_pImgDataOut, m_pImgData1, imgBufSize);
	
	//�������ɫ��������ɫ������Ҫ�Ŀռ䣬��������ɫ��
	if(m_nColorTableLengthOut!=0){
		m_lpColorTableOut= new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable1, 
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	//�����ͼ���ϻ�ʮ��
	DrawCross(m_pImgDataOut,m_imgWidthOut,m_imgHeightOut,&m_cornerpos1);	
}

/***********************************************************************
* �������ƣ�
* RegisterBasedConner()
*
*����������
* unsigned char *imgBuf1, ͼ��1
* unsigned char *imgBuf2, ͼ��2
* int width, ͼ��Ŀ�
* int height, ͼ��ĸ�
* ptArray *pCornerpos1, ͼ��1������ȡ�Ľǵ� 
* ptArray *pCornerpos2 , ͼ��2������ȡ�Ľǵ�
* ptArray *pCornerMatch1 , ��׼��ͼ��1����ͼ��2��ƥ���ϵĽǵ�
* ptArray *pCornerMatch2 ,��׼��ͼ��2����ͼ��1��ƥ���ϵĽǵ�
*����ֵ��
*  ��
*
*˵������������ͼ�������ȡ�Ľǵ������׼
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
	//GΪ��ؾ���
	double * G = new double[cnt1*cnt2];
	double r;
	//������ؾ���
	for ( i = 0 ; i < cnt1 ; i++)
	{
		for ( j = 0 ; j < cnt2 ; j++)
		{
			//��ȡ�ǵ�
			x1 = pCornerpos1->GetAt(i).x;
			y1 = pCornerpos1->GetAt(i).y;
			x2 = pCornerpos2->GetAt(j).x;
			y2 = pCornerpos2->GetAt(j).y;
			//aver1��aver2Ϊ�ǵ��������صľ�ֵ
			aver1 = 0;
			aver2 = 0;
			//sigama1��sigama2Ϊ�ǵ��������صķ���
			sigama1 = 0;
			sigama2 = 0;
			temp=0;
			//��ȡ��ֵ
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
			//��ȡ����
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
	 //��������ֵ�ֽ�
	 dluav( G , cnt1 , cnt2 , U , V , eps , ka);
	 int length = min(cnt1,cnt2);
	 //���Խ����ϵ�ֵ��Ϊ1 
	 for ( i = 0 ; i < length ; i++ )
	 {
	   G[i*cnt2+i] = 1;
     }
	 //P�µ���ؾ���
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
		 //��ȡ�������ĵ����ڵ���
		 for ( j = 0 ; j < cnt2 ; j++)
		 {
			 if (P[i*cnt2+j] > maxrow)
			 {
				 maxrow = P[i*cnt2+j];
				 jmax = j ;
			 }
		 }
		 //��ȡjmax�����ĵ����ڵ���
		 for ( m = 0 ; m < cnt1 ; m++)
		 {
			 if (P[m*cnt2+jmax] > maxcol)
			 {
				 maxcol = P[m*cnt2+jmax];
				 imax = m;
			 }
		 }
		 //�ж��Ƿ��Ϊ�����������
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
* �������ƣ�
* ConnerRegistering()
*
*����������
*     1Ϊ�ɹ�,0Ϊʧ��
*
*����ֵ��
*  ��
*
*˵��������matchBasedConner()������ͼ��1��ͼ��2���нǵ�ƥ�� ��������׼���
*      ����ͼ�ϲ���һ��ͼ�У��������ǵ�ԵĶ�Ӧ��ϵ������Ľӿ�
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

	//ͼ��1��ȡ�ǵ�
	HarisConnerDetect(m_pImgData1,m_imgWidth1,m_imgHeight1,&m_cornerpos1);

	//ͼ2��ȡ�ǵ�
	HarisConnerDetect(m_pImgData2,m_imgWidth2,m_imgHeight2,&m_cornerpos2);

	//�ǵ�ƥ��
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
	
	//���ͼ���λ��������ͼ����ͬ
	m_nBitCountOut=m_nBitCount1;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCount1);
	
    
	int lineByte=(m_imgWidth1*m_nBitCount1/8+3)/4*4;
	int imgBufSize=m_imgHeight1*lineByte;
	
	//���ͼ��Ŀ�Ϊԭͼ������������ԭͼ����ͬ
	m_imgWidthOut=2*lineByte;
	m_imgHeightOut=m_imgHeight1;

	m_pImgDataOut=new BYTE[imgBufSize*2];
    
	unsigned char *tempbuf1 = new unsigned char[lineByte*m_imgHeight1];
	unsigned char *tempbuf2 = new unsigned char[lineByte*m_imgHeight2];
	
	memcpy(tempbuf1,m_pImgData1,lineByte*m_imgHeight1);
	memcpy(tempbuf2,m_pImgData2,lineByte*m_imgHeight1);

	//��ʮ��
	DrawCross(tempbuf1,m_imgWidth1,m_imgHeight1,&m_Matchpt1);
	DrawCross(tempbuf2,m_imgWidth2,m_imgHeight2,&m_Matchpt2);

	//��ƥ�������ͼ�ϲ�Ϊһ��ͼ����һ��ͼ����ߣ��ڶ���ͼ���ұ�
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

	//ƥ��ĵ��֮�仭��
	DrawLine(m_pImgDataOut,m_imgWidthOut,m_imgHeightOut,&m_Matchpt1,&m_Matchpt2);
    
	//�������ɫ��������ɫ������Ҫ�Ŀռ䣬��������ɫ��
	if(m_nColorTableLengthOut!=0){
		m_lpColorTableOut= new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable1, sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	delete[] tempbuf1;
	delete[] tempbuf2;
	return true;
}

/***********************************************************************
* �������ƣ�
* DrawLine()
*
*����������
* unsigned char *imgBufOut, ͼ������
* int widthOut, ͼ��Ŀ�
* int heightOut, ͼ��ĸ�
* ptArray *pcornerpos1, ��źϲ�ǰ�ĵ�һ��ͼ���ƥ���ϵĽǵ�, ����ԭͼ�е�����
* ptArray *pcornerpos2, ��źϲ�ǰ�ĵڶ���ͼ���ƥ���ϵĽǵ㣬����ԭͼ�е�����
*����ֵ��
*  ��
*
*˵����ƥ��ĵ��֮�仭�ߣ������imgBufOut�����
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
		//kΪֱ��б��
		k = 1.0*(pt2.y-pt1.y)/(1.0*(pt2.x-pt1.x));
		//���ߣ��������ϵ�����ֵ��Ϊ255
		for (int x = pt1.x ; x<=pt2.x ; x++)
		{
			int y = (int)(pt1.y+k*(x-pt1.x));
			imgBufOut[y*widthOut+x] = 255;
		}
	}
}

//ʵ�����������ֵ�ֽ�
//����Householder�任������QR�㷨
//a-����Ϊm*n�����飬����ʱ��Խ������θ�������ֵ������Ԫ��Ϊ0
//m-���������
//n-���������
//u-����Ϊm*m�����飬����ʱ�������ֵ�ֽ����������U
//v-����Ϊn*n�����飬����ʱ�������ֵ�ֽ����������V
//eps-����Ҫ��
//ka-���ͱ�������ֵΪmax(n,m)+1
//���ú�����dluav(),ppp(),sss()
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