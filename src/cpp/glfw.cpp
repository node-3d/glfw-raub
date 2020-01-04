#include <sstream>
#include <iostream>
#include <locale.h>

#include "platform.hpp"
#include "win-state.hpp"
#include "events.hpp"
#include "glfw.hpp"


#define THIS_WINDOW                                                           \
	REQ_OFFS_ARG(0, __win_handle);                                            \
	GLFWwindow *window = reinterpret_cast<GLFWwindow*>(__win_handle);


namespace glfw {


std::string intToString(int number) {
	
	std::ostringstream buff;
	buff << number;
	return buff.str();
	
}


std::string floatToString(float number) {
	
	std::ostringstream buff;
	buff << number;
	return buff.str();
	
}


std::string buttonToString(unsigned char c) {
	
	int number = static_cast<int>(c);
	return intToString(number);
	
}


// The default context for all to share
GLFWwindow *_share = nullptr;

// Cached visibility hint value
bool hintVisible = true;

std::vector<WinState *> states;

bool isInited = false;


void errorCb(int error, const char* description) {
	std::cout << "GLFW Error " << error << ": " << description << std::endl;
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
	}
	
	for (int i = states.size() - 1; i >= 0; i--) {
		WinState *state = states[i];
		GLFWwindow *window = state->window;
		if ( ! window ) {
			continue;
		}
		// Destroy
		glfwDestroyWindow(window);
		state->window = nullptr;
		delete state;
	}
	
	states.clear();
	isInited = false;
	_share = nullptr;
	
	glfwTerminate();
	
}


JS_METHOD(init) { NAPI_ENV;
	
	setlocale(LC_ALL, "");
	
	glfwSetErrorCallback(errorCb);
	
	isInited = glfwInit() == GLFW_TRUE;
	RET_BOOL(isInited);
	
}


JS_METHOD(initHint) { NAPI_ENV;
	
	REQ_INT32_ARG(0, hint);
	REQ_INT32_ARG(1, value);
	
	glfwInitHint(hint, value);
	RET_UNDEFINED;
	
}


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


JS_METHOD(getError) { NAPI_ENV;
	
	const char *err;
	int code = glfwGetError(&err);
	
	if (code != GLFW_NO_ERROR) {
		RET_STR(err);
	}
	
	RET_NULL;
	
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
	
	if ( ! primary ) {
		RET_NULL;
	}
	
	RET_VALUE(describeMonitor(env, primary, true));
	
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
	
	REQ_UINT32_ARG(0, hint);
	REQ_UINT32_ARG(1, value);
	
	if (hint == GLFW_VISIBLE) {
		hintVisible = value != 0;
	}
	
	glfwWindowHint(hint, value);
	RET_UNDEFINED;
	
}


JS_METHOD(windowHintString) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, hint);
	REQ_STR_ARG(1, value);
	
	glfwWindowHintString(hint, value.c_str());
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


