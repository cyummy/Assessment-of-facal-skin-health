// LinearStepParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "LinearStepParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LinearStepParamDlg dialog


LinearStepParamDlg::LinearStepParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LinearStepParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(LinearStepParamDlg)
	m_nX1 = 50;
	m_nX2 = 200;
	m_nY1 = 50;
	m_nY2 = 200;
	//}}AFX_DATA_INIT
}


void LinearStepParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LinearStepParamDlg)
	DDX_Text(pDX, IDC_EDIT_LinetransX1, m_nX1);
	DDX_Text(pDX, IDC_EDIT_LinetransX2, m_nX2);
	DDX_Text(pDX, IDC_EDIT_LinetransY1, m_nY1);
	DDX_Text(pDX, IDC_EDIT_LinetransY2, m_nY2);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LinearStepParamDlg, CDialog)
	//{{AFX_MSG_MAP(LinearStepParamDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_EN_KILLFOCUS(IDC_EDIT_LinetransX1, OnKillfocusEDITLinetransX1)
	ON_EN_KILLFOCUS(IDC_EDIT_LinetransX2, OnKillfocusEDITLinetransX2)
	ON_EN_KILLFOCUS(IDC_EDIT_LinetransY1, OnKillfocusEDITLinetransY1)
	ON_EN_KILLFOCUS(IDC_EDIT_LinetransY2, OnKillfocusEDITLinetransY2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LinearStepParamDlg message handlers



BOOL LinearStepParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_LINTRANS_SHOW);
	
	// �����������¼�����Ч����
	pWnd->GetClientRect(m_rectMouse);
	pWnd->ClientToScreen(&m_rectMouse);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_rectMouse.top -= rect.top;
	m_rectMouse.left -= rect.left;
	
	// ���ý�������¼�����Ч����
	m_rectMouse.top += 25;
	m_rectMouse.left += 10;
	m_rectMouse.bottom = m_rectMouse.top + 255;
	m_rectMouse.right = m_rectMouse.left + 256;
	
	// ��ʼ���϶�״̬
	m_nIsDraging = 0;


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void LinearStepParamDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// �ַ���
	CString str;
	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_LINTRANS_SHOW);
	
	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,305);
	
	// �������ʶ���
	CPen* pPenRed = new CPen;
	
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255,0,0));
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0, 255));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// ����������
	pDC->MoveTo(10,10);
	
	// ��ֱ��
	pDC->LineTo(10,280);
	
	// ˮƽ��
	pDC->LineTo(320,280);
	
	// ����˵�
	str.Format("0");
	pDC->TextOut(10, 281, str);
	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// ����X���ͷ
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// ����X���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// ���ĳ���ɫ����
	pDC->SelectObject(pPenBlue);	
	
	// ��������ֵ
	str.Format("(%d, %d)", m_nX1, m_nY1);
	pDC->TextOut(m_nX1 + 10, 281 - m_nY1, str);

	str.Format("(%d, %d)", m_nX2, m_nY2);
	pDC->TextOut(m_nX2 + 10, 281 - m_nY2, str);

 
	// �����û�ָ���ı任ֱ��
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_nX1 + 10, 280 - m_nY1);
	pDC->LineTo(m_nX2 + 10, 280 - m_nY2);
	pDC->LineTo(265, 25);
	
	// ���Ƶ��Ե��С����
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0,255,0));
	
	// ѡ��ˢ��
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);
	
	// ����С����
	pDC->Rectangle(m_nX1 + 10 - 2, 280 - m_nY1 - 2, m_nX1 + 12, 280 - m_nY1 + 2);
	pDC->Rectangle(m_nX2 + 10 - 2, 280 - m_nY2 - 2, m_nX2 + 12, 280 - m_nY2 + 2);
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ���Ʊ�Ե
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;

	// Do not call CDialog::OnPaint() for painting messages
}

void LinearStepParamDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// ���û�������������ʼ�϶�
	if(m_rectMouse.PtInRect(point))
	{
		CRect	rectTemp;
		
		// �����1�ٽ�����
		rectTemp.left = m_rectMouse.left + m_nX1 - 2;
		rectTemp.right = m_rectMouse.left + m_nX1 + 2;
		rectTemp.top = 255 + m_rectMouse.top - m_nY1 - 2;
		rectTemp.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
		
		// �ж��û��ǲ������϶���1
		if (rectTemp.PtInRect(point))
		{
			
			// �����϶�״̬1���϶���1
			m_nIsDraging = 1;
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			// �����2�ٽ�����
			rectTemp.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// �ж��û��ǲ������϶���2
			if (rectTemp.PtInRect(point))
			{
				
				// �����϶�״̬Ϊ2���϶���2
				m_nIsDraging = 2;
				
				// ���Ĺ��
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void LinearStepParamDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nIsDraging != 0)
	{
		// �����϶�״̬
		m_nIsDraging = 0;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void LinearStepParamDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// �жϵ�ǰ����Ƿ��ڻ�������
	if(m_rectMouse.PtInRect(point))
	{
		// �ж��Ƿ������϶�
		if (m_nIsDraging != 0)
		{
			// �ж������϶���1���ǵ�2
			if (m_nIsDraging == 1)
			{
				// �ж��Ƿ�����<����
				if (point.x - m_rectMouse.left < m_nX2)
				{
					// ��������
					m_nX1 =  point.x - m_rectMouse.left;
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ����-1
					m_nX1 = m_nX2 - 1;
					
					// �������λ��
					point.x = m_rectMouse.left + m_nX2 - 1;
				}
				
				// ����Y����
				m_nY1 = 255 + m_rectMouse.top - point.y;
			}
			else
			{
				// �����϶���2
				
				// �ж��Ƿ�����>����
				if (point.x - m_rectMouse.left > m_nX1)
				{
					// ��������
					m_nX2 = point.x - m_rectMouse.left;
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ���ޣ�1
					m_nX2 = m_nX1 + 1;
					
					// �������λ��
					point.x = m_rectMouse.left + m_nX1 + 1;
				}
				
				// ����Y����
				m_nY2 = 255 + m_rectMouse.top - point.y;
			}
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			
			// ����
			UpdateData(FALSE);
			
			// �ػ�
			InvalidateRect(m_rectMouse, TRUE);
		}
		else
		{
			CRect	rectTemp1;
			CRect	rectTemp2;
			
			// �����1�ٽ�����
			rectTemp1.left = m_rectMouse.left + m_nX1 - 2;
			rectTemp1.right = m_rectMouse.left + m_nX1 + 2;
			rectTemp1.top = 255 + m_rectMouse.top - m_nY1 - 2;
			rectTemp1.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
			
			// �����2�ٽ�����
			rectTemp2.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp2.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp2.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp2.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// �ж��û��ڵ�1���2�Ա�
			if ((rectTemp1.PtInRect(point)) || (rectTemp2.PtInRect(point)))
			{
				// ���Ĺ��
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void LinearStepParamDlg::OnOK() 
{
	// TODO: Add extra validation here
/**/
	// �ж��Ƿ����޳�������
	if (m_nX1 > m_nX2)
	{
		// ����
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// ����
		UpdateData(FALSE);
	}
	
	CDialog::OnOK();
}

void LinearStepParamDlg::OnKillfocusEDITLinetransX1() 
{
	// TODO: Add your control notification handler code here
	// ����
	UpdateData(TRUE);
	
	// �ж��Ƿ����޳�������
	if (m_nX1 > m_nX2)
	{
		// ����
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// ����
		UpdateData(FALSE);
	}
	
	// �ػ�
	InvalidateRect(m_rectMouse, TRUE);
}

void LinearStepParamDlg::OnKillfocusEDITLinetransX2() 
{
	// TODO: Add your control notification handler code here
	// ����
	UpdateData(TRUE);
	
	// �ж��Ƿ����޳�������
	if (m_nX1 > m_nX2)
	{
		// ����
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// ����
		UpdateData(FALSE);
	}
}

void LinearStepParamDlg::OnKillfocusEDITLinetransY1() 
{
	// TODO: Add your control notification handler code here
	// ����
	UpdateData(TRUE);
	
	// �ػ�
	InvalidateRect(m_rectMouse, TRUE);
}

void LinearStepParamDlg::OnKillfocusEDITLinetransY2() 
{
	// TODO: Add your control notification handler code here
	// ����
	UpdateData(TRUE);
	
	// �ػ�
	InvalidateRect(m_rectMouse, TRUE);
}


