#include "GameRunScene.h"
#include "GameOverScene.h"
//#include "platform/platform.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d;

#pragma comment(lib,"winmm.lib")

CCScene* GameRun::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        GameRun *layer = GameRun::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameRun::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

		// Init Member Var
		m_TouchType = TT_NONE;
		m_psPlayerState = PS_NOT_START;
		m_pCurPlayerOn = NULL;

		m_CntOfFloors = 0;
		m_CntOfLifes = MAX_LIFE;
		m_CanHert = true;
		// Enabled touch utility
		setTouchEnabled(true);

        //// Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        //pLabel->setPosition(ccp(size.width / 2, size.height - 50));

		CCSprite* background = CCSprite::create( "bg_game1.png" );
		CC_BREAK_IF( !background );
		background->setScaleX( CANVAS_WIDTH / background->getContentSize().width );
		background->setScaleY( SCREEN_HEIGHT / background->getContentSize().height );
		background->setPosition( ccp( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ) );
		addChild( background, -1 );

		m_pHurtSprite = CCSprite::create( "hurt1.png" );
		CC_BREAK_IF( !m_pHurtSprite );
		m_pHurtSprite->setScaleX( CANVAS_WIDTH / m_pHurtSprite->getContentSize().width );
		m_pHurtSprite->setScaleY( SCREEN_HEIGHT / m_pHurtSprite->getContentSize().height );
		m_pHurtSprite->setPosition( ccp( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ) );
		m_pHurtSprite->setVisible( false );
		addChild( m_pHurtSprite );

		CCSize springSize = CCSizeMake( 510, 30 );
		CCSprite* top_spring = CCSprite::create("top_bar2.png", CCRectMake( 0, 287 - springSize.height , springSize.width, springSize.height ) );
		CC_BREAK_IF( !top_spring );
		top_spring->setScaleX( CANVAS_WIDTH / top_spring->getContentSize().width );
		top_spring->setPosition( ccp( size.width / 2, size.height - springSize.height / 2 ) );
		addChild( top_spring );

		for(int i = 0;i < MAX_LIFE;i++)
		{
			m_pLifeHeart[i] = CCSprite::create("food_heart.png");
			CC_BREAK_IF( !m_pLifeHeart[i] );
			addChild( m_pLifeHeart[i] );
			const int X_Off = (SCREEN_WIDTH - CANVAS_WIDTH) / 2 + 30; 
			m_pLifeHeart[i]->setPosition(CCPoint(X_Off + i * 35, 20));
		}


		// Create Player
		m_pPlayer = CCSprite::create("role_standing.png");
		CC_BREAK_IF(!m_pPlayer);
		m_pPlayer->setPosition( ccp( size.width / 2, size.height - 60 ) );
		m_pPlayer->setScale(1.2f);
		m_pPlayer->setAnchorPoint( ccp( 1.2f * m_pPlayer->getAnchorPoint().x, 1.2f * m_pPlayer->getAnchorPoint().y ) );
		addChild(m_pPlayer);
		m_pPlayerOrigin = m_pPlayer->getTexture();
		

		// Create Two Animation
		CCAnimation* animation = CCAnimation::create();
		animation->addSpriteFrameWithFileName("role_moving_left1.png");
		animation->addSpriteFrameWithFileName("role_moving_left2.png");
		animation->addSpriteFrameWithFileName("role_moving_left3.png");
		animation->addSpriteFrameWithFileName("role_moving_left4.png");
		// should last 2.8 seconds. And there are 14 frames.
		animation->setDelayPerUnit( 1.0f / 10.0f );
		animation->setRestoreOriginalFrame(true);
		animation->setLoops(kCCRepeatForever);
		m_AnimLeft = CCAnimate::create(animation);

		animation = CCAnimation::create();
		animation->addSpriteFrameWithFileName("role_moving_right1.png");
		animation->addSpriteFrameWithFileName("role_moving_right2.png");
		animation->addSpriteFrameWithFileName("role_moving_right3.png");
		animation->addSpriteFrameWithFileName("role_moving_right4.png");
		// should last 2.8 seconds. And there are 14 frames.
		animation->setDelayPerUnit(1.0f / 10.0f);
		animation->setRestoreOriginalFrame(true);
		animation->setLoops(kCCRepeatForever);
		m_AnimRight = CCAnimate::create(animation);

		animation = CCAnimation::create();
		animation->addSpriteFrameWithFileName("hurt1.png");
		animation->addSpriteFrameWithFileName("hurt2.png");
		// should last 2.8 seconds. And there are 14 frames.
		animation->setDelayPerUnit( 1.0f / 15.0f );
		m_AnimHurt = CCAnimate::create(animation);

		m_AnimLeft->retain();
		m_AnimRight->retain();
		m_AnimHurt->retain();

		/*m_MoveLeft	= CCRepeatForever::create( CCMoveBy::create( .5f, ccp( -100, 0 ) ) );
		m_MoveRight = CCRepeatForever::create( CCMoveBy::create( .5f, ccp(  100, 0 ) ) );*/
		m_MoveLeft	= CCMoveBy::create( 1.8f, ccp( -CANVAS_WIDTH, 0 ) );
		m_MoveRight = CCMoveBy::create( 1.8f, ccp(  CANVAS_WIDTH, 0 ) );
		m_MoveLeft->retain();
		m_MoveRight->retain();

		// Create 2 Menu Items
		CCMenuItemImage* startItem = CCMenuItemImage::create( "go_up.png", "go_down.png" );
		startItem->setTarget( this, menu_selector( GameRun::menuStartCallback ) );
		startItem->setPosition( ccp( size.width / 2 , 400 ) );
		startItem->setScale( 1.5f );

		// Add to Menu
		CCMenu* pMenu = CCMenu::create( startItem, NULL );
		pMenu->setPosition( CCPointZero );
		this->addChild( pMenu );

		// Font
	    m_FloorInfo = cocos2d::CCLabelTTF::create("","Stencil Std",80.0f);
		m_FloorInfo->setPosition(CCPoint(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2));
		m_FloorInfo->setScale(3.0f);
		m_FloorInfo->setOpacity(128);
		addChild(m_FloorInfo, 1.0f );


		schedule(schedule_selector(GameRun::cbUpdateTile),1.0f);
		schedule(schedule_selector(GameRun::cbUpdateForever));

        bRet = true;
    } while (0);

    return bRet;
}

