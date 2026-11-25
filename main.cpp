// Game idea inspired by "Papers, Please" by Lucas Pope 
// Theme: Crawford & Joler, "Calculating Empires", https://calculatingempires.net/
// Font: https://fonts.google.com/
// Music: https://pixabay.com/music/search/detective/
// raylib functions: https://www.raylib.com/cheatsheet/raylib_cheatsheet_v5.5.pdf

// 3D Modules citations:
// https://sketchfab.com/3d-models/sonya-9486429b08004768be0811402d439245 by Sonya
// https://sketchfab.com/3d-models/supportitseez3dcom-590385a31bb541fa99baa1e07d783ac7 by itseez3d
// https://sketchfab.com/3d-models/arnaud-from-mapado-056c52620bc94d369bdae94be26d8e3e by 3D Selfie
// https://sketchfab.com/3d-models/julien-d-from-mapado-408242104978456286f53174b221452d by 3D Selfie
// https://sketchfab.com/3d-models/537-0d37f50871f74c6f939e55be512913c2 by nbarnea1
// https://sketchfab.com/3d-models/johnny-depp-52fa8f620415487fa2d11a2d9a72de46 by tipatat

#include <iostream>
#include <string>
#include <ctime>
#include "raylib.h"
#include "Traveler.h"
using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

// Named numbers for the screens and for the player's actions
const int MENU = 0;
const int PLAYING = 1;
const int PAUSED = 2;

const int ACTION_APPROVE = 0;
const int ACTION_DENY = 1;
const int ACTION_PRISON = 2;

// STEP 3: color palette (change the look of the whole game here)
const Color COLOR_MENU_BG     = {18, 36, 54, 255};
const Color COLOR_BAR         = {10, 22, 34, 255};
const Color COLOR_WALL        = {58, 78, 92, 255};
const Color COLOR_COUNTER     = {92, 64, 44, 255};
const Color COLOR_COUNTER_TOP = {120, 86, 60, 255};
const Color COLOR_PAPER       = {245, 238, 220, 255};
const Color COLOR_INK         = {35, 35, 45, 255};
const Color COLOR_GOLD        = {240, 200, 90, 255};
const Color COLOR_BUTTON      = {45, 85, 120, 255};
const Color COLOR_GREEN       = {46, 160, 90, 255};
const Color COLOR_ORANGE      = {225, 140, 40, 255};
const Color COLOR_RED         = {190, 45, 50, 255};

// STEP Wanted list (and whether each wanted person is a woman)
const int WANTED_COUNT = 4;
const string wantedNames[WANTED_COUNT] = {"Viktor Kane", "Mira Volkov", "Dario Pesce", "Elena Marsh"};
const bool wantedIsWoman[WANTED_COUNT] = {false, true, false, true};

Font gameFont;   // The font used by the whole game

// ---------------------------------------------------------------
// STEP 2: text helpers that use our font
// ---------------------------------------------------------------
void drawText(const char* text, int x, int y, int size, Color color) {
    DrawTextEx(gameFont, text, Vector2{(float)x, (float)y}, (float)size, 1, color);
}

int textWidth(const char* text, int size) {
    return (int)MeasureTextEx(gameFont, text, (float)size, 1).x;
}

// draws text in the middle of the screen
void drawCenteredText(const char* text, int y, int size, Color color) {
    int x = (SCREEN_WIDTH - textWidth(text, size)) / 2;
    drawText(text, x, y, size, color);
}

// draws a button, makes it lighter when the mouse is over it,
// and returns true if it was clicked
bool drawButton(Rectangle box, const char* text, Color baseColor) {
    bool hovering = CheckCollisionPointRec(GetMousePosition(), box);

    Color boxColor = baseColor;
    if (hovering) {
        boxColor = ColorBrightness(baseColor, 0.3f);   // lighter on hover
    }

    // white border, then the colored button on top
    DrawRectangleRounded(Rectangle{box.x - 3, box.y - 3, box.width + 6, box.height + 6}, 0.25f, 8, WHITE);
    DrawRectangleRounded(box, 0.25f, 8, boxColor);

    int w = textWidth(text, 30);
    int textX = (int)(box.x + (box.width - w) / 2);
    int textY = (int)(box.y + (box.height - 30) / 2);
    drawText(text, textX, textY, 30, WHITE);

    if (hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
    }
    return false;
}

