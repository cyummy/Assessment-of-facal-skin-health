// ImageDib.h: interface for the ImageDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEDIB_H__4FC00616_753D_4313_8CAE_4B5E8ED02544__INCLUDED_)
#define AFX_IMAGEDIB_H__4FC00616_753D_4313_8CAE_4B5E8ED02544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class ImageDib  
{
	//��Ա����
	public:	
		unsigned char * m_pImgData; 				//ͼ������ָ��
		LPRGBQUAD m_lpColorTable; 				//ͼ����ɫ��ָ��
		int m_nBitCount;						//ÿ����ռ��λ��
	private:
		LPBYTE m_lpDib;						//ָ��DIB��ָ��
		HPALETTE m_hPalette;					//�߼���ɫ����
		int m_nColorTableLength; 					//��ɫ���ȣ����ٸ����
	public:
		int m_imgWidth;							//ͼ��Ŀ�����Ϊ��λ
		int m_imgHeight; 						//ͼ��ĸߣ�����Ϊ��λ
		LPBITMAPINFOHEADER m_lpBmpInfoHead; 	//ͼ����Ϣͷָ��
	//��Ա����
	public:
		ImageDib();							//���캯��
		ImageDib(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);           //�������Ĺ��캯��
		~ImageDib();						//��������
		BOOL Read(LPCTSTR lpszPathName); 		//DIB������
		BOOL Write(LPCTSTR lpszPathName);       //DIBд����
		int ComputeColorTabalLength(int nBitCount);	//������ɫ��ĳ���
		BOOL Draw(CDC* pDC, CPoint origin, CSize size); //ͼ�����
		CSize GetDimensions();					//��ȡͼ��ά��
		void ReplaceDib(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
		unsigned char *pImgData);              	//���µ������滻DIB
	private:
		void MakePalette();						//�����߼���ɫ��
		void Empty();                         	//����ռ�

};

#endif // !defined(AFX_IMAGEDIB_H__4FC00616_753D_4313_8CAE_4B5E8ED02544__INCLUDED_)
