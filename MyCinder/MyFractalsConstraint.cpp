#include "MyFractalsConstraint.h"
#include "cinder/Rand.h"

MyFractalsConstraint::MyFractalsConstraint(void)
{
}


MyFractalsConstraint::~MyFractalsConstraint(void)
{
}

ci::Vec3f         MyFractalsConstraint::setEndpoint(MyFractalsParam &param){
	CColorAssociation col;
	return param.mStartPos + ci::randVec3f()*50.0f;
};

CColorAssociation MyFractalsConstraint::setColor   (MyFractalsParam &param){
	CColorAssociation col;
	col.setTONE2(param.mHue,ci::randFloat(0.8f,1.0f));
	return col;
};
