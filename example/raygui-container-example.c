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

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    // Create the GUI
    GuiContainer gui = InitGuiContainer();
    GuiElement* newGame = AddGuiButton(gui, (Rectangle){ 30, 30, 125, 30 }, "New Game");
    GuiElement* loadGame = AddGuiButton(gui, (Rectangle){ 30, 70, 125, 30 }, "Load Game");
    GuiElement* exit = AddGuiButton(gui, (Rectangle){ 30, 110, 125, 30 }, "Exit");
    const char* message = "Use the keyboard to handle input. Arrows/Enter";
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update the GUI
        UpdateGuiContainer(&gui);

        // Check Button States
        if (IsGuiButtonPressed(gui, newGame)) {
            message = "New Game! This should start a new game.";
        }
        if (IsGuiButtonPressed(gui, loadGame)) {
            message = "Load Game! This will load the game.";
        }
        if (IsGuiButtonPressed(gui, exit)) {
            message = "Exit time... Bye bye!";
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText(message, 190, 200, 20, DARKGRAY);

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