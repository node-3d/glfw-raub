#ifndef _GLFW_INPUT_HPP_
#define _GLFW_INPUT_HPP_

#include "glfw-common.hpp"


namespace glfw {
	JS_METHOD(setInputMode);
	JS_METHOD(getInputMode);
	JS_METHOD(getKey);
	JS_METHOD(getMouseButton);
	JS_METHOD(rawMouseMotionSupported);
	JS_METHOD(getKeyName);
	JS_METHOD(getKeyScancode);
} // namespace glfw


#endif /* _GLFW_INPUT_HPP_ */