// ---------------------------------------------------------------
// STEP 4: drawing the traveler
// These helpers draw shapes relative to the middle of the head (cx, cy).
// s is the size: 1.0 = full size, 0.62 = small (used for the ID photo).
// ---------------------------------------------------------------
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
    DrawRing(Vector2{cx + dx * s, cy + dy * s}, inner * s, outer * s, 0, 360, 24, c);
}

void drawHead(int cx, int cy, float s, bool woman, Color skin, Color hair, bool glasses, bool beard) {
    Color shadow = ColorBrightness(skin, -0.12f);   // slightly darker skin

    if (woman) {
        sEllipse(cx, cy, s, 0, 35, 68, 100, hair);     // long hair behind
    }
    sRect(cx, cy, s, -20, 40, 40, 60, shadow);         // neck
    sCircle(cx, cy, s, -56, 5, 11, skin);              // ears
    sCircle(cx, cy, s, 56, 5, 11, skin);
    sEllipse(cx, cy, s, 0, -18, 62, 62, hair);         // hair behind the top of the face
    sEllipse(cx, cy, s, 0, 0, 55, 62, skin);           // face
    sEllipse(cx, cy, s, 0, -48, 50, 20, hair);         // fringe

    if (beard && !woman) {
        sEllipse(cx, cy, s, 0, 38, 40, 26, hair);      // beard
    }

    sCircle(cx, cy, s, 0, 14, 7, shadow);              // nose

    sEllipse(cx, cy, s, -22, -5, 10, 7, WHITE);        // eyes (white part)
    sEllipse(cx, cy, s, 22, -5, 10, 7, WHITE);
    sCircle(cx, cy, s, -22, -5, 4, BLACK);             // pupils
    sCircle(cx, cy, s, 22, -5, 4, BLACK);

    sRect(cx, cy, s, -34, -22, 24, 5, hair);           // eyebrows
    sRect(cx, cy, s, 10, -22, 24, 5, hair);

    if (glasses) {
        sRing(cx, cy, s, -22, -5, 11, 14, BLACK);      // glasses
        sRing(cx, cy, s, 22, -5, 11, 14, BLACK);
        sRect(cx, cy, s, -9, -8, 18, 3, BLACK);
    }

    sRect(cx, cy, s, -14, 34, 28, 5, Color{150, 55, 55, 255});   // mouth
}

// the whole person: head first, then the body on top (so shoulders cover the long hair)
void drawPerson(Traveler& t, int x) {
    drawHead(x, 255, 1.0f, t.isWoman(), t.getSkinColor(), t.getHairColor(), t.hasGlasses(), t.hasBeard());

    DrawRectangleRounded(Rectangle{(float)(x - 95), 340, 190, 330}, 0.3f, 8, t.getShirtColor());   // body
    DrawTriangle(Vector2{(float)(x - 32), 340}, Vector2{(float)x, 392}, Vector2{(float)(x + 32), 340},
                 ColorBrightness(t.getSkinColor(), -0.1f));                                            // V-neck
}

