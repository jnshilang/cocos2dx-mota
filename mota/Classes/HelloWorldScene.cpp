#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    isHeroWalking=false;
    /////////////////////////////
    // 3. add your codes below...

    map=CCTMXTiledMap::create("0.tmx");
    this->addChild(map);
    
    /*walkAnimations=new CCAnimation*[4];
    walkAnimations[kDown]=createAnimationByDirection(kDown);
    walkAnimations[kLeft]=createAnimationByDirection(kLeft);
    walkAnimations[kRight]=createAnimationByDirection(kRight);
    walkAnimations[kUp]=createAnimationByDirection(kUp);*/
    
    //printf("%d",walkAnimations);
    //创建一个CCSprite用来显示勇士，可以使用Animation中的一帧来作为勇士静止时的画面
    CCAnimationFrame* pAnimationFrame = dynamic_cast<CCAnimationFrame*>(createAnimationByDirection(kUp)->getFrames()->objectAtIndex(0));
    CCSpriteFrame* pSpriteFrame = pAnimationFrame->getSpriteFrame();
    heroSprite=CCSprite::createWithSpriteFrame(pSpriteFrame);
    
    //暂时将勇士显示在（100，100）处
    //创建不断重复的动画，并让heroSprite播放
    heroSprite->setAnchorPoint(CCPointZero);
    heroSprite->setPosition(positionForTileCoord(ccp(1,11)));
    //CCAnimate *animate=CCAnimate::create(walkAnimations[kDown]);
    //heroSprite->runAction(CCRepeatForever::create(animate));
    addChild(heroSprite);
    
    CCMenuItem *down=CCMenuItemFont::create("down", this, menu_selector(HelloWorld::menuCallBackMove));
    CCMenuItem *left=CCMenuItemFont::create("left", this, menu_selector(HelloWorld::menuCallBackMove));
    CCMenuItem *right=CCMenuItemFont::create("right", this, menu_selector(HelloWorld::menuCallBackMove));
    CCMenuItem *up=CCMenuItemFont::create("up", this, menu_selector(HelloWorld::menuCallBackMove));
    CCMenu *menu=CCMenu::create(down,left,right,up,NULL);
    
    //为了方便查找，给menuItem设置Ｔag
    down->setTag(kDown);
    left->setTag(kLeft);
    right->setTag(kRight);
    up->setTag(kUp);
    //菜单项按间距水平排列
    menu->alignItemsHorizontallyWithPadding(50);
    addChild(menu);
    
    schedule(schedule_selector(HelloWorld::update));
    
    return true;
}
CCAnimation *HelloWorld::createAnimationByDirection(HeroDirection direction){
    //将图片生成纹理，保存到全局的纹理缓存取
    CCTexture2D *heroTexture=CCTextureCache::sharedTextureCache()->addImage("hero.png");
    //用纹理创建4幅帧动画
    CCSpriteFrame *frame0,*frame1,*frame2,*frame3;
    //第二个参数表示显示区域的x,y,width,height
    frame0=CCSpriteFrame::createWithTexture(heroTexture, cocos2d::CCRectMake(32*0,32*direction,32,32));
    frame1=CCSpriteFrame::createWithTexture(heroTexture, cocos2d::CCRectMake(32*1,32*direction,32,32));
    frame2=CCSpriteFrame::createWithTexture(heroTexture, cocos2d::CCRectMake(32*2,32*direction,32,32));
    frame3=CCSpriteFrame::createWithTexture(heroTexture, cocos2d::CCRectMake(32*3,32*direction,32,32));
    //CCMutableArray<CCSpriteFrame*> *animFramess=new CCMutableArray<CCSpriteFrame*>(4);
    
    CCArray *animFrames=CCArray::create();
    animFrames->addObject(frame0);
    animFrames->addObject(frame1);
    animFrames->addObject(frame2);
    animFrames->addObject(frame3);
    //根据4幅帧生成ＣＣＡnimation对象
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(animFrames);
    
    //根据动画模板创建动画
    animation->setDelayPerUnit(0.2f);
    //animFrames->release();
    return animation;
}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void HelloWorld::menuCallBackMove(CCObject *pSender){
    //勇士正在移动中，不接受新的指令
    if (isHeroWalking) {
        return;
    }
    CCNode *node=(CCNode *) pSender;
    HeroDirection targetDirection=(HeroDirection) node->getTag();
    isHeroWalking=true;
    //移动的距离
    CCPoint moveByPosition;
    //根据方向计算移动的距离
    switch (targetDirection) {
        case kDown:
            moveByPosition=ccp(0,-32);
            break;
        case kLeft:
            moveByPosition=ccp(-32,0);
            break;
        case kRight:
            moveByPosition=ccp(32,0);
            break;
        case kUp:
            moveByPosition=ccp(0,32);
            break;
        default:
            break;
    }
    CCAction *action=CCSequence::create(CCSpawn::create(
                                                        CCAnimate::create(createAnimationByDirection(targetDirection)),
                                                        CCMoveBy::create(0.28f, moveByPosition),NULL),
                                        CCCallFuncND::create(this, callfuncND_selector(HelloWorld::onWalkDone), (void*)targetDirection),
                                        NULL);
    
    //CCAnimate *animate=CCAnimate::create(createAnimationByDirection(targetDirection));
    //创建不断重复的动画，并让heroSprite播放
    //heroSprite->runAction(CCRepeatForever::create(animate));
    
    heroSprite->runAction(action);
    
}

