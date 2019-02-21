// demo1Doc.h : interface of the CDemo1Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMO1DOC_H__AAFD1A30_4BF5_4F8F_AA69_FBF9B91153BA__INCLUDED_)
#define AFX_DEMO1DOC_H__AAFD1A30_4BF5_4F8F_AA69_FBF9B91153BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class CDemo1Doc : public CDocument
{
protected: // create from serialization only
	CDemo1Doc();
	DECLARE_DYNCREATE(CDemo1Doc)

// Attributes
public:
	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo1Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemo1Doc();
	//获取m_dib的指针
	ImageDib *GetPDib()
	{
		return m_dib;
	}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
//private:
public:
	//ImgDib类的对象，从打开文件中读入的数据放在该对象中
	ImageDib* m_dib;
protected:
	//{{AFX_MSG(CDemo1Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO1DOC_H__AAFD1A30_4BF5_4F8F_AA69_FBF9B91153BA__INCLUDED_)
