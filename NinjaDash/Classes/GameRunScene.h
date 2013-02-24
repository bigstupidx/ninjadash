#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "UTL_pub.h"
#include <queue>
#include <list>

class GameRun : public cocos2d::CCLayer
{

	typedef uint8_t brick_type;enum
	{
		BK_NORMAL,
		BK_LEFT_RD,
		BK_RIGHT_RD,
		BK_SHARP,
		BK_SPRING,
		BK_UNSTABLE,

		BK_BRICK_COUNT
	};

	typedef uint8_t player_state;enum
	{
		PS_NOT_START,
		PS_NORMAL,
		PS_DROPPING,
		PS_JUMPING,
		PS_LEFT_MOVING,
		PS_RIGHT_MOVING,

		PS_STATE_COUNT
	};

	typedef uint8_t touch_type;enum
	{
		TT_LEFT,
		TT_RIGHT,
		TT_NONE,

		TT_COUNT
	};

	typedef std::pair<cocos2d::CCSprite*,brick_type> list_type;


public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    // a selector callback
    void menuStartCallback(CCObject* pSender);
	void spriteMoveFinished(CCNode* pSender);
	void spriteHurtFinished(CCNode* pSender);
	void unstableFinished(CCNode* target,void* data);

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	//virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	//virtual void draw();
    // implement the "static node()" method manually
    CREATE_FUNC(GameRun);
private:
	void cbUpdateTile(float aDelta);
	void cbUpdateForever(float aDelta);
	void resetCanHert(float aDelta);

private:
	cocos2d::CCSprite* m_pPlayer;
	cocos2d::CCSprite* m_pCurPlayerOn;
	cocos2d::CCSprite* m_pHurtSprite;
	cocos2d::CCSprite* m_pLifeHeart[8];
	cocos2d::CCTexture2D* m_pPlayerOrigin;
	cocos2d::CCAnimate* m_AnimLeft;
	cocos2d::CCAnimate* m_AnimRight;
	cocos2d::CCAnimate* m_AnimHurt;
	cocos2d::CCMoveBy* m_MoveLeft;
	cocos2d::CCMoveBy* m_MoveRight;
	cocos2d::CCLabelTTF* m_FloorInfo;

	touch_type m_TouchType;
	player_state m_psPlayerState;
	std::list<list_type> m_TileList;
	int m_JumpHeight;
	int m_CntOfFloors;
	int m_CntOfLifes;
	bool m_CanHert;
	//int m_miliInternal;
	//int m_prevTime;

	static const int MAX_ELAPSE = 1500;
	static const int MIN_ELAPSE = 1000;
	static const int SPEED = 300;
	static const int DROP_SPEED = 400;
	static const int MAX_LIFE = 8;
};

#endif  // __HELLOWORLD_SCENE_H__