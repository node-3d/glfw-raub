'use strict';

const EventEmitter = require('events');

const glfw = require('../core');


class Window extends EventEmitter {
	
	constructor(opts = {}) {
		
		super();
		
		this._title = opts.title;
		this._icon = null;
		this._modeCache = {};
		
		if ( ! this._title ) {
			const pathMatch2 = process.mainModule.filename.replace(/\\/g, '/').match(
				/(\/(.*))*\/(.*?)\/[^/]*$/
			);
			this._title = pathMatch2 ? pathMatch2[pathMatch2.length - 1] : 'Untitled';
		}
		
		this._width = opts.width || 800;
		this._height = opts.height || 600;
		
		this._display = opts.display;
		this._monitors = glfw.getMonitors();
		this._primaryDisplay = this._monitors.filter(d => d.is_primary)[0];
		
		this._vsync = opts.vsync ? 1 : 0; // 0 for vsync off
		this._autoIconify = opts.autoIconify === false ? false : true;
		
		const mode = opts.fullscreen ? 'fullscreen' : (opts.mode ? opts.mode : 'windowed');
		
		this._decorated = mode === 'windowed';
		if (opts.decorated !== undefined) {
			this._decorated = opts.decorated;
		}
		
		this._msaa = opts.msaa === undefined ? 2 : opts.msaa;
		
		// we use OpenGL 2.1, GLSL 1.20. Comment this for now as this is for GLSL 1.50
		//glfw.windowHint(glfw.OPENGL_FORWARD_COMPAT, 1);
		//glfw.windowHint(glfw.OPENGL_VERSION_MAJOR, 3);
		//glfw.windowHint(glfw.OPENGL_VERSION_MINOR, 2);
		//glfw.windowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE);
		glfw.windowHint(glfw.RESIZABLE, 1);
		glfw.windowHint(glfw.VISIBLE, 1);
		glfw.windowHint(glfw.DECORATED, 1);
		glfw.windowHint(glfw.RED_BITS, 8);
		glfw.windowHint(glfw.GREEN_BITS, 8);
		glfw.windowHint(glfw.BLUE_BITS, 8);
		glfw.windowHint(glfw.DEPTH_BITS, 24);
		glfw.windowHint(glfw.REFRESH_RATE, 0);
		glfw.windowHint(glfw.DOUBLEBUFFER, glfw.TRUE);
		glfw.windowHint(glfw.AUTO_ICONIFY, this._autoIconify ? glfw.TRUE : glfw.FALSE);
		glfw.windowHint(glfw.DECORATED, this._decorated ? glfw.TRUE : glfw.FALSE);
		glfw.windowHint(glfw.SAMPLES, this._msaa);
		
		
		this._emitter = { emit: (t, e) => this.emit(t, e) };
		
		
		// This CREATES window, as mode switches from `undefined`
		this.mode = mode;
		
		
		this.icon = opts.icon;
		
		//can only be called after window creation!
		this._major = glfw.getWindowAttrib(this._window, glfw.CONTEXT_VERSION_MAJOR);
		this._minor = glfw.getWindowAttrib(this._window, glfw.CONTEXT_VERSION_MINOR);
		this._rev = glfw.getWindowAttrib(this._window, glfw.CONTEXT_REVISION);
		this._prof = glfw.getWindowAttrib(this._window, glfw.OPENGL_PROFILE);
		
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
	
	
	get mode() { return this._mode; }
	set mode(v) {
		
		if (this._mode === v) {
			return;
		}
		const prevMode = this._mode;
		this._mode = v;
		
		if (this._window) {
			
			// Fullscreen can't be hidden (uh-oh)
			if (prevMode === 'fullscreen') {
				this.destroy();
				this._modeCache[prevMode] = null;
			} else {
				this.hide();
			}
			
		}
		
		if (this._monitors.length < 1) {
			throw new Error('No suitable display found for a new GLFW Window.');
		}
		
		const isBadId = this._display === undefined || this._display >= this._monitors.length;
		const dispId = isBadId ? -1 : this._display;
		const currentScreen = dispId > -1 ? this._monitors[dispId] : this._primaryDisplay;
		
		this._display = this._monitors.indexOf(currentScreen);
		
		if (this._mode === 'windowed') {
			
			this._width = this._prevWidth || this._width;
			this._height = this._prevHeight || this._height;
			this._decorated = this._prevDecorated || this._decorated;
			delete this._prevWidth;
			delete this._prevHeight;
			delete this._prevDecorated;
			
		} else {
			
			if (
				this._width !== currentScreen.width ||
				this._height !== currentScreen.height
			) {
				this._prevWidth = this._width;
				this._prevHeight = this._height;
				this._width = currentScreen.width;
				this._height = currentScreen.height;
			}
			
		}
		
		if ( ! this._modeCache[this._mode] ) {
			
			this._create();
			this._modeCache[this._mode] = this._window;
			
		} else {
			
			this._window = this._modeCache[this._mode];
			this.show();
			
		}
		
		this.makeCurrent();
		
		this.emit('resize', { width: this._width, height: this._height });
		
	}
	
	
	get width() { return this._width; }
	get height() { return this._height; }
	
	set width(v) {
		if (this._width === v) {
			return;
		}
		this._width = v;
		glfw.setWindowSize(this._window, this._width, this._height);
	}
	set height(v) {
		if (this._height === v) {
			return;
		}
		this._height = v;
		glfw.setWindowSize(this._window, this._width, this._height);
	}
	
	get w() { return this.width; }
	set w(v) { this.width = v; }
	get h() { return this.height; }
	set h(v) { this.height = v; }
	get wh() { return [this.width, this.height]; }
	set wh([width, height]) { this.size = { width, height }; }
	
	
	get size() {
		const size = glfw.getWindowSize(this._window);
		this._width = size.width;
		this._height = size.height;
		return size;
	}
	
	set size({ width, height }) {
		if (this._width === width && this._height === height) {
			return;
		}
		this._width = width;
		this._height = height;
		glfw.setWindowSize(this._window, width, height);
	}
	
	
	get title() { return this._title; }
	set title(v) {
		this._title = v || 'Untitled';
		glfw.setWindowTitle(this._window, this._title);
	}
	
	
	get icon() { return this._icon; }
	set icon(v) {
		if ( ! (v && typeof v === 'object') ) {
			this._icon = null;
			return;
		}
		this._icon = v;
		glfw.setWindowIcon(this._window, this._icon);
	}
	
	
	get msaa() { return this._msaa; }
	
	get version() {
		return `GL ${this._major}.${this._minor}.${this._rev} Profile: ${this._prof}`;
	}
	
	get shouldClose() { return glfw.windowShouldClose(this._window); }
	set shouldClose(v) { glfw.setWindowShouldClose(v); }
	
	get platformWindow() { return glfw.platformWindow(this._window); }
	get platformContext() { return glfw.platformContext(this._window); }
	
	
	set x(v) {
		if (this._x === v) {
			return;
		}
		this._x = v;
		glfw.setWindowSize(this._window, this._x, this._y);
	}
	set y(v) {
		if (this._y === v) {
			return;
		}
		this._y = v;
		glfw.setWindowSize(this._window, this._x, this._y);
	}
	
	get pos() {
		const pos = glfw.getWindowPos(this._window);
		this._x = pos.x;
		this._y = pos.y;
		return pos;
	}
	set pos({ x, y }) {
		if (this._x === x && this._y === y) {
			return;
		}
		this._x = x;
		this._y = y;
		glfw.setWindowPos(this._window, x, y);
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
			event.preventDefault = () => {};
			event.stopPropagation = () => {};
		}
		
		super.emit(type, event);
		
	}
	
	
	// Create a new window according to the current 'mode'
	_create() {
		
		if (this._mode === 'windowed') {
			
			glfw.windowHint(glfw.DECORATED, this._decorated ? glfw.TRUE : glfw.FALSE);
			this._window = glfw.createWindow(this._width, this._height, this._emitter, this._title);
			
		} else if (this._mode === 'borderless') {
			
			this._prevDecorated = this._decorated;
			this._decorated = false;
			
			glfw.windowHint(glfw.DECORATED, glfw.FALSE);
			
			this._window = glfw.createWindow(this._width, this._height, this._emitter, this._title);
			
			glfw.setWindowPos(this._window, 0, 0);
			
		} else if (this._mode === 'fullscreen') {
			
			this._adjustFullscreen();
			
			this._window = glfw.createWindow(
				this._width, this._height, this._emitter, this._title, this._display
			);
			
		} else {
			
			throw new Error(`Not supported display mode: '${this._mode}'.`);
			
		}
		
		if ( ! this._window ) {
			throw new Error('Failed to open a new GLFW Window');
		}
		
	}
	
	
	// ----- Fullscreen mode helpers
	
	_areaDiff(mode) {
		return Math.abs(mode.width * mode.height - this._width * this._height);
	}
	
	
	_sizeEqual(mode) {
		return mode.width === this._width && mode.height === this._height;
	}
	
	
	_sortByAreaDiff(modes) {
		const sorted = modes.sort((a, b) => this._areaDiff(a) - this._areaDiff(b));
		const best = this._areaDiff(sorted[0]);
		return sorted.filter(mode => this._areaDiff(mode) === best);
	}
	
	
	_sortByRate(modes) {
		return modes.sort((a, b) => this._areaDiff(a) - this._areaDiff(b));
	}
	
	
	_adjustFullscreen() {
		
		const mode = (() => {
			const modes = this._monitors[this._display].modes;
			const exact = modes.filter(mode => this._sizeEqual(mode));
			const chosen = (exact.length ? exact : this._sortByAreaDiff(modes));
			return chosen.sort((a, b) => b.rate - a.rate)[0];
		})();
		
		this._width = mode.width;
		this._height = mode.height;
		
		glfw.windowHint(glfw.REFRESH_RATE, mode.rate);
		
	}
	
}


module.exports = Window;
