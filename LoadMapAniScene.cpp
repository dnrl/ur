//
//  LoadMapAniScene.cpp
//  DragonBaseball
//
//  Created by kim on 13. 11. 21..
//
//

#include "LoadMapAniScene.h"
#include "PlayingGame.h"
#include "Manager.h"
#include "MainScene.h"
#include "LeagueManager.h"

#include "ResourceLoadManager.h"

USING_NS_CC;

LoadMapAniScene::LoadMapAniScene()
{
}

LoadMapAniScene::~LoadMapAniScene()
{
    
    CCLOGINFO("cocos2d: deallocing LoadMapAniScene");
}

Scene* LoadMapAniScene::scene()
{
    Scene *scene = Scene::create();\
    
    LoadMapAniScene *layer = LoadMapAniScene::create();
	
    scene->addChild(layer);
	
    return scene;
}

// on "init" you need to initialize your instance
bool LoadMapAniScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    m_iStadiumNum = GameDataMgr::GetInst()->getInGameStadiumNum();   //입력값
    m_bIsAway = GameDataMgr::GetInst()->getHome();   //입력값

    
    PTEAMINFO pHomeInfo = LeagueManager::GetInst()->getMyInfo();
    PTEAMINFO pAwayInfo = m_bIsAway == false ? GameDataMgr::GetInst()->getOpenGameHome() : GameDataMgr::GetInst()->getOpenGameAway();
    
    int iAwayStadiumNum = pAwayInfo->iMapPos;
    
    m_pLoadMap = Sprite::create("gamescene/move/bg_map.png");
    m_pLoadMap->setPosition(Point(GetWinSize().width / 2, GetWinSize().height / 2));
    
    log("%s 홈위치 : %d 어웨이위치 : %d", m_bIsAway == true ? "어웨이" : "홈", pHomeInfo->iMapPos, m_iStadiumNum);
    this->addChild(m_pLoadMap);
    
    Sprite *pHome_Island = Sprite::create("gamescene/move/home.png");
    pHome_Island->setPosition(Point(294.5 * GetPoint().x, 163.75 * GetPoint().y + 5 * GetPoint().y));
    m_pLoadMap->addChild(pHome_Island);
    
    Sprite *pHomeStadium = Sprite::create("gamescene/move/stadium_home.png");
    pHomeStadium->setPosition(Point(65.5 * GetPoint().x, 87 * GetPoint().y));
    pHome_Island->addChild(pHomeStadium);
    
    Sprite *pHomeText = Sprite::create("gamescene/move/text_home.png");
    pHomeText->setPosition(Point(pHomeStadium->getBoundingBox().getMidX(), pHomeStadium->getBoundingBox().getMinY()));
    pHomeText->setAnchorPoint(Point(0.5, 1.f));
    pHome_Island->addChild(pHomeText);
    pHome_Island->runAction(RepeatForever::create(Sequence::create(EaseSineOut::create(MoveBy::create(2.f, Point(0, -5 * GetPoint().y))),
                                                                   EaseSineOut::create(MoveBy::create(2.f, Point(0, 5 * GetPoint().y))),
                                                                   NULL)));

    
    
    
    
    Point awayStadiumPos[5] = {
        Point(435.25 * GetPoint().x, 250.5 * GetPoint().y),
        Point(142 * GetPoint().x, 235.75 * GetPoint().y),
        Point(131.5 * GetPoint().x, 132.75 * GetPoint().y),
        Point(420.75 * GetPoint().x, 141.25 * GetPoint().y),
        Point(291.75 * GetPoint().x, 282.5 * GetPoint().y)
    };
    
    Point awayEmblemPos[5] =
    {
        Point(920, 494),
        Point(240, 466),
        Point(221, 259),
        Point(890, 279),
        Point(535, 561),
    };
    
    Point awayEmblemAnchor[5] =
    {
        Point(0.f, 0.5),
        Point(1.f, 0.5),
        Point(1.f, 0.5),
        Point(0.f, 0.5),
        Point(1.f, 0.5f),
    };
    
    
    
    
    
    for(int i = 0; i < 5; i++)
    {
        if(iAwayStadiumNum == i)
        {
            m_pAwayStadium[i] = Sprite::create(String::createWithFormat("gamescene/move/stadium_%d.png", i + 1)->getCString());
        }
        else
        {
            m_pAwayStadium[i] = Sprite::create("gamescene/move/stadium_off.png");
        }

        m_pAwayStadium[i]->setPosition(awayStadiumPos[i]);
        m_pLoadMap->addChild(m_pAwayStadium[i]);
        
        if(iAwayStadiumNum == i)
        {
            Sprite *pAwayText = Sprite::create("gamescene/move/text_away.png");
            pAwayText->setPosition(Point(m_pAwayStadium[i]->getBoundingBox().getMidX(), m_pAwayStadium[i]->getBoundingBox().getMinY()));
            pAwayText->setAnchorPoint(Point(0.5, 1.f));
            m_pLoadMap->addChild(pAwayText);

            Layer *pAwayEmblemLayer = Layer::create();
            Layer *pAwayRank = makeNumberLayer(5);
            pAwayRank->setAnchorPoint(Point(0.f, 1.f));
            pAwayRank->setScale(0.7);
            pAwayRank->ignoreAnchorPointForPosition(false);
            Sprite *pAwayEmblem = NULL;
            if(pAwayInfo->szType == "user")
            {
                pAwayEmblem = Sprite::create(String::createWithFormat("common/degree/user_emblem_%d.png", getEmblemBgNumberAtEmblemNumber(DataManager::GetInst()->getMyInfo()->iEmblem))->getCString());
                Sprite *emblem = Sprite::create(String::createWithFormat("common/degree/%d.png", DataManager::GetInst()->getMyInfo()->iEmblem)->getCString());
                emblem->setPosition(Point(pAwayEmblem->getContentSize().width / 2, 52 * GetPoint().y));
                emblem->setScale(0.85f);
                pAwayEmblem->addChild(emblem);
            }
            else
            {
                pAwayEmblem = Sprite::create(String::createWithFormat("emblem/emblem_%d.png",pAwayInfo->iEmblemNum)->getCString());
            }
            pAwayEmblem->setScale(m_pAwayStadium[i]->getContentSize().height * 0.9 / pAwayEmblem->getContentSize().height);
            
            pAwayEmblemLayer->setContentSize(Size(pAwayRank->getContentSize().width * pAwayRank->getScaleX() + pAwayEmblem->getContentSize().width * pAwayEmblem->getScaleX() - 5 * GetPoint().x,
                                                  MAX(pAwayRank->getContentSize().height * pAwayRank->getScaleY(), pAwayEmblem->getContentSize().height * pAwayEmblem->getScaleY())));
            
            pAwayRank->setPosition(Point(0.f, pAwayEmblemLayer->getContentSize().height));
            pAwayEmblemLayer->addChild(pAwayRank);
            
            pAwayEmblem->setPosition(Point(pAwayRank->getBoundingBox().getMaxX() - 5 * GetPoint().y, pAwayEmblemLayer->getContentSize().height / 2));
            pAwayEmblem->setAnchorPoint(Point(0.f, 0.5f));
            pAwayEmblemLayer->addChild(pAwayEmblem);
            pAwayEmblemLayer->setAnchorPoint(awayEmblemAnchor[i]);
            pAwayEmblemLayer->ignoreAnchorPointForPosition(false);
            pAwayEmblemLayer->setPosition(awayEmblemPos[i]);
            m_pLoadMap->addChild(pAwayEmblemLayer);
        }
        
        
        
        
        
    }
    
    Point nameContPos[5] = {
        Point(864, 610),
        Point(278, 603),
        Point(268, 150),
        Point(826, 162),
        Point(581, 670)
    };
    
    string pNameStr[5] = {
        string("불의산"),
        string("얼음의계곡"),
        string("희망의숲"),
        string("빛의사막"),
        string("어둠의섬")
    };
    
    for(int i = 0; i < 5; i++)
    {
        Layer *pNameLayer = Layer::create();
        Sprite *pNameCont = Sprite::create("gamescene/move/island_name.png");
        Sprite *pElement = Sprite::createWithSpriteFrameName(String::createWithFormat("elemental_%d.png", i)->getCString());

        pNameLayer->setContentSize(Size(pNameCont->getContentSize().width + pElement->getContentSize().width, MAX(pNameCont->getContentSize().height, pElement->getContentSize().height)));
        
        pElement->setAnchorPoint(Point(0.f, 0.5f));
        pElement->setPosition(Point(0, pNameLayer->getContentSize().height / 2));
        pNameLayer->addChild(pElement);
        

        pNameCont->setAnchorPoint(Point(0.f, 0.5f));
        pNameCont->setPosition(Point(pElement->getBoundingBox().getMaxX(), pNameLayer->getContentSize().height / 2));
        pNameLayer->addChild(pNameCont);

        pNameLayer->setAnchorPoint(Point(0.5f, 0.5f));
        pNameLayer->ignoreAnchorPointForPosition(false);
        pNameLayer->setPosition(nameContPos[i]);
        m_pLoadMap->addChild(pNameLayer);
        
        LabelBMFont *pNameLabel = LabelBMFont::create(pNameStr[i].c_str(), "font/Nanum.fnt");
        pNameLabel->setPosition(Point(pNameCont->getContentSize().width / 2, pNameCont->getContentSize().height / 2));
        pNameCont->addChild(pNameLabel);
    }
    
    
    
    
    Point awayLinePos[5] = {
        Point(319 * GetPoint().x, m_pLoadMap->getContentSize().height - 140 * GetPoint().y),
        Point(166.5 * GetPoint().x, m_pLoadMap->getContentSize().height - 148 * GetPoint().y),
        Point(158 * GetPoint().x, m_pLoadMap->getContentSize().height - 201 * GetPoint().y),
        Point(314.5 * GetPoint().x, m_pLoadMap->getContentSize().height - 197.5 * GetPoint().y),
        Point(285 * GetPoint().x, m_pLoadMap->getContentSize().height - 123 * GetPoint().y)
    };
    
    Point awayLineHomeMid[5] =
    {
        Point(0.f, 0.f),
        Point(1.f, 0.f),
        Point(1.f, 1.f),
        Point(0.f, 1.f),
        Point(0.f, 0.f)
    };
    
    Point awayLineHomeChangeRate[5] =
    {
        Point(1.f, 0.f),
        Point(1.f, 0.f),
        Point(1.f, 0.f),
        Point(1.f, 0.f),
        Point(0.f, 1.f)
    };
    
    
    
    Sprite *pGrayLineArr[5];
    
    for(int i = 0; i < 5; i++)
    {
        Sprite *pGrayLine = Sprite::create(String::createWithFormat("gamescene/move/grayline_%d.png", i+1)->getCString());
        pGrayLine->setAnchorPoint(Point(0.f, 1.f));
        pGrayLine->setPosition(awayLinePos[i]);
        m_pLoadMap->addChild(pGrayLine);
        pGrayLineArr[i] = pGrayLine;
    }
    
    Point homeBalloonPos[5] =
    {
        Point(pGrayLineArr[0]->getBoundingBox().getMinX(), pGrayLineArr[0]->getBoundingBox().getMinY()),
        Point(pGrayLineArr[1]->getBoundingBox().getMaxX(), pGrayLineArr[1]->getBoundingBox().getMinY()),
        Point(pGrayLineArr[2]->getBoundingBox().getMaxX(), pGrayLineArr[2]->getBoundingBox().getMaxY()),
        Point(pGrayLineArr[3]->getBoundingBox().getMinX(), pGrayLineArr[3]->getBoundingBox().getMaxY()),
        Point(pGrayLineArr[4]->getBoundingBox().getMidX(), pGrayLineArr[4]->getBoundingBox().getMinY()),
    };
    
    Point awayBalloonPos[5] =
    {
        Point(pGrayLineArr[0]->getBoundingBox().getMaxX(), pGrayLineArr[0]->getBoundingBox().getMaxY()),
        Point(pGrayLineArr[1]->getBoundingBox().getMinX(), pGrayLineArr[1]->getBoundingBox().getMaxY()),
        Point(pGrayLineArr[2]->getBoundingBox().getMinX(), pGrayLineArr[2]->getBoundingBox().getMinY()),
        Point(pGrayLineArr[3]->getBoundingBox().getMaxX(), pGrayLineArr[3]->getBoundingBox().getMinY()),
        Point(pGrayLineArr[4]->getBoundingBox().getMidX(), pGrayLineArr[4]->getBoundingBox().getMaxY()),
    };
    
    
    ProgressTimer *pRedLine = ProgressTimer::create(Sprite::create(String::createWithFormat("gamescene/move/redline_%d.png", iAwayStadiumNum + 1)->getCString()));
    pRedLine->setAnchorPoint(Point(0.f, 1.f));
    pRedLine->setPosition(awayLinePos[iAwayStadiumNum]);
    if(GameDataMgr::GetInst()->getHome() == false)
    {
        pRedLine->setMidpoint(Point(awayLineHomeMid[iAwayStadiumNum].x == 0.f ? 1.f : 0.f, awayLineHomeMid[iAwayStadiumNum].y == 0.f ? 1.f : 0.f));
    }
    else
    {
        pRedLine->setMidpoint(awayLineHomeMid[iAwayStadiumNum]);        
    }
    
    pRedLine->setType(ProgressTimer::Type::BAR);
    pRedLine->setBarChangeRate(awayLineHomeChangeRate[iAwayStadiumNum]);
    pRedLine->setPercentage(0.f);
    m_pLoadMap->addChild(pRedLine);
    pRedLine->runAction(Sequence::create(DelayTime::create(1.f), EaseExponentialOut::create(ProgressFromTo::create(5.f, 0.f, 100.f)), NULL));

    
    Sprite *pBalloon = Sprite::create("gamescene/move/balloon.png");
    m_pLoadMap->addChild(pBalloon);
    if(GameDataMgr::GetInst()->getHome() == false)
    {
        pBalloon->setPosition(awayBalloonPos[iAwayStadiumNum]);
        pBalloon->runAction(Sequence::create(DelayTime::create(1.f),EaseExponentialOut::create(JumpTo::create(3.5f, homeBalloonPos[iAwayStadiumNum], 20 * GetPoint().y, 1)), NULL));
    }
    else
    {
        pBalloon->setPosition(homeBalloonPos[iAwayStadiumNum]);
        pBalloon->runAction(Sequence::create(DelayTime::create(1.f),EaseExponentialOut::create(JumpTo::create(3.5f, awayBalloonPos[iAwayStadiumNum], 20 * GetPoint().y, 1)), NULL));
    }
    
    this->runAction(Sequence::create(DelayTime::create(1.f),
                                     CallFunc::create(CC_CALLBACK_0(LoadMapAniScene::setSecondAct, this)),
                                     NULL));
