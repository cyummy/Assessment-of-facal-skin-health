// HistogramDrawDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "HistogramDrawDlg.h"
#include "ImageDib.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HistogramDrawDlg dialog


HistogramDrawDlg::HistogramDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(HistogramDrawDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(HistogramDrawDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	for(int i=0;i<256;i++)
		m_histArray[i]=0;
	
	m_average=0;//��ֵΪ0
	m_deviation=0;//����Ϊ0
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0

}


void HistogramDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HistogramDrawDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HistogramDrawDlg, CDialog)
	//{{AFX_MSG_MAP(HistogramDrawDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HistogramDrawDlg message handlers
BOOL HistogramDrawDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		// TODO: Add extra initialization here

	m_nBitCountOut=m_himageDib.m_nBitCount;
    m_imgWidthOut=m_himageDib.m_imgWidth;
    m_imgHeightOut=m_himageDib.m_imgHeight;
	//����̬��1����rectָ��λ�ã�������ֱ��ͼ
	CRect rect;
	rect=CRect(50,60,300,170);
	GetDlgItem(IDC_STATIC_Hist)->MoveWindow(&rect);
	
	//����ɫ��ʾ������ָ��λ��
	rect=CRect(50,195,300,228);
	GetDlgItem(IDC_STATIC_GrayColor)->MoveWindow(&rect);

 
	// ����ǻҶ�ͼ������ûҶ�ֱ��ͼͳ�ƺ���

	if(m_nBitCountOut==8)
		computeHistGray();
	else//��ɫͼ����ͳ������ֱ��ͼ
		computeHistBrightness();
	
	//����ֱ��ͼ�ľ�ֵ�ͷ���
    float m_ave=computeAverage();
    float m_dev=computeDeviation();
	
	//����ֵ��Ϣ��ʾ�ھ�ֵ��̬����
	char szStr[10];    
 	sprintf(szStr, "%.2f",m_ave); 
    SetDlgItemText(IDC_STATIC_AveShow, szStr);         
	
	//��������Ϣ��ʾ�ڷ��̬����
    sprintf(szStr, "%.2f", m_dev);            
    SetDlgItemText(IDC_STATIC_DevShow, szStr); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void HistogramDrawDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//��ɫ�ͻҶ�ͼ����Ч
	if(m_nBitCountOut!= 8 && m_nBitCountOut!= 24) 
		return ;
	
	//��ȡֱ��ͼ���ƾ�̬��ľ�������
	CRect clientRect;  
	GetDlgItem(IDC_STATIC_Hist)->GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);
	
	BeginWaitCursor();	    // Draw Back Ground
	//������
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(clientRect.left, clientRect.top-5,
		clientRect.right, clientRect.bottom+1);
	
	//��ֱ��ͼ�Ҷ�Ƶ����ֱ��ͼ���ƾ������������ϵ����ֱ��ͼ
	unsigned int max=0;;
	for(int i=0;i<256;i++){
		if(m_histArray[i]>max)
			max=m_histArray[i];
	}
	DWORD bufTemp[256];
	double x=clientRect.Height();
	for(i=0;i<256;i++){
		bufTemp[i]=(unsigned long)((float)m_histArray[i]*x/max);
	}
	for(i=clientRect.left;i<=clientRect.right;i++){
		dc.MoveTo(i,clientRect.bottom);
		dc.LineTo(i,clientRect.bottom-bufTemp[i-clientRect.left]);
	}   
	
	CString str;
	SetBkMode(dc,1);      //���屳����Ϊ͸��
    str.Format("0");
	dc.TextOut(clientRect.left,clientRect.bottom+1,str);
	str.Format("50");
	dc.TextOut(clientRect.left+50,clientRect.bottom+1,str);
	str.Format("100");
	dc.TextOut(clientRect.left+100,clientRect.bottom+1,str);
	str.Format("150");
	dc.TextOut(clientRect.left+150,clientRect.bottom+1,str);
	str.Format("200");
	dc.TextOut(clientRect.left+200,clientRect.bottom+1,str);
	str.Format("255");
	dc.TextOut(clientRect.left+255,clientRect.bottom+1,str);
	
 	EndWaitCursor();
	// Do not call CDialog::OnPaint() for painting messages
}


