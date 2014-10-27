//
//  GameCenter_ios.m
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 10. 21..
//
//

#import "GameCenter_ios.h"

#import "AppController.h"
#import "RootViewController.h"

static NSString* kCachedAchievementsFile = @"CachedAchievements.archive";

@interface GameCenter_ios(intern)
- (void)registerForAuthenticationNotification;
- (void)authenticationChanged;

-(void) initCachedAchievements;
-(void) cacheAchievement:(GKAchievement*)achievement;
-(void) uncacheAchievement:(GKAchievement*)achievement;
-(void) loadAchievements;
-(void) initMatchInvitationHandler;

- (UIViewController*) getRootViewController;
@end


@implementation GameCenter_ios {
    GameCenterDelegate*     _delegate;
}

static GameCenter_ios* instance = nil;

+ (GameCenter_ios*)getInstance
{
    @synchronized(self) {
        if (instance == nil) {
            instance = [[self alloc] init];
        }
    }
    return instance;
}

@synthesize isGameCenterAvailable;
@synthesize achievements;
@synthesize currentMatch;
@synthesize matchStarted;

- (id)init
{
    if((self = [super init])) {
        _delegate = nullptr;
        
        //디바이스 정보
        // Test for Game Center availability
        Class gameKitLocalPlayerClass = NSClassFromString(@"GKLocalPlayer");
        bool isLocalPlayerAvailable = (gameKitLocalPlayerClass != nil);
        
        // Test if device is running iOS 4.1 or higher
        NSString* reqSysVer = @"4.1";
        NSString* currSysVer = [[UIDevice currentDevice] systemVersion];
        bool isOSVer41 = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
        
        isGameCenterAvailable = (isLocalPlayerAvailable && isOSVer41);
        NSLog(@"GameCenter available = %@", isGameCenterAvailable ? @"YES" : @"NO");
        
        [self registerForAuthenticationNotification];
        
    }
    return self;
}

- (void)dealloc
{
    [instance release];
    instance = nil;
    
    [self saveCachedAchievements];
    [cachedAchievements release];
    
    [currentMatch release];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
    [super dealloc];
}

#pragma mark - connect
- (void)connect
{
    if (isGameCenterAvailable == NO)
        return;
    
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (localPlayer.isAuthenticated) {
        return;
    }
    
    //ios 5
    [localPlayer authenticateWithCompletionHandler:^(NSError* error) {
        if(error) {
            if(error.code != GKErrorAuthenticationInProgress) {
                if(_delegate != nullptr) {
                    _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
                }
            }
        }
        else {
            if(_delegate != nullptr) {
                _delegate->onConnected();
            }
            
            [self initMatchInvitationHandler]; //are not using achievements so commented out these three lines
            [self reportCachedAchievements];
            //업적 로드
            [self loadAchievements];
        }
    }];
}

-(BOOL)isAuthenticated
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    return localPlayer.authenticated;
}

#pragma mark - delegate
- (void)setDelegate:(GameCenterDelegate *)GCdelegate
{
    _delegate = GCdelegate;
}

#pragma mark - viewControll
-(UIViewController*) getRootViewController
{
    return [UIApplication sharedApplication].keyWindow.rootViewController;
}

-(void) presentViewController:(UIViewController*)vc
{
    UIViewController* rootVC = [self getRootViewController];
    [rootVC presentModalViewController:vc animated:YES];
}

-(void) dismissModalViewController
{
    UIViewController* rootVC = [self getRootViewController];
    [rootVC dismissModalViewControllerAnimated:YES];
}

#pragma mark - Achievements

- (void)loadAchievements
{
    //업적 정보를 읽어 들인다.
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *loadedAchievements, NSError *error) {
        if(error != nil) {
            if(_delegate != nullptr) {
                _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
            }
        }
        else {
            if (achievements == nil) {
                achievements = [[NSMutableDictionary alloc] init];
            }
            else {
                [achievements removeAllObjects];
            }
            
            for (GKAchievement* achievement in loadedAchievements)
            {
                [achievements setObject:achievement forKey:achievement.identifier];
            }
            
            if (_delegate != nullptr) {
                _delegate->onLoadAchievement([self convertNSDictionaryOfGKAchievementTostdMap:achievements]);
            }
        }
    }];
}

-(void) showAchievements
{
    if (isGameCenterAvailable == NO)
        return;
    
    GKAchievementViewController* achievementsVC = [[[GKAchievementViewController alloc] init] autorelease];
    if (achievementsVC != nil)
    {
        achievementsVC.achievementDelegate = self;
        [self presentViewController:achievementsVC];
    }
}

