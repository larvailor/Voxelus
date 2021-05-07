#pragma once

#include "core/Service.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION

#ifndef DEBUG_MODE
	#define DEBUG_MODE
#endif //DEBUG_MODE