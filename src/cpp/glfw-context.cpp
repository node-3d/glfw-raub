#include "glfw-common.hpp"
#include "glfw-context.hpp"


namespace glfw {

JS_METHOD(makeContextCurrent) { NAPI_ENV; THIS_WINDOW;
	glfwMakeContextCurrent(window);
	RET_UNDEFINED;
}


JS_METHOD(getCurrentContext) { NAPI_ENV;
	GLFWwindow *window = glfwGetCurrentContext();
	RET_NUM(reinterpret_cast<uint64_t>(window));
}

} // namespace glfw
