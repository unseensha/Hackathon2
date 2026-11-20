#ifndef TEXT_H
#define TEXT_H
#include "raylib.h"
#include<string>

struct Text {
    Color color;
    Vector2 pos;
    std::string content;
    Font font;
    float fontSize;
    float spacing;

};


#endif