#include <cstdlib>
#include <sstream>

#include "platform.hpp"
#include "win-state.hpp"
#include "events.hpp"


using namespace std;


#define THIS_STATE                                                            \
	WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));

#define STATE_ENV                                                             \
	Napi::Env env = state->emitter.Env();

namespace glfw {

const char typeKeyup[] = "keyup";
const char typeKeydown[] = "keydown";
const char typeMouseenter[] = "mouseenter";
const char typeMouseleave[] = "mouseleave";
const char typeFocusin[] = "focusin";
const char typeFocusout[] = "focusout";
const char typeFocus[] = "focus";
const char typeBlur[] = "blur";


inline void _emit(WinState *state, int argc, Napi::Value argv[]) {
	
	if ( ! state->window ) {
		return;
	}
	
	Napi::Value emitValue = state->emitter.Get("emit");
	
	if (emitValue) {
		Napi::Function emitFunc = emitValue.As<Napi::Function>();
		if (emitFunc) {
			std::vector<napi_value> args;
			for (int i = 0; i < argc; i++) {
				args.push_back(argv[i]);
			}
			emitFunc.MakeCallback(
				state->emitter.Value(),
				args,
				state->context
			);
		}
	}
	
}


void fillMouse(Napi::Object evt, GLFWwindow *window, int mods = -1) {
	
	WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));
	
	int buttons = 0;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		buttons |= 1;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		buttons |= 2;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
		buttons |= 4;
	}
	evt.Set("buttons", static_cast<double>(buttons));
	
	evt.Set("clientX", static_cast<double>(state->mouseX));
	evt.Set("clientY", static_cast<double>(state->mouseY));
	evt.Set("pageX", static_cast<double>(state->mouseX));
	evt.Set("pageY", static_cast<double>(state->mouseY));
	evt.Set("x", static_cast<double>(state->mouseX));
	evt.Set("y", static_cast<double>(state->mouseY));
	
	if (mods > -1) {
		evt.Set("shiftKey", static_cast<bool>(mods & GLFW_MOD_SHIFT));
		evt.Set("ctrlKey", static_cast<bool>(mods & GLFW_MOD_CONTROL));
		evt.Set("altKey", static_cast<bool>(mods & GLFW_MOD_ALT));
		evt.Set("metaKey", static_cast<bool>(mods & GLFW_MOD_SUPER));
	} else {
		evt.Set("ctrlKey", static_cast<bool>(
			glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS
		));
		evt.Set("shiftKey", static_cast<bool>(
			glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS
		));
		evt.Set("altKey", static_cast<bool>(
			glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS
		));
		evt.Set("metaKey", static_cast<bool>(
			glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS
		));
	}
	
}


void fillKey(Napi::Object evt, int glfwKey, int scancode, int action, int mods) {
	
	const char *keyName = glfwGetKeyName(glfwKey, scancode);
	
	evt.Set("repeat", static_cast<bool>(action == GLFW_REPEAT));
	evt.Set("altKey", static_cast<bool>(mods & GLFW_MOD_ALT));
	evt.Set("ctrlKey", static_cast<bool>(mods & GLFW_MOD_CONTROL));
	evt.Set("metaKey", static_cast<bool>(mods & GLFW_MOD_SUPER));
	evt.Set("shiftKey", static_cast<bool>(mods & GLFW_MOD_SHIFT));
	
	
	if (keyName) {
		evt.Set("code", keyName);
		evt.Set("key", keyName);
	} else {
		evt.Set("code", evt.Env().Null());
		evt.Set("key", evt.Env().Null());
	}
	
	evt.Set("which", static_cast<double>(glfwKey));
	
}


void windowPosCB(GLFWwindow *window, int xpos, int ypos) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	Napi::Object evt = Napi::Object::New(env);
	evt.Set("type", "move");
	evt.Set("x", static_cast<double>(xpos));
	evt.Set("y", static_cast<double>(ypos));
	
	Napi::Value argv[2] = { JS_STR("move"), evt };
	_emit(state, 2, argv);
	
}


