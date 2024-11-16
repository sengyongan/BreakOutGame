#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include"../tool/header.h"
#include"../game.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Window {
	
public:
	//单例
	static Window& GetInstance() {
		static Window instance;
		return instance;

	}

	int Init(const GLuint width, const GLuint height, const char* windowName);
	void Clear();
	void Update();
	void Terminate();

	GLFWwindow* GetWindow() {
		return window;
	}
	static glm::mat4 GetProjection() {
		return Window::projection;
	}

	static glm::mat4 projection;

	void SetWindowSize();

private:
	GLFWwindow* window;


	//私有构造
	Window() {}

};