#pragma once
#include <cocos2d.h>
#include <CocosGUI.h>
#include "Observer.h"
#include "Subject.h"
#include "MainUIInfoModel.h"
#include "BuildingWindow.h"
#include "ButtonWithImage.h"
#include "UnitWindow.h"
#include "MapModel.h"
#include "WeatherLayer.h"
#include "CCPomeloWrapper.h"
#include "json.h"
#include "NetManager.h"
#include "PlayerManager.h"
#include "MapWindow.h"
#include <HttpClient.h>

USING_NS_CC::network;
USING_NS_CC;
using namespace cocos2d::ui;

//scene
class MainScene:public Layer{
private:
protected:
public:
    static Scene* createScene();
};

//menu ui
class MainMenuLayer : public Layer, public Observer{
private:
	Layout* _mainLayout;
	Button* _buildingButton;
	Button* _unitButton;
	Button* _statusButton;
	Button* _itemButton;
	Button* _alliesButton;

	//----Other----//
	Button* _optionButton;
    
    string _curMapCoord;
protected:
public:
	virtual bool init();
	virtual void Update(Subject* _subject);

	MainMenuLayer();
	~MainMenuLayer();

	void BuildingButtonCallback(Ref* pSender, Widget::TouchEventType type);
	void BuildingButtonCallbackEnded(Ref* pSender, Widget::TouchEventType type);

	void UnitButtonCallback(Ref* pSender, Widget::TouchEventType type);
    void TrainButtonPressedCallback(Ref* pSender,Widget::TouchEventType type);

	void ItemButtonCallback(Ref* pSender, Widget::TouchEventType type);

	void MapButtonCallback(Ref* pSender, Widget::TouchEventType type);

	void AlliesButtonCallback(Ref* pSender, Widget::TouchEventType type);

	void OptionButtonCallback(Ref* pSender, Widget::TouchEventType type);

	CREATE_FUNC(MainMenuLayer);

    void queriedMapResultCallback(const CCPomeloRequestResult& result);

    void whenNBFadeOut();
    
    void onHttpResponseCallback(HttpClient* sender,HttpResponse* resp);
private:
	MapModel *mm;
	int ID;
};

//infomation (Upper)
//information Layer also manage network...
//DO NOT REPLACE THIS LAYER MAY CAUSE FAILURE...
class MainInfoLayer : public Layer, public Observer{
private:
    //Data
    MainUIInfoModel* _bindModel;
    //Components
    Label* _scrollingLabel;
    Sprite* _infoNoticeBackground;
    
    ImageView* _infoIcon;
    
    ////resource and weathers...
    Layout* _infoLayout;
    Text* _gPowerValue;
    Text* _lManaValue;
    Text* _foodValue;
    Text* _locationValue;
    
    Sprite* _weatherIcon;
    
    WeatherLayer* _weatherLayer;
    
    //model chaged
    void _scrollingTextModelChanged();
	
    //Update scrolling text position.
    void _scroll(float delta);
    
    //refresh layout
    void _refreshLayout();
    
protected:
public:
    MainInfoLayer();
    ~MainInfoLayer();
    
    virtual bool init();
    
    virtual void Update(Subject* changedSubject);
    
	CREATE_FUNC(MainInfoLayer);

    //Network
    void PrepareNetwork();
    
    void onConnectLost();
    
    void queryMapWeather();
    
    void produce(float dt);

private:
	MapModel *mm;
    PlayerModel* _bindedPlayer;
    
    void _onQueryWeatherRequestCallback(const CCPomeloRequestResult& result);
};
