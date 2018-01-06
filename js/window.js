'use strict';

const EventEmitter = require('events');

const glfw = require('../binary/glfw');


class Window extends EventEmitter {
	
	constructor(opts = {}) {
		
		super();
		
		this._title = opts.title;
		
		if ( ! this.title ) {
			const pathMatch2 = process.mainModule.filename.replace(/\\/g, '/').match(
				/(\/(.*))*\/(.*?)\/[^\/]*$/
			);
			this._title = pathMatch2 ? pathMatch2[pathMatch2.length - 1] : 'Untitled';
		}
		
		this._width = opts.width || 800;
		this._height = opts.height || 600;
		
		this._display = opts.display || undefined;
		this._vsync = opts.vsync ? 1 : 0; // 0 for vsync off
		
		this._fullscreen = opts.fullscreen ? true : false;
		
		const attribs = this._fullscreen ? glfw.WINDOW : glfw.FULLSCREEN;
		
		// we use OpenGL 2.1, GLSL 1.20. Comment this for now as this is for GLSL 1.50
		//glfw.OpenWindowHint(glfw.OPENGL_FORWARD_COMPAT, 1);
		//glfw.OpenWindowHint(glfw.OPENGL_VERSION_MAJOR, 3);
		//glfw.OpenWindowHint(glfw.OPENGL_VERSION_MINOR, 2);
		//glfw.OpenWindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE);
		glfw.WindowHint(glfw.RESIZABLE, 1);
		glfw.WindowHint(glfw.VISIBLE, 1);
		glfw.WindowHint(glfw.DECORATED, 1);
		glfw.WindowHint(glfw.RED_BITS, 8);
		glfw.WindowHint(glfw.GREEN_BITS, 8);
		glfw.WindowHint(glfw.BLUE_BITS, 8);
		glfw.WindowHint(glfw.DEPTH_BITS, 24);
		glfw.WindowHint(glfw.REFRESH_RATE, 0);
		
		
		const emitter = { emit: (t, e) => this.emit(t, e) };
		
		if (this._display !== undefined) {
			this._window = glfw.CreateWindow(this._width, this._height, emitter, this._title, this._display);
		} else {
			this._window = glfw.CreateWindow(this._width, this._height, emitter, this._title);
		}
		
		if ( ! this._window ) {
			throw new Error('Failed to open GLFW window');
		}
		
		//can only be called after window creation!
		this._major = glfw.GetWindowAttrib(this._window, glfw.CONTEXT_VERSION_MAJOR);
		this._minor = glfw.GetWindowAttrib(this._window, glfw.CONTEXT_VERSION_MINOR);
		this._rev   = glfw.GetWindowAttrib(this._window, glfw.CONTEXT_REVISION);
		this._prof  = glfw.GetWindowAttrib(this._window, glfw.OPENGL_PROFILE);
		
		// Vertical sync (on cards that support it)
		glfw.SwapInterval(this._vsync);
		
	}
	
	
	get handle() { return this._window; }
	
	get width() { return this._width; }
	get height() { return this._height; }
	
	get w() { return this.width; }
	get h() { return this.height; }
	get wh() { return [this.width, this.height]; }
	
	get size() {
		const size = glfw.GetWindowSize(this._window);
		this._width = size.width;
		this._height = size.height;
		return size;
	}
	set size(v) {
		this._width = v.width;
		this._height = v.height;
		glfw.SetWindowSize(this._window, v.width, v.height);
	}
	
	get title()  { return this._title; }
	set title(v) {
		this._title = v || 'Untitled';
		glfw.SetWindowTitle(this._window, this._title);
	}
	
	get version() {
		return `GL ${this._major}.${this._minor}.${this._rev} Profile: ${this._prof}`;
	}
	
	get shouldClose() { return glfw.WindowShouldClose(this._window); }
	set shouldClose(v) { glfw.SetWindowShouldClose(v); }
	
	get platformWindow() { return glfw.PlatformWindow(this._window); }
	get platformContext() { return glfw.PlatformContext(this._window); }
	
	get pos() {
		const pos = glfw.GetWindowPos(this._window);
		this._x = pos.x;
		this._y = pos.y;
		return pos;
	}
	set pos(v) {
		this._x = v.x;
		this._y = v.y;
		glfw.SetWindowSize(this._window, v.x, v.y);
	}
	
	get framebufferSize() { return glfw.GetFramebufferSize(this._window); }
	
	get currentContext() { return glfw.GetCurrentContext(this._window); }
	
	get cursorPos() { return glfw.GetCursorPos(this._window); }
	set cursorPos(v) { glfw.SetCursorPos(this._window, v.x, v.y); }
	
	
	emit(type, event) {
		
		if (event) {
			event.preventDefault  = () => {};
			event.stopPropagation = () => {};
		}
		
		super.emit(type, event);
		
	}
	
	
	getKey(key) {
		return glfw.GetKey(this._window, key);
	}
	
	
	getMouseButton(button) {
		return glfw.GetMouseButton(this._window, button);
	}
	
	
	getWindowAttrib(attrib) {
		return glfw.GetWindowAttrib(this._window, attrib);
	}
	
	
	setInputMode(mode) {
		return glfw.SetInputMode(this._window, mode);
	}
	
	
	swapBuffers() {
		return glfw.SwapBuffers(this._window);
	}
	
	
	makeCurrent() {
		glfw.MakeContextCurrent(this._window);
	}
	
	
	destroy() {
		glfw.DestroyWindow(this._window);
	}
	
	
	iconify() {
		glfw.IconifyWindow(this._window);
	}
	
	
	restore() {
		glfw.RestoreWindow(this._window);
	}
	
	
	hide() {
		glfw.HideWindow(this._window);
	}
	
	
	show() {
		glfw.ShowWindow(this._window);
	}
	
}


module.exports = Window;
