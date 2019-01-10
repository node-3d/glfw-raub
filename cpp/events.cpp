#include <cstdlib>
#include <sstream>

#include "platform.hpp"
#include "win-state.hpp"
#include "events.hpp"


using namespace v8;
using namespace node;
using namespace std;


namespace glfw {

void NAN_INLINE(_emit(GLFWwindow *window, int argc, V8_VAR_VAL argv[])) {
	
	WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));
	
	if (Nan::New(state->events)->Has(JS_STR("emit"))) {
		
		Nan::Callback callback(Nan::New(state->events)->Get(JS_STR("emit")).As<Function>());
		
		if ( ! callback.IsEmpty() ) {
			Nan::AsyncResource async("glfw::_emit()");
			callback.Call(argc, argv, &async);
		}
		
	}
	
}


void windowPosCB(GLFWwindow *window, int xpos, int ypos) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("window_pos"));
	SET_PROP(evt, "x", JS_INT(xpos));
	SET_PROP(evt, "y", JS_INT(ypos));
	
	V8_VAR_VAL argv[2] = { JS_STR("window_pos"), evt };
	_emit(window, 2, argv);
	
}


void windowSizeCB(GLFWwindow *window, int w, int h) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("resize"));
	SET_PROP(evt, "width", JS_INT(w));
	SET_PROP(evt, "height", JS_INT(h));
	
	V8_VAR_VAL argv[2] = { JS_STR("resize"), evt };
	_emit(window, 2, argv);
	
}


void windowFramebufferSizeCB(GLFWwindow *window, int w, int h) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("framebuffer_resize"));
	SET_PROP(evt, "width", JS_INT(w));
	SET_PROP(evt, "height", JS_INT(h));
	
	V8_VAR_VAL argv[2] = { JS_STR("framebuffer_resize"), evt };
	_emit(window, 2, argv);
	
}


void windowDropCB(GLFWwindow *window, int count, const char **paths) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("drop"));
	
	V8_VAR_ARR list = Nan::New<Array>(count);
	for (int i = 0; i < count; i++) {
		list->Set(i, JS_STR(paths[i]));
	}
	
	SET_PROP(evt, "list", list);
	
	V8_VAR_VAL argv[2] = { JS_STR("drop"), evt };
	_emit(window, 2, argv);
	
}


void windowCloseCB(GLFWwindow *window) { NAN_HS;
	
	V8_VAR_VAL argv[1] = { JS_STR("quit") };
	_emit(window, 1, argv);
	
}


void windowRefreshCB(GLFWwindow *window) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("refresh"));
	SET_PROP(evt, "window", JS_OFFS(reinterpret_cast<uint64_t>(window)));
	
	V8_VAR_VAL argv[2] = { JS_STR("refresh"), evt };
	_emit(window, 2, argv);
	
}


void windowIconifyCB(GLFWwindow *window, int iconified) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("iconified"));
	SET_PROP(evt, "iconified", JS_BOOL(iconified));
	
	V8_VAR_VAL argv[2] = { JS_STR("iconified"), evt };
	_emit(window, 2, argv);
	
}


void windowFocusCB(GLFWwindow *window, int focused) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("focused"));
	SET_PROP(evt, "focused", JS_BOOL(focused));
	
	V8_VAR_VAL argv[2] = { JS_STR("focused"), evt };
	_emit(window, 2, argv);
	
}


#define KEY_CASE(NAME, ID) case NAME: key = ID; break;

