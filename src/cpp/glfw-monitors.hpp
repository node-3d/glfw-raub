#ifndef _GLFW_MONITORS_HPP_
#define _GLFW_MONITORS_HPP_

#include "glfw-common.hpp"


namespace glfw {
	DBG_EXPORT Napi::Object describeMonitor(Napi::Env env, GLFWmonitor *monitor, bool isPrimary);
	
	DBG_EXPORT JS_METHOD(getMonitors);
	DBG_EXPORT JS_METHOD(getPrimaryMonitor);
} // namespace glfw


#endif /* _GLFW_MONITORS_HPP_ */
