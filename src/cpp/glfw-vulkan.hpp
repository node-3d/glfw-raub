#ifndef _GLFW_VULKAN_HPP_
#define _GLFW_VULKAN_HPP_

#include "glfw-common.hpp"


namespace glfw {
	DBG_EXPORT JS_METHOD(vulkanSupported);
	DBG_EXPORT JS_METHOD(getRequiredInstanceExtensions);
	DBG_EXPORT JS_METHOD(getInstanceProcAddress);
	DBG_EXPORT JS_METHOD(getPhysicalDevicePresentationSupport);
	DBG_EXPORT JS_METHOD(createWindowSurface);
	
	DBG_EXPORT JS_METHOD(vulkanCreateInstance);
	DBG_EXPORT JS_METHOD(vulkanCreateDevice);
	DBG_EXPORT JS_METHOD(vulkanDestroyDevice);
	DBG_EXPORT JS_METHOD(vulkanDestroyInstance);
} // namespace glfw


#endif /* _GLFW_VULKAN_HPP_ */
