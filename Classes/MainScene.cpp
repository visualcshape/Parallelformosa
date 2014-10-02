//
//  MainScene.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/29.
//
//

#include "MainScene.h"
#include "MainUIButtonFactory.h"
#include "VisibleRect.h"
#include "AppMacro.h"

Scene* MainScene::createScene(){
    auto scene = Scene::create();
    
    //layer create
    auto mainMenuUILayer = MainMenuLayer::create();
    auto mainMenuInfoLayer = MainInfoLayer::create();
    //add layer
    scene->addChild(mainMenuUILayer,0);
    scene->addChild(mainMenuInfoLayer,1);
    
    return scene;
}

//////////////////////
//Main menu ui Layer//
//////////////////////
MainMenuLayer::MainMenuLayer(){
    
}

bool MainMenuLayer::init(){
    bool ret = false;
    
    //Super init
    if(!Layer::init())
        return false;
    
    //Layout created
    _mainLayout = Layout::create();
    Sprite* tmpSprite = Sprite::create("UI/MainUI_Background.png");
    Size layoutSize = tmpSprite->getContentSize();
    _mainLayout->setLayoutType(cocos2d::ui::Layout::Type::HORIZONTAL);
    _mainLayout->setAnchorPoint(Vec2::ZERO);
    _mainLayout->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x, VisibleRect::getVisibleRect().origin.y));
    //Button
    std::function<void(Ref*,Widget::TouchEventType)> bindFunction;
    bindFunction = CC_CALLBACK_2(MainMenuLayer::BuildingButtonCallback, this);
    _buildingButton = MainUIButtonFactory::create(Vec2(layoutSize.width/2,layoutSize.height/2), bindFunction);
    CCASSERT(_buildingButton!=nullptr, "_buildingButton cannot be null");
    bindFunction = CC_CALLBACK_2(MainMenuLayer::UnitButtonCallback, this);
    _unitButton = MainUIButtonFactory::create(Vec2(layoutSize.width/2, layoutSize.height/2), bindFunction);
    CCASSERT(_unitButton!=nullptr, "_unitButton cannot be null");
    bindFunction = CC_CALLBACK_2(MainMenuLayer::ItemButtonCallback, this);
    _itemButton = MainUIButtonFactory::create(Vec2(layoutSize.width/2, layoutSize.height/2), bindFunction);
    CCASSERT(_itemButton!=nullptr, "_itemButton cannot be null");
    bindFunction = CC_CALLBACK_2(MainMenuLayer::StatusButtonCallback, this);
    _statusButton = MainUIButtonFactory::create(Vec2(layoutSize.width/2, layoutSize.height/2), bindFunction);
    CCASSERT(_statusButton!=nullptr, "_statusButton cannot be null");
    bindFunction = CC_CALLBACK_2(MainMenuLayer::AlliesButtonCallback, this);
    _alliesButton = MainUIButtonFactory::create(Vec2(layoutSize.width/2, layoutSize.height/2), bindFunction);
    CCASSERT(_alliesButton!=nullptr, "_alliesButton cannot be null");
    
    //Option
    bindFunction = CC_CALLBACK_2(MainMenuLayer::OptionButtonCallback, this);
    _optionButton = MainUIButtonFactory::create(Vec2(VisibleRect::getVisibleRect().size.width, 0), bindFunction);
    CCASSERT(_optionButton!=nullptr,"_optionButton cannot be null");
    _optionButton->setAnchorPoint(Vec2(1.0,0.0));
    this->addChild(_optionButton);
    //
    
    Button* mainButtonArray[] = {_buildingButton,_unitButton,_itemButton,_statusButton,_alliesButton};
    for(int i = 0 ; i < 5 ; i++){
        _mainLayout->addChild(mainButtonArray[i]);
    }
    
    //Add to layer
    this->addChild(_mainLayout);
    ret = true;
    return ret;
}

void MainMenuLayer::BuildingButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
    //Used for test
    MainUIInfoModel::getInstance()->setScrollingText("Building");
    WindowProtocol* p = WindowProtocol::create("Buildings", nullptr);
    addChild(p,100);
}

void MainMenuLayer::UnitButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
    
}

void MainMenuLayer::ItemButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
    
}

