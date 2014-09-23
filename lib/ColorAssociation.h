#pragma once
#include "cinder/Color.h"

class CColorAssociation
{
public:
	float dLch;//0-100
	float dCch;//0-100
	float dHch;//0-360

	void setRGB(cinder::Color &daRGB);
	
	cinder::Color getRGB();
	void setTONE(float dHch,float dToneVal,float dMaxCVal = 0.9);   //L0->CMax->L100
	void setTONE2(float dHch,float dToneVal,float dMaxCVal = 0.9);  //L0->CMax
	void setTONE3(float dHch,float dToneVal,float dMaxCVal = 0.9);  //CMax->L100
	
	CColorAssociation(void);
	CColorAssociation(float Lch,float Cch,float Hch);
	CColorAssociation(CColorAssociation &copy);

	void debug();
};

const CColorAssociation interpolateColor(float fItpVal,CColorAssociation &col1,CColorAssociation &col2);
void  getRand3Color1(CColorAssociation *BgCol,CColorAssociation *InfoCol,CColorAssociation *AccCol,unsigned long seed = 0x010101);//[0]bg color [1]info color [2]accent color

