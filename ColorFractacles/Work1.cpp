#include "Work1.h"

#include "cinder/Rand.h"


#include "cinder/Color.h"
#include "cinder/Channel.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"



using namespace ci;
using std::list;

Particle::Particle()
{
}

Particle::Particle( Vec2f loc )
{
	mLoc	= loc;
	mDir	= Rand::randVec2f();
	mVel	= 0.0f;
	mRadius	= 4.0f;
}	
	
void Particle::update( const Channel32f &channel )
{
	mRadius = channel.getValue( mLoc ) * 7.0f;

	//float gray = channel.getValue( mLoc );
	//mColor = Color( gray, gray, gray );
	
	mLoc += mDir * mVel;
}

void Particle::draw()
{
	//gl::color( mColor );
	gl::drawSolidCircle( mLoc, mRadius );
}


Work1::Work1(){

	Url url( "http://libcinder.org/media/tutorial/paris.jpg" );
	mChannel = Channel32f( loadImage( loadUrl( url ) ) );
	//mTexture = mChannel;	
	mDrawParticles = true;
	mDrawImage = false;


	mXRes =  WIN_WIDTH/RESOLUTION;
	mYRes =  WIN_HEIGHT/RESOLUTION;
	
	for( int y=0; y<mYRes; y++ ){
		for( int x=0; x<mXRes; x++ ){
			addParticle( x, y, RESOLUTION );
		}
	}
}


void Work1::update( )
{
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		p->update(  mChannel );
	}
}

void Work1::draw()
{
	gl::clear( Color( 0, 0, 0 ), true );
	
	if( mDrawImage ){
		//mTexture.enableAndBind();
		gl::draw( mChannel, getWindowBounds() );
	}
	
	if( mDrawParticles ){
		glDisable( GL_TEXTURE_2D );
		Work1::drawParticles();
	}


}

void  Work1::keyDown( KeyEvent kevent ){
	if( kevent.getChar() == '1' ){
		mDrawImage = ! mDrawImage;
	} else if( kevent.getChar() == '2' ){
		mDrawParticles = ! mDrawParticles;
	}
};


void Work1::drawParticles(){
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		p->draw();
	}
};

void Work1::addParticle( int xi, int yi, int res )
{
	float x = ( xi + 0.5f ) * (float)res;
	float y = ( yi + 0.5f ) * (float)res;
	mParticles.push_back( Particle( Vec2f( x, y ) ) );
}

void Work1::addParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		float x = Rand::randFloat( (float)app::getWindowWidth() );
		float y = Rand::randFloat( (float)app::getWindowHeight() );
		mParticles.push_back( Particle( Vec2f( x, y ) ) );
	}
}

void Work1::removeParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		mParticles.pop_back();
	}
}