-(void) achievementViewControllerDidFinish:(GKAchievementViewController*)viewController
{
    [self dismissModalViewController];
    
    if (_delegate != nullptr) {
        _delegate->onAchievementViewDidFinish();
    }
    
}

-(GKAchievement*) getAchievementByID:(NSString*)identifier
{
    if (isGameCenterAvailable == NO)
        return nil;
    
    // Try to get an existing achievement with this identifier
    GKAchievement* achievement = [achievements objectForKey:identifier];
    
    if (achievement == nil)
    {
        // Create a new achievement object
        achievement = [[[GKAchievement alloc] initWithIdentifier:identifier] autorelease];
        [achievements setObject:achievement forKey:achievement.identifier];
    }
    
    return [[achievement retain] autorelease];
}

-(void) reportAchievementWithID:(NSString*)identifier percentComplete:(float)percent showCompletionBanner:(BOOL)showBanner
{
    if (isGameCenterAvailable == NO)
        return;
    
    GKAchievement* achievement = [self getAchievementByID:identifier];
    
    if (achievement != nil && achievement.percentComplete < percent)
    {
        achievement.percentComplete = percent;
        achievement.showsCompletionBanner = showBanner;
        [achievement reportAchievementWithCompletionHandler:^(NSError* error) {
            if(error) {
                if(_delegate != nullptr) {
                    _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
                }
            }
            else {
                // Keep achievement to try to submit it later
                [self cacheAchievement:achievement];
                if (_delegate != nullptr) {
                    _delegate->onAchievementReported([self convertGKAchievementToGKAchievementStruct:achievement]);
                }
            }
        }];
    }
}

-(void) resetAchievements
{
    if (isGameCenterAvailable == NO)
        return;
    
    [achievements removeAllObjects];
    [cachedAchievements removeAllObjects];
    
    [GKAchievement resetAchievementsWithCompletionHandler:^(NSError* error) {
        if(error) {
            if(_delegate != nullptr) {
                _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
            }
        }
        else {
            if (_delegate != nullptr) {
                _delegate->onResetAchievements();
            }
        }
     }];
}

-(void) reportCachedAchievements
{
    if (isGameCenterAvailable == NO)
        return;
    
    if ([cachedAchievements count] == 0)
        return;
    
    for (GKAchievement* achievement in [cachedAchievements allValues])
    {
        [achievement reportAchievementWithCompletionHandler:^(NSError* error) {
            if(error) {
                if(_delegate != nullptr) {
                    _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
                }
            }
            else {
                [self uncacheAchievement:achievement];
            }
        }];
    }
}

-(void) initCachedAchievements
{
    NSString* file = [NSHomeDirectory() stringByAppendingPathComponent:kCachedAchievementsFile];
    id object = [NSKeyedUnarchiver unarchiveObjectWithFile:file];
    
    if ([object isKindOfClass:[NSMutableDictionary class]])
    {
        NSMutableDictionary* loadedAchievements = (NSMutableDictionary*)object;
        cachedAchievements = [[NSMutableDictionary alloc] initWithDictionary:loadedAchievements];
    }
    else
    {
        cachedAchievements = [[NSMutableDictionary alloc] init];
    }
}

-(void) saveCachedAchievements
{
    NSString* file = [NSHomeDirectory() stringByAppendingPathComponent:kCachedAchievementsFile];
    [NSKeyedArchiver archiveRootObject:cachedAchievements toFile:file];
}

-(void) cacheAchievement:(GKAchievement*)achievement
{
    [cachedAchievements setObject:achievement forKey:achievement.identifier];
    
    // Save to disk immediately, to keep achievements around even if the game crashes.
    [self saveCachedAchievements];
}

-(void) uncacheAchievement:(GKAchievement*)achievement
{
    [cachedAchievements removeObjectForKey:achievement.identifier];
    
    // Save to disk immediately, to keep the removed cached achievement from being loaded again
    [self saveCachedAchievements];
}

#pragma mark - Leaderboard
-(void) showLeaderboard
{
    if (isGameCenterAvailable == NO)
        return;
    
    GKLeaderboardViewController* leaderboardVC = [[[GKLeaderboardViewController alloc] init] autorelease];
    if (leaderboardVC != nil)
    {
        leaderboardVC.leaderboardDelegate = self;
        [self presentViewController:leaderboardVC];
    }
}

