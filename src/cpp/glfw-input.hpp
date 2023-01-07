#ifndef _GLFW_INPUT_HPP_
#define _GLFW_INPUT_HPP_

#include "glfw-common.hpp"


namespace glfw {
	DBG_EXPORT JS_METHOD(setInputMode);
	DBG_EXPORT JS_METHOD(getInputMode);
	DBG_EXPORT JS_METHOD(getKey);
	DBG_EXPORT JS_METHOD(getMouseButton);
	DBG_EXPORT JS_METHOD(rawMouseMotionSupported);
	DBG_EXPORT JS_METHOD(getKeyName);
	DBG_EXPORT JS_METHOD(getKeyScancode);
} // namespace glfw


#endif /* _GLFW_INPUT_HPP_ */
