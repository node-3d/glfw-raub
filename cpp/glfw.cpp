#include <cstdlib>
#include <sstream>
#include <locale.h>

#include "platform.hpp"
#include "win-state.hpp"
#include "events.hpp"
#include "glfw.hpp"


using namespace v8;
using namespace node;
using namespace std;


#define THIS_WINDOW                                                           \
REQ_OFFS_ARG(0, __win_handle);                                                \
GLFWwindow *window = reinterpret_cast<GLFWwindow*>(__win_handle);


namespace glfw {

// The default context for all to share
GLFWwindow *_share = nullptr;

// Cached visibility hint value
bool hintVisible = true;


void errorCb(int error, const char* description) {
	V8_VAR_VAL argv[] = { JS_STR("GLFW Error"), JS_INT(error), JS_STR(description) };
	consoleLog(3, argv);
}


NAN_METHOD(init) {
	
	setlocale(LC_ALL, "");
	
	glfwSetErrorCallback(errorCb);
	
	RET_BOOL(glfwInit() == GLFW_TRUE );
	
}


NAN_METHOD(terminate) {
	
	glfwTerminate();
	
}


NAN_METHOD(getVersion) {
	
	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	
	V8_VAR_OBJ obj = Nan::New<Object>();
	SET_PROP(obj, "major", JS_INT(major));
	SET_PROP(obj, "minor", JS_INT(minor));
	SET_PROP(obj, "rev", JS_INT(rev));
	
	RET_VALUE(obj);
	
}


NAN_METHOD(getVersionString) {
	
	const char *ver = glfwGetVersionString();
	RET_STR(ver);
	
}


NAN_METHOD(getTime) {
	
	RET_NUM(glfwGetTime());
	
}


NAN_METHOD(setTime) {
	
	REQ_DOUBLE_ARG(0, time);
	
	glfwSetTime(time);
	
}


/* TODO: Monitor configuration change callback */

NAN_METHOD(getMonitors) {
	
	int monitor_count, mode_count, xpos, ypos, width, height;
	
	GLFWmonitor **monitors = glfwGetMonitors(&monitor_count);
	GLFWmonitor *primary = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode, *modes;
	
	V8_VAR_ARR js_monitors = Nan::New<Array>(monitor_count);
	V8_VAR_OBJ js_monitor, js_mode;
	V8_VAR_ARR js_modes;
	
	for (int i = 0; i < monitor_count; i++) {
		
		js_monitor = Nan::New<Object>();
		SET_PROP(js_monitor, "is_primary", JS_BOOL(monitors[i] == primary));
		SET_PROP(js_monitor, "index", JS_INT(i));
		SET_PROP(js_monitor, "name", JS_STR(glfwGetMonitorName(monitors[i])));
		
		glfwGetMonitorPos(monitors[i], &xpos, &ypos);
		SET_PROP(js_monitor, "pos_x", JS_INT(xpos));
		SET_PROP(js_monitor, "pos_y", JS_INT(ypos));
		
		glfwGetMonitorPhysicalSize(monitors[i], &width, &height);
		SET_PROP(js_monitor, "width_mm", JS_INT(width));
		SET_PROP(js_monitor, "height_mm", JS_INT(height));
		
		mode = glfwGetVideoMode(monitors[i]);
		SET_PROP(js_monitor, "width", JS_INT(mode->width));
		SET_PROP(js_monitor, "height", JS_INT(mode->height));
		SET_PROP(js_monitor, "rate", JS_INT(mode->refreshRate));
		
		modes = glfwGetVideoModes(monitors[i], &mode_count);
		js_modes = Nan::New<Array>(mode_count);
		
		for (int j = 0; j < mode_count; j++) {
			
			js_mode = Nan::New<Object>();
			SET_PROP(js_mode, "width", JS_INT(modes[j].width));
			SET_PROP(js_mode, "height", JS_INT(modes[j].height));
			SET_PROP(js_mode, "rate", JS_INT(modes[j].refreshRate));
			
			// NOTE: Are color bits necessary?
			js_modes->Set(JS_INT(j), js_mode);
			
		}
		
		SET_PROP(js_monitor, "modes", js_modes);
		js_monitors->Set(JS_INT(i), js_monitor);
		
	}
	
	RET_VALUE(js_monitors);
	
}


NAN_METHOD(testJoystick) {
	
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
	
}


NAN_METHOD(testScene) {
	
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
	
}


NAN_METHOD(windowHint) {
	
	REQ_UINT32_ARG(0, target);
	REQ_UINT32_ARG(1, hint);
	
	if (target == GLFW_VISIBLE) {
		hintVisible = hint != 0;
	}
	
	glfwWindowHint(target, hint);
	
}


NAN_METHOD(defaultWindowHints) {
	
	glfwDefaultWindowHints();
	
}


NAN_METHOD(joystickPresent) {
	
	REQ_UINT32_ARG(0, joy);
	
	bool isPresent = glfwJoystickPresent(joy);
	
	RET_VALUE(JS_BOOL(isPresent));
	
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


NAN_METHOD(getJoystickAxes) {
	
	REQ_UINT32_ARG(0, joy);
	
	int count;
	const float *axisValues = glfwGetJoystickAxes(joy, &count);
	string response = "";
	
	for (int i = 0; i < count; i++) {
		response.append(floatToString(axisValues[i]));
		response.append(","); //Separator
	}
	
	RET_VALUE(JS_STR(response.c_str()));
	
}


NAN_METHOD(getJoystickButtons) {
	
	REQ_UINT32_ARG(0, joy);
	
	int count = 0;
	const unsigned char* response = glfwGetJoystickButtons(joy, &count);
	
	string strResponse = "";
	for (int i = 0; i < count; i++) {
		strResponse.append(buttonToString(response[i]));
		strResponse.append(",");
	}
	
	RET_VALUE(JS_STR(strResponse.c_str()));
	
}


NAN_METHOD(getJoystickName) {
	
	REQ_UINT32_ARG(0, joy);
	
	const char* response = glfwGetJoystickName(joy);
	
	RET_VALUE(JS_STR(response));
	
}


// Name altered due to windows.h collision
NAN_METHOD(createWindow) {
	
	REQ_UINT32_ARG(0, width);
	REQ_UINT32_ARG(1, height);
	REQ_OBJ_ARG(2, emitter);
	LET_UTF8_ARG(3, str);
	LET_INT32_ARG(4, monitor_idx);
	
	GLFWmonitor **monitors = nullptr;
	GLFWmonitor *monitor = nullptr;
	int monitor_count;
	
	if (info.Length() >= 5 && monitor_idx >= 0) {
		monitors = glfwGetMonitors(&monitor_count);
		if (monitor_idx >= monitor_count) {
			return Nan::ThrowError("Invalid monitor");
		}
		monitor = monitors[monitor_idx];
	}
	
	if ( ! _share ) {
		glfwWindowHint(GLFW_VISIBLE, false);
		_share = glfwCreateWindow(128, 128, "_GLFW_ROOT_SHARED", nullptr, nullptr);
		glfwWindowHint(GLFW_VISIBLE, hintVisible);
	}
	
	GLFWwindow *window = glfwCreateWindow(width, height, *str, monitor, _share);
	
	if ( ! window ) {
		// can't create window, throw error
		return Nan::ThrowError("Can't create GLFW window");
	}
	
	glfwMakeContextCurrent(window);
	
	// make sure cursor is always shown
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	
	// Store WinState as user pointer
	WinState *state = new WinState(emitter);
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
	
	RET_VALUE(JS_OFFS(reinterpret_cast<uint64_t>(window)));
	
}


NAN_METHOD(platformWindow) { THIS_WINDOW;
	
#ifdef _WIN32
	RET_VALUE(JS_OFFS(reinterpret_cast<uint64_t>(glfwGetWin32Window(window))));
#elif __linux__
	RET_VALUE(JS_OFFS(reinterpret_cast<uint64_t>(glfwGetX11Window(window))));
#elif __APPLE__
	RET_VALUE(JS_OFFS(reinterpret_cast<uint64_t>(glfwGetCocoaWindow(window))));
#endif
	
}


NAN_METHOD(platformContext) { THIS_WINDOW;
	
#ifdef _WIN32
	RET_VALUE(JS_OFFS(reinterpret_cast<uint64_t>(glfwGetWGLContext(window))));
#elif __linux__
	RET_VALUE(JS_OFFS(reinterpret_cast<uint64_t>(glfwGetGLXContext(window))));
#elif __APPLE__
	RET_VALUE(JS_OFFS(reinterpret_cast<uint64_t>(glfwGetNSGLContext(window))));
#endif
	
}


NAN_METHOD(destroyWindow) { THIS_WINDOW;
	
	WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));
	delete state;
	
