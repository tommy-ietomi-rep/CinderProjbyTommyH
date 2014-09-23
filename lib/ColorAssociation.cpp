#include "ColorAssociation.h"
#include "cinder/app/AppBasic.h"
#include "cinder/qtime/MovieWriter.h"
#include "cinder/Rand.h"

#include <math.h>

const float PI = 3.14159265358f;

const float XYZ2sRGB[3][3] = {{3.240479f,-1.537150f,-0.498535f},{-0.969256f,1.875992f,0.041556f},{0.055648f,-0.204043f,1.057311f}};
const float sRGB2XYZ[3][3] = {{0.4124532201f,0.3575795813f,0.18042259f},{0.2126711213f,0.7151592053f,0.0721687768f},{0.0193338165f,0.1191935402f,0.9502269223f}};
const float XYZ_D65[3]     = {0.9504553914f,0.9999991034f,1.088754279f};

//[Hue][Max_L/Max_C]
const float MAX_CVAL_sRGB[13][2] = {
{	55	,	90	},
{	63	,	90	},
{	80	,	90	},
{	90	,	95	},
{	90	,	102	},
{	90	,	87	},
{	90	,	65	},
{	85	,	58	},
{	75	,	60	},
{	64	,	75	},
{	50	,	95	},
{	60	,	100	},
{	55	,	88	}
};

//{{55.0f,84.0f},{53.0f,92.0f},{67.0f,85.0f},{93.0f,95.0f},{92.0f,102.0f},{88.0f,87.0f},{90.0f,56.0f},{85.0f,45.0f},{75.0f,44.0f},{64.0f,56.0f},{41.0f,107.0f},{60.0f,110.0f},{55.0f,84.0f}};

float getMacC(float dLch,float dHch){
	
	int iHueNum = ((int)dHch %360) / 30;

	float MaxC[2];

	for(int i=0;i<2;i++){
		float dMaxL = MAX_CVAL_sRGB[iHueNum][0];
		float dMaxC = MAX_CVAL_sRGB[iHueNum][1];

		if(dLch < dMaxL){
			MaxC[i] = dMaxC * dLch / dMaxL;
		}else{
			MaxC[i] = dMaxC * (1.0f - (dLch - dMaxL)/(100.0f - dMaxL));
		}

		iHueNum++;
	}

	return (MaxC[0] + (MaxC[1]-MaxC[0])* (float)(((int)dHch %30))/30.0f);
};

float getMacL(float dHch){
	
	int iHueNum = ((int)dHch %360) / 30;
	float dMaxL0 = MAX_CVAL_sRGB[iHueNum  ][0];
	float dMaxL1 = MAX_CVAL_sRGB[iHueNum+1][0];
	return (dMaxL0 + (dMaxL1-dMaxL0)* (float)(((int)dHch %30))/30.0f);
};

class cCaXYZ{
public:
	float dXch;//0-1
	float dYch;//0-1
	float dZch;//0-1
};

cCaXYZ convsRGB2XYZ(cinder::Color &Rgb){

	cinder::Color cLinRgb;
	cLinRgb.r = pow(Rgb.r,2.4f);//(Rgb.r<0.04045)?Rgb.r/12.92:pow((Rgb.r+0.055)/1.055,2.4);
	cLinRgb.g = pow(Rgb.g,2.4f);//(Rgb.g<0.04045)?Rgb.g/12.92:pow((Rgb.g+0.055)/1.055,2.4);
	cLinRgb.b = pow(Rgb.b,2.4f);//(Rgb.b<0.04045)?Rgb.b/12.92:pow((Rgb.b+0.055)/1.055,2.4);

	cLinRgb.r = (cLinRgb.r<0.0)?0.0f:(cLinRgb.r>1.0f)?1.0f:cLinRgb.r;
	cLinRgb.g = (cLinRgb.g<0.0)?0.0f:(cLinRgb.g>1.0f)?1.0f:cLinRgb.g;
	cLinRgb.b = (cLinRgb.b<0.0)?0.0f:(cLinRgb.b>1.0f)?1.0f:cLinRgb.b;


	cCaXYZ cXyz;
	cXyz.dXch = sRGB2XYZ[0][0]*cLinRgb.r+sRGB2XYZ[0][1]*cLinRgb.g+sRGB2XYZ[0][2]*cLinRgb.b;
	cXyz.dYch = sRGB2XYZ[1][0]*cLinRgb.r+sRGB2XYZ[1][1]*cLinRgb.g+sRGB2XYZ[1][2]*cLinRgb.b;
	cXyz.dZch = sRGB2XYZ[2][0]*cLinRgb.r+sRGB2XYZ[2][1]*cLinRgb.g+sRGB2XYZ[2][2]*cLinRgb.b;
	
	return cXyz;
};

