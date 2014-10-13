#include "MyParticle1.h"
#include "param.h"
#include <math.h>

const float NUM_COF_RAD     = 0.1f;
const float NUM_COF_TONE    = 0.3f;
const float RADIUS_SIZE     = 60.0f;
const float START_HUE       = 200.0f;

const float MOVIE_TIME  = 150.0f;//sec

bool MyParticle1::draw(){


	gl::color(mCol.getRGB());

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


	this->mfRadius = this->mfBaseRadius*  sqrt(mNoise.x);
	//this->mfTone   = sqrt(abs(mNoise.y));
	//float dHue     = sqrt(mNoise.z)*360.0;

	//mCol.setTONE3(dHue,this->mfTone);
	mCol.setRGB(Color(1.0, 1.0, 1.0));

	return true;
};
