#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include"../gameobject/game_object.h"
#include"../gameobject/ball_object.h"
#include <tuple>

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

class collision
{
public:
	//检查是否碰撞
	static GLboolean  CheckCollision_AABB_AABB(GameObject& one, GameObject& two); // AABB - AABB collision
	static Collision  CheckCollision_Circle_AABB(BallObject& one, GameObject& two); // AABB - AABB collision
	//检查碰撞方向
	static Direction VectorDirection(glm::vec2 target);
};

