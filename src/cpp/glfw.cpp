#include <cstdlib>
#include <sstream>
#include <iostream>
#include <locale.h>

#include "platform.hpp"
#include "win-state.hpp"
#include "events.hpp"
#include "glfw.hpp"


using namespace std;


#define THIS_WINDOW                                                           \
	REQ_OFFS_ARG(0, __win_handle);                                            \
	GLFWwindow *window = reinterpret_cast<GLFWwindow*>(__win_handle);


namespace glfw {

// The default context for all to share
GLFWwindow *_share = nullptr;

// Cached visibility hint value
bool hintVisible = true;

std::vector<WinState *> states;

bool isInited = false;


void errorCb(int error, const char* description) {
	std::cout << "GLFW Error " << error << ": " << description << std::endl;
}


JS_METHOD(init) { NAPI_ENV;
	
	setlocale(LC_ALL, "");
	
	glfwSetErrorCallback(errorCb);
	
	isInited = glfwInit() == GLFW_TRUE;
	RET_BOOL(isInited);
	
}


// Cleanup resources
void deinit() {
	
	if ( ! isInited ) {
		return;
	}
	
	for (int i = states.size() - 1; i >= 0; i--) {
		WinState *state = states[i];
		GLFWwindow *window = state->window;
		if ( ! window ) {
			continue;
		}
		// Window callbacks
		glfwSetWindowPosCallback(window, nullptr);
		glfwSetWindowSizeCallback(window, nullptr);
		glfwSetWindowCloseCallback(window, nullptr);
		glfwSetWindowRefreshCallback(window, nullptr);
		glfwSetWindowFocusCallback(window, nullptr);
		glfwSetWindowIconifyCallback(window, nullptr);
		glfwSetFramebufferSizeCallback(window, nullptr);
		glfwSetDropCallback(window, nullptr);
		// Input callbacks
		glfwSetKeyCallback(window, nullptr);
		glfwSetCharCallback(window, nullptr);
		glfwSetMouseButtonCallback(window, nullptr);
		glfwSetCursorPosCallback(window, nullptr);
		glfwSetCursorEnterCallback(window, nullptr);
		glfwSetScrollCallback(window, nullptr);
		// Destroy
		glfwDestroyWindow(window);
		state->window = nullptr;
	}
	
	states.clear();
	isInited = false;
	_share = nullptr;
	
	glfwTerminate();
	
}


JS_METHOD(terminate) { NAPI_ENV;
	
	deinit();
	RET_UNDEFINED;
	
}


JS_METHOD(getVersion) { NAPI_ENV;
	
	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	
	Napi::Object obj = Napi::Object::New(env);
	obj.Set("major", JS_NUM(major));
	obj.Set("minor", JS_NUM(minor));
	obj.Set("rev", JS_NUM(rev));
	
	RET_VALUE(obj);
	
}


JS_METHOD(getVersionString) { NAPI_ENV;
	
	const char *ver = glfwGetVersionString();
	RET_STR(ver);
	
}


JS_METHOD(getTime) { NAPI_ENV;
	
	RET_NUM(glfwGetTime());
	
}


JS_METHOD(setTime) { NAPI_ENV;
	
	REQ_DOUBLE_ARG(0, time);
	
	glfwSetTime(time);
	RET_UNDEFINED;
	
}


/* TODO: Monitor configuration change callback */

JS_METHOD(getMonitors) { NAPI_ENV;
	
	int monitor_count, mode_count, xpos, ypos, width, height;
	
	GLFWmonitor **monitors = glfwGetMonitors(&monitor_count);
	GLFWmonitor *primary = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode, *modes;
	
	Napi::Array js_monitors = Napi::Array::New(env);
	
	for (int i = 0; i < monitor_count; i++) {
		
		Napi::Object js_monitor = Napi::Object::New(env);
		js_monitor.Set("is_primary", static_cast<bool>(monitors[i] == primary));
		js_monitor.Set("index", JS_NUM(i));
		js_monitor.Set("name", JS_STR(glfwGetMonitorName(monitors[i])));
		
		glfwGetMonitorPos(monitors[i], &xpos, &ypos);
		js_monitor.Set("pos_x", JS_NUM(xpos));
		js_monitor.Set("pos_y", JS_NUM(ypos));
		
		glfwGetMonitorPhysicalSize(monitors[i], &width, &height);
		js_monitor.Set("width_mm", JS_NUM(width));
		js_monitor.Set("height_mm", JS_NUM(height));
		
		mode = glfwGetVideoMode(monitors[i]);
		js_monitor.Set("width", JS_NUM(mode->width));
		js_monitor.Set("height", JS_NUM(mode->height));
		js_monitor.Set("rate", JS_NUM(mode->refreshRate));
		
		modes = glfwGetVideoModes(monitors[i], &mode_count);
		Napi::Array js_modes = Napi::Array::New(env);
		
		for (int j = 0; j < mode_count; j++) {
			
			Napi::Object js_mode = Napi::Object::New(env);
			js_mode.Set("width", JS_NUM(modes[j].width));
			js_mode.Set("height", JS_NUM(modes[j].height));
			js_mode.Set("rate", JS_NUM(modes[j].refreshRate));
			
			// NOTE: Are color bits necessary?
			js_modes.Set(JS_NUM(j), js_mode);
			
		}
		
		js_monitor.Set("modes", js_modes);
		js_monitors.Set(JS_NUM(i), js_monitor);
		
	}
	
	RET_VALUE(js_monitors);
	
}


JS_METHOD(testJoystick) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, width);
	REQ_UINT32_ARG(1, height);
	REQ_FLOAT_ARG(2, translateX);
	REQ_FLOAT_ARG(3, translateY);
	REQ_FLOAT_ARG(4, translateZ);
	REQ_FLOAT_ARG(5, rotateX);
	REQ_FLOAT_ARG(6, rotateY);
	REQ_FLOAT_ARG(7, rotateZ);
	REQ_FLOAT_ARG(8, angle);
	
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	
	float ratio = static_cast<float>(width) / static_cast<float>(height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glRotatef(angle, rotateX, rotateY, rotateZ);
	glTranslatef(translateX, translateY, translateZ);
	
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();
	
	RET_UNDEFINED;
	
}


