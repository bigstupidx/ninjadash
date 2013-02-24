#ifndef __GAMEMENU_SCENE_H__
#define __GAMEMENU_SCENE_H__

#include "cocos2d.h"

class GameMenu : public cocos2d::CCLayer
{
	typedef unsigned char Menu_Type;enum
	{
		MT_StartGame,
		MT_Setting,
		MT_Ranking,
		MT_MoreGame,
		MT_Exit,

		MT_Count
	};
public:
	virtual bool init();  
	static cocos2d::CCScene* scene();
	void menuCallback(CCObject* pSender);
	CREATE_FUNC( GameMenu );
};

#endif