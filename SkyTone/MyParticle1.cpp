#include "MyParticle1.h"
#include "param.h"
#include <math.h>

const float NUM_COF_RAD     = 0.1f;
const float NUM_COF_TONE    = 0.3f;
const float RADIUS_SIZE     = 60.0f;
const float START_HUE       = 200.0f;

const float MOVIE_TIME  = 150.0f;//sec

bool MyParticle1::draw(){


	gl::color(mCol.getRGB().r, mCol.getRGB().g, mCol.getRGB().b,0.2);

	gl::drawSolidCircle( mLoc, mfRadius );
	
	return true;
};

bool MyParticle1::setup(Perlin *perlin,unsigned int v){


	this->mNumber      = v;
	this->mfBaseRadius = (MOVIE_WIDTH / TOTAL_H_PARTICLES) / 2;
	
	this->mLoc.y = (v%TOTAL_H_PARTICLES) * 2 * mfBaseRadius + mfBaseRadius;
	this->mLoc.x = floor(v / TOTAL_H_PARTICLES) * 2 * mfBaseRadius + mfBaseRadius;
	
	this->mPerlin = perlin;

	return true;
};

bool MyParticle1::update(unsigned int uiTime){

	ci::Vec3f	      mNoise = mPerlin[0].dfBm(mLoc.x / MOVIE_WIDTH * 3, mLoc.y / MOVIE_WIDTH * 3, uiTime / MOVIE_TIME*2);


	this->mfRadius = this->mfBaseRadius*  sqrt((mNoise.x+1.0)/2);
	this->mfTone   = (mNoise.y+1.0)/2.0;
	//float dHue     = sqrt(mNoise.z)*360.0;

	//mCol.setTONE(100,this->mfTone);
	mCol.setRGB(ColorA(1.0, 1.0, 1.0, mfTone));

	return true;
};
