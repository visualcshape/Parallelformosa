# Parallelformosa
##Libarary Dependencies
1. SQL Lite 3.
2. libpomelo.
3. jansson.lib
4. libuv.lib

### Addtional lib. for Win32
1. ws2_32.lib
2. psapi.lib
3. Iphlpapi.lib

### Additional lib. for iOS
1.sqlite3.0.dylib
2.sqlite3.dylib

##The Class API
###-Dialogue Window
#####Includes
	"DialogueWindowConfirm.h"
#####API
create

	DialogueWindowConfirm* DialogueWindowConfirm::create(std::string title,cocos2d::Color3B titleColor,std::string content,Color3B contentColor)

addButtonListener
	
		pDialogue->addButtonListener(std::function<void(Ref*,cocos2d::ui::Widget::EventListenerType)> callback);
	
__Notice__
You must bind callback first.

####Example

lambada

	std::function<void(Ref*,ui::Widget::TouchEventType)> callback = [=](Ref* pSender,ui::Widget::TouchEventType type){
            if(type==ui::Widget::TouchEventType::ENDED){
                thisLayer->removeChildByName("Dialogue");
                pDialogue->release();
            }
        };
        pDialogue->addButtonListener(callback);