-(void) leaderboardViewControllerDidFinish:(GKLeaderboardViewController*)viewController
{
    [self dismissModalViewController];
    
    if (_delegate != nullptr) {
        _delegate->onLeaderBoardViewDidFinish();
    }
}

#pragma mark Scores & Leaderboard

-(void) submitScore:(int64_t)score category:(NSString*)category
{
    if (isGameCenterAvailable == NO)
        return;
    
    GKScore* gkScore = [[[GKScore alloc] initWithCategory:category] autorelease];
    gkScore.value = score;
    
    [gkScore reportScoreWithCompletionHandler:^(NSError* error)     {
        if(error) {
            if(_delegate != nullptr) {
                _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
            }
        }
        else {
            if (_delegate != nullptr) {
                _delegate->onScoresSubmitted();
            }
        }
    }];
}

-(void) retrieveScoresForPlayers:(NSArray*)players
                        category:(NSString*)category
                           range:(NSRange)range
                     playerScope:(GKLeaderboardPlayerScope)playerScope
                       timeScope:(GKLeaderboardTimeScope)timeScope
{
    if (isGameCenterAvailable == NO)
        return;
    
    GKLeaderboard* leaderboard = nil;
    if ([players count] > 0)
    {
        leaderboard = [[[GKLeaderboard alloc] initWithPlayerIDs:players] autorelease];
    }
    else
    {
        leaderboard = [[[GKLeaderboard alloc] init] autorelease];
        leaderboard.playerScope = playerScope;
    }
    
    if (leaderboard != nil)
    {
        leaderboard.timeScope = timeScope;
        leaderboard.category = category;
        leaderboard.range = range;
        [leaderboard loadScoresWithCompletionHandler:^(NSArray* scores, NSError* error) {
            if(error) {
                if(_delegate != nullptr) {
                    _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
                }
            }
            else {
                if (_delegate != nullptr) {
                    _delegate->onScoresReceived([self convertNSArrayOfGKScoresToCppVector:scores]);
                }
            }
         }];
    }
}

-(void) retrieveTopTenAllTimeGlobalScoresForCatagory:(NSString*)catagory
{
    [self retrieveScoresForPlayers:nil
                          category:catagory
                             range:NSMakeRange(1, 10)
                       playerScope:GKLeaderboardPlayerScopeGlobal
                         timeScope:GKLeaderboardTimeScopeAllTime];
}

#pragma mark - Friends List

- (void)loadFriendsList
{
    if (isGameCenterAvailable == NO)
        return;
    
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (localPlayer.authenticated)
    {
        // First, get the list of friends (player IDs)
        [localPlayer loadFriendsWithCompletionHandler:^(NSArray* friends, NSError* error) {
            if(error) {
                if(_delegate != nullptr) {
                    _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
                }
            }
            else {
                if (_delegate != nullptr) {
                    _delegate->onFriendsListRecieved([self convertNSArrayOfStringToCppVector:friends]);
                }
            }
        }];
    }
}

-(void) getPlayerInfo:(NSArray*)playerList
{
    if (isGameCenterAvailable == NO)
        return;
    
    // Get detailed information about a list of players
    if ([playerList count] > 0)
    {
        [GKPlayer loadPlayersForIdentifiers:playerList withCompletionHandler:^(NSArray* players, NSError* error) {
            if(error) {
                if(_delegate != nullptr) {
                    _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
                }
            }
            else {
                if (_delegate != nullptr) {
                    _delegate->onPlayerInfoRecieved([self convertNSArrayOfGKPlayersToCppVector:players]);
                }
            }
        }];
    }
}

#pragma mark - Player Info

- (void)loadPlayerInfo
{
    
}

#pragma mark - Match
#pragma mark Matchmaking

-(void) disconnectCurrentMatch
{
    [currentMatch disconnect];
    currentMatch.delegate = nil;
    [currentMatch release];
    currentMatch = nil;
}

-(void) setCurrentMatch:(GKMatch*)match
{
    if ([currentMatch isEqual:match] == NO)
    {
        [self disconnectCurrentMatch];
        currentMatch = [match retain];
        currentMatch.delegate = self;
    }
}