void keyCB(GLFWwindow *window, int key, int scancode, int action, int mods) { NAN_HS;
	
	const char *actionNames = "keyup\0  keydown\0keypress";
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR(&actionNames[action << 3]));
	SET_PROP(evt, "ctrlKey", JS_BOOL(mods & GLFW_MOD_CONTROL));
	SET_PROP(evt, "shiftKey", JS_BOOL(mods & GLFW_MOD_SHIFT));
	SET_PROP(evt, "altKey", JS_BOOL(mods & GLFW_MOD_ALT));
	SET_PROP(evt, "metaKey", JS_BOOL(mods & GLFW_MOD_SUPER));
	
	int which = key, charCode = key;
	
	switch (key) {
		KEY_CASE(GLFW_KEY_ESCAPE, 27)
		KEY_CASE(GLFW_KEY_ENTER, 13)
		KEY_CASE(GLFW_KEY_TAB, 9)
		KEY_CASE(GLFW_KEY_BACKSPACE, 8)
		KEY_CASE(GLFW_KEY_INSERT, 45)
		KEY_CASE(GLFW_KEY_DELETE, 46)
		KEY_CASE(GLFW_KEY_RIGHT, 39)
		KEY_CASE(GLFW_KEY_LEFT, 37)
		KEY_CASE(GLFW_KEY_DOWN, 40)
		KEY_CASE(GLFW_KEY_UP, 38)
		KEY_CASE(GLFW_KEY_PAGE_UP, 33)
		KEY_CASE(GLFW_KEY_PAGE_DOWN, 34)
		KEY_CASE(GLFW_KEY_HOME, 36)
		KEY_CASE(GLFW_KEY_END, 35)
		KEY_CASE(GLFW_KEY_CAPS_LOCK, 20)
		KEY_CASE(GLFW_KEY_SCROLL_LOCK, 145)
		KEY_CASE(GLFW_KEY_NUM_LOCK, 144)
		KEY_CASE(GLFW_KEY_PRINT_SCREEN, 144)
		KEY_CASE(GLFW_KEY_PAUSE, 19)
		KEY_CASE(GLFW_KEY_F1, 112)
		KEY_CASE(GLFW_KEY_F2, 113)
		KEY_CASE(GLFW_KEY_F3, 114)
		KEY_CASE(GLFW_KEY_F4, 115)
		KEY_CASE(GLFW_KEY_F5, 116)
		KEY_CASE(GLFW_KEY_F6, 117)
		KEY_CASE(GLFW_KEY_F7, 118)
		KEY_CASE(GLFW_KEY_F8, 119)
		KEY_CASE(GLFW_KEY_F9, 120)
		KEY_CASE(GLFW_KEY_F10, 121)
		KEY_CASE(GLFW_KEY_F11, 122)
		KEY_CASE(GLFW_KEY_F12, 123)
		KEY_CASE(GLFW_KEY_F13, 123)
		KEY_CASE(GLFW_KEY_F14, 123)
		KEY_CASE(GLFW_KEY_F15, 123)
		KEY_CASE(GLFW_KEY_F16, 123)
		KEY_CASE(GLFW_KEY_F17, 123)
		KEY_CASE(GLFW_KEY_F18, 123)
		KEY_CASE(GLFW_KEY_F19, 123)
		KEY_CASE(GLFW_KEY_F20, 123)
		KEY_CASE(GLFW_KEY_F21, 123)
		KEY_CASE(GLFW_KEY_F22, 123)
		KEY_CASE(GLFW_KEY_F23, 123)
		KEY_CASE(GLFW_KEY_F24, 123)
		KEY_CASE(GLFW_KEY_F25, 123)
		KEY_CASE(GLFW_KEY_KP_0, 96)
		KEY_CASE(GLFW_KEY_KP_1, 97)
		KEY_CASE(GLFW_KEY_KP_2, 98)
		KEY_CASE(GLFW_KEY_KP_3, 99)
		KEY_CASE(GLFW_KEY_KP_4, 100)
		KEY_CASE(GLFW_KEY_KP_5, 101)
		KEY_CASE(GLFW_KEY_KP_6, 102)
		KEY_CASE(GLFW_KEY_KP_7, 103)
		KEY_CASE(GLFW_KEY_KP_8, 104)
		KEY_CASE(GLFW_KEY_KP_9, 105)
		KEY_CASE(GLFW_KEY_KP_DECIMAL, 110)
		KEY_CASE(GLFW_KEY_KP_DIVIDE, 111)
		KEY_CASE(GLFW_KEY_KP_MULTIPLY, 106)
		KEY_CASE(GLFW_KEY_KP_SUBTRACT, 109)
		KEY_CASE(GLFW_KEY_KP_ADD, 107)
		KEY_CASE(GLFW_KEY_KP_ENTER, 13)
		KEY_CASE(GLFW_KEY_KP_EQUAL, 187)
		KEY_CASE(GLFW_KEY_LEFT_SHIFT, 16)
		KEY_CASE(GLFW_KEY_LEFT_CONTROL, 17)
		KEY_CASE(GLFW_KEY_LEFT_ALT, 18)
		KEY_CASE(GLFW_KEY_LEFT_SUPER, 91)
		KEY_CASE(GLFW_KEY_RIGHT_SHIFT, 16)
		KEY_CASE(GLFW_KEY_RIGHT_CONTROL, 17)
		KEY_CASE(GLFW_KEY_RIGHT_ALT, 18)
		KEY_CASE(GLFW_KEY_RIGHT_SUPER, 93)
		KEY_CASE(GLFW_KEY_MENU, 18)
		KEY_CASE(GLFW_KEY_SEMICOLON, 186)
		KEY_CASE(GLFW_KEY_EQUAL, 187)
		KEY_CASE(GLFW_KEY_COMMA, 188)
		KEY_CASE(GLFW_KEY_MINUS, 189)
		KEY_CASE(GLFW_KEY_PERIOD, 190)
		KEY_CASE(GLFW_KEY_SLASH, 191)
		KEY_CASE(GLFW_KEY_GRAVE_ACCENT, 192)
		KEY_CASE(GLFW_KEY_LEFT_BRACKET, 219)
		KEY_CASE(GLFW_KEY_BACKSLASH, 220)
		KEY_CASE(GLFW_KEY_RIGHT_BRACKET, 221)
		KEY_CASE(GLFW_KEY_APOSTROPHE, 222)
	}
	
	SET_PROP(evt, "which", JS_INT(which));
	SET_PROP(evt, "keyCode", JS_INT(key));
	SET_PROP(evt, "charCode", JS_INT(charCode));
	
	V8_VAR_VAL argv[2] = { JS_STR(&actionNames[action << 3]), evt };
	_emit(window, 2, argv);
	
}


