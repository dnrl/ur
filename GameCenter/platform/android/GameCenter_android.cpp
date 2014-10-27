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

@interface GameCenter_ios(intern)
- (void)registerForAuthenticationNotification;
- (void)authenticationChanged;

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

- (id)init
{
    if((self = [super init])) {
        _delegate = nullptr;
        
        //디바이스 정보
        NSString* deviceSystemVersion = [[UIDevice currentDevice] systemVersion];
        NSLog(@"ios device system Info : %@", deviceSystemVersion);
        
        [self registerForAuthenticationNotification];
        
    }
    return self;
}

- (void)connect
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (localPlayer.isAuthenticated) {
        return;
    }
    
    //ios 5
    [localPlayer authenticateWithCompletionHandler:^(NSError* error) {
        if(error) {
            if(error.code != GKErrorAuthenticationInProgress) {
                if(_delegate != nullptr) {
                    _delegate->onLoginFailed([error.localizedDescription cStringUsingEncoding: NSASCIIStringEncoding]);
                }
            }
        }
        else {
            if(_delegate != nullptr) {
                _delegate->onConnected([localPlayer.playerID cStringUsingEncoding: NSASCIIStringEncoding], [localPlayer.displayName cStringUsingEncoding: NSASCIIStringEncoding]);
            }
            //업적 로드
            [self loadAchievements];
        }
    }];
}

#pragma mark - delegate
- (void)setDelegate:(GameCenterDelegate *)delegate
{
    _delegate = delegate;
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
    if(_delegate != nullptr) {
    }
}

-(void) achievementViewControllerDidFinish:(GKAchievementViewController*)viewController
{
    [self dismissModalViewController];
    //[delegate onAchievementsViewDismissed];
    if (_delegate != nullptr) {
        _delegate->onAchievementViewDidFinish();
    }
    
}

#pragma mark - Leaderboard

-(void) leaderboardViewControllerDidFinish:(GKLeaderboardViewController*)viewController
{
    [self dismissModalViewController];
    //[delegate onLeaderboardViewDismissed];
    if (_delegate != nullptr) {
        _delegate->onLeaderBoardViewDidFinish();
    }
    
}

#pragma mark - Friends List

- (void)loadFriendsList
{
    
}

#pragma mark - Player Info

- (void)loadPlayerInfo
{
    
}

#pragma mark - Match

-(void) matchmakerViewControllerWasCancelled:(GKMatchmakerViewController*)viewController
{
    [self dismissModalViewController];
//    [delegate onMatchmakingViewDismissed];
    if (_delegate != nullptr) {
        _delegate->onMatchmakerViewWasCancelled();
    }
}

-(void) matchmakerViewController:(GKMatchmakerViewController*)viewController didFailWithError:(NSError*)error
{
    [self dismissModalViewController];
//    [delegate onMatchmakingViewError];
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

-(std::vector<std::string>)convertNSArrayOfStringToCppVector:(NSArray*)array{
    std::vector<std::string> vector;
    vector.reserve(array.count);
    
    for (NSString *objcString in array) {
        std::string cppString = [objcString UTF8String];
        vector.push_back(cppString);
    }
    return vector;
}

-(NSArray*)convertCppVectorOfStringToNSArray:(std::vector<std::string>)vector{
    NSMutableArray *array = [[[NSMutableArray alloc]init]autorelease];
    
    for (std::vector<std::string>::iterator it = vector.begin(); it != vector.end(); it++) {
        NSString *string = [NSString stringWithUTF8String:it->c_str()];
        [array addObject:string];
    }
    return array;
}

@end
