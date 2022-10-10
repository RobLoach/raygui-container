/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0

*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define RAYGUI_CONTAINER_IMPLEMENTATION
#include "raygui-container.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raygui-container example");
    SetTargetFPS(30);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);

    // Create the GUI
    GuiContainer gui = InitGuiContainer();
    GuiElement* newGame = AddGuiButton(gui, (Rectangle){ 20, 20, 250, 60 }, "New Game");
    GuiElement* loadGame = AddGuiButton(gui, (Rectangle){ 20, 90, 250, 60 }, "Load Game");
    GuiElement* exit = AddGuiButton(gui, (Rectangle){ 20, 160, 250, 60 }, "Exit");
    GuiElement* options = AddGuiButton(gui, (Rectangle){ 280, 90, 250, 60 }, "Options");
    GuiElement* highscores = AddGuiButton(gui, (Rectangle){ 540, 90, 250, 60 }, "Highscores");
    GuiElement* showFPS = AddCheckBox(gui, (Rectangle){ 20, 360, 60, 60 }, "Show FPS", false);
    GuiElement* slider = AddSlider(gui, (Rectangle){ screenWidth - 300, 200, 150, 30 }, "Less", "More", 30, 0, 100);

    const char* message = "Use the keyboard to handle input. Arrows/Enter";
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose())
    {
        // Update the GUI
        UpdateGuiContainer(&gui);

        // Check Button States
        if (IsGuiButtonPressed(newGame)) {
            message = "Pressed the New Game button!";
        }
        if (IsGuiButtonPressed(loadGame)) {
            message = "Now Loading the Game";
        }
        if (IsGuiButtonPressed(exit)) {
            message = "Exit time... Bye bye!";
        }
        if (IsGuiButtonPressed(options)) {
            message = "Options Time";
        }
        if (IsGuiButtonPressed(highscores)) {
            message = "Show Highscores";
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText(message, 190, 300, 20, DARKGRAY);

            // Check the High Resolution check box
            if (IsGuiCheckBoxChecked(showFPS)) {
                DrawFPS(20, 0);
            }

            // Draw the GUI
            DrawGuiContainer(gui);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // Unload the GUI
    UnloadGuiContainer(gui);

    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