void windowSizeCB(GLFWwindow *window, int w, int h) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	Napi::Object evt = Napi::Object::New(env);
	evt.Set("type", "resize");
	evt.Set("width", static_cast<double>(w));
	evt.Set("height", static_cast<double>(h));
	
	Napi::Value argv[2] = { JS_STR("resize"), evt };
	_emit(state, 2, argv);
	
}


void windowFramebufferSizeCB(GLFWwindow *window, int w, int h) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	Napi::Object evt = Napi::Object::New(env);
	evt.Set("type", "fbresize");
	evt.Set("width", static_cast<double>(w));
	evt.Set("height", static_cast<double>(h));
	
	Napi::Value argv[2] = { JS_STR("fbresize"), evt };
	_emit(state, 2, argv);
	
}


void windowDropCB(GLFWwindow *window, int count, const char **paths) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	Napi::Object evt = Napi::Object::New(env);
	fillMouse(evt, window);
	evt.Set("type", "drop");
	
	Napi::Object dataTransfer = Napi::Object::New(env);
	evt.Set("dataTransfer", dataTransfer);
	
	Napi::Array list = Napi::Array();
	for (int i = 0; i < count; i++) {
		list.Set(i, paths[i]);
	}
	
	evt.Set("dropEffect", "none");
	evt.Set("effectAllowed", "all");
	evt.Set("files", list);
	evt.Set("items", list);
	evt.Set("types", Napi::Array());
	
	Napi::Value argv[2] = { JS_STR("drop"), evt };
	_emit(state, 2, argv);
	
}


void windowCloseCB(GLFWwindow *window) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	Napi::Object evt = Napi::Object::New(env);
	evt.Set("type", "quit");
	
	Napi::Value argv[2] = { JS_STR("quit"), evt };
	_emit(state, 2, argv);
	
	state->window = nullptr;
	
}


void windowRefreshCB(GLFWwindow *window) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	Napi::Object evt = Napi::Object::New(env);
	evt.Set("type", "refresh");
	
	Napi::Value argv[2] = { JS_STR("refresh"), evt };
	_emit(state, 2, argv);
	
}


void windowIconifyCB(GLFWwindow *window, int iconified) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	Napi::Object evt = Napi::Object::New(env);
	evt.Set("type", "iconify");
	evt.Set("iconified", static_cast<bool>(iconified));
	
	Napi::Value argv[2] = { JS_STR("iconify"), evt };
	_emit(state, 2, argv);
	
}


void windowFocusCB(GLFWwindow *window, int focused) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	const char *typeFound;
	const char *typeDirFound;
	if (focused) {
		typeFound = typeFocus;
		typeDirFound = typeFocusin;
	} else {
		typeFound = typeBlur;
		typeDirFound = typeFocusout;
	}
	
	Napi::Object evt1 = Napi::Object::New(env);
	evt1.Set("type", "typeFound");
	Napi::Value argv1[2] = { JS_STR(typeFound), evt1 };
	_emit(state, 2, argv1);
	
	Napi::Object evt2 = Napi::Object::New(env);
	evt2.Set("type", "typeDirFound");
	Napi::Value argv2[2] = { JS_STR(typeDirFound), evt2 };
	_emit(state, 2, argv2);
	
}


void keyCB(GLFWwindow *window, int glfwKey, int scancode, int action, int mods) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	const char *keyName = glfwGetKeyName(glfwKey, scancode);
	
	bool isAltPressed = mods & GLFW_MOD_ALT;
	bool isCtrlPressed = mods & GLFW_MOD_CONTROL;
	
	if (
		keyName && (action == GLFW_PRESS || action == GLFW_REPEAT) &&
		! (isAltPressed || isCtrlPressed)
	) {
		WinState *state = reinterpret_cast<WinState*>(glfwGetWindowUserPointer(window));
		state->pendingKey = glfwKey;
		state->pendingScan = scancode;
		state->pendingAction = action;
		state->pendingMods = mods;
		return;
	}
	
	const char *typeFound;
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		typeFound = typeKeydown;
	} else {
		typeFound = typeKeyup;
	}
	
	Napi::Object evt = Napi::Object::New(env);
	fillKey(evt, glfwKey, scancode, action, mods);
	evt.Set("charCode", static_cast<double>(0));
	evt.Set("type", "typeFound");
	
	Napi::Value argv[2] = { JS_STR(typeFound), evt };
	_emit(state, 2, argv);
	
}