void GameRun::menuStartCallback(CCObject* pSender)
{
// 	if( !m_bStarted )
// 	{
	m_psPlayerState = PS_DROPPING;
	CCMenuItem* pItem = (CCMenuItem*)pSender;
	CCMenu* pMenu = (CCMenu*)pItem->getParent();
	pMenu->setVisible( false );
	//	removeChild( pMenu, true );
//	pMenu->release();


}

void GameRun::spriteMoveFinished( CCNode* pSender )
{
	this->removeChild( pSender, true );
	pSender->release();
}


void GameRun::spriteHurtFinished( CCNode* pSender )
{
	pSender->stopAllActions();
	pSender->setVisible( false );
	
}

void GameRun::resetCanHert( float aDelta )
{
	m_CanHert = true;
}

void GameRun::cbUpdateTile( float aDelta )
{
	float r = (float)rand() / RAND_MAX * 0.2f + 1;

	unschedule(schedule_selector(GameRun::cbUpdateTile));
	schedule(schedule_selector(GameRun::cbUpdateTile),r);

	CCLOG("ABCDEF ==> %f %f",aDelta,r);

	CCSprite* sprite = NULL;
	CCFiniteTimeAction* action1 = NULL;
	brick_type bt = rand() % BK_BRICK_COUNT;
	switch(bt)
	{
	case BK_NORMAL:
		{
			sprite = CCSprite::create("footboard_normal.png");
		}		
		break;
	case BK_LEFT_RD:
		{		
			sprite = CCSprite::create("footboard_moving_left1.png");
			CCAnimation* animation = CCAnimation::create();
			animation->addSpriteFrameWithFileName("footboard_moving_left1.png");
			animation->addSpriteFrameWithFileName("footboard_moving_left2.png");
			// should last 2.8 seconds. And there are 14 frames.
			animation->setDelayPerUnit(2.8f / 14.0f);
			animation->setRestoreOriginalFrame(true);
			animation->setLoops(kCCRepeatForever);
			action1 = CCAnimate::create(animation);
		}
		break;
	case BK_RIGHT_RD:
		{	
			sprite = CCSprite::create("footboard_moving_right1.png");
			CCAnimation* animation = CCAnimation::create();
			animation->addSpriteFrameWithFileName("footboard_moving_right1.png");
			animation->addSpriteFrameWithFileName("footboard_moving_right2.png");
			// should last 2.8 seconds. And there are 14 frames.
			animation->setDelayPerUnit(2.8f / 14.0f);
			animation->setRestoreOriginalFrame(true);
			animation->setLoops(kCCRepeatForever);
			action1 = CCAnimate::create(animation);
		}
		break;
	case BK_SHARP:
		{
			sprite = CCSprite::create("footboard_spiked.png");
		}
		break;
	case BK_SPRING:
		{
			sprite = CCSprite::create("footboard_spring.png");
			CCAnimation* animation = CCAnimation::create();
			animation->addSpriteFrameWithFileName("footboard_spring.png");
			animation->addSpriteFrameWithFileName("footboard_spring2.png");
			// should last 2.8 seconds. And there are 14 frames.
			animation->setDelayPerUnit(2.8f / 14.0f);
			animation->setRestoreOriginalFrame(true);
			animation->setLoops(kCCRepeatForever);
			action1 = CCAnimate::create(animation);
		}
		break;
	case BK_UNSTABLE:
		{
			sprite = CCSprite::create("footboard_unstable1.png");
			//CCAnimation* animation = CCAnimation::create();
			//animation->addSpriteFrameWithFileName("footboard_unstable1.png");
			//animation->addSpriteFrameWithFileName("footboard_unstable2.png");
			//// should last 2.8 seconds. And there are 14 frames.
			//animation->setDelayPerUnit(2.8f / 14.0f);
			//animation->setRestoreOriginalFrame(true);
			//animation->setLoops(kCCRepeatForever);
			//action1 = CCAnimate::create(animation);
		}
		break;
	default:
		{
			assert(false);
		}
		break;
	}
	
	int imgWidth = sprite->getContentSize().width;
	int imgHeight = sprite->getContentSize().height;

	int min = (SCREEN_WIDTH - CANVAS_WIDTH + imgWidth) / 2;
	int max = (SCREEN_WIDTH + CANVAS_WIDTH - imgWidth) / 2;

	int x = min + (float)rand() / RAND_MAX * (max - min);

	sprite->setPosition(ccp(x,-50));
	
	CCFiniteTimeAction* action2 = CCMoveTo::create(5.f,ccp(x,850));
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( sprite, callfuncN_selector(GameRun::spriteMoveFinished));
	if(action1)
	{
		//CCFiniteTimeAction* tmpAction = CCSpawn::create(action1,action2,NULL);
		//sprite->runAction(CCSequence::create(tmpAction,actionMoveDone,NULL));
		sprite->runAction(CCSpawn::create(action1,action2,NULL));
	}
	else
	{
		//sprite->runAction(CCSequence::create(action2,actionMoveDone,NULL));
		sprite->runAction(action2);
	}

	//sprite->retain();
	m_TileList.push_back( make_pair( sprite, bt ) );
	this->addChild(sprite);

	// Update Floor Info
	CCString* s = CCString::stringWithFormat("%d",++m_CntOfFloors);
	m_FloorInfo->setString(s->getCString());

	//CCLOG("List Count is ===> %d",m_TileList.size());

	//do 
	//{

	//	CCSprite* sprite = CCSprite::create("GameRun.png");
	//	CC_BREAK_IF(! sprite);

	//	int temp = sprite->getContentSize().width;
	//	int x = ((float)rand() / RAND_MAX) * (CANVAS_WIDTH) + (SCREEN_WIDTH - CANVAS_WIDTH) / 2;;
	//	sprite->setPosition(ccp(x,-50));
	//	sprite->setScaleX(0.4);
	//	sprite->setScaleY(0.1);

	//	CCFiniteTimeAction* action = CCMoveTo::create(5.f,ccp(x,850));
	//	sprite->runAction(action);

	//	this->addChild(sprite);
	//	float r = (float)(std::rand() & (RAND_MAX));
	//	r /= RAND_MAX;
	//	r = (3 - 1) * r + 1;
	//	//scheduleOnce(schedule_selector(GameRun::cbUpdateTile), r);
	//	//schedule(schedule_selector(GameRun::cbUpdateTile), 0,0,r);
	//} while (0);
}

