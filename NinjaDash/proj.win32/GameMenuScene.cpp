#include "GameMenuScene.h"
#include "GameRunScene.h"
#include "GameOverScene.h"

USING_NS_CC;

bool GameMenu::init()
{
	bool bRet = false;
	do 
	{

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// Important,enable key
		setKeypadEnabled(true);

		CCSprite* logo = CCSprite::create( "game_logo.png" );
		logo->setScale( 1.5f );
		logo->setPosition( ccp( visibleSize.width / 2, visibleSize.height - 140 ) );
		addChild( logo );

		CCSprite* background = CCSprite::create( "bg_splash.png" );
		CC_BREAK_IF(!background);
		background->setScaleX(CCEGLView::sharedOpenGLView()->getFrameSize().width / background->getContentSize().width);
		background->setScaleY(CCEGLView::sharedOpenGLView()->getFrameSize().height / background->getContentSize().height);
		background->setPosition( ccp( CCEGLView::sharedOpenGLView()->getFrameSize().width / 2, CCEGLView::sharedOpenGLView()->getFrameSize().height / 2 ) );
		this->addChild( background, -1 );

		// Create 4 Menu Items
		CCMenuItemImage* startItem = CCMenuItemImage::create( "startgame_up.png", "startgame_down.png" );
		startItem->setTag( MT_StartGame );
		startItem->setTarget( this, menu_selector( GameMenu::menuCallback ) );
		startItem->setPosition( ccp( visibleSize.width / 2, visibleSize.height * 0.53 ) );
		startItem->setScale( 1.5f );

		CCMenuItemImage* settingItem = CCMenuItemImage::create( "setting_up.png", "setting_down.png" );
		settingItem->setTag( MT_Setting );
		settingItem->setTarget( this, menu_selector( GameMenu::menuCallback ) );
		settingItem->setPosition( ccp( visibleSize.width / 2, visibleSize.height * 0.39 ) );
		settingItem->setScale( 1.5f );

		CCMenuItemImage* rankingItem = CCMenuItemImage::create( "ranking_up.png", "ranking_down.png" );
		rankingItem->setTag( MT_Ranking );
		rankingItem->setTarget( this, menu_selector( GameMenu::menuCallback ) );
		rankingItem->setPosition( ccp( visibleSize.width / 2, visibleSize.height * 0.27 ) );
		rankingItem->setScale( 1.5f );

		CCMenuItemImage* moregameItem = CCMenuItemImage::create( "more_up.png", "more_down.png" );
		moregameItem->setTag( MT_MoreGame );
		moregameItem->setTarget( this, menu_selector( GameMenu::menuCallback ) );
		moregameItem->setPosition( ccp( visibleSize.width / 2, visibleSize.height * 0.145 ) );
		moregameItem->setScale( 1.5f );

		CCMenuItemImage* exitItem = CCMenuItemImage::create( "exit_up.png", "exit_down.png" );
		exitItem->setTag( MT_Exit );
		exitItem->setTarget( this, menu_selector( GameMenu::menuCallback ) );
		exitItem->setPosition( ccp( visibleSize.width * 1.1 / 2, visibleSize.height * 0.04 ) );
		exitItem->setScale( 1.5f );

		// Add to Menu
		CCMenu* pMenu = CCMenu::create( startItem, settingItem, rankingItem, moregameItem,exitItem, NULL );
		pMenu->setPosition( CCPointZero );
		this->addChild( pMenu );


			

		bRet = true;
	} while (0);
	return bRet;
}


void GameMenu::menuCallback( CCObject* pSender )
{
	switch( ((CCMenuItem*)pSender)->getTag() )
	{
	case MT_StartGame:
		{
			CCTransitionFadeTR* transScene = CCTransitionFadeTR::create( 0.8f, GameRun::scene() );
			CCDirector::sharedDirector()->replaceScene( transScene );
		}
		break;
	case MT_Setting:
		{
			int n = 1;
		}
		break;
	case MT_Ranking:
		{
			CCTransitionFadeTR* transScene = CCTransitionFadeTR::create( 0.8f, GameOver::scene(0,true) );
			CCDirector::sharedDirector()->replaceScene( transScene );
		}
		break;
	case MT_MoreGame:
		{
			int n = 1;
		}
		break;
	case MT_Exit:
		{
			exit(0);
		}
		break;
	default:
		{
			assert( 0 );
		}
		break;
	}
}



CCScene* GameMenu::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GameMenu *layer = GameMenu::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


void GameMenu::keyBackClicked()
{
	CCLOG("111111111111111111111111111111111111111111111111111");
	exit(0);
}
