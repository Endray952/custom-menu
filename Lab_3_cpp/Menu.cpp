#include "Menu.h"
#include <fstream>
#include <iostream>
#include <sstream> 
#include <GL/glut.h>
#include <GL\freeglut.h>
#include "Rect.h"
#include "onClickListiners.cpp"
using namespace std;


Menu::Menu(const string& path){
	MenuFromConfig(path);
    current_submenu = &(menu_holder.find(1)->second);
    menu_holder.find(1)->second[0].setOnClickListiner(Button_1);
    menu_holder.find(1)->second[1].setOnClickListiner(Button_2);
    menu_holder.find(2)->second[0].setOnClickListiner(Button_4);
    menu_holder.find(2)->second[1].setOnClickListiner(Button_5);
}


void Menu::MakeTransitionTo(int id) {
    current_submenu = &(menu_holder.find(id)->second);
}


void Menu::FindFocus(int x, int y) {
    int size = (*current_submenu).size();
    for (int i = 0; i < size; i++) {
        Button btn = (*current_submenu)[i];
        
        Rect pos = btn.getCoordinates();
        if (x >= pos.x0 && x <= pos.x1 && y >= pos.y0 && y <= pos.y1) {
            vector <Button>::iterator it = (*current_submenu).begin() + i;
            current_button = &(*it);
            break;
        }
        else {
            current_button = nullptr;
        }       
    }
}


void Menu::Render() {
    for (auto& btn : *current_submenu) {
        Rect pos = btn.getCoordinates();    
        if (current_button == &btn) {
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_QUADS);
            glVertex2f(pos.x0, pos.y0);
            glVertex2f(pos.x1, pos.y0);
            glVertex2f(pos.x1, pos.y1);
            glVertex2f(pos.x0, pos.y1);
            glEnd();
            glColor3f(0.1, 1.0, 1.0);
            glBegin(GL_QUADS);
            glVertex2f(pos.x0 + 5, pos.y0 + 5);
            glVertex2f(pos.x1 - 5, pos.y0 + 5);
            glVertex2f(pos.x1 - 5, pos.y1 - 5);
            glVertex2f(pos.x0 + 5, pos.y1 - 5);
            glEnd();
        }
        else {
            glColor3f(0.1, 1.0, 1.0);
            glBegin(GL_QUADS);
            glVertex2f(pos.x0, pos.y0);
            glVertex2f(pos.x1, pos.y0);
            glVertex2f(pos.x1, pos.y1);
            glVertex2f(pos.x0, pos.y1);
            glEnd();         
        }
        string s = btn.getText();
        const char* c = s.c_str();
        glColor3f(0.0, 0.0, 0.0);
        int indent = s.length() *4;
        glRasterPos2f((pos.x0 + pos.x1)/2 - indent, (pos.y0 + pos.y1) / 2);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)c);
    }
    
}


void Menu::HandleClick() {
    if (current_button == nullptr) {
        return;
    }
    (*current_button).onClick();
}


void Menu::ParseButton(ifstream& file, vector<Button>& buttons) {
    string str;
    string text;
    Rect rect;
    while (getline(file, str) && (int)str.find("EndButton") < 0) {
        int begin = (int)str.find_first_of('\'') + 1;
        int end = (int)str.find_last_of('\'') ;
        int len = end - begin;
        int index = str.find("name");
        if (index >= 0) {
            text = str.substr(begin, len);
            continue;
        }
        index = str.find("position");
        if (index >= 0) {
            stringstream position = (stringstream)str.substr(begin, len);
            vector<string> coords;
            string segment;
            while (getline(position, segment, ',')){
                coords.push_back(segment);
            }
            rect.x0 = stoi(coords[0]);
            rect.y0 = stoi(coords[1]);
            rect.x1 = stoi(coords[2]);
            rect.y1 = stoi(coords[3]);
            continue;
        }
    }
    Button btn(text, this, rect);
    buttons.push_back(btn);
}


void Menu::ParseMenu(ifstream& file) {
    string str;
    getline(file, str);
    int index = str.find("id");
    if (index < 0) {
        return;
    }
    int begin = str.find_first_of('\'') + 1;
    int end = str.find_last_of('\'');
    int id = stoi(str.substr(begin, end));
    vector<Button> buttons ;
    map<int, vector<Button>> menu;
    while (getline(file, str) && str != "#EndMenu") {
        if (str.find("Button" >= 0)) {
            ParseButton(file, buttons);
        }
    }
    menu_holder.insert({ id, buttons });
}


void Menu::MenuFromConfig(const string& path) {
    ifstream file(path);
    string str;   
    bool endFile = false;
    while (getline(file, str)){
        int index = str.find("#") + 1;
        if (index >= 0) {
            string type = str.substr(index , str.length());
            if (type == "Menu") {
                ParseMenu(file);
            }
        }
    }
}



