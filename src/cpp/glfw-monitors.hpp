#ifndef _GLFW_MONITORS_HPP_
#define _GLFW_MONITORS_HPP_

#include "glfw-common.hpp"


namespace glfw {
	Napi::Object describeMonitor(Napi::Env env, GLFWmonitor *monitor, bool isPrimary);
	
	JS_METHOD(getMonitors);
	JS_METHOD(getPrimaryMonitor);
} // namespace glfw


#endif /* _GLFW_MONITORS_HPP_ */
