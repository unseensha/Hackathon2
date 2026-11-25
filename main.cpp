#include "raylib.h"
#include "Scene.h"
#include "Object.h"
#include "Sprite.h"
#include "Text.h"
#include "Button.h"
#include "Traveler.h"
#include <vector>
#include <string>
#include <iostream>

enum GameState { START, GAME, FINAL };//just to make more readable i used an enum ts just an integer  

// colors copied from Run.cpp which was the code we had in the presentation
const Color COLOR_PAPER = { 245, 238, 220, 255 };
const Color COLOR_INK = { 35, 35, 45, 255 };
const Color COLOR_BUTTON = { 45, 85, 120, 255 };

// ADDED: background colors copied from Run.cpp
const Color COLOR_WALL = { 58, 78, 92, 255 };
const Color COLOR_BAR = { 10, 22, 34, 255 };
const Color COLOR_COUNTER = { 92, 64, 44, 255 };
const Color COLOR_COUNTER_TOP = { 120, 86, 60, 255 };


// helpers from Run.cpp which was the code we had in the presentation

void drawText(const char* text, int x, int y, int size, Color color) {
    DrawTextEx(GetFontDefault(), text, Vector2{ (float)x, (float)y }, (float)size, 1, color);
}

void sCircle(int cx, int cy, float s, float dx, float dy, float r, Color c) {
    DrawCircle(cx + (int)(dx * s), cy + (int)(dy * s), r * s, c);
}
void sEllipse(int cx, int cy, float s, float dx, float dy, float rx, float ry, Color c) {
    DrawEllipse(cx + (int)(dx * s), cy + (int)(dy * s), rx * s, ry * s, c);
}
void sRect(int cx, int cy, float s, float dx, float dy, float w, float h, Color c) {
    DrawRectangle(cx + (int)(dx * s), cy + (int)(dy * s), (int)(w * s), (int)(h * s), c);
}
void sRing(int cx, int cy, float s, float dx, float dy, float inner, float outer, Color c) {
    DrawRing(Vector2{ cx + dx * s, cy + dy * s }, inner * s, outer * s, 0, 360, 24, c);
}

void drawHead(int cx, int cy, float s, bool woman, Color skin, Color hair, bool glasses, bool beard) {
    Color shadow = ColorBrightness(skin, -0.12f);

    if (woman) sEllipse(cx, cy, s, 0, 35, 68, 100, hair);
    sRect(cx, cy, s, -20, 40, 40, 60, shadow);
    sCircle(cx, cy, s, -56, 5, 11, skin);
    sCircle(cx, cy, s, 56, 5, 11, skin);
    sEllipse(cx, cy, s, 0, -18, 62, 62, hair);
    sEllipse(cx, cy, s, 0, 0, 55, 62, skin);
    sEllipse(cx, cy, s, 0, -48, 50, 20, hair);
    if (beard && !woman) sEllipse(cx, cy, s, 0, 38, 40, 26, hair);
    sCircle(cx, cy, s, 0, 14, 7, shadow);
    sEllipse(cx, cy, s, -22, -5, 10, 7, WHITE);
    sEllipse(cx, cy, s, 22, -5, 10, 7, WHITE);
    sCircle(cx, cy, s, -22, -5, 4, BLACK);
    sCircle(cx, cy, s, 22, -5, 4, BLACK);
    sRect(cx, cy, s, -34, -22, 24, 5, hair);
    sRect(cx, cy, s, 10, -22, 24, 5, hair);
    if (glasses) {
        sRing(cx, cy, s, -22, -5, 11, 14, BLACK);
        sRing(cx, cy, s, 22, -5, 11, 14, BLACK);
        sRect(cx, cy, s, -9, -8, 18, 3, BLACK);
    }
    sRect(cx, cy, s, -14, 34, 28, 5, Color{ 150, 55, 55, 255 });
}



// plain rectangle texture for buttons 
Texture2D makeColorTexture(int w, int h, Color c) {
    Image img = GenImageColor(w, h, c);
    Texture2D t = LoadTextureFromImage(img);
    UnloadImage(img);
    return t;
}

