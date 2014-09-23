#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <vector>

class MyFractalsBase
{
public:
	MyFractalsBase(void);
	~MyFractalsBase(void);
	
	virtual bool draw();
	virtual bool setup();
	virtual bool update();

	
};