JS_METHOD(getJoystickAxes) { NAPI_ENV;
	
	REQ_UINT32_ARG(0, joy);
	
	int count;
	const float *axisValues = glfwGetJoystickAxes(joy, &count);
	std::string response = "";
	
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
	
	std::string strResponse = "";
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
	int monitorCount;
	
	if (info.Length() >= 5 && monitor_idx >= 0) {
		monitors = glfwGetMonitors(&monitorCount);
		if (monitor_idx >= monitorCount) {
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
	glfwDestroyWindow(window);
	state->window = nullptr;
	
	RET_UNDEFINED;
	
}


JS_METHOD(setWindowTitle) { NAPI_ENV; THIS_WINDOW;
	
	REQ_STR_ARG(1, str);
	
	glfwSetWindowTitle(window, str.c_str());
	RET_UNDEFINED;
	
}


JS_METHOD(setWindowIcon) { NAPI_ENV; THIS_WINDOW;
	
	REQ_OBJ_ARG(1, icon);
	
	if ( ! (icon.Has("width") && icon.Has("height")) ) {
		RET_UNDEFINED;
	}
	
	bool noflip = icon.Has("noflip") ? icon.Get("noflip").ToBoolean().Value() : false;
	
	GLFWimage image;
	image.width = icon.Get("width").ToNumber().Int32Value();
	image.height = icon.Get("height").ToNumber().Int32Value();
	
	uint8_t *src = reinterpret_cast<unsigned char*>(getData(env, icon));
	
	if (noflip == false) {
		uint8_t *dest = new uint8_t [image.width * image.height * 4];
		int32_t lastY = image.height - 1;
		for (int32_t y = 0; y < image.height; y++) {
			for (int32_t x = 0; x < image.width; x++) {
				int32_t iForward = (y * image.width + x) << 2;
				int32_t iBackward = ((lastY - y) * image.width + x) << 2;
				dest[iForward + 0] = src[iBackward + 0];
				dest[iForward + 1] = src[iBackward + 1];
				dest[iForward + 2] = src[iBackward + 2];
				dest[iForward + 3] = src[iBackward + 3];
			}
		}
		image.pixels = dest;
		glfwSetWindowIcon(window, 1, &image);
		delete [] dest;
	} else {
		image.pixels = src;
		glfwSetWindowIcon(window, 1, &image);
	}
	
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


JS_METHOD(getWindowFrameSize) { NAPI_ENV; THIS_WINDOW;
	
	int left, top, right, bottom;
	glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
	
	Napi::Object jsMonitor = Napi::Object::New(env);
	
	jsMonitor.Set("left", left);
	jsMonitor.Set("top", top);
	jsMonitor.Set("right", right);
	jsMonitor.Set("bottom", bottom);
	
	RET_VALUE(jsMonitor);
	
}


JS_METHOD(getWindowContentScale) { NAPI_ENV; THIS_WINDOW;
	
	float xscale, yscale;
	glfwGetWindowContentScale(window, &xscale, &yscale);
	
	Napi::Object jsMonitor = Napi::Object::New(env);
	
	jsMonitor.Set("xscale", xscale);
	jsMonitor.Set("yscale", yscale);
	
	RET_VALUE(jsMonitor);
	
}


JS_METHOD(setWindowSize) { NAPI_ENV; THIS_WINDOW;
	
	REQ_UINT32_ARG(1, w);
	REQ_UINT32_ARG(2, h);
	
	glfwSetWindowSize(window, w, h);
	RET_UNDEFINED;
	
}


JS_METHOD(setWindowSizeLimits) { NAPI_ENV; THIS_WINDOW;
	
	REQ_UINT32_ARG(1, minwidth);
	REQ_UINT32_ARG(2, minheight);
	REQ_UINT32_ARG(3, maxwidth);
	REQ_UINT32_ARG(4, maxheight);
	
	glfwSetWindowSizeLimits(window, minwidth, minheight, maxwidth, maxheight);
	RET_UNDEFINED;
	
}


JS_METHOD(setWindowAspectRatio) { NAPI_ENV; THIS_WINDOW;
	
	USE_UINT32_ARG(1, numer, GLFW_DONT_CARE);
	USE_UINT32_ARG(2, denom, GLFW_DONT_CARE);
	
	glfwSetWindowAspectRatio(window, numer, denom);
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


JS_METHOD(getWindowOpacity) { NAPI_ENV; THIS_WINDOW;
	
	float opacity = glfwGetWindowOpacity(window);
	RET_NUM(opacity);
	
}


JS_METHOD(setWindowOpacity) { NAPI_ENV; THIS_WINDOW;
	
	REQ_FLOAT_ARG(1, opacity);
	
	glfwSetWindowOpacity(window, opacity);
	RET_UNDEFINED;
	
}


JS_METHOD(maximizeWindow) { NAPI_ENV; THIS_WINDOW;
	
	glfwMaximizeWindow(window);
	RET_UNDEFINED;
	
}


JS_METHOD(focusWindow) { NAPI_ENV; THIS_WINDOW;
	
	glfwFocusWindow(window);
	RET_UNDEFINED;
	
}


JS_METHOD(requestWindowAttention) { NAPI_ENV; THIS_WINDOW;
	
	glfwRequestWindowAttention(window);
	RET_UNDEFINED;
	
}


JS_METHOD(getWindowMonitor) { NAPI_ENV; THIS_WINDOW;
	
	GLFWmonitor *monitor = glfwGetWindowMonitor(window);
	
	if ( ! monitor ) {
		RET_NULL;
	}
	
	GLFWmonitor *primary = glfwGetPrimaryMonitor();
	
	RET_VALUE(describeMonitor(env, monitor, primary ? true : monitor == primary));
	
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


JS_METHOD(setWindowAttrib) { NAPI_ENV; THIS_WINDOW;
	
	REQ_UINT32_ARG(1, attrib);
	REQ_UINT32_ARG(2, value);
	
	glfwSetWindowAttrib(window, attrib, value);
	
	RET_UNDEFINED;
	
}


JS_METHOD(setInputMode) { NAPI_ENV; THIS_WINDOW;
	
	REQ_INT32_ARG(1, mode);
	REQ_INT32_ARG(2, value);
	
	glfwSetInputMode(window, mode, value);
	RET_UNDEFINED;
	
}

JS_METHOD(getInputMode) { NAPI_ENV; THIS_WINDOW;
	
	REQ_INT32_ARG(1, mode);
	
	RET_NUM(glfwGetInputMode(window, mode));
	
}


JS_METHOD(pollEvents) { NAPI_ENV;
	
	glfwPollEvents();
	RET_UNDEFINED;
	
}


JS_METHOD(waitEvents) { NAPI_ENV;
	
	glfwWaitEvents();
	RET_UNDEFINED;
	
}


JS_METHOD(waitEventsTimeout) { NAPI_ENV;
	
	REQ_DOUBLE_ARG(1, timeout);
	
	glfwWaitEventsTimeout(timeout);
	RET_UNDEFINED;
	
}


JS_METHOD(postEmptyEvent) { NAPI_ENV;
	
	glfwPostEmptyEvent();
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
	
	RET_BOOL(glfwExtensionSupported(str.c_str()) == GLFW_TRUE);
	
}


JS_METHOD(rawMouseMotionSupported) { NAPI_ENV;
	
	RET_BOOL(glfwRawMouseMotionSupported() == GLFW_TRUE);
	
}


JS_METHOD(getKeyName) { NAPI_ENV;
	
	REQ_INT32_ARG(0, key);
	REQ_INT32_ARG(1, scancode);
	
	RET_STR(glfwGetKeyName(key, scancode));
	
}


JS_METHOD(getKeyScancode) { NAPI_ENV;
	
	REQ_INT32_ARG(0, key);
	
	RET_NUM(glfwGetKeyScancode(key));
	
}


// TODO
// JS_METHOD(createCursor) { NAPI_ENV; THIS_WINDOW;
// 	glfwCreateCursor(const GLFWimage* image, int xhot, int yhot);
// }
// JS_METHOD(createStandardCursor) { NAPI_ENV; THIS_WINDOW;
// 	glfwCreateStandardCursor(int shape);
// }
// JS_METHOD(destroyCursor) { NAPI_ENV; THIS_WINDOW;
// 	glfwDestroyCursor(GLFWcursor* cursor);
// }
// JS_METHOD(setCursor) { NAPI_ENV; THIS_WINDOW;
// 	glfwSetCursor(GLFWwindow* window, GLFWcursor* cursor);
// }


JS_METHOD(getJoystickHats) { NAPI_ENV;
	
	REQ_INT32_ARG(0, jid);
	
	int count;
	const unsigned char *hats = glfwGetJoystickHats(jid, &count);
	
	Napi::Array jsHats = Napi::Array::New(env);
	
	if ( ! hats ) {
		RET_VALUE(jsHats);
	}
	
	for (int i = 0; i < count; i++) {
		jsHats.Set(i, JS_NUM(hats[i]));
	}
	
	RET_VALUE(jsHats);
	
}


JS_METHOD(getJoystickGUID) { NAPI_ENV;
	
	REQ_INT32_ARG(0, jid);
	
	const char *guid = glfwGetJoystickGUID(jid);
	
	if ( ! guid ) {
		RET_NULL;
	}
	
	RET_STR(guid);
	
}


JS_METHOD(joystickIsGamepad) { NAPI_ENV;
	
	REQ_INT32_ARG(0, jid);
	
	RET_BOOL(glfwJoystickIsGamepad(jid) == GLFW_TRUE);
	
}


JS_METHOD(updateGamepadMappings) { NAPI_ENV;
	
	REQ_STR_ARG(0, mappings);
	// https://www.glfw.org/docs/latest/input_guide.html#gamepad_mapping
	RET_BOOL(glfwUpdateGamepadMappings(mappings.c_str()) == GLFW_TRUE);
	
}


JS_METHOD(getGamepadName) { NAPI_ENV;
	
	REQ_INT32_ARG(0, jid);
	
	const char *name = glfwGetGamepadName(jid);
	
	if ( ! name ) {
		RET_NULL;
	}
	
	RET_STR(name);
	
}


JS_METHOD(getGamepadState) { NAPI_ENV;
	
	REQ_INT32_ARG(0, jid);
	
	GLFWgamepadstate state;
	if (glfwGetGamepadState(jid, &state) != GLFW_TRUE) {
		RET_NULL;
	}
	
	Napi::Object jsState = Napi::Object::New(env);
	Napi::Array jsButtons = Napi::Array::New(env);
	Napi::Array jsAxes = Napi::Array::New(env);
	
	for (int j = 0; j < 15; j++) {
		jsButtons.Set(j, JS_NUM(state.buttons[j]));
	}
	
	for (int j = 0; j < 6; j++) {
		jsAxes.Set(j, JS_NUM(state.axes[j]));
	}
	
	jsState.Set("buttons", jsButtons);
	jsState.Set("axes", jsAxes);
	
	RET_VALUE(jsState);
	
}


JS_METHOD(setClipboardString) { NAPI_ENV; THIS_WINDOW;
	
	REQ_STR_ARG(1, str);
	
	glfwSetClipboardString(window, str.c_str());
	RET_UNDEFINED;
	
}


JS_METHOD(getClipboardString) { NAPI_ENV; THIS_WINDOW;
	
	const char *str = glfwGetClipboardString(window);
	
	if ( ! str ) {
		RET_STR("");
	}
	
	RET_STR(str);
	
}


JS_METHOD(getTimerValue) { NAPI_ENV;
	
	RET_NUM(glfwGetTimerValue());
	
}


JS_METHOD(getTimerFrequency) { NAPI_ENV;
	
	RET_NUM(glfwGetTimerFrequency());
	
}


} // namespace glfw
