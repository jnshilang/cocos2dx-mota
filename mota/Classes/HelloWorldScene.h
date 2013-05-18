#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

typedef enum {
    kDown=0,
    kLeft=1,
    kRight=2,
    kUp=3
} HeroDirection;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    ~HelloWorld();
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    cocos2d::CCSprite *heroSprite;
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void menuCallBackMove(CCObject* pSender);
    //数组保存勇士向4个方向行走的动画模板
    cocos2d::CCAnimation **walkAnimations;
    
    cocos2d::CCPoint positionForTileCoord(cocos2d::CCPoint tileCoord);
    void setSceneScrollPosition(cocos2d::CCPoint position);
    //根据方向创建动画
    cocos2d::CCAnimation *createAnimationByDirection(HeroDirection direction);
    void onWalkDone(CCNode *pTarget,void *data);
    
    void update(cocos2d::CCTime dt);
    bool isHeroWalking;
    cocos2d::CCTMXTiledMap *map;
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
