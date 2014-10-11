//
//  ButtonWithImage.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/5.
//
//

#ifndef __Parallelformosa_Cocos2dx__ButtonWithImage__
#define __Parallelformosa_Cocos2dx__ButtonWithImage__

#include <cocos2d.h>
#include <CocosGUI.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC::ui;
USING_NS_CC_EXT;

class ButtonWithImage:public Button{
private:
    std::string _thumbnailFilename;
    TextureResType _thumbnailTexRes;
    
    std::string _key;
    
    static const int _kActionTag;
    
    int _index;
protected:
    virtual void onSizeChanged() override;
    virtual void updateTitleLocation() override;
    virtual void updateThumbnailLocation();
    virtual void initRenderer() override;
    Node* _thumbnailRenderer;
    Size _thumbnailTextureSize;
    Scale9Sprite* _focusRenderer;
    bool _isFocus;
public:
    ButtonWithImage();
    ~ButtonWithImage();
    
    static ButtonWithImage* create(const std::string& normalImage,
                                   const std::string& selectedImage,
                                   const std::string& disableImage,
                                   const std::string& thumbnailImage,
                                   TextureResType texType);
    
    bool init(const std::string& normalImage,
              const std::string& selectedImage,
              const std::string& disabledImage,
              const std::string& thumbnailImage,
              TextureResType texType);
    
    void loadThumbnailTexture(const std::string& thumbnail,TextureResType texType);
    
    void loadFocusTexture();
    
    void setKey(std::string key);
    
    void setIndex(int index);
    
    int getIndex();
    
    std::string getKey();
    
    void setFocus(bool isFocus);
    
    void focusAction();
};

#endif /* defined(__Parallelformosa_Cocos2dx__ButtonWithImage__) */
