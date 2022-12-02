#ifndef _GLFW_JOYSTICK_HPP_
#define _GLFW_JOYSTICK_HPP_

#include "glfw-common.hpp"


namespace glfw {
	JS_METHOD(joystickPresent);
	JS_METHOD(getJoystickAxes);
	JS_METHOD(getJoystickButtons);
	JS_METHOD(getJoystickName);
	JS_METHOD(getJoystickGUID);
	JS_METHOD(getJoystickHats);
	JS_METHOD(joystickIsGamepad);
	JS_METHOD(testJoystick);
	JS_METHOD(updateGamepadMappings);
	JS_METHOD(getGamepadName);
	JS_METHOD(getGamepadState);
} // namespace glfw


#endif /* _GLFW_JOYSTICK_HPP_ */
