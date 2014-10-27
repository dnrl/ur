//
//  GameCenter_ios.h
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 10. 21..
//
//

#include <vector>
#include <string>
#import <GameKit/GameKit.h>
#import "GameCenterDelegate.h"


@interface GameCenter_ios : NSObject <GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate, GKMatchmakerViewControllerDelegate, GKMatchDelegate>
{
    GKMatch*    _currentMatch;
}

+ (GameCenter_ios*) getInstance;

- (void)connect;
- (void)loadAchievements;
- (void)loadFriendsList;
- (void)loadPlayerInfo;



//delegate
- (void)setDelegate:(GameCenterDelegate*)delegate;

//convert
-(std::vector<std::string>)convertNSArrayOfStringToCppVector:(NSArray*)array;
-(NSArray*)convertCppVectorOfStringToNSArray:(std::vector<std::string>)vector;

@end