cinder::Color convsXYZ2RGB(cCaXYZ &Xyz){

	cinder::Color cLinRgb;
	cLinRgb.r = XYZ2sRGB[0][0]*Xyz.dXch+XYZ2sRGB[0][1]*Xyz.dYch+XYZ2sRGB[0][2]*Xyz.dZch;
	cLinRgb.g = XYZ2sRGB[1][0]*Xyz.dXch+XYZ2sRGB[1][1]*Xyz.dYch+XYZ2sRGB[1][2]*Xyz.dZch;
	cLinRgb.b = XYZ2sRGB[2][0]*Xyz.dXch+XYZ2sRGB[2][1]*Xyz.dYch+XYZ2sRGB[2][2]*Xyz.dZch;

	cLinRgb.r = (cLinRgb.r<0.0)?0.0f:(cLinRgb.r>1.0)?1.0f:cLinRgb.r;
	cLinRgb.g = (cLinRgb.g<0.0)?0.0f:(cLinRgb.g>1.0)?1.0f:cLinRgb.g;
	cLinRgb.b = (cLinRgb.b<0.0)?0.0f:(cLinRgb.b>1.0)?1.0f:cLinRgb.b;

	cinder::Color cRgb;
	cRgb.r = pow(cLinRgb.r,1.0f/2.4f);//(cLinRgb.r<0.0031308)?cLinRgb.r*12.92:1.055*pow(cLinRgb.r,1.0/2.4)-0.055;
	cRgb.g = pow(cLinRgb.g,1.0f/2.4f);(cLinRgb.g<0.0031308)?cLinRgb.g*12.92:1.055*pow(cLinRgb.g,1.0/2.4)-0.055;
	cRgb.b = pow(cLinRgb.b,1.0f/2.4f);(cLinRgb.b<0.0031308)?cLinRgb.b*12.92:1.055*pow(cLinRgb.b,1.0/2.4)-0.055;

	return cRgb;
};


CColorAssociation::CColorAssociation(void){
	this->dLch = 0.0;
	this->dCch = 0.0;
	this->dHch = 0.0;
};

CColorAssociation::CColorAssociation(float Lch,float Cch,float Hch){
    this->dLch = Lch;
	this->dCch = Cch;
	this->dHch = Hch;
};

CColorAssociation::CColorAssociation(CColorAssociation &copy){
    this->dLch = copy.dLch;
	this->dCch = copy.dCch;
	this->dHch = copy.dHch;
};

void CColorAssociation::setRGB(cinder::Color &daRGB){
	cCaXYZ cXyz = convsRGB2XYZ(daRGB);

	float Lab[3];	
	float XXn,YYn,ZZn;

	if ((cXyz.dXch / XYZ_D65[0]) > pow(6.0/29.0,3.0))	XXn = (float)(pow((cXyz.dXch / XYZ_D65[0]), (1.0 / 3.0))); 
	else						                        XXn = (float)(7.787037037f * (cXyz.dXch / XYZ_D65[0]) + 16.0f / 116.0f);
	if ((cXyz.dYch / XYZ_D65[1]) > pow(6.0/29.0,3.0))	YYn = (float) (pow((cXyz.dYch / XYZ_D65[1]), (1.0 / 3.0))); 
	else						                        YYn = (float)(7.787037037f * (cXyz.dYch / XYZ_D65[1]) + 16.0f / 116.0f);
	if ((cXyz.dZch / XYZ_D65[2]) > pow(6.0/29.0,3.0))	ZZn = (float)(pow((cXyz.dZch / XYZ_D65[2]), (1.0 / 3.0))); 
	else						                        ZZn = (float)(7.787037037f * (cXyz.dZch / XYZ_D65[2]) + 16.0f / 116.0f);

	Lab[0] = (116.0f * YYn - 16.0f); 
	Lab[1] = (500.0f * (XXn - YYn));
	Lab[2] = (200.0f * (YYn - ZZn));
	
	this->dLch = Lab[0];
	this->dCch = sqrt(Lab[1]*Lab[1]+Lab[2]*Lab[2]);
	this->dHch = atan2(Lab[2],Lab[1])/(2.0f*PI)*360.0f;
	
	return;

};



cinder::Color CColorAssociation::getRGB(){
	float Lab[3];
	Lab[0] = this->dLch;
	Lab[1] = this->dCch*cos(this->dHch/360.0f*2.0f*PI);
	Lab[2] = this->dCch*sin(this->dHch/360.0f*2.0f*PI);

	float XXn,YYn,ZZn;

	YYn = (Lab[0]+16.0f)/116.0f;
	XXn = YYn+Lab[1]/500.0f;
	ZZn = YYn-Lab[2]/200.0f;

	cCaXYZ cXyz;
	if(XXn>6.0/29.0) cXyz.dYch = XYZ_D65[1]*pow(YYn,3.0f);
	else             cXyz.dYch = XYZ_D65[1]*(YYn-16.0f/116.0f)*3*(6.0f/29.0f)*6.0f/29.0f;

	if(XXn>6.0/29.0) cXyz.dXch = XYZ_D65[0]*pow(XXn,3.0f);
	else             cXyz.dXch = XYZ_D65[0]*(XXn-16.0f/116.0f)*3*(6.0f/29.0f)*6.0f/29.0f;

	if(XXn>6.0/29.0) cXyz.dZch = XYZ_D65[2]*pow(ZZn,3.0f);
	else             cXyz.dZch = XYZ_D65[2]*(ZZn-16.0f/116.0f)*3*(6.0f/29.0f)*6.0f/29.0f;

	cinder::Color cRgb = convsXYZ2RGB(cXyz);
	return cRgb;

};



