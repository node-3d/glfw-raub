#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>

#include "common.hpp"

using namespace v8;
using namespace node;
using namespace std;

#define JS_GLFW_CONSTANT(name) target->Set(JS_STR( #name ), JS_INT(GLFW_ ## name))
#define JS_GLFW_SET_METHOD(name) Nan::SetMethod(target, #name , glfw::name);

#define THIS_WINDOW                                                           \
REQ_OFFS_ARG(0, __win_handle);                                                \
GLFWwindow *window = reinterpret_cast<GLFWwindow*>(__win_handle);



namespace glfw {

// Window info
struct WinState {
	
	int mouseX;
	int mouseY;
	Nan::Persistent<Object> events;
	
	WinState() {
		mouseX = 0;
		mouseY = 0;
	}
	
	const WinState & operator = (const WinState &other) {
		mouseX = other.mouseX;
		mouseY = other.mouseY;
		return *this;
	}
	
};

std::map<GLFWwindow *, WinState> states;


NAN_METHOD(init) {
	
	RET_VALUE(JS_BOOL(glfwInit() == 1));
	
}


NAN_METHOD(terminate) {
	
	glfwTerminate();
	
}


NAN_METHOD(getVersion) {
	
	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	
	Local<Object> obj = Nan::New<Object>();
	SET_PROP(obj, "major", JS_INT(major));
	SET_PROP(obj, "minor", JS_INT(minor));
	SET_PROP(obj, "rev", JS_INT(rev));
	
	RET_VALUE(obj);
	
}


NAN_METHOD(getVersionString) {
	
	const char *ver = glfwGetVersionString();
	RET_VALUE(JS_STR(ver));
	
}


NAN_METHOD(getTime) {
	
	RET_VALUE(JS_NUM(glfwGetTime()));
	
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
	
	Local<Array> js_monitors = Nan::New<Array>(monitor_count);
	Local<Object> js_monitor, js_mode;
	Local<Array> js_modes;
	
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
		
		for(int j = 0; j < mode_count; j++){
			
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


void NAN_INLINE(_emit(GLFWwindow *window, int argc, Local<Value> argv[])) { NAN_HS;
	
	WinState &state = states[window];
	
	if (Nan::New(state.events)->Has(JS_STR("emit"))) {
		
		Nan::Callback callback(Nan::New(state.events)->Get(JS_STR("emit")).As<Function>());
		
		if ( ! callback.IsEmpty() ) {
			callback.Call(argc, argv);
		}
		
	}
	
}


void windowPosCB(GLFWwindow *window, int xpos, int ypos) { NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("window_pos"));
	SET_PROP(evt, "x", JS_INT(xpos));
	SET_PROP(evt, "y", JS_INT(ypos));
	
	Local<Value> argv[2] = { JS_STR("window_pos"), evt };
	_emit(window, 2, argv);
	
}


void windowSizeCB(GLFWwindow *window, int w, int h) { NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("resize"));
	SET_PROP(evt, "width", JS_INT(w));
	SET_PROP(evt, "height", JS_INT(h));
	
	Local<Value> argv[2] = { JS_STR("resize"), evt };
	_emit(window, 2, argv);
	
}


void windowFramebufferSizeCB(GLFWwindow *window, int w, int h) { NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("framebuffer_resize"));
	SET_PROP(evt, "width", JS_INT(w));
	SET_PROP(evt, "height", JS_INT(h));
	
	Local<Value> argv[2] = { JS_STR("framebuffer_resize"), evt };
	_emit(window, 2, argv);
	
}


void windowDropCB(GLFWwindow *window, int count, const char **paths) { NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("drop"));
	
	Local<Array> list = Nan::New<Array>(count);
	for (int i = 0; i < count; i++) {
		list->Set(i, JS_STR(paths[i]));
	}
	
	SET_PROP(evt, "list", list);
	
	Local<Value> argv[2] = { JS_STR("drop"), evt };
	_emit(window, 2, argv);
	
}


void windowCloseCB(GLFWwindow *window) { NAN_HS;
	
	Local<Value> argv[1] = { JS_STR("quit") };
	_emit(window, 1, argv);
	
}


void windowRefreshCB(GLFWwindow *window) { NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("refresh"));
	SET_PROP(evt, "window", JS_NUM((uint64_t) window));
	
	Local<Value> argv[2] = { JS_STR("refresh"), evt };
	_emit(window, 2, argv);
	
}


void windowIconifyCB(GLFWwindow *window, int iconified) { NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("iconified"));
	SET_PROP(evt, "iconified", JS_BOOL(iconified));
	
	Local<Value> argv[2] = { JS_STR("iconified"), evt };
	_emit(window, 2, argv);
	
}


void windowFocusCB(GLFWwindow *window, int focused) { NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("focused"));
	SET_PROP(evt, "focused", JS_BOOL(focused));
	
	Local<Value> argv[2] = { JS_STR("focused"), evt };
	_emit(window, 2, argv);
	
}


