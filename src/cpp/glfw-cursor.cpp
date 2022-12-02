#include "glfw-common.hpp"
#include "glfw-cursor.hpp"


namespace glfw {

JS_METHOD(getCursorPos) { NAPI_ENV; THIS_WINDOW;
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	
	Napi::Object obj = Napi::Object::New(env);
	obj.Set("x", JS_NUM(x));
	obj.Set("y", JS_NUM(y));
	
	RET_VALUE(obj);
}


JS_METHOD(setCursorPos) { NAPI_ENV; THIS_WINDOW;
	REQ_INT32_ARG(1, x);
	REQ_INT32_ARG(2, y);
	
	glfwSetCursorPos(window, x, y);
	RET_UNDEFINED;
}

// TODO
// JS_METHOD(createCursor) { NAPI_ENV; THIS_WINDOW;
// 	glfwCreateCursor(const GLFWimage* image, int xhot, int yhot);
// }
// JS_METHOD(createStandardCursor) { NAPI_ENV; THIS_WINDOW;
// 	glfwCreateStandardCursor(int shape);
// }
// JS_METHOD(destroyCursor) { NAPI_ENV; THIS_WINDOW;
// 	glfwDestroyCursor(GLFWcursor* cursor);
// }
// JS_METHOD(setCursor) { NAPI_ENV; THIS_WINDOW;
// 	glfwSetCursor(GLFWwindow* window, GLFWcursor* cursor);
// }

} // namespace glfw