void MainMenuLayer::StatusButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
    
}

void MainMenuLayer::AlliesButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
    
}

void MainMenuLayer::OptionButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
    
}
///////////////////

///////////////////
//Main Info Layer//
///////////////////

MainInfoLayer::MainInfoLayer(){
    
}

MainInfoLayer::~MainInfoLayer(){
    if(_bindModel!=nullptr){
        _bindModel->Detach(this);
        _bindModel = nullptr;
    }
}

bool MainInfoLayer::init(){
    bool ret = false;
    
    if(!Layer::init())
        return false;
    
    //Bind with model
    _bindModel = MainUIInfoModel::getInstance();
    _bindModel->Attach(this);
    
    //background
    Sprite* infoBackground = Sprite::create("UI/MainInfo_Background.png");
    CCASSERT(infoBackground!=nullptr, "infoBackground Should not be null");
    infoBackground->setAnchorPoint(Vec2(0.0, 1.0));
    infoBackground->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x,VisibleRect::getVisibleRect().size.height));
    //Scrolling Text
    TTFConfig config("fonts/Avenir.ttf",computeFontSize(24));
    _scrollingLabel = Label::createWithTTF(config,_bindModel->getScrollingText(),TextHAlignment::CENTER);
    CCASSERT(_scrollingLabel!=nullptr, "_scrolling text cannot be null");
    _scrollingLabel->setAnchorPoint(Vec2(0.0,0.5));
    _scrollingLabel->setColor(Color3B(119, 79, 56));
    //Notice(Add to infoBackground)
    _infoNoticeBackground = Sprite::create("UI/MainInfo_Notice.png");
    CCASSERT(_infoNoticeBackground!=nullptr, "infoNoticeBackground cannot be null.");
    _infoNoticeBackground->setAnchorPoint(Vec2(0.0, 0.5));
    //Position x will be modified
    _infoNoticeBackground->setPosition(Vec2(50, infoBackground->getContentSize().height/2));
    //set scrolling text position.
    _scrollingLabel->setPosition(_infoNoticeBackground->getPosition());
    //Mask
    Sprite* infoMask = Sprite::create("UI/MainInfo_Mask.png");
    CCASSERT(infoMask!=nullptr, "infoMask cannot be null.");
    infoMask->setAnchorPoint(Vec2(0.0,1.0));
    infoMask->setPosition(infoBackground->getPosition());
    
    //add to background
    infoBackground->addChild(_scrollingLabel,1);
    infoBackground->addChild(_infoNoticeBackground,0);
    
    //add to layer
    addChild(infoBackground,0);
    addChild(infoMask,1);
    
    //add a schedule
    schedule(schedule_selector(MainInfoLayer::_scroll), 0.01f);
    
    _bindModel->setScrollingText("Test...");
    
    ret = true;
    return ret;
}

void MainInfoLayer::Update(Subject *changedSubject){
    MainUIInfoModel* changedModel = dynamic_cast<MainUIInfoModel*>(changedSubject);
    CCASSERT(changedModel!=nullptr, "ChangedModel cannot be null.");
    
    switch (changedModel->getChagedData()) {
        case MainUIInfoModel::SCROLLING_TEXT:
            _scrollingTextModelChanged();
            break;
        case MainUIInfoModel::NONE:
            break;
        default:
            break;
    }
}

void MainInfoLayer::_scrollingTextModelChanged(){
    _scrollingLabel->setString(_bindModel->getScrollingText());
    _scrollingLabel->setPosition(Vec2(_infoNoticeBackground->getPosition().x+_infoNoticeBackground->getContentSize().width,_infoNoticeBackground->getPosition().y));
}

void MainInfoLayer::_scroll(float delta){
    Vec2 prePos = _scrollingLabel->getPosition();
    Vec2 postPos(prePos.x-1.f,prePos.y);
    float rePositionThreshold = _infoNoticeBackground->getPosition().x-_scrollingLabel->getContentSize().width-2;
    if(postPos.x<rePositionThreshold){
        postPos = Vec2(_infoNoticeBackground->getPosition().x+_infoNoticeBackground->getContentSize().width, prePos.y);
    }
    
    _scrollingLabel->setPosition(postPos);
}