#include "stdio.h"
#include "stdafx.h"
#include "demo1.h"
#include "Detect.h"
#include "math.h"
#include "stdlib.h"
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
/***********************************************************************
* �������ƣ� Detect()
* ˵�����޲����Ĺ��캯�����Գ�Ա�������г�ʼ��
***********************************************************************/
Detect::Detect()
{
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}

/***********************************************************************
* �������ƣ� Detect()
* ���������� CSize size -ͼ���С�����ߣ�
*            int nBitCount  -ÿ���ر�����
*            LPRGBQUAD lpColorTable  -��ɫ��ָ��
*            unsigned char *pImgData  -λͼ����ָ��
* ����ֵ��   ��
* ˵����������Ϊ�������Ĺ��캯��������λͼ�Ĵ�С��ÿ����λ������ɫ��
*      ��λͼ���ݣ�����ImgDib()�Ի����Ա��ʼ������Ϊ����ͼ������
*      ��ص����ݳ�Ա,����ʼ�����ͼ����ص����ݳ�Ա
***********************************************************************/
Detect::Detect(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
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
* �������ƣ�  ~Detect()
* ˵���������������ͷ���Դ
***********************************************************************/
Detect::~Detect()
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
CSize Detect::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


bool v[2000][2000];
int c[2000][2000],ys[2000][2000];   //ys�Ǹ���ģ� 
struct color{
	int num,x,id;
	int cx,cy;
};
vector<color> p;

struct note{
	int x,y;
};
vector<note> d;

struct rec{
	int x,y;
}f[2000][2000];
int fn[2000][2000],ft[2000][2000],ansn[2000][2000],tim[2000][2000];//fnĳ�����ڵ��µ����������ĳ�����ڵ�ı�ú�
int a,b,ccc,dd,e,ff,gg,h,zz;//a 15-25; b 25-35;c 35-50;
int nm=0,cnt,cc;
bool pan(int x,int y,int h,int z){
	if (x==0 || y==0) return 0;
	if (x==h-1 || y==z-1) return 0;
	if (c[x][y]<=c[x-1][y] && c[x][y]<=c[x+1][y] && c[x][y]<=c[x][y+1]
		&& c[x][y]<=c[x][y-1] && c[x][y]<=c[x+1][y+1] && c[x][y]<=c[x-1][y-1]
		&& c[x][y]<=c[x+1][y-1] && c[x][y]<=c[x-1][y+1]) return 1;
	return 0;
}

void floodfill(int x,int y,int rm,int cm){
	v[x][y]=0;
	int cx,cy,cl,op,xx,yy,mm;
	cl=0; op=1;
	xx=yy=0;
	mm=0;
	note ds;
	ds.x=x; ds.y=y;
	d.clear();
	d.push_back(ds);
	while (cl<op){
		cx=d[cl].x; 
		cy=d[cl].y;
		cl++;
		ys[cx][cy]=p.size();
		xx+=cx;
		yy+=cy;
		if (c[cx][cy]>mm) mm=c[cx][cy];
		if (cx>0 && v[cx-1][cy] && abs(c[cx][cy]-c[cx-1][cy])<1){
			op++;
			ds.x=cx-1; ds.y=cy;
			d.push_back(ds);
			v[cx-1][cy]=0;
		}
		if (cy>0 && v[cx][cy-1] && abs(c[cx][cy]-c[cx][cy-1])<1){
			op++;
			ds.x=cx; ds.y=cy-1;
			d.push_back(ds);
			v[cx][cy-1]=0;
		}
		if (cx<rm-1 && v[cx+1][cy] && abs(c[cx][cy]-c[cx+1][cy])<1){
			op++;
			ds.x=cx+1; ds.y=cy;
			d.push_back(ds);
			v[cx+1][cy]=0;
		}
		if (cy<cm+1 && v[cx][cy+1] && abs(c[cx][cy]-c[cx][cy+1])<1){
			op++;
			ds.x=cx; ds.y=cy+1;
			d.push_back(ds);
			v[cx][cy+1]=0;
		}
	}
	for (int i=0;i<d.size();i++) c[d[i].x][d[i].y]=mm;
	
	color ltk;
	ltk.num=op;
	ltk.x=c[x][y];
	ltk.cx=xx/op;
	ltk.cy=yy/op;
	ltk.id=p.size();
	p.push_back(ltk);
}

bool cmp(const color &x,const color &y){
	return x.x<y.x;
}

rec get_pa(int x,int y){                  //�ҵ����ڵ�
	if (f[x][y].x==x && f[x][y].y==y) return f[x][y];//������Լ��Ǹ��ڵ㣬��ֱ�ӷ���������ĺ�������
	else f[x][y]=get_pa(f[x][y].x,f[x][y].y);		//����Լ����Ǹ��ڵ㣬�Ǿ�ȥ�ҵ�ǰ������ϼ����ϼ���Ϊ���ܵĸ��ڵ㣬Ȼ�����һ���ݹ飨ѭ����ֱ���ҵ�������ڵ㣬�ҵ��˾ͷ���������ڵ�ĺ�������	
	return f[x][y];
}
int g[8][2];
void find_pa(int x,int y,int h,int z){      //������µ����ϵͳȷ�ϵ�λ
	rec l,r;
	int i,j,cx,cy;
	for (i=0;i<7;i++){
		cx=x+g[i][0];
		cy=y+g[i][1];
		if (cx>=0 && cy>=0 && cx<h && cy<z && ys[cx][cy]==255){     //ys�Ǹ���ģ�
			r=get_pa(x,y);
			l=get_pa(cx,cy);
			if (l.x==r.x && l.y==r.y) continue;
			if (ft[l.x][l.y]<=ft[r.x][r.y]){
				if (fn[r.x][r.y]>50) continue;                                         //����50��
				f[r.x][r.y].x=l.x;
				f[r.x][r.y].y=l.y;
				fn[l.x][l.y]+=fn[r.x][r.y];   //������ڵ�ĸ���+1
			}
			else {
				if (fn[l.x][l.y]>50) continue;
				f[l.x][l.y].x=r.x;
				f[l.x][l.y].y=r.y;
				fn[r.x][r.y]+=fn[l.x][l.y];
			}
		}
	}
}

int Detect::Pot(){
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
		int m,n,i,j;
		
		//���ݻ�ֵ����ʽΪ���ͼ��ֵ
		for(m=0;m<m_imgHeight;m++){
			for(n=0;n<m_imgWidth;n++){
				*(m_pImgDataOut+m*lineByteOut+n)=0.11**(m_pImgData+m*lineByteIn+n*pixelByteIn+0)
				+0.59**(m_pImgData+m*lineByteIn+n*pixelByteIn+1)
				+0.30**(m_pImgData+m*lineByteIn+n*pixelByteIn+2)+0.5;
				c[m][n]=*(m_pImgDataOut+m*lineByteOut+n);
			}
		}

		memset(v,1,sizeof(v));
		memset(ys,0,sizeof(ys));
		memset(fn,0,sizeof(fn));
		memset(ft,0,sizeof(ft));
		memset(ansn,0,sizeof(ansn));

		g[0][0]=0; g[0][1]=1;
		g[1][0]=1; g[1][1]=0;
		g[2][0]=0; g[2][1]=-1;
		g[3][0]=-1; g[3][1]=0;
		g[4][0]=1; g[4][1]=1;
		g[5][0]=1; g[5][1]=-1;
		g[6][0]=-1; g[6][1]=1;
		g[7][0]=-1; g[7][1]=-1;

		for(m=0;m<m_imgHeight;m++){
			for(n=0;n<m_imgWidth;n++){
				ys[m][n]=c[m][n];
				if (v[m][n] && pan(m,n,m_imgHeight,m_imgWidth))
					floodfill(m,n,m_imgHeight,m_imgWidth);
			}
		}
		sort(p.begin(),p.end(),cmp);
		rec l;
		for (i=0;i<p.size();i++)
		{
			if (i && p[i].x==p[i-1].x) continue;
			for(m=0;m<m_imgHeight;m++)
			{
				for(n=0;n<m_imgWidth;n++)
				{
					if (p[i].x==c[m][n] && ys[m][n]!=255)
					{
						ys[m][n]=255;
						f[m][n].x=m;
						f[m][n].y=n;
						ft[m][n]=i;
						fn[m][n]=1;
						find_pa(m,n,m_imgHeight,m_imgWidth);
						l=get_pa(m,n);
						if (fn[l.x][l.y]<100) 
						{                                                                           //���ص����С��100
							ansn[l.x][l.y]=fn[l.x][l.y];              //�ӵ����ظ���
							tim[l.x][l.y]=i;
						}
					}
				}
			}
		}
		a=0;
		b=0;
		ccc=0;
		dd=0;
		e=0;
		ff=0;
		gg=0;
		h=0;
		zz=0;
		for(m=0,nm=0;m<m_imgHeight;m++)
		{
			for(n=0;n<m_imgWidth;n++)
			{
				l=get_pa(m,n);
				if (ansn[l.x][l.y]>15 && l.x==m && l.y==n)                                                    // >15
				{      
						*(m_pImgDataOut+m*lineByteOut+n)=255;  //���
						nm++;//����				
						if(ansn[l.x][l.y]>15 && ansn[l.x][l.y]<25) a=a+1;
						if(ansn[l.x][l.y]>25 && ansn[l.x][l.y]<=35) b++;
						if(ansn[l.x][l.y]>35 && ansn[l.x][l.y]<=50) ccc++;
						if(ansn[l.x][l.y]>50 && ansn[l.x][l.y]<60) dd++;
						if(ansn[l.x][l.y]>60 && ansn[l.x][l.y]<70) e++;
						if(ansn[l.x][l.y]>70 && ansn[l.x][l.y]<80) ff++;
						if(ansn[l.x][l.y]>80 && ansn[l.x][l.y]<90) gg++;
						if(ansn[l.x][l.y]>90 && ansn[l.x][l.y]<100) h++;
						if(ansn[l.x][l.y]>100 && ansn[l.x][l.y]<110) zz;

				}
			}
		}
		return nm;
}

int black[2000][2000],white[2000][2000];
void work(int lx,int ly,int rx,int ry){
		int i,j,k,s,thr,m,n,max_N,min_N;
		max_N=0;
		min_N=255;

		for(m=lx;m<=rx;m++){
			for(n=ly;n<=ry;n++){
				if (black[m][n]) white[m][n]=20;
			}
		}

		for(m=lx;m<=rx;m++){
			for(n=ly;n<=ry;n++){
				if (black[m][n]) continue;
				if (c[m][n]>max_N) max_N=c[m][n];
				if (c[m][n]<min_N) min_N=c[m][n];
			}
		}
		n=2000000000;
		for (m=min_N;m<=max_N;m++){
			k=0;
			for(i=lx;i<=rx;i++)
				for(j=ly;j<=ry;j++) {
					if (black[i][j]) continue;
					k+=(c[i][j]-m)*(c[i][j]-m);
				}
			if (k<n){
				n=k;
				s=m;
			}
		}
		for(m=lx;m<=rx;m++){
			for(n=ly;n<=ry;n++){
				if (c[m][n]<s) black[m][n]++;
				else		   white[m][n]++;
			}
		}
}

/*
void Detect::Pot()
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
 	//���ݻ�ֵ����ʽΪ���ͼ��ֵ
    for(m=top+2;m<bottom-1;m++){
	   for(n=left+2;n<right-1;n++){
		   *(m_pImgDataOut1+m*lineByteOut1+n)=0.11**(m_pImgData+m*lineByteIn+n*pixelByteIn+0)
				                             +0.59**(m_pImgData+m*lineByteIn+n*pixelByteIn+1)
			                                 +0.30**(m_pImgData+m*lineByteIn+n*pixelByteIn+2)+0.5;			
	   }   
	}
    

	// ��ͼ���ƽ��ֵ
	int nLocAvg ;
	int C=10;
	int max,min;
	// ��ͼ�����ɨ�裺
	
	for(m=top+2;m<bottom-1;m+=4){
		for(n=left+2;n<right-1;n+=4){
		    max=min=*(m_pImgDataOut1+m*lineByteOut1+n);
			nLocAvg = 0 ;
            for(i=0;i<4;i++){
				for(j=0;j<4;j++){	
					if(*(m_pImgDataOut1+(m+i)*lineByteOut1+(n+j))>max)
					    max=*(m_pImgDataOut1+(m+i)*lineByteOut1+(n+j));
					if(*(m_pImgDataOut1+(m+i)*lineByteOut1+(n+j))<min)
					    min=*(m_pImgDataOut1+(m+i)*lineByteOut1+(n+j));
				}
			}
            nLocAvg=(max+min)/2;
			for(i=0;i<4;i++){
				for(j=0;j<4;j++){	
					if(abs(*(m_pImgDataOut1+(m+i)*lineByteOut1+(n+j))-nLocAvg)>C)
					    *(m_pImgDataOut+(m+i)*lineByteOut+(n+j))=0;
					else
						*(m_pImgDataOut+(m+i)*lineByteOut+(n+j))=255;
				}
			}
			
		}
	}
	


}*/
/***********************************************************************
* ������������
* ���ڻҶȹ�����������ɫ��λ���ڴֲڶ�
***********************************************************************/
 //�����Ҷȹ������� ��ȡ����ֵ
double gray[16][16];
//int c[2000][2000];
Detect::zw Detect::Wrinkle(){
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


		
		//ѭ������,ͼ�������
		int m,n;
		double sum;
		zw as;
		//���ݻ�ֵ����ʽΪ���ͼ��ֵ
		for(m=0;m<m_imgHeight;m++){
			for(n=0;n<m_imgWidth;n++){
				*(m_pImgDataOut+m*lineByteOut+n)=0.11**(m_pImgData+m*lineByteIn+n*pixelByteIn+0)
				                                +0.59**(m_pImgData+m*lineByteIn+n*pixelByteIn+1)
			                                  	+0.30**(m_pImgData+m*lineByteIn+n*pixelByteIn+2)+0.5;
				c[m][n]=*(m_pImgDataOut+m*lineByteOut+n)/16;    //����16���ĻҶȼ�
			}
		}

		as.asmm=as.ent=as.con=as.idm=sum=0;
		memset(gray,0,sizeof(gray));
		for(m=top+2;m<bottom-1;m++){
			for(n=left+2;n<right-1;n++){
				gray[c[m][n]][c[m][n+1]]+=1;      //�Ҷȹ�������ÿ��������Ӧ�ĻҶȶԳ��ֵĴ���
				sum++;
			}
		}
		m=1;
	        	for(m=0;m<16;m++){
					for(n=0;n<16;n++){
			            gray[m][n]=gray[m][n]/sum;     //���ϸ����ܶ�
					}
				}


				for (m=0;m<16;m++){
					for (n=0;n<16;n++){
				        if (gray[m][n]-0>0.0005)
					    as.asmm+=gray[m][n]*gray[m][n];    //�Ƕ��׾�
					}
				}
				for (m=0;m<16;m++){
					for (n=0;n<16;n++){
				        if (gray[m][n]-0>0.0005)
					    as.ent+=-gray[m][n]*log(gray[m][n]);   //��
					}
				}
				for (m=0;m<16;m++){
					for (n=0;n<16;n++){
			        	if (gray[m][n]-0>0.0005)
				    	as.con+=(m-n)*(m-n)*gray[m][n];       //�Աȶ�
					}
				}
				for (m=0;m<16;m++){
					for (n=0;n<16;n++){
			        	if (gray[m][n]-0>0.0005)
					    as.idm+=gray[m][n]/(1+(m-n)*(m-n));    //����
					}
				}

                
	return as;
	
}


	

	


