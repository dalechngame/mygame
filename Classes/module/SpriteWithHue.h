#ifndef __SnippetsProject__CCSpriteWithHue__
#define __SnippetsProject__CCSpriteWithHue__

#include "cocos2d.h"
#include "LBLibraryBase.h"

USING_NS_CC;

// 通过改变精灵的色像改变精灵的颜色(0 ~ 2 * PI)
class SpriteWithHue :public WJSprite {
    
private://variable
    
    GLint m_hueLocation;
    GLint m_alphaLocation;

public://variable
    
    CC_SYNTHESIZE_READONLY(GLfloat, m_hue, Hue);
    
private://method
    
	static cocos2d::Map<std::string, GLProgram*>* s_glProgramHueCache;
  
protected://method
    
    virtual void setupDefaultSettings();
    
    virtual void getUniformLocations();
    
    virtual void updateColorMatrix();
    
    virtual void updateAlpha();
    
    virtual GLfloat alpha();
    
	virtual bool initWithTexture(Texture2D *texture, const Rect& rect, bool rotated) override;
   
    virtual void initShader();

	GLProgram* addShader(const char *shaderName = "hue");
    
public://method
    
    SpriteWithHue();
    
    virtual ~SpriteWithHue();
    
    static SpriteWithHue* create(const std::string& filename);

	static SpriteWithHue* createWithTexture(Texture2D *texture, GLfloat hue = 0.0f);
    
    virtual void setHue(GLfloat _hue);
    
	WJSpriteShaderHue* getSpriteShaderHue();

	virtual void updateColor() override;
};


#endif /* defined(__SnippetsProject__CCSpriteWithHue__) */
