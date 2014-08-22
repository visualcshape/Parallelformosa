//
//  DialogueWindow.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/8/19.
//
//

#ifndef Parallelformosa_Cocos2dx_DialogueWindow_h
#define Parallelformosa_Cocos2dx_DialogueWindow_h

#include <cocos2d.h>
#include <string>
#include <cocos-ext.h>
#include <CocosGUI.h>

class DialogueWindow:public cocos2d::Layer
{
private:
    cocos2d::extension::Scale9Sprite* _baseWindow;
    cocos2d::LayerColor* _baseLayer;
    //should change depend on content..
    cocos2d::Sprite* _contentBackground;
    
    //UI
    cocos2d::ui::Text* _titleText;
    cocos2d::ui::RichText* _contentRichText;
    
    //data
    std::string _title;
    std::string _content;
    
    cocos2d::Color3B _titleColor;
    cocos2d::Color3B _contentColor;
protected:
    DialogueWindow();
public:
    CREATE_FUNC(DialogueWindow);
    ~DialogueWindow();
    //virtual DialogueWindow* create() = 0;
    virtual bool init();
    
    //set
    void setTitleText(std::string text);
    void setContentText(std::string text);
    void setTitletextColor(cocos2d::Color3B color);
    void setContentTextColor(cocos2d::Color3B color);
    
    //get
    cocos2d::extension::Scale9Sprite* getBaseWindow();
    cocos2d::Sprite* getContentBackground();
    cocos2d::ui::Text* getTitleText();
    cocos2d::ui::RichText* getContentRichText();
};

#endif
