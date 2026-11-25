#ifndef BUTTON_H
#define BUTTON_H

#include "Sprite.h"
#include "Text.h"


class Button :public Sprite {//Inherits sprite since I need to draw it in scene and its basically a sprite with button functionality,
	//this is because in my scene class there are 3 arrays of text sprites and objects I didnt want to adda  fourth one 
	// for buttons. inheritance made the code a  lot cleaner and easier to read and understand plus it stops me from rewriting 
	// basically the same code as in sprite for button I know we didnt learn it yet but its a pretty basic application so it should be fine
	Text text;

public:
	Button(Texture2D tex, Vector2 pos = { 0.0f, 0.0f }, float scale = 1.0f, float rotation = 0.0f, Color tint = WHITE,
		Text text = { DARKGRAY,{ 0.0f, 0.0f }, " ", GetFontDefault(), 24.0f, 2.0f });
	bool clicked(Vector2 mousePos, bool mouseClicked);
	void draw() const override;//add the text that the button has to the draw function

};


#endif