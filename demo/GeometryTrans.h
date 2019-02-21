// GeometryTrans.h: interface for the GeometryTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMETRYTRANS_H__02D0603E_60BD_4147_9F76_2CF74447F2F6__INCLUDED_)
#define AFX_GEOMETRYTRANS_H__02D0603E_60BD_4147_9F76_2CF74447F2F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class GeometryTrans : public ImageDib  
{
public:
    //���ͼ��ÿ����λ��
	int m_nBitCountOut;
	
	//���ͼ��λͼ����ָ��
	unsigned char * m_pImgDataOut;
	
	//���ͼ����ɫ��
	LPRGBQUAD m_lpColorTableOut;
private:
	//���ͼ��Ŀ�
	int m_imgWidthOut;
	
	//���ͼ��ĸ�
	int m_imgHeightOut;
	
	//���ͼ����ɫ����
	int m_nColorTableLengthOut;
	
public:
	//���캯��
	GeometryTrans();
	
	//�������Ĺ��캯��
	GeometryTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
	
	//��������
	~GeometryTrans();
	
	//������Ϊ��λ�������ͼ��Ŀ�͸�
	CSize GetDimensions();
	
	//ƽ��
	void Move(int offsetX, int offsetY);
	
	//����
	void Zoom(float ratioX, float ratioY);//����
	
	//ˮƽ����
	void MirrorHorTrans();
	
	//��ֱ����
	void MirrorVerTrans();
	
	//˳ʱ����ת90��
	void Clockwise90();
	
	//��ʱ����ת90��
	void Anticlockwise90();
	
	//��ת180
	void Rotate180();
	
	//0-360��֮������Ƕ���ת 
	void Rotate(int angle);//angle��ת�Ƕ�
};

#endif // !defined(AFX_GEOMETRYTRANS_H__02D0603E_60BD_4147_9F76_2CF74447F2F6__INCLUDED_)