JS_METHOD(testScene) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, width);
	REQ_UINT32_ARG(1, height);
	LET_FLOAT_ARG(2, z);
	
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	
	float ratio = static_cast<float>(width) / static_cast<float>(height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glRotatef(static_cast<float>(glfwGetTime()) * 50.f, 0.f, 0.f, 1.f);
	
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f + z, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f + z, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f + z, 0.6f, 0.f);
	glEnd();
	
	RET_UNDEFINED;
	
}


JS_METHOD(windowHint) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, target);
	REQ_UINT32_ARG(1, hint);
	
	if (target == GLFW_VISIBLE) {
		hintVisible = hint != 0;
	}
	
	glfwWindowHint(target, hint);
	RET_UNDEFINED;
	
}


JS_METHOD(defaultWindowHints) { NAPI_ENV;
	
	glfwDefaultWindowHints();
	RET_UNDEFINED;
	
}


JS_METHOD(joystickPresent) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, joy);
	
	bool isPresent = glfwJoystickPresent(joy);
	
	RET_BOOL(static_cast<bool>(isPresent));
	
}


string intToString(int number) {
	
	ostringstream buff;
	buff << number;
	return buff.str();
	
}


string floatToString(float number) {
	
	ostringstream buff;
	buff << number;
	return buff.str();
	
}


string buttonToString(unsigned char c) {
	
	int number = static_cast<int>(c);
	return intToString(number);
	
}


JS_METHOD(getJoystickAxes) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, joy);
	
	int count;
	const float *axisValues = glfwGetJoystickAxes(joy, &count);
	string response = "";
	
	for (int i = 0; i < count; i++) {
		response.append(floatToString(axisValues[i]));
		response.append(","); //Separator
	}
	
	RET_STR(response);
	
}


JS_METHOD(getJoystickButtons) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, joy);
	
	int count = 0;
	const unsigned char* response = glfwGetJoystickButtons(joy, &count);
	
	string strResponse = "";
	for (int i = 0; i < count; i++) {
		strResponse.append(buttonToString(response[i]));
		strResponse.append(",");
	}
	
	RET_STR(strResponse.c_str());
	
}


JS_METHOD(getJoystickName) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, joy);
	
	const char* response = glfwGetJoystickName(joy);
	
	RET_STR(response);
	
}


