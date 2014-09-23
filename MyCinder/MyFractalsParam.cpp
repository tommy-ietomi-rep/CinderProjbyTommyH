#include "MyFractalsParam.h"


MyFractalsParam::MyFractalsParam(void){
	mMaxNode = 0;
	mAliveMaxNode = 0;
	mNode    = 0;
	mStartPos = ci::Vec3f(0.0,0.0,0.0);
	//Constraint = NULL;
	//mPerlin    = NULL;
}

MyFractalsParam::MyFractalsParam(MyFractalsParam &copy){
	this->mMaxNode      = copy.mMaxNode;
	this->mAliveMaxNode = copy.mAliveMaxNode;
	this->mNode         = copy.mNode;
	this->mStartPos     = copy.mStartPos;
	this->mHue          = copy.mHue;
	//this->Constraint    = copy.Constraint;
	//this->mPerlin       = copy.mPerlin;
}
