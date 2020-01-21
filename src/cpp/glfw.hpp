#ifndef _GLFW_HPP_
#define _GLFW_HPP_

#include <addon-tools.hpp>


namespace glfw {

std::string intToString(int number);
std::string floatToString(float number);
std::string buttonToString(unsigned char c);

void deinit();

JS_METHOD(hideConsole);
JS_METHOD(showConsole);
JS_METHOD(init);
JS_METHOD(initHint);
JS_METHOD(terminate);
JS_METHOD(getVersion);
JS_METHOD(getVersionString);
JS_METHOD(getError);
JS_METHOD(getTime);
JS_METHOD(setTime);
JS_METHOD(getMonitors);
JS_METHOD(getPrimaryMonitor);
JS_METHOD(windowHint);
JS_METHOD(windowHintString);
JS_METHOD(defaultWindowHints);
JS_METHOD(joystickPresent);
JS_METHOD(getJoystickAxes);
JS_METHOD(getJoystickButtons);
JS_METHOD(getJoystickName);
JS_METHOD(getJoystickGUID);
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
JS_METHOD(setInputMode);
JS_METHOD(getInputMode);
JS_METHOD(pollEvents);
JS_METHOD(waitEvents);
JS_METHOD(waitEventsTimeout);
JS_METHOD(postEmptyEvent);
JS_METHOD(getKey);
JS_METHOD(getMouseButton);
JS_METHOD(getCursorPos);
JS_METHOD(setCursorPos);
JS_METHOD(makeContextCurrent);
JS_METHOD(getCurrentContext);
JS_METHOD(swapBuffers);
JS_METHOD(swapInterval);
JS_METHOD(extensionSupported);
JS_METHOD(rawMouseMotionSupported);
JS_METHOD(getKeyName);
JS_METHOD(getKeyScancode);
// TODO
// JS_METHOD(createCursor);
// JS_METHOD(createStandardCursor);
// JS_METHOD(destroyCursor);
// JS_METHOD(setCursor);
JS_METHOD(getJoystickHats);
JS_METHOD(joystickIsGamepad);
JS_METHOD(updateGamepadMappings);
JS_METHOD(getGamepadName);
JS_METHOD(getGamepadState);
JS_METHOD(setClipboardString);
JS_METHOD(getClipboardString);
JS_METHOD(getTimerValue);
JS_METHOD(getTimerFrequency);

JS_METHOD(platformWindow);
JS_METHOD(platformContext);

JS_METHOD(testScene);
JS_METHOD(testJoystick);

} // namespace glfw


#endif /* _GLFW_HPP_ */