void HelloWorld::onWalkDone(CCNode *pTarget, void* data){
    //
    isHeroWalking = false;
}

//Ｔilemap坐标系到cocos2d-x坐标系的转换
CCPoint HelloWorld::positionForTileCoord(CCPoint tileCoord){
    CCPoint pos=ccp((tileCoord.x*map->getTileSize().width),
                    (map->getMapSize().height-tileCoord.y-1)*map->getTileSize().height);
    return pos;
}
//将场景移动到相应位置
void HelloWorld::setSceneScrollPosition(cocos2d::CCPoint position){
    //获取屏幕尺寸
    CCSize screenSize=CCDirector::sharedDirector()->getWinSize();
    //计算timeMap的高度，单位像素
    CCSize mapSizeInPixel=CCSizeMake(map->getMapSize().width*map->getTileSize().width, map->getMapSize().height*map->getTileSize().height);
    //获取勇士位置和屏幕中点x和y的最大值
    float x=MAX(position.x, screenSize.width/2.0f);
    float y=MAX(position.y, screenSize.height/2.0f);
    //地图总宽度大于屏幕宽度时才可能滚动
    if (mapSizeInPixel.width>screenSize.width) {
        //场景移动距离不能超过地图总宽度减去屏幕宽的一半
        x=MIN(x, mapSizeInPixel.width-screenSize.width/2.0f);
    }
    //地图总高度大于屏幕高度时才可能滚动
    if (mapSizeInPixel.height>screenSize.height) {
        //场景移动距离不能超过地图总高度减去屏幕高的一半
        y=MIN(y, mapSizeInPixel.height-screenSize.height/2.0f);
    }
    //勇士实际位置
    CCPoint heroPosition=ccp(x,y);
    //屏幕中点位置
    CCPoint screenCenter=ccp(screenSize.width/2.0f,screenSize.height/2.0f);
    //计算勇士实际位置和重点位置的距离
    CCPoint scrollPosition=ccpSub(screenCenter, heroPosition);
    
    //将场景移动到相应位置
    this->setPosition(scrollPosition);
    CCLog("%f,%f",scrollPosition.x,scrollPosition.y);
    
}
void HelloWorld::update(CCTime dt){
    //如果勇士不在行走状态，不需要更新场景位置
    if (isHeroWalking) {
        setSceneScrollPosition(heroSprite->getPosition());
    }
}
HelloWorld::~HelloWorld(){
    this->unscheduleAllSelectors();
}
/*void HelloWorld::onWalkDone(CCNode *pTarget, void *data){
    
}*/