void keyCB(GLFWwindow *window, int key, int scancode, int action, int mods) { NAN_HS;
	
	const char *actionNames = "keyup\0  keydown\0keypress";
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR( &actionNames[action << 3] ));
	SET_PROP(evt, "ctrlKey", JS_BOOL(mods & GLFW_MOD_CONTROL));
	SET_PROP(evt, "shiftKey", JS_BOOL(mods & GLFW_MOD_SHIFT));
	SET_PROP(evt, "altKey", JS_BOOL(mods & GLFW_MOD_ALT));
	SET_PROP(evt, "metaKey", JS_BOOL(mods & GLFW_MOD_SUPER));
	
	int which = key, charCode = key;
	
	switch (key) {
		case GLFW_KEY_ESCAPE:       key = 27; break;
		case GLFW_KEY_ENTER:        key = 13; break;
		case GLFW_KEY_TAB:          key = 9; break;
		case GLFW_KEY_BACKSPACE:    key = 8; break;
		case GLFW_KEY_INSERT:       key = 45; break;
		case GLFW_KEY_DELETE:       key = 46; break;
		case GLFW_KEY_RIGHT:        key = 39; break;
		case GLFW_KEY_LEFT:         key = 37; break;
		case GLFW_KEY_DOWN:         key = 40; break;
		case GLFW_KEY_UP:           key = 38; break;
		case GLFW_KEY_PAGE_UP:      key = 33; break;
		case GLFW_KEY_PAGE_DOWN:    key = 34; break;
		case GLFW_KEY_HOME:         key = 36; break;
		case GLFW_KEY_END:          key = 35; break;
		case GLFW_KEY_CAPS_LOCK:    key = 20; break;
		case GLFW_KEY_SCROLL_LOCK:  key = 145; break;
		case GLFW_KEY_NUM_LOCK:     key = 144; break;
		case GLFW_KEY_PRINT_SCREEN: key = 144; break; /* TODO */
		case GLFW_KEY_PAUSE:        key = 19; break;
		case GLFW_KEY_F1:           key = 112; break;
		case GLFW_KEY_F2:           key = 113; break;
		case GLFW_KEY_F3:           key = 114; break;
		case GLFW_KEY_F4:           key = 115; break;
		case GLFW_KEY_F5:           key = 116; break;
		case GLFW_KEY_F6:           key = 117; break;
		case GLFW_KEY_F7:           key = 118; break;
		case GLFW_KEY_F8:           key = 119; break;
		case GLFW_KEY_F9:           key = 120; break;
		case GLFW_KEY_F10:          key = 121; break;
		case GLFW_KEY_F11:          key = 122; break;
		case GLFW_KEY_F12:          key = 123; break;
		case GLFW_KEY_F13:          key = 123; break; /* unknown */
		case GLFW_KEY_F14:          key = 123; break; /* unknown */
		case GLFW_KEY_F15:          key = 123; break; /* unknown */
		case GLFW_KEY_F16:          key = 123; break; /* unknown */
		case GLFW_KEY_F17:          key = 123; break; /* unknown */
		case GLFW_KEY_F18:          key = 123; break; /* unknown */
		case GLFW_KEY_F19:          key = 123; break; /* unknown */
		case GLFW_KEY_F20:          key = 123; break; /* unknown */
		case GLFW_KEY_F21:          key = 123; break; /* unknown */
		case GLFW_KEY_F22:          key = 123; break; /* unknown */
		case GLFW_KEY_F23:          key = 123; break; /* unknown */
		case GLFW_KEY_F24:          key = 123; break; /* unknown */
		case GLFW_KEY_F25:          key = 123; break; /* unknown */
		case GLFW_KEY_KP_0:         key = 96; break;
		case GLFW_KEY_KP_1:         key = 97; break;
		case GLFW_KEY_KP_2:         key = 98; break;
		case GLFW_KEY_KP_3:         key = 99; break;
		case GLFW_KEY_KP_4:         key = 100; break;
		case GLFW_KEY_KP_5:         key = 101; break;
		case GLFW_KEY_KP_6:         key = 102; break;
		case GLFW_KEY_KP_7:         key = 103; break;
		case GLFW_KEY_KP_8:         key = 104; break;
		case GLFW_KEY_KP_9:         key = 105; break;
		case GLFW_KEY_KP_DECIMAL:   key = 110; break;
		case GLFW_KEY_KP_DIVIDE:    key = 111; break;
		case GLFW_KEY_KP_MULTIPLY:  key = 106; break;
		case GLFW_KEY_KP_SUBTRACT:  key = 109; break;
		case GLFW_KEY_KP_ADD:       key = 107; break;
		case GLFW_KEY_KP_ENTER:     key = 13; break;
		case GLFW_KEY_KP_EQUAL:     key = 187; break;
		case GLFW_KEY_LEFT_SHIFT:   key = 16; break;
		case GLFW_KEY_LEFT_CONTROL: key = 17; break;
		case GLFW_KEY_LEFT_ALT:     key = 18; break;
		case GLFW_KEY_LEFT_SUPER:   key = 91; break;
		case GLFW_KEY_RIGHT_SHIFT:  key = 16; break;
		case GLFW_KEY_RIGHT_CONTROL: key = 17; break;
		case GLFW_KEY_RIGHT_ALT:    key = 18; break;
		case GLFW_KEY_RIGHT_SUPER:  key = 93; break;
		case GLFW_KEY_MENU:         key = 18; break;
		case GLFW_KEY_SEMICOLON:    key = 186; break; // ;
		case GLFW_KEY_EQUAL:        key = 187; break; // =
		case GLFW_KEY_COMMA:        key = 188; break; // ,
		case GLFW_KEY_MINUS:        key = 189; break; // -
		case GLFW_KEY_PERIOD:       key = 190; break; // .
		case GLFW_KEY_SLASH:        key = 191; break; // /
		case GLFW_KEY_GRAVE_ACCENT: key = 192; break; // `
		case GLFW_KEY_LEFT_BRACKET: key = 219; break; // [
		case GLFW_KEY_BACKSLASH:    key = 220; break; /* \ */
		case GLFW_KEY_RIGHT_BRACKET: key = 221; break; // ]
		case GLFW_KEY_APOSTROPHE:   key = 222; break; // '
	}
	
	SET_PROP(evt, "which", JS_INT(which));
	SET_PROP(evt, "keyCode", JS_INT(key));
	SET_PROP(evt, "charCode", JS_INT(charCode));
	
	Local<Value> argv[2] = { JS_STR(&actionNames[action << 3]), evt };
	_emit(window, 2, argv);
	
}


