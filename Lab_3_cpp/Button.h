#pragma once
#include <string>
#include <vector>

#include "Rect.h"
using namespace std;
class Menu;

class Button
{
public:
	Button(string& text, Menu* menu, Rect position ): text(text), menu(menu), position(position)  {};
	Rect getCoordinates() { return position; };
	string getText() { return text; };
	void setOnClickListiner(void(*onClick)(Button*)) { this->onClickFun = onClick; };
	void onClick() { if (onClickFun != nullptr) onClickFun(this); };
	Menu* getPToMenu() { return menu; };
private:
	string text;
	Menu* menu;
	Rect position;
	void(*onClickFun)(Button*) = nullptr;
};



