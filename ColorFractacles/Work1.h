#pragma once
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include <vector>
#include <list>

using namespace ci::app;
using namespace ci;

//movie parameters//
const int WIN_WIDTH  = 1280;
const int WIN_HEIGHT = 720;
const float WIN_FPS = 30.0f;

const char          MOVIE_PATH[100]    = "F:\\GenArt\\No1\\work_no1.mov";
const uint32_t      MOVIE_FOUR_CC = 'avc1';//H.264 'tiff';//no compress
const float         MOVIE_QUOLITY = 0.9f;

//test
#define TOTAL_PARTICLES 4800
#define RESOLUTION 10

//Innter Class
class Particle {
 public:
	Particle();
	Particle( ci::Vec2f );
	void update( const ci::Channel32f &channel );
	void draw();
	
	ci::Vec2f	mLoc;
	ci::Vec2f	mDir;
	float		mVel;
	
	float		mRadius;
	ci::Color	mColor;
};


class Work1 {
 public:
	std::list<Particle>	mParticles;
	int mXRes, mYRes;
	Channel32f mChannel;
	//gl::Texture	mTexture;
	bool mDrawParticles;
	bool mDrawImage; 

	void drawParticles();
	void addParticle( int xRes, int yRes, int res );
	void addParticles( int amt );
	void removeParticles( int amt );

	//virtual functions
	Work1();
	void update( );
	void draw();
	void keyDown( KeyEvent kevent );
};

