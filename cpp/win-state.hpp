#ifndef _WIN_STATE_HPP_
#define _WIN_STATE_HPP_


#include <addon-tools.hpp>

namespace glfw {

// Window info
struct WinState {
	
	int mouseX;
	int mouseY;
	V8_STORE_OBJ events;
	
	explicit WinState(V8_VAR_OBJ obj) {
		mouseX = 0;
		mouseY = 0;
		events.Reset(obj);
	}
	
	~WinState() {
		events.Reset();
	}
	
};

} // namespace glfw


#endif /* _WIN_STATE_HPP_ */
