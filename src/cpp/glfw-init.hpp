#ifndef _GLFW_INIT_HPP_
#define _GLFW_INIT_HPP_

#include "glfw-common.hpp"


namespace glfw {
	void deinit();
	
	JS_METHOD(init);
	JS_METHOD(initHint);
	JS_METHOD(terminate);
	JS_METHOD(testScene);
} // namespace glfw


#endif /* _GLFW_INIT_HPP_ */
