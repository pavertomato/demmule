#include "test.h"
#include <iostream>

USING_NS_CC;

static Size s_visibleSize;
static Vec2 s_origin;
static Vec2 s_centre;
static Scene* s_currScene;
static const int DRAG_BODYS_TAG = 0x80;

//=====================================================================================
#define STATIC_SPRITE_TAG 10
bool TestPhysics::init()
{
    if (PhysicsDemos::init()) {

        std::cout<<"init";
        //create a static PhysicsBody
        s_currScene->getPhysicsWorld()->setGravity(Vec2(0,-800));
        auto physicsBody = PhysicsBody::createBox(Size(10.0f , 50.0f ),PhysicsMaterial(3.0f, 1.0f, 0.f));
        physicsBody->setDynamic(true);
        physicsBody->setGravityEnable(false);
        physicsBody->setRotationEnable(false);
       // physicsBody->setTag(DRAG_BODYS_TAG);
        _muleSprite = Sprite::create();
        _muleSprite->setPosition(Vec2(s_centre.x - 150 ,s_centre.y));
        _muleSprite->setPhysicsBody(physicsBody);
        //create a sprite
        addChild(_muleSprite);
        auto sprite = Sprite::create();

        auto touchListener = EventListenerTouchAllAtOnce::create();
        touchListener->onTouchesEnded = [=](const std::vector<Touch*>& touches, Event* event){
            for( auto &touch: touches)
            {
                auto sprite = addSpriteAtPositions(touch->getLocation());
                sprite->getPhysicsBody()->setGravityEnable(true);
                sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

        auto contactListener = EventListenerPhysicsContact::create();
        contactListener->onContactBegin = CC_CALLBACK_1(TestPhysics::onContactBegins, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyPressed = CC_CALLBACK_2(TestPhysics::onKeyPressed, this);
        keyListener->onKeyReleased = CC_CALLBACK_2(TestPhysics::onKeyReleased, this);
        _eventDispatcher->addEventListenerWithFixedPriority(keyListener, -11);

        for (int i = 0; i < 5000; ++i)
        {
            physicsBody = PhysicsBody::createBox(Size(5.0f , 5.0f ),PhysicsMaterial(0.1f, 1.0f, 0.0f));
            //set the body isn't affected by the physics world's gravitational force
            physicsBody->setGravityEnable(false);
            physicsBody->setRotationEnable(true);
            physicsBody->setContactTestBitmask(0xFFFFFFFF);

            //set initial velocity of physicsBody
           // physicsBody->setVelocity(Vec2(cocos2d::random(-500,500),cocos2d::random(-500,500)));
            sprite = Sprite::create();
            sprite->setPosition(Vec2(s_centre.x + ((i%50)*6),s_centre.y -300 + (i/50)*6));
            sprite->setPhysicsBody(physicsBody);
            sprite->setTag(STATIC_SPRITE_TAG);
            addChild(sprite);
        }
        return true;
    }
    
    return false;
}

void TestPhysics::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    int impulse = 100;
    Vec2 velocity = _muleSprite->getPhysicsBody()->getVelocity();
    if(code == EventKeyboard::KeyCode::KEY_W){
        _muleSprite->getPhysicsBody()->setVelocity(Vec2(velocity.x,velocity.y+impulse));
    }
    if(code == EventKeyboard::KeyCode::KEY_S){
        _muleSprite->getPhysicsBody()->setVelocity(Vec2(velocity.x,velocity.y-impulse));
    }
    if(code == EventKeyboard::KeyCode::KEY_A){
        _muleSprite->getPhysicsBody()->setVelocity(Vec2(velocity.x-impulse,velocity.y));
    }
    if(code == EventKeyboard::KeyCode::KEY_D){
        _muleSprite->getPhysicsBody()->setVelocity(Vec2(velocity.x+impulse,velocity.y));
    }
    if(code == EventKeyboard::KeyCode::KEY_SPACE){
        auto sprite = addSpriteAtPositions(_muleSprite->getPosition());
        sprite->getPhysicsBody()->setGravityEnable(true);
        sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
    }
    //m_test->Keyboard(static_cast<unsigned char>(code));
}
void TestPhysics::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
    int impulse = -100;
    Vec2 velocity = _muleSprite->getPhysicsBody()->getVelocity();
    if(code == EventKeyboard::KeyCode::KEY_W){
        _muleSprite->getPhysicsBody()->setVelocity(Vec2(velocity.x,velocity.y+impulse));
    }
    if(code == EventKeyboard::KeyCode::KEY_S){
        _muleSprite->getPhysicsBody()->setVelocity(Vec2(velocity.x,velocity.y-impulse));
    }
    if(code == EventKeyboard::KeyCode::KEY_A){
        _muleSprite->getPhysicsBody()->setVelocity(Vec2(velocity.x-impulse,velocity.y));
    }
    if(code == EventKeyboard::KeyCode::KEY_D){
        _muleSprite->getPhysicsBody()->setVelocity(Vec2(velocity.x+impulse,velocity.y));
    }
}

bool TestPhysics::onContactBegins(PhysicsContact& contact)
{
   // std::cout<<"asdasd";
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
   // nodeA->getPhysicsBody()->setGravityEnable(true);
    //nodeB->getPhysicsBody()->setGravityEnable(true);

    if(nodeA) nodeA->getPhysicsBody()->setGravityEnable(true);
    if(nodeB) nodeB->getPhysicsBody()->setGravityEnable(true);

    return true;
}
std::string TestPhysics::subtitle() const
{
    return "";
}

//=====================================================================================
#define STATIC_SPRITE_TAG 10
#define CL(__className__) [](){ return __className__::create();}

namespace
{
    static std::function<Layer*()> createFunctions[] = {
        CL(TestPhysics)
    };

    static int sceneIdx = -1;

#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

    static Layer* next()
    {
        sceneIdx++;
        sceneIdx = sceneIdx % MAX_LAYER;

        auto layer = (createFunctions[sceneIdx])();
        return layer;
    }

    static Layer* back()
    {
        sceneIdx--;
        int total = MAX_LAYER;
        if( sceneIdx < 0 )
            sceneIdx += total;

        auto layer = (createFunctions[sceneIdx])();
        return layer;
    }

    static Layer* restart()
    {
        auto layer = (createFunctions[sceneIdx])();
        return layer;
    }
}

bool PhysicsDemos::init()
{
    if (Layer::init())
    {
        s_currScene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

        //create title
        //TTFConfig ttfConfig("Arial", 32);
        //auto titleLabel = Label::createWithTTF(ttfConfig,"Physics Chapter Samples");
        
        auto titleLabel = LabelTTF::create("Physics", "Arial", 24);
        
        addChild(titleLabel, 9999);
        titleLabel->setPosition(Vec2(s_origin.x + s_visibleSize.width/2, s_origin.y + s_visibleSize.height - 30));

        //create subtitle
        std::string strSubtitle = subtitle();
        if( ! strSubtitle.empty() )
        {
            //ttfConfig.fontSize = 20;
            //auto subTitleLabel = Label::createWithTTF(ttfConfig, strSubtitle.c_str());
            
            auto subTitleLabel = LabelTTF::create(strSubtitle.c_str(), "Arial", 20);
            addChild(subTitleLabel, 9999);
            subTitleLabel->setPosition(Vec2(s_origin.x + s_visibleSize.width/2, s_origin.y + s_visibleSize.height - 70));
        }

        //create menu
        auto backItem = MenuItemImage::create("", "leftSelected.png", CC_CALLBACK_1(PhysicsDemos::backCallback, this) );
        auto restartItem = MenuItemImage::create("restartNormal.png", "restartSelected.png", CC_CALLBACK_1(PhysicsDemos::restartCallback, this) );
        auto nextItem = MenuItemImage::create("", "rightSelected.png", CC_CALLBACK_1(PhysicsDemos::nextCallback, this) );

        auto menu = Menu::create(backItem, restartItem, nextItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        addChild(menu, 9999);

        auto restartItemSize = restartItem->getContentSize();
        backItem->setPosition(s_centre.x - restartItemSize.width * 2, s_origin.y + restartItemSize.height / 2);
        restartItem->setPosition(s_centre.x, s_origin.y + restartItemSize.height / 2);
        nextItem->setPosition(s_centre.x + restartItemSize.width * 2, s_origin.y + restartItemSize.height/2);

        //create border
     //   auto node = Node::create();
      //  auto physicsBody = PhysicsBody::createEdgeBox(s_visibleSize,PhysicsMaterial(0.1f, 1, 0.0f));
       // node->setPhysicsBody(physicsBody);
       // node->setPosition(s_centre);
       // this->addChild(node);

        //add touch event listener
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(PhysicsDemos::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(PhysicsDemos::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(PhysicsDemos::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(PhysicsDemos::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        return true;
    }

    return false;
}

void PhysicsDemos::restartCallback(Ref* sender)
{
    s_currScene = Scene::createWithPhysics();
    s_currScene->addChild( restart() );
    Director::getInstance()->replaceScene(s_currScene);
}

void PhysicsDemos::nextCallback(Ref* sender)
{
    s_currScene = Scene::createWithPhysics();
    s_currScene->addChild( next() );
    Director::getInstance()->replaceScene(s_currScene);
}

void PhysicsDemos::backCallback(Ref* sender)
{
    s_currScene = Scene::createWithPhysics();
    s_currScene->addChild( back() );
    Director::getInstance()->replaceScene(s_currScene);
}

bool PhysicsDemos::onTouchBegan(Touch* touch, Event* event)
{
    auto location = touch->getLocation();
    auto arr = s_currScene->getPhysicsWorld()->getShapes(location);

    PhysicsBody* body = nullptr;
    for (auto& obj : arr)
    {
        if ((obj->getBody()->getTag() & DRAG_BODYS_TAG) != 0)
        {
            body = obj->getBody();
            break;
        }
    }

    if (body != nullptr)
    {
        Node* mouse = Node::create();
        mouse->setPhysicsBody(PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY));
        mouse->getPhysicsBody()->setDynamic(false);
        mouse->setPosition(location);
        this->addChild(mouse);
        PhysicsJointPin* joint = PhysicsJointPin::construct(mouse->getPhysicsBody(), body, location);
        joint->setMaxForce(5000.0f * body->getMass());
        s_currScene->getPhysicsWorld()->addJoint(joint);
        _mouses.insert(std::make_pair(touch->getID(), mouse));

        return true;
    }

    return false;
}

void PhysicsDemos::onTouchMoved(Touch* touch, Event* event)
{
    auto it = _mouses.find(touch->getID());

    if (it != _mouses.end())
    {
        it->second->setPosition(touch->getLocation());
    }
}

void PhysicsDemos::onTouchEnded(Touch* touch, Event* event)
{
    auto it = _mouses.find(touch->getID());

    if (it != _mouses.end())
    {
        this->removeChild(it->second);
        _mouses.erase(it);
    }
}


Sprite* PhysicsDemos::addSpriteAtPositions(const Vec2& pos, int spriteIndex /* = 0 */, float scale /* = 1.0f */)
{
    auto sprite = Sprite::create();
    auto physicsBody = PhysicsBody::createBox(Size(5.0f * scale, 5.0f * scale),PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setVelocity(Vec2(500,cocos2d::random(-50,50)));
    sprite->setScale(scale);
    sprite->setPosition(pos);
    sprite->setPhysicsBody(physicsBody);
    addChild(sprite);

    return sprite;
}

Scene* Test::createScene()
{
    sceneIdx = -1;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // some upfront items that we need
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    s_visibleSize = Director::getInstance()->getVisibleSize();
    s_origin = Director::getInstance()->getVisibleOrigin();
    s_centre = s_origin + s_visibleSize / 2;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // create a scene with physics world for our game to hold everything
    // 'scene' is an autorelease object
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    s_currScene = Scene::createWithPhysics();

    s_currScene->addChild(next());

    // return the scene
    return s_currScene;
}