void charCB(GLFWwindow* window, unsigned codepoint) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	if ( ! state->pendingKey ) {
		return;
	}
	
	Napi::Object evt = Napi::Object::New(env);
	fillKey(
		evt,
		state->pendingKey,
		state->pendingScan,
		state->pendingAction,
		state->pendingMods
	);
	evt.Set("charCode", static_cast<double>(codepoint));
	evt.Set("type", "typeKeydown");
	
	state->pendingKey = 0;
	state->pendingScan = 0;
	state->pendingAction = 0;
	state->pendingMods = 0;
	
	Napi::Value argv[2] = { JS_STR(typeKeydown), evt };
	_emit(state, 2, argv);
	
}


void cursorPosCB(GLFWwindow* window, double x, double y) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	
	if (x < 0 || x >= w || y < 0 || y >= h) {
		return;
	}
	
	int newX = static_cast<int>(x);
	int newY = static_cast<int>(y);
	
	Napi::Object evt = Napi::Object::New(env);
	evt.Set("movementX", static_cast<double>(newX - state->mouseX));
	evt.Set("movementY", static_cast<double>(newY - state->mouseY));
	
	state->mouseX = newX;
	state->mouseY = newY;
	
	fillMouse(evt, window);
	evt.Set("type", "mousemove");
	
	Napi::Value argv[2] = { JS_STR("mousemove"), evt };
	_emit(state, 2, argv);
	
}


void cursorEnterCB(GLFWwindow* window, int entered) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	const char *typeFound;
	if (entered) {
		typeFound = typeMouseenter;
	} else {
		typeFound = typeMouseleave;
	}
	
	Napi::Object evt = Napi::Object::New(env);
	fillMouse(evt, window);
	evt.Set("type", "typeFound");
	
	Napi::Value argv[2] = { JS_STR(typeFound), evt };
	_emit(state, 2, argv);
	
}


void mouseButtonCB(GLFWwindow *window, int button, int action, int mods) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	int btnId = button;
	if (button == GLFW_MOUSE_BUTTON_2) {
		btnId = 2;
	} else if (button == GLFW_MOUSE_BUTTON_3) {
		btnId = 1;
	}
	int which = btnId + 1;
	
	Napi::Object evt1 = Napi::Object::New(env);
	fillMouse(evt1, window);
	evt1.Set("type", action ? "mousedown" : "mouseup");
	evt1.Set("button", static_cast<double>(btnId));
	
	evt1.Set("which", static_cast<double>(which));
	
	Napi::Value argv[2] = { JS_STR(action ? "mousedown" : "mouseup"), evt1 };
	_emit(state, 2, argv);
	
	if ( ! action ) {
		
		Napi::Object evt2 = Napi::Object::New(env);
		fillMouse(evt2, window);
		evt2.Set("type", "click");
		evt2.Set("button", static_cast<double>(btnId));
		evt2.Set("which", static_cast<double>(which));
		
		Napi::Value argv[2] = { JS_STR("click"), evt2 };
		_emit(state, 2, argv);
		
	}
	
}


void scrollCB(GLFWwindow *window, double xoffset, double yoffset) {
	THIS_STATE; STATE_ENV; NAPI_HS;
	
	Napi::Object evt = Napi::Object::New(env);
	fillMouse(evt, window);
	evt.Set("type", "wheel");
	evt.Set("deltaX", static_cast<double>(static_cast<int>(xoffset * 100)));
	evt.Set("deltaY", static_cast<double>(static_cast<int>(yoffset * 100)));
	evt.Set("deltaZ", static_cast<double>(0));
	evt.Set("wheelDeltaX", static_cast<double>(static_cast<int>(xoffset * 120)));
	evt.Set("wheelDeltaY", static_cast<double>(static_cast<int>(yoffset * 120)));
	evt.Set("wheelDelta", static_cast<double>(static_cast<int>(yoffset * 120)));
	
	Napi::Value argv[2] = { JS_STR("wheel"), evt };
	_emit(state, 2, argv);
	
	Napi::Value argv2[2] = { JS_STR("mousewheel"), evt };
	_emit(state, 2, argv2);
	
}


} // namespace glfw
