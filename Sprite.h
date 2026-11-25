#ifndef SPRITE_H
#define SPRITE_H

#include "raylib.h"

class Sprite {//all in one .h since its basically an agregate i didnt do a strutc since it has the draw function. 
    //Honestky should just be a struct with a function but whatverrt i already wrote it
    Texture2D tex;
    Vector2 pos;
    float scale;
    float rotation;
    Color tint;

public:
    Sprite(Texture2D tex, Vector2 pos = { 0.0f, 0.0f}, float scale=1.0f, float rotation =0.0f, Color tint =WHITE)
        : tex(tex),
        pos(pos),
        scale(scale),
        rotation(rotation),
        tint(tint) {
    }
    virtual ~Sprite() {
        UnloadTexture(tex);
    }
    virtual void draw() const {//virtual just means the class that inherits sprite can change 
        //this function and overidde it, done for button so it draws the text and the sprite
        DrawTextureEx(tex, pos, rotation, scale, tint);
    }


    void setPos(Vector2 newPos) { pos = newPos; }
    void setScale(float newScale) { scale = newScale; }
    void setRotation(float newRotation) { rotation = newRotation; }
    void setTint(Color newTint) { tint = newTint; }

    Vector2 getPos() const { return pos; }
    float getScale() const { return scale; }
    float getRotation() const { return rotation; }
    Color getTint() const { return tint; }
    Texture2D getTexture() const { return tex; }

};

#endif
