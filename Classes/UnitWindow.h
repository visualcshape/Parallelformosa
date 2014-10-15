//
//  UnitWindow.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/12.
//
//

#ifndef __Parallelformosa_Cocos2dx__UnitWindow__
#define __Parallelformosa_Cocos2dx__UnitWindow__

#include <cocos2d.h>
#include "WindowProtocol.h"
#include "UnitTypeModel.h"
#include "ButtonWithImage.h"
#include "AppMacro.h"
#include "LayoutFactory.h"

USING_NS_CC;
USING_NS_CC::ui;
using namespace std;

class UnitWindow:public WindowProtocol
{
private:
    std::function<void(Ref*,Widget::TouchEventType)> _trainButtonPressedCallback;
    
    static string _buttonName;
    static int _buttonZOrder;
    
    Layout* _unitMainLayout;
    Layout* _leftUnitLayout;
    Layout* _rightUnitLayout;
    
    //Cost
    Text* _gPowerValue;
    Text* _lManaValue;
    Text* _foodValue;
    Text* _durationValue;
    
    //Info
    Text* _hpValue;
    Text* _rangeValue;
    Text* _typeValue;
    Text* _atkValue;
    Text* _defValue;
    
    ui::ScrollView* _descriptionScrollView;
    Text* _descriptionValue;
    
    ListView* _unitButtonListView;
    
    Button* _plusButton;
    Button* _minusButton;
    Button* _trainButton;
    Text* _amounts;
    
    int _curAmt;
    
    ButtonWithImage* _curButton;
    
    void _updateValues(string key);
    
    void _setFocus(int index);
    
    void _refreshLayout();
protected:
    UnitWindow(string titleText,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> trainButtonPressedCallback);
public:
    ~UnitWindow();
    
    static UnitWindow* create(string titleText,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> trainButtonCallback);
    
    virtual bool init();
    
    int getCurrentAmount();
    
    ButtonWithImage* getCurrentButton();
    
    void UnitButtonPressedCallback(Ref* pSender,Widget::TouchEventType type);
    
    void PlusButtonPressedCallback(Ref* pSender,Widget::TouchEventType type);
    
    void MinusButtonPressedCallback(Ref* pSender,Widget::TouchEventType type);
    
    virtual ButtonWithImage* getCurButton() override;
    //set focus not now
};

#endif /* defined(__Parallelformosa_Cocos2dx__UnitWindow__) */
