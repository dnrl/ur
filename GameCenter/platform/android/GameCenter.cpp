//
//  GameCenter.cpp
//
//
//  Created by 조 중욱 on 2014. 10. 21..
//
//

#include "../../GameCenter.h"
#include <jni.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include "GameCenter_android.h"
    
USING_NS_CC;

const char* const CLASS_NAME = "com/ur/gamecenter";

void GameCenter::connect(SuccessCallback onSuccess, ErrorCallback onErrorCallback)
{
    _onSuccessCallback = onSuccess;
    _onErrorCallback = onErrorCallback;
//    JniMethodInfo methodInfo;
//    if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "connect", "()V") == false)
//    {
//        return ;
//    }
//    
//    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
//    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}


bool GameCenter::isLocalPlayerAuthenticated()
{
    return false;
}

void GameCenter::getLocalPlayerFriends()
{
}

void GameCenter::getPlayerInfo(std::vector<std::string>playerList)
{
}

#pragma mark Scores
void GameCenter::submitScoreToCatagory(int64_t s, std::string c)
{
}

void GameCenter::retrieveTopTenAllTimeGlobalScores(std::string catagory)
{
}

void GameCenter::retrieveScoresForPlayersToday(std::vector<std::string> *playerIDs, std::string catagory, int startIndex, int numPlayers, bool friendsOnly)
{
}

void GameCenter::retrieveScoresForPlayersThisWeek(std::vector<std::string> *playerIDs, std::string catagory, int startIndex, int numPlayers, bool friendsOnly)
{
}

void GameCenter::retrieveScoresForPlayersAllTime(std::vector<std::string> *playerIDs, std::string catagory, int startIndex, int numPlayers, bool friendsOnly)
{
}

#pragma mark Achievements
void GameCenter::loadAchievements()
{
}

GCAchievement GameCenter::getAchievementByID(std::string identifier)
{
}

void GameCenter::reportAchievement(std::string identifier, float percentComplete, bool showCompletionBanner)
{
}

void GameCenter::resetAchievements()
{
}

void GameCenter::reportCachedAchievements()
{
}

void GameCenter::saveCachedAchievements()
{
}


#pragma mark Game Centre Views

void GameCenter::showLeaderBoard()
{
}


void GameCenter::showAchievements(SuccessCallback callback)
{
    _onSuccessCallback = callback;
}

void GameCenter::setDelegate(GameCenterDelegate* delegate)
{
}

#pragma mark - delegate
void GameCenter::onConnected()
{
    if(_onSuccessCallback)
        _onSuccessCallback();
}
void GameCenter::onError(const char* error)
{
    log("game center error : %s", error);
    _onErrorCallback(error);
}

void GameCenter::onAuthenticationChanged()
{
    
}

void GameCenter::onFriendsListRecieved(std::vector<std::string> friendList)
{
    
}
void GameCenter::onPlayerInfoRecieved(std::vector<GCPlayer> playerInfo)
{
    
}

//achievement
void GameCenter::onLoadAchievement(std::map<std::string, GCAchievement> achievements)
{
    
}
void GameCenter::onAchievementViewDidFinish()
{
    if(_onSuccessCallback)
        _onSuccessCallback();
}
void GameCenter::onAchievementReported(GCAchievement acv)
{
    
}
void GameCenter::onResetAchievements()
{
    
}

//leaderboard
void GameCenter::onLeaderBoardViewDidFinish()
{
    
}
void GameCenter::onScoresSubmitted()
{
    
}
void GameCenter::onScoresReceived(std::vector<GCScore> scores)
{
    
}

//match maker
void GameCenter::onMatchmakerViewWasCancelled()
{
    
}
void GameCenter::onMatchmakerViewError()
{
    
}
void GameCenter::onReceivedMatchmakingActivity(int activity)
{
    
}
void GameCenter::onPlayerConnected(const char* playerId)
{
    
}
void GameCenter::onPlayerDisconnected(const char* playerId)
{
    
}
void GameCenter::onStartMatch()
{
    
}
void GameCenter::onAddPlayerToMatch()
{
    
}
void GameCenter::onMatchFound()
{
    
}
