/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef POWER_UP_H
#define POWER_UP_H
#include <string>
#include<vector>

//#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include"../tool/resource_manager.h"
#include"../post//post_processor.h"
#include"ball_object.h"

// The size of a PowerUp block
const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
// Velocity a PowerUp block has when spawned
const glm::vec2 VELOCITY(0.0f, 150.0f);


// PowerUp inherits its state and rendering functions from
// GameObject but also holds extra information to state its
// active duration and whether it is activated or not. 
// The type of PowerUp is stored as a string.
class PowerUp : public GameObject
{
public:
    // powerup state
    std::string Type;
    float       Duration;
    bool        Activated;
    std::vector<PowerUp>  PowerUps;
    PowerUp();
    // constructor
    PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture)
        : GameObject(position, POWERUP_SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() { }
    void SpawnPowerUps(GameObject& block);
    void UpdatePowerUps(GLfloat dt, BallObject* ball, GameObject* player, PostProcessor* effects);
    void ActivatePowerUp(PowerUp& powerUp, BallObject* ball, GameObject* player, PostProcessor* effects);
};

#endif