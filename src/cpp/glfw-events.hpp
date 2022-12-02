#ifndef _GLFW_EVENTS_HPP_
#define _GLFW_EVENTS_HPP_

#include "glfw-common.hpp"

struct GLFWwindow;

namespace glfw {
	JS_METHOD(pollEvents);
	JS_METHOD(waitEvents);
	JS_METHOD(waitEventsTimeout);
	JS_METHOD(postEmptyEvent);
	
	void windowPosCB(GLFWwindow *window, int xpos, int ypos);
	void windowSizeCB(GLFWwindow *window, int w, int h);
	void windowFramebufferSizeCB(GLFWwindow *window, int w, int h);
	void windowDropCB(GLFWwindow *window, int count, const char **paths);
	void windowCloseCB(GLFWwindow *window);
	void windowRefreshCB(GLFWwindow *window);
	void windowIconifyCB(GLFWwindow *window, int iconified);
	void windowFocusCB(GLFWwindow *window, int focused);
	void keyCB(GLFWwindow *window, int key, int scancode, int action, int mods);
	void charCB(GLFWwindow* window, unsigned codepoint);
	void cursorPosCB(GLFWwindow* window, double x, double y);
	void cursorEnterCB(GLFWwindow* window, int entered);
	void mouseButtonCB(GLFWwindow *window, int button, int action, int mods);
	void scrollCB(GLFWwindow *window, double xoffset, double yoffset);
	void joystickCb(int jid, int event);
} // namespace glfw


#endif /* _GLFW_EVENTS_HPP_ */
