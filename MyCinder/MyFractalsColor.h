#pragma once
#include "cinder/Perlin.h"
#include "cinder/Vector.h"
#include "MyFractalsBase.h"
#include "MyFractalsParam.h"
#include "MyFractalsConstraint.h"
#include "ColorAssociation.h"

class MyFractalsColor : public MyFractalsBase
{
public:
	MyFractalsColor(void);
	~MyFractalsColor(void);

	virtual bool setup(MyFractalsParam &param,MyFractalsConstraint *fCons);   //children allocat
	virtual bool update(MyFractalsParam &param);  //update position
	virtual bool draw();                          //draw

	//Child Fractals
	ci::Vec3f    mStartPos;        //Your Start Position
	ci::Vec3f    mEndPos;          //Your Start Position

	CColorAssociation mCol;
	MyFractalsConstraint *Constraint;

	MyFractalsColor *mChildren;
	unsigned char   mNumOfChild;
	bool		    mIsDead;
};