// ---------------------------------------------------------------
// Making a random traveler (STEPS 5, 6 and 7 are used here)
// ---------------------------------------------------------------
Traveler makeRandomTraveler() {
    string menNames[6] = {"Amir Khan", "Daniel Moore", "Omar Hassan", "Luca Rossi", "Kenji Sato", "Carlos Diaz"};
    string womenNames[6] = {"Lena Fischer", "Maya Patel", "Sara Novak", "Yuki Tanaka", "Nora Silva", "Aisha Bello"};
    string countries[6] = {"Veldonia", "Marovia", "Kestria", "Solenna", "Brivland", "Tarnova"};

    Color shirts[6] = {{200, 70, 60, 255}, {60, 110, 170, 255}, {70, 140, 90, 255},
                       {220, 160, 50, 255}, {130, 90, 160, 255}, {90, 90, 100, 255}};
    Color hairs[5] = {{30, 25, 25, 255}, {110, 70, 40, 255}, {225, 185, 90, 255},
                      {150, 50, 40, 255}, {190, 190, 190, 255}};
    Color skins[4] = {{255, 224, 189, 255}, {234, 192, 134, 255}, {198, 134, 66, 255}, {141, 85, 36, 255}};

    // STEP 7: about 15% of travelers are on the wanted list
    bool woman;
    string name;
    bool wanted = false;

    if (GetRandomValue(0, 99) < 15) {
        int pick = GetRandomValue(0, WANTED_COUNT - 1);
        name = wantedNames[pick];
        woman = wantedIsWoman[pick];
        wanted = true;
    } else {
        woman = (GetRandomValue(0, 1) == 1);
        if (woman) {
            name = womenNames[GetRandomValue(0, 5)];
        } else {
            name = menNames[GetRandomValue(0, 5)];
        }
    }

    // STEP 6: about 15% are under 18
    int age;
    if (GetRandomValue(0, 99) < 15) {
        age = GetRandomValue(12, 17);
    } else {
        age = GetRandomValue(18, 70);
    }

    string country = countries[GetRandomValue(0, 5)];
    string passport = to_string(GetRandomValue(10000, 99999));   // 5 digits

    // visa: 70% valid, 20% expired, 10% none
    string visa;
    int roll = GetRandomValue(0, 9);
    if (roll < 7) {
        visa = "valid";
    } else if (roll < 9) {
        visa = "expired";
    } else {
        visa = "none";
    }

    Traveler t(name, age, country, passport, visa);

    // how the traveler looks
    int hairIndex = GetRandomValue(0, 4);
    bool glasses = (GetRandomValue(0, 2) == 0);            // 1 in 3 wear glasses
    bool beard = (!woman && GetRandomValue(0, 2) == 0);    // 1 in 3 men have a beard
    t.setLook(woman, shirts[GetRandomValue(0, 5)], hairs[hairIndex], skins[GetRandomValue(0, 3)], glasses, beard);

    // STEP 5: the ID photo starts as a copy of the real face...
    bool photoWoman = woman;
    Color photoHair = hairs[hairIndex];
    bool photoGlasses = glasses;

    // ...but 25% of the time we change ONE thing, so the photo is wrong
    if (GetRandomValue(0, 99) < 25) {
        int change = GetRandomValue(0, 2);
        if (change == 0) {
            int other = (hairIndex + GetRandomValue(1, 4)) % 5;   // always a different hair color
            photoHair = hairs[other];
        } else if (change == 1) {
            photoGlasses = !glasses;
        } else {
            photoWoman = !woman;
        }
    }
    t.setPhoto(photoWoman, photoHair, photoGlasses);

    t.setWanted(wanted);
    return t;
}

// STEP 7: what is the right decision for this traveler?
int correctAction(Traveler& t) {
    if (t.isWanted()) {
        return ACTION_PRISON;     // wanted people always go to prison
    }
    if (t.canEnter()) {
        return ACTION_APPROVE;    // age, visa and photo are all fine
    }
    return ACTION_DENY;
}

int main() {
    // Terminal input
    string officer;
    cout << "Officer name? ";
    getline(cin, officer);

    SetRandomSeed((unsigned int)time(NULL));   // different travelers every run

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Group 4 Game");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);                      // ESC pauses the game instead of closing it

    // STEP 2: load the font (font.ttf must be in the project folder)
    gameFont = GetFontDefault();
    bool fontLoaded = false;
    if (FileExists("font.ttf")) {
        gameFont = LoadFontEx("font.ttf", 64, NULL, 0);
        SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);   // smooth text
        fontLoaded = true;
    }

    // music (music.mp3 must be in the project folder)
    InitAudioDevice();
    Music music = LoadMusicStream("music.mp3");
    bool musicLoaded = IsMusicValid(music);
    if (musicLoaded) {
        SetMusicVolume(music, 0.5f);
        PlayMusicStream(music);
    }

    int state = MENU;
    bool running = true;

    Traveler current = makeRandomTraveler();
    int score = 0;
    int mistakes = 0;
    string feedback = "";

    // positions for the transition
    int personX = -150;            // the person walks in from the left
    int cardX = SCREEN_WIDTH;      // the passport slides in from the right
    float fadeAlpha = 0.0f;        // 1.0 = fully black, 0.0 = see-through

    while (running && !WindowShouldClose()) {

        if (musicLoaded) {
            UpdateMusicStream(music);
        }

        // ---------- UPDATE ----------
        if (state == PLAYING) {
            if (personX < 420) {
                personX = personX + 8;
            } else if (cardX > 640) {
                cardX = cardX - 15;
            }

            if (fadeAlpha > 0.0f) {
                fadeAlpha = fadeAlpha - 0.02f;
            }

            if (IsKeyPressed(KEY_ESCAPE)) {
                state = PAUSED;
            }
        } else if (state == PAUSED) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                state = PLAYING;
            }
        }

        // ---------- DRAW ----------
        BeginDrawing();

        if (state == MENU) {
            ClearBackground(COLOR_MENU_BG);

            DrawRectangle(0, 0, SCREEN_WIDTH, 100, COLOR_BAR);
            DrawRectangle(0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100, COLOR_BAR);

            drawCenteredText("Welcome to Paper Please!", 204, 64, BLACK);   // shadow
            drawCenteredText("Welcome to Paper Please!", 200, 64, COLOR_GOLD);
            drawCenteredText("Check the passport, the photo and the wanted list.", 290, 28, COLOR_PAPER);

            // decorations: two "stamps"
            DrawRectangleLinesEx(Rectangle{120, 340, 240, 80}, 5, COLOR_GREEN);
            drawText("APPROVED", 150, 362, 36, COLOR_GREEN);
            DrawRectangleLinesEx(Rectangle{840, 340, 240, 80}, 5, COLOR_RED);
            drawText("DENIED", 900, 362, 36, COLOR_RED);

            if (drawButton(Rectangle{450, 450, 300, 80}, "PLAY", COLOR_BUTTON)) {
                state = PLAYING;
                score = 0;
                mistakes = 0;
                feedback = "";
                current = makeRandomTraveler();
                personX = -150;
                cardX = SCREEN_WIDTH;
                fadeAlpha = 1.0f;
            }
            if (drawButton(Rectangle{450, 560, 300, 80}, "QUIT", COLOR_BUTTON)) {
                running = false;
            }
        }
        else if (state == PLAYING) {
            ClearBackground(COLOR_WALL);

            // top bar
            DrawRectangle(0, 0, SCREEN_WIDTH, 90, COLOR_BAR);
            drawText(TextFormat("Score: %d    Mistakes: %d", score, mistakes), 30, 28, 32, WHITE);
            drawText(feedback.c_str(), 30, 100, 26, COLOR_GOLD);

            // the person
            drawPerson(current, personX);

            // counter in front of the person
            DrawRectangle(0, 600, SCREEN_WIDTH, 200, COLOR_COUNTER);
            DrawRectangle(0, 600, SCREEN_WIDTH, 12, COLOR_COUNTER_TOP);

            // STEP 7: wanted board
            DrawRectangle(20, 150, 250, 250, COLOR_PAPER);
            DrawRectangle(20, 150, 250, 45, COLOR_RED);
            drawText("WANTED", 20 + (250 - textWidth("WANTED", 28)) / 2, 158, 28, WHITE);
            for (int i = 0; i < WANTED_COUNT; i++) {
                drawText(wantedNames[i].c_str(), 40, 215 + i * 40, 24, COLOR_INK);
            }

            // rules board
            DrawRectangle(20, 420, 250, 160, COLOR_PAPER);
            DrawRectangle(20, 420, 250, 40, COLOR_BUTTON);
            drawText("RULES", 20 + (250 - textWidth("RULES", 26)) / 2, 427, 26, WHITE);
            drawText("Age 18 or older", 35, 470, 22, COLOR_INK);
            drawText("Visa must be valid", 35, 498, 22, COLOR_INK);
            drawText("Photo = traveler", 35, 526, 22, COLOR_INK);
            drawText("Wanted = PRISON", 35, 554, 22, COLOR_RED);

            // passport
            DrawRectangle(cardX, 110, 520, 420, COLOR_PAPER);
            DrawRectangle(cardX, 110, 520, 60, COLOR_BUTTON);
            drawText("PASSPORT", cardX + 20, 124, 34, WHITE);

            // STEP 5: the ID photo (drawn smaller with the photo's face)
            DrawRectangle(cardX + 20, 200, 150, 190, Color{190, 205, 215, 255});
            drawHead(cardX + 95, 270, 0.62f, current.isPhotoWoman(), current.getSkinColor(),
                     current.getPhotoHairColor(), current.photoHasGlasses(), current.hasBeard());
            DrawRectangle(cardX + 30, 323, 130, 67, Color{110, 115, 125, 255});    // shoulders in the photo
            DrawRectangleLines(cardX + 20, 200, 150, 190, COLOR_INK);

            // passport information
            drawText(TextFormat("Name: %s", current.getName().c_str()), cardX + 190, 200, 26, COLOR_INK);
            drawText(TextFormat("Age: %d", current.getAge()), cardX + 190, 245, 26, COLOR_INK);
            drawText(TextFormat("Nationality: %s", current.getNationality().c_str()), cardX + 190, 290, 26, COLOR_INK);
            drawText(TextFormat("Passport No: %s", current.getPassportNumber().c_str()), cardX + 190, 335, 26, COLOR_INK);
            drawText(TextFormat("Visa: %s", current.getVisaStatus().c_str()), cardX + 190, 380, 26, COLOR_INK);

            // buttons (they only work after the passport has arrived)
            bool ready = (cardX <= 640);
            int answer = -1;     // -1 = no decision yet

            if (drawButton(Rectangle{150, 660, 260, 80}, "APPROVE", COLOR_GREEN) && ready) {
                answer = ACTION_APPROVE;
            }
            if (drawButton(Rectangle{470, 660, 260, 80}, "DENY", COLOR_ORANGE) && ready) {
                answer = ACTION_DENY;
            }
            if (drawButton(Rectangle{790, 660, 260, 80}, "PRISON", COLOR_RED) && ready) {
                answer = ACTION_PRISON;
            }
            if (drawButton(Rectangle{1040, 20, 140, 50}, "PAUSE", COLOR_BUTTON)) {
                state = PAUSED;
            }

            if (answer != -1) {
                int right = correctAction(current);

                if (answer == right) {
                    score++;
                    if (right == ACTION_PRISON) {
                        feedback = "Correct! A wanted traveler was arrested.";
                    } else {
                        feedback = "Correct decision.";
                    }
                } else {
                    mistakes++;
                    if (right == ACTION_PRISON) {
                        feedback = "Wrong! That traveler was wanted.";
                    } else if (answer == ACTION_PRISON) {
                        feedback = "Wrong! That traveler was not wanted.";
                    } else if (right == ACTION_APPROVE) {
                        feedback = "Wrong! All documents were fine.";
                    } else {
                        feedback = "Wrong! The documents were not valid.";
                    }
                }

                // next traveler comes in
                current = makeRandomTraveler();
                personX = -150;
                cardX = SCREEN_WIDTH;
            }

            // fade-in effect (drawn last, on top of everything)
            if (fadeAlpha > 0.0f) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, fadeAlpha));
            }
        }
        else if (state == PAUSED) {
            ClearBackground(COLOR_MENU_BG);
            drawCenteredText("PAUSED", 220, 70, COLOR_GOLD);

            if (drawButton(Rectangle{450, 350, 300, 80}, "RESUME", COLOR_BUTTON)) {
                state = PLAYING;
            }
            if (drawButton(Rectangle{450, 460, 300, 80}, "QUIT", COLOR_BUTTON)) {
                running = false;
            }
        }

        EndDrawing();
    }

    if (musicLoaded) {
        UnloadMusicStream(music);
    }
    CloseAudioDevice();
    if (fontLoaded) {
        UnloadFont(gameFont);
    }
    CloseWindow();

    // Terminal output
    cout << "Shift report for " << officer << endl;
    cout << "Correct decisions: " << score << endl;
    cout << "Mistakes: " << mistakes << endl;

    return 0;
}