void CColorAssociation::setTONE(float diHch,float diToneVal,float diMaxCVal){

	diToneVal = (diToneVal>1.0)?1.0f:diToneVal;
	diToneVal = (diToneVal<0.0)?0.0f:diToneVal;

	this->dHch = diHch;
	this->dLch = 10.0f + diToneVal*80.0f;
	
	this->dCch = diMaxCVal * getMacC(this->dLch,this->dHch);

	return;
};

void CColorAssociation::setTONE2(float diHch,float diToneVal,float diMaxCVal){

	diToneVal = (diToneVal>1.0)?1.0f:diToneVal;
	diToneVal = (diToneVal<0.0)?0.0f:diToneVal;

	this->dHch = diHch;
	this->dLch = diToneVal*getMacL(diHch);
	this->dCch = diMaxCVal * getMacC(this->dLch,this->dHch);

	return;
};

void CColorAssociation::setTONE3(float diHch,float diToneVal,float diMaxCVal){

	diToneVal = (diToneVal>1.0)?1.0f:diToneVal;
	diToneVal = (diToneVal<0.0)?0.0f:diToneVal;

	this->dHch = diHch;

	float fMaxL = getMacL(diHch);
	this->dLch = (1.0f - diToneVal) * (100.0f-fMaxL)+fMaxL;
	this->dCch = diMaxCVal * getMacC(this->dLch,this->dHch);

	return;
};

const CColorAssociation interpolateColor(float fItpVal,CColorAssociation &col1,CColorAssociation &col2){
	
	CColorAssociation col;
	col.dLch = col1.dLch + (col2.dLch - col1.dLch)*fItpVal;
	col.dCch = col1.dCch + (col2.dCch - col1.dCch)*fItpVal;
	col.dHch = col1.dHch + (col2.dHch - col1.dHch)*fItpVal;

	return col;
};

void  getRand3Color1(CColorAssociation *BgCol,CColorAssociation *InfoCol,CColorAssociation *AccCol,unsigned long seed){
	cinder::Rand rand(seed);

	float fPlusMinusInfo = (rand.randFloat(0.0f,1.0f)>0.5f)?-1.0f:1.0f;
	float fPlusMinusAcc  = (rand.randFloat(0.0f,1.0f)>0.5f)?-1.0f:1.0f;

	float fBgHue        = rand.randFloat(0.0f,360.0f);
	float fInfoHue      = (float)((int)(fBgHue + fPlusMinusInfo * rand.randFloat(120.0f,150.0f))%360);
	float fAccHue       = (float)((int)(fBgHue + fPlusMinusAcc  * rand.randFloat(120.0f,150.0f))%360);

	BgCol   ->setTONE3(fBgHue   , rand.randFloat(0.2f,0.9f));
	InfoCol ->setTONE2(fInfoHue , rand.randFloat(0.3f,1.0f));
	AccCol  ->setTONE2(fAccHue  , rand.randFloat(0.3f,1.0f));


};

void CColorAssociation::debug(){

	for(int h=0;h<=360;h+=15){
		
		cinder::Surface32f sf(180,100,1);

		char fn[100];
		sprintf_s(fn,"test%d.csv",h);
		FILE *fp;
		fopen_s(&fp,fn,"w");

		fprintf(fp,",");
		for(int c=0;c<=180;c++)fprintf(fp,"%d,",c);
		fprintf(fp,"\n");

		for(int l=0;l<=100;l++){
			fprintf(fp,"%d,",l);
			for(int c=0;c<=180;c++){

				/*Col.dLch = (double)l;
				Col.dCch = (double)c;
				Col.dHch = (double)h;

				Color rgb =  Col.getRGB();

				Col.setRGB(rgb);
				rgb =  Col.getRGB();*/
				setTONE((float)h,(float)l/100.0f,0.9f);
				cinder::Color rgb =  getRGB();

				//if(rgb.r>0.0 && rgb.r<1.0 && 
				//	rgb.g>0.0 && rgb.g<1.0 && 
				//	rgb.b>0.0 && rgb.b<1.0 )
				//{
				sf.setPixel(cinder::Vec2i(c,l),rgb);
				fprintf(fp,"1,");
				//}
				//else{
				//	sf.setPixel(Vec2i(c,l),Color(0.0,0.0,0.0));
				//	fprintf(fp,"0,");
				//}
			}
			fprintf(fp,"\n");

		}
		fclose(fp);
		sprintf_s(fn,"test%d.tiff",h);
		cinder::writeImage(fn, sf );
	}
};