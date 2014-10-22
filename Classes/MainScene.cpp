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
#include "BuildingModel.h"
#include "SceneManager.h"
#include "ConnectingSign.h"
#include "NoticeBox.h"
#include "OptionWindow.h"
#include <UtilFunc.h>

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
	CCLOG("MainMenuLayer construct");
	mm = MapModel::getModel();
	mm->Attach(this);
}

MainMenuLayer::~MainMenuLayer(){
	CCLOG("MainMenuLayer destruct");
	mm->Detach(this);
	mm = nullptr;
	removeAllChildren();
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
    _buildingButton = MainUIButtonFactory::create(Vec2(layoutSize.width/2,layoutSize.height/2), bindFunction,"UI/buildingNormal.png","UI/buildingPressed.png");
    CCASSERT(_buildingButton!=nullptr, "_buildingButton cannot be null");

    bindFunction = CC_CALLBACK_2(MainMenuLayer::UnitButtonCallback, this);
    _unitButton = MainUIButtonFactory::create(Vec2(layoutSize.width/2, layoutSize.height/2), bindFunction,"UI/unitNormal.png","UI/unitPressed.png");
    CCASSERT(_unitButton!=nullptr, "_unitButton cannot be null");

    bindFunction = CC_CALLBACK_2(MainMenuLayer::MapButtonCallback, this);
    _statusButton = MainUIButtonFactory::create(Vec2(layoutSize.width/2, layoutSize.height/2), bindFunction,"UI/mapNormal.png","UI/mapPressed.png");
    CCASSERT(_statusButton!=nullptr, "_statusButton cannot be null");
    
    //Option
    bindFunction = CC_CALLBACK_2(MainMenuLayer::OptionButtonCallback, this);
    _optionButton = MainUIButtonFactory::create(Vec2(VisibleRect::getVisibleRect().size.width, 0), bindFunction,"UI/optionNormal.png","UI/optionPressed.png");
    CCASSERT(_optionButton!=nullptr,"_optionButton cannot be null");
    _optionButton->setAnchorPoint(Vec2(1.0,0.0));
    this->addChild(_optionButton);
    //
    
    Button* mainButtonArray[] = {_buildingButton,_unitButton,_statusButton};
    for(int i = 0 ; i < 3 ; i++){
        _mainLayout->addChild(mainButtonArray[i]);
    }
    
    //Add to layer
    this->addChild(_mainLayout);
    ret = true;
    return ret;
}

void MainMenuLayer::Update(Subject* _subject){
	setPosition(mm->getHUDBasePosition());
}

void MainMenuLayer::BuildingButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
	//Used for test
	if (type == Widget::TouchEventType::ENDED)
	{
		std::string windowName = "BuildingWindow";
		MainUIInfoModel::getInstance()->setScrollingText("Building");
		Layer* thisLayer = this;
		BuildingWindow* p = BuildingWindow::create("Building", [=](Ref* pSender, Widget::TouchEventType type){
			if (type == Widget::TouchEventType::ENDED)
				thisLayer->removeChildByName(windowName);
		}, CC_CALLBACK_2(MainMenuLayer::BuildingButtonCallbackEnded, this));
		ID = p->getCurButton()->getID();
		addChild(p, 100, windowName);
	}
}
void MainMenuLayer::BuildingButtonCallbackEnded(cocos2d::Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::ENDED)
	{
		BuildingWindow* p = dynamic_cast<BuildingWindow*>(this->getChildByName("BuildingWindow"));

		int ID = p->getCurButton()->getID();
		this->removeChildByName("BuildingWindow");

		//@brief later modify
		MapModel::getModel()->clickToAddBuildingCursor(ID);
	}
}

void MainMenuLayer::UnitButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::ENDED)
	{
		std::string windowName = "UnitWindow";
		MainUIInfoModel::getInstance()->setScrollingText("Unit");
		Layer* thisLayer = this;
		UnitWindow* p = UnitWindow::create("Unit", [=](Ref* pSender, Widget::TouchEventType type){
			if (type == Widget::TouchEventType::ENDED)
				thisLayer->removeChildByName(windowName);
        }, CC_CALLBACK_2(MainMenuLayer::TrainButtonPressedCallback, this));
		addChild(p, 100, windowName);
	}
}

