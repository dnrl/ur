//
//  GameCenter_ios.h
//
//  Created by 조 중욱 on 2014. 10. 21..
//
//

//#include <vector>
//#include <string>
//#include <map>
#import <GameKit/GameKit.h>
#import "../../GameCenterDelegate.h"

@protocol GameKitHelperProtocol

-(void) onLocalPlayerAuthenticationChanged;

-(void) onFriendListReceived:(NSArray*)friends;
-(void) onPlayerInfoReceived:(NSArray*)players;

-(void) onScoresSubmitted:(bool)success;
-(void) onScoresReceived:(NSArray*)scores;

-(void) onAchievementReported:(GKAchievement*)achievement;
-(void) onAchievementsLoaded:(NSDictionary*)achievements;
-(void) onResetAchievements:(bool)success;

-(void) onMatchFound:(GKMatch*)match;
-(void) onPlayersAddedToMatch:(bool)success;
-(void) onReceivedMatchmakingActivity:(NSInteger)activity;

-(void) onPlayerConnected:(NSString*)playerID;
-(void) onPlayerDisconnected:(NSString*)playerID;
-(void) onStartMatch;
-(void) onReceivedData:(NSData*)data fromPlayer:(NSString*)playerID;

-(void) onMatchmakingViewDismissed;
-(void) onMatchmakingViewError;
-(void) onLeaderboardViewDismissed;
-(void) onAchievementsViewDismissed;

@end

@interface GameCenter_ios : NSObject <GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate, GKMatchmakerViewControllerDelegate, GKMatchDelegate>
{
    id<GameKitHelperProtocol> delegate;
    bool isGameCenterAvailable;
    
    NSMutableDictionary* achievements;
    NSMutableDictionary* cachedAchievements;
    
    GKMatch*    currentMatch;
    bool matchStarted;
}

@property (nonatomic, readonly) bool isGameCenterAvailable;
@property (nonatomic, readonly) NSMutableDictionary* achievements;
@property (nonatomic, readonly) GKMatch* currentMatch;
@property (nonatomic, readonly) bool matchStarted;

+ (GameCenter_ios*) getInstance;

- (void)connect;
- (void)loadAchievements;
- (void)loadFriendsList;
- (void)loadPlayerInfo;
- (BOOL)isAuthenticated;

// Player authentication, info
-(void) getPlayerInfo:(NSArray*)players;

// Scores
-(void) submitScore:(int64_t)score category:(NSString*)category;

-(void) retrieveScoresForPlayers:(NSArray*)players
                        category:(NSString*)category
                           range:(NSRange)range
                     playerScope:(GKLeaderboardPlayerScope)playerScope
                       timeScope:(GKLeaderboardTimeScope)timeScope;
-(void) retrieveTopTenAllTimeGlobalScoresForCatagory:(NSString*)catagory;

// Achievements
-(GKAchievement*) getAchievementByID:(NSString*)identifier;
-(void) reportAchievementWithID:(NSString*)identifier percentComplete:(float)percent showCompletionBanner:(BOOL)showBanner;
-(void) resetAchievements;
-(void) reportCachedAchievements;
-(void) saveCachedAchievements;

// Matchmaking
-(void) disconnectCurrentMatch;
-(void) findMatchForRequest:(GKMatchRequest*)request;
-(void) addPlayersToMatch:(GKMatchRequest*)request;
-(void) cancelMatchmakingRequest;
-(void) queryMatchmakingActivity;

// Game Center Views
-(void) showLeaderboard;
-(void) showAchievements;
-(void) showMatchmakerWithInvite:(GKInvite*)invite;
-(void) showMatchmakerWithRequest:(GKMatchRequest*)request;
-(void) sendDataToAllPlayers:(void*)data length:(NSUInteger)length;


//delegate
- (void)setDelegate:(GameCenterDelegate*)GCdelegate;

//convert
-(std::vector<std::string>)convertNSArrayOfStringToCppVector:(NSArray*)array;
-(NSArray*)convertCppVectorOfStringToNSArray:(std::vector<std::string>)vector;
-(GCPlayer)convertGKPLayerToGKPLayerStruct:(GKPlayer*)player;
-(std::vector<GCPlayer>)convertNSArrayOfGKPlayersToCppVector:(NSArray*)array;
-(GCScore)convertGKScoreToGKScoreStruct:(GKScore*)gkScore;
-(std::vector<GCScore>)convertNSArrayOfGKScoresToCppVector:(NSArray*)array;
-(GCAchievement)convertGKAchievementToGKAchievementStruct:(GKAchievement*)acv;
-(std::map<std::string, GCAchievement>)convertNSDictionaryOfGKAchievementTostdMap:(NSDictionary*)dict;

@end
