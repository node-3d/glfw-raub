#ifndef COMMON_H_
#define COMMON_H_

// OpenGL Graphics Includes
#ifndef _WIN32
#define GLEW_STATIC
#endif
#include <GL/glew.h>

#define GLFW_NO_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

// NodeJS includes
#include <node.h>

#pragma warning(push)
#pragma warning(disable:4244)
#include <nan.h>
#pragma warning(pop)


#define JS_STR(...) Nan::New<v8::String>(__VA_ARGS__).ToLocalChecked()
#define JS_INT(val) Nan::New<v8::Integer>(val)
#define JS_NUM(val) Nan::New<v8::Number>(val)
#define JS_BOOL(val) (val) ? Nan::True() : Nan::False()
#define JS_RETHROW(tc) v8::Local<v8::Value>::New(tc.Exception());


#define REQ_ARGS(N)                                                     \
	if (info.Length() < (N))                                            \
		Nan::ThrowTypeError("Expected " #N " arguments");


#define REQ_STR_ARG(I, VAR)                                             \
	if (info.Length() <= (I) || !info[I]->IsString())                   \
		Nan::ThrowTypeError("Argument " #I " must be a string");        \
	v8::String::Utf8Value VAR(info[I]->ToString());


#define REQ_EXT_ARG(I, VAR)                                             \
	if (info.Length() <= (I) || !info[I]->IsExternal())                 \
		Nan::ThrowTypeError("Argument " #I " invalid");                 \
	v8::Local<v8::External> VAR = v8::Local<v8::External>::Cast(info[I]);


#define REQ_FUN_ARG(I, VAR)                                             \
	if (info.Length() <= (I) || !info[I]->IsFunction())                 \
		Nan::ThrowTypeError("Argument " #I " must be a function");      \
	v8::Local<Function> VAR = v8::Local<v8::Function>::Cast(info[I]);


#define REQ_ERROR_THROW(error)                                          \
	if (ret == error)                                                   \
		Nan::ThrowError(v8::String::New(#error));


#endif /* COMMON_H_ */
