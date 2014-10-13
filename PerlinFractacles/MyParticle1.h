#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include "MyParticleBase.h"
#include "ColorAssociation.h"

const int NUM_OF_PERLIN = 3;

const int TOTAL_H_PARTICLES = 100;
const int TOTAL_PARTICLES = TOTAL_H_PARTICLES*TOTAL_H_PARTICLES;

class MyParticle1 : public MyParticleBase
{
protected:
	unsigned int      mNumber;
	ci::Vec2f	      mLoc;
	float             mfBaseRadius;
	Perlin            *mPerlin;

	CColorAssociation mCol;

	float             mfRadius;
	float             mfTone;


public:
	virtual bool draw();
	virtual bool setup(Perlin *perlin1,unsigned int uiNUmber);
	virtual bool update(unsigned int uiTime);
};