-(void) initMatchInvitationHandler
{
    if (isGameCenterAvailable == NO)
        return;
    
    [GKMatchmaker sharedMatchmaker].inviteHandler = ^(GKInvite* acceptedInvite, NSArray* playersToInvite) {
        [self disconnectCurrentMatch];
        
        if (acceptedInvite) {
            [self showMatchmakerWithInvite:acceptedInvite];
        }
        else if (playersToInvite) {
            GKMatchRequest* request = [[[GKMatchRequest alloc] init] autorelease];
            request.minPlayers = 2;
            request.maxPlayers = 4;
            request.playersToInvite = playersToInvite;
            
            [self showMatchmakerWithRequest:request];
        }
    };
}

-(void) findMatchForRequest:(GKMatchRequest*)request
{
    if (isGameCenterAvailable == NO)
        return;
    
    [[GKMatchmaker sharedMatchmaker] findMatchForRequest:request withCompletionHandler:^(GKMatch* match, NSError* error) {
        if(error) {
            if(_delegate != nullptr) {
                _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
            }
        }
        else {
            if (match) {
                [self setCurrentMatch:match];
//                [delegate onMatchFound:match];
                if(_delegate != nullptr) {
                    _delegate->onMatchFound();
                }
            }
        }
    }];
}

-(void) addPlayersToMatch:(GKMatchRequest*)request
{
    if (isGameCenterAvailable == NO)
        return;
    
    if (currentMatch == nil)
        return;
    
    [[GKMatchmaker sharedMatchmaker] addPlayersToMatch:currentMatch matchRequest:request completionHandler:^(NSError* error) {
        if(error) {
            if(_delegate != nullptr) {
                _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
            }
        }
        else {
            if(_delegate != nullptr) {
                _delegate->onAddPlayerToMatch();
            }
        }
    }];
}

-(void) cancelMatchmakingRequest
{
    if (isGameCenterAvailable == NO)
        return;
    
    [[GKMatchmaker sharedMatchmaker] cancel];
}

-(void) queryMatchmakingActivity
{
    if (isGameCenterAvailable == NO)
        return;
    
    [[GKMatchmaker sharedMatchmaker] queryActivityWithCompletionHandler:^(NSInteger activity, NSError* error) {
        if(error) {
            if(_delegate != nullptr) {
                _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
            }
        }
        else {
            if(_delegate != nullptr) {
                _delegate->onReceivedMatchmakingActivity(activity);;
            }
        }
    }];
}

#pragma mark Match Connection

-(void) showMatchmakerWithInvite:(GKInvite*)invite
{
    GKMatchmakerViewController* inviteVC = [[[GKMatchmakerViewController alloc] initWithInvite:invite] autorelease];
    if (inviteVC != nil)    {
        inviteVC.matchmakerDelegate = self;
        [self presentViewController:inviteVC];
    }
}

-(void) showMatchmakerWithRequest:(GKMatchRequest*)request
{
    GKMatchmakerViewController* hostVC = [[[GKMatchmakerViewController alloc] initWithMatchRequest:request] autorelease];
    if (hostVC != nil) {
        hostVC.matchmakerDelegate = self;
        [self presentViewController:hostVC];
    }
}

-(void) match:(GKMatch*)match player:(NSString*)playerID didChangeState:(GKPlayerConnectionState)state
{
    switch (state) {
        case GKPlayerStateUnknown:
            if(_delegate != nullptr) {
                _delegate->onError("player State is Unknown");
            }
            break;
        case GKPlayerStateConnected:
            if(_delegate != nullptr) {
                _delegate->onPlayerConnected([playerID cStringUsingEncoding:NSASCIIStringEncoding]);
            }
            break;
        case GKPlayerStateDisconnected:
            if(_delegate != nullptr) {
                _delegate->onPlayerConnected([playerID cStringUsingEncoding:NSASCIIStringEncoding]);
            }
            break;
    }
    
    if (matchStarted == NO && match.expectedPlayerCount == 0) {
        matchStarted = YES;
        if(_delegate != nullptr) {
            _delegate->onStartMatch();
        }
    }
}

