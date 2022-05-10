#include "Menu.h"
#include "onClickListiners.h"
#include <Windows.h>

void Button_1(Button* btn) {
	if (btn != nullptr) {
		btn->getPToMenu()->MakeTransitionTo(2);
	}
}


void Button_5(Button* btn) {
	if (btn != nullptr) {
		btn->getPToMenu()->MakeTransitionTo(1);
	}
}


void Button_2(Button* btn) {
	MessageBox(NULL, L"alert dialog", L"Info",
		MB_OK | MB_ICONEXCLAMATION);
}


void Button_4(Button* btn) {
	MessageBox(NULL, L"Description", L"Info",
		MB_OK | MB_ICONEXCLAMATION);
}
