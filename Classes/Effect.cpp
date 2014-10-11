//
//  Effect.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/11.
//
//

#include "Effect.h"

bool Effect::initGLProgramState(const std::string &fragmentFilename)
{
    auto fileUtils = FileUtils::getInstance();
    auto fragmentFullPath  = fileUtils->fullPathForFilename(fragmentFilename);
    auto fragSource = fileUtils->getStringFromFile(fragmentFullPath);
    auto glProgram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _fragSource = fragSource;
#endif
    _glProgramState = GLProgramState::getOrCreateWithGLProgram(glProgram);
    _glProgramState->retain();
    
    return _glProgramState!=nullptr;
}

Effect::Effect():_glProgramState(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                      [this](EventCustom*)
                                                      {
                                                          auto glProgram = _glprogramstate->getGLProgram();
                                                          glProgram->reset();
                                                          glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
                                                          glProgram->link();
                                                          glProgram->updateUniforms();
                                                      }
                                                      );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif

}

Effect::~Effect()
{
    CC_SAFE_RELEASE_NULL(_glProgramState);
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

////////////////////
//EffectOutline/////
////////////////////

bool EffectOutline::init()
{
    initGLProgramState("Shaders/outline.fsh");
    return true;
}

void EffectOutline::setTarget(EffectSprite* sprite)
{
    //auto s = sprite->getTexture()->getContentSizeInPixel();
    //getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));
}
