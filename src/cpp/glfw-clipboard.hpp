#ifndef _GLFW_CLIPBOARD_HPP_
#define _GLFW_CLIPBOARD_HPP_

#include "glfw-common.hpp"


namespace glfw {
	DBG_EXPORT JS_METHOD(setClipboardString);
	DBG_EXPORT JS_METHOD(getClipboardString);
} // namespace glfw


#endif /* _GLFW_CLIPBOARD_HPP_ */
