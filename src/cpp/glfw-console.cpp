#include "glfw-console.hpp"
#include "glfw-platform.hpp"


namespace glfw {

DBG_EXPORT JS_METHOD(hideConsole) { NAPI_ENV;
	#ifdef _WIN32
		auto wnd = FindWindowA("ConsoleWindowClass", nullptr);
		ShowWindow(wnd, 0);
	#endif
	RET_UNDEFINED;
}

DBG_EXPORT JS_METHOD(showConsole) { NAPI_ENV;
	#ifdef _WIN32
		auto wnd = FindWindowA("ConsoleWindowClass", nullptr);
		ShowWindow(wnd, 1);
	#endif
	RET_UNDEFINED;
}

} // namespace glfw