//
//    for (int i = 0; i < 5; i++)
//    {
//        if(iAwayStadiumNum == i)
//        {
//            ProgressTimer *pRedLine = ProgressTimer::create(Sprite::create(String::createWithFormat("gamescene/move/redline_%d.png", i+1)->getCString()));
//            pRedLine->setAnchorPoint(Point(0.f, 1.f));
//            pRedLine->setPosition(awayLinePos[i]);
//            if(GameDataMgr::GetInst()->getHome() == false)
//            {
//                pRedLine->setMidpoint(awayLineHomeMid[i]);
//            }
//            else
//            {
//                pRedLine->setMidpoint(Point(awayLineHomeMid[i].x == 0.f ? 1.f : 0.f, awayLineHomeMid[i].y == 0.f ? 1.f : 0.f));
//            }
//        
//            pRedLine->setType(ProgressTimer::Type::BAR);
//            pRedLine->setBarChangeRate(awayLineHomeChangeRate[i]);
//            pRedLine->setPercentage(0.f);
//            m_pLoadMap->addChild(pRedLine);
//            pRedLine->runAction(ProgressFromTo::create(5.f, 0.f, 100.f));
//        }
//    }
    
//    for(int i = 0; i < 5; i++)
//    {
//        Sprite *pBalloon = Sprite::create("gamescene/move/balloon.png");
//        pBalloon->setPosition(homeBalloonPos[i]);
//        m_pLoadMap->addChild(pBalloon);
//        pBalloon->runAction(JumpTo::create(5.f, awayBalloonPos[i], 20 * GetPoint().y, 1));
//    }
    
    
    
    
    
