//
//  BuildingWindow.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/4.
//
//

#include "BuildingWindow.h"
#include <math.h>

BuildingWindow* BuildingWindow::create(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback, std::function<void(Ref*,Widget::TouchEventType)> buildButtonPressedCallback){
    BuildingWindow* ret = new BuildingWindow(titleText,closeCallback,buildButtonPressedCallback);
    if(ret&&ret->init()){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

BuildingWindow::BuildingWindow(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback,std::function<void(Ref*,Widget::TouchEventType)> buildButtonPressedCallback):WindowProtocol(titleText,closeCallback){
    _buildButtonPressedCallback = buildButtonPressedCallback;
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
            btn = ButtonWithImage::create("buildButtonBaseNormal_Normal.png", "buildButtonBaseNormal_Pressed.png", "ButtonDisabled.png", itr->second.thumbResourceName, Widget::TextureResType::PLIST);
            btn->setName("NormalButton");
        }
        else if(itr->second.type=="Defense")
        {
            btn = ButtonWithImage::create("buildButtonBaseDefense_Normal.png", "buildButtonBaseDefense_Pressed.png", "ButtonDisabled.png", itr->second.thumbResourceName, Widget::TextureResType::PLIST);
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
    
    //////////////
    //Right layout
    _buildingRightLayout = Layout::create();
    CCASSERT(_buildingRightLayout!=nullptr, "_buildRightLayout shall not be null.");
    _buildingRightLayout->setContentSize(Size((baseSizeWithMargin.width/3)*2, baseSizeWithMargin.height));
    _buildingRightLayout->setLayoutType(Layout::Type::VERTICAL);
    
    //Compnents
    float fontSize = computeFontSize(30);
    std::string fontName = "fonts/Silom.ttf";
    
    //Apply below linear parameters (cost global)
    LinearLayoutParameter* costComponentsLP = LinearLayoutParameter::create();
    costComponentsLP->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    costComponentsLP->setMargin(Margin(2, 2, 2, 2));
    
    _gPowerIcon = ImageView::create("GPower.png",Widget::TextureResType::PLIST);
    CCASSERT(_gPowerIcon!=nullptr, "_gPowerIcon cannot be null.");
    _gPowerIcon->setLayoutParameter(costComponentsLP);
    
    _gPowerValue = Text::create("0000000",fontName,fontSize);
    CC_ASSERT(_gPowerValue!=nullptr);
    _gPowerValue->setLayoutParameter(costComponentsLP);
    
    _lManaIcon = ImageView::create("LMana.png",Widget::TextureResType::PLIST);
    CCASSERT(_lManaIcon!=nullptr, "_lMana cannot be null.");
    _lManaIcon->setLayoutParameter(costComponentsLP);
    
    _lManaValue = Text::create("0000000", fontName, fontSize);
    CC_ASSERT(_lManaValue!=nullptr);
    _lManaValue->setLayoutParameter(costComponentsLP);
    
    //Icon 1 layouts
    Layout* iconsAndTextGLLayout = Layout::create();
    LinearLayoutParameter* iatLP = LinearLayoutParameter::create();
    iatLP->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    iatLP->setMargin(Margin(0, 2, 0, 2));
    
    iconsAndTextGLLayout->setLayoutType(Layout::Type::HORIZONTAL);
    iconsAndTextGLLayout->setContentSize(Size(_gPowerIcon->getContentSize().width+_gPowerValue->getContentSize().width+_lManaIcon->getContentSize().width+_lManaValue->getContentSize().width,_gPowerIcon->getContentSize().height));
    iconsAndTextGLLayout->setLayoutParameter(iatLP);
    
    //Put components to layout
    iconsAndTextGLLayout->addChild(_gPowerIcon);
    iconsAndTextGLLayout->addChild(_gPowerValue);
    iconsAndTextGLLayout->addChild(_lManaIcon);
    iconsAndTextGLLayout->addChild(_lManaValue);
    
    
    /////
    
    _foodIcon = ImageView::create("Food.png",Widget::TextureResType::PLIST);
    CCASSERT(_foodIcon!=nullptr, "_foodIcon cannot be null.");
    _foodIcon->setLayoutParameter(costComponentsLP);
    
    _foodValue = Text::create("0000000", fontName, fontSize);
    CC_ASSERT(_foodValue!=nullptr);
    _foodValue->setLayoutParameter(costComponentsLP);
    
    _durationIcon = ImageView::create("duration.png",Widget::TextureResType::PLIST);
    CCASSERT(_durationIcon!=nullptr, "_durationIcon cannot be null.");
    _durationIcon->setLayoutParameter(costComponentsLP);
    
    _durationValue = Text::create("00H00M", fontName, fontSize);
    CC_ASSERT(_durationValue!=nullptr);
    _durationValue->setLayoutParameter(costComponentsLP);
    
    //cost 2 group layouts
    Layout* iconsAndTextFDLayout = Layout::create();
    iconsAndTextFDLayout->setLayoutType(Layout::Type::HORIZONTAL);
    iconsAndTextFDLayout->setContentSize(Size(_foodIcon->getContentSize().width+_foodValue->getContentSize().width+_durationIcon->getContentSize().width+_durationValue->getContentSize().width,_foodIcon->getContentSize().height));
    iconsAndTextFDLayout->setLayoutParameter(iatLP);
    
    //Put components to layout
    iconsAndTextFDLayout->addChild(_foodIcon);
    iconsAndTextFDLayout->addChild(_foodValue);
    iconsAndTextFDLayout->addChild(_durationIcon);
    iconsAndTextFDLayout->addChild(_durationValue);
    
    ////
    LinearLayoutParameter* infoCompnentsLP = LinearLayoutParameter::create();
    infoCompnentsLP->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    infoCompnentsLP->setMargin(Margin(2, 2, 2, 2));
    
    _healthIcon = ImageView::create("Health.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_healthIcon!=nullptr);
    _healthIcon->setLayoutParameter(infoCompnentsLP);
    
    _healthValue = Text::create("0000000", fontName, fontSize);
    CC_ASSERT(_healthValue!=nullptr);
    _healthValue->setLayoutParameter(infoCompnentsLP);
    
    _rangeIcon = ImageView::create("Range.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_rangeIcon!=nullptr);
    _rangeIcon->setLayoutParameter(infoCompnentsLP);
    
    _rangeValue = Text::create("0000000", fontName, fontSize);
    CC_ASSERT(_rangeValue!=nullptr);
    _rangeValue->setLayoutParameter(infoCompnentsLP);
    
    //1
    Layout* iconsAndTextHRLayout = Layout::create();
    iconsAndTextHRLayout->setLayoutType(Layout::Type::HORIZONTAL);
    iconsAndTextHRLayout->setContentSize(Size(_healthIcon->getContentSize().width+_healthValue->getContentSize().width+_rangeIcon->getContentSize().width+_rangeValue->getContentSize().width,_healthIcon->getContentSize().height));
    iconsAndTextHRLayout->setLayoutParameter(iatLP);
    //Put
    iconsAndTextHRLayout->addChild(_healthIcon);
    iconsAndTextHRLayout->addChild(_healthValue);
    iconsAndTextHRLayout->addChild(_rangeIcon);
    iconsAndTextHRLayout->addChild(_rangeValue);
    
    _typeIcon = ImageView::create("Type.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_typeIcon!=nullptr);
    _typeIcon->setLayoutParameter(infoCompnentsLP);
    
    _typeValue = Text::create("Undefined",fontName,fontSize);
    CC_ASSERT(_typeValue!=nullptr);
    _typeValue->setLayoutParameter(infoCompnentsLP);
    
    Layout* iconsAndTextTLayout = Layout::create();
    iconsAndTextTLayout->setLayoutType(Layout::Type::HORIZONTAL);
    iconsAndTextTLayout->setContentSize(Size(_typeIcon->getContentSize().width+_typeValue->getContentSize().width, _typeIcon->getContentSize().height));
    iconsAndTextTLayout->setLayoutParameter(iatLP);
    //PUT
    iconsAndTextTLayout->addChild(_typeIcon);
    iconsAndTextTLayout->addChild(_typeValue);
    
    
    //////////
    _costTitleText = Text::create("Cost", "fonts/Silom.ttf", computeFontSize(36));
    CC_ASSERT(_costTitleText!=nullptr);
    _costTitleText->setColor(Color3B(11,72,107));
    LinearLayoutParameter* costTitleLP = LinearLayoutParameter::create();
    costTitleLP->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    costTitleLP->setMargin(Margin(0, 0, 0, 0));
    _costTitleText->setLayoutParameter(costTitleLP);
    
    _infoTitleText = Text::create("Info", "fonts/Silom.ttf", computeFontSize(36));
    CC_ASSERT(_infoTitleText!=nullptr);
    _infoTitleText->setColor(Color3B(11,72,107));
    LinearLayoutParameter* infoLP = LinearLayoutParameter::create();
    infoLP->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    infoLP->setMargin(Margin(0, 0, 0, 0));
    _infoTitleText->setLayoutParameter(infoLP);
    
    _descrptionTitleTitleText = Text::create("Description", "fonts/Silom.ttf", computeFontSize(36));
    CC_ASSERT(_descrptionTitleTitleText!=nullptr);
    _descrptionTitleTitleText->setColor(Color3B(11, 72, 107));
    LinearLayoutParameter* descriptionLP = LinearLayoutParameter::create();
    descriptionLP->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    descriptionLP->setMargin(Margin(0, 0, 0, 0));
    _descrptionTitleTitleText->setLayoutParameter(descriptionLP);
    ////
    
    //***Attention : 80 px for building button....
    _descriptionScrollView = ui::ScrollView::create();
    Size descriptionScrollViewSize(_buildingRightLayout->getContentSize().width,_buildingRightLayout->getContentSize().height-_costTitleText->getContentSize().height-iconsAndTextGLLayout->getContentSize().height-iconsAndTextFDLayout->getContentSize().height-_infoTitleText->getContentSize().height-iconsAndTextHRLayout->getContentSize().height-iconsAndTextTLayout->getContentSize().height-_descrptionTitleTitleText->getContentSize().height-60);
    _descriptionScrollView->setContentSize(descriptionScrollViewSize);
    
    //
    Layout* dvLayout = Layout::create();
    dvLayout->setLayoutType(Layout::Type::VERTICAL);
    _descriptionValue = Text::create(TEST_TEXT, "AmericanTypewriter", fontSize);
    float valueHeight = _descriptionValue->getContentSize().height;
    float valueWidth = _descriptionValue->getContentSize().width;
    float rows = roundf(valueWidth/descriptionScrollViewSize.width);
    _descriptionValue->ignoreContentAdaptWithSize(false);
    _descriptionValue->setContentSize(Size(descriptionScrollViewSize.width,rows*valueHeight));
    dvLayout->setContentSize(Size(descriptionScrollViewSize.width,rows*valueHeight));
    dvLayout->addChild(_descriptionValue);
    //
    
    _descriptionScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _descriptionScrollView->setTouchEnabled(true);
    _descriptionScrollView->setBounceEnabled(true);
    _descriptionScrollView->setInnerContainerSize(dvLayout->getContentSize());
    _descriptionScrollView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _descriptionScrollView->setBackGroundColor(Color3B(59, 134, 134));
    _descriptionScrollView->setBackGroundColorOpacity(255*0.25);
    _descriptionScrollView->addChild(dvLayout);
    
    Button* buildButton = Button::create("BlueButton.png","BlueButtonPressed.png","ButtonDisabled.png",Widget::TextureResType::PLIST);
    CC_ASSERT(buildButton!=nullptr);
    buildButton->setScale9Enabled(true);
    buildButton->setCapInsets(Rect(1, 1, buildButton->getContentSize().width-2, buildButton->getContentSize().height-2));
    buildButton->setContentSize(Size(_buildingRightLayout->getContentSize().width/4, _buildingRightLayout->getContentSize().height-_costTitleText->getContentSize().height-iconsAndTextGLLayout->getContentSize().height-iconsAndTextFDLayout->getContentSize().height-_infoTitleText->getContentSize().height-iconsAndTextHRLayout->getContentSize().height-iconsAndTextTLayout->getContentSize().height-_descrptionTitleTitleText->getContentSize().height-_descriptionScrollView->getContentSize().height-20));
    buildButton->setTitleText("Build");
    buildButton->setTitleFontName(fontName);
    buildButton->setTitleFontSize(fontSize);
    buildButton->setAnchorPoint(Vec2(1.0, 0.0));
    buildButton->setPosition(Vec2( _buildingRightLayout->getContentSize().width,0));
    //Callback
    buildButton->addTouchEventListener(_buildButtonPressedCallback);
    
    Layout* buildButtonLayout = Layout::create();
    buildButtonLayout->setLayoutType(Layout::Type::ABSOLUTE);
    buildButtonLayout->setContentSize(Size(_buildingRightLayout->getContentSize().width, buildButton->getContentSize().height));
    buildButtonLayout->addChild(buildButton);
    
    //right layout
    _buildingRightLayout->addChild(_costTitleText);
    _buildingRightLayout->addChild(iconsAndTextGLLayout);
    _buildingRightLayout->addChild(iconsAndTextFDLayout);
    _buildingRightLayout->addChild(_infoTitleText);
    _buildingRightLayout->addChild(iconsAndTextHRLayout);
    _buildingRightLayout->addChild(iconsAndTextTLayout);
    _buildingRightLayout->addChild(_descrptionTitleTitleText);
    _buildingRightLayout->addChild(_descriptionScrollView);
    _buildingRightLayout->addChild(buildButtonLayout);
    
    LinearLayoutParameter* rlp = LinearLayoutParameter::create();
    rlp->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    rlp->setMargin(Margin(2, 0, 0, 0));
    _buildingRightLayout->setLayoutParameter(rlp);
    
    ////left layout implet.
    _buildingLeftLayout->addChild(_buildingButtonsListView);
    LinearLayoutParameter* llp = LinearLayoutParameter::create();
    llp->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    llp->setMargin(Margin(0, 0, 2, 0));
    _buildingLeftLayout->setLayoutParameter(llp);
    
    //main
    _buildingMainLayout->addChild(_buildingLeftLayout);
    _buildingMainLayout->addChild(_buildingRightLayout);
    
    //base
    _baseLayout->addChild(_buildingMainLayout);
    //add to base
    
    ret = true;
    return ret;
}

void BuildingWindow::buildingButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(type==Widget::TouchEventType::ENDED)
    {
        CCLOG("BTN Touched.");
    }
}

float BuildingWindow::MARGIN = 2;