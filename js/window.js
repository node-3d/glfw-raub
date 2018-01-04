'use strict';

const EventEmitter = require('events');

const glfw = require('../binary/glfw');


class Window extends EventEmitter {
	
	get width() { return core.canvas.width; }
	get height() { return core.canvas.height; }
	
	get w() { return this.width; }
	get h() { return this.height; }
	
	get size() { return [this.width, this.height]; }
	
	
	get title()  { return this._title; }
	set title(v) {
		this._title = v || 'Untitled';
		glfw.SetWindowTitle(this._window, this._title);
	}
	
	
	get version() {
		return `GL ${this._major}.${this._minor}.${this._rev} Profile: ${this._prof}`;
	}
	
	
	get shouldClose() {
		return glfw.WindowShouldClose(this._window);
	}
	
	
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
	
	
	makeCurrent() {
		glfw.MakeContextCurrent(this._window);
	}
	
}


module.exports = Window;
