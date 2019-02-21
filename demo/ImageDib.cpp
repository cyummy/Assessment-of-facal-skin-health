// ImageDib.cpp: implementation of the ImageDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "ImageDib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageDib::ImageDib()
{
	m_lpDib=NULL;//初始化m_lpDib为空。
	m_lpColorTable=NULL;				//颜色表指针为空
	m_pImgData=NULL;  				//图像数据指针为空
	m_lpBmpInfoHead=NULL;				//图像信息头指针为空
	m_hPalette = NULL;					//调色板为空
}

ImageDib::~ImageDib()
{
	//释放m_lpDib所指向的内存缓冲区
	if(m_lpDib != NULL) 
		delete [] m_lpDib; 
	
	//如果有调色板，释放调色板缓冲区
	if(m_hPalette != NULL)
		::DeleteObject(m_hPalette);
}

ImageDib::ImageDib(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
			   unsigned char *pImgData)
{
	//如果没有位图数据传入，我们认为是空的DIB，此时不分配DIB内存
	if(pImgData==NULL){
		m_lpDib=NULL;
		m_lpColorTable=NULL;
		m_pImgData=NULL;  // 图像数据
		m_lpBmpInfoHead=NULL; //  图像信息头
		m_hPalette = NULL;
	}
	else{//如果有位图数据传入
		
		//图像的宽、高、每像素位数等成员变量赋值
		m_imgWidth=size.cx;
		m_imgHeight=size.cy;
		m_nBitCount=nBitCount;
		
		//根据每像素位数，计算颜色表长度
		m_nColorTableLength=ComputeColorTabalLength(nBitCount);
		
		//每行像素所占字节数，必须扩展成4的倍数
		int lineByte=(m_imgWidth*nBitCount/8+3)/4*4;
		
		//位图数据缓冲区的大小（图像大小）
		int imgBufSize=m_imgHeight*lineByte;
		
		//为m_lpDib一次性分配内存，生成DIB结构
		m_lpDib=new BYTE [sizeof(BITMAPINFOHEADER) + 
			sizeof(RGBQUAD) * m_nColorTableLength+imgBufSize];
		
		//填写BITMAPINFOHEADER结构
		m_lpBmpInfoHead = (LPBITMAPINFOHEADER) m_lpDib;
		m_lpBmpInfoHead->biSize = sizeof(BITMAPINFOHEADER);
		m_lpBmpInfoHead->biWidth = m_imgWidth;
		m_lpBmpInfoHead->biHeight = m_imgHeight;
		m_lpBmpInfoHead->biPlanes = 1;
		m_lpBmpInfoHead->biBitCount = m_nBitCount;
		m_lpBmpInfoHead->biCompression = BI_RGB;
		m_lpBmpInfoHead->biSizeImage = 0;
		m_lpBmpInfoHead->biXPelsPerMeter = 0;
		m_lpBmpInfoHead->biYPelsPerMeter = 0;
		m_lpBmpInfoHead->biClrUsed = m_nColorTableLength;
		m_lpBmpInfoHead->biClrImportant = m_nColorTableLength;
		
		//调色板句柄初始化为空，有颜色表时，MakePalette()函数要生成新的调色板
		m_hPalette = NULL;
		//如果有颜色表，则将颜色表拷贝进DIB的颜色表位置
		if(m_nColorTableLength!=0){
			
			//m_lpColorTable指向DIB颜色表的起始位置
			m_lpColorTable=(LPRGBQUAD)(m_lpDib+sizeof(BITMAPINFOHEADER));
			
			//颜色表拷贝
			memcpy(m_lpColorTable,lpColorTable,sizeof(RGBQUAD) * m_nColorTableLength);
			
			//创建逻辑调色板
			MakePalette();
		}
		
		//m_pImgData指向DIB位图数据起始位置
		m_pImgData = (LPBYTE)m_lpDib+sizeof(BITMAPINFOHEADER)+
			sizeof(RGBQUAD) * m_nColorTableLength;
		
		//拷贝图像数据进DIB位图数据区
		memcpy(m_pImgData,pImgData,imgBufSize);
	}
	
}


