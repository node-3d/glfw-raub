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
	V8_STORE_OBJ events;
	
	explicit WinState(V8_VAR_OBJ obj) {
		mouseX = 0;
		mouseY = 0;
		pendingKey = 0;
		pendingScan = 0;
		pendingAction = 0;
		events.Reset(obj);
	}
	
	~WinState() {
		events.Reset();
	}
	
};

} // namespace glfw


#endif /* _WIN_STATE_HPP_ */
