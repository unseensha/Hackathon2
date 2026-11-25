#ifndef TEXT_H
#define TEXT_H
#include "raylib.h"
#include<string>

struct Text {//just a quick agregate to store text info
    Color color;
    Vector2 pos;
    std::string content;
    Font font;
    float fontSize;
    float spacing;

};


#endif