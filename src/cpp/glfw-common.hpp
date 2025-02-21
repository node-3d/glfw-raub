#ifndef _GLFW_COMMON_HPP_
#define _GLFW_COMMON_HPP_

#include <addon-tools.hpp>

#ifdef _WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
#elif __APPLE__
	#define VK_USE_PLATFORM_MACOS_MVK
#elif __linux__
	#define VK_USE_PLATFORM_XCB_KHR
#endif


#define GLFW_NO_GLU
#define GLFW_DLL
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


// Fix bad defines
#undef True
#undef False


#define THIS_WINDOW                                                           \
	GLFWwindow *window = reinterpret_cast<GLFWwindow*>(info[0].As<Napi::Number>().Int64Value());


#define THIS_STATE                                                            \
	WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));


#define STATE_ENV                                                             \
	Napi::Env env = state->emitter.Env();


#endif /* _GLFW_COMMON_HPP_ */
