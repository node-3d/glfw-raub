#ifndef _GLFW_PLATFORM_HPP_
#define _GLFW_PLATFORM_HPP_

#include "glfw-common.hpp"


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


namespace glfw {
	JS_METHOD(platformDevice);
	JS_METHOD(platformWindow);
	JS_METHOD(platformContext);
} // namespace glfw


#endif /* _GLFW_PLATFORM_HPP_ */
