//
//  GameCenter.cpp
//
//
//  Created by 조 중욱 on 2014. 10. 21..
//
//

#include "../../GameCenter.h"
#include "GameCenter_ios.h"
    
USING_NS_CC;

void GameCenter::init()
{
    [[GameCenter_ios getInstance]setDelegate:this];
}

void GameCenter::connect(SuccessCallback onConneted, ErrorCallback onError)
{
    _onLoginCallback = onConneted;
    _onErrorCallback = onError;
    [[GameCenter_ios getInstance] connect];
}

bool GameCenter::isLocalPlayerAuthenticated()
{
    return [[GameCenter_ios getInstance] isAuthenticated];
}

void GameCenter::getLocalPlayerFriends()
{
    [[GameCenter_ios getInstance] loadFriendsList];
}

void GameCenter::getPlayerInfo(std::vector<std::string>playerList)
{
    NSMutableArray *objcArray = [NSMutableArray array];
    
    for (int i = 0; i < playerList.size(); i++) {
        NSString *playerID = [NSString stringWithUTF8String:playerList.at(i).c_str()];
        [objcArray addObject:playerID];
    }
    [[GameCenter_ios getInstance] getPlayerInfo:objcArray];
}

#pragma mark Scores
void GameCenter::submitScoreToCatagory(int64_t s, std::string c)
{
    NSString *catagory = [NSString stringWithUTF8String:c.c_str()];
    [[GameCenter_ios getInstance] submitScore:s category:catagory];
}

void GameCenter::retrieveTopTenAllTimeGlobalScores(std::string catagory){
    
    [[GameCenter_ios getInstance] retrieveTopTenAllTimeGlobalScoresForCatagory:[NSString stringWithUTF8String:catagory.c_str()]];
    
}

void GameCenter::retrieveScoresForPlayersToday(std::vector<std::string> *playerIDs, std::string catagory, int startIndex, int numPlayers, bool friendsOnly)
{
    GKLeaderboardPlayerScope playerScope;
    
    if (friendsOnly) {playerScope = GKLeaderboardPlayerScopeFriendsOnly;}
    else {playerScope = GKLeaderboardPlayerScopeGlobal;}
    
    NSArray *playerIDsArray = nil;
    if (playerIDs != nullptr) {
        playerIDsArray = [[GameCenter_ios getInstance] convertCppVectorOfStringToNSArray:*playerIDs];
    }
    
    [[GameCenter_ios getInstance] retrieveScoresForPlayers:playerIDsArray
                                   category:[NSString stringWithUTF8String:catagory.c_str()]
                                      range:NSMakeRange(startIndex, numPlayers)
                                playerScope:playerScope
                                  timeScope:GKLeaderboardTimeScopeToday];
}

void GameCenter::retrieveScoresForPlayersThisWeek(std::vector<std::string> *playerIDs, std::string catagory, int startIndex, int numPlayers, bool friendsOnly)
{
    GKLeaderboardPlayerScope playerScope;
    
    if (friendsOnly) {playerScope = GKLeaderboardPlayerScopeFriendsOnly;}
    else {playerScope = GKLeaderboardPlayerScopeGlobal;}
    
    NSArray *playerIDsArray = nil;
    if (playerIDs != nullptr) {
        playerIDsArray = [[GameCenter_ios getInstance] convertCppVectorOfStringToNSArray:*playerIDs];
    }
    
    [[GameCenter_ios getInstance] retrieveScoresForPlayers:playerIDsArray
                                   category:[NSString stringWithUTF8String:catagory.c_str()]
                                      range:NSMakeRange(startIndex, numPlayers)
                                playerScope:playerScope
                                  timeScope:GKLeaderboardTimeScopeWeek];
}

void GameCenter::retrieveScoresForPlayersAllTime(std::vector<std::string> *playerIDs, std::string catagory, int startIndex, int numPlayers, bool friendsOnly)
{
    GKLeaderboardPlayerScope playerScope;
    
    if (friendsOnly) {playerScope = GKLeaderboardPlayerScopeFriendsOnly;}
    else {playerScope = GKLeaderboardPlayerScopeGlobal;}
    
    NSArray *playerIDsArray = nil;
    if (playerIDs != nullptr) {
        playerIDsArray = [[GameCenter_ios getInstance] convertCppVectorOfStringToNSArray:*playerIDs];
    }
    
    [[GameCenter_ios getInstance] retrieveScoresForPlayers:playerIDsArray
                                   category:[NSString stringWithUTF8String:catagory.c_str()]
                                      range:NSMakeRange(startIndex, numPlayers)
                                playerScope:playerScope
                                  timeScope:GKLeaderboardTimeScopeAllTime];
    
}

#pragma mark Achievements
void GameCenter::loadAchievements()
{
    [[GameCenter_ios getInstance]loadAchievements];
}

GCAchievement GameCenter::getAchievementByID(std::string identifier)
{
    NSString *nssid = [NSString stringWithUTF8String:identifier.c_str()];
    
    GKAchievement *achievement = [[GameCenter_ios getInstance] getAchievementByID:nssid];
    
    GCAchievement gkaCpp = [[GameCenter_ios getInstance] convertGKAchievementToGKAchievementStruct:achievement];
    return gkaCpp;
}

void GameCenter::reportAchievement(std::string identifier, float percentComplete, bool showCompletionBanner)
{
    NSString *nssIndentifier = [NSString stringWithUTF8String:identifier.c_str()];
    
    [[GameCenter_ios getInstance] reportAchievementWithID:nssIndentifier percentComplete:percentComplete showCompletionBanner:showCompletionBanner];
}

void GameCenter::resetAchievements()
{
    [[GameCenter_ios getInstance] resetAchievements];
}

void GameCenter::reportCachedAchievements()
{
    [[GameCenter_ios getInstance] reportCachedAchievements];
}

void GameCenter::saveCachedAchievements()
{
    [[GameCenter_ios getInstance] saveCachedAchievements];
}


#pragma mark Game Centre Views

void GameCenter::showLeaderBoard()
{
    [[GameCenter_ios getInstance] showLeaderboard];
}


void GameCenter::showAchievements(SuccessCallback callback)
{
    _onSuccessCallback = callback;
    [[GameCenter_ios getInstance] showAchievements];
}

void GameCenter::setDelegate(GameCenterDelegate* delegate)
{
    [[GameCenter_ios getInstance] setDelegate:delegate];
}



#pragma mark - delegate
void GameCenter::onConnected()
{
    if(_onLoginCallback)
        _onLoginCallback();
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