void cursorPosCB(GLFWwindow* window, double x, double y) {
	
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	
	if (x < 0 || x >= w || y < 0 || y >= h) {
		return;
	}
	
	WinState &state = states[window];
	
	state.mouseX = static_cast<int>(x);
	state.mouseY = static_cast<int>(y);
	
	NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	
	SET_PROP(evt, "type", JS_STR("mousemove"));
	SET_PROP(evt, "clientX", JS_NUM(x));
	SET_PROP(evt, "clientY", JS_NUM(y));
	SET_PROP(evt, "pageX", JS_NUM(x));
	SET_PROP(evt, "pageY", JS_NUM(y));
	SET_PROP(evt, "ctrlKey", JS_BOOL(
		glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS
	));
	SET_PROP(evt, "shiftKey", JS_BOOL(
		glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS
	));
	SET_PROP(evt, "altKey", JS_BOOL(
		glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS
	));
	SET_PROP(evt, "metaKey", JS_BOOL(
		glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS
	));
	SET_PROP(evt, "x", JS_NUM(x));
	SET_PROP(evt, "y", JS_NUM(y));
	
	Local<Value> argv[2] = { JS_STR("mousemove"), evt };
	_emit(window, 2, argv);
	
}


void cursorEnterCB(GLFWwindow* window, int entered) { NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("mouseenter"));
	SET_PROP(evt, "entered", JS_INT(entered));
	
	Local<Value> argv[2] = { JS_STR("mouseenter"), evt };
	_emit(window, 2, argv);
	
}


void mouseButtonCB(GLFWwindow *window, int button, int action, int mods) { NAN_HS;
	
	const WinState &state = states[window];
	
	Local<Object> evt = Nan::New<Object>();
	
	SET_PROP(evt, "type", JS_STR(action ? "mousedown" : "mouseup"));
	SET_PROP(evt, "button", JS_INT(button));
	SET_PROP(evt, "which", JS_INT(button));
	SET_PROP(evt, "clientX", JS_INT(state.mouseX));
	SET_PROP(evt, "clientY", JS_INT(state.mouseY));
	SET_PROP(evt, "pageX", JS_INT(state.mouseX));
	SET_PROP(evt, "pageY", JS_INT(state.mouseY));
	
	SET_PROP(evt, "x", JS_INT(state.mouseX));
	SET_PROP(evt, "y", JS_INT(state.mouseY));
	SET_PROP(evt, "shiftKey", JS_BOOL(mods & GLFW_MOD_SHIFT));
	SET_PROP(evt, "ctrlKey", JS_BOOL(mods & GLFW_MOD_CONTROL));
	SET_PROP(evt, "altKey", JS_BOOL(mods & GLFW_MOD_ALT));
	SET_PROP(evt, "metaKey", JS_BOOL(mods & GLFW_MOD_SUPER));
	
	Local<Value> argv[2] = { JS_STR(action ? "mousedown" : "mouseup"), evt };
	_emit(window, 2, argv);
	
	if ( ! action ) {
		
		Local<Object> evt = Nan::New<Object>();
		
		SET_PROP(evt, "type", JS_STR("click"));
		SET_PROP(evt, "button", JS_INT(button));
		SET_PROP(evt, "which", JS_INT(button));
		SET_PROP(evt, "clientX", JS_INT(state.mouseX));
		SET_PROP(evt, "clientY", JS_INT(state.mouseY));
		SET_PROP(evt, "pageX", JS_INT(state.mouseX));
		SET_PROP(evt, "pageY", JS_INT(state.mouseY));
		SET_PROP(evt, "shiftKey", JS_BOOL(mods & GLFW_MOD_SHIFT));
		SET_PROP(evt, "ctrlKey", JS_BOOL(mods & GLFW_MOD_CONTROL));
		SET_PROP(evt, "altKey", JS_BOOL(mods & GLFW_MOD_ALT));
		SET_PROP(evt, "metaKey", JS_BOOL(mods & GLFW_MOD_SUPER));
		
		Local<Value> argv[2] = { JS_STR("click"), evt };
		_emit(window, 2, argv);
		
	}
	
}


