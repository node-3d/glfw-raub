#include "glfw-common.hpp"
#include "glfw-platform.hpp"

#ifdef __APPLE__
#include <OpenGL/CGLCurrent.h>
#include <OpenGL/CGLDevice.h>
#endif


namespace glfw {

DBG_EXPORT JS_METHOD(platformDevice) { NAPI_ENV;
#ifdef _WIN32
	RET_NUM(reinterpret_cast<uint64_t>(wglGetCurrentDC()));
#elif __linux__
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetX11Display()));
#elif __APPLE__
	CGLContextObj kCGLContext = CGLGetCurrentContext();
	RET_NUM(reinterpret_cast<uint64_t>(CGLGetShareGroup(kCGLContext)));
#endif
}


DBG_EXPORT JS_METHOD(platformWindow) { NAPI_ENV; THIS_WINDOW;
#ifdef _WIN32
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetWin32Window(window)));
#elif __linux__
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetX11Window(window)));
#elif __APPLE__
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetCocoaWindow(window)));
#endif
}


DBG_EXPORT JS_METHOD(platformContext) { NAPI_ENV; THIS_WINDOW;
#ifdef _WIN32
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetWGLContext(window)));
#elif __linux__
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetGLXContext(window)));
#elif __APPLE__
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetNSGLContext(window)));
#endif
}

} // namespace glfw
