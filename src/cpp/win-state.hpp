#ifndef _WIN_STATE_HPP_
#define _WIN_STATE_HPP_


#include <addon-tools.hpp>

namespace glfw {

// Window info
struct WinState {
	
	int mouseX;
	int mouseY;
	int pendingKey;
	int pendingScan;
	int pendingAction;
	int pendingMods;
	GLFWwindow *window;
	Napi::ObjectReference emitter;
	Napi::AsyncContext context;
	
	WinState(GLFWwindow *_window, Napi::Object _emitter):
	mouseX(0),
	mouseY(0),
	pendingKey(0),
	pendingScan(0),
	pendingAction(0),
	window(_window),
	context(_emitter.Env(), "GLFWEvent") {
		emitter.Reset(_emitter, 1);
	}
	
	~WinState() {
		emitter.Reset();
	}
	
};

} // namespace glfw


#endif /* _WIN_STATE_HPP_ */