// Name altered due to windows.h collision
JS_METHOD(createWindow) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, width);
	REQ_UINT32_ARG(1, height);
	REQ_OBJ_ARG(2, emitter);
	LET_STR_ARG(3, str);
	LET_INT32_ARG(4, monitor_idx);
	
	GLFWmonitor **monitors = nullptr;
	GLFWmonitor *monitor = nullptr;
	int monitor_count;
	
	if (info.Length() >= 5 && monitor_idx >= 0) {
		monitors = glfwGetMonitors(&monitor_count);
		if (monitor_idx >= monitor_count) {
			JS_THROW("Invalid monitor");
			RET_NULL;
		}
		monitor = monitors[monitor_idx];
	}
	
	if ( ! _share ) {
		glfwWindowHint(GLFW_VISIBLE, false);
		_share = glfwCreateWindow(128, 128, "_GLFW_ROOT_SHARED", nullptr, nullptr);
		glfwWindowHint(GLFW_VISIBLE, hintVisible);
	}
	
	GLFWwindow *window = glfwCreateWindow(
		width,
		height,
		str.c_str(),
		monitor,
		_share
	);
	
	if ( ! window ) {
		// can't create window, throw error
		JS_THROW("Can't create GLFW window");
		RET_NULL;
	}
	
	glfwMakeContextCurrent(window);
	
	// make sure cursor is always shown
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	
	// Store WinState as user pointer
	WinState *state = new WinState(window, emitter);
	states.push_back(state);
	glfwSetWindowUserPointer(window, state);
	
	
	// Window callbacks
	glfwSetWindowPosCallback(window, windowPosCB);
	glfwSetWindowSizeCallback(window, windowSizeCB);
	glfwSetWindowCloseCallback(window, windowCloseCB);
	glfwSetWindowRefreshCallback(window, windowRefreshCB);
	glfwSetWindowFocusCallback(window, windowFocusCB);
	glfwSetWindowIconifyCallback(window, windowIconifyCB);
	glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCB);
	glfwSetDropCallback(window, windowDropCB);
	
	// Input callbacks
	glfwSetKeyCallback(window, keyCB);
	glfwSetCharCallback(window, charCB);
	glfwSetMouseButtonCallback(window, mouseButtonCB);
	glfwSetCursorPosCallback(window, cursorPosCB);
	glfwSetCursorEnterCallback(window, cursorEnterCB);
	glfwSetScrollCallback(window, scrollCB);
	
	RET_NUM(reinterpret_cast<uint64_t>(window));
	
}


JS_METHOD(platformWindow) { NAPI_ENV; THIS_WINDOW;
	
#ifdef _WIN32
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetWin32Window(window)));
#elif __linux__
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetX11Window(window)));
#elif __APPLE__
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetCocoaWindow(window)));
#endif
	
}


JS_METHOD(platformContext) { NAPI_ENV; THIS_WINDOW;
	
#ifdef _WIN32
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetWGLContext(window)));
#elif __linux__
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetGLXContext(window)));
#elif __APPLE__
	RET_NUM(reinterpret_cast<uint64_t>(glfwGetNSGLContext(window)));
#endif
	
}


JS_METHOD(destroyWindow) { NAPI_ENV; THIS_WINDOW;
	
	WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));
	delete state;
	
	glfwDestroyWindow(window);
	RET_UNDEFINED;
	
}


JS_METHOD(setWindowTitle) { NAPI_ENV; THIS_WINDOW;
	
	REQ_STR_ARG(1, str);
	
	glfwSetWindowTitle(window, str.c_str());
	RET_UNDEFINED;
	
}


JS_METHOD(setWindowIcon) { NAPI_ENV; THIS_WINDOW;
	
	REQ_OBJ_ARG(1, icon);
	
	GLFWimage image;
	image.width = icon.Get("width").ToNumber().Int32Value();
	image.height = icon.Get("height").ToNumber().Int32Value();
	image.pixels = reinterpret_cast<unsigned char*>(getData(env, icon));
	
	glfwSetWindowIcon(window, 1, &image);
	RET_UNDEFINED;
	
}


JS_METHOD(getWindowSize) { NAPI_ENV; THIS_WINDOW;
	
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	
	Napi::Object obj = Napi::Object::New(env);
	obj.Set("width", JS_NUM(w));
	obj.Set("height", JS_NUM(h));
	
	RET_VALUE(obj);
	
}


JS_METHOD(setWindowSize) { NAPI_ENV; THIS_WINDOW;
	
	REQ_UINT32_ARG(1, w);
	REQ_UINT32_ARG(2, h);
	
	glfwSetWindowSize(window, w, h);
	RET_UNDEFINED;
	
}


