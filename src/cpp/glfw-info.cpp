#include "glfw-common.hpp"
#include "glfw-info.hpp"


namespace glfw {

JS_METHOD(getError) { NAPI_ENV;
	const char *err;
	int code = glfwGetError(&err);
	
	if (code != GLFW_NO_ERROR) {
		RET_STR(err);
	}
	
	RET_NULL;
}


JS_METHOD(getFramebufferSize) { NAPI_ENV; THIS_WINDOW;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	Napi::Object obj = Napi::Object::New(env);
	obj.Set("width", JS_NUM(width));
	obj.Set("height", JS_NUM(height));
	
	RET_VALUE(obj);
}


JS_METHOD(swapBuffers) { NAPI_ENV; THIS_WINDOW;
	glfwSwapBuffers(window);
	RET_UNDEFINED;
}


JS_METHOD(swapInterval) { NAPI_ENV;
	REQ_INT32_ARG(0, interval);
	
	glfwSwapInterval(interval);
	RET_UNDEFINED;
}


JS_METHOD(extensionSupported) { NAPI_ENV;
	REQ_STR_ARG(0, str);
	
	RET_BOOL(glfwExtensionSupported(str.c_str()) == GLFW_TRUE);
}

} // namespace glfw
