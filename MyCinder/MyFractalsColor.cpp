#include "MyFractalsColor.h"
#include "cinder/gl/gl.h"

MyFractalsColor::MyFractalsColor(void){
	//Child Fractals
	mStartPos  = ci::Vec3f(0.0,0.0,0.0);        //Your Start Position
	mEndPos    = ci::Vec3f(0.0,0.0,0.0);          //Your Start Position

	mChildren   = NULL;
	mNumOfChild = 0;
	mIsDead     = true;
};

MyFractalsColor::~MyFractalsColor(void){
	if(mNumOfChild!=0)delete [] mChildren;
};

bool MyFractalsColor::setup(MyFractalsParam &param,MyFractalsConstraint *fCons){
	if(param.mNode<param.mMaxNode){

		MyFractalsParam cparam(param);
		cparam.mNode++;

		this->mNumOfChild = 2;
		this->mChildren   = new MyFractalsColor[2];
		
		for(int i=0;i<mNumOfChild;i++){
			this->mChildren[i].setup(cparam,fCons);
		}

		return true;
	}else{
		return false;
	}
};

bool MyFractalsColor::update(MyFractalsParam &param){
	if(param.mNode<param.mAliveMaxNode){
		
		//Alive
		this->mIsDead = false;

		//UpdatePosition
		this->mStartPos = param.mStartPos;
		this->mEndPos   = this->Constraint->setEndpoint(param);
		this->mCol      = this->Constraint->setColor(param);


		//Update Children Node
		MyFractalsParam cparam(param);
		cparam.mNode++;
		cparam.mStartPos = this->mEndPos;

		for(int i=0;i<this->mNumOfChild;i++){
			this->mChildren[i].update(cparam);
		}

		return true;
	}else{
		return false;
	}
};

bool MyFractalsColor::draw(){
	if(!this->mIsDead){
		ci::gl::color(mCol.getRGB());
		//ci::gl::drawLine(this->mStartPos,this->mEndPos);
		//glMaterialfv( GL_FRONT, GL_DIFFUSE,	mCol.getRGB() );
		ci::gl::drawCube(this->mStartPos,this->mEndPos*0.5f);

		for(int i=0;i<this->mNumOfChild;i++){
			this->mChildren[i].draw();
		}
		return true;
	}else{
		return false;
	}
};

