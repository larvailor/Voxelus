#include "stdfax.h"

#include "FileManager.h"

#include <commdlg.inl>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

std::string FileDialogs::OpenFile(const char* filter, GLFWwindow* window)
{
	OPENFILENAMEA fileName;
	CHAR szFile[512] = { 0 };
	ZeroMemory(&fileName, sizeof(OPENFILENAMEA));
	fileName.lStructSize = sizeof(OPENFILENAMEA);
	fileName.hwndOwner = glfwGetWin32Window(window);
	fileName.lpstrFile = szFile;
	fileName.nMaxFile = sizeof(szFile);
	fileName.lpstrFilter = filter;
	fileName.nFilterIndex = 1;
	fileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileNameA(&fileName) == TRUE)
	{
		return fileName.lpstrFile;
	}

	return std::string();
}

std::string FileDialogs::SaveFile(const char* filter, GLFWwindow* window)
{
	OPENFILENAMEA fileName;
	CHAR szFile[512] = { 0 };
	ZeroMemory(&fileName, sizeof(OPENFILENAMEA));
	fileName.lStructSize = sizeof(OPENFILENAMEA);
	fileName.hwndOwner = glfwGetWin32Window(window);
	fileName.lpstrFile = szFile;
	fileName.nMaxFile = sizeof(szFile);
	fileName.lpstrFilter = filter;
	fileName.nFilterIndex = 1;
	fileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetSaveFileNameA(&fileName) == TRUE)
	{
		return fileName.lpstrFile;
	}

	return std::string();
}