HistogramDrawDlg::~HistogramDrawDlg()
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
CSize HistogramDrawDlg::GetDimensions()
{	
	m_imgWidthOut=m_himageDib.m_imgWidth;
	m_imgHeightOut=m_himageDib.m_imgHeight;
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* �������ƣ�
* computeHistGray()
*
*˵�����Ҷ�ͼ��ͳ��ֱ��ͼ��m_histArray�д���˵�ǰͼ���ͳ������
***********************************************************************/
void HistogramDrawDlg::computeHistGray()
{
	
	//ֻ����Ҷ�ͼ��
	if(m_nBitCountOut!=8)
		return;

	//ѭ������
	int i,j;

	//ֱ��ͼ������0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;
    
	//ÿ��������ռ�ֽ���
	int lineByte=(m_himageDib.m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//�м����
	int temp;

	//ͳ�ƻҶ�ֱ��ͼ
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			temp=*(m_himageDib.m_pImgData+i*lineByte+j);
		    m_histArray[temp]++;
		}
	}
}


/***********************************************************************
* �������ƣ�
* computeHistBrightness()
*
*˵������ɫͼ������ֱ��ͼ��m_histArray�д���˵�ǰͼ�������ͳ������
***********************************************************************/
 
void HistogramDrawDlg::computeHistBrightness()
{
	//��ɫͼ����Ч
	if(m_nBitCountOut!=24)
		return;

	//ѭ������
	int i,j;

	//ֱ��ͼ������0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;
	//ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;

	//�м����
	int temp;

	//ͳ������ֱ��ͼ
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			temp=0.11**(m_himageDib.m_pImgData+i*lineByte+j*3+0)
				+0.59**(m_himageDib.m_pImgData+i*lineByte+j*3+1)
				+0.30**(m_himageDib.m_pImgData+i*lineByte+j*3+2)+0.5;
		    m_histArray[temp]++;
		}
	}
}

                                                                  


/***********************************************************************
* �������ƣ�
* computeAverage()
*
*˵��������ֱ��ͼ��ֵ��ֱ��ͼ��ͳ������
***********************************************************************/
float HistogramDrawDlg::computeAverage()
{
	int sum=0;
	for(int i=0;i<256;i++)
		sum+= i*m_histArray[i];
	m_average=(float)sum/(m_himageDib.m_imgWidth*m_himageDib.m_imgHeight);
    float m_aver=m_average;
    return m_aver;
}

/***********************************************************************
* �������ƣ�
* computeDeviation()
*
*˵��������ֱ��ͼ���ֱ��ͼ��ͳ������
***********************************************************************/
float HistogramDrawDlg::computeDeviation()
{

	double deviation=0;
	for(int i=0;i<256;i++)
		deviation += (i-m_average)*(i-m_average)*m_histArray[i];

	deviation/=(m_himageDib.m_imgWidth*m_himageDib.m_imgHeight);
	m_deviation=sqrt(deviation);
    float m_devi=m_deviation;
	return m_devi;
}


void HistogramDrawDlg::histogramAve()
{
  	// ֻ����Ҷ�
	if(m_himageDib.m_nBitCount!=8)
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

	//���ͼ��ÿ����λ��������ͼ����ͬ
	m_nBitCountOut=m_himageDib.m_nBitCount;

	//������ɫ����
	m_nColorTableLengthOut=m_himageDib.ComputeColorTabalLength(m_nBitCountOut);

	
	//������ɫ������ͼ�����ɫ�������ͼ����ɫ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_himageDib.m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_himageDib.m_imgWidth;
	m_imgHeightOut=m_himageDib.m_imgHeight;

	//���ͼ��ÿ��������ռ���ֽ���
	int lineByteOut=(m_himageDib.m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_himageDib.m_imgHeight];

	//ѭ������
	int i,j;

	//ӳ���
	double map[256];

	//�м����
	int sum,tmp;

	//ͳ�ƻҶ�ֱ��ͼ
	computeHistGray();

	//����ӳ���
	sum=0;
	for(i=0;i<256;i++){
		sum+=m_histArray[i];
		map[i]=(double)sum*255/(m_himageDib.m_imgWidth*m_himageDib.m_imgHeight)+0.5;
	}
	
	//������ݸ�ֵ
	for(i=0;i<m_himageDib.m_imgHeight;i++)
	{
		for(j=0;j<m_himageDib.m_imgWidth;j++)
		{		
			tmp=*(m_himageDib.m_pImgData+i*lineByteOut+j);
			*(m_pImgDataOut+i*lineByteOut+j)=(int)map[tmp];
		}
	}
}
