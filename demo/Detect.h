#ifndef DETECT_H_
#define DETECT_H_

#include "ImageDib.h"

class Detect:public ImageDib
{
public:
	int m_nBitCountOut; 
	unsigned char * m_pImgDataOut;
	LPRGBQUAD m_lpColorTableOut;
	int m_imgWidthOut;
	int m_imgHeightOut;
	int m_nColorTableLengthOut;

    int m_nBitCountOut1;	
    unsigned char * m_pImgDataOut1;
	LPRGBQUAD m_lpColorTableOut1;
	int m_nColorTableLengthOut1;

    struct zw{
		double asmm,ent,con,idm;  //定义结构：角二阶矩 熵 对比度 逆差矩
	};

public:
	Detect();
	Detect(CSize size,int nBitCount, LPRGBQUAD lpColorTable , unsigned char *pImgData);
	virtual ~Detect();

public:
	/*double ZwJc();*/
	zw Wrinkle();
	int Pot();
	CSize GetDimensions();
};

#endif
   