JS_METHOD(setWindowPos) { NAPI_ENV; THIS_WINDOW;
	
	REQ_INT32_ARG(1, x);
	REQ_INT32_ARG(2, y);
	
	glfwSetWindowPos(window, x, y);
	RET_UNDEFINED;
	
}


JS_METHOD(getWindowPos) { NAPI_ENV; THIS_WINDOW;
	
	int xpos, ypos;
	glfwGetWindowPos(window, &xpos, &ypos);
	
	Napi::Object obj = Napi::Object::New(env);
	obj.Set("x", JS_NUM(xpos));
	obj.Set("y", JS_NUM(ypos));
	
	RET_VALUE(obj);
	
}


JS_METHOD(getFramebufferSize) { NAPI_ENV; THIS_WINDOW;
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	Napi::Object obj = Napi::Object::New(env);
	obj.Set("width", JS_NUM(width));
	obj.Set("height", JS_NUM(height));
	
	RET_VALUE(obj);
	
}


JS_METHOD(iconifyWindow) { NAPI_ENV; THIS_WINDOW;
	
	glfwIconifyWindow(window);
	RET_UNDEFINED;
	
}


JS_METHOD(restoreWindow) { NAPI_ENV; THIS_WINDOW;
	
	glfwRestoreWindow(window);
	RET_UNDEFINED;
	
}


JS_METHOD(hideWindow) { NAPI_ENV; THIS_WINDOW;
	
	glfwHideWindow(window);
	RET_UNDEFINED;
	
}


JS_METHOD(showWindow) { NAPI_ENV; THIS_WINDOW;
	
	glfwShowWindow(window);
	RET_UNDEFINED;
	
}


JS_METHOD(windowShouldClose) { NAPI_ENV; THIS_WINDOW;
	
	RET_NUM(glfwWindowShouldClose(window));
	
}


JS_METHOD(setWindowShouldClose) { NAPI_ENV; THIS_WINDOW;
	
	REQ_UINT32_ARG(1, value);
	
	glfwSetWindowShouldClose(window, value);
	RET_UNDEFINED;
	
}


JS_METHOD(getWindowAttrib) { NAPI_ENV; THIS_WINDOW;
	
	REQ_UINT32_ARG(1, attrib);
	
	RET_NUM(glfwGetWindowAttrib(window, attrib));
	
}


JS_METHOD(setInputMode) { NAPI_ENV; THIS_WINDOW;
	
	REQ_INT32_ARG(1, mode);
	REQ_INT32_ARG(2, value);
	
	glfwSetInputMode(window, mode, value);
	RET_UNDEFINED;
	
}


JS_METHOD(pollEvents) { NAPI_ENV;
	
	glfwPollEvents();
	RET_UNDEFINED;
	
}


JS_METHOD(waitEvents) { NAPI_ENV;
	
	glfwWaitEvents();
	RET_UNDEFINED;
	
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


JS_METHOD(getCursorPos) { NAPI_ENV; THIS_WINDOW;
	
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	
	Napi::Object obj = Napi::Object::New(env);
	obj.Set("x", JS_NUM(x));
	obj.Set("y", JS_NUM(y));
	
	RET_VALUE(obj);
	
}


JS_METHOD(setCursorPos) { NAPI_ENV; THIS_WINDOW;
	
	REQ_INT32_ARG(1, x);
	REQ_INT32_ARG(2, y);
	
	glfwSetCursorPos(window, x, y);
	RET_UNDEFINED;
	
}


/* @Module Context handling */
JS_METHOD(makeContextCurrent) { NAPI_ENV; THIS_WINDOW;
	
	glfwMakeContextCurrent(window);
	RET_UNDEFINED;
	
}


JS_METHOD(getCurrentContext) { NAPI_ENV;
	
	GLFWwindow *window = glfwGetCurrentContext();
	
	RET_NUM(reinterpret_cast<uint64_t>(window));
	
}


JS_METHOD(swapBuffers) { NAPI_ENV; THIS_WINDOW;
	
	glfwSwapBuffers(window);
	RET_UNDEFINED;
	
}


JS_METHOD(swapInterval) { NAPI_ENV;
	
	REQ_INT32_ARG(0, interval);
	
	glfwSwapInterval(interval);
	RET_UNDEFINED;
	
}


JS_METHOD(extensionSupported) { NAPI_ENV;
	
	REQ_STR_ARG(0, str);
	
	RET_BOOL(glfwExtensionSupported(str.c_str()) == 1);
	
}


} // namespace glfw
