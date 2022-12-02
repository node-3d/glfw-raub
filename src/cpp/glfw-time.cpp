#include "glfw-common.hpp"
#include "glfw-time.hpp"


namespace glfw {

JS_METHOD(getTime) { NAPI_ENV;
	RET_NUM(glfwGetTime());
}


JS_METHOD(setTime) { NAPI_ENV;
	REQ_DOUBLE_ARG(0, time);
	glfwSetTime(time);
	RET_UNDEFINED;
}

} // namespace glfw
