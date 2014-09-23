#pragma once
#include "cinder/Perlin.h"
#include "cinder/Vector.h"
#include "MyFractalsParam.h"
#include "ColorAssociation.h"

class MyFractalsConstraint
{
public:
	MyFractalsConstraint(void);
	~MyFractalsConstraint(void);
    
	ci::Vec3f         setEndpoint(MyFractalsParam &param);
	CColorAssociation setColor   (MyFractalsParam &param);
};

