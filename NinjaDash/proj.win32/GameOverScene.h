#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::CCLayer
{
	typedef unsigned char Menu_Type;enum
	{
		MT_Menu,
		MT_Retry,

		MT_Count
	};
public:
	virtual bool init();  
	static cocos2d::CCScene* scene(int aScroe);
	void menuCallback(CCObject* pSender);
	void onEnter();
	CREATE_FUNC( GameOver );
private:
	int m_nThisScore;
	int m_nMaxScore;
};

#endif