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
	emitter(Napi::Persistent(_emitter)),
	context(_emitter.Env(), "GLFWEvent" ) {
		emitter.SuppressDestruct();
	}
	
	~WinState() {
		emitter.Reset();
	}
	
};

} // namespace glfw


#endif /* _WIN_STATE_HPP_ */
