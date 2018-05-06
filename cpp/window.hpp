#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_


#include <event-emitter.hpp>


#include "common.hpp"


class Window : public EventEmitter {
	
public:
	
	// Public V8 init/deinit
	static void init(V8_VAR_OBJ target);
	
	static bool isWindow(V8_VAR_OBJ obj);
	
	// Destroy an instance from C++ land
	void _destroy();
	
protected:
	
	static NAN_METHOD(newCtor);
	
	static NAN_METHOD(destroy);
	static NAN_GETTER(isDestroyedGetter);
	
	static NAN_GETTER(widthGetter);
	static NAN_GETTER(heightGetter);
	
	~Window();
	
	
private:
	
	static V8_STORE_FT _protoWindow; // for inheritance
	static V8_STORE_FUNC _ctorWindow;
	
	bool _isDestroyed;
	
	int mouseX;
	int mouseY;
	
	GLFWwindow *_window;
	
};

#endif // _IMAGE_HPP_
