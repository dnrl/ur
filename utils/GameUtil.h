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

inline bool calculate(float percent) {
    if ( !(arc4random()%(int)(100.0f / percent)) )  return true;
    return false;
};

#endif /* defined(__MonsterVillage__GameUtil__) */
