//
//  UnitWindow.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/12.
//
//

#include "UnitWindow.h"


UnitWindow::UnitWindow(string titleText,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> trainButtonPressedCallback):WindowProtocol(titleText,closeCallback),
    _unitMainLayout(nullptr)
{
    _trainButtonPressedCallback = trainButtonPressedCallback;
}

UnitWindow::~UnitWindow()
{
    
}

UnitWindow* UnitWindow::create(string titleText, function<void (cocos2d::Ref *, Widget::TouchEventType)> closeCallback, function<void (cocos2d::Ref *, Widget::TouchEventType)> trainButtonCallback)
{
    UnitWindow* ret = new UnitWindow(titleText,closeCallback,trainButtonCallback);
    if(ret&&ret->init())
    {
        ret->autorelease();
        
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool UnitWindow::init()
{
    if(!WindowProtocol::init())
    {
        return false;
    }
    
    //Data type already loaded
    
    Size baseSize = Size(_baseWindow->getContentSize().width-2,_baseWindow->getContentSize().height-2-_titleLayout->getContentSize().height);
    Size leftSize = Size(baseSize.width/3,baseSize.height);
    
    Size rightSize = Size((baseSize.width/3)*2,baseSize.height);
    
    //Main layout
    _unitMainLayout = Layout::create();
    CC_ASSERT(_unitMainLayout!=nullptr);
    _unitMainLayout->setContentSize(baseSize);
    _unitMainLayout->setLayoutType(Layout::Type::HORIZONTAL);
    
    //////////////
    //Left Layout
    _leftUnitLayout = Layout::create();
    _leftUnitLayout->setContentSize(leftSize);
    _leftUnitLayout->setLayoutType(Layout::Type::VERTICAL);
    LinearLayoutParameter* llp = LinearLayoutParameter::create();
    llp->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    llp->setMargin(Margin(0, 0, 2, 0));
    _leftUnitLayout->setLayoutParameter(llp);
    
    //List view
    _unitButtonListView = ListView::create();
    CC_ASSERT(_unitButtonListView!=nullptr);
    _unitButtonListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _unitButtonListView->setBounceEnabled(true);
    _unitButtonListView->setBackGroundColor(Color3B::RED);
    _unitButtonListView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _unitButtonListView->setBackGroundColorOpacity(255*0.25);
    _unitButtonListView->setContentSize(leftSize);
    
    const auto unitTypes = UnitTypeModel::getInstance()->getUnitTypeMap();
    map<string,UnitType>::const_iterator itr;
    int index;
    for(itr= unitTypes->begin(),index=0; itr!=unitTypes->end();itr++,index++)
    {
        ButtonWithImage* btn = nullptr;
        //all use green btn
        btn = ButtonWithImage::create("RedButton.png", "RedButtonPressed.png", "ButtonDisabled.png", itr->second.resourceThumb,Widget::TextureResType::PLIST);
        CC_ASSERT(btn!=nullptr);
        //
        btn->setName(itr->second.name);
        btn->setTitleText(itr->second.name);
        btn->setKey(itr->second.name);
        btn->setTitleFontName("fonts/Silom.ttf");
        btn->setTitleFontSize(computeFontSize(24));
        btn->setTitleColor(Color3B(255, 255, 255));
        btn->setScale9Enabled(true);
        btn->setCapInsets(Rect(1, 1, btn->getContentSize().width-2, btn->getContentSize().height-2));
        btn->setContentSize(Size(_unitButtonListView->getContentSize().width-2,80));
        btn->loadFocusTexture();
        btn->setFocus(false);
        btn->setIndex(index);
        btn->addTouchEventListener(CC_CALLBACK_2(UnitWindow::UnitButtonPressedCallback,this));
        
        Layout* btnLayout = Layout::create();
        btnLayout->setContentSize(btn->getContentSize());
        btn->setPosition(Vec2(btnLayout->getContentSize().width/2, btnLayout->getContentSize().height/2));
        btnLayout->addChild(btn);
        //No add name
        
        _unitButtonListView->insertCustomItem(btnLayout, index);
    }
    _unitButtonListView->setGravity(ListView::Gravity::CENTER_VERTICAL);
    _unitButtonListView->setItemsMargin(5.0f);
    _leftUnitLayout->addChild(_unitButtonListView);
    //////////////
    
    //////////////
    //Right Layout
    _rightUnitLayout = Layout::create();
    _rightUnitLayout->setContentSize(rightSize);
    _rightUnitLayout->setLayoutType(Layout::Type::HORIZONTAL);
    
    Size rightMainBlockUpper(_rightUnitLayout->getContentSize().width,(_rightUnitLayout->getContentSize().height/4)*3);
    Size rightMainBlockDown(_rightUnitLayout->getContentSize().width,_rightUnitLayout->getContentSize().height-rightMainBlockUpper.height);
    Size upperEachLayoutSize(_rightUnitLayout->getContentSize().width,rightMainBlockUpper.height/7);
    Size downEachLaoutSize(_rightUnitLayout->getContentSize().width,rightMainBlockDown.height/2);
    
    //Title
    
    //////////////
    //Main
    _unitMainLayout->addChild(_leftUnitLayout);
    
    //////////////
    //Root
    _baseLayout->addChild(_unitMainLayout);
    
    return true;
}

void UnitWindow::UnitButtonPressedCallback(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    CCLOG("123");
}