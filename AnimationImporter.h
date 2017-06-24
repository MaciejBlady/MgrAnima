#pragma once

#include <iostream>
#include <list>
#include <string>
#include "fbxsdk.h"
#include "Common/Common.h"
#include "Animation.h"

class AnimationImporter
{
public:
	AnimationImporter(char* filename);
	~AnimationImporter();
private:
	int _animationStackCount;
	std::list<Animation> _animations;
};