-(void) sendDataToAllPlayers:(void*)data length:(NSUInteger)length
{
    if (isGameCenterAvailable == NO)
        return;
    
    NSError* error = nil;
    NSData* packet = [NSData dataWithBytes:data length:length];
    [currentMatch sendDataToAllPlayers:packet withDataMode:GKMatchSendDataUnreliable error:&error];
    
    if(error) {
        if(_delegate != nullptr) {
            _delegate->onError([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
        }
    }
}

-(void) match:(GKMatch*)match didReceiveData:(NSData*)data fromPlayer:(NSString*)playerID
{
    /* match ReceivedData */
}

-(void) matchmakerViewControllerWasCancelled:(GKMatchmakerViewController*)viewController
{
    [self dismissModalViewController];
    if (_delegate != nullptr) {
        _delegate->onMatchmakerViewWasCancelled();
    }
}

-(void) matchmakerViewController:(GKMatchmakerViewController*)viewController didFailWithError:(NSError*)error
{
    [self dismissModalViewController];
    if (_delegate != nullptr) {
        _delegate->onMatchmakerViewError();
    }
}

#pragma mark -
#pragma mark Cache Helper

- (void)registerForAuthenticationNotification
{
    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc addObserver: self selector:@selector(authenticationChanged) name:GKPlayerAuthenticationDidChangeNotificationName object:nil];
}

- (void)authenticationChanged
{
    //인증 변경
    if(_delegate != nullptr) {
        _delegate->onAuthenticationChanged();
    }
}

#pragma mark -
#pragma mark Convert

-(std::vector<std::string>)convertNSArrayOfStringToCppVector:(NSArray*)array
{
    std::vector<std::string> vector;
    vector.reserve(array.count);
    
    for (NSString *objcString in array) {
        std::string cppString = [objcString UTF8String];
        vector.push_back(cppString);
    }
    return vector;
}

-(NSArray*)convertCppVectorOfStringToNSArray:(std::vector<std::string>)vector
{
    NSMutableArray *array = [[[NSMutableArray alloc]init]autorelease];
    
    for (std::vector<std::string>::iterator it = vector.begin(); it != vector.end(); it++) {
        NSString *string = [NSString stringWithUTF8String:it->c_str()];
        [array addObject:string];
    }
    return array;
}

-(GCPlayer)convertGKPLayerToGKPLayerStruct:(GKPlayer*)player
{
    GCPlayer gkPlayerCpp;
    
    gkPlayerCpp.playerID = player.playerID.UTF8String;
    gkPlayerCpp.alias = player.alias.UTF8String;
    gkPlayerCpp.displayName = player.displayName.UTF8String;
    gkPlayerCpp.isFriend = player.isFriend;
    
    return gkPlayerCpp;
}

-(std::vector<GCPlayer>)convertNSArrayOfGKPlayersToCppVector:(NSArray*)array
{
    std::vector<GCPlayer> vector;
    
    for (GKPlayer *gkPlayer in array) {
        GCPlayer gkPlayerCpp = [self convertGKPLayerToGKPLayerStruct:gkPlayer];
        vector.push_back(gkPlayerCpp);
    }
    return vector;
}

-(GCScore)convertGKScoreToGKScoreStruct:(GKScore*)gkScore
{
    GCScore gkScoreCpp;
    
    gkScoreCpp.playerID = gkScore.playerID.UTF8String;
    gkScoreCpp.catagory = gkScore.category.UTF8String;
    gkScoreCpp.value = gkScore.value;
    gkScoreCpp.context = gkScore.context;
    gkScoreCpp.formattedValue = gkScore.formattedValue.UTF8String;
    gkScoreCpp.rank = gkScore.rank;
    
    return gkScoreCpp;
}

-(std::vector<GCScore>)convertNSArrayOfGKScoresToCppVector:(NSArray*)array
{
    std::vector<GCScore> vector;
    
    for (GKScore *gkScore in array) {
        GCScore gkScoreCpp = [self convertGKScoreToGKScoreStruct:gkScore];
        vector.push_back(gkScoreCpp);
    }
    return vector;
}

-(GCAchievement)convertGKAchievementToGKAchievementStruct:(GKAchievement*)acv
{
    GCAchievement achievement;
    achievement.completed = acv.completed;
    achievement.identifier = [acv.identifier UTF8String];
    achievement.percentComplete = acv.percentComplete;
    achievement.showsCompletionBanner = acv.showsCompletionBanner;
    
    return achievement;
}

-(std::map<std::string, GCAchievement>)convertNSDictionaryOfGKAchievementTostdMap:(NSDictionary*)dict
{
    std::map<std::string, GCAchievement> map;
    
    for (NSString *key in dict) {
        GKAchievement *a = [dict objectForKey:key];
        
        GCAchievement achievement;
        
        achievement.completed = a.completed;
        achievement.identifier = [a.identifier UTF8String];
        achievement.percentComplete = a.percentComplete;
        achievement.showsCompletionBanner = a.showsCompletionBanner;
        
        map[achievement.identifier] = achievement;
    }
    return map;
}

@end
