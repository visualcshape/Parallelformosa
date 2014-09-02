#include "GameLayer\LoginTitleLayer.h"
#include "VisibleRect.h"
#include "AppMacro.h"
#include <pomelo.h>
#include "DialogueWindowConfirm.h"
#include "SceneManager.h"

USING_NS_CC;

//////////////////////////
//class LoginTitleLayer //
//////////////////////////
LoginTitleLayer::LoginTitleLayer(){
}

LoginTitleLayer::~LoginTitleLayer(){
}

bool LoginTitleLayer::init(){
	if (!BaseLayer::init()) return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    //Put item
	Layer* menuLayer = MenuLayer::create();
	Layer* infoLayer = InfoLayer::create();

    menuLayer->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y));
    infoLayer->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y));

    addChild(menuLayer,1);
    addChild(infoLayer,0);

    return true;
}

/////////////
//MenuLayer//
/////////////

MenuLayer::MenuLayer(){
    
}
MenuLayer::~MenuLayer(){
}

bool MenuLayer::init(){
	if (!BaseLayer::init()) return false;

	m_aboutMenuItemImage = MenuItemImage::create("LoginTitle/about_normal.png", "LoginTitle/about_selected.png", "LoginTitle/about_disabled.png", CC_CALLBACK_1(MenuLayer::aboutClickCallback, this));
	CC_ASSERT(m_aboutMenuItemImage != NULL);
	m_aboutMenuItemImage->setAnchorPoint(Vec2(0.0, 1.0));
	m_aboutMenuItemImage->setPosition(Vec2(750, VisibleRect::getVisibleRect().origin.y + 100));

	m_settingMenuItemImage = MenuItemImage::create("LoginTitle/setting_normal.png", "LoginTitle/setting_selected.png", "LoginTitle/setting_disabled.png", CC_CALLBACK_1(MenuLayer::settingClickCallback, this));
	CC_ASSERT(m_settingMenuItemImage != NULL);
	m_settingMenuItemImage->setAnchorPoint(Vec2(0.0, 1.0));
	m_settingMenuItemImage->setPosition(Vec2(855, VisibleRect::getVisibleRect().origin.y + 100));

	TTFConfig ttfConfig("fonts/Silom.ttf", computeFontSize(18 * 4));
	m_touchToStartLabel = Label::createWithTTF(ttfConfig, "Touch to Start", TextHAlignment::CENTER);
	CC_ASSERT(m_touchToStartLabel != nullptr);

	m_touchToStartMenuItemLabel = CustomMenuItemLabel::create(m_touchToStartLabel, CC_CALLBACK_1(MenuLayer::startClickCallback, this));
	CC_ASSERT(m_touchToStartMenuItemLabel != NULL);
	m_touchToStartMenuItemLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_touchToStartMenuItemLabel->setPosition(Vec2(VisibleRect::getVisibleRect().size.width / 2, VisibleRect::getVisibleRect().origin.y + 180));
	//run action
	loopTouchToStartMenuItemLabel();

	//A transparent sprite..
	auto transparentSprite = Sprite::create();
	CC_ASSERT(transparentSprite != nullptr);
	transparentSprite->setTextureRect(Rect(VisibleRect::getVisibleRect().origin.x, VisibleRect::getVisibleRect().origin.y, VisibleRect::getVisibleRect().size.width, VisibleRect::getVisibleRect().size.height));

	//Add transparent sprite as menu item
	m_transparentStartSprite = MenuItemSprite::create(transparentSprite, NULL);
	CC_ASSERT(m_transparentStartSprite != NULL);
	m_transparentStartSprite->setCallback(CC_CALLBACK_1(MenuLayer::startClickCallback, this));
	m_transparentStartSprite->setOpacity(0);

	//menu
	m_menu = Menu::create(m_aboutMenuItemImage, m_settingMenuItemImage, m_touchToStartMenuItemLabel, nullptr);
	CC_ASSERT(m_menu != nullptr);
	m_menu->setAnchorPoint(Vec2::ZERO);
	m_menu->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x, VisibleRect::getVisibleRect().origin.y));
	addChild(m_menu, 1);

	//Menu for transparents sprite
	m_menuForTransparentSpriteMenuItem = Menu::create(m_transparentStartSprite, NULL);
	CC_ASSERT(m_menuForTransparentSpriteMenuItem != nullptr);
	addChild(m_menuForTransparentSpriteMenuItem, 0);
	return true;
}
void MenuLayer::aboutClickCallback(Object *pSender){
    
}

void MenuLayer::settingClickCallback(Object *pSender){
    
}

void MenuLayer::startClickCallback(Object *pSender){
    
	SceneManager::goMapScreen();
	CCLOG("enter map");
	/*******************************************************/
	/* v0.1.1 just to test maplayer, don't directly login! */
	/*******************************************************/

	/////////////////////////////////////////////////////////
	//CCLOG("Start Login!");
	//_doLoginWaterFall();
	/////////////////////////////////////////////////////////
}