void scrollCB(GLFWwindow *window, double xoffset, double yoffset) { NAN_HS;
	
	Local<Object> evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("mousewheel"));
	SET_PROP(evt, "wheelDeltaX", JS_NUM(xoffset*120));
	SET_PROP(evt, "wheelDeltaY", JS_NUM(yoffset*120));
	SET_PROP(evt, "wheelDelta", JS_NUM(yoffset*120));
	
	Local<Value> argv[2] = { JS_STR("mousewheel"), evt };
	_emit(window, 2, argv);
	
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
	
	int number = (int)c;
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
	
	GLFWmonitor **monitors = NULL;
	GLFWmonitor *monitor = NULL;
	int monitor_count;
	
	if (info.Length() >= 5 && monitor_idx >= 0) {
		monitors = glfwGetMonitors(&monitor_count);
		if (monitor_idx >= monitor_count) {
			return Nan::ThrowError("Invalid monitor");
		}
		monitor = monitors[monitor_idx];
	}
	
	GLFWwindow *share = NULL;
	if (states.size() > 0) {
		share = states.begin()->first;
	}
	
	GLFWwindow *window = glfwCreateWindow(width, height, *str, monitor, share);
	
	if ( ! window ) {
		// can't create window, throw error
		return Nan::ThrowError("Can't create GLFW window");
	}
	
	states[window] = WinState();
	
	glfwMakeContextCurrent(window);
	
	// make sure cursor is always shown
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	GLenum err = glewInit();
	
	if (err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		string msg="Can't init GLEW (glew error ";
		msg += (const char*) glewGetErrorString(err);
		msg += ")";
		
		fprintf(stderr, "%s", msg.c_str());
		return Nan::ThrowError(msg.c_str());
	}
	
	
	// Set callback functions
	WinState &state = states[window];
	state.events.Reset( emitter );
	
	// window callbacks
	glfwSetWindowPosCallback(window, windowPosCB);
	glfwSetWindowSizeCallback(window, windowSizeCB);
	glfwSetWindowCloseCallback(window, windowCloseCB);
	glfwSetWindowRefreshCallback(window, windowRefreshCB);
	glfwSetWindowFocusCallback(window, windowFocusCB);
	glfwSetWindowIconifyCallback(window, windowIconifyCB);
	glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCB);
	glfwSetDropCallback(window, windowDropCB);
	
	// input callbacks
	glfwSetKeyCallback(window, keyCB);
	// glfwSetCharCallback(window, charCB);
	glfwSetMouseButtonCallback(window, mouseButtonCB);
	glfwSetCursorPosCallback(window, cursorPosCB);
	glfwSetCursorEnterCallback(window, cursorEnterCB);
	glfwSetScrollCallback(window, scrollCB);
	
	RET_VALUE(JS_NUM((uint64_t) window));
	
}


NAN_METHOD(platformWindow) { THIS_WINDOW;
	
#ifdef _WIN32
	RET_VALUE(JS_NUM((uint64_t) glfwGetWin32Window(window)));
#elif __linux__
	RET_VALUE(JS_NUM((uint64_t) glfwGetX11Window(window)));
#elif __APPLE__
	RET_VALUE(JS_NUM((uint64_t) glfwGetCocoaWindow(window)));
#endif
	
}


NAN_METHOD(platformContext) { THIS_WINDOW;
	
#ifdef _WIN32
	RET_VALUE(JS_NUM((uint64_t) glfwGetWGLContext(window)));
#elif __linux__
	RET_VALUE(JS_NUM((uint64_t) glfwGetGLXContext(window)));
#elif __APPLE__
	RET_VALUE(JS_NUM((uint64_t) glfwGetNSGLContext(window)));
#endif
	
}


NAN_METHOD(getRenderTarget) {
	
	REQ_UINT32_ARG(0, width);
	REQ_UINT32_ARG(1, height);
	REQ_UINT32_ARG(2, samples);
	
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	GLuint renderBuffer;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	
	if (samples > 1) {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, width, height);
	} else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	}
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,	renderBuffer);
	
	GLuint tex;
	glGenTextures(1, &tex);
	
	if (samples > 1) {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
		// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, width, height, true);
		// glFramebufferTexture2DMultisampleEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0, samples);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
	} else {
		glBindTexture(GL_TEXTURE_2D, tex);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if (framebufferStatus == GL_FRAMEBUFFER_COMPLETE) {
		
		Local<Array> result = Nan::New<Array>(2);
		result->Set(0, JS_NUM(fbo));
		result->Set(1, JS_NUM(tex));
		
		RET_VALUE(result);
		
	} else {
		RET_VALUE(Null(Isolate::GetCurrent()));
	}
	
}


NAN_METHOD(bindFrameBuffer) {
	
	REQ_UINT32_ARG(0, fbo);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
}


NAN_METHOD(blitFrameBuffer) {
	
	REQ_UINT32_ARG(0, fbo1);
	REQ_UINT32_ARG(1, fbo2);
	REQ_UINT32_ARG(2, sw);
	REQ_UINT32_ARG(3, sh);
	REQ_UINT32_ARG(4, dw);
	REQ_UINT32_ARG(5, dh);
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo1);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo2);
	
	glBlitFramebuffer(0, 0, sw, sh, 0, 0, dw, dh, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}


NAN_METHOD(destroyWindow) { THIS_WINDOW;
	
	glfwDestroyWindow(window);
	
}


NAN_METHOD(setWindowTitle) { THIS_WINDOW;
	
	REQ_UTF8_ARG(1, str);
	
	glfwSetWindowTitle(window, *str);
	
}


