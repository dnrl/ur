//
//  ActionLabel.h
//  MonsterVillage
//
//  Created by 이재은 on 2014. 11. 5..
//
//

#ifndef __MonsterVillage__ActionLabel__
#define __MonsterVillage__ActionLabel__

#include <cocos2d.h>

NS_CC_BEGIN

typedef enum action_template {
    NONE,
    FADE_IN,
    FADE_OUT,
    SCALE_IN,
    SCALE_OUT
} ACTION_TYPE;

class ActionLabel : public Label {
public:
    ActionLabel();
    virtual ~ActionLabel();
    
    static void adjustAction();
};

NS_CC_END

#endif /* defined(__MonsterVillage__ActionLabel__) */
