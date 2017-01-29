#ifndef __TEST_H__
#define __TEST_H__

#include "cocos2d.h"

USING_NS_CC;

class Test
{
public:
    static cocos2d::Scene* createScene();
};

class PhysicsDemos : public cocos2d::Layer
{
public:
    virtual std::string subtitle() const { return "";}
    
	void restartCallback(Ref* sender);
	void nextCallback(Ref* sender);
	void backCallback(Ref* sender);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

protected:
    virtual bool init();

    Sprite* addSpriteAtPositions(const Vec2& pos,int spriteIndex = 0, float scale = 1.0f);

    std::unordered_map<int, Node*> _mouses;
};

class TestPhysics : public PhysicsDemos
{
public:
    CREATE_FUNC(TestPhysics);
    
    virtual std::string subtitle() const override;
    bool onContactBegins(PhysicsContact& contact);
    void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

protected:
    virtual bool init();
private:
    Sprite* _muleSprite;
};

#endif // __TEST_14_H__