// turns the image drawn in Run.cpp into a texture so I can use it in sprite
//based on this example https://www.raylib.com/examples/textures/loader.html?name=textures_mouse_painting and some ai searched functions
Texture2D makePassportTexture(Traveler& t) {
    RenderTexture2D rt = LoadRenderTexture(520, 420);

    BeginTextureMode(rt);
    ClearBackground(COLOR_PAPER);
    DrawRectangle(0, 0, 520, 60, COLOR_BUTTON);
    drawText("PASSPORT", 20, 14, 34, WHITE);


    DrawRectangle(20, 90, 150, 190, Color{ 190, 205, 215, 255 });
    drawHead(95, 160, 0.62f, t.isPhotoWoman(), t.getSkinColor(),
        t.getPhotoHairColor(), t.photoHasGlasses(), t.hasBeard());
    DrawRectangle(30, 213, 130, 67, Color{ 110, 115, 125, 255 });
    DrawRectangleLines(20, 90, 150, 190, COLOR_INK);


    drawText(TextFormat("Name: %s", t.getName().c_str()), 190, 90, 26, COLOR_INK);
    drawText(TextFormat("Age: %d", t.getAge()), 190, 135, 26, COLOR_INK);
    drawText(TextFormat("Nationality: %s", t.getNationality().c_str()), 190, 180, 26, COLOR_INK);
    drawText(TextFormat("Passport No: %s", t.getPassportNumber().c_str()), 190, 225, 26, COLOR_INK);
    drawText(TextFormat("Visa: %s", t.getVisaStatus().c_str()), 190, 270, 26, COLOR_INK);
    EndTextureMode();


    Image img = LoadImageFromTexture(rt.texture);
    ImageFlipVertical(&img);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    UnloadRenderTexture(rt);
    return tex;
}

Sprite makePassport(Traveler t, Vector2 pos) {
    return Sprite(makePassportTexture(t), pos, 0.8f);
}

// quick way to make a traveler whose photo matches their real look
Traveler makeTraveler(std::string name, int age, std::string nationality, std::string passportNo,
    std::string visa, bool woman, Color hair, Color skin, bool glasses, bool beard) {
    Traveler t(name, age, nationality, passportNo, visa);
    t.setLook(woman, WHITE, hair, skin, glasses, beard);
    t.setPhoto(woman, hair, glasses);
    return t;
}