void MainMenuLayer::TrainButtonPressedCallback(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(type==Widget::TouchEventType::ENDED)
    {
        ConnectingSign* cs = ConnectingSign::create("Connecting", Color3B::WHITE);
        addChild(cs,100000,"cs");
        
        NoticeBox* nb = NoticeBox::create("Notice", "Sending Request");
        addChild(nb,100000,"nb");
        
        auto unitTypeMap = UnitTypeModel::getInstance()->getUnitTypeMap();
        auto curPlayer = PlayerManager::getInstance()->getCurPlayer();
        auto button = dynamic_cast<Button*>(pSender);
        CC_ASSERT(button!=nullptr);
        auto window = dynamic_cast<UnitWindow*>(button->getBindedObject());
        
        auto troopName = window->getCurButton()->getKey();
        
        auto itr = unitTypeMap->find(troopName);
        CC_ASSERT(itr!=unitTypeMap->end());
        
        int consumedGPower = itr->second.gPowerCost*window->getCurrentAmount();
        int consumedLMana = itr->second.lManaCost*window->getCurrentAmount();
        int consumedFood = itr->second.foodCost*window->getCurrentAmount();
        
        //set to player
        curPlayer->setGmag(curPlayer->getGmag()-consumedGPower);
        curPlayer->setLstr(curPlayer->getLstr()-consumedLMana);
        curPlayer->setFood(curPlayer->getFood()-consumedFood);
        
        if(itr->second.name=="Sword Man")
        {
            curPlayer->setPlayerOwnedSwordMan(curPlayer->getPlayerOwnedSwordMan()+window->getCurrentAmount());
        }
        else if(itr->second.name=="Archer")
        {
            curPlayer->setPlayerOwnedArcher(curPlayer->getPlayerOwnedArcher()+window->getCurrentAmount());
        }
        else if(itr->second.name=="Priest")
        {
            curPlayer->setPlayerOwnedPriest(curPlayer->getPlayerOwnedPriest()+window->getCurrentAmount());
        }
        else if(itr->second.name=="Magician")
        {
            curPlayer->setPlayerOwnedMagician(curPlayer->getPlayerOwnedMagician()+window->getCurrentAmount());
        }
        else
        {
            CC_ASSERT(false);
        }
        //send to server
        const char* route = "parallelSpace.parallelSpaceHandler.trainTroop";
        Json::Value msg;
        Json::FastWriter writer;
        
        msg["troopName"] = itr->second.name;
        msg["troopCount"] = window->getCurrentAmount();
        
        CCPomeloWrapper::getInstance()->notify(route, writer.write(msg), nullptr);
        
        removeChildByName("cs");
        removeChildByName("nb");
        removeChildByName("UnitWindow");
    }
}

void MainMenuLayer::ItemButtonCallback(Ref* pSender, Widget::TouchEventType type){

}

void MainMenuLayer::MapButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::ENDED)
	{
		MapWindow* mw = MapWindow::create("Find Map", [=](Ref* pSender, Widget::TouchEventType type){
			this->removeChildByName("mw");
		}, [=](Ref* pSender, Widget::TouchEventType type){

			if (type == Widget::TouchEventType::ENDED){
				TextField* txt = dynamic_cast<TextField*>(((Button*)pSender)->getBindedObject());
				CC_ASSERT(txt != nullptr);
				string mapCoord = txt->getStringValue();
				const char* route = "parallelSpace.parallelSpaceHandler.queryMapInfo";
				Json::Value root;
				Json::FastWriter writer;

				root["queriedMapCoord"] = mapCoord;

				CCPomeloWrapper::getInstance()->request(route, writer.write(root), CC_CALLBACK_1(MainMenuLayer::queriedMapResultCallback, this));
				txt->setText("");
			}
		});
		CC_ASSERT(mw != nullptr);
		this->addChild(mw, 10, "mw");
	}
}

