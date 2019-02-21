// demo1Doc.cpp : implementation of the CDemo1Doc class
//

#include "stdafx.h"
#include "demo1.h"

#include "demo1Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc

IMPLEMENT_DYNCREATE(CDemo1Doc, CDocument)

BEGIN_MESSAGE_MAP(CDemo1Doc, CDocument)
	//{{AFX_MSG_MAP(CDemo1Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc construction/destruction

CDemo1Doc::CDemo1Doc()
{
	// TODO: add one-time construction code here
	m_dib = new ImageDib;
}

CDemo1Doc::~CDemo1Doc()
{
	if (m_dib != NULL)
	{
		delete m_dib;
		m_dib = 0;
	}
}

BOOL CDemo1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc serialization

void CDemo1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc diagnostics

#ifdef _DEBUG
void CDemo1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDemo1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc commands

BOOL CDemo1Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (m_dib->Read(lpszPathName) == TRUE)
	{	
		SetModifiedFlag(FALSE);     // start off with unmodified
		return TRUE;
	}
	else 
		return 0;

}

BOOL CDemo1Doc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_dib->Write(lpszPathName) == TRUE)	{
		SetModifiedFlag(FALSE);     // have saved
		return TRUE;
	}
	else 
		return 0;	
//	return CDocument::OnSaveDocument(lpszPathName);
}
