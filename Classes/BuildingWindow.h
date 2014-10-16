//
//  BuildingWindow.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/4.
//
//

#ifndef __Parallelformosa_Cocos2dx__BuildingWindow__
#define __Parallelformosa_Cocos2dx__BuildingWindow__

#include "WindowProtocol.h"
#include <cocos2d.h>
#include <CocosGUI.h>
#include "AppMacro.h"
#include "ButtonWithImage.h"
#include "BuildingType.h"
#include "BuildingModel.h"
#include <map>

USING_NS_CC;
USING_NS_CC::ui;

class BuildingWindow : public WindowProtocol{
private:
    std::function<void(Ref*,Widget::TouchEventType)> _buildButtonPressedCallback;
    Layout* _buildingMainLayout;
    Layout* _buildingLeftLayout;
    Layout* _buildingRightLayout;
    ListView* _buildingButtonsListView;
    ImageView* _gPowerIcon;
    ImageView* _lManaIcon;
    ImageView* _healthIcon;
    ImageView* _rangeIcon;
    ImageView* _foodIcon;
    ImageView* _durationIcon;
    ImageView* _widthIcon;
    ImageView* _heightIcon;
    ImageView* _atkIcon;
    ImageView* _defIcon;
    Text* _atkValue;
    Text* _defValue;
    Text* _costTitleText;
    Text* _infoTitleText;
    Text* _descrptionTitleTitleText;
    Text* _gPowerValue;
    Text* _lManaValue;
    Text* _healthValue;
    Text* _durationValue;
    Text* _rangeValue;
    Text* _foodValue;
    Text* _widthValue;
    Text* _heightValue;
    ImageView* _typeIcon;
    Text* _typeValue;
    ui::ScrollView* _descriptionScrollView;
    Text* _descriptionValue;
    
    ButtonWithImage* _curBTN;
    
    static float MARGIN;
    
    static std::string GLLayoutName;
    
    static std::string FDLayoutName;
    
    static std::string HRLayoutName;
    
    static std::string TLayoutName;
    
    static std::string DVLayoutName;
    
    static int zOrder;
    
    int _curItem;
    
    void _setValue(std::string key);
    
    void _refreshLayout();
protected:
    BuildingWindow(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback,std::function<void(Ref*,Widget::TouchEventType)> buildButtonPressedCallback);
public:
    ~BuildingWindow();
    //Factory
    static BuildingWindow* create(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback, std::function<void(Ref*,Widget::TouchEventType)> buildButtonPressedCallback);
    
    virtual bool init();
    
    void setFocus(int index);
    
    void buildingButtonCallback(Ref* pSender,Widget::TouchEventType type);
    
    virtual ButtonWithImage* getCurButton() override;
};

#endif /* defined(__Parallelformosa_Cocos2dx__BuildingWindow__) */
