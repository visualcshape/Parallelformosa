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
1. sqlite3.0.dylib
2. sqlite3.dylib

##The Class API
###Dialogue Window
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
                pDialogue->autorelease();
            }
        };
        pDialogue->addButtonListener(callback);
###Notice Manager
A notice manager manages the notice obj like windows,label...

Notice Manager is a singleton.

__Notice__

__DO NOT__ put a dialogue window into manager when in the static function, it may mess up with memory.

#####Include
	#include "NoticeManager.h"
#####API
######put
Put a dialogue window by key.

	NoticeManager::getInstance->put(string key,string title,Color3B titleColor,string content,Color3B contentColor,function<void(Ref*,ui::Widget::TouchEventType)> callback)
	
######get
Get a dialogue window by key.

	NoticeManager::getInstance->get(string key)
	
######erase
Erase a dialogue window by key.

#####Example
In non-static function..

	NoticeManager::getInstance()->put("UIDNotFound", "ERROR", Color3B::RED, "UID不存在，重設UID", Color3B::BLACK, nullptr);

and you can get this item everywhere...

	auto pScene = Director::getInstance()->getRunningScene();
                DialogueWindowConfirm* pDialogue = NoticeManager::getInstance()->get("UIDNotFound");
                CCASSERT(pDialogue!=nullptr, "pDialogue cannot be null");
                pScene->addChild(pDialogue,100,"UIDNotFound");
                auto cb = [=](Ref* pSender,ui::Widget::TouchEventType type)
                {
                    if(type==ui::Widget::TouchEventType::ENDED)
                    {
                        pScene->removeChildByName("UIDNotFound");
                        //reset UID
                        LoginTitleModel::getInstance()->setUID("0");
                    }
                };
                pDialogue->addButtonListener(cb);