#include "HelloWorldScene.h"
#include "test.h"

USING_NS_CC;


// on "init" you need to initialize your instance
bool MainScene::init()
{
    return true;
}


Scene* MainScene::createScene()
{
    auto scene = Test::createScene();

    return scene;
}
void MainScene::onMenuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
