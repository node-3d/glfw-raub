#pragma once

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

#define RET_GLFW_VOID                                                         \
	return glfw::undefined;

inline Napi::Number NewInt64(napi_env env, int64_t val) {
	napi_value value;
	napi_status status = napi_create_double(env, val, &value);
	NAPI_THROW_IF_FAILED(env, status, Napi::Number());
	return Napi::Number(env, value);
}

#define RET_PTR(VAL) return NewInt64(env, reinterpret_cast<int64_t>(VAL))
	
namespace glfw {
	Napi::Value undefined;
}
