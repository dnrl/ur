//
//  GameCenter.h
//  
//
//  Created by 조 중욱 on 2014. 10. 21..
//
//

#ifndef __DragonVillage3__GameCenter__
#define __DragonVillage3__GameCenter__

#include <cocos2d.h>
#include <vector>
#include "Singleton.h"

#include "GameCenterDelegate.h"

class GameCenterDelegate;

NS_CC_BEGIN

class GameCenter :
public Singleton<GameCenter>,
public GameCenterDelegate
{
public:
    GameCenter(void) {}
    virtual ~GameCenter(void) {}
    
    void init();
    
    /* delegate */
    virtual void onConnected();
    virtual void onError(const char* error);
    virtual void onAuthenticationChanged();
    
    virtual void onFriendsListRecieved(std::vector<std::string> friendList);
    virtual void onPlayerInfoRecieved(std::vector<GCPlayer> playerInfo);
    
    //achievement
    virtual void onLoadAchievement(std::map<std::string, GCAchievement> achievements);
    virtual void onAchievementViewDidFinish();
    virtual void onAchievementReported(GCAchievement acv);
    virtual void onResetAchievements();
    
    //leaderboard
    virtual void onLeaderBoardViewDidFinish();
    virtual void onScoresSubmitted();
    virtual void onScoresReceived(std::vector<GCScore> scores);
    
    //match maker
    virtual void onMatchmakerViewWasCancelled();
    virtual void onMatchmakerViewError();
    virtual void onReceivedMatchmakingActivity(int activity);
    virtual void onPlayerConnected(const char* playerId);
    virtual void onPlayerDisconnected(const char* playerId);
    virtual void onStartMatch();
    virtual void onAddPlayerToMatch();
    virtual void onMatchFound();
    
    //콜백
    using ErrorCallback = std::function<void(const char*)>;
    using SuccessCallback = std::function<void(void)>;
    
private:
    ErrorCallback           _onErrorCallback;
    SuccessCallback         _onSuccessCallback;
    SuccessCallback         _onLoginCallback;
    
public:
    void connect(SuccessCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    
    bool isLocalPlayerAuthenticated();
    void getLocalPlayerFriends();
    void getPlayerInfo(std::vector<std::string>playerList);
    
    // Scores
    void submitScoreToCatagory(int64_t s, std::string c);
    void retrieveTopTenAllTimeGlobalScores(std::string catagory);
    void retrieveScoresForPlayersToday(std::vector<std::string> *playerIDs, std::string catagory, int startIndex, int numPlayers, bool friendsOnly);
    void retrieveScoresForPlayersThisWeek(std::vector<std::string> *playerIDs, std::string catagory, int startIndex, int numPlayers, bool friendsOnly);
    void retrieveScoresForPlayersAllTime(std::vector<std::string> *playerIDs, std::string catagory, int startIndex, int numPlayers, bool friendsOnly);
    
    // Achivements
    void loadAchievements();
    GCAchievement getAchievementByID(std::string identifier);
    void reportAchievement(std::string identifier, float percentComplete, bool showCompletionBanner);
    void resetAchievements();
    void reportCachedAchievements();
    void saveCachedAchievements();
    
    // Game Center Views
    void showLeaderBoard();
    void showAchievements(SuccessCallback callback = nullptr);
    
    
    
    void setDelegate(GameCenterDelegate* delegate);
    
};

NS_CC_END

#endif