//    MenuItemLabel *pLabel = MenuItemLabel::create(LayerColor::create(Color4B(255, 255, 255, 0)), CC_CALLBACK_1(LoadMapAniScene::repeat, this));
//    pLabel->setPosition(Point(GetWinSize().width / 2, GetWinSize().height / 2));
//    Menu *pMenu = Menu::create(pLabel, NULL);
//    pMenu->setPosition(Point::ZERO);
//    this->addChild(pMenu, 1000);
    
    
    
    
    
    
    
    
    return true;
}

void LoadMapAniScene::repeat(Object *pSender)
{
    Director::getInstance()->replaceScene(LoadMapAniScene::scene());
}

void LoadMapAniScene::setSecondAct()
{
    PTEAMINFO pAwayInfo = m_bIsAway == false ? GameDataMgr::GetInst()->getOpenGameHome() : GameDataMgr::GetInst()->getOpenGameAway();
    
    int iAwayStadiumNum = pAwayInfo->iMapPos;
    
    
    float scale = 1.6;
    float variableX;
    float variableY;
    
    if(m_bIsAway == true)
    {
        variableX = (m_pAwayStadium[iAwayStadiumNum]->getPositionX() - m_pLoadMap->getContentSize().width / 2) * scale;
        variableY = (m_pAwayStadium[iAwayStadiumNum]->getPositionY() - m_pLoadMap->getContentSize().height / 2) * scale;
    }
    else
    {
        variableX = 0;
        variableY = (21 * GetPoint().y) * scale;
    }
    
    log("%f", variableY);
    if((m_pLoadMap->getPositionX() + (m_pLoadMap->getContentSize().width / 2) * scale - variableX < GetWinSize().width))
    {
        variableX = m_pLoadMap->getPositionX() + (m_pLoadMap->getContentSize().width / 2) * scale - GetWinSize().width;
    }
    else if(m_pLoadMap->getPositionX() - (m_pLoadMap->getContentSize().width / 2) * scale - variableX > 0)
    {
        variableX = m_pLoadMap->getPositionX() - (m_pLoadMap->getContentSize().width / 2) * scale;
    }
    
    if(m_pLoadMap->getPositionY() + (m_pLoadMap->getContentSize().height / 2) * scale - variableY < GetWinSize().height)
    {
        variableY = m_pLoadMap->getPositionY() + (m_pLoadMap->getContentSize().height / 2) * scale - GetWinSize().height;
    }
    else if(m_pLoadMap->getPositionY() - (m_pLoadMap->getContentSize().height / 2) * scale - variableY > 0)
    {
        variableY = m_pLoadMap->getPositionY() - (m_pLoadMap->getContentSize().height / 2) * scale;
    }
    
    m_pLoadMap->runAction(EaseExponentialOut::create(Spawn::create(MoveBy::create(3.5, Point(-variableX, -variableY)), ScaleBy::create(3.5, scale), NULL)));
    
    this->runAction(Sequence::create(DelayTime::create(3.5),
                                     CallFunc::create(CC_CALLBACK_0(LoadMapAniScene::close, this)),
                                     NULL));

}

