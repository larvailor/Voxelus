#pragma once

class FileDialogs
{
public:

	static std::string OpenFile(const char* filter, GLFWwindow* window);
	static std::string SaveFile(const char* filter, GLFWwindow* window);
};
