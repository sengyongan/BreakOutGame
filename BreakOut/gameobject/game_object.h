/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

//#include <glad/glad.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../tool/texture.h"
#include "../renderer/sprite_renderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    float       Rotation;
    bool        IsSolid;//�Ƿ�̶������ɱ��ƻ���
    bool        Destroyed;
    //
    float       MaxHealth = 1;
    float       CurHealth;
    // render state
    Texture2D   Sprite;
    // constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // draw sprite
    virtual void Draw(SpriteRenderer& renderer);
    //
    void DecreaseHealth() { CurHealth--; }
    void DecreaseColor() {
        Color *= (CurHealth / MaxHealth) + ((1 - (CurHealth / MaxHealth)) / 2);//������ɫ͸����̫��
    }
    
};

#endif