Layer *LoadMapAniScene::makeNumberLayer(int number)
{
    int rankCount;
    int awayRank = number;
    int tempRank = awayRank;
    for(rankCount = 1; true; rankCount++)
    {
        if(tempRank / 10 == 0)
        {
            break;
        }
        tempRank /= 10;
    }
    Layer *pRankLayer = Layer::create();
    pRankLayer->setContentSize(Size((rankCount + 1) * 12 * GetPoint().x, 18 * GetPoint().y));
    
    int afterSymbolNum = awayRank == 1 ? 10 : awayRank == 2 ? 11 : awayRank == 3 ? 12 : 13;
    Sprite *pAfterNum = Sprite::create("common/etc/rank_number.png", Rect(18 * GetPoint().x * afterSymbolNum, 0, 18 * GetPoint().x, 18 * GetPoint().y));
    pAfterNum->setAnchorPoint(Point(1.f, 0.5f));
    pAfterNum->setPosition(Point(pRankLayer->getContentSize().width, pRankLayer->getContentSize().height / 2));
    pRankLayer->addChild(pAfterNum);
    
    for(int i = 0; i < rankCount; i++)
    {
        int number = awayRank % 10;
        Sprite *pRankNum = Sprite::create("common/etc/rank_number.png", Rect(18 * GetPoint().x * number, 0, 18 * GetPoint().x, 18 * GetPoint().y));
        pRankNum->setAnchorPoint(Point(1.f, 0.5f));
        pRankNum->setPosition(Point(pRankLayer->getContentSize().width - 12 * GetPoint().x - 8 * i * GetPoint().x, pRankLayer->getContentSize().height / 2));
        pRankLayer->addChild(pRankNum);
        awayRank /= 10;
        
    }
    pRankLayer->ignoreAnchorPointForPosition(false);
    return pRankLayer;
}


void LoadMapAniScene::close()
{
    auto s = Director::getInstance()->getRunningScene();
    
    ResourceLoadManager* p = ResourceLoadManager::create(GameScene);
    p->setPosition(Point::ZERO);
    s->addChild(p, LOADING_TAG, LOADING_TAG);
}

