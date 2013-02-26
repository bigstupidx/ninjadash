#include "GameOverScene.h"
#include "GameMenuScene.h"
#include "GameRunScene.h"

USING_NS_CC;

bool GameOver::init()
{
	bool bRet = false;
	do 
	{
		// Important,enable key
		setKeypadEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void GameOver::menuCallback( CCObject* pSender )
{
	switch( ((CCMenuItem*)pSender)->getTag() )
	{
	case MT_Menu:
	case MT_Back:
		{
			CCTransitionFadeTR* transScene = CCTransitionFadeTR::create( 0.8f, GameMenu::scene() );
			CCDirector::sharedDirector()->replaceScene( transScene );
		}
		break;
	case MT_Retry:
		{
			CCTransitionFadeTR* transScene = CCTransitionFadeTR::create( 0.8f, GameRun::scene() );
			CCDirector::sharedDirector()->replaceScene( transScene );
		}
		break;
	default:
		{
			assert( 0 );
		}
		break;
	}
}

cocos2d::CCScene* GameOver::scene(int aScore,bool aFromMenu)
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GameOver *layer = GameOver::create();
		layer->m_nThisScore = aScore;
		layer->m_bFromMenu = aFromMenu;

		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
		
	} while (0);

	// return the scene
	return scene;
}

void GameOver::onEnter()
{	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	const char* FILE_NAME = "/mnt/sdcard/ninjadash/ninjadata";
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	const char* FILE_NAME = "../assets/data";
#endif
	FILE* p = fopen(FILE_NAME,"r");
	if(!p)
	{
		FILE* w = fopen(FILE_NAME,"w");
		fwrite("0",1,1,w);
		fclose(w);
	}
	p = fopen(FILE_NAME,"r+");

	char data[8] = {0};
	fread(data,1,8,p);
	
	CCString* sMax = CCString::create(data);
	int nMax = sMax->intValue();
	if(m_nThisScore > nMax)
	{
		CCString* sWrite = CCString::createWithFormat("%d",m_nThisScore);
		fseek(p,0,SEEK_SET);
		int l = sWrite->length();
		fwrite(sWrite->getCString(),l,1,p);
	}
	fclose(p);
	// End Haha

	const float fontSize = 40.0f;
	
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	if( !m_bFromMenu )
	{
		CCSprite* background = CCSprite::create( "bg_gameover.png" );
		background->setScaleX(CCEGLView::sharedOpenGLView()->getFrameSize().width / background->getContentSize().width);
		background->setScaleY(CCEGLView::sharedOpenGLView()->getFrameSize().height / background->getContentSize().height);
		background->setPosition( ccp( CCEGLView::sharedOpenGLView()->getFrameSize().width / 2, CCEGLView::sharedOpenGLView()->getFrameSize().height / 2 ) );
		this->addChild( background, -1 );

		// Create 2 Menu Items
		CCMenuItemImage* quitItem = CCMenuItemImage::create( "quit_up.png", "quit_down.png" );
		quitItem->setTag( MT_Menu );
		quitItem->setTarget( this, menu_selector( GameOver::menuCallback ) );
		quitItem->setPosition( ccp( visibleSize.width * (1.0 - 0.3) / 2, CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.375 ) );
		quitItem->setScale( 1.5f );

		CCMenuItemImage* retryItem = CCMenuItemImage::create( "tryagain_up.png", "tryagain_down.png" );
		retryItem->setTag( MT_Retry );
		retryItem->setTarget( this, menu_selector( GameOver::menuCallback ) );
		retryItem->setPosition( ccp( visibleSize.width * (1.0 + 0.3) / 2, CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.375 ) );
		retryItem->setScale( 1.5f );

		// Add to Menu
		CCMenu* pMenu = CCMenu::create( quitItem, retryItem, NULL );
		pMenu->setPosition( CCPointZero );
		this->addChild( pMenu );

		
		// Font
		CCString* s1 = CCString::createWithFormat("%d",m_nThisScore);
		CCLabelTTF* info1 = CCLabelTTF::create(s1->getCString(),"Stencil Std",fontSize);
		info1->setPosition(CCPoint(CCEGLView::sharedOpenGLView()->getFrameSize().width / 2,CCEGLView::sharedOpenGLView()->getFrameSize().height * 1.35 / 2 ));
		addChild(info1);
		
		CCLabelTTF* info2 = CCLabelTTF::create(sMax->getCString(),"Stencil Std",fontSize);
		info2->setPosition(CCPoint(CCEGLView::sharedOpenGLView()->getFrameSize().width / 2,CCEGLView::sharedOpenGLView()->getFrameSize().height * 1.1 / 2 ));
		addChild(info2);

	}
	else
	{
		CCSprite* background = CCSprite::create( "bg_setting.png" );
		background->setScaleX(CCEGLView::sharedOpenGLView()->getFrameSize().width / background->getContentSize().width);
		background->setScaleY(CCEGLView::sharedOpenGLView()->getFrameSize().height / background->getContentSize().height);
		background->setPosition( ccp( CCEGLView::sharedOpenGLView()->getFrameSize().width / 2, CCEGLView::sharedOpenGLView()->getFrameSize().height / 2 ) );
		this->addChild( background, -1 );
		
		CCSprite* bscore = CCSprite::create( "bestscore.png" );
		bscore->setScale(1.5);
		bscore->setPosition( ccp( CCEGLView::sharedOpenGLView()->getFrameSize().width / 2, CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.8 ) );
		this->addChild( bscore, -1 );
	
		// Create 2 Menu Items
		CCMenuItemImage* backItem = CCMenuItemImage::create( "back_up.png", "back_down.png" );
		backItem->setTag( MT_Back );
		backItem->setTarget( this, menu_selector( GameOver::menuCallback ) );
		backItem->setPosition( ccp( visibleSize.width / 2, CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.375 ) );
		backItem->setScale( 1.5f );

		// Add to Menu
		CCMenu* pMenu = CCMenu::create( backItem, NULL );
		pMenu->setPosition( CCPointZero );
		this->addChild( pMenu );

		CCLabelTTF* info2 = CCLabelTTF::create(sMax->getCString(),"Stencil Std",fontSize);
		info2->setPosition(CCPoint(CCEGLView::sharedOpenGLView()->getFrameSize().width / 2,CCEGLView::sharedOpenGLView()->getFrameSize().height * 1.4 / 2 ));
		addChild(info2);

	}

	CCLayer::onEnter();
}

// void GameOver::keyBackClicked()
// {
// 	CCTransitionFadeTR* transScene = CCTransitionFadeTR::create( 0.8f, GameMenu::scene() );
// 	CCDirector::sharedDirector()->replaceScene( transScene );
// }

void GameOver::keyBackClicked()
{
	CCTransitionFadeTR* transScene = CCTransitionFadeTR::create( 0.8f, GameMenu::scene() );
	CCDirector::sharedDirector()->replaceScene( transScene );
}




