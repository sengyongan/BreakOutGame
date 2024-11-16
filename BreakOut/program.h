#pragma once

#include "game.h"
#include "tool/resource_manager.h"
#include"tool/debug.h"
#include"Window/window.h"

Game& Breakout = GetGameRef();

Window& window = Window::GetInstance();


int main(int argc, char* argv[])
{
    //init window
    window.Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Sen's BreakOut_Game");

    // Initialize game
    Breakout.Init();

    // DeltaTime variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Start Game within Menu State
    Breakout.State = GAME_ACTIVE;


    while (!glfwWindowShouldClose(window.GetWindow()))
    {
        // Calculate delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        deltaTime = 0.001f;

         //Manage user input
        Breakout.ProcessInput(deltaTime);
        // Update Game state
        Breakout.Update(deltaTime);
        glCheckError();

        //clear
        window.Clear();

        //
        Breakout.Render();
        glCheckError();

        //update
        window.Update();

    }

    // Delete all resources as loaded using the resource manager
    ResourceManager::Clear();

    window.Terminate();
    return 0;
}


