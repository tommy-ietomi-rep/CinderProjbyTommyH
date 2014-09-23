#pragma once
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci::app;
using namespace ci;

class MyParticleBase
{
public:
	virtual bool draw();
	virtual bool setup();
	virtual bool update();
};

