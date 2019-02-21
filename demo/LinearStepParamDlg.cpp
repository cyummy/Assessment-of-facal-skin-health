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
	
	// 计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_rectMouse);
	pWnd->ClientToScreen(&m_rectMouse);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_rectMouse.top -= rect.top;
	m_rectMouse.left -= rect.left;
	
	// 设置接受鼠标事件的有效区域
	m_rectMouse.top += 25;
	m_rectMouse.left += 10;
	m_rectMouse.bottom = m_rectMouse.top + 255;
	m_rectMouse.right = m_rectMouse.left + 256;
	
	// 初始化拖动状态
	m_nIsDraging = 0;


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void LinearStepParamDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// 字符串
	CString str;
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_LINTRANS_SHOW);
	
	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,305);
	
	// 创建画笔对象
	CPen* pPenRed = new CPen;
	
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0, 255));
	
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// 绘制坐标轴
	pDC->MoveTo(10,10);
	
	// 垂直轴
	pDC->LineTo(10,280);
	
	// 水平轴
	pDC->LineTo(320,280);
	
	// 坐标端点
	str.Format("0");
	pDC->TextOut(10, 281, str);
	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// 绘制X轴箭头
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);	
	
	// 绘制坐标值
	str.Format("(%d, %d)", m_nX1, m_nY1);
	pDC->TextOut(m_nX1 + 10, 281 - m_nY1, str);

	str.Format("(%d, %d)", m_nX2, m_nY2);
	pDC->TextOut(m_nX2 + 10, 281 - m_nY2, str);

 
	// 绘制用户指定的变换直线
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_nX1 + 10, 280 - m_nY1);
	pDC->LineTo(m_nX2 + 10, 280 - m_nY2);
	pDC->LineTo(265, 25);
	
	// 绘制点边缘的小矩形
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0,255,0));
	
	// 选中刷子
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);
	
	// 绘制小矩形
	pDC->Rectangle(m_nX1 + 10 - 2, 280 - m_nY1 - 2, m_nX1 + 12, 280 - m_nY1 + 2);
	pDC->Rectangle(m_nX2 + 10 - 2, 280 - m_nY2 - 2, m_nX2 + 12, 280 - m_nY2 + 2);
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 绘制边缘
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;

	// Do not call CDialog::OnPaint() for painting messages
}

void LinearStepParamDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// 当用户单击鼠标左键开始拖动
	if(m_rectMouse.PtInRect(point))
	{
		CRect	rectTemp;
		
		// 计算点1临近区域
		rectTemp.left = m_rectMouse.left + m_nX1 - 2;
		rectTemp.right = m_rectMouse.left + m_nX1 + 2;
		rectTemp.top = 255 + m_rectMouse.top - m_nY1 - 2;
		rectTemp.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
		
		// 判断用户是不是想拖动点1
		if (rectTemp.PtInRect(point))
		{
			
			// 设置拖动状态1，拖动点1
			m_nIsDraging = 1;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			// 计算点2临近区域
			rectTemp.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// 判断用户是不是想拖动点2
			if (rectTemp.PtInRect(point))
			{
				
				// 设置拖动状态为2，拖动点2
				m_nIsDraging = 2;
				
				// 更改光标
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
		// 重置拖动状态
		m_nIsDraging = 0;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void LinearStepParamDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// 判断当前光标是否在绘制区域
	if(m_rectMouse.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_nIsDraging != 0)
		{
			// 判断正在拖动点1还是点2
			if (m_nIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_rectMouse.left < m_nX2)
				{
					// 更改下限
					m_nX1 =  point.x - m_rectMouse.left;
				}
				else
				{
					// 下限拖过上限，设置为上限-1
					m_nX1 = m_nX2 - 1;
					
					// 重设鼠标位置
					point.x = m_rectMouse.left + m_nX2 - 1;
				}
				
				// 更改Y坐标
				m_nY1 = 255 + m_rectMouse.top - point.y;
			}
			else
			{
				// 正在拖动点2
				
				// 判断是否上限>下限
				if (point.x - m_rectMouse.left > m_nX1)
				{
					// 更改下限
					m_nX2 = point.x - m_rectMouse.left;
				}
				else
				{
					// 下限拖过上限，设置为下限＋1
					m_nX2 = m_nX1 + 1;
					
					// 重设鼠标位置
					point.x = m_rectMouse.left + m_nX1 + 1;
				}
				
				// 更改Y坐标
				m_nY2 = 255 + m_rectMouse.top - point.y;
			}
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			
			// 更新
			UpdateData(FALSE);
			
			// 重绘
			InvalidateRect(m_rectMouse, TRUE);
		}
		else
		{
			CRect	rectTemp1;
			CRect	rectTemp2;
			
			// 计算点1临近区域
			rectTemp1.left = m_rectMouse.left + m_nX1 - 2;
			rectTemp1.right = m_rectMouse.left + m_nX1 + 2;
			rectTemp1.top = 255 + m_rectMouse.top - m_nY1 - 2;
			rectTemp1.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
			
			// 计算点2临近区域
			rectTemp2.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp2.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp2.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp2.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// 判断用户在点1或点2旁边
			if ((rectTemp1.PtInRect(point)) || (rectTemp2.PtInRect(point)))
			{
				// 更改光标
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
	// 判断是否下限超过上限
	if (m_nX1 > m_nX2)
	{
		// 互换
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	CDialog::OnOK();
}

void LinearStepParamDlg::OnKillfocusEDITLinetransX1() 
{
	// TODO: Add your control notification handler code here
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_nX1 > m_nX2)
	{
		// 互换
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}

void LinearStepParamDlg::OnKillfocusEDITLinetransX2() 
{
	// TODO: Add your control notification handler code here
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_nX1 > m_nX2)
	{
		// 互换
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// 更新
		UpdateData(FALSE);
	}
}

void LinearStepParamDlg::OnKillfocusEDITLinetransY1() 
{
	// TODO: Add your control notification handler code here
	// 更新
	UpdateData(TRUE);
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}

void LinearStepParamDlg::OnKillfocusEDITLinetransY2() 
{
	// TODO: Add your control notification handler code here
	// 更新
	UpdateData(TRUE);
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}


