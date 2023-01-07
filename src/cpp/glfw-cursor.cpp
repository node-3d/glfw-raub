#include "glfw-common.hpp"
#include "glfw-cursor.hpp"


namespace glfw {

DBG_EXPORT JS_METHOD(getCursorPos) { NAPI_ENV; THIS_WINDOW;
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	
	Napi::Object obj = JS_OBJECT;
	obj.Set("x", JS_NUM(x));
	obj.Set("y", JS_NUM(y));
	
	RET_VALUE(obj);
}


DBG_EXPORT JS_METHOD(setCursorPos) { NAPI_ENV; THIS_WINDOW;
	REQ_INT32_ARG(1, x);
	REQ_INT32_ARG(2, y);
	
	glfwSetCursorPos(window, x, y);
	RET_UNDEFINED;
}


DBG_EXPORT JS_METHOD(createCursor) { NAPI_ENV; THIS_WINDOW;
	REQ_OBJ_ARG(0, icon);
	
	if (!(icon.Has("width") && icon.Has("height"))) {
		RET_UNDEFINED;
	}
	
	bool noflip = icon.Has("noflip") ? icon.Get("noflip").ToBoolean().Value() : false;
	
	GLFWimage image;
	image.width = icon.Get("width").ToNumber().Int32Value();
	image.height = icon.Get("height").ToNumber().Int32Value();
	
	uint8_t *src = reinterpret_cast<unsigned char*>(getData(env, icon));
	
	GLFWcursor *cursor = nullptr;
	
	if (!noflip) {
		uint8_t *dest = new uint8_t[image.width * image.height * 4];
		int32_t lastY = image.height - 1;
		for (int32_t y = 0; y < image.height; y++) {
			for (int32_t x = 0; x < image.width; x++) {
				int32_t iForward = (y * image.width + x) << 2;
				int32_t iBackward = ((lastY - y) * image.width + x) << 2;
				dest[iForward + 0] = src[iBackward + 0];
				dest[iForward + 1] = src[iBackward + 1];
				dest[iForward + 2] = src[iBackward + 2];
				dest[iForward + 3] = src[iBackward + 3];
			}
		}
		image.pixels = dest;
		cursor = glfwCreateCursor(&image, image.width / 2, image.height / 2);
		delete [] dest;
	} else {
		image.pixels = src;
		cursor = glfwCreateCursor(&image, image.width / 2, image.height / 2);
	}
	
	RET_NUM(reinterpret_cast<uint64_t>(cursor));
}

DBG_EXPORT JS_METHOD(createStandardCursor) { NAPI_ENV;
	REQ_INT32_ARG(0, shape);
	GLFWcursor *cursor = glfwCreateStandardCursor(shape);
	RET_NUM(reinterpret_cast<uint64_t>(cursor));
}


DBG_EXPORT JS_METHOD(destroyCursor) { NAPI_ENV;
	REQ_OFFS_ARG(0, cursorPtr);
	GLFWcursor *cursor = reinterpret_cast<GLFWcursor*>(cursorPtr);
	glfwDestroyCursor(cursor);
	RET_UNDEFINED;
}


DBG_EXPORT JS_METHOD(setCursor) { NAPI_ENV; THIS_WINDOW;
	REQ_OFFS_ARG(1, cursorPtr);
	GLFWcursor *cursor = reinterpret_cast<GLFWcursor*>(cursorPtr);
	glfwSetCursor(window, cursor);
	RET_UNDEFINED;
}

} // namespace glfw
