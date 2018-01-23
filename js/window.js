'use strict';

const EventEmitter = require('events');

const glfw = require('../core');


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
		
		this._display = opts.display;
		this._vsync = opts.vsync ? 1 : 0; // 0 for vsync off
		
		this._fullscreen = opts.fullscreen ? true : false;
		this._decorated = opts.decorated === false ? false : true;
		this._msaa = opts.msaa === undefined ? 2 : opts.msaa;
		
		const attribs = this._fullscreen ? glfw.WINDOW : glfw.FULLSCREEN;
		
		// we use OpenGL 2.1, GLSL 1.20. Comment this for now as this is for GLSL 1.50
		//glfw.openWindowHint(glfw.OPENGL_FORWARD_COMPAT, 1);
		//glfw.openWindowHint(glfw.OPENGL_VERSION_MAJOR, 3);
		//glfw.openWindowHint(glfw.OPENGL_VERSION_MINOR, 2);
		//glfw.openWindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE);
		glfw.windowHint(glfw.RESIZABLE, 1);
		glfw.windowHint(glfw.VISIBLE, 1);
		glfw.windowHint(glfw.DECORATED, 1);
		glfw.windowHint(glfw.RED_BITS, 8);
		glfw.windowHint(glfw.GREEN_BITS, 8);
		glfw.windowHint(glfw.BLUE_BITS, 8);
		glfw.windowHint(glfw.DEPTH_BITS, 24);
		glfw.windowHint(glfw.REFRESH_RATE, 0);
		glfw.windowHint(glfw.DOUBLEBUFFER, glfw.TRUE);
		glfw.windowHint(glfw.DECORATED, this._decorated ? glfw.TRUE : glfw.FALSE);
		glfw.windowHint(glfw.SAMPLES, this._msaa);
		
		
		const emitter = { emit: (t, e) => this.emit(t, e) };
		
		if (this._display !== undefined) {
			this._window = glfw.createWindow(this._width, this._height, emitter, this._title, this._display);
		} else {
			this._window = glfw.createWindow(this._width, this._height, emitter, this._title);
		}
		
		if ( ! this._window ) {
			throw new Error('Failed to open GLFW window');
		}
		
		//can only be called after window creation!
		this._major = glfw.getWindowAttrib(this._window, glfw.CONTEXT_VERSION_MAJOR);
		this._minor = glfw.getWindowAttrib(this._window, glfw.CONTEXT_VERSION_MINOR);
		this._rev   = glfw.getWindowAttrib(this._window, glfw.CONTEXT_REVISION);
		this._prof  = glfw.getWindowAttrib(this._window, glfw.OPENGL_PROFILE);
		
		// Vertical sync (on cards that support it)
		glfw.swapInterval(this._vsync);
		
		this.on('window_pos', ({ x, y }) => {
			this._x = x;
			this._y = y;
		});
		this.on('resize', ({ width, height }) => {
			this._width = width;
			this._height = height;
		});
		
	}
	
	
	get handle() { return this._window; }
	
	get width() { return this._width; }
	get height() { return this._height; }

	set width(w)  { this._width = w; }
	set height(h) { this._height = h; }
	
	get w() { return this.width; }
	get h() { return this.height; }
	get wh() { return [this.width, this.height]; }
	
	get size() {
		const size = glfw.getWindowSize(this._window);
		this._width = size.width;
		this._height = size.height;
		return size;
	}
	set size({ width, height }) {
		this._width = width;
		this._height = height;
		glfw.setWindowSize(this._window, width, height);
	}
	
	get title() { return this._title; }
	set title(v) {
		this._title = v || 'Untitled';
		glfw.setWindowTitle(this._window, this._title);
	}
	
	get msaa() { return this._msaa; }
	
	get version() {
		return `GL ${this._major}.${this._minor}.${this._rev} Profile: ${this._prof}`;
	}
	
	get shouldClose() { return glfw.windowShouldClose(this._window); }
	set shouldClose(v) { glfw.setWindowShouldClose(v); }
	
	get platformWindow() { return glfw.platformWindow(this._window); }
	get platformContext() { return glfw.platformContext(this._window); }
	
	get pos() {
		const pos = glfw.getWindowPos(this._window);
		this._x = pos.x;
		this._y = pos.y;
		return pos;
	}
	set pos({ x, y }) {
		this._x = x;
		this._y = y;
		glfw.setWindowSize(this._window, x, y);
	}
	
	get framebufferSize() { return glfw.getFramebufferSize(this._window); }
	
	get currentContext() { return glfw.getCurrentContext(this._window); }
	
	get cursorPos() { return glfw.getCursorPos(this._window); }
	set cursorPos({ x, y }) { glfw.setCursorPos(this._window, x, y); }
	
	
	getKey(key) { return glfw.getKey(this._window, key); }
	
	getMouseButton(button) { return glfw.getMouseButton(this._window, button); }
	
	getWindowAttrib(attrib) { return glfw.getWindowAttrib(this._window, attrib); }
	
	setInputMode(mode) { glfw.setInputMode(this._window, mode); }
	
	swapBuffers() { glfw.swapBuffers(this._window); }
	
	makeCurrent() { glfw.makeContextCurrent(this._window); }
	
	destroy() { glfw.destroyWindow(this._window); }
	
	iconify() { glfw.iconifyWindow(this._window); }
	
	restore() { glfw.restoreWindow(this._window); }
	
	hide() { glfw.hideWindow(this._window); }
	
	show() { glfw.showWindow(this._window); }
	
	
	emit(type, event) {
		
		if (event) {
			event.preventDefault  = () => {};
			event.stopPropagation = () => {};
		}
		
		super.emit(type, event);
		
	}
	
}


module.exports = Window;