NAN_METHOD(setWindowIcon) { THIS_WINDOW;
	
	REQ_OBJ_ARG(1, icon);
	
	GLFWimage image;
	image.width = icon->Get(JS_STR("width"))->Int32Value();
	image.height = icon->Get(JS_STR("height"))->Int32Value();
	image.pixels = reinterpret_cast<unsigned char*>(getImageData(icon));
	
	glfwSetWindowIcon(window, 1, &image);
	
}


NAN_METHOD(getWindowSize) { THIS_WINDOW;
	
	int w,h;
	glfwGetWindowSize(window, &w, &h);
	
	Local<Object> obj = Nan::New<Object>();
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
	
	REQ_UINT32_ARG(1, x);
	REQ_UINT32_ARG(2, y);
	
	glfwSetWindowPos(window, x, y);
	
}


NAN_METHOD(getWindowPos) { THIS_WINDOW;
	
	int xpos, ypos;
	glfwGetWindowPos(window, &xpos, &ypos);
	
	Local<Object> obj = Nan::New<Object>();
	SET_PROP(obj, "x", JS_INT(xpos));
	SET_PROP(obj, "y", JS_INT(ypos));
	
	RET_VALUE(obj);
	
}


NAN_METHOD(getFramebufferSize) { THIS_WINDOW;
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	Local<Object> obj = Nan::New<Object>();
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
	
	double x,y;
	glfwGetCursorPos(window, &x, &y);
	
	Local<Object> obj = Nan::New<Object>();
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
	
	RET_VALUE(JS_NUM((uint64_t) window));
	
}


NAN_METHOD(swapBuffers) { THIS_WINDOW;
	
	glfwSwapBuffers(window);
	
}


NAN_METHOD(swapInterval) {
	
	REQ_INT32_ARG(0, interval);
	
	glfwSwapInterval(interval);
	
}


/* Extension support */
NAN_METHOD(extensionSupported) {
	
	REQ_UTF8_ARG(0, str);
	
	RET_VALUE(JS_BOOL(glfwExtensionSupported(*str) == 1));
	
}


// make sure we close everything when we exit
void deinit() {
	glfwTerminate();
}


} // namespace glfw