void MainMenuLayer::queriedMapResultCallback(const CCPomeloRequestResult &result)
{
    Json::Value root;
    Json::Reader reader;
    
    if(reader.parse(result.jsonMsg, root))
	{
		CCLOG("%s", root.toStyledString().c_str());
		if (root["mapExist"].asBool() == true){
            //blocker
            NoticeBox* nb = NoticeBox::create("Notice", "Downloading Info File");
            addChild(nb,100,"mapNB");
            ConnectingSign* cs = ConnectingSign::create("Connecting", Color3B::WHITE);
            addChild(cs,100,"mapCS");
            
			string downloadPath = root["downloadPath"].asString();
            string mapCoord = root["mapCoord"].asString();
            
            UtilFunc::getInstance()->copyPlayerMapTMXFile(mapCoord);
            _curMapCoord = mapCoord;
            //download info file
            HttpRequest* req = new cocos2d::network::HttpRequest();
            req->setRequestType(HttpRequest::Type::GET);
            req->setUrl(downloadPath.c_str());
            req->setTag("Download info");
            req->setResponseCallback(CC_CALLBACK_2(MainMenuLayer::onHttpResponseCallback,this));
            HttpClient::getInstance()->send(req);
            req->release();
            //
		}
        else
        {
            string reason = root["reason"].asString();
            
            NoticeBox* nb = NoticeBox::create("Notice", reason);
            addChild(nb,100,"nbMap");
            
            auto act = FadeOut::create(1.5f);
            CallFunc* cf = CallFunc::create(CC_CALLBACK_0(MainMenuLayer::whenNBFadeOut, this));
            auto seq = Sequence::create(act,cf, NULL);
            nb->runAction(seq);
        }
    }
}

void MainMenuLayer::whenNBFadeOut()
{
    removeChildByName("nbMap");
}

void MainMenuLayer::onHttpResponseCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *resp)
{
	CC_ASSERT(resp != nullptr);
	CC_ASSERT(
		resp->isSucceed());

	//open file
#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
	FILE* fp = fopen((FileUtils::getInstance()->getWritablePath()+"/"+_curMapCoord+".info").c_str(), "wb");
#else
	FILE* fp = fopen((FileUtils::getInstance()->getWritablePath() + _curMapCoord + ".info").c_str(), "wb");
#endif
	CC_ASSERT(fp != nullptr);
	vector<char>* buffer = resp->getResponseData();
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		fwrite(&buffer->at(i), 1, 1, fp);
	}
	fclose(fp);

	removeChildByName("mapNB");
	removeChildByName("mapCS");
	if (PlayerManager::getInstance()->getCurPlayer()->getPlayerOwnMapCoord().compare(_curMapCoord + ".tmx") == 0)
		SceneManager::goMapScreen(_curMapCoord + ".tmx", HUD_ID::DEFENSE);
	else
		SceneManager::goMapScreen(_curMapCoord + ".tmx", HUD_ID::ATTACK);

}

void MainMenuLayer::AlliesButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::ENDED)
	{
		char buffer[1000];
		sprintf(buffer, "0.%d.tmx", rand() % 14);

		SceneManager::goMapScreen(buffer, HUD_ID::DEFENSE);
	}
}

void MainMenuLayer::OptionButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
    
    if(type == Widget::TouchEventType::ENDED)
    {
        OptionWindow* ow = OptionWindow::create("Options", [=](Ref* pSender,Widget::TouchEventType type){
            if(type==Widget::TouchEventType::ENDED)
            {
                this->removeChildByName("ow");
            }
        }, nullptr);
        
        addChild(ow,100,"ow");
        
    }
}
///////////////////

///////////////////
//Main Info Layer//
///////////////////
MainInfoLayer::MainInfoLayer(){
	CCLOG("MainInfoLayer construct");
	mm = MapModel::getModel();
	mm->Attach(this);
    
    PlayerModel* curPlayer = PlayerManager::getInstance()->getCurPlayer();
    _bindedPlayer = curPlayer;
    _bindedPlayer->Attach(this);
}

MainInfoLayer::~MainInfoLayer(){
	CCLOG("MainMenuLayer destruct");
	mm->Detach(this);
	mm = nullptr;
	removeAllChildren();
	if (_bindModel != nullptr){
		_bindModel->Detach(this);
		_bindModel = nullptr;
	}
    
    if(_bindedPlayer!=nullptr)
    {
        _bindedPlayer->Detach(this);
        _bindedPlayer = nullptr;
    }
}