BOOL ImageDib::Read(LPCTSTR lpszPathName)
{
	//读模式打开图像文件
	CFile file;
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite))
		return FALSE;
	BITMAPFILEHEADER bmfh;
	//读取BITMAPFILEHEADER结构到变量bmfh中
	int nCount=file.Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
	//为m_lpDib分配空间，读取DIB进内存
	if(m_lpDib!=NULL)	delete []m_lpDib;
	m_lpDib=new BYTE[file.GetLength() -sizeof(BITMAPFILEHEADER)];
	file.Read(m_lpDib, file.GetLength() -sizeof(BITMAPFILEHEADER));
	//m_lpBmpInfoHead位置为m_lpDib起始位置
	m_lpBmpInfoHead = (LPBITMAPINFOHEADER)m_lpDib;
	//为成员变量赋值
	m_imgWidth=m_lpBmpInfoHead->biWidth;
	m_imgHeight=m_lpBmpInfoHead->biHeight;
	m_nBitCount=m_lpBmpInfoHead->biBitCount; 
	//计算颜色表长度
	m_nColorTableLength=	ComputeColorTabalLength(m_lpBmpInfoHead->biBitCount);
	//如果有颜色表,则创建逻辑调色板
	m_hPalette = NULL;
	if(m_nColorTableLength!=0){m_lpColorTable=
	(LPRGBQUAD)(m_lpDib+sizeof(BITMAPINFOHEADER));
	    MakePalette();
	}
	//m_pImgData指向DIB的位图数据起始位置
	m_pImgData = (LPBYTE)m_lpDib+sizeof(BITMAPINFOHEADER) +
		sizeof(RGBQUAD) * m_nColorTableLength;
	return TRUE;
}

BOOL ImageDib::Write(LPCTSTR lpszPathName)
{
	//写模式打开文件
	CFile file;
	if (!file.Open(lpszPathName, CFile::modeCreate | CFile::modeReadWrite 
		| CFile::shareExclusive))
		return FALSE;
	
	//填写文件头结构
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;  // 'BM'
	bmfh.bfSize = 0;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
		sizeof(RGBQUAD) * m_nColorTableLength;	
	try {
		//文件头结构写进文件
		file.Write((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		
		//文件信息头结构写进文件
		file.Write(m_lpBmpInfoHead,  sizeof(BITMAPINFOHEADER));
		
		//如果有颜色表的话，颜色表写进文件
		if(m_nColorTableLength!=0)
			file.Write(m_lpColorTable, sizeof(RGBQUAD) * m_nColorTableLength);
		
		//位图数据写进文件
		int imgBufSize=(m_imgWidth*m_nBitCount/8+3)/4*4*m_imgHeight;
		file.Write(m_pImgData, imgBufSize);
	}
	catch(CException* pe) {
		pe->Delete();
		AfxMessageBox("write error");
		return FALSE;
	}
	
	//函数返回
	return TRUE;
}


void ImageDib::MakePalette()
{
	//如果颜色表长度为0，则不创建逻辑调色板
	if(m_nColorTableLength == 0) 
		return;
	//删除旧的逻辑调色板句柄
	if(m_hPalette != NULL) ::DeleteObject(m_hPalette);
	//申请空间，根据颜色表生成LOGPALETTE结构
	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
		m_nColorTableLength * sizeof(PALETTEENTRY)];
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = m_nColorTableLength;
	LPRGBQUAD m_lpDibQuad = (LPRGBQUAD) m_lpColorTable;
	for(int i = 0; i < m_nColorTableLength; i++) {
		pLogPal->palPalEntry[i].peRed = m_lpDibQuad->rgbRed;
		pLogPal->palPalEntry[i].peGreen = m_lpDibQuad->rgbGreen;
		pLogPal->palPalEntry[i].peBlue = m_lpDibQuad->rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
		m_lpDibQuad++;
	}	
	//创建逻辑调色板
	m_hPalette = ::CreatePalette(pLogPal);
	//释放空间
	delete pLogPal;
}

int ImageDib::ComputeColorTabalLength(int nBitCount)
{
	int colorTableLength;
	switch(nBitCount) {
	case 1:
		colorTableLength = 2;
		break;
	case 4:
		colorTableLength = 16;
		break;
	case 8:
		colorTableLength = 256;
		break;
	case 16:
	case 24:
	case 32:
		colorTableLength = 0;
		break;
	default:
		ASSERT(FALSE);
	}
	ASSERT((colorTableLength >= 0) && (colorTableLength <= 256)); 
	return colorTableLength;
}

BOOL ImageDib::Draw(CDC* pDC, CPoint origin, CSize size)
{
	HPALETTE hOldPal=NULL;				//旧的调色板句柄
	if(m_lpDib == NULL) return FALSE;			//如果DIB为空，则返回0
	if(m_hPalette != NULL) {					//如果DIB有调色板
										//将调色板选进设备环境中
		hOldPal=::SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
		pDC->RealizePalette();			
	}
	pDC->SetStretchBltMode(COLORONCOLOR);  //设置位图伸缩模式
	//将DIB在pDC所指向的设备上进行显示
	::StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y, size.cx, size.cy,
		0, 0, m_lpBmpInfoHead->biWidth, m_lpBmpInfoHead->biHeight,m_pImgData,
		 (LPBITMAPINFO) m_lpBmpInfoHead, DIB_RGB_COLORS, SRCCOPY);
	if(hOldPal!=NULL)						//恢复旧的调色板
		::SelectPalette(pDC->GetSafeHdc(), hOldPal, TRUE);
	return TRUE;
}

