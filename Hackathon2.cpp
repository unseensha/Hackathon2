#include "raylib.h"
#include "Scene.h"
#include "Object.h"
#include "Sprite.h"
#include "Text.h"
#include<iostream>
int main() {
    //ChangeDirectory(GetApplicationDirectory());//got this from the ai overview  needed an absolute path before(it didnt work)
 //  std::cout<< GetWorkingDirectory();
    //the paths are broken just fix later

    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Scene Render Test");
    SetTargetFPS(60);

    // make da camera
    Camera3D cam = { 0 };
    cam.position = { 40.0f, 40.0f, 40.0f };
    cam.target = { 0.0f, 0.5f, 0.0f };
    cam.up = { 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    // make scene
    Scene myScene(cam, BLACK);

    //  source for models https://github.com/rbarril75/Scratched-Blue-Teapot/blob/master/blue.jpg https://github.com/raysan5/raylib/blob/master/examples/models/resources/models/obj/castle.obj
    Object modelObj("C:\\Users\\Lev\\Desktop\\Hackathon2\\Hackathon2\\assets\\model.obj", "C:\\Users\\Lev\\Desktop\\Hackathon2\\Hackathon2\\assets\\texture.png", { 0.0f, 0.0f, 0.0f }, 1.0f);
    myScene.addObject(modelObj);

    // load stuff again 2d stuff this time
    Texture2D spriteTex = LoadTexture("assets\\sprite.png");
    Sprite hudSprite(spriteTex, { 20.0f, 80.0f }, 1.0f, 0.0f, WHITE);
    myScene.addSprite(hudSprite);

    // make text and add it no laoding for text maybe laoding for font
    Text titleText = {
        DARKGRAY,                           // color
        { 20.0f, 20.0f },                   // pos
        "3D Model + 2D Sprite + Text Test", // content
        GetFontDefault(),                   // font
        24.0f,                              // fontSize
        2.0f                                // spacing
    };
    myScene.addText(titleText);

    //gaem loop
    while (!WindowShouldClose()) {
        UpdateCamera(&cam, CAMERA_ORBITAL);
        myScene.setCamera(cam);

        BeginDrawing();
        myScene.draw();
        DrawFPS(screenWidth - 100, 20);
        EndDrawing();
    }

    // cleanup pointers so no mem leak
    UnloadTexture(spriteTex);
    CloseWindow();
    return 0;
}
