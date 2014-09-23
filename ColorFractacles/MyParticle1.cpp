#include "MyParticle1.h"
#include "param.h"
#include <math.h>

const float NUM_COF_RAD     = 0.1f;
const float NUM_COF_TONE    = 0.3f;
const float RADIUS_SIZE     = 60.0f;
const float START_HUE       = 200.0f;

const float MOVIE_TIME  = 150.0f;//sec

bool MyParticle1::draw(){

	if(this->mfLife){
		gl::color(mCol.getRGB());
		gl::drawSolidCircle( mLoc, mfRadius );
	}
	return true;
};

bool MyParticle1::setup(Perlin *perlin,unsigned int uiNUmber){

	this->mfLife    = false;
	this->mNumber   = uiNUmber;
	
	float fTmpRand  = perlin[0].fBm((float)this->mNumber*NUM_COF_RAD)*4.0f+0.5f;
	this->mLoc.y    = MOVIE_HEIGHT*(0.15f+0.7f * fTmpRand);
	this->mfRadius  = (float)(fTmpRand/2.0+0.5)*RADIUS_SIZE;

	//console() << fTmpRand  << std::endl;

	this->mfTone = 0.8f*(perlin[1].fBm(this->mNumber*NUM_COF_TONE)+0.5f);
	
	return true;
};

bool MyParticle1::update(unsigned int uiTime){

	if(this->mfLife){
		//idou
		this->mSpeed.x *= 1.001;
		this->mLoc.x += this->mSpeed.x;
		this->mSpeed.y *= 1.002;
		this->mLoc.y += this->mSpeed.y;
		
		//shibouhantei
		if (this->mLoc.x<-RADIUS_SIZE*1.5 || this->mLoc.y>MOVIE_WIDTH || this->mLoc.y<0){
			this->mfLife = false;
			
		}
	}else{
		//Time
		unsigned int    iThres = (unsigned int)MOVIE_TIME * (unsigned int)MOVIE_FPS *2 /3;
		unsigned int    iTime  = (uiTime<iThres)? uiTime : iThres + (unsigned int)pow(uiTime-iThres,1.17);

		unsigned int    iNumTh = iThres / TOTAL_PARTICLES;

		if( uiTime/iNumTh >= this->mNumber && uiTime < MOVIE_TIME * MOVIE_FPS ){

			//birth of Particle
			this->mfLife = true;

			//Location & Speed SetUp
			this->mLoc.x    = MOVIE_WIDTH + RADIUS_SIZE*1.5f;
			
			this->mSpeed.x  = Rand::randFloat(-2,-1);
			this->mSpeed.y  = Rand::randFloat(-0.5,0.5);

			float dTimeCof = 380.0f / (float)iThres;
			float dHue     = (float)((int)(START_HUE+iTime*dTimeCof)%360);

			//SetColor
			mCol.setTONE3(dHue,this->mfTone);

		}
	}

	return true;
};
