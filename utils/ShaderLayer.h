//
//  ShaderLayer.h
//
//  Created by 조중욱 on 2015. 1. 18..
//
//

#ifndef __commando__ShaderLayer__
#define __commando__ShaderLayer__

#include <cocos2d.h>

NS_CC_BEGIN

class ShaderLayer :
public Layer
{
public:
	ShaderLayer(void);
	virtual ~ShaderLayer(void);
	
    virtual bool init(const std::string& fsh, const std::string& vsh);
    
    static ShaderLayer* create(const std::string& fsh, const std::string& vsh);
    
    virtual void visit(Renderer *renderer,
                       const Mat4& parentTransform,
                       uint32_t parentFlags) override;
private:
    GLProgram*          _p;
	
protected:
    RenderTexture*      _rt;
    Sprite*             _rtSprite;
	
};

NS_CC_END

#endif /* defined(__ShaderLayer__) */