void GameRun::ccTouchesBegan( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	//if( !m_bStarted )
	//{
	//	m_psPlayerState = PS_DROPPING;
	//	m_bStarted = true;
	//}
	m_pPlayer->setFlipX(false);
	CCTouch* tch = (CCTouch*)(pTouches->anyObject());
	if(tch->getLocation().x < 1024 / 2)
	{

		m_pPlayer->runAction( m_AnimLeft );
		m_TouchType = TT_LEFT;
	}
	else
	{
		m_pPlayer->runAction( m_AnimRight );
		m_TouchType = TT_RIGHT;
	}
}

void GameRun::ccTouchesMoved( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	m_pPlayer->setFlipX(false);
	CCTouch* tch = (CCTouch*)(pTouches->anyObject());
	if(tch->getLocation().x < 1024 / 2)
	{
		m_pPlayer->stopAllActions();
// 		if( m_psPlayerState = PS_DROPPING )
// 		{
// 			m_pPlayer->runAction( m_AnimLeft );
// 		}
// 		else
// 		{
// 			m_pPlayer->runAction( CCSpawn::create( m_AnimLeft, m_MoveLeft, NULL ) );
// 		}

		m_pPlayer->runAction( m_AnimLeft );
		
		m_TouchType = TT_LEFT;
	}
	else
	{
		m_pPlayer->stopAllActions();
		m_pPlayer->runAction( m_AnimRight );

		m_TouchType = TT_RIGHT;
	}
}

