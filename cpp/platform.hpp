#ifndef _PLATFORM_HPP_
#define _PLATFORM_HPP_


// OpenGL stuff

#include <GL/glew.h>

#define GLFW_NO_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>

#ifdef _WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define GLFW_EXPOSE_NATIVE_WGL
#elif __APPLE__
	#define GLFW_EXPOSE_NATIVE_COCOA
	#define GLFW_EXPOSE_NATIVE_NSGL
#elif __linux__
	#define GLFW_EXPOSE_NATIVE_X11
	#define GLFW_EXPOSE_NATIVE_GLX
#endif
#include <GLFW/glfw3native.h>


// Fix bad defines

#undef True
#undef False


#endif /* _PLATFORM_HPP_ */
