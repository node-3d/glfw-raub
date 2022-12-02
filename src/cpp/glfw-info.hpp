#ifndef _GLFW_INFO_HPP_
#define _GLFW_INFO_HPP_

#include "glfw-common.hpp"


namespace glfw {
	JS_METHOD(getError);
	JS_METHOD(getFramebufferSize);
	JS_METHOD(swapBuffers);
	JS_METHOD(swapInterval);
	JS_METHOD(extensionSupported);
} // namespace glfw


#endif /* _GLFW_INFO_HPP_ */