void cursorPosCB(GLFWwindow* window, double x, double y) {
	
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	
	if (x < 0 || x >= w || y < 0 || y >= h) {
		return;
	}
	
	WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));
	
	state->mouseX = static_cast<int>(x);
	state->mouseY = static_cast<int>(y);
	
	NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	
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
	
	V8_VAR_VAL argv[2] = { JS_STR("mousemove"), evt };
	_emit(window, 2, argv);
	
}


void cursorEnterCB(GLFWwindow* window, int entered) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("mouseenter"));
	SET_PROP(evt, "entered", JS_INT(entered));
	
	V8_VAR_VAL argv[2] = { JS_STR("mouseenter"), evt };
	_emit(window, 2, argv);
	
}


void mouseButtonCB(GLFWwindow *window, int button, int action, int mods) { NAN_HS;
	
	WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	
	SET_PROP(evt, "type", JS_STR(action ? "mousedown" : "mouseup"));
	SET_PROP(evt, "button", JS_INT(button));
	SET_PROP(evt, "which", JS_INT(button));
	SET_PROP(evt, "clientX", JS_INT(state->mouseX));
	SET_PROP(evt, "clientY", JS_INT(state->mouseY));
	SET_PROP(evt, "pageX", JS_INT(state->mouseX));
	SET_PROP(evt, "pageY", JS_INT(state->mouseY));
	
	SET_PROP(evt, "x", JS_INT(state->mouseX));
	SET_PROP(evt, "y", JS_INT(state->mouseY));
	SET_PROP(evt, "shiftKey", JS_BOOL(mods & GLFW_MOD_SHIFT));
	SET_PROP(evt, "ctrlKey", JS_BOOL(mods & GLFW_MOD_CONTROL));
	SET_PROP(evt, "altKey", JS_BOOL(mods & GLFW_MOD_ALT));
	SET_PROP(evt, "metaKey", JS_BOOL(mods & GLFW_MOD_SUPER));
	
	V8_VAR_VAL argv[2] = { JS_STR(action ? "mousedown" : "mouseup"), evt };
	_emit(window, 2, argv);
	
	if ( ! action ) {
		
		V8_VAR_OBJ evt = Nan::New<Object>();
		
		SET_PROP(evt, "type", JS_STR("click"));
		SET_PROP(evt, "button", JS_INT(button));
		SET_PROP(evt, "which", JS_INT(button));
		SET_PROP(evt, "clientX", JS_INT(state->mouseX));
		SET_PROP(evt, "clientY", JS_INT(state->mouseY));
		SET_PROP(evt, "pageX", JS_INT(state->mouseX));
		SET_PROP(evt, "pageY", JS_INT(state->mouseY));
		SET_PROP(evt, "shiftKey", JS_BOOL(mods & GLFW_MOD_SHIFT));
		SET_PROP(evt, "ctrlKey", JS_BOOL(mods & GLFW_MOD_CONTROL));
		SET_PROP(evt, "altKey", JS_BOOL(mods & GLFW_MOD_ALT));
		SET_PROP(evt, "metaKey", JS_BOOL(mods & GLFW_MOD_SUPER));
		
		V8_VAR_VAL argv[2] = { JS_STR("click"), evt };
		_emit(window, 2, argv);
		
	}
	
}


void scrollCB(GLFWwindow *window, double xoffset, double yoffset) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	SET_PROP(evt, "type", JS_STR("mousewheel"));
	SET_PROP(evt, "wheelDeltaX", JS_NUM(xoffset*120));
	SET_PROP(evt, "wheelDeltaY", JS_NUM(yoffset*120));
	SET_PROP(evt, "wheelDelta", JS_NUM(yoffset*120));
	
	V8_VAR_VAL argv[2] = { JS_STR("mousewheel"), evt };
	_emit(window, 2, argv);
	
}


} // namespace glfw
