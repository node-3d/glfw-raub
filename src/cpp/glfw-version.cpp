#include "glfw-common.hpp"
#include "glfw-version.hpp"


namespace glfw {

JS_METHOD(getVersion) { NAPI_ENV;
	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	
	Napi::Object obj = Napi::Object::New(env);
	obj.Set("major", JS_NUM(major));
	obj.Set("minor", JS_NUM(minor));
	obj.Set("rev", JS_NUM(rev));
	
	RET_VALUE(obj);
}


JS_METHOD(getVersionString) { NAPI_ENV;
	const char *ver = glfwGetVersionString();
	RET_STR(ver);
}

} // namespace glfw
