#include "glfw-common.hpp"
#include "glfw-input.hpp"


namespace glfw {

JS_METHOD(setInputMode) { NAPI_ENV; THIS_WINDOW;
	REQ_INT32_ARG(1, mode);
	REQ_INT32_ARG(2, value);
	
	glfwSetInputMode(window, mode, value);
	RET_UNDEFINED;
}


JS_METHOD(getInputMode) { NAPI_ENV; THIS_WINDOW;
	REQ_INT32_ARG(1, mode);
	RET_NUM(glfwGetInputMode(window, mode));
}


/* Input handling */
JS_METHOD(getKey) { NAPI_ENV; THIS_WINDOW;
	REQ_UINT32_ARG(1, key);
	RET_NUM(glfwGetKey(window, key));
}


JS_METHOD(getMouseButton) { NAPI_ENV; THIS_WINDOW;
	REQ_UINT32_ARG(1, button);
	RET_NUM(glfwGetMouseButton(window, button));
}


JS_METHOD(rawMouseMotionSupported) { NAPI_ENV;
	RET_BOOL(glfwRawMouseMotionSupported() == GLFW_TRUE);
}


JS_METHOD(getKeyName) { NAPI_ENV;
	REQ_INT32_ARG(0, key);
	REQ_INT32_ARG(1, scancode);
	RET_STR(glfwGetKeyName(key, scancode));
}


JS_METHOD(getKeyScancode) { NAPI_ENV;
	REQ_INT32_ARG(0, key);
	RET_NUM(glfwGetKeyScancode(key));
}


} // namespace glfw
