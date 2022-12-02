#include "glfw-common.hpp"
#include "glfw-timers.hpp"


namespace glfw {

JS_METHOD(getTimerValue) { NAPI_ENV;
	RET_NUM(glfwGetTimerValue());
}


JS_METHOD(getTimerFrequency) { NAPI_ENV;
	RET_NUM(glfwGetTimerFrequency());
}

} // namespace glfw