extern "C" {


NAN_MODULE_INIT(_init) {
	
	atexit(glfw::deinit);
	
	NAN_HS;
	
	/* GLFW initialization, termination and version querying */
	JS_GLFW_SET_METHOD(init);
	JS_GLFW_SET_METHOD(terminate);
	JS_GLFW_SET_METHOD(getVersion);
	JS_GLFW_SET_METHOD(getVersionString);
	
	/* Time */
	JS_GLFW_SET_METHOD(getTime);
	JS_GLFW_SET_METHOD(setTime);
	
	/* Monitor handling */
	JS_GLFW_SET_METHOD(getMonitors);
	
	/* Window handling */
	JS_GLFW_SET_METHOD(createWindow);
	JS_GLFW_SET_METHOD(getRenderTarget);
	JS_GLFW_SET_METHOD(bindFrameBuffer);
	JS_GLFW_SET_METHOD(blitFrameBuffer);
	JS_GLFW_SET_METHOD(windowHint);
	JS_GLFW_SET_METHOD(defaultWindowHints);
	JS_GLFW_SET_METHOD(platformWindow);
	JS_GLFW_SET_METHOD(platformContext);
	JS_GLFW_SET_METHOD(destroyWindow);
	JS_GLFW_SET_METHOD(setWindowShouldClose);
	JS_GLFW_SET_METHOD(windowShouldClose);
	JS_GLFW_SET_METHOD(setWindowTitle);
	JS_GLFW_SET_METHOD(setWindowIcon);
	JS_GLFW_SET_METHOD(getWindowSize);
	JS_GLFW_SET_METHOD(setWindowSize);
	JS_GLFW_SET_METHOD(setWindowPos);
	JS_GLFW_SET_METHOD(getWindowPos);
	JS_GLFW_SET_METHOD(getFramebufferSize);
	JS_GLFW_SET_METHOD(iconifyWindow);
	JS_GLFW_SET_METHOD(restoreWindow);
	JS_GLFW_SET_METHOD(showWindow);
	JS_GLFW_SET_METHOD(hideWindow);
	JS_GLFW_SET_METHOD(getWindowAttrib);
	JS_GLFW_SET_METHOD(setInputMode);
	JS_GLFW_SET_METHOD(pollEvents);
	JS_GLFW_SET_METHOD(waitEvents);
	
	/* Input handling */
	JS_GLFW_SET_METHOD(getKey);
	JS_GLFW_SET_METHOD(getMouseButton);
	JS_GLFW_SET_METHOD(getCursorPos);
	JS_GLFW_SET_METHOD(setCursorPos);
	
	/* Context handling */
	JS_GLFW_SET_METHOD(makeContextCurrent);
	JS_GLFW_SET_METHOD(getCurrentContext);
	JS_GLFW_SET_METHOD(swapBuffers);
	JS_GLFW_SET_METHOD(swapInterval);
	JS_GLFW_SET_METHOD(extensionSupported);
	
	/* Joystick */
	JS_GLFW_SET_METHOD(joystickPresent);
	JS_GLFW_SET_METHOD(getJoystickAxes);
	JS_GLFW_SET_METHOD(getJoystickButtons);
	JS_GLFW_SET_METHOD(getJoystickName);
	
	/*************************************************************************
	* GLFW version
	*************************************************************************/
	
	JS_GLFW_CONSTANT(VERSION_MAJOR);
	JS_GLFW_CONSTANT(VERSION_MINOR);
	JS_GLFW_CONSTANT(VERSION_REVISION);
	
	/*************************************************************************
	* Input handling definitions
	*************************************************************************/
	
	/* Key and button state/action definitions */
	JS_GLFW_CONSTANT(RELEASE);
	JS_GLFW_CONSTANT(PRESS);
	JS_GLFW_CONSTANT(REPEAT);
	
	/* These key codes are inspired by the *USB HID Usage Tables v1.12* (p. 53-60),
	* but re-arranged to map to 7-bit ASCII for printable keys (function keys are
	* put in the 256+ range).
	*
	* The naming of the key codes follow these rules:
	*  - The US keyboard layout is used
	*  - Names of printable alpha-numeric characters are used (e.g. "A", "R",
	*    "3", etc.)
	*  - For non-alphanumeric characters, Unicode:ish names are used (e.g.
	*    "COMMA", "LEFT_SQUARE_BRACKET", etc.). Note that some names do not
	*    correspond to the Unicode standard (usually for brevity)
	*  - Keys that lack a clear US mapping are named "WORLD_x"
	*  - For non-printable keys, custom names are used (e.g. "F4",
	*    "BACKSPACE", etc.)
	*/
	
	/* The unknown key */
	JS_GLFW_CONSTANT(KEY_UNKNOWN);
	
	/* Printable keys */
	JS_GLFW_CONSTANT(KEY_SPACE);
	JS_GLFW_CONSTANT(KEY_APOSTROPHE);
	JS_GLFW_CONSTANT(KEY_COMMA);
	JS_GLFW_CONSTANT(KEY_MINUS);
	JS_GLFW_CONSTANT(KEY_PERIOD);
	JS_GLFW_CONSTANT(KEY_SLASH);
	JS_GLFW_CONSTANT(KEY_0);
	JS_GLFW_CONSTANT(KEY_1);
	JS_GLFW_CONSTANT(KEY_2);
	JS_GLFW_CONSTANT(KEY_3);
	JS_GLFW_CONSTANT(KEY_4);
	JS_GLFW_CONSTANT(KEY_5);
	JS_GLFW_CONSTANT(KEY_6);
	JS_GLFW_CONSTANT(KEY_7);
	JS_GLFW_CONSTANT(KEY_8);
	JS_GLFW_CONSTANT(KEY_9);
	JS_GLFW_CONSTANT(KEY_SEMICOLON);
	JS_GLFW_CONSTANT(KEY_EQUAL);
	JS_GLFW_CONSTANT(KEY_A);
	JS_GLFW_CONSTANT(KEY_B);
	JS_GLFW_CONSTANT(KEY_C);
	JS_GLFW_CONSTANT(KEY_D);
	JS_GLFW_CONSTANT(KEY_E);
	JS_GLFW_CONSTANT(KEY_F);
	JS_GLFW_CONSTANT(KEY_G);
	JS_GLFW_CONSTANT(KEY_H);
	JS_GLFW_CONSTANT(KEY_I);
	JS_GLFW_CONSTANT(KEY_J);
	JS_GLFW_CONSTANT(KEY_K);
	JS_GLFW_CONSTANT(KEY_L);
	JS_GLFW_CONSTANT(KEY_M);
	JS_GLFW_CONSTANT(KEY_N);
	JS_GLFW_CONSTANT(KEY_O);
	JS_GLFW_CONSTANT(KEY_P);
	JS_GLFW_CONSTANT(KEY_Q);
	JS_GLFW_CONSTANT(KEY_R);
	JS_GLFW_CONSTANT(KEY_S);
	JS_GLFW_CONSTANT(KEY_T);
	JS_GLFW_CONSTANT(KEY_U);
	JS_GLFW_CONSTANT(KEY_V);
	JS_GLFW_CONSTANT(KEY_W);
	JS_GLFW_CONSTANT(KEY_X);
	JS_GLFW_CONSTANT(KEY_Y);
	JS_GLFW_CONSTANT(KEY_Z);
	JS_GLFW_CONSTANT(KEY_LEFT_BRACKET);
	JS_GLFW_CONSTANT(KEY_BACKSLASH);
	JS_GLFW_CONSTANT(KEY_RIGHT_BRACKET);
	JS_GLFW_CONSTANT(KEY_GRAVE_ACCENT);
	JS_GLFW_CONSTANT(KEY_WORLD_1);
	JS_GLFW_CONSTANT(KEY_WORLD_2);
	
	/* Function keys */
	JS_GLFW_CONSTANT(KEY_ESCAPE);
	JS_GLFW_CONSTANT(KEY_ENTER);
	JS_GLFW_CONSTANT(KEY_TAB);
	JS_GLFW_CONSTANT(KEY_BACKSPACE);
	JS_GLFW_CONSTANT(KEY_INSERT);
	JS_GLFW_CONSTANT(KEY_DELETE);
	JS_GLFW_CONSTANT(KEY_RIGHT);
	JS_GLFW_CONSTANT(KEY_LEFT);
	JS_GLFW_CONSTANT(KEY_DOWN);
	JS_GLFW_CONSTANT(KEY_UP);
	JS_GLFW_CONSTANT(KEY_PAGE_UP);
	JS_GLFW_CONSTANT(KEY_PAGE_DOWN);
	JS_GLFW_CONSTANT(KEY_HOME);
	JS_GLFW_CONSTANT(KEY_END);
	JS_GLFW_CONSTANT(KEY_CAPS_LOCK);
	JS_GLFW_CONSTANT(KEY_SCROLL_LOCK);
	JS_GLFW_CONSTANT(KEY_NUM_LOCK);
	JS_GLFW_CONSTANT(KEY_PRINT_SCREEN);
	JS_GLFW_CONSTANT(KEY_PAUSE);
	JS_GLFW_CONSTANT(KEY_F1);
	JS_GLFW_CONSTANT(KEY_F2);
	JS_GLFW_CONSTANT(KEY_F3);
	JS_GLFW_CONSTANT(KEY_F4);
	JS_GLFW_CONSTANT(KEY_F5);
	JS_GLFW_CONSTANT(KEY_F6);
	JS_GLFW_CONSTANT(KEY_F7);
	JS_GLFW_CONSTANT(KEY_F8);
	JS_GLFW_CONSTANT(KEY_F9);
	JS_GLFW_CONSTANT(KEY_F10);
	JS_GLFW_CONSTANT(KEY_F11);
	JS_GLFW_CONSTANT(KEY_F12);
	JS_GLFW_CONSTANT(KEY_F13);
	JS_GLFW_CONSTANT(KEY_F14);
	JS_GLFW_CONSTANT(KEY_F15);
	JS_GLFW_CONSTANT(KEY_F16);
	JS_GLFW_CONSTANT(KEY_F17);
	JS_GLFW_CONSTANT(KEY_F18);
	JS_GLFW_CONSTANT(KEY_F19);
	JS_GLFW_CONSTANT(KEY_F20);
	JS_GLFW_CONSTANT(KEY_F21);
	JS_GLFW_CONSTANT(KEY_F22);
	JS_GLFW_CONSTANT(KEY_F23);
	JS_GLFW_CONSTANT(KEY_F24);
	JS_GLFW_CONSTANT(KEY_F25);
	JS_GLFW_CONSTANT(KEY_KP_0);
	JS_GLFW_CONSTANT(KEY_KP_1);
	JS_GLFW_CONSTANT(KEY_KP_2);
	JS_GLFW_CONSTANT(KEY_KP_3);
	JS_GLFW_CONSTANT(KEY_KP_4);
	JS_GLFW_CONSTANT(KEY_KP_5);
	JS_GLFW_CONSTANT(KEY_KP_6);
	JS_GLFW_CONSTANT(KEY_KP_7);
	JS_GLFW_CONSTANT(KEY_KP_8);
	JS_GLFW_CONSTANT(KEY_KP_9);
	JS_GLFW_CONSTANT(KEY_KP_DECIMAL);
	JS_GLFW_CONSTANT(KEY_KP_DIVIDE);
	JS_GLFW_CONSTANT(KEY_KP_MULTIPLY);
	JS_GLFW_CONSTANT(KEY_KP_SUBTRACT);
	JS_GLFW_CONSTANT(KEY_KP_ADD);
	JS_GLFW_CONSTANT(KEY_KP_ENTER);
	JS_GLFW_CONSTANT(KEY_KP_EQUAL);
	JS_GLFW_CONSTANT(KEY_LEFT_SHIFT);
	JS_GLFW_CONSTANT(KEY_LEFT_CONTROL);
	JS_GLFW_CONSTANT(KEY_LEFT_ALT);
	JS_GLFW_CONSTANT(KEY_LEFT_SUPER);
	JS_GLFW_CONSTANT(KEY_RIGHT_SHIFT);
	JS_GLFW_CONSTANT(KEY_RIGHT_CONTROL);
	JS_GLFW_CONSTANT(KEY_RIGHT_ALT);
	JS_GLFW_CONSTANT(KEY_RIGHT_SUPER);
	JS_GLFW_CONSTANT(KEY_MENU);
	JS_GLFW_CONSTANT(KEY_LAST);
	
	/*Modifier key flags*/
	
	/*If this bit is set one or more Shift keys were held down. */
	JS_GLFW_CONSTANT(MOD_SHIFT);
	/*If this bit is set one or more Control keys were held down. */
	JS_GLFW_CONSTANT(MOD_CONTROL);
	/*If this bit is set one or more Alt keys were held down. */
	JS_GLFW_CONSTANT(MOD_ALT);
	/*If this bit is set one or more Super keys were held down. */
	JS_GLFW_CONSTANT(MOD_SUPER);
	
	/*Mouse buttons*/
	JS_GLFW_CONSTANT(MOUSE_BUTTON_1);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_2);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_3);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_4);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_5);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_6);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_7);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_8);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_LAST);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_LEFT);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_RIGHT);
	JS_GLFW_CONSTANT(MOUSE_BUTTON_MIDDLE);
	
	/*Joysticks*/
	JS_GLFW_CONSTANT(JOYSTICK_1);
	JS_GLFW_CONSTANT(JOYSTICK_2);
	JS_GLFW_CONSTANT(JOYSTICK_3);
	JS_GLFW_CONSTANT(JOYSTICK_4);
	JS_GLFW_CONSTANT(JOYSTICK_5);
	JS_GLFW_CONSTANT(JOYSTICK_6);
	JS_GLFW_CONSTANT(JOYSTICK_7);
	JS_GLFW_CONSTANT(JOYSTICK_8);
	JS_GLFW_CONSTANT(JOYSTICK_9);
	JS_GLFW_CONSTANT(JOYSTICK_10);
	JS_GLFW_CONSTANT(JOYSTICK_11);
	JS_GLFW_CONSTANT(JOYSTICK_12);
	JS_GLFW_CONSTANT(JOYSTICK_13);
	JS_GLFW_CONSTANT(JOYSTICK_14);
	JS_GLFW_CONSTANT(JOYSTICK_15);
	JS_GLFW_CONSTANT(JOYSTICK_16);
	JS_GLFW_CONSTANT(JOYSTICK_LAST);
	
	/*errors Error codes*/
	
	/*GLFW has not been initialized.*/
	JS_GLFW_CONSTANT(NOT_INITIALIZED);
	/*No context is current for this thread.*/
	JS_GLFW_CONSTANT(NO_CURRENT_CONTEXT);
	/*One of the enum parameters for the function was given an invalid enum.*/
	JS_GLFW_CONSTANT(INVALID_ENUM);
	/*One of the parameters for the function was given an invalid value.*/
	JS_GLFW_CONSTANT(INVALID_VALUE);
	/*A memory allocation failed.*/
	JS_GLFW_CONSTANT(OUT_OF_MEMORY);
	/*GLFW could not find support for the requested client API on the system.*/
	JS_GLFW_CONSTANT(API_UNAVAILABLE);
	/*The requested client API version is not available.*/
	JS_GLFW_CONSTANT(VERSION_UNAVAILABLE);
	/*A platform-specific error occurred that does not match any of the more specific categories.*/
	JS_GLFW_CONSTANT(PLATFORM_ERROR);
	/*The clipboard did not contain data in the requested format.*/
	JS_GLFW_CONSTANT(FORMAT_UNAVAILABLE);
	
	JS_GLFW_CONSTANT(FOCUSED);
	JS_GLFW_CONSTANT(ICONIFIED);
	JS_GLFW_CONSTANT(RESIZABLE);
	JS_GLFW_CONSTANT(VISIBLE);
	JS_GLFW_CONSTANT(DECORATED);
	
	JS_GLFW_CONSTANT(RED_BITS);
	JS_GLFW_CONSTANT(GREEN_BITS);
	JS_GLFW_CONSTANT(BLUE_BITS);
	JS_GLFW_CONSTANT(ALPHA_BITS);
	JS_GLFW_CONSTANT(DEPTH_BITS);
	JS_GLFW_CONSTANT(STENCIL_BITS);
	JS_GLFW_CONSTANT(ACCUM_RED_BITS);
	JS_GLFW_CONSTANT(ACCUM_GREEN_BITS);
	JS_GLFW_CONSTANT(ACCUM_BLUE_BITS);
	JS_GLFW_CONSTANT(ACCUM_ALPHA_BITS);
	JS_GLFW_CONSTANT(AUX_BUFFERS);
	JS_GLFW_CONSTANT(STEREO);
	JS_GLFW_CONSTANT(SAMPLES);
	JS_GLFW_CONSTANT(SRGB_CAPABLE);
	JS_GLFW_CONSTANT(REFRESH_RATE);
	JS_GLFW_CONSTANT(DOUBLEBUFFER);
	JS_GLFW_CONSTANT(TRUE);
	JS_GLFW_CONSTANT(FALSE);
	
	JS_GLFW_CONSTANT(CLIENT_API);
	JS_GLFW_CONSTANT(CONTEXT_VERSION_MAJOR);
	JS_GLFW_CONSTANT(CONTEXT_VERSION_MINOR);
	JS_GLFW_CONSTANT(CONTEXT_REVISION);
	JS_GLFW_CONSTANT(CONTEXT_ROBUSTNESS);
	JS_GLFW_CONSTANT(OPENGL_FORWARD_COMPAT);
	JS_GLFW_CONSTANT(OPENGL_DEBUG_CONTEXT);
	JS_GLFW_CONSTANT(OPENGL_PROFILE);
	
	JS_GLFW_CONSTANT(OPENGL_API);
	JS_GLFW_CONSTANT(OPENGL_ES_API);
	
	JS_GLFW_CONSTANT(NO_ROBUSTNESS);
	JS_GLFW_CONSTANT(NO_RESET_NOTIFICATION);
	JS_GLFW_CONSTANT(LOSE_CONTEXT_ON_RESET);
	
	JS_GLFW_CONSTANT(OPENGL_ANY_PROFILE);
	JS_GLFW_CONSTANT(OPENGL_CORE_PROFILE);
	JS_GLFW_CONSTANT(OPENGL_COMPAT_PROFILE);
	
	JS_GLFW_CONSTANT(CURSOR);
	JS_GLFW_CONSTANT(STICKY_KEYS);
	JS_GLFW_CONSTANT(STICKY_MOUSE_BUTTONS);
	
	JS_GLFW_CONSTANT(CURSOR_NORMAL);
	JS_GLFW_CONSTANT(CURSOR_HIDDEN);
	JS_GLFW_CONSTANT(CURSOR_DISABLED);
	
	JS_GLFW_CONSTANT(CONNECTED);
	JS_GLFW_CONSTANT(DISCONNECTED);
	
	// test scene
	JS_GLFW_SET_METHOD(testScene);
	JS_GLFW_SET_METHOD(testJoystick);
	
}


NODE_MODULE(glfw, _init)

}
