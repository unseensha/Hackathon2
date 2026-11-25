#include"Button.h"


Button::Button(Texture2D tex, Vector2 pos, float scale, float rotation, Color tint, Text text)
    : Sprite(tex, pos, scale, rotation, tint), text(text)
{
  
    Vector2 textSize = MeasureTextEx(//searched and found this function using the ai just measures the text size in pixels
        this->text.font,
        this->text.content.c_str(), 
        this->text.fontSize,
        this->text.spacing
    );

    
    float btnWidth = tex.width * scale;//the scale is multiplied by later when you draw so i correct for it here.
    float btnHeight = tex.height * scale;

    this->text.pos = {//center the text pos so the text will always be in the center, 
        //theres a glitch where if you change the buttons pos now using the inherited setPos it wont update with the text but ill fix that later by overriding the setter
        pos.x + (btnWidth - textSize.x) / 2.0f,
        pos.y + (btnHeight - textSize.y) / 2.0f
    };
}

void Button::draw() const {
    Sprite::draw(); 
    DrawTextEx(text.font, text.content.c_str(), text.pos, text.fontSize, text.spacing, text.color);
}

bool Button::clicked(Vector2 mousePos, bool mouseClicked) {
    if (!mouseClicked) return false;

    Rectangle bounds = {//make a rectangular bound around the mouse
        getPos().x,
        getPos().y,
        getTexture().width * getScale(),
        getTexture().height * getScale()
    };

    return CheckCollisionPointRec(mousePos, bounds);// chekc if the mouse point collides and return it got from this example https://www.raylib.com/examples/shapes/loader.html?name=shapes_drag_puzzle
}
