#include "cinder/app/AppBasic.h"
#include "cinder/gl/Fbo.h"
#include "cinder/ip/Resize.h"
#include "cinder/qtime/MovieWriter.h"
#include "cinder/Perlin.h"
#include "cinder/Camera.h"
#include <sstream>

//const parameter
#include "param.h"

//draw class
#include "MyParticleBase.h"
#include "MyParticle1.h"

using namespace ci;
using namespace ci::app;

class TempleteApp : public AppBasic {
 public:
	void prepareSettings( Settings *settings );
	void keyDown( KeyEvent event );
	void setup();
	void update();
	void draw();

	Perlin *mPerlin;
	GenWrokClass *gen;
	qtime::MovieWriterRef	mMovieWriter;
	bool mSaveFrames;
	int  mSaveFrameCnt;

	unsigned int mNumOfTotalFrames;

	gl::Fbo *mFbo;

};



void TempleteApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( WIN_WIDTH , WIN_HEIGHT );
	settings->setFrameRate( MOVIE_FPS );
}

void TempleteApp::setup()
{
	mNumOfTotalFrames = 0;

	gl::Fbo::Format msaaFormat;
    msaaFormat.setSamples( 4 );
	mFbo              = new gl::Fbo(MOVIE_WIDTH,MOVIE_HEIGHT,msaaFormat);

	mSaveFrames       = false;
	mSaveFrameCnt     = 0;

	//for movie
	if(MOVIE_ON){
		fs::path path = MOVIE_PATH;//getSaveFilePath();
		qtime::MovieWriter::Format format( MOVIE_FOUR_CC,MOVIE_QUOLITY);
		mMovieWriter = qtime::MovieWriter::create( path, MOVIE_WIDTH, MOVIE_HEIGHT, format );
	}


	//Set Up Particles
	mPerlin  = new Perlin[NUM_OF_PERLIN];
	for(int i=0;i<NUM_OF_PERLIN;i++){
		mPerlin[i].setOctaves((uint8_t)PERLIN_OCT[i]);
		mPerlin[i].setSeed(PERLIN_SEED[i]);
	}
	gen = new GenWrokClass[TOTAL_PARTICLES];
	for(int i=0;i<TOTAL_PARTICLES;i++){
		gen[i].setup( mPerlin ,i);
	}

	mFbo->bindFramebuffer();
	gl::clear(Color(BG_COL_R, BG_COL_G, BG_COL_B), true);
	mFbo->unbindFramebuffer();
}

void TempleteApp::update()
{ 
	
	for(int i=0;i<TOTAL_PARTICLES;i++){
		gen[i].update( mNumOfTotalFrames );
	}

	mNumOfTotalFrames++;

	mFbo->bindFramebuffer();
	gl::setViewport( mFbo->getBounds() );
	
	CameraPersp cam( mFbo->getWidth(), mFbo->getHeight(), MOVIE_FPS );
	gl::setMatrices( cam );

	gl::clear( Color( BG_COL_R, BG_COL_G, BG_COL_B ), true );

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gl::enable(GL_BLEND);

	for(int i=0;i<TOTAL_PARTICLES;i++){
		gen[i].draw();
	}


	mFbo->unbindFramebuffer();

	gl::color(Color( BG_COL_R, BG_COL_G, BG_COL_B ));
}

void TempleteApp::draw()
{	

	// add this frame to our movie
	if( MOVIE_ON && mMovieWriter )
		mMovieWriter->addFrame( this->mFbo->getTexture() );
  

		//
	if( mSaveFrames ){
		ImageTarget::Options opt;
		std::stringstream ss;
		ss << IMAGE_PATH <<"_"<< this->mSaveFrameCnt<<"."<<IMAGE_EXT;
		writeImage(ss.str(), this->mFbo->getTexture() , opt ,IMAGE_EXT);
		//writeImage(ss.str(), copyWindowSurface(), opt ,IMAGE_EXT);
		mSaveFrames = false;
		mSaveFrameCnt++;
	}


	gl::setViewport( getWindowBounds() );
	gl::setMatricesWindow( getWindowSize() );
	gl::draw( mFbo->getTexture(0), Rectf( 0.0f, 0.0f, (float)WIN_WIDTH, (float)WIN_HEIGHT ) );
}

void TempleteApp::keyDown( KeyEvent kevent )
{
	if( kevent.getChar() == 'a' ){
	   mSaveFrames = true; 
	} 
}


CINDER_APP_BASIC( TempleteApp, RendererGl )
