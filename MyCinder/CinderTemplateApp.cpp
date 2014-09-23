#include "cinder/app/AppBasic.h"
#include "cinder/gl/Fbo.h"
#include "cinder/ip/Resize.h"
#include "cinder/qtime/MovieWriter.h"
#include "cinder/Perlin.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
#include "cinder/Rand.h"
#include <sstream>

//const parameter
#include "param.h"

//draw class
#include "MyFractalsColor.h"
#include "MyFractalsParam.h"
#include "MyFractalsConstraint.h"

using namespace ci;
using namespace ci::app;

class ColorFractalApp : public AppBasic {
 public:
	void prepareSettings( Settings *settings );
	void keyDown( KeyEvent event );
	void setup();
	void update();
	void draw();

	Perlin *mPerlin;
	//GenWrokClass *gen;
	qtime::MovieWriterRef	mMovieWriter;
	bool mSaveFrames;
	int  mSaveFrameCnt;

	unsigned int mNumOfTotalFrames;

	gl::Fbo *mFbo;

	//Fractasl
	MyFractalsColor      mMyFractal;
	MyFractalsParam      mFractalParam;
	MyFractalsConstraint mFractalConst;

	// PARAMS
	//params::InterfaceGlRef	mParams;

	// CAMERA
	CameraPersp			mCam;
	Quatf				mSceneRotation;
	float				mCameraDistance;
	Vec3f				mEye, mCenter, mUp;

	//music
	unsigned int        mSpeed;
	unsigned int        mSpeedCnt;

};



void ColorFractalApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( WIN_WIDTH , WIN_HEIGHT );
	settings->setFrameRate( MOVIE_FPS );
}

void ColorFractalApp::setup()
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


	// SETUP CAMERA
	mCameraDistance = 400.0f;
	mEye			= Vec3f( 0.0f, 0.0f, mCameraDistance );
	mCenter			= Vec3f::zero();
	mUp				= Vec3f::yAxis();
	mCam.setPerspective( 75.0f, getWindowAspectRatio(), 5.0f, 2000.0f );
	
	// SETUP PARAMS
	//mParams = params::InterfaceGl::create( "Flocking", Vec2i( 200, 160 ) );
	//mParams->addParam( "Scene Rotation", &mSceneRotation, "opened=1" );
	//mParams->addSeparator();
	//mParams->addParam( "Eye Distance", &mCameraDistance, "min=50.0 max=1500.0 step=50.0 keyIncr=s keyDecr=w" );
	//mParams->addParam( "Center Gravity", &mCentralGravity, "keyIncr=g" );


	

	//SETUP FRACTALS
	//mFractalParam.Constraint    = &mFractalConst;
	mFractalParam.mMaxNode      = 11;
	mFractalParam.mNode         = 0;
	mFractalParam.mAliveMaxNode = 0;
	mFractalParam.mStartPos     = ci::Vec3f(0.5f,0.5f,0.5f);

	//console() << "testddd3"<< std::endl;
	mMyFractal.setup(mFractalParam,&mFractalConst);

	//console() << "test3"<< std::endl;

	//mSpeed
	mSpeed    = 1000;
	mSpeedCnt = 1;
}

void ColorFractalApp::update()
{ 
	
	//update Fractas
	mFractalParam.mAliveMaxNode = 11;//mNumOfTotalFrames/20;
	if(mNumOfTotalFrames%mSpeed==0){
		mFractalParam.mHue = ci::randFloat(0.0f,360.0f);
		mMyFractal.update(mFractalParam);
	}
	mNumOfTotalFrames++;
	mCameraDistance = (float)(mNumOfTotalFrames%mSpeed)*(500.0f/(float)mSpeed)+200;

	// UPDATE CAMERA
	mEye = Vec3f( mCameraDistance*cos(mCameraDistance*0.01f), mCameraDistance*cos(mCameraDistance*0.01f), mCameraDistance );
	mCenter.rotate(Vec3f(0.5f,0.5f,0.0f),30.0f);
	mCam.lookAt( mEye, mCenter, mUp );


	gl::SaveFramebufferBinding bindingSaver;
	mFbo->bindFramebuffer();
	gl::setViewport( mFbo->getBounds() );

	gl::setMatrices( mCam );
	//mSceneRotation = Quatf(60.0f,0.0f,1.0f,0.0f);
	//gl::rotate(Quatf((float)mNumOfTotalFrames*10.0f,0.0,0.0,100.0f));

	gl::clear(Color(BG_COL_R, BG_COL_G, BG_COL_B), true);
	gl::enableDepthRead();
	gl::enableDepthWrite();

	//gl::enable( GL_LIGHTING );
	//gl::enable( GL_LIGHT0 );

	//GLfloat light_position[] = { 0.001f,0.001f,mCameraDistance ,0.00001 };
	//glLightfv( GL_LIGHT0, GL_POSITION,  light_position);

	// DRAW PARTICLES
	//gl::begin(GL_QUADS);
	mMyFractal.draw();
	//gl::end();
	gl::color(Color(1.0f, 1.0f, 1.0f));

	mFbo->unbindFramebuffer();
	
	//SpeedCnt
	if(mSpeedCnt<1000)mSpeedCnt++;

}

void ColorFractalApp::draw()
{	
	//Fbo to Window
	gl::clear( Color( 0.0f, 0.0f, 0.0f ), true );
	gl::setViewport( getWindowBounds() );    
	gl::setMatricesWindow( getWindowSize() );
	gl::draw( mFbo->getTexture(0), getWindowBounds()/*Rectf( 0.0f, 0.0f, (float)WIN_WIDTH, (float)WIN_HEIGHT )*/ );
	
	//params
	//mParams->draw();

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
}

void ColorFractalApp::keyDown( KeyEvent kevent )
{
	if( kevent.getChar() == 'a' ){
	   mSaveFrames = true; 
	}else if ( kevent.getChar() == 'f' ){
	   setFullScreen( ! isFullScreen() );
	}else if ( kevent.getChar() == 'z' ){
	  mSpeed = mSpeedCnt;
	  mSpeedCnt = 1;
	  mNumOfTotalFrames = 0;
	}
}


CINDER_APP_BASIC( ColorFractalApp, RendererGl )
