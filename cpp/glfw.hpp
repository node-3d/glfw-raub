#ifndef _GLFW_HPP_
#define _GLFW_HPP_


#include <addon-tools.hpp>


namespace glfw {

std::string intToString(int number);
std::string floatToString(float number);
std::string buttonToString(unsigned char c);

NAN_METHOD(init);
NAN_METHOD(terminate);
NAN_METHOD(getVersion);
NAN_METHOD(getVersionString);
NAN_METHOD(getTime);
NAN_METHOD(setTime);
NAN_METHOD(getMonitors);
NAN_METHOD(extensionSupported);

NAN_METHOD(testJoystick);
NAN_METHOD(joystickPresent);
NAN_METHOD(getJoystickAxes);
NAN_METHOD(getJoystickButtons);
NAN_METHOD(getJoystickName);

NAN_METHOD(createWindow);
NAN_METHOD(destroyWindow);
NAN_METHOD(testScene);

NAN_METHOD(platformWindow);
NAN_METHOD(platformContext);
NAN_METHOD(makeContextCurrent);
NAN_METHOD(getCurrentContext);
NAN_METHOD(getFramebufferSize);

NAN_METHOD(windowHint);
NAN_METHOD(defaultWindowHints);
NAN_METHOD(setWindowTitle);
NAN_METHOD(setWindowIcon);
NAN_METHOD(getWindowSize);
NAN_METHOD(setWindowSize);
NAN_METHOD(setWindowPos);
NAN_METHOD(getWindowPos);
NAN_METHOD(iconifyWindow);
NAN_METHOD(restoreWindow);
NAN_METHOD(hideWindow);
NAN_METHOD(showWindow);
NAN_METHOD(windowShouldClose);
NAN_METHOD(setWindowShouldClose);
NAN_METHOD(getWindowAttrib);

NAN_METHOD(pollEvents);
NAN_METHOD(waitEvents);

NAN_METHOD(setInputMode);
NAN_METHOD(getKey);
NAN_METHOD(getMouseButton);
NAN_METHOD(getCursorPos);
NAN_METHOD(setCursorPos);

NAN_METHOD(swapBuffers);
NAN_METHOD(swapInterval);

void deinit();

} // namespace glfw


#endif /* _GLFW_HPP_ */
