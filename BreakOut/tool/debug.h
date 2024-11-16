#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include<iostream>

GLenum glCheckError_(const char* file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__) 