bool MainInfoLayer::init(){
    bool ret = false;
    
    if(!Layer::init())
        return false;

	mm->setHUDBasePosition(getPosition());
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
    //Mask
    Sprite* infoMask = Sprite::create("UI/MainInfo_Mask.png");
    CCASSERT(infoMask!=nullptr, "infoMask cannot be null.");
    infoMask->setAnchorPoint(Vec2(0.0,1.0));
    infoMask->setPosition(infoBackground->getPosition());
    
    //Position x will be modified
    _infoNoticeBackground->setPosition(Vec2(53, infoMask->getPosition().y-infoMask->getContentSize().height/2));
    //set scrolling text position.
    _scrollingLabel->setPosition(_infoNoticeBackground->getPosition());
    
    _infoIcon = ImageView::create("info.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_infoIcon!=nullptr);
    _infoIcon->setAnchorPoint(Vec2(0.0, 0.5));
    _infoIcon->setPosition(Vec2(5, infoBackground->getContentSize().height/2));
    ////////////
    string fontName  = "fonts/Silom.ttf";
    float fontSize = computeFontSize(24);
    Color3B fontColor(11,72,107);
    
    LinearLayoutParameter* infoCompLLP = LinearLayoutParameter::create();
    infoCompLLP->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    infoCompLLP->setMargin(Margin(2, 0, 2, 0));

    ImageView* gPowerIcon = ImageView::create("GPower.png",Widget::TextureResType::PLIST);
    CC_ASSERT(gPowerIcon!=nullptr);
    gPowerIcon->setLayoutParameter(infoCompLLP);
    
    _gPowerValue = Text::create("000000", fontName, fontSize);
    CC_ASSERT(_gPowerValue!=nullptr);
    _gPowerValue->setLayoutParameter(infoCompLLP);
    _gPowerValue->setColor(fontColor);
    
    ImageView* lManaIcon =  ImageView::create("LMana.png",Widget::TextureResType::PLIST);
    CC_ASSERT(lManaIcon!=nullptr);
    lManaIcon->setLayoutParameter(infoCompLLP);
    
    _lManaValue = Text::create("000000", fontName, fontSize);
    CC_ASSERT(_lManaValue!=nullptr);
    _lManaValue->setLayoutParameter(infoCompLLP);
    _lManaValue->setColor(fontColor);
    
    ImageView* foodIcon = ImageView::create("Food.png",Widget::TextureResType::PLIST);
    CC_ASSERT(foodIcon!=nullptr);
    foodIcon->setLayoutParameter(infoCompLLP);
    
    _foodValue = Text::create("000000",fontName,fontSize);
    CC_ASSERT(_foodValue!=nullptr);
    _foodValue->setLayoutParameter(infoCompLLP);
    _foodValue->setColor(fontColor);
    
    ImageView* locationIcon = ImageView::create("location.png",Widget::TextureResType::PLIST);
    CC_ASSERT(locationIcon!=nullptr);
    locationIcon->setLayoutParameter(infoCompLLP);
    
    _locationValue = Text::create("0.0.00-00", fontName, fontSize);
    CC_ASSERT(_locationValue!=nullptr);
    _locationValue->setLayoutParameter(infoCompLLP);
    _locationValue->setColor(fontColor);
    
    _infoLayout = Layout::create();
    _infoLayout->setLayoutType(Layout::Type::HORIZONTAL);
    _infoLayout->setAnchorPoint(Vec2(0.0, 0.5));
    _infoLayout->setContentSize(Size(infoBackground->getContentSize().width-_infoIcon->getContentSize().width-_infoNoticeBackground->getContentSize().width-3,infoBackground->getContentSize().height));
    _infoLayout->setPosition(Vec2(_infoNoticeBackground->getContentSize().width+53+2, _infoNoticeBackground->getContentSize().height/2));
    
    _weatherIcon = Sprite::create("UI/sunny_icon.png");
    CC_ASSERT(_weatherIcon!=nullptr);
    _weatherIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _weatherIcon->setPosition(Vec2(infoMask->getContentSize().width-5, infoMask->getContentSize().height/2));
    
    //Add components...
    _infoLayout->addChild(gPowerIcon);
    _infoLayout->addChild(_gPowerValue);
    _infoLayout->addChild(lManaIcon);
    _infoLayout->addChild(_lManaValue);
    _infoLayout->addChild(foodIcon);
    _infoLayout->addChild(_foodValue);
    _infoLayout->addChild(locationIcon);
    _infoLayout->addChild(_locationValue);
    
    ////////////////////
    
    //add to background
    infoMask->addChild(_infoIcon,1);
    infoMask->addChild(_infoLayout,2);
    infoMask->addChild(_weatherIcon,3);
    
    //add to layer
    addChild(_infoNoticeBackground,-1);
    addChild(_scrollingLabel,0);
    addChild(infoMask,1);
    
    //add a schedule
    schedule(schedule_selector(MainInfoLayer::_scroll), 0.01f);
    
    _bindModel->setScrollingText("Welcome to Parallelformosa Alpha Version...");
    
    //NetWorkFunc
    PrepareNetwork();
    queryMapWeather();
    //////////////
    
    //set resource init value//
    _gPowerValue->setString(to_string(_bindedPlayer->getGmag()));
    _lManaValue->setString(to_string(_bindedPlayer->getLstr()));
    _foodValue->setString(to_string(_bindedPlayer->getFood()));
    //////////////////
    //set now coord...///
    _locationValue->setString(mm->getMapName());
    
    this->schedule(schedule_selector(MainInfoLayer::produce), 3.0f);
    
    ret = true;
    return ret;
}

void MainInfoLayer::produce(float dt)
{
    PlayerManager::getInstance()->getCurPlayer()->produce(dt);
}

void MainInfoLayer::PrepareNetwork()
{
    auto p = bind(&MainInfoLayer::onConnectLost, this);
    CCPomeloWrapper::getInstance()->setDisconnectedCallback(p);
}

void MainInfoLayer::onConnectLost()
{
    auto p = NetManager::getInstance();
    p->onConnectLost();
}

void MainInfoLayer::Update(Subject *changedSubject){
    MainUIInfoModel* changedModel = dynamic_cast<MainUIInfoModel*>(changedSubject);
    PlayerModel* playerModelChanged = dynamic_cast<PlayerModel*>(changedSubject);
    MapModel* mapModelChanged = dynamic_cast<MapModel*>(changedSubject);
    
    //changedModel == MapModel
    if (mapModelChanged != nullptr)
    {
        setPosition(mm->getHUDBasePosition());
        _refreshLayout();
        return;
    }
    //changedModel == MainUIInfoModel
	if (changedModel != nullptr)
	{
		switch (changedModel->getChagedData()) {
		case MainUIInfoModel::SCROLLING_TEXT:
			_scrollingTextModelChanged();
			break;
		case MainUIInfoModel::NONE:
			break;
		default:
			break;
		}
        _refreshLayout();
        return;
	}
    //ChangedModel==playerModel
    if(playerModelChanged!=nullptr)
    {
        int gPower = playerModelChanged->getGmag();
        int lMana = playerModelChanged->getLstr();
        int food = playerModelChanged->getFood();
        
        _gPowerValue->setString(to_string(gPower));
        _lManaValue->setString(to_string(lMana));
        _foodValue->setString(to_string(food));
        
        _refreshLayout();
        return;
    }
    
    
    CCASSERT(false,"Unknown Modelchanged or Model not attached yet");
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

void MainInfoLayer::_refreshLayout()
{
    _infoLayout->updateSizeAndPosition();
}

void MainInfoLayer::queryMapWeather()
{
    const char* route = "parallelSpace.parallelSpaceHandler.getWeatherByMapID";
    Json::Value msg;
    Json::FastWriter writer;
    msg["mapID"] = mm->getMapName();
    
    CCPomeloWrapper::getInstance()->request(route, writer.write(msg), CC_CALLBACK_1(MainInfoLayer::_onQueryWeatherRequestCallback, this));
}

void MainInfoLayer::_onQueryWeatherRequestCallback(const CCPomeloRequestResult &result)
{
    Json::Value root;
    Json::Reader reader;
    
    if(reader.parse(result.jsonMsg, root))
    {
        CCLOG("server response : %s",root.toStyledString().c_str());
        string weather = root["weather"].asString();
        if(weather=="sunny")
        {
            _weatherIcon->setTexture("UI/sunny_icon.png");
            removeChildByName("weather");
            mm->setWeather(MapModel::Weather::SUNNY);
            
            if(mm->getMapName()==PlayerManager::getInstance()->getCurPlayer()->getPlayerOwnMapCoord())
            {
                PlayerManager::getInstance()->getCurPlayer()->setPlayerOwnMapWeather(MapModel::Weather::SUNNY);
            }
        }
        else
        {
            _weatherIcon->setTexture("UI/rain_icon.png");
            _weatherLayer = WeatherLayer::create();
            //_weatherLayer->retain();
            CC_ASSERT(_weatherLayer!=nullptr);
            _weatherLayer->setPosition(VisibleRect::leftBottom());
            _weatherLayer->setAnchorPoint(Vec2(0, 0));
            addChild(_weatherLayer,10);
            mm->setWeather(MapModel::Weather::RAIN);
            
            if(mm->getMapName()==PlayerManager::getInstance()->getCurPlayer()->getPlayerOwnMapCoord())
            {
                PlayerManager::getInstance()->getCurPlayer()->setPlayerOwnMapWeather(MapModel::Weather::RAIN);
            }
        }
    }
}
