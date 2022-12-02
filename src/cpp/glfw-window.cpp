#include <sstream>
#include <iostream>
#include <locale.h>

#include "glfw-common.hpp"
#include "glfw-monitors.hpp"
#include "glfw-platform.hpp"
#include "glfw-events.hpp"
#include "glfw-window.hpp"


namespace glfw {

std::vector<WinState *> states;

// Cached visibility hint value
bool isHintVisible = true;


// The default context for all to share
GLFWwindow *_share = nullptr;


void dropShare() {
	_share = nullptr;
}


void forEachWindow(const StatesIterator& fn) {
	size_t stateNum = states.size();
	
	std::for_each(
		states.begin(),
		states.end(),
		[&](WinState *state) {
			if (!state->window) {
				return;
			}
			fn(state);
		}
	);
}


// Cleanup resources
void destroyAllWindows() {
	// Clear all callbacks
	forEachWindow([](WinState *state) {
		GLFWwindow *window = state->window;
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
	});
	
	// Destroy all windows
	forEachWindow([](WinState *state) {
		glfwDestroyWindow(state->window);
		state->window = nullptr;
	});
	
	states.clear();
}


JS_METHOD(windowHint) { NAPI_ENV;
	REQ_UINT32_ARG(0, hint);
	REQ_UINT32_ARG(1, value);
	
	if (hint == GLFW_VISIBLE) {
		isHintVisible = value != 0;
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
	
	if (!_share) {
		glfwWindowHint(GLFW_VISIBLE, false);
		_share = glfwCreateWindow(128, 128, "_GLFW_ROOT_SHARED", nullptr, nullptr);
		glfwWindowHint(GLFW_VISIBLE, isHintVisible);
	}
	
	GLFWwindow *window = glfwCreateWindow(
		width,
		height,
		str.c_str(),
		monitor,
		_share
	);
	
	if ( ! window ) {
		JS_THROW("Can't create GLFW window");
		RET_NULL;
	}
	
	glfwMakeContextCurrent(window);
	
	// make sure cursor is always shown
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	emitter.Set("__mouse", Napi::Object::New(env));
	emitter.Set("__key", Napi::Object::New(env));
	
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
	
	if (!(icon.Has("width") && icon.Has("height"))) {
		RET_UNDEFINED;
	}
	
	bool noflip = icon.Has("noflip") ? icon.Get("noflip").ToBoolean().Value() : false;
	
	GLFWimage image;
	image.width = icon.Get("width").ToNumber().Int32Value();
	image.height = icon.Get("height").ToNumber().Int32Value();
	
	uint8_t *src = reinterpret_cast<unsigned char*>(getData(env, icon));
	
	if (noflip == false) {
		uint8_t *dest = new uint8_t[image.width * image.height * 4];
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
	
	Napi::Object jsFrame = Napi::Object::New(env);
	
	jsFrame.Set("left", left);
	jsFrame.Set("top", top);
	jsFrame.Set("right", right);
	jsFrame.Set("bottom", bottom);
	
	RET_VALUE(jsFrame);
}


JS_METHOD(getWindowContentScale) { NAPI_ENV; THIS_WINDOW;
	float xscale, yscale;
	glfwGetWindowContentScale(window, &xscale, &yscale);
	
	Napi::Object jsScale = Napi::Object::New(env);
	
	jsScale.Set("xscale", xscale);
	jsScale.Set("yscale", yscale);
	
	RET_VALUE(jsScale);
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


} // namespace glfw
