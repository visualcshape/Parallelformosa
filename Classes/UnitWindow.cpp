//
//  UnitWindow.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/12.
//
//

#include "UnitWindow.h"
#include "PlayerManager.h"

string UnitWindow::_buttonName = "btn";
int UnitWindow::_buttonZOrder = 10;

UnitWindow::UnitWindow(string titleText,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> trainButtonPressedCallback):WindowProtocol(titleText,closeCallback),
    _unitMainLayout(nullptr)
{
    _trainButtonPressedCallback = trainButtonPressedCallback;
    _curAmt = 0;
    _curButton = nullptr;
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
        ret->_setFocus(0);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        ret->retain();
#endif
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
        btn->setID(itr->second.id);
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
        btnLayout->addChild(btn,_buttonZOrder,_buttonName);
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
    _rightUnitLayout->setLayoutType(Layout::Type::VERTICAL);
    
    Size rightMainBlockUpper(_rightUnitLayout->getContentSize().width,(_rightUnitLayout->getContentSize().height/4)*3);
    Size rightMainBlockDown(_rightUnitLayout->getContentSize().width,_rightUnitLayout->getContentSize().height-rightMainBlockUpper.height);
    Size upperEachLayoutSize(_rightUnitLayout->getContentSize().width,rightMainBlockUpper.height/7);
    Size downEachLayoutSize(_rightUnitLayout->getContentSize().width,rightMainBlockDown.height/2);
    
    //font config
    string fontName = "fonts/Silom.ttf";
    float fontSize = computeFontSize(30);
    Color3B titleColor(11,72,107);
    
    ////////Preset layout parameters
    //Title LinearLayoutParameter
    LinearLayoutParameter* titleLLP = LinearLayoutParameter::create();
    titleLLP->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    titleLLP->setMargin(Margin(0, 0, 0, 0));
    
    LinearLayoutParameter* componentLLP = LinearLayoutParameter::create();
    componentLLP->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    componentLLP->setMargin(Margin(5, 0, 5, 0));
    
    LinearLayoutParameter* descriptionLLP = LinearLayoutParameter::create();
    descriptionLLP->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    descriptionLLP->setMargin(Margin(0, 0, 0, 0));
    
    LinearLayoutParameter* addMinusAmountLLP = LinearLayoutParameter::create();
    addMinusAmountLLP->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    addMinusAmountLLP->setMargin(Margin(1, 0, 5, 0));
    
    LinearLayoutParameter* trainButtonLLP = LinearLayoutParameter::create();
    trainButtonLLP->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    trainButtonLLP->setMargin(Margin((downEachLayoutSize.width/10)*3.75, 0, 0, 0));
    
    ///////////////////////////////
    
    //title:cost
    Text* costText = Text::create("Cost", fontName,fontSize);
    CC_ASSERT(costText!=nullptr);
    costText->setLayoutParameter(titleLLP);
    costText->setColor(titleColor);
    //Title Layout
    Layout* titleCostLayout = LayoutFactory::createLayout(upperEachLayoutSize, Layout::Type::HORIZONTAL);
    titleCostLayout->addChild(costText);
    
    //each cost para
    ImageView* gPowerIcon = ImageView::create("GPower.png",Widget::TextureResType::PLIST);
    CC_ASSERT(gPowerIcon!=nullptr);
    gPowerIcon->setLayoutParameter(componentLLP);
    
    _gPowerValue = Text::create("000000",fontName,fontSize);
    CC_ASSERT(_gPowerValue!=nullptr);
    _gPowerValue->setLayoutParameter(componentLLP);
    
    ImageView* lManaIcon = ImageView::create("LMana.png",Widget::TextureResType::PLIST);
    CC_ASSERT(lManaIcon!=nullptr);
    lManaIcon->setLayoutParameter(componentLLP);
    
    _lManaValue = Text::create("000000", fontName, fontSize);
    CC_ASSERT(_lManaValue!=nullptr);
    _lManaValue->setLayoutParameter(componentLLP);
    
    Layout* costGMLayout = LayoutFactory::createLayout(upperEachLayoutSize, Layout::Type::HORIZONTAL);
    costGMLayout->addChild(gPowerIcon);
    costGMLayout->addChild(_gPowerValue);
    costGMLayout->addChild(lManaIcon);
    costGMLayout->addChild(_lManaValue);
    
    ////cost:2////
    ImageView* foodIcon = ImageView::create("Food.png",Widget::TextureResType::PLIST);
    CC_ASSERT(foodIcon!=nullptr);
    foodIcon->setLayoutParameter(componentLLP);
    
    _foodValue = Text::create("000000", fontName, fontSize);
    CC_ASSERT(_foodValue!=nullptr);
    _foodValue->setLayoutParameter(componentLLP);
    
    ImageView* durationIcon = ImageView::create("duration.png",Widget::TextureResType::PLIST);
    CC_ASSERT(durationIcon!=nullptr);
    durationIcon->setLayoutParameter(componentLLP);
    
    _durationValue = Text::create("00M00S", fontName, fontSize);
    CC_ASSERT(_durationValue!=nullptr);
    _durationValue->setLayoutParameter(componentLLP);
    
    Layout* costFDLayout = LayoutFactory::createLayout(upperEachLayoutSize, Layout::Type::HORIZONTAL);
    costFDLayout->addChild(foodIcon);
    costFDLayout->addChild(_foodValue);
    costFDLayout->addChild(durationIcon);
    costFDLayout->addChild(_durationValue);
    
    //////Info///////
    Text* infoTitle = Text::create("Info",fontName,fontSize);
    CC_ASSERT(infoTitle!=nullptr);
    infoTitle->setLayoutParameter(titleLLP);
    infoTitle->setColor(titleColor);
    
    Layout* infoTitleLayout = LayoutFactory::createLayout(upperEachLayoutSize, Layout::Type::HORIZONTAL);
    infoTitleLayout->addChild(infoTitle);
    
    ////
    ImageView* hpIcon = ImageView::create("Health.png",Widget::TextureResType::PLIST);
    CC_ASSERT(hpIcon!=nullptr);
    hpIcon->setLayoutParameter(componentLLP);
    
    _hpValue = Text::create("000000", fontName, fontSize);
    CC_ASSERT(_hpValue!=nullptr);
    _hpValue->setLayoutParameter(componentLLP);
    
    ImageView* rangeIcon = ImageView::create("Range.png",Widget::TextureResType::PLIST);
    CC_ASSERT(rangeIcon!=nullptr);
    rangeIcon->setLayoutParameter(componentLLP);
    
    _rangeValue = Text::create("000000", fontName, fontSize);
    CC_ASSERT(_rangeValue!=nullptr);
    _rangeValue->setLayoutParameter(componentLLP);
    
    Layout* infoHRLayout = LayoutFactory::createLayout(upperEachLayoutSize, Layout::Type::HORIZONTAL);
    infoHRLayout->addChild(hpIcon);
    infoHRLayout->addChild(_hpValue);
    infoHRLayout->addChild(rangeIcon);
    infoHRLayout->addChild(_rangeValue);
    
    //info 2//
    ImageView* typeMR = ImageView::create("TypeMR.png",Widget::TextureResType::PLIST);
    CC_ASSERT(typeMR!=nullptr);
    typeMR->setLayoutParameter(componentLLP);
    
    _typeValue = Text::create("Undefined",fontName,fontSize);
    CC_ASSERT(_typeValue!=nullptr);
    _typeValue->setLayoutParameter(componentLLP);
    
    ImageView* atk = ImageView::create("ATK.png",Widget::TextureResType::PLIST);
    CC_ASSERT(atk!=nullptr);
    atk->setLayoutParameter(componentLLP);
    
    _atkValue = Text::create("000000", fontName, fontSize);
    CC_ASSERT(_atkValue!=nullptr);
    _atkValue->setLayoutParameter(componentLLP);
    
    ImageView* def = ImageView::create("DEF.png",Widget::TextureResType::PLIST);
    CC_ASSERT(def!=nullptr);
    def->setLayoutParameter(componentLLP);
    
    _defValue = Text::create("000000", fontName, fontSize);
    CC_ASSERT(_defValue!=nullptr);
    _defValue->setLayoutParameter(componentLLP);
    
    Layout* infoTADLayout = LayoutFactory::createLayout(upperEachLayoutSize, Layout::Type::HORIZONTAL);
    CC_ASSERT(infoTADLayout!=nullptr);
    infoTADLayout->addChild(typeMR);
    infoTADLayout->addChild(_typeValue);
    infoTADLayout->addChild(atk);
    infoTADLayout->addChild(_atkValue);
    infoTADLayout->addChild(def);
    infoTADLayout->addChild(_defValue);
    
    ///Description title
    Text* descriptionTitle = Text::create("Description", fontName, fontSize);
    CC_ASSERT(descriptionTitle!=nullptr);
    descriptionTitle->setLayoutParameter(descriptionLLP);
    descriptionTitle->setColor(titleColor);
    
    Layout* descriptionTitleLayout = LayoutFactory::createLayout(upperEachLayoutSize, Layout::Type::HORIZONTAL);
    CC_ASSERT(descriptionTitle!=nullptr);
    descriptionTitleLayout->addChild(descriptionTitle);
    ////End of Compoments////
    
    ////Start of description and buttons////
    //setup description value
    _descriptionValue = Text::create(TEST_TEXT, "AmericanTypeWriter", fontSize);
    CC_ASSERT(_descriptionValue!=nullptr);
    float descriptionValueWidth = _descriptionValue->getContentSize().width;
    float descriptionValueHeight = _descriptionValue->getContentSize().height;
    float rows = roundf(descriptionValueWidth/downEachLayoutSize.height);
    _descriptionValue->ignoreContentAdaptWithSize(false);
    _descriptionValue->setContentSize(Size(downEachLayoutSize.width,rows*descriptionValueHeight));
    
    Layout* descriptionValueLayout = Layout::create();
    descriptionValueLayout->setContentSize(_descriptionValue->getContentSize());
    descriptionValueLayout->setLayoutType(Layout::Type::VERTICAL);
    descriptionValueLayout->addChild(_descriptionValue);
    
    _descriptionScrollView = ui::ScrollView::create();
    CC_ASSERT(_descriptionScrollView!=nullptr);
    _descriptionScrollView->setContentSize(downEachLayoutSize);
    _descriptionScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _descriptionScrollView->setTouchEnabled(true);
    _descriptionScrollView->setBounceEnabled(true);
    _descriptionScrollView->setInnerContainerSize(descriptionValueLayout->getContentSize());
    _descriptionScrollView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _descriptionScrollView->setBackGroundColor(Color3B(59, 134, 134));
    _descriptionScrollView->setBackGroundColorOpacity(255*0.25);
    
    //scrollview addChild
    _descriptionScrollView->addChild(descriptionValueLayout);
    
    ///////////////////
    //Add/Minus Button
    _plusButton = Button::create("Plus.png","PlusPressed.png","PlusDisabled.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_plusButton!=nullptr);
    _plusButton->setScale9Enabled(true);
    _plusButton->setCapInsets(Rect(5, 5, _plusButton->getContentSize().width-10, _plusButton->getContentSize().height-10));
    _plusButton->addTouchEventListener(CC_CALLBACK_2(UnitWindow::PlusButtonPressedCallback, this));
    _plusButton->setContentSize(Size(downEachLayoutSize.height,downEachLayoutSize.height));
    _plusButton->setLayoutParameter(addMinusAmountLLP);
    
    _minusButton = Button::create("Minus.png","MinusPressed.png","MinusDisabled.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_minusButton!=nullptr);
    _minusButton->setScale9Enabled(true);
    _minusButton->setCapInsets(Rect(5, 5, _minusButton->getContentSize().width-10, _minusButton->getContentSize().height-10));
    _minusButton->addTouchEventListener(CC_CALLBACK_2(UnitWindow::MinusButtonPressedCallback, this));
    _minusButton->setContentSize(Size(downEachLayoutSize.height,downEachLayoutSize.height));
    _minusButton->setLayoutParameter(addMinusAmountLLP);
    
    _amounts = Text::create("00", fontName, fontSize);
    CC_ASSERT(_amounts!=nullptr);
    _amounts->setLayoutParameter(addMinusAmountLLP);
    
    _trainButton = Button::create("BlueButton.png","BlueButtonPressed.png","ButtonDisabled.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_trainButton!=nullptr);
    _trainButton->setScale9Enabled(true);
    _trainButton->setCapInsets(Rect(2, 2, _trainButton->getContentSize().width-4, _trainButton->getContentSize().height-4));
    _trainButton->setTitleText("Train");
    _trainButton->setTitleColor(Color3B::WHITE);
    _trainButton->setTitleFontName(fontName);
    _trainButton->setTitleFontSize(fontSize);
    _trainButton->setLayoutParameter(trainButtonLLP);
    _trainButton->setContentSize(Size((downEachLayoutSize.width/10)*3,downEachLayoutSize.height));
    _trainButton->addTouchEventListener(_trainButtonPressedCallback);
    
    Size buttonAmtLayoutSize(_plusButton->getContentSize().width+_amounts->getContentSize().width+_minusButton->getContentSize().width,_plusButton->getContentSize().height);
    
    Layout* buttonAmtLayout = LayoutFactory::createLayout(buttonAmtLayoutSize, Layout::Type::HORIZONTAL);
    buttonAmtLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    buttonAmtLayout->setBackGroundColor(Color3B(59, 134, 134));
    buttonAmtLayout->setBackGroundColorOpacity(255*0.75);
    buttonAmtLayout->addChild(_plusButton);
    buttonAmtLayout->addChild(_amounts);
    buttonAmtLayout->addChild(_minusButton);
    
    Layout* buttonsLayout = LayoutFactory::createLayout(downEachLayoutSize, Layout::Type::HORIZONTAL);
    buttonsLayout->addChild(buttonAmtLayout);
    buttonsLayout->addChild(_trainButton);
    
    /////////////////////////
    //addAllComponentsLayout
    _rightUnitLayout->addChild(titleCostLayout);
    _rightUnitLayout->addChild(costGMLayout);
    _rightUnitLayout->addChild(costFDLayout);
    _rightUnitLayout->addChild(infoTitleLayout);
    _rightUnitLayout->addChild(infoHRLayout);
    _rightUnitLayout->addChild(infoTADLayout);
    _rightUnitLayout->addChild(descriptionTitleLayout);
    _rightUnitLayout->addChild(_descriptionScrollView);
    _rightUnitLayout->addChild(buttonsLayout);
    
    //////////////
    //Main
    _unitMainLayout->addChild(_leftUnitLayout);
    _unitMainLayout->addChild(_rightUnitLayout);
    
    //////////////
    //Root
    _baseLayout->addChild(_unitMainLayout);
    
    //init status
    _plusButton->setEnabled(true);
    _minusButton->setEnabled(false);
    _minusButton->setBright(false);
    _trainButton->setEnabled(false);
    _trainButton->setBright(false);
    
    //bind with this window
    _trainButton->setBindedObject(this);
    
    return true;
}

void UnitWindow::UnitButtonPressedCallback(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(type==Widget::TouchEventType::ENDED)
    {
        ButtonWithImage* btn = dynamic_cast<ButtonWithImage*>(pSender);
        CC_ASSERT(btn!=nullptr);
        _setFocus(btn->getIndex());
    }
}

void UnitWindow::PlusButtonPressedCallback(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(type==Widget::TouchEventType::ENDED)
    {
        _curAmt++;
        //update ui
        _updateValues(_curButton->getKey());
    }
}

void UnitWindow::MinusButtonPressedCallback(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        _curAmt--;
        //update ui
        _updateValues(_curButton->getKey());
    }
}

int UnitWindow::getCurrentAmount()
{
    return _curAmt;
}

void UnitWindow::_updateValues(string key)
{
    auto curPlayer = PlayerManager::getInstance()->getCurPlayer();
    auto itr = UnitTypeModel::getInstance()->getUnitTypeMap()->find(key);
    int multiplier = 0;
    
    if(_curAmt<=1)
        multiplier=1;
    else
        multiplier=_curAmt;
    
    
    _gPowerValue->setString(to_string(itr->second.gPowerCost*multiplier));
    _lManaValue->setString(to_string(itr->second.lManaCost*multiplier));
    _foodValue->setString(to_string(itr->second.foodCost*multiplier));
    
    int amtDura = itr->second.duration*multiplier;
    string dura = "";
    int min = amtDura/60;
    int sec = amtDura%60;
    if(min<10)
    {
        dura += "0"+to_string(min)+"M";
    }
    else
    {
        dura+=to_string(min)+"M";
    }
    
    if(sec<10)
    {
        dura+="0"+to_string(sec)+"S";
    }
    else
    {
        dura+=to_string(sec)+"S";
    }
    
    _durationValue->setString(dura);
    _hpValue->setString(to_string(itr->second.baseHP));
    _rangeValue->setString(to_string(itr->second.range));
    _typeValue->setString(itr->second.type);
    _atkValue->setString(to_string(itr->second.atk));
    _defValue->setString(to_string(itr->second.def));
    _descriptionValue->setString(itr->second.description);
    
    //determinate ui status
    if(_curAmt==0)
    {
        _minusButton->setEnabled(false);
        _minusButton->setBright(false);
        _trainButton->setEnabled(false);
        _trainButton->setBright(false);
    }
    else
    {
        _minusButton->setEnabled(true);
        _minusButton->setBright(true);
        _trainButton->setEnabled(true);
        _trainButton->setBright(true);
        if(curPlayer->getGmag()<itr->second.gPowerCost*multiplier)
        {
            _gPowerValue->setColor(Color3B::RED);
            _trainButton->setEnabled(false);
            _trainButton->setBright(false);
        }
        else
        {
            _gPowerValue->setColor(Color3B::WHITE);
        }
        
        if(curPlayer->getLstr()<itr->second.lManaCost*multiplier)
        {
            _lManaValue->setColor(Color3B::RED);
            _trainButton->setEnabled(false);
            _trainButton->setBright(false);
        }
        else
        {
            _lManaValue->setColor(Color3B::WHITE);
        }
        
        if(curPlayer->getFood()<itr->second.foodCost*multiplier)
        {
            _foodValue->setColor(Color3B::RED);
            _trainButton->setEnabled(false);
            _trainButton->setBright(false);
        }
        else
        {
            _foodValue->setColor(Color3B::WHITE);
        }
    }
    
    string amt = "";
    if(_curAmt<10)
    {
        amt+="0"+to_string(_curAmt);
    }
    else
    {
        amt+=to_string(_curAmt);
    }
    
    _amounts->setString(amt);
    
    _refreshLayout();
}

void UnitWindow::_setFocus(int index)
{
    Layout* layout = dynamic_cast<Layout*>(_unitButtonListView->getItem(index));
    CC_ASSERT(layout!=nullptr);
    
    ButtonWithImage* btn = dynamic_cast<ButtonWithImage*>(layout->getChildByName(_buttonName));
    CC_ASSERT(btn!=nullptr);
    
    if(_curButton!=nullptr)
    {
        if(_curButton->getKey()!=btn->getKey())
            _curAmt = 0;
        _curButton->setFocus(false);
    }
    btn->setFocus(true);
    _curButton = btn;
    
    _trainButton->setEnabled(true);
    _trainButton->setBright(true);
    _gPowerValue->setColor(Color3B::WHITE);
    _lManaValue->setColor(Color3B::WHITE);
    _foodValue->setColor(Color3B::WHITE);
    
    _updateValues(_curButton->getKey());
}

void UnitWindow::_refreshLayout()
{
    Vector<Node*> layoutChildren = _rightUnitLayout->getChildren();
    Vector<Layout*> layoutToUpdate;
    
    Vector<Node*>::iterator itr;
    for(itr=layoutChildren.begin();itr!=layoutChildren.end();itr++)
    {
        Layout* layoutToPush = dynamic_cast<Layout*>(*itr);
        if(layoutToPush!=nullptr)
        {
            layoutToUpdate.pushBack(layoutToPush);
        }
    }
    
    Vector<Layout*>::iterator lItr;
    for(lItr=layoutToUpdate.begin();lItr!=layoutToUpdate.end();lItr++)
    {
        (*lItr)->updateSizeAndPosition();
    }
}

ButtonWithImage* UnitWindow::getCurButton()
{
    return _curButton;
}