// used to place the 3D model on a specific part of the screen so we can control how muhc of the screen it takes up
Vector3 screenToWorld(float px, float py, const Camera3D& cam) {
    Ray ray = GetScreenToWorldRay(Vector2{ px, py }, cam);   // ray from the camera through that pixel
    float t = -ray.position.z / ray.direction.z;             // how far along the ray it hits the z = 0 plane
    return { ray.position.x + ray.direction.x * t,
             ray.position.y + ray.direction.y * t,
             0.0f };
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Scene Render Test");
    SetTargetFPS(60);

    Camera3D cam = { 0 };
    cam.position = { 1.0f, 1.0f, 40.0f };
    cam.target = { 0.0f, 0.5f, 0.0f };
    cam.up = { 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    { // inner scope so everything is destroyed before close window runs cleans stuff up

        const float MODEL_SCALE = 16.0f;  //use these to control the current model and add more for each new model
        const float MODEL_SCREEN_X = 320;  
        const float MODEL_SCREEN_Y = 400;  

        //all the 3d models go here
        Object people[] = {
            { "assets\\johnny-depp\\model.obj", "assets\\johnny-depp\\model_texture.jpg",{ 0.0f, 0.0f, 0.0f }, MODEL_SCALE }
            
        };

        // the corresponding passport for each model!!!! 
        //THEY NEED TO BE THE SAME SIZE
        Vector2 passportPos = { 830.0f, 100.0f };
        Sprite passports[] = {
            makePassport(makeTraveler("Jon Dee", 45, "Veldonia", "48213", "valid",
                                      false, BROWN, Color{ 234, 192, 134, 255 }, false, true), passportPos)
        };

        const int personCount = sizeof(people) / sizeof(people[0]);
        static_assert(sizeof(people) / sizeof(people[0]) == sizeof(passports) / sizeof(passports[0]),//an assert to check for that
            "people and passports NNEEEEEEDDDD TO BE SAME SIZE");

        
        for (int i = 0; i < personCount; i++) {
            people[i].updatePos(screenToWorld(MODEL_SCREEN_X, MODEL_SCREEN_Y, cam));   // NEW: uses the settings above
        }

        // all the buttons for the game
        Text startText = { WHITE,{ 0, 0 }, "START", GetFontDefault(), 40.0f, 2.0f };
        Text endText = { WHITE,{ 0, 0 }, "END",   GetFontDefault(), 40.0f, 2.0f };
        Text allowText = { WHITE,{ 0, 0 }, "ALLOW", GetFontDefault(), 40.0f, 2.0f };
        Text prisonText = { WHITE,{ 0, 0 }, "PRISON", GetFontDefault(), 40.0f, 2.0f };
        Text quitText = { WHITE,{ 0, 0 }, "QUIT",  GetFontDefault(), 30.0f, 2.0f };

        Button startButton(makeColorTexture(300, 100, DARKGREEN), { 490.0f, 250.0f }, 1.0f, 0.0f, WHITE, startText);
        Button endButton(makeColorTexture(300, 100, MAROON), { 490.0f, 400.0f }, 1.0f, 0.0f, WHITE, endText);

        Button allowButton(makeColorTexture(280, 80, DARKGREEN), { 340.0f, 600.0f }, 1.0f, 0.0f, WHITE, allowText);
        Button prisonButton(makeColorTexture(280, 80, MAROON), { 660.0f, 600.0f }, 1.0f, 0.0f, WHITE, prisonText);
        Button quitButton(makeColorTexture(140, 50, MAROON), { 1120.0f, 20.0f }, 1.0f, 0.0f, WHITE, quitText);

        
        Sprite topBar(makeColorTexture(screenWidth, 90, COLOR_BAR), { 0.0f, 0.0f });             // dark bar at the top
        Sprite counter(makeColorTexture(screenWidth, 120, COLOR_COUNTER), { 0.0f, 600.0f });      // counter in front of the person
        Sprite counterTop(makeColorTexture(screenWidth, 12, COLOR_COUNTER_TOP), { 0.0f, 600.0f });// lighter edge on top of the counter

        // starst screen stuff
        Scene startScreen(cam, BLACK);
        startScreen.addSprite(&startButton);
        startScreen.addSprite(&endButton);

        // final scene shown after every person has been rated, blank for now (add stuff to it later)
        Scene finalScreen(cam, BLACK);

        //each person having a passport check is their own game scene
        std::vector<Scene> gameScenes;
        for (int i = 0; i < personCount; i++) {
            Scene s(cam, COLOR_WALL);          
            s.addObject(&people[i]);           
            s.addSprite(&topBar);             
            s.addSprite(&counter);             
            s.addSprite(&counterTop);         
            s.addSprite(&passports[i]);
            s.addSprite(&allowButton);
            s.addSprite(&prisonButton);
            s.addSprite(&quitButton);
            gameScenes.push_back(s);
        }

        GameState state = START;//we started thge ganeamae 
        bool quit = false;
        int current = 0;              // which person is at the checker corresponds to the indes in the objects list and corresponding passport
        std::string feedback = "";

        while (!WindowShouldClose() && !quit) {

            Vector2 mouse = GetMousePosition();
            bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if (state == START) {
                if (startButton.clicked(mouse, click)) state = GAME;
                if (endButton.clicked(mouse, click))   quit = true;
            }
            else if (state == GAME) {   
                

                bool decided = false;  

                if (allowButton.clicked(mouse, click)) {
                    feedback = "Allowed through";
                    decided = true;
                }
                else if (prisonButton.clicked(mouse, click)) {
                    feedback = "Sent to prison";
                    decided = true;
                }
                else if (quitButton.clicked(mouse, click)) {
                    quit = true;
                }

                
                if (decided) {
                    if (current + 1 >= personCount) {
                        state = FINAL;
                    }
                    else {
                        current++;
                    }
                }
            }
            

            // draw the stuff
            BeginDrawing();
            if (state == START) {
                startScreen.draw();
            }
            else if (state == GAME) {   
                gameScenes[current].draw();
                drawText(feedback.c_str(), 20, 20, 30, GOLD);
            }
            else {                     
                finalScreen.draw();
            }
            DrawFPS(screenWidth - 100, screenHeight - 30);
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
