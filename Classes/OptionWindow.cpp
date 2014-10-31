//
//  OptionWindow.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/20.
//
//

#include "OptionWindow.h"
#include "Database.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "LoginTitleModel.h"

OptionWindow::OptionWindow(string title,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> resetButtonPressedCallback):WindowProtocol(title,closeCallback)
{
    _resetButtonPressedCallback = resetButtonPressedCallback;
}

OptionWindow::~OptionWindow()
{
    
}

OptionWindow* OptionWindow::create(string title, function<void (cocos2d::Ref *, Widget::TouchEventType)> closeCallback, function<void (cocos2d::Ref *, Widget::TouchEventType)> resetButtonPressedCallback)
{
    OptionWindow* ret = new OptionWindow(title,closeCallback,resetButtonPressedCallback);
    if(ret&&ret->init())
    {
        ret->autorelease();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        ret->retain();
#endif
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool OptionWindow::init()
{
    if(!WindowProtocol::init())
    {
        return false;
    }
    
    LinearLayoutParameter* compLLP = LinearLayoutParameter::create();
    compLLP->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    compLLP->setMargin(Margin(5, 0, 0, 5));
    
    Text* musicEnabledTitle = Text::create("Enable Sound", "fonts/Silom.ttf",computeFontSize(24));
    CC_ASSERT(musicEnabledTitle!=nullptr);
    musicEnabledTitle->setLayoutParameter(compLLP);
    
    _musicEnabledCheckBox = CheckBox::create("check_box_normal.png", "check_box_normal_press.png", "check_box_active.png", "check_box_normal_disabled.png", "check_box_active_disabled.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_musicEnabledCheckBox!=nullptr);
    _musicEnabledCheckBox->addEventListener(CC_CALLBACK_2(OptionWindow::onMusicEnabledCheckBoxCallback, this));
    _musicEnabledCheckBox->setScale(0.5);
    _musicEnabledCheckBox->setLayoutParameter(compLLP);
    
    Layout* musicOptionLayout = Layout::create();
    musicOptionLayout->setLayoutType(Layout::Type::VERTICAL);
    musicOptionLayout->setContentSize(Size(_baseWindow->getContentSize().width-2,musicEnabledTitle->getContentSize().height+_musicEnabledCheckBox->getContentSize().height));
    musicOptionLayout->addChild(musicEnabledTitle);
    musicOptionLayout->addChild(_musicEnabledCheckBox);
    
    musicOptionLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    musicOptionLayout->setPosition(Vec2(_baseWindow->getContentSize().width/2, _baseWindow->getContentSize().height-_titleLayout->getContentSize().height));
    
    Text* resetTitle = Text::create("Reset Account", "fonts/Silom.ttf", computeFontSize(24));
    CC_ASSERT(resetTitle!=nullptr);
    resetTitle->setLayoutParameter(compLLP);
    
    Text* subNoticeTitle = Text::create("Attention:Use this option will cause lost of your account data.", "fonts/Silom.ttf", computeFontSize(20));
    CC_ASSERT(subNoticeTitle!=nullptr);
    subNoticeTitle->setColor(Color3B::RED);
    subNoticeTitle->setLayoutParameter(compLLP);
    
    _resetButton = Button::create("BlueButton.png","BlueButtonPressed.png","ButtonDisabled.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_resetButton!=nullptr);
    _resetButton->setLayoutParameter(compLLP);
    _resetButton->setScale9Enabled(true);
    _resetButton->setCapInsets(Rect(2, 2, _resetButton->getContentSize().width-4, _resetButton->getContentSize().height-4));
    _resetButton->setContentSize(Size(200,50));
    _resetButton->setTitleColor(Color3B::RED);
    _resetButton->setTitleFontName("fonts/Silom.ttf");
    _resetButton->setTitleFontSize(computeFontSize(24));
    _resetButton->setTitleText("Reset!");
    _resetButton->addTouchEventListener(CC_CALLBACK_2(OptionWindow::onResetButtonClick, this));
    
    Layout* resetOptions = Layout::create();
    resetOptions->setLayoutType(Layout::Type::VERTICAL);
    resetOptions->cocos2d::Node::setContentSize(Size(_baseWindow->getContentSize().width-2,resetTitle->getContentSize().height+subNoticeTitle->getContentSize().height+_resetButton->getContentSize().height));
    resetOptions->addChild(resetTitle);
    resetOptions->addChild(subNoticeTitle);
    resetOptions->addChild(_resetButton);
    resetOptions->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    resetOptions->setPosition(Vec2(_baseWindow->getContentSize().width/2, resetOptions->getContentSize().height+20));
    
    _baseWindow->addChild(musicOptionLayout);
    _baseWindow->addChild(resetOptions);
    
    if(CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
    {
        _musicEnabledCheckBox->setSelectedState(true);
    }
    else
    {
        _musicEnabledCheckBox->setSelectedState(false);
    }
    
    return true;
}

void OptionWindow::onMusicEnabledCheckBoxCallback(cocos2d::Ref *pSender, CheckBox::EventType type)
{
    if(type==CheckBox::EventType::SELECTED)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/1-2.mp3");
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
}

void OptionWindow::onResetButtonClick(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(Widget::TouchEventType::ENDED==type)
    {
        
        auto pDB = Database::getInstance()->getDatabasePointer();
        char sql[512];
        char* errMsg;
        sprintf(sql, "Update User SET ID=0,FirstTimeLogin=1 WHERE ID='%s'",LoginTitleModel::getInstance()->getUID().c_str());
        
        int result = sqlite3_exec(pDB, sql, nullptr, nullptr, &errMsg);
        CC_ASSERT(result==SQLITE_OK);
        LoginTitleModel::getInstance()->setUID("0");
        
        SceneManager::goTitleScreen();
    }
}