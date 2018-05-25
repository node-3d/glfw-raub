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
