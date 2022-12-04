#ifndef _GLFW_VULKAN_HPP_
#define _GLFW_VULKAN_HPP_

#include "glfw-common.hpp"


namespace glfw {
	JS_METHOD(vulkanSupported);
	JS_METHOD(getRequiredInstanceExtensions);
	JS_METHOD(getInstanceProcAddress);
	JS_METHOD(getPhysicalDevicePresentationSupport);
	JS_METHOD(createWindowSurface);
	
	JS_METHOD(vulkanCreateInstance);
	JS_METHOD(vulkanCreateDevice);
	JS_METHOD(vulkanDestroyDevice);
	JS_METHOD(vulkanDestroyInstance);
} // namespace glfw


#endif /* _GLFW_VULKAN_HPP_ */
