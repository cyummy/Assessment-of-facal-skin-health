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
    //输出图像每像素位数
	int m_nBitCountOut;
	
	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;
	
	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;
private:
	//输出图像的宽
	int m_imgWidthOut;
	
	//输出图像的高
	int m_imgHeightOut;
	
	//输出图像颜色表长度
	int m_nColorTableLengthOut;
	
public:
	//构造函数
	GeometryTrans();
	
	//带参数的构造函数
	GeometryTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
	
	//析构函数
	~GeometryTrans();
	
	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();
	
	//平移
	void Move(int offsetX, int offsetY);
	
	//缩放
	void Zoom(float ratioX, float ratioY);//缩放
	
	//水平镜像
	void MirrorHorTrans();
	
	//垂直镜像
	void MirrorVerTrans();
	
	//顺时针旋转90度
	void Clockwise90();
	
	//逆时针旋转90度
	void Anticlockwise90();
	
	//旋转180
	void Rotate180();
	
	//0-360度之间任意角度旋转 
	void Rotate(int angle);//angle旋转角度
};

#endif // !defined(AFX_GEOMETRYTRANS_H__02D0603E_60BD_4147_9F76_2CF74447F2F6__INCLUDED_)
