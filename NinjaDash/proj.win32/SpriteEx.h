#pragma once

#include "cocos2d.h"

class SpriteEx : public cocos2d::CCSprite
{
public:
	static SpriteEx* create(const char *pszFileName);
};