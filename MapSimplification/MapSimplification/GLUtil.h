#pragma once

// Win32 Includes
#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES

// Include glm
#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>

// Basic util includes
#include <string>
#include <sstream>

// OpenGL Includes
#include <gl/glew.h> //Needs to be included first
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glext.h>
#include <gl/wglew.h>

#include <gl/freeglut.h>


// Link to libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

bool OutErrorMsg(LPCSTR message);

// OpenGL helper functions
void DrawString(std::string text, void* stringtype, int x, int y);

float Lerp(float v0, float v1, float t);

float Clamp(float min, float max, float val);