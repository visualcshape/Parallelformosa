# Parallelformosa
##Libarary Dependencies
1. SQL Lite 3.
2. libpomelo.

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
