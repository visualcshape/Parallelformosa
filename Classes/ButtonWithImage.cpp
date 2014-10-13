//
//  ButtonWithImage.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/5.
//
//

#include "ButtonWithImage.h"

static const int THUMBNAIL_IMAGE_Z = (-1);
static const int FOCUS_Z = (-2);

const int ButtonWithImage::_kActionTag = 0;

ButtonWithImage::ButtonWithImage():
_thumbnailRenderer(nullptr),
_isFocus(false){
    
}

ButtonWithImage::~ButtonWithImage(){
    
}

void ButtonWithImage::onSizeChanged(){
    Widget::onSizeChanged();
    updateThumbnailLocation();
    updateTitleLocation();
}

void ButtonWithImage::updateTitleLocation(){
    _titleRenderer->setAnchorPoint(Vec2(0.0, 0.5));
    _titleRenderer->setPosition(Vec2(_thumbnailRenderer->getPosition().x+_thumbnailRenderer->getContentSize().width+2, _contentSize.height*0.5));
}

void ButtonWithImage::updateThumbnailLocation(){
    _thumbnailRenderer->setPosition(Vec2(5.0f, _contentSize.height*0.5-3));
}

void ButtonWithImage::initRenderer(){
    //super call
    Button::initRenderer();
    
    _thumbnailRenderer = Sprite::create();
    _thumbnailRenderer->setAnchorPoint(Vec2(0.0f, 0.5f));
    addProtectedChild(_thumbnailRenderer, THUMBNAIL_IMAGE_Z, -1);
}

ButtonWithImage* ButtonWithImage::create(const std::string &normalImage, const std::string &selectedImage, const std::string &disableImage,const std::string& thumbnailImage,cocos2d::ui::Widget::TextureResType texType){
    ButtonWithImage* ret = new ButtonWithImage();
    if(ret&&ret->init(normalImage, selectedImage, disableImage, thumbnailImage, texType)){
        ret->autorelease();
        return ret;
    }
    
    return nullptr;
}


bool ButtonWithImage::init(const std::string &normalImage, const std::string &selectedImage, const std::string &disabledImage, const std::string &thumbnailImage, cocos2d::ui::Widget::TextureResType texType){
    //super init
    
    if(!Button::init(normalImage,selectedImage,disabledImage,texType)){
        return false;
    }
    
    //load thumbnail texture (orignally from plist)
    loadThumbnailTexture(thumbnailImage, texType);
    //loadFocusTexture();
    
    return true;
    
}

void ButtonWithImage::loadThumbnailTexture(const std::string &thumbnail, cocos2d::ui::Widget::TextureResType texType){
    if(thumbnail.empty())
    {
        return;
    }
    
    _thumbnailFilename = thumbnail;
    _thumbnailTexRes = texType;
    
    //Does not supprt scale9
    Sprite* thumbnailSprite = static_cast<Sprite*>(_thumbnailRenderer);
    switch (texType) {
        case cocos2d::ui::Widget::TextureResType::LOCAL:
            thumbnailSprite->setTexture(thumbnail);
            break;
        case cocos2d::ui::Widget::TextureResType::PLIST:
            thumbnailSprite->setSpriteFrame(thumbnail);
            break;
        default:
            break;
    }
    
    _thumbnailTextureSize = _thumbnailRenderer->getContentSize();
    updateFlippedX();
    updateFlippedY();
    updateContentSizeWithTextureSize(_thumbnailTextureSize);
    //No true/false
    //!!
}

void ButtonWithImage::loadFocusTexture()
{
    //only loaded from plist
    _focusRenderer = Scale9Sprite::createWithSpriteFrameName("ButtonFocus.png", Rect(6, 6, 100-6*2, 100-6*2));
    CC_ASSERT(_focusRenderer!=nullptr);
    _focusRenderer->setContentSize(getContentSize());
    
    _focusRenderer->setAnchorPoint(Vec2(0.0f, 0.0f));
    _focusRenderer->setPosition(Vec2(0, 0));
    
    //_focusRenderer->setAnchorPoint(Vec2(0.0f, 0.5f));
    addProtectedChild(_focusRenderer, FOCUS_Z, -1);
    //no true/false
    //!!
}

void ButtonWithImage::setBID(int key)
{
    _bid = key;
}

int ButtonWithImage::getBID()
{
    return _bid;
}

void ButtonWithImage::setFocus(bool isFocus)
{
    if(isFocus)
    {
        _focusRenderer->setVisible(true);
        focusAction();
    }
    else
    {
        _focusRenderer->setVisible(false);
        if(_focusRenderer->getActionByTag(_kActionTag))
        {
            _focusRenderer->stopAllActions();
        }
        _focusRenderer->setVisible(false);
    }
}

void ButtonWithImage::focusAction()
{
    float duration = 0.5f;
    auto actDis = FadeTo::create(duration, 0);
    auto actApe = FadeTo::create(duration, 255);
    
    CallFunc* repeat = CallFunc::create(CC_CALLBACK_0(ButtonWithImage::focusAction, this));
    Sequence* seq = Sequence::create(actDis,DelayTime::create(0.2f),actApe,repeat, NULL);
    seq->setTag(_kActionTag);
    
    _focusRenderer->runAction(seq);
}

void ButtonWithImage::setIndex(int index)
{
    _index = index;
}

int ButtonWithImage::getIndex()
{
    return _index;
}

void ButtonWithImage::setKey(std::string key)
{
    _key = key;
}

std::string ButtonWithImage::getKey()
{
    return _key;
}