void GameRun::ccTouchesEnded( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	m_pPlayer->stopAllActions();
	m_pPlayer->setTexture(m_pPlayerOrigin);
	if(m_TouchType == TT_LEFT)
	{
		m_pPlayer->setFlipX(true);
	}
	m_TouchType = TT_NONE;
}

void GameRun::cbUpdateForever( float aDelta )
{
	switch(m_psPlayerState)
	{
	case PS_DROPPING:
		{
			m_pPlayer->setPositionY( m_pPlayer->getPositionY() - DROP_SPEED * aDelta );
		}
		break;
	case PS_NORMAL:
		{
			assert( m_pCurPlayerOn );
			m_pPlayer->setPositionY( m_pCurPlayerOn->getPositionY() + m_pPlayer->getContentSize().height - 10 );
//			CCLOG("NNNNNNNNNNOOOOOOOOOOOOOORRRRRRRRRRRRRMMMMMMMMMMMMAALL  %d",timeGetTime());
		}
		break;
	case PS_JUMPING:
		{
			if( m_pPlayer->getPositionY() > m_JumpHeight )
			{
				m_psPlayerState = PS_DROPPING;
			}
			else
			{
				m_pPlayer->setPositionY( m_pPlayer->getPositionY() + aDelta * SPEED * 1.5 );
			}
		}
		break;
	case PS_LEFT_MOVING:
		{
			assert( m_pCurPlayerOn );
			m_pPlayer->setPositionY( m_pCurPlayerOn->getPositionY() + m_pPlayer->getContentSize().height - 10 );
			m_pPlayer->setPositionX( m_pPlayer->getPositionX() - SPEED * aDelta * .5f );
		}
		break;
	case PS_RIGHT_MOVING:
		{
			assert( m_pCurPlayerOn );
			m_pPlayer->setPositionY( m_pCurPlayerOn->getPositionY() + m_pPlayer->getContentSize().height - 10 );
			m_pPlayer->setPositionX( m_pPlayer->getPositionX() + SPEED * aDelta * .5f );
		}
		break;
	case PS_NOT_START:
		{

		}
		break;
	default:
		assert(0);
	}

	// Move Player Left & Right
	if( TT_LEFT == m_TouchType  )
	{
		m_pPlayer->setPositionX( m_pPlayer->getPositionX() - SPEED * aDelta );
	}
	else if( TT_RIGHT == m_TouchType )
	{
		m_pPlayer->setPositionX( m_pPlayer->getPositionX() + SPEED * aDelta );
	}

	// Clip the player`s position
	if( m_pPlayer->getPositionX() < ( SCREEN_WIDTH - CANVAS_WIDTH + m_pPlayer->getContentSize().width ) / 2 )
	{
		m_pPlayer->setPositionX( ( SCREEN_WIDTH - CANVAS_WIDTH + m_pPlayer->getContentSize().width ) / 2 );
	}
	else if( m_pPlayer->getPositionX() > ( SCREEN_WIDTH + CANVAS_WIDTH  - m_pPlayer->getContentSize().width ) / 2 )
	{
		m_pPlayer->setPositionX( ( SCREEN_WIDTH + CANVAS_WIDTH - m_pPlayer->getContentSize().width ) / 2 );
	}

	// Check whether player hurt
	float ClipYPos = 770 - m_pPlayer->getContentSize().height / 2;
	if( m_pPlayer->getPositionY() >  ClipYPos )
	{
		m_pPlayer->setPositionY( ClipYPos );
		m_psPlayerState = PS_DROPPING;

		if( m_CanHert )
		{
			--m_CntOfLifes;
			m_CanHert = false;
			scheduleOnce(schedule_selector(GameRun::resetCanHert),1.0f);

			m_pHurtSprite->setVisible( true );
			m_pHurtSprite->runAction( CCSequence::actions( 
				m_AnimHurt,
				CCCallFuncN::create( m_pHurtSprite, callfuncN_selector( GameRun::spriteHurtFinished ) ),
				NULL
				) );
		}
	}

	if( m_psPlayerState != PS_NOT_START )
	{
		m_psPlayerState = m_psPlayerState == PS_JUMPING ? PS_JUMPING : PS_DROPPING;
		//bool isOnTile = false;
		list<list_type>::iterator iter = m_TileList.begin();
		while(iter != m_TileList.end())
		{
			if( iter->first->getPositionY() >= 845) // equal 850
			{
				this->removeChild( iter->first, true );
				iter = m_TileList.erase( iter );
			}
			else
			{
				if( iter->first->boundingBox().intersectsRect( m_pPlayer->boundingBox() ) && iter->first->getPositionY() < m_pPlayer->getPositionY() )
				{
					m_pCurPlayerOn = iter->first;	

					switch(iter->second)
					{
					case BK_NORMAL:
						m_psPlayerState = PS_NORMAL;
						break;
					case BK_LEFT_RD:
						m_psPlayerState = PS_LEFT_MOVING;
						break;
					case BK_RIGHT_RD:
						m_psPlayerState = PS_RIGHT_MOVING;
						break;
					case BK_UNSTABLE:
						{
							CCAnimation* animation = CCAnimation::create();
							animation->addSpriteFrameWithFileName("footboard_unstable1.png");
							animation->addSpriteFrameWithFileName("footboard_unstable2.png");
							// should last 2.8 seconds. And there are 14 frames.
							animation->setDelayPerUnit(2.8f / 14.0f);
							animation->setRestoreOriginalFrame(true);

							CCFiniteTimeAction* actionMoveDone = CCCallFuncND::create( iter->first, callfuncND_selector( GameRun::unstableFinished ), (void*)&m_TileList );
							CCAnimate* action1 = CCAnimate::create(animation );
							CCFiniteTimeAction* action2 = CCSequence::create( action1, actionMoveDone, NULL );
							iter->first->runAction(action2);
							iter->second = BK_NORMAL;
						}
						break;
					case BK_SPRING:
						{
							m_psPlayerState = PS_JUMPING;
							m_JumpHeight = iter->first->getPositionY() + 150;
						}
						break;
					case BK_SHARP:
						{
							if( m_CanHert )
							{
								--m_CntOfLifes;
								m_CanHert = false;
								scheduleOnce(schedule_selector(GameRun::resetCanHert),1.0f);

								m_pHurtSprite->setVisible( true );
								m_pHurtSprite->runAction( CCSequence::actions( 
									m_AnimHurt,
									CCCallFuncN::create( m_pHurtSprite, callfuncN_selector( GameRun::spriteHurtFinished ) ),
									NULL
									) );
							}
							m_psPlayerState = PS_NORMAL;
						}
						break;
					default:
						m_psPlayerState = PS_NORMAL;
						break;
					}
				}
				++iter;
			}		
		}

		// Check whether player dead
		if( m_pPlayer->getPositionY() < -(m_pPlayer->getContentSize().height / 2) || m_CntOfLifes <= 0 )
		{
			CCTransitionFadeTR* transScene = CCTransitionFadeTR::create( 0.8f, GameOver::scene(m_CntOfFloors) );
			CCDirector::sharedDirector()->replaceScene( transScene );
		}
	}

	for(int i = 0;i < MAX_LIFE;i++)
	{
		if(i < m_CntOfLifes)
		{
			m_pLifeHeart[i]->setVisible(true);
		}
		else
		{
			m_pLifeHeart[i]->setVisible(false);
		}
	}
}

void GameRun::unstableFinished(CCNode* target,void* data)
{
	list<list_type>* tileList = (list<list_type>*)data;
	list<list_type>::iterator iter = tileList->begin();
	while( iter != tileList->end() )
	{
		if( target == iter->first )
		{
			iter = tileList->erase( iter );
		}
		else
		{
			++iter;
		}
	}

	((CCSprite*)target)->setVisible( false );
	this->removeChild((CCSprite*)target,true);
}










