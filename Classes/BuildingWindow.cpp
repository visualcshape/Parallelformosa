//
//  BuildingWindow.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/4.
//
//

#include "BuildingWindow.h"

BuildingWindow* BuildingWindow::create(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback, std::function<void()> windowDismissCallback){
    BuildingWindow* ret = new BuildingWindow(titleText,closeCallback,windowDismissCallback);
    if(ret&&ret->init()){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

BuildingWindow::BuildingWindow(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback,std::function<void()> windowDismissCallback):WindowProtocol(titleText,closeCallback){
    _windowDismissCallback = windowDismissCallback;
}

BuildingWindow::~BuildingWindow(){
    
}

bool BuildingWindow::init(){
    bool ret = false;
    
    //super init
    if(!WindowProtocol::init()){
        return false;
    }
    
    //Data types already loaded
    
    //Layout
    Size baseSize = Size(_baseWindow->getContentSize().width-_baseWindow->getCapInsets().getMinX()*2, _baseWindow->getContentSize().height-_baseWindow->getCapInsets().getMinY()*2-_titleLayout->getContentSize().height);
    //main
    _buildingMainLayout = Layout::create();
    CCASSERT(_buildingMainLayout!=nullptr, "_buildMainLayout");
    _buildingMainLayout->setContentSize(baseSize);
    _buildingMainLayout->setLayoutType(Layout::Type::HORIZONTAL);
    
    Size baseSizeWithMargin = Size(baseSize.width-MARGIN, baseSize.height);
    //Left
    _buildingLeftLayout = Layout::create();
    CCASSERT(_buildingLeftLayout!=nullptr, "_buildLeftLayout shall not be null.");
    _buildingLeftLayout->setContentSize(Size((baseSizeWithMargin.width/3)*1,baseSizeWithMargin.height));
    _buildingLeftLayout->setLayoutType(Layout::Type::VERTICAL);
    
    //Right
    _buildingRightLayout = Layout::create();
    CCASSERT(_buildingRightLayout!=nullptr, "_buildRightLayout shall not be null.");
    _buildingRightLayout->setContentSize(Size((baseSizeWithMargin.width/3)*2, baseSizeWithMargin.height));
    
    //Building button Listview
    _buildingButtonsListView = ListView::create();
    CCASSERT(_buildingButtonsListView!=nullptr, "_buildingButtonsListView shall not view");
    _buildingButtonsListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _buildingButtonsListView->setBounceEnabled(true);
    _buildingButtonsListView->setBackGroundColor(Color3B::GRAY);
    _buildingButtonsListView->setBackGroundColorOpacity(255*0.5);
    _buildingButtonsListView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _buildingButtonsListView->setContentSize(_buildingLeftLayout->getContentSize());
    
    const std::map<std::string,BuildingType>* buildingTypes = BuildingModel::getInstance()->getBuildingModelMap();
    std::map<std::string,BuildingType>::const_iterator itr;
    int index;
    for(itr=buildingTypes->begin(),index=0;itr!=buildingTypes->end();itr++,index++)
    {
        ButtonWithImage* btn = nullptr;
        if(itr->second.type=="Normal")
        {
            btn = ButtonWithImage::create("buildButtonBaseNormal_Normal.png", "buildButtonBaseNormal_Pressed.png", "", itr->second.thumbResourceName, Widget::TextureResType::PLIST);
            btn->setName("NormalButton");
        }
        else if(itr->second.type=="Defense")
        {
            btn = ButtonWithImage::create("buildButtonBaseDefense_Normal.png", "buildButtonBaseDefense_Pressed.png", "", itr->second.thumbResourceName, Widget::TextureResType::PLIST);
            btn->setName("DefenseButton");
        }
        CCASSERT(btn!=nullptr, "Button cannot be null.");
        //color
        btn->setTitleText(itr->second.name);
        btn->setKey(itr->second.name);
        btn->setTitleFontName("fonts/Silom.ttf");
        btn->setTitleFontSize(computeFontSize(24));
        btn->setTitleColor(Color3B(255, 255, 255));
        btn->setScale9Enabled(true);
        btn->setCapInsets(Rect(1, 1, btn->getContentSize().width-2, btn->getContentSize().height-2));
        btn->setContentSize(Size(_buildingButtonsListView->getContentSize().width-2,80));
        
        Layout* btnLayout = Layout::create();
        btnLayout->setContentSize(btn->getContentSize());
        btn->setPosition(Vec2(btnLayout->getContentSize().width/2, btnLayout->getContentSize().height/2));
        btnLayout->addChild(btn);
        btn->addTouchEventListener(CC_CALLBACK_2(BuildingWindow::buildingButtonCallback, this));
        //set tag noneeded...
        
        _buildingButtonsListView->insertCustomItem(btnLayout, index);
    }
    
    //put list view
    _buildingButtonsListView->setGravity(ListView::Gravity::CENTER_VERTICAL);
    _buildingButtonsListView->setItemsMargin(5.0f);
    
    _buildingLeftLayout->addChild(_buildingButtonsListView);
    LinearLayoutParameter* llp = LinearLayoutParameter::create();
    llp->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    llp->setMargin(Margin(0, 0, 2, 0));
    _buildingLeftLayout->setLayoutParameter(llp);
    _buildingMainLayout->addChild(_buildingLeftLayout);
    _baseLayout->addChild(_buildingMainLayout);
    //add to base
    
    ret = true;
    return ret;
}

void BuildingWindow::buildingButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType)
{
    CCLOG("BTN Touched.");
}

float BuildingWindow::MARGIN = 2;