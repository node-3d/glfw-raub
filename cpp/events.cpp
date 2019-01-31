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


#define KEY_CASE(NAME, ID) case NAME: which = ID; break;

void keyCB(GLFWwindow *window, int glfwKey, int scancode, int action, int mods) { NAN_HS;
	
	const char typeKeyUp[] = "keyup";
	const char typeKeyDown[] = "keydown";
	const char *typeFound = nullptr;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		typeFound = typeKeyDown;
		if (action == GLFW_REPEAT) {
			SET_PROP(evt, "repeat", JS_BOOL(true));
		}
	} else {
		typeFound = typeKeyUp;
	}
	
	const char *keyName = glfwGetKeyName(glfwKey, scancode);
	
	SET_PROP(evt, "altKey", JS_BOOL(mods & GLFW_MOD_ALT));
	SET_PROP(evt, "ctrlKey", JS_BOOL(mods & GLFW_MOD_CONTROL));
	SET_PROP(evt, "metaKey", JS_BOOL(mods & GLFW_MOD_SUPER));
	SET_PROP(evt, "shiftKey", JS_BOOL(mods & GLFW_MOD_SHIFT));
	
	
	SET_PROP(evt, "charCode", JS_INT(0));
	if (keyName) {
		SET_PROP(evt, "code", JS_STR(keyName));
		SET_PROP(evt, "key", JS_STR(keyName));
	} else {
		SET_PROP(evt, "code", Nan::Null());
		SET_PROP(evt, "key", Nan::Null());
	}
	
	SET_PROP(evt, "type", JS_STR(typeFound));
	SET_PROP(evt, "which", JS_INT(glfwKey));
	SET_PROP(evt, "scancode", JS_INT(scancode));
	
	V8_VAR_VAL argv[2] = { JS_STR(typeFound), evt };
	_emit(window, 2, argv);
	
}


void charCB(GLFWwindow* window, unsigned codepoint) { NAN_HS;
	
	V8_VAR_OBJ evt = Nan::New<Object>();
	
	SET_PROP(evt, "charCode", JS_INT(codepoint));
	
	V8_VAR_VAL argv[2] = { JS_STR("char"), evt };
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
