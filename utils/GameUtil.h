//
//  GameUtil.h
//  MonsterVillage
//
//  Created by 이재은 on 2014. 11. 7..
//
//

#ifndef __MonsterVillage__GameUtil__
#define __MonsterVillage__GameUtil__


#define calc(per)                   calculate(per)

#include <cocos2d.h>

inline bool calculate(float percent) {
    /*  0% ~ 100%  */
    if ( percent <= 0.0f )                              return false;
    else if ( percent >= 100.0f )                       return true;
    else if ( !(arc4random()%(int)(100.0f / percent)))  return true;
    
    return false;
};

#endif /* defined(__MonsterVillage__GameUtil__) */
