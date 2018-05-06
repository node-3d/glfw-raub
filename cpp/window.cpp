#include <cstdlib>

#include "window.hpp"


using namespace v8;
using namespace node;
using namespace std;


// ------ Aux macros

#define THIS_WINDOW                                                            \
	Window *window = ObjectWrap::Unwrap<Window>(info.This());
	
#define THIS_CHECK                                                            \
	if (window->_isDestroyed) return;


// ------ Constructor and Destructor

Window::~Window() {
	
	_destroy();
	
}


void Window::_destroy() { DES_CHECK;
	
	_isDestroyed = true;
	
	EventEmitter::_destroy();
	
}


// ------ Methods and props

NAN_GETTER(Window::widthGetter) { THIS_WINDOW; THIS_CHECK;
	
}


NAN_GETTER(Window::heightGetter) { THIS_WINDOW; THIS_CHECK;
	
}


// ------ System methods and props for ObjectWrap

V8_STORE_FT Window::_protoWindow;
V8_STORE_FUNC Window::_ctorWindow;


void Window::init(V8_VAR_OBJ target) {
	
	V8_VAR_FT proto = Nan::New<FunctionTemplate>(newCtor);
	
	// class Window inherits EventEmitter
	V8_VAR_FT parent = Nan::New(EventEmitter::_protoEventEmitter);
	proto->Inherit(parent);
	
	proto->InstanceTemplate()->SetInternalFieldCount(1);
	proto->SetClassName(JS_STR("Window"));
	
	
	// Accessors
	V8_VAR_OT obj = proto->PrototypeTemplate();
	ACCESSOR_R(obj, isDestroyed);
	
	ACCESSOR_R(obj, width);
	ACCESSOR_R(obj, height);
	
	// -------- dynamic
	
	Nan::SetPrototypeMethod(proto, "destroy", destroy);
	
	// -------- static
	
	V8_VAR_FUNC ctor = Nan::GetFunction(proto).ToLocalChecked();
	
	_protoWindow.Reset(proto);
	_ctorWindow.Reset(ctor);
	
	Nan::Set(target, JS_STR("Window"), ctor);
	
	
}


bool Window::isWindow(V8_VAR_OBJ obj) {
	return Nan::New(_protoWindow)->HasInstance(obj);
}


NAN_METHOD(Window::newCtor) {
	
	CTOR_CHECK("Window");
	
	Window *window = new Window();
	window->Wrap(info.This());
	
	RET_VALUE(info.This());
	
}


NAN_METHOD(Window::destroy) { THIS_WINDOW; THIS_CHECK;
	
	window->emit("destroy");
	
	window->_destroy();
	
}


NAN_GETTER(Window::isDestroyedGetter) { THIS_WINDOW;
	
	RET_VALUE(JS_BOOL(window->_isDestroyed));
	
}

