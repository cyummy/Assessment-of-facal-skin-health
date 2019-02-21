// HistSegmentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "HistSegmentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HistSegmentDlg dialog


HistSegmentDlg::HistSegmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(HistSegmentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(HistSegmentDlg)
	m_threshchange = 128;
	//}}AFX_DATA_INIT
	for(int i=0;i<256;i++)
		m_histArray[i]=0;
	
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0
	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0
	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0

}


void HistSegmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HistSegmentDlg)
	DDX_Slider(pDX, IDC_SLIDER_GrayValue, m_threshchange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HistSegmentDlg, CDialog)
	//{{AFX_MSG_MAP(HistSegmentDlg)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HistSegmentDlg message handlers

HistSegmentDlg::~HistSegmentDlg()
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

BOOL HistSegmentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	
//	// TODO: Add extra initialization here
	m_nBitCountOut=m_himageDib.m_nBitCount;
    m_imgWidthOut=m_himageDib.m_imgWidth;
    m_imgHeightOut=m_himageDib.m_imgHeight;
	//����̬��1����rectָ��λ�ã�������ֱ��ͼ
	CRect rect;
	rect=CRect(100,60,350,170);
	GetDlgItem(IDC_STATIC_Hist)->MoveWindow(&rect);
	
	//�������������ƶ�λ��
	rect=CRect(100,195,350,228);
	GetDlgItem(IDC_SLIDER_GrayValue)->MoveWindow(&rect);

	//����ɫ��ʾ������ָ��λ��
	rect=CRect(100,235,350,268);
	GetDlgItem(IDC_STATIC_GrayColor)->MoveWindow(&rect);
	
	//���ı�������ָ��λ��
	rect=CRect(10,195,95,228);
	GetDlgItem(IDC_STATIC_text)->MoveWindow(&rect) ;
	rect=CRect(350,195,420,228);
	GetDlgItem(IDC_STATIC_Value)->MoveWindow(&rect) ;

	
//	// ����ǻҶ�ͼ������ûҶ�ֱ��ͼͳ�ƺ���
	
	if(m_nBitCountOut==8)
		computeHistGray();
	else//��ɫͼ����ͳ������ֱ��ͼ
		computeHistBrightness();
    
	char szStr[10];
	CSliderCtrl *pSlider=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_GrayValue);
	pSlider->SetRange(0,255);
	pSlider->SetPos(m_threshchange);
	sprintf(szStr, "%d",m_threshchange);            
    SetDlgItemText(IDC_STATIC_Value, szStr); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void HistSegmentDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_nBitCountOut!= 8 && m_nBitCountOut!= 24) 
		return ;
	
	//��ȡֱ��ͼ���ƾ�̬��ľ�������
	CRect clientRect;  
	GetDlgItem(IDC_STATIC_Hist)->GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);
	
	BeginWaitCursor();	    // Draw Back Ground
	//������
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(clientRect.left-1, clientRect.top-5,
		clientRect.right+5, clientRect.bottom+1);
	
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
	str.Format("255");
	dc.TextOut(clientRect.left+255,clientRect.bottom+1,str);
	


	//������ɫ����
	CPen *pBluePen=new CPen;
	pBluePen->CreatePen(PS_SOLID,1,RGB(0,0,255));
	
	//ѡ�е�ǰ��ɫ���ʣ�������ɵĻ���
	CGdiObject *pOldPen=dc.SelectObject(pBluePen);
	
	//��ֱ��ͼ�л�����ֵ��λ��
	dc.MoveTo(m_threshchange+100,clientRect.bottom);
	dc.LineTo(m_threshchange+100,clientRect.top);
	
	//�ָ��ɻ���
	dc.SelectObject(pOldPen);
	// Do not call CDialog::OnPaint() for painting messages
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
CSize HistSegmentDlg::GetDimensions()
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
void HistSegmentDlg::computeHistGray()
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
 
void HistSegmentDlg::computeHistBrightness()
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

                                                          


void HistSegmentDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	// ���ݻ�����λ�ø�����ֵ
	CSliderCtrl *pSlider=(CSliderCtrl *)pScrollBar;
	m_threshchange=pSlider->GetPos();
	
	//�ھ�̬������ʾ��������ֵ
	char szStr[20];
	sprintf(szStr, "%d",m_threshchange);            
    SetDlgItemText(IDC_STATIC_Value, szStr);  
	//ˢ����ʾ
	Invalidate();
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

