#pragma once
#include "cinder/Vector.h"
//#include "MyFractalsConstraint.h"


class MyFractalsParam
{
public:
	MyFractalsParam(void);
	MyFractalsParam(MyFractalsParam &copy);

	unsigned int mMaxNode;         //Number Of Max Node
	unsigned int mAliveMaxNode;    //Number Of Alive Node
	unsigned int mNode;            //Your Node
	ci::Vec3f    mStartPos;        //Your Start Position

	float        mHue;


	
	//ci::Perlin              *mPerlin;     //Pointer Of Perlin Class
};
