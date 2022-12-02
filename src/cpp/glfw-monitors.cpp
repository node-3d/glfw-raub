#include <sstream>
#include <iostream>
#include <locale.h>

#include "glfw-common.hpp"
#include "glfw-events.hpp"

#ifdef __APPLE__
#include <OpenGL/CGLCurrent.h>
#include <OpenGL/CGLDevice.h>
#endif


namespace glfw {

Napi::Object describeMonitor(Napi::Env env, GLFWmonitor *monitor, bool isPrimary) {
	Napi::Object jsMonitor = Napi::Object::New(env);
	
	jsMonitor.Set("is_primary", isPrimary);
	jsMonitor.Set("name", JS_STR(glfwGetMonitorName(monitor)));
	
	int xpos, ypos;
	glfwGetMonitorPos(monitor, &xpos, &ypos);
	jsMonitor.Set("pos_x", JS_NUM(xpos));
	jsMonitor.Set("pos_y", JS_NUM(ypos));
	
	int width, height;
	glfwGetMonitorPhysicalSize(monitor, &width, &height);
	jsMonitor.Set("width_mm", JS_NUM(width));
	jsMonitor.Set("height_mm", JS_NUM(height));
	
	float xscale, yscale;
	glfwGetMonitorContentScale(monitor, &xscale, &yscale);
	
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	jsMonitor.Set("width", JS_NUM(mode->width));
	jsMonitor.Set("height", JS_NUM(mode->height));
	jsMonitor.Set("rate", JS_NUM(mode->refreshRate));
	
	int modeCount;
	const GLFWvidmode *modes = glfwGetVideoModes(monitor, &modeCount);
	Napi::Array jsModes = Napi::Array::New(env);
	
	for (int j = 0; j < modeCount; j++) {
		
		Napi::Object jsMode = Napi::Object::New(env);
		jsMode.Set("width", JS_NUM(modes[j].width));
		jsMode.Set("height", JS_NUM(modes[j].height));
		jsMode.Set("rate", JS_NUM(modes[j].refreshRate));
		
		// NOTE: Are color bits necessary?
		jsModes.Set(j, jsMode);
		
	}
	
	jsMonitor.Set("modes", jsModes);
	
	return jsMonitor;
}

/* TODO: Monitor configuration change callback */

JS_METHOD(getMonitors) { NAPI_ENV;
	int monitorCount;
	
	GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
	GLFWmonitor *primary = glfwGetPrimaryMonitor();
	
	Napi::Array jsMonitors = Napi::Array::New(env);
	
	for (int i = 0; i < monitorCount; i++) {
		jsMonitors.Set(
			i,
			describeMonitor(env, monitors[i], monitors[i] == primary)
		);
	}
	
	RET_VALUE(jsMonitors);
}


JS_METHOD(getPrimaryMonitor) { NAPI_ENV;
	GLFWmonitor *primary = glfwGetPrimaryMonitor();
	if (!primary) {
		RET_NULL;
	}
	
	RET_VALUE(describeMonitor(env, primary, true));
}


} // namespace glfw
