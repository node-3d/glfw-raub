#ifndef _GLFW_COMMON_HPP_
#define _GLFW_COMMON_HPP_

#include <addon-tools.hpp>

#define GLFW_NO_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>


// Fix bad defines
#undef True
#undef False


#define THIS_WINDOW                                                           \
	REQ_OFFS_ARG(0, __win_handle);                                            \
	GLFWwindow *window = reinterpret_cast<GLFWwindow*>(__win_handle);


#define THIS_STATE                                                            \
	WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));


#define STATE_ENV                                                             \
	Napi::Env env = state->emitter.Env();


#endif /* _GLFW_COMMON_HPP_ */
