//
//  GameCenterDelegate.h
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 10. 21..
//
//

#ifndef __DragonVillage3__GameCenterDelegate__
#define __DragonVillage3__GameCenterDelegate__


#include <iostream>
#include <vector>
#include <string>
#include <map>

typedef struct GameCenterPlayer {
    std::string     playerID;
    std::string     alias;
    std::string     displayName;
    bool            isFriend;
}GCPlayer;

typedef struct GameCenterAchievement {
    bool        completed;
    std::string identifier;
    double      percentComplete;
    bool        showsCompletionBanner;
}GCAchievement;

typedef struct GameCenterScore {
    std::string playerID;
    std::string catagory;
    int64_t     value;
    uint64_t    context;
    std::string formattedValue;
    long long   rank;
}GCScore;



class GameCenterDelegate
{
public:
    virtual ~GameCenterDelegate(void){}
	
    virtual void onConnected() {}
    virtual void onError(const char* error) {}
    virtual void onAuthenticationChanged() {}
    
    virtual void onFriendsListRecieved(std::vector<std::string> friendList) {}
    virtual void onPlayerInfoRecieved(std::vector<GCPlayer> playerInfo) {}
    
    //achievement
    virtual void onLoadAchievement(std::map<std::string, GCAchievement> achievements) {}
    virtual void onAchievementViewDidFinish() {}
    virtual void onAchievementReported(GCAchievement acv) {}
    virtual void onResetAchievements() {}
    
    //leaderboard
    virtual void onLeaderBoardViewDidFinish() {}
    virtual void onScoresSubmitted() {}
    virtual void onScoresReceived(std::vector<GCScore> scores){}
    
    //match maker
	virtual void onMatchmakerViewWasCancelled() {}
    virtual void onMatchmakerViewError() {}
    virtual void onReceivedMatchmakingActivity(int activity) {}
    virtual void onPlayerConnected(const char* playerId) {}
    virtual void onPlayerDisconnected(const char* playerId) {}
    virtual void onStartMatch() {}
    virtual void onAddPlayerToMatch() {}
    virtual void onMatchFound() {}
};

#endif