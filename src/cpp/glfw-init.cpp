#include <iostream>
#include <locale.h>

#include "glfw-common.hpp"
#include "glfw-init.hpp"
#include "glfw-window.hpp"
#include "glfw-events.hpp"


namespace glfw {

bool isInited = false;

void errorCb(int error, const char* description) {
	std::cout << "GLFW Error " << error << ": " << description << std::endl;
}


// Cleanup resources
void deinit() {
	if (!isInited) {
		return;
	}
	
	destroyAllWindows();
	
	glfwSetJoystickCallback(nullptr);
	
	isInited = false;
	dropShare();
	
	glfwTerminate();
}


JS_METHOD(init) { NAPI_ENV;
	setlocale(LC_ALL, "");
	
	glfwSetErrorCallback(errorCb);
	
	isInited = glfwInit() == GLFW_TRUE;
	
	glfwSetJoystickCallback(joystickCb);
	
	RET_BOOL(isInited);
}


JS_METHOD(initHint) { NAPI_ENV;
	REQ_INT32_ARG(0, hint);
	REQ_INT32_ARG(1, value);
	
	glfwInitHint(hint, value);
	RET_UNDEFINED;
}


JS_METHOD(terminate) { NAPI_ENV;
	deinit();
	RET_UNDEFINED;
}


JS_METHOD(testScene) { NAPI_ENV;
	REQ_UINT32_ARG(0, width);
	REQ_UINT32_ARG(1, height);
	LET_FLOAT_ARG(2, z);
	
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	
	float ratio = static_cast<float>(width) / static_cast<float>(height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glRotatef(static_cast<float>(glfwGetTime()) * 50.f, 0.f, 0.f, 1.f);
	
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f + z, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f + z, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f + z, 0.6f, 0.f);
	glEnd();
	
	RET_UNDEFINED;
}

} // namespace glfw
