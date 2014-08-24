# Parallelformosa
##Libarary Dependencies
1. SQL Lite 3.
2. libpomelo.

##The Class API
###-Dialogue Window
#####Includes
	"DialogueWindowConfirm.h"
#####API
	DialogueWindowConfirm* DialogueWindowConfirm::create(std::string title,cocos2d::Color3B titleColor,std::string content,Color3B contentColor,std::function<cocos2d::Ref*,cocos2d::ui::Widget::TouchEventType>* callback)
	
__Notice__
You must bind callback first.

####Example

	//bind
	std::function<void(Ref*,cocos2d::ui::Widget::TouchEventType)> bind(std::bind(&MenuLayer::dialogueCallback, this,std::placeholders::_2));
	DialogueWindowConfirm* dialogue = DialogueWindowConfirm::create("Test", Color3B(0, 0, 0), "Hello World", Color3B(0,0,0),&bind);
    addChild(dialogue,100,"Dialogue");