//Login waterfall
void MenuLayer::_doLoginWaterFall(){
    //new a client
    pc_client_t* client = pc_client_new();
    _connectServer(client);
}

void MenuLayer::_connectServer(pc_client_t* client){
    const char* ip = GATE_HOST;
    const int port = GATE_PORT;
    struct sockaddr_in address;
    Layer* thisLayer = this;
    //Notice
    LoginTitleModel::getInstance()->setBindedLayer(this);
    //
    
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    
    if(pc_client_connect(client, &address)){
		DialogueWindowConfirm* pDialogue = DialogueWindowConfirm::create("Error", Color3B(184,41,47), "Fail to connect to server", Color3B::BLACK);
        addChild(pDialogue,100,"Dialogue");
        std::function<void(Ref*,ui::Widget::TouchEventType)> callback = [=](Ref* pSender,ui::Widget::TouchEventType type){
            if(type==ui::Widget::TouchEventType::ENDED){
                thisLayer->removeChildByName("Dialogue");
				pDialogue->autorelease();
            }
        };
        pDialogue->addButtonListener(callback);
        CCLOGERROR("Fail to connect to server.");
        pc_client_destroy(client);
        return;
    }
    
    //Print on Console
    CCLOG("Connect to server.");
    //pass to next step
    _sendRequest(client);
}

void MenuLayer::_sendRequest(pc_client_t* client){
    const char* route = "gate.gateHandler.authUID";
    json_t* msg = json_object();
    json_t* uid = json_string(LoginTitleModel::getInstance()->getUID().c_str());
    json_object_set(msg, "uid", uid);
    //decref
    json_decref(uid);
    pc_request_t* req = pc_request_new();
    pc_request(client, req, route, msg, _onAuthUIDRequestCallback);
}

void MenuLayer::_onAuthUIDRequestCallback(pc_request_t* req,int status,json_t* resp){
    if(status==-1){
        DialogueWindowConfirm* pDialogue = DialogueWindowConfirm::create("Error", Color3B(184, 41, 47), "無法傳送要求至伺服器．", Color3B(0, 0, 0));
        LoginTitleModel::getInstance()->getBindedLayer()->addChild(pDialogue, 100, "Dialogue");
        std::function<void(Ref*,ui::Widget::TouchEventType)> callback = [=](Ref* pSender,ui::Widget::TouchEventType type){
            if(type==ui::Widget::TouchEventType::ENDED)
            {
                LoginTitleModel::getInstance()->getBindedLayer()->removeChildByName("Dialogue");
                pDialogue->release();
            }
        };
        CCLOG("Fail to send request to server");
    }
    else if(status==0){
        char* dumped = json_dumps(resp, 0);
        CCLOG("Server Response:\n%s",dumped);
        json_t* unpack = json_object_get(resp, "resp");
        dumped = json_dumps(unpack, 0);
        free(dumped);
        std::string jUID = json_string_value(json_object_get(unpack, "uid"));
        LoginTitleModel::getInstance()->setUID(jUID);
    }
    
    //release
    json_t* msg = req->msg;
    pc_client_t* client = req->client;
    json_decref(msg);
    pc_request_destroy(req);
    CCLOG("Released.");
    
    //stop
    pc_client_stop(client);
}
//

void MenuLayer::loopTouchToStartMenuItemLabel(){
    Color3B originalColor = m_touchToStartMenuItemLabel->getColor();
    auto toAction = TintTo::create(.3f, 69, 174, 255);
    auto recoverAction = TintTo::create(.3f, originalColor.r, originalColor.g, originalColor.b);
    CallFunc* repeat = CallFunc::create(CC_CALLBACK_0(MenuLayer::loopTouchToStartMenuItemLabel, this));
    Sequence* sequence = Sequence::create(toAction,DelayTime::create(.3f),recoverAction,repeat, NULL);
    
    m_touchToStartMenuItemLabel->runAction(sequence);
}

/////////////
//InfoLayer//
/////////////

InfoLayer::InfoLayer(){
    //Add Subject
    _subject = LoginTitleModel::getInstance();
    _subject->Attach(this);
    //TODO : Set Position
    //TODO : Add child
    TTFConfig config("fonts/Avenir.ttf",computeFontSize(8*4));
    m_UIDLabel = Label::createWithTTF(config, "UID:Undefined", TextHAlignment::LEFT);
    CC_ASSERT(m_UIDLabel!=NULL);
    m_UIDLabel->setAnchorPoint(Vec2(0.0, 1.0));
    m_UIDLabel->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x+20,VisibleRect::getVisibleRect().size.height-3));
    addChild(m_UIDLabel,2);
    //Notify...
    _subject->Notify();
}

InfoLayer::~InfoLayer(){
    _subject->Detach(this);
    _subject = nullptr;
}

void InfoLayer::Update(Subject* changedSubject){
    //Update uid
    if(changedSubject->getTypeName()==LoginTitleModel::getInstance()->getTypeName()){
        std::string uid = _subject->getUID();
        m_UIDLabel->setString("UID:"+uid);
    }
    return;
}