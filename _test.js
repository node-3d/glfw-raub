'use strict';

const util = require('util');

const glfw = require('.');


const version = glfw.GetVersion();
console.log(`glfw ${version.major}.${version.minor}.${version.rev}`);
console.log(`glfw version-string: ${glfw.GetVersionString()}`);

// Initialize GLFW
if ( ! glfw.Init() ) {
	console.log('Failed to initialize GLFW');
	process.exit(-1);
}

// Open OpenGL window
glfw.DefaultWindowHints();

const width  = 640;
const height = 480;
const wnd = glfw.CreateWindow(width, height, 'Test');

if ( ! wnd ) {
	console.log('Failed to open GLFW window');
	glfw.Terminate();
	process.exit(-1);
}

glfw.MakeContextCurrent(wnd);

glfw.SetWindowTitle('GLFW Simple Test');

// testing events
glfw.events.on('keydown', e => console.log(`[keydown] ${util.inspect(e)}`));
glfw.events.on('mousedown', e => console.log(`[mousedown] ${util.inspect(e)}`));
glfw.events.on('mousemove', e => console.log(`[mousemove] ${e.x}, ${e.y}`));
glfw.events.on('mousewheel', e => console.log(`[mousewheel] ${e.position}`));
glfw.events.on('resize', e => console.log(`[resize] ${e.width}, ${e.height}`));


//can only be called after window creation!
const major = glfw.GetWindowAttrib(wnd, glfw.CONTEXT_VERSION_MAJOR);
const minor = glfw.GetWindowAttrib(wnd, glfw.CONTEXT_VERSION_MINOR);
const rev   = glfw.GetWindowAttrib(wnd, glfw.CONTEXT_REVISION);
const prof  = glfw.GetWindowAttrib(wnd, glfw.OPENGL_PROFILE);
console.log(`GL ${major}.${minor}.${rev} Profile: ${prof}`);

// Enable vertical sync (on cards that support it)
glfw.SwapInterval(0 /*1*/); // 0 for vsync off


const start = glfw.GetTime();

while( ! glfw.WindowShouldClose(wnd) && ! glfw.GetKey(wnd, glfw.KEY_ESCAPE) ) {
	
	// Get wnd size (may be different than the requested size)
	const wsize = glfw.GetFramebufferSize(wnd);
	
	glfw.testScene(wsize.width, wsize.height);
	
	// Swap buffers
	glfw.SwapBuffers(wnd);
	glfw.PollEvents();
	
}

// Close OpenGL window and terminate GLFW
glfw.DestroyWindow(wnd);
glfw.Terminate();

process.exit(0);
