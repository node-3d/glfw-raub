#ifndef _GLFW_HPP_
#define _GLFW_HPP_


#include <addon-tools.hpp>


namespace glfw {

std::string intToString(int number);
std::string floatToString(float number);
std::string buttonToString(unsigned char c);

JS_METHOD(init);
JS_METHOD(terminate);
JS_METHOD(getVersion);
JS_METHOD(getVersionString);
JS_METHOD(getTime);
JS_METHOD(setTime);
JS_METHOD(getMonitors);
JS_METHOD(extensionSupported);

JS_METHOD(testJoystick);
JS_METHOD(joystickPresent);
JS_METHOD(getJoystickAxes);
JS_METHOD(getJoystickButtons);
JS_METHOD(getJoystickName);

JS_METHOD(createWindow);
JS_METHOD(destroyWindow);
JS_METHOD(testScene);

JS_METHOD(platformWindow);
JS_METHOD(platformContext);
JS_METHOD(makeContextCurrent);
JS_METHOD(getCurrentContext);
JS_METHOD(getFramebufferSize);

JS_METHOD(windowHint);
JS_METHOD(defaultWindowHints);
JS_METHOD(setWindowTitle);
JS_METHOD(setWindowIcon);
JS_METHOD(getWindowSize);
JS_METHOD(setWindowSize);
JS_METHOD(setWindowPos);
JS_METHOD(getWindowPos);
JS_METHOD(iconifyWindow);
JS_METHOD(restoreWindow);
JS_METHOD(hideWindow);
JS_METHOD(showWindow);
JS_METHOD(windowShouldClose);
JS_METHOD(setWindowShouldClose);
JS_METHOD(getWindowAttrib);

JS_METHOD(pollEvents);
JS_METHOD(waitEvents);

JS_METHOD(setInputMode);
JS_METHOD(getKey);
JS_METHOD(getMouseButton);
JS_METHOD(getCursorPos);
JS_METHOD(setCursorPos);

JS_METHOD(swapBuffers);
JS_METHOD(swapInterval);

void deinit();

} // namespace glfw


#endif /* _GLFW_HPP_ */
