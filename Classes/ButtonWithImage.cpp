//
//  ButtonWithImage.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/5.
//
//

#include "ButtonWithImage.h"

static const int THUMBNAIL_IMAGE_Z = (-1);

ButtonWithImage::ButtonWithImage():
_thumbnailRenderer(nullptr){
    
}

ButtonWithImage::~ButtonWithImage(){
    
}

void ButtonWithImage::onSizeChanged(){
    Widget::onSizeChanged();
    updateThumbnailLocation();
    updateTitleLocation();
}

void ButtonWithImage::updateTitleLocation(){
    _titleRenderer->setPosition(Vec2(_thumbnailRenderer->getPosition().x+_thumbnailRenderer->getContentSize().width+2, _contentSize.height*0.5));
}

void ButtonWithImage::updateThumbnailLocation(){
    _thumbnailRenderer->setPosition(Vec2(5.0f, _contentSize.height*0.5));
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