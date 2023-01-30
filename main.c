#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"

#include <string.h>
#include <time.h>

typedef struct {
    Vector2 center;
    float innerRadius;
    float outerRadius;
    float startAngle;
    float endAngle;
    int segments;
    Color color;
} Loading;

typedef struct {
    bool isEaten;
    int eatenTime;
    Texture2D texture;
    int textureWidth;
    int textureHeight;
    int x;
    int y;
    Color tint;
    float xVelocity;
    float yVelocity;
} MovingTexture;

typedef struct {
    char text[1000];
    int x;
    int y;
    int fontSize;
    Color color;
} Phrase;

typedef struct {
    Rectangle rec;
    char text[1000];
} Button;

typedef struct {
    Texture2D texture;
    int textureWidth;
    int textureHeight;
    int x;
    int y;
    Color tint;
    float xVelocity;
    float yVelocity;
} Android;

typedef enum {
    MENU,
    SETTINGS,
    QUIT,
    GAME,
    LOST,
    WON
} GameScreen;

int main() {
    int i;

    InitWindow(1000, 1000, "Apple eater");
    InitAudioDevice();

    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor), monitorHeight = GetMonitorHeight(monitor);

    SetWindowSize(monitorWidth, monitorHeight);
    ToggleFullscreen();

    int APPLE_LENGTH = GetRandomValue(50, 250);
    int HAMBURGERS_LENGTH = GetRandomValue(5, 10);

    GuiLoadStyle("mystyle.rgs");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 128);

    GameScreen currentScreen = MENU;

    Phrase title;

    strcpy(title.text, "APPLE EATER");
    title.x = 25;
    title.y = 50;
    title.fontSize = 256;
    title.color = RED;

    Button start;

    start.rec = (Rectangle) {
        monitorWidth / 2 - 375,
        300,
        750,
        200
    };
    strcpy(start.text, "Inizia");

    Button settings;

    settings.rec = (Rectangle) {
        monitorWidth / 2 - 375,
        525,
        750,
        200
    };
    strcpy(settings.text, "Impostazioni");

    Button quit;

    quit.rec = (Rectangle) {
        monitorWidth / 2 - 375,
        750,
        750,
        200
    };
    strcpy(quit.text, "Esci");

    /*bool dropdownActive = false;
    int nonSoCosa = 0;*/
    float audioVolume = 1.0;
    bool buttonFocus[3] = {
        true,
        false,
        false
    };
    int buttonIndex = 0;

    Phrase ora;

    strcpy(ora.text, "Ora attuale:");
    ora.x = 10;
    ora.y = 10;
    ora.fontSize = 32;
    ora.color = BLACK;

    time_t currentTime;
    struct tm *timeInfo;

    Phrase score;

    strcpy(score.text, "Punteggio:");
    score.x = GetScreenWidth() - 320;
    score.y = 10;
    score.fontSize = 32;
    score.color = BLACK;

    int punteggio = 0;

    MovingTexture apples[250];
    Rectangle movingTextureRectangle[250];
    Rectangle textureRectangle[250];

    int appleFullTexture = 0,
    appleEatenTexture = 112;

    int appleTextureWidth = 112, appleTextureHeight = 120;

    Sound eating1 = LoadSound("resources/audio/eating1.mp3");
    Sound eating2 = LoadSound("resources/audio/eating2.mp3");
    Sound eating3 = LoadSound("resources/audio/eating3.mp3");

    for (i = 0; i < APPLE_LENGTH; i++) {
        int randomXVelocity, randomYVelocity;

        do {
            randomXVelocity = GetRandomValue(-500, 500);
        } while (randomXVelocity > -100 && randomXVelocity < 100);

        do {
            randomYVelocity = GetRandomValue(-500, 500);
        } while (randomYVelocity > -100 && randomYVelocity < 100);

        apples[i].isEaten = false;
        apples[i].eatenTime = 0;
        apples[i].texture = LoadTexture("resources/textures/apple.png");
        apples[i].textureWidth = appleTextureWidth;
        apples[i].textureHeight = appleTextureHeight;
        apples[i].x = GetRandomValue(0, monitorWidth - appleTextureWidth);
        apples[i].y = GetRandomValue(0, monitorHeight - appleTextureHeight);
        apples[i].tint = RAYWHITE;
        apples[i].xVelocity = randomXVelocity;
        apples[i].yVelocity = randomYVelocity;

        movingTextureRectangle[i].x = apples[i].x;
        movingTextureRectangle[i].y = apples[i].y;
        movingTextureRectangle[i].width = appleTextureWidth;
        movingTextureRectangle[i].height = appleTextureHeight;

        textureRectangle[i].x = appleFullTexture;
        textureRectangle[i].y = 0;
        textureRectangle[i].width = appleTextureWidth;
        textureRectangle[i].height = appleTextureHeight;
    }

    MovingTexture hamburgers[10];
    Rectangle movingTextureHamburgers[10];

    Sound death = LoadSound("resources/audio/death.mp3");

    for (i = 0; i < HAMBURGERS_LENGTH; i++) {
        int randomX, randomY, randomXVelocity, randomYVelocity;

        do {
            randomX = GetRandomValue(0, monitorWidth - 150);
        } while (randomX > monitorWidth / 2 - 400 && randomX > monitorWidth / 2 + 400);

        do {
            randomY = GetRandomValue(0, monitorHeight - 84);
        } while (randomY > monitorHeight / 2 - 300);

        do {
            randomXVelocity = GetRandomValue(-500, 500);
        } while (randomXVelocity > -100 && randomXVelocity < 100);

        do {
            randomYVelocity = GetRandomValue(-500, 500);
        } while (randomYVelocity > -100 && randomYVelocity < 100);

        hamburgers[i].isEaten = false;
        hamburgers[i].eatenTime = 0;
        hamburgers[i].texture = LoadTexture("resources/textures/hamburger.png");
        hamburgers[i].textureWidth = 112;
        hamburgers[i].textureHeight = 91;
        hamburgers[i].x = randomX;
        hamburgers[i].y = randomY;
        hamburgers[i].tint = RAYWHITE;
        hamburgers[i].xVelocity = randomXVelocity;
        hamburgers[i].yVelocity = randomYVelocity;

        movingTextureHamburgers[i].x = hamburgers[i].x;
        movingTextureHamburgers[i].y = hamburgers[i].y;
        movingTextureHamburgers[i].width = hamburgers[i].textureWidth;
        movingTextureHamburgers[i].height = hamburgers[i].textureHeight;
    }

    Android android;

    android.texture = LoadTexture("resources/textures/android.png");
    android.textureWidth = 161;
    android.textureHeight = 280;
    android.x = monitorWidth / 2 - android.textureWidth;
    android.y = monitorHeight - android.textureHeight;
    android.tint = RAYWHITE;
    android.xVelocity = 600;
    android.yVelocity = 1300;

    bool isJumping = false, hasJumped = false;
    const int maximiumHeightJumpY = 150;
    const int gravityY = 600;

    Rectangle androidRectangleTexture;
    int androidTextureStandingX = 0,
    androidTextureLeftX = android.textureWidth,
    androidTextureRightX = android.textureWidth * 2,
    androidTextureJumpingX = android.textureWidth * 3;

    androidRectangleTexture.x = androidTextureStandingX;
    androidRectangleTexture.y = 0;
    androidRectangleTexture.width = android.textureWidth;
    androidRectangleTexture.height = android.textureHeight;

    Rectangle androidRectangle;

    androidRectangle.x = android.x;
    androidRectangle.y = android.y;
    androidRectangle.width = android.textureWidth;
    androidRectangle.height = android.textureHeight;

    bool reset;

    SetTargetFPS(60);

    while (!WindowShouldClose() && currentScreen != QUIT) {

        switch (currentScreen) {
            case MENU:;
                GuiSetStyle(DEFAULT, TEXT_SIZE, 128);

                if (IsKeyPressed(KEY_ONE))
                    GuiSetState(STATE_FOCUSED);

                if (IsKeyPressed(KEY_TWO))
                    currentScreen = SETTINGS;

                if (IsKeyPressed(KEY_THREE))
                    currentScreen = QUIT;

                if (IsKeyPressed(KEY_DOWN)) {
                    if (buttonIndex < 2) {
                        buttonFocus[buttonIndex] = false;

                        buttonIndex++;

                        buttonFocus[buttonIndex] = true;
                    }
                }

                if (IsKeyPressed(KEY_UP)) {
                    if (buttonIndex > 0) {
                        buttonFocus[buttonIndex] = false;

                        buttonIndex--;

                        buttonFocus[buttonIndex] = true;
                    }
                }

                if (IsKeyPressed(KEY_ENTER)) {
                    switch (buttonIndex) {
                        case 0:
                            currentScreen = GAME;

                            break;

                        case 1:
                            currentScreen = SETTINGS;

                            break;

                        case 2:
                            currentScreen = QUIT;

                            break;
                    }
                }

                punteggio = 0;

                break;

            case GAME:;
                reset = true;

                if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_RIGHT))) {
                    android.x -= android.xVelocity * GetFrameTime();

                    if (android.x + android.textureWidth < 0)
                        android.x = monitorWidth;

                    androidRectangleTexture.x = androidTextureLeftX;
                }
                
                if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_LEFT))) {
                    android.x += android.xVelocity * GetFrameTime();

                    if (android.x > monitorWidth)
                        android.x = 0 - android.textureWidth;

                    androidRectangleTexture.x = androidTextureRightX;
                }

                if (
                    ((!IsKeyDown(KEY_A) && !IsKeyDown(KEY_LEFT)) && (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_RIGHT))) 
                    || (IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) || (IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_RIGHT))
                    || (IsKeyDown(KEY_A) && IsKeyDown(KEY_RIGHT)) || (IsKeyDown(KEY_D) && IsKeyDown(KEY_LEFT))
                ) androidRectangleTexture.x = androidTextureStandingX;

                if (IsKeyPressed(KEY_SPACE)) {
                    if (!isJumping && android.y + android.textureHeight >= monitorHeight) {
                        isJumping = true;
                        android.xVelocity = android.xVelocity / 2;
                    }
                }

                if (isJumping) {
                    android.y -= android.yVelocity * GetFrameTime();

                    androidRectangleTexture.x = androidTextureJumpingX;

                    if (android.y + android.textureHeight < monitorHeight - maximiumHeightJumpY) {
                        isJumping = false;
                        hasJumped = true;
                    }
                }

                if (android.y + android.textureHeight < monitorHeight)
                    android.y += gravityY * GetFrameTime();

                if (android.y + android.textureHeight >= monitorHeight && hasJumped) {
                    android.xVelocity = android.xVelocity * 2;
                    hasJumped = false;
                }

                androidRectangle.x = android.x;
                androidRectangle.y = android.y;

                break;

            case SETTINGS:;
                if (IsKeyPressed(KEY_ENTER))
                    currentScreen = MENU;

                break;

            case LOST:;
                if (IsKeyPressed(KEY_ENTER))
                    currentScreen = MENU;

                if (reset) {
                    APPLE_LENGTH = GetRandomValue(50, 250);
                    HAMBURGERS_LENGTH = GetRandomValue(5, 10);

                    for (i = 0; i < APPLE_LENGTH; i++) {
                        int randomXVelocity, randomYVelocity;

                        do {
                            randomXVelocity = GetRandomValue(-500, 500);
                        } while (randomXVelocity > -100 && randomXVelocity < 100);

                        do {
                            randomYVelocity = GetRandomValue(-500, 500);
                        } while (randomYVelocity > -100 && randomYVelocity < 100);

                        apples[i].isEaten = false;
                        apples[i].eatenTime = 0;
                        apples[i].x = GetRandomValue(0, monitorWidth - appleTextureWidth);
                        apples[i].y = GetRandomValue(0, monitorHeight - appleTextureHeight);
                        apples[i].xVelocity = randomXVelocity;
                        apples[i].yVelocity = randomYVelocity;

                        movingTextureRectangle[i].x = apples[i].x;
                        movingTextureRectangle[i].y = apples[i].y;

                        textureRectangle[i].x = appleFullTexture;
                        textureRectangle[i].y = 0;
                    }

                    for (i = 0; i < HAMBURGERS_LENGTH; i++) {
                        int randomX, randomY, randomXVelocity, randomYVelocity;

                        do {
                            randomX = GetRandomValue(0, monitorWidth - 150);
                        } while (randomX > monitorWidth / 2 - 400 && randomX > monitorWidth / 2 + 400);

                        do {
                            randomY = GetRandomValue(0, monitorHeight - 84);
                        } while (randomY > monitorHeight / 2 - 300);

                        do {
                            randomXVelocity = GetRandomValue(-500, 500);
                        } while (randomXVelocity > -100 && randomXVelocity < 100);

                        do {
                            randomYVelocity = GetRandomValue(-500, 500);
                        } while (randomYVelocity > -100 && randomYVelocity < 100);

                        hamburgers[i].x = randomX;
                        hamburgers[i].y = randomY;
                        hamburgers[i].xVelocity = randomXVelocity;
                        hamburgers[i].yVelocity = randomYVelocity;

                        movingTextureHamburgers[i].x = randomX;
                        movingTextureHamburgers[i].y = randomY;
                    }

                    android.x = monitorWidth / 2 - android.textureWidth;
                    android.y = monitorHeight - android.textureHeight;

                    android.xVelocity = 600;
                    isJumping = false;
                    hasJumped = false;

                    reset = false;
                }

                break;

            case WON:;
                if (IsKeyPressed(KEY_ENTER))
                    currentScreen = MENU;

                if (reset) {
                    APPLE_LENGTH = GetRandomValue(50, 250);
                    HAMBURGERS_LENGTH = GetRandomValue(5, 10);

                    for (i = 0; i < APPLE_LENGTH; i++) {
                        int randomXVelocity, randomYVelocity;

                        do {
                            randomXVelocity = GetRandomValue(-500, 500);
                        } while (randomXVelocity > -100 && randomXVelocity < 100);

                        do {
                            randomYVelocity = GetRandomValue(-500, 500);
                        } while (randomYVelocity > -100 && randomYVelocity < 100);

                        apples[i].isEaten = false;
                        apples[i].eatenTime = 0;
                        apples[i].x = GetRandomValue(0, monitorWidth - appleTextureWidth);
                        apples[i].y = GetRandomValue(0, monitorHeight - appleTextureHeight);
                        apples[i].xVelocity = randomXVelocity;
                        apples[i].yVelocity = randomYVelocity;

                        movingTextureRectangle[i].x = apples[i].x;
                        movingTextureRectangle[i].y = apples[i].y;

                        textureRectangle[i].x = appleFullTexture;
                        textureRectangle[i].y = 0;
                    }

                    for (i = 0; i < HAMBURGERS_LENGTH; i++) {
                        int randomX, randomY, randomXVelocity, randomYVelocity;

                        do {
                            randomX = GetRandomValue(0, monitorWidth - 150);
                        } while (randomX > monitorWidth / 2 - 400 && randomX > monitorWidth / 2 + 400);

                        do {
                            randomY = GetRandomValue(0, monitorHeight - 84);
                        } while (randomY > monitorHeight / 2 - 300);

                        do {
                            randomXVelocity = GetRandomValue(-500, 500);
                        } while (randomXVelocity > -100 && randomXVelocity < 100);

                        do {
                            randomYVelocity = GetRandomValue(-500, 500);
                        } while (randomYVelocity > -100 && randomYVelocity < 100);

                        hamburgers[i].x = randomX;
                        hamburgers[i].y = randomY;
                        hamburgers[i].xVelocity = randomXVelocity;
                        hamburgers[i].yVelocity = randomYVelocity;

                        movingTextureHamburgers[i].x = randomX;
                        movingTextureHamburgers[i].y = randomY;
                    }

                    android.x = monitorWidth / 2 - android.textureWidth;
                    android.y = monitorHeight - android.textureHeight;

                    android.xVelocity = 600;
                    isJumping = false;
                    hasJumped = false;

                    reset = false;
                }

                break;
            
            default:;
                break;
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch (currentScreen) {
                case MENU:;
                    ClearBackground(BLACK);

                    DrawText(title.text, title.x, title.y, title.fontSize, title.color);
                    
                    if (buttonFocus[0])
                        GuiSetState(STATE_FOCUSED);

                    if (GuiButton(start.rec, start.text))
                        currentScreen = GAME;

                    GuiSetState(STATE_NORMAL);

                    if (buttonFocus[1])
                        GuiSetState(STATE_FOCUSED);
                    
                    if (GuiButton(settings.rec, settings.text))
                        currentScreen = SETTINGS;

                    GuiSetState(STATE_NORMAL);

                    if (buttonFocus[2])
                        GuiSetState(STATE_FOCUSED);
                    
                    if (GuiButton(quit.rec, quit.text))
                        currentScreen = QUIT;

                    GuiSetState(STATE_NORMAL);
                    
                    break;

                case SETTINGS:;
                    ClearBackground(BLACK);

                    GuiSetStyle(DEFAULT, TEXT_SIZE, 64);

                    int settingsX = monitorWidth / 2 - (((float) monitorWidth / 1.2) / 2),
                    settingsY = monitorHeight / 2 - (((float) monitorHeight / 1.2) / 2);

                    DrawRectangleGradientEx(
                        (Rectangle) {
                            settingsX,
                            settingsY,
                            (float) monitorWidth / 1.2,
                            (float) monitorHeight / 1.2
                        },
                        RED,
                        ORANGE,
                        YELLOW,
                        GREEN
                    );

                    audioVolume = GuiSlider(
                        (Rectangle) {
                            settingsX + 50 + MeasureText("1.00", 64),
                            settingsY + 50,
                            500,
                            50
                        },
                        TextFormat("%.2f", audioVolume),
                        "Volume audio",
                        audioVolume,
                        0,
                        1
                    );

                    SetMasterVolume(audioVolume);

                    /* if (GuiLabelButton(
                            (Rectangle) {
                                settingsX +  50,
                                settingsY + 150,
                                MeasureText("Textures:", 64),
                                50
                            },
                            "Textures:"
                        )
                    ) nonSoCosa = !nonSoCosa;

                    if (GuiDropdownBox(
                            (Rectangle) {
                                settingsX + MeasureText("Textures:", 64),
                                settingsY + 155,
                                250,
                                50
                            },
                            "Alte;Basse",
                            &nonSoCosa,
                            dropdownActive
                        )
                    ) dropdownActive = !dropdownActive; */

                    GuiSetState(STATE_FOCUSED);

                    if (GuiButton(
                            (Rectangle) {
                                settingsX + (float) monitorWidth / 1.2 - 300 - 50,
                                settingsY + (float) monitorHeight / 1.2 - 100 - 50,
                                300,
                                100
                            },
                            "Indietro"
                        )
                    ) currentScreen = MENU;

                    GuiSetState(STATE_NORMAL);

                    break;
            
                case GAME:;
                    ClearBackground(GRAY);

                    time(&currentTime);
                    timeInfo = localtime(&currentTime);
                    DrawText(TextFormat("%s %02d:%02d:%02d", ora.text, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec), ora.x, ora.y, ora.fontSize, ora.color);
                    DrawText(TextFormat("%s %07d", score.text, punteggio), score.x, score.y, score.fontSize, score.color);

                    if (punteggio >= APPLE_LENGTH)
                        currentScreen = WON;

                    for (i = 0; i < APPLE_LENGTH; i++) {
                        if (apples[i].eatenTime < 300)
                            DrawTextureRec(apples[i].texture, textureRectangle[i], (Vector2) {apples[i].x, apples[i].y}, apples[i].tint);

                        if (apples[i].isEaten)
                            apples[i].eatenTime += 1;

                        if (apples[i].isEaten && apples[i].y < monitorHeight - appleTextureHeight)
                            apples[i].y += 10;

                        if (!apples[i].isEaten) {
                            apples[i].x += apples[i].xVelocity * GetFrameTime();
                            apples[i].y += apples[i].yVelocity * GetFrameTime();
                        }

                        if (apples[i].x <= 0 && !apples[i].isEaten)
                            if (apples[i].xVelocity < 0)
                                apples[i].xVelocity *= -1;

                        if (apples[i].x >= monitorWidth - appleTextureWidth && !apples[i].isEaten)
                            if (apples[i].xVelocity > 0)
                                apples[i].xVelocity *= -1;

                        if (apples[i].y <= 0 && !apples[i].isEaten)
                            if (apples[i].yVelocity < 0)
                                apples[i].yVelocity *= -1;

                        if (apples[i].y >= monitorHeight - appleTextureHeight && !apples[i].isEaten)
                            if (apples[i].yVelocity > 0)
                                apples[i].yVelocity *= -1;

                        if (!apples[i].isEaten) {
                            movingTextureRectangle[i].x = apples[i].x;
                            movingTextureRectangle[i].y = apples[i].y;
                        }

                        if (CheckCollisionRecs(androidRectangle, movingTextureRectangle[i])) {
                            apples[i].isEaten = true;
                            
                            textureRectangle[i].x = appleEatenTexture;

                            movingTextureRectangle[i].x = 0;
                            movingTextureRectangle[i].y = 0;

                            switch (GetRandomValue(0, 2)) {
                                case 0:
                                    PlaySoundMulti(eating1);
                                    break;
                                case 1:
                                    PlaySoundMulti(eating2);
                                    break;
                                case 2:
                                    PlaySoundMulti(eating3);
                                    break;
                            }

                            punteggio++;
                        }
                    }

                    for (i = 0; i < HAMBURGERS_LENGTH; i++) {
                        DrawTexture(hamburgers[i].texture, hamburgers[i].x, hamburgers[i].y, hamburgers[i].tint);
       
                        hamburgers[i].x += hamburgers[i].xVelocity * GetFrameTime();
                        hamburgers[i].y += hamburgers[i].yVelocity * GetFrameTime();

                        if (hamburgers[i].x <= 0)
                            if (hamburgers[i].xVelocity < 0)
                                hamburgers[i].xVelocity *= -1;

                        if (hamburgers[i].x >= monitorWidth - hamburgers[i].textureWidth)
                            if (hamburgers[i].xVelocity > 0)
                                hamburgers[i].xVelocity *= -1;

                        if (hamburgers[i].y <= 0)
                            if (hamburgers[i].yVelocity < 0)
                                hamburgers[i].yVelocity *= -1;

                        if (hamburgers[i].y >= monitorHeight - hamburgers[i].textureHeight)
                            if (hamburgers[i].yVelocity > 0)
                                hamburgers[i].yVelocity *= -1;

                        movingTextureHamburgers[i].x = hamburgers[i].x;
                        movingTextureHamburgers[i].y = hamburgers[i].y;

                        if (CheckCollisionRecs(androidRectangle, movingTextureHamburgers[i])) {
                            textureRectangle[i].x = appleEatenTexture;

                            PlaySoundMulti(death);

                            currentScreen = LOST;
                        }
                    }

                    DrawTextureRec(android.texture, androidRectangleTexture, (Vector2) {android.x, android.y}, android.tint);

                    break;

                case LOST:;
                    ClearBackground(
                        (Color) {
                            255,
                            0,
                            0,
                            0
                        }
                    );

                    DrawText(
                        "HAI PERSO",
                        monitorWidth / 2 - MeasureText("HAI PERSO", 128) / 2,
                        monitorHeight / 2 - 220,
                        128,
                        (Color) {
                            255,
                            164,
                            0,
                            255
                        }
                    );

                    DrawText(
                        TextFormat("Punteggio: %07d", punteggio),
                        monitorWidth / 2 - MeasureText(TextFormat("Punteggio: %07d", punteggio), 64) / 2,
                        monitorHeight / 2 - 80,
                        64,
                        YELLOW
                    );

                    GuiSetState(STATE_FOCUSED);

                    if (GuiButton(
                            (Rectangle) {
                                monitorWidth / 2 - 400,
                                monitorHeight / 2,
                                800,
                                200
                            },
                            "Ritorna al menu"
                        )
                    ) currentScreen = MENU;

                    GuiSetState(STATE_NORMAL);

                    break;

                case WON:;
                    ClearBackground(
                        (Color) {
                            0,
                            255,
                            0,
                            0
                        }
                    );

                    DrawText(
                        "HAI VINTO",
                        monitorWidth / 2 - MeasureText("HAI PERSO", 128) / 2,
                        monitorHeight / 2 - 220,
                        128,
                        (Color) {
                            255,
                            255,
                            0,
                            255
                        }
                    );

                    DrawText(
                        TextFormat("Punteggio: %07d", punteggio),
                        monitorWidth / 2 - MeasureText(TextFormat("Punteggio: %07d", punteggio), 64) / 2,
                        monitorHeight / 2 - 80,
                        64,
                        (Color) {
                            255,
                            255,
                            0,
                            255
                        }
                    );

                    GuiSetState(STATE_FOCUSED);

                    if (GuiButton(
                            (Rectangle) {
                                monitorWidth / 2 - 400,
                                monitorHeight / 2,
                                800,
                                200
                            },
                            "Ritorna al menu"
                        )
                    ) currentScreen = MENU;

                    GuiSetState(STATE_NORMAL);

                    break;
                
                default:;
                    break;
            }

        EndDrawing();
    }

    for (i = 0; i < 250; i++)
        UnloadTexture(apples[i].texture);

    for (i = 0; i < 10; i++)
        UnloadTexture(hamburgers[i].texture);

    UnloadTexture(android.texture);

    StopSoundMulti();

    UnloadSound(eating1);
    UnloadSound(eating2);
    UnloadSound(eating3);
    UnloadSound(death);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}