	glfwDestroyWindow(window);
	
}


NAN_METHOD(setWindowTitle) { THIS_WINDOW;
	
	REQ_UTF8_ARG(1, str);
	
	glfwSetWindowTitle(window, *str);
	
}


NAN_METHOD(setWindowIcon) { THIS_WINDOW;
	
	REQ_OBJ_ARG(1, icon);
	
	GLFWimage image;
	image.width = icon->Get(JS_STR("width")).As<v8::Int32>()->Value();
	image.height = icon->Get(JS_STR("height")).As<v8::Int32>()->Value();
	image.pixels = reinterpret_cast<unsigned char*>(getData(icon));
	
	glfwSetWindowIcon(window, 1, &image);
	
}


NAN_METHOD(getWindowSize) { THIS_WINDOW;
	
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	
	V8_VAR_OBJ obj = Nan::New<Object>();
	SET_PROP(obj, "width", JS_INT(w));
	SET_PROP(obj, "height", JS_INT(h));
	
	RET_VALUE(obj);
	
}


NAN_METHOD(setWindowSize) { THIS_WINDOW;
	
	REQ_UINT32_ARG(1, w);
	REQ_UINT32_ARG(2, h);
	
	glfwSetWindowSize(window, w, h);
	
}


NAN_METHOD(setWindowPos) { THIS_WINDOW;
	
	REQ_INT32_ARG(1, x);
	REQ_INT32_ARG(2, y);
	
	glfwSetWindowPos(window, x, y);
	
}


NAN_METHOD(getWindowPos) { THIS_WINDOW;
	
	int xpos, ypos;
	glfwGetWindowPos(window, &xpos, &ypos);
	
	V8_VAR_OBJ obj = Nan::New<Object>();
	SET_PROP(obj, "x", JS_INT(xpos));
	SET_PROP(obj, "y", JS_INT(ypos));
	
	RET_VALUE(obj);
	
}


NAN_METHOD(getFramebufferSize) { THIS_WINDOW;
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	V8_VAR_OBJ obj = Nan::New<Object>();
	SET_PROP(obj, "width", JS_INT(width));
	SET_PROP(obj, "height", JS_INT(height));
	
	RET_VALUE(obj);
	
}


NAN_METHOD(iconifyWindow) { THIS_WINDOW;
	
	glfwIconifyWindow(window);
	
}


NAN_METHOD(restoreWindow) { THIS_WINDOW;
	
	glfwRestoreWindow(window);
	
}


NAN_METHOD(hideWindow) { THIS_WINDOW;
	
	glfwHideWindow(window);
	
}


NAN_METHOD(showWindow) { THIS_WINDOW;
	
	glfwShowWindow(window);
	
}


NAN_METHOD(windowShouldClose) { THIS_WINDOW;
	
	RET_VALUE(JS_INT(glfwWindowShouldClose(window)));
	
}


NAN_METHOD(setWindowShouldClose) { THIS_WINDOW;
	
	REQ_UINT32_ARG(1, value);
	
	glfwSetWindowShouldClose(window, value);
	
}


NAN_METHOD(getWindowAttrib) { THIS_WINDOW;
	
	REQ_UINT32_ARG(1, attrib);
	
	RET_VALUE(JS_INT(glfwGetWindowAttrib(window, attrib)));
	
}


NAN_METHOD(setInputMode) { THIS_WINDOW;
	
	REQ_INT32_ARG(1, mode);
	REQ_INT32_ARG(2, value);
	
	glfwSetInputMode(window, mode, value);
	
}


NAN_METHOD(pollEvents) {
	
	glfwPollEvents();
	
}


NAN_METHOD(waitEvents) {
	
	glfwWaitEvents();
	
}


/* Input handling */
NAN_METHOD(getKey) { THIS_WINDOW;
	
	REQ_UINT32_ARG(1, key);
	
	RET_VALUE(JS_INT(glfwGetKey(window, key)));
	
}


NAN_METHOD(getMouseButton) { THIS_WINDOW;
	
	REQ_UINT32_ARG(1, button);
	
	RET_VALUE(JS_INT(glfwGetMouseButton(window, button)));
	
}


NAN_METHOD(getCursorPos) { THIS_WINDOW;
	
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	
	V8_VAR_OBJ obj = Nan::New<Object>();
	SET_PROP(obj, "x", JS_NUM(x));
	SET_PROP(obj, "y", JS_NUM(y));
	
	RET_VALUE(obj);
	
}


NAN_METHOD(setCursorPos) { THIS_WINDOW;
	
	REQ_INT32_ARG(1, x);
	REQ_INT32_ARG(2, y);
	
	glfwSetCursorPos(window, x, y);
	
}


/* @Module Context handling */
NAN_METHOD(makeContextCurrent) { THIS_WINDOW;
	
	glfwMakeContextCurrent(window);
	
}


NAN_METHOD(getCurrentContext) {
	
	GLFWwindow *window = glfwGetCurrentContext();
	
	RET_OFFS(reinterpret_cast<uint64_t>(window));
	
}


NAN_METHOD(swapBuffers) { THIS_WINDOW;
	
	glfwSwapBuffers(window);
	
}


NAN_METHOD(swapInterval) {
	
	REQ_INT32_ARG(0, interval);
	
	glfwSwapInterval(interval);
	
}


NAN_METHOD(extensionSupported) {
	
	REQ_UTF8_ARG(0, str);
	
	RET_BOOL(glfwExtensionSupported(*str) == 1);
	
}


// Cleanup resources
void deinit() {
	glfwTerminate();
}


} // namespace glfw