CSize ImageDib::GetDimensions()
{	
	if(m_lpDib == NULL) return CSize(0, 0);
	return CSize(m_imgWidth, m_imgHeight);
}

void ImageDib::Empty()
{
	//释放DIB内存缓冲区
	if(m_lpDib != NULL) {
		delete [] m_lpDib;
		m_lpDib=NULL;
		m_lpColorTable=NULL;
		m_pImgData=NULL;  
		m_lpBmpInfoHead=NULL;
	}
	//释放逻辑调色板缓冲区
	if(m_hPalette != NULL){
		::DeleteObject(m_hPalette);
		m_hPalette = NULL;
	}
}


void ImageDib::ReplaceDib(CSize size, int nBitCount,  
						LPRGBQUAD lpColorTable,unsigned char *pImgData)
{ 
	//释放原DIB所占空间
	Empty();
	
	//成员变量赋值
	m_imgWidth=size.cx;
	m_imgHeight=size.cy;
	m_nBitCount=nBitCount;
	
	//计算颜色表的长度
	m_nColorTableLength=ComputeColorTabalLength(nBitCount);
	
	//每行像素所占字节数，扩展成4的倍数
	int lineByte=(m_imgWidth*nBitCount/8+3)/4*4;
	
	//位图数据的大小
	int imgBufSize=m_imgHeight*lineByte;
	
	//为m_lpDib重新分配空间，以存放新的DIB
	m_lpDib=new BYTE [sizeof(BITMAPINFOHEADER) + 
		sizeof(RGBQUAD) * m_nColorTableLength+imgBufSize];
	
	//填写位图信息头BITMAPINFOHEADER结构
	m_lpBmpInfoHead = (LPBITMAPINFOHEADER) m_lpDib;
	m_lpBmpInfoHead->biSize = sizeof(BITMAPINFOHEADER);
	m_lpBmpInfoHead->biWidth = m_imgWidth;
	m_lpBmpInfoHead->biHeight = m_imgHeight;
	m_lpBmpInfoHead->biPlanes = 1;
	m_lpBmpInfoHead->biBitCount = m_nBitCount;
	m_lpBmpInfoHead->biCompression = BI_RGB;
	m_lpBmpInfoHead->biSizeImage = 0;
	m_lpBmpInfoHead->biXPelsPerMeter = 0;
	m_lpBmpInfoHead->biYPelsPerMeter = 0;
	m_lpBmpInfoHead->biClrUsed = m_nColorTableLength;
	m_lpBmpInfoHead->biClrImportant = m_nColorTableLength;
	
	//调色板置空
	m_hPalette = NULL;
	
	//如果有颜色表，则将颜色表拷贝至新生成的DIB，并创建逻辑调色板
	if(m_nColorTableLength!=0){
		m_lpColorTable=(LPRGBQUAD)(m_lpDib+sizeof(BITMAPINFOHEADER));
		memcpy(m_lpColorTable,lpColorTable,sizeof(RGBQUAD) * m_nColorTableLength);
		MakePalette();
	}
	
	//m_pImgData指向DIB的位图数据起始位置
	m_pImgData = (LPBYTE)m_lpDib+sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD) * m_nColorTableLength;
	
	//将新位图数据拷贝至新的DIB中
	memcpy(m_pImgData,pImgData,imgBufSize);
}
