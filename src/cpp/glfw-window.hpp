#ifndef _GLFW_WINDOW_HPP_
#define _GLFW_WINDOW_HPP_

#include "glfw-common.hpp"


namespace glfw {
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
	
	using StatesIterator = std::function<void(WinState *)>;
	void forEachWindow(const StatesIterator& fn);
	
	void dropShare();
	void destroyAllWindows();
	
	JS_METHOD(windowHint);
	JS_METHOD(windowHintString);
	JS_METHOD(defaultWindowHints);
	JS_METHOD(createWindow);
	JS_METHOD(destroyWindow);
	JS_METHOD(setWindowTitle);
	JS_METHOD(setWindowIcon);
	JS_METHOD(getWindowSize);
	JS_METHOD(getWindowFrameSize);
	JS_METHOD(getWindowContentScale);
	JS_METHOD(setWindowSize);
	JS_METHOD(setWindowSizeLimits);
	JS_METHOD(setWindowAspectRatio);
	JS_METHOD(setWindowPos);
	JS_METHOD(getWindowPos);
	JS_METHOD(getWindowOpacity);
	JS_METHOD(setWindowOpacity);
	JS_METHOD(maximizeWindow);
	JS_METHOD(focusWindow);
	JS_METHOD(requestWindowAttention);
	JS_METHOD(getWindowMonitor);
	JS_METHOD(getFramebufferSize);
	JS_METHOD(iconifyWindow);
	JS_METHOD(restoreWindow);
	JS_METHOD(hideWindow);
	JS_METHOD(showWindow);
	JS_METHOD(windowShouldClose);
	JS_METHOD(setWindowShouldClose);
	JS_METHOD(getWindowAttrib);
	JS_METHOD(setWindowAttrib);
} // namespace glfw


#endif /* _GLFW_WINDOW_HPP_ */
