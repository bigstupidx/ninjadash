#include "GameOverScene.h"
#include "GameMenuScene.h"
#include "GameRunScene.h"

USING_NS_CC;

bool GameOver::init()
{
	bool bRet = false;
	do 
	{
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		CCSprite* background = CCSprite::create( "bg_gameover.png" );
		CC_BREAK_IF(!background);
		background->setScaleX(CCEGLView::sharedOpenGLView()->getFrameSize().width / background->getContentSize().width);
		background->setScaleY(CCEGLView::sharedOpenGLView()->getFrameSize().height / background->getContentSize().height);
		background->setPosition( ccp( CCEGLView::sharedOpenGLView()->getFrameSize().width / 2, CCEGLView::sharedOpenGLView()->getFrameSize().height / 2 ) );
		this->addChild( background, -1 );

		// Create 2 Menu Items
		CCMenuItemImage* quitItem = CCMenuItemImage::create( "quit_up.png", "quit_down.png" );
		quitItem->setTag( MT_Menu );
		quitItem->setTarget( this, menu_selector( GameOver::menuCallback ) );
		quitItem->setPosition( ccp( visibleSize.width / 2 - 100, 300 ) );
		quitItem->setScale( 1.5f );

		CCMenuItemImage* retryItem = CCMenuItemImage::create( "tryagain_up.png", "tryagain_down.png" );
		retryItem->setTag( MT_Retry );
		retryItem->setTarget( this, menu_selector( GameOver::menuCallback ) );
		retryItem->setPosition( ccp( visibleSize.width / 2 + 100, 300 ) );
		retryItem->setScale( 1.5f );

		// Add to Menu
		CCMenu* pMenu = CCMenu::create( quitItem, retryItem, NULL );
		pMenu->setPosition( CCPointZero );
		this->addChild( pMenu );

		bRet = true;
	} while (0);

	return bRet;
}

void GameOver::menuCallback( CCObject* pSender )
{
	switch( ((CCMenuItem*)pSender)->getTag() )
	{
	case MT_Menu:
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

cocos2d::CCScene* GameOver::scene(int aScore)
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
		layer->m_nMaxScore = 1000;
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
		
	} while (0);

	// return the scene
	return scene;
}

void GameOver::onEnter()
{
	// Haha Logic
//	char buf[64];
//	::GetCurrentDirectoryA(64,buf);
	/*FILE* w = fopen("abc","w");
	fwrite("abcdef",1,20,w);
	fclose(w);
*/
	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	const char* FILE_NAME = "/mnt/sdcard/ninjadata";
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
	// Font
	CCString* s1 = CCString::createWithFormat("%d",m_nThisScore);
	CCLabelTTF* info1 = CCLabelTTF::create(s1->getCString(),"Stencil Std",fontSize);
	info1->setPosition(CCPoint(CCEGLView::sharedOpenGLView()->getFrameSize().width / 2,CCEGLView::sharedOpenGLView()->getFrameSize().height * 1.35 / 2 ));
	addChild(info1);
;
	CCLabelTTF* info2 = CCLabelTTF::create(sMax->getCString(),"Stencil Std",fontSize);
	info2->setPosition(CCPoint(CCEGLView::sharedOpenGLView()->getFrameSize().width / 2,CCEGLView::sharedOpenGLView()->getFrameSize().height * 1.1 / 2 ));
	addChild(info2);

	CCNode::onEnter();
}


