#ifndef _GLFW_CURSOR_HPP_
#define _GLFW_CURSOR_HPP_

#include "glfw-common.hpp"


namespace glfw {
	JS_METHOD(getCursorPos);
	JS_METHOD(setCursorPos);
    
	JS_METHOD(createCursor);
	JS_METHOD(createStandardCursor);
	JS_METHOD(destroyCursor);
	JS_METHOD(setCursor);
} // namespace glfw


#endif /* _GLFW_CURSOR